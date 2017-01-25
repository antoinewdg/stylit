//
// Created by antoinewdg on 1/25/17.
//

#ifndef STYLIT_UNIFORM_NNF_H
#define STYLIT_UNIFORM_NNF_H

#include "patch_match_core/patch_matcher.h"
#include "patch_match_opencv/adapters.h"

#include "quadruplet.h"
#include "patch_distance.h"
#include "patch_server.h"
#include "regression.h"
#include "logger.h"
#include "misc.h"

class UniformNNF {
public:
    typedef pm::opencv::OffsetMap2D OffsetMap;
    typedef pm::opencv::DistanceMap2d<float> DistanceMap;
    typedef RestrictedPatchServer PatchServer;
    typedef pm::core::PatchMatcher<
            PatchServer, PatchServer, PatchDistance<false>,
            OffsetMap, DistanceMap
    > Matcher;

    UniformNNF(Quadruplet &q, float inv_mu, Logger &logger) :
            m_q(q),
            m_nnf(q.b_rendered.size()),
            m_distance_map(q.b_rendered.size()),
            m_b_patches_mask(q.b_rendered.size(), true),
            m_a_patches_mask(q.a_rendered.size(), true),
            m_patch_distance(q, inv_mu),
            m_logger(logger),
            n_fixed_patches(0) {
        n_total_patches = (m_q.b_rendered.rows - P + 1) * (m_q.b_rendered.cols - P + 1);
    }

    void select_patches(const Mat_<Vec2i> &nnf, const Mat_<float> &distance_map) {
        Mat_<float> heat_map = build_heat_map(nnf);
        for_each_patch<P>(m_b_patches_mask, [this, &nnf, &heat_map, &distance_map](int i, int j) {
            if (!m_b_patches_mask(i, j)) {
                return;
            }
            auto q = nnf(i, j);
            float k = heat_map(q);
            float p = std::min(1.f, 10.f / k);

            std::bernoulli_distribution dist(p);
            bool keep = dist(RandomGlobals::GENERATOR);
            if (keep) {
                m_a_patches_mask(q) = false;
                m_b_patches_mask(i, j) = false;
                m_nnf(i, j) = q;
                m_distance_map(i, j) = distance_map(i, j);
                n_fixed_patches++;
            }
        });
    }

    Mat_<float> build_heat_map(const Mat_<Vec2i> &nnf) {
        Mat_<float> heat_map(m_q.a_drawn.size(), 0.f);

        for_each_patch<P>(m_b_patches_mask, [this, &heat_map, &nnf](int i, int j) {
            if (!m_b_patches_mask(i, j)) {
                return;
            }
            auto q = nnf(i, j);
            heat_map(q) += 1;
        });

        return heat_map;
    }

    void perform_step() {
        OffsetMap offset_map(m_q.b_rendered.size());
        DistanceMap distance_map(m_q.b_rendered.size());
        PatchServer a_patches(m_a_patches_mask, P);
        PatchServer b_patches(m_b_patches_mask, P);

        Matcher matcher(b_patches, a_patches, m_patch_distance, offset_map, distance_map);
        matcher.initialize_offset_map_randomly();
        matcher.iterate_n_times(5);

        Mat_<Vec2i> nnf(offset_map.to_mat().size());
        for (auto p : b_patches) {
            nnf(p) = offset_map(p) + p;
        }
        select_patches(nnf, distance_map.to_mat());

    }

    std::tuple<Mat_<Vec2i>, Mat_<float>> build_nnf() {
        while (n_fixed_patches < n_total_patches) {
            perform_step();
        }
        return std::make_tuple(m_nnf, m_distance_map);
    }

private:

    PatchDistance<false> m_patch_distance;
    Mat_<bool> m_a_patches_mask, m_b_patches_mask;
    Mat_<Vec2i> m_nnf;
    Mat_<float> m_distance_map;
    unsigned long n_total_patches, n_fixed_patches;
    Quadruplet &m_q;
    Logger &m_logger;
};

#endif //STYLIT_UNIFORM_NNF_H
