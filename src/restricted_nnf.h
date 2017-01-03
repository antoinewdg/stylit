//
// Created by antoinewdg on 12/23/16.
//

#ifndef STYLIT_RESTRICTED_NNF_H
#define STYLIT_RESTRICTED_NNF_H

#include <fstream>

#include <boost/iterator/counting_iterator.hpp>

#include "patch_match_core/patch_matcher.h"
#include "patch_match_opencv/adapters.h"

#include "quadruplet.h"
#include "patch_distance.h"
#include "patch_server.h"
#include "regression.h"


class RestrictedNNF {
public:
    typedef pm::opencv::OffsetMap2D OffsetMap;
    typedef pm::opencv::DistanceMap2d<float> DistanceMap;
    typedef pm::opencv::WholeImagePatches APatchServer;
    typedef RestrictedPatchServer BPatchServer;
    typedef PatchDistance<true> ReverseDistance;
    typedef PatchDistance<false> RegularDistance;

    typedef pm::core::PatchMatcher<
            APatchServer, BPatchServer, ReverseDistance,
            OffsetMap, DistanceMap
    > ReverseMatcher;

    typedef pm::core::PatchMatcher<
            BPatchServer, APatchServer, RegularDistance,
            OffsetMap, DistanceMap
    > RegularMatcher;

    static constexpr int P = 5;

    RestrictedNNF(Quadruplet &q, float inv_mu) :
            m_q(q),
            m_nnf(q.b_rendered.size()),
            m_distance_map(q.b_rendered.size(), std::numeric_limits<float>::max()),
            m_b_patches_mask(q.b_rendered.size(), true),
            m_fixed_patches(0),
            m_reverse_distance(q, inv_mu),
            m_regular_distance(q, inv_mu) {

    }

    Mat_<Vec2i> build_nnf() {
        unsigned long n_patches = (m_q.b_rendered.rows - P + 1) * (m_q.b_rendered.cols - P + 1);
        unsigned long max_fixed_patches((n_patches * 85) / 100);
        while (m_fixed_patches < max_fixed_patches) {
            BPatchServer b_patches(m_b_patches_mask, P);
            auto p = _build_reverse_nnf(b_patches);
            _choose_fixed_patches(b_patches, p.first.to_mat(), p.second.to_mat());
        }
//
        _build_final_nnf();

        return m_nnf;

    }

    std::pair<OffsetMap, DistanceMap> _build_reverse_nnf(const BPatchServer &b_patches) {
        APatchServer a_patches(m_q.a_rendered.size(), P);
        OffsetMap offset_map(m_q.a_rendered.size());
        DistanceMap distance_map(m_q.a_rendered.size());
        ReverseMatcher matcher(a_patches, b_patches, m_reverse_distance, offset_map, distance_map);
        matcher.initialize_offset_map_randomly();
        matcher.iterate_n_times(5);

        return std::make_pair(offset_map, distance_map);
    }

    void _choose_fixed_patches(const BPatchServer &b_patches, Mat_<Vec2i> offset_map, Mat_<float> distance_map) {
        float max_d = _find_distance_limit(distance_map);

        for (int i = P / 2; i < distance_map.rows - P / 2; i++) {
            for (int j = P / 2; j < distance_map.cols - P / 2; j++) {
                Vec2i p(i, j);
                Vec2i q = offset_map(p) + p;
                if (distance_map(p) < m_distance_map(q)) {
                    m_distance_map(q) = distance_map(p);
                    m_nnf(q) = p;
                    m_b_patches_mask(q) = false;
                }
            }
        }

        for (const Vec2i &q : b_patches) {
            if (m_b_patches_mask(q) == false) {
                m_fixed_patches++;
            }
        }
    }

    float _find_distance_limit(Mat_<float> d) {

        d = d(cv::Rect(P / 2, P / 2, d.cols - P + 1, d.rows - P + 1));
        vector<float> distances(d.cols * d.rows);
        for (float a : d) {
            distances.push_back(a);
        }

        auto max_it = distances.begin() + int(0.5f * distances.size());
        std::nth_element(distances.begin(), max_it, distances.end());

        return *max_it;
    }

    void _build_final_nnf() {

        APatchServer a_patches(m_q.a_rendered.size(), P);
        BPatchServer b_patches(m_b_patches_mask, P);
        OffsetMap offset_map(m_q.b_rendered.size());
        DistanceMap distance_map(m_q.b_rendered.size());
        RegularMatcher matcher(b_patches, a_patches, m_regular_distance, offset_map, distance_map);
        matcher.initialize_offset_map_randomly();
        matcher.iterate_n_times(5);

        for (const Vec2i &q : b_patches) {
            m_nnf(q) = offset_map(q) + q;
        }
    }

private:

    ReverseDistance m_reverse_distance;
    RegularDistance m_regular_distance;
    Mat_<bool> m_b_patches_mask;
    Mat_<Vec2i> m_nnf;
    Mat_<float> m_distance_map;
    Quadruplet &m_q;
    unsigned long m_fixed_patches;
};

#endif //STYLIT_RESTRICTED_NNF_H
