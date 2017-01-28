//
// Created by antoinewdg on 12/2/16.
//

#ifndef STYLIT_SINGLE_SCALE_PAINTER_H
#define STYLIT_SINGLE_SCALE_PAINTER_H

#include <fstream>

#include "logger.h"
#include "quadruplet.h"
#include "patch_distance.h"
#include "restricted_nnf.h"
#include "misc.h"
#include "uniform_nnf.h"

#include "patch_match_core/patch_matcher.h"
#include "patch_match_opencv/adapters.h"

class SingleScalePainter {
public:


    typedef pm::opencv::WholeImagePatches PatchServer;
    typedef pm::opencv::OffsetMap2D OffsetMap;
    typedef pm::opencv::DistanceMap2d<float> DistanceMap;
    typedef pm::core::PatchMatcher<
            PatchServer, PatchServer, PatchDistance<false>,
            OffsetMap, DistanceMap
    > Matcher;

    SingleScalePainter(Quadruplet &q, Logger &logger) : m_q(q), m_logger(logger) {}


    void iterate(vector<float> inv_mus) {
        for (float inv_mu : inv_mus) {
            Mat_<Vec2i> nnf;
            Mat_<float> distance_map;
            std::tie(nnf, distance_map) = _build_nnf(inv_mu);
            _build_b_from_nnf(nnf, distance_map);
//            m_logger.log_image(build_wrong_b(nnf));
            m_logger.log_distance_map(distance_map);
            for (int k = 0; k <= 4; k++) {
                for (int l = 0; l <= 4; l++) {
                    int n = k * 5 + l;
                    m_logger.log_image_bad(build_wrong_b(nnf, k, l), n);
                }
            }
            m_logger.next_iter();
        }
    }


private:

    std::tuple<Mat_<Vec2i>, Mat_<float>> _build_nnf(float inv_mu = 0);

    void _build_b_from_nnf(const Mat_<Vec2i> &nnf, const Mat_<float> &distances);

    Mat_<Vec3b> build_wrong_b(const Mat_<Vec2i> &nnf, int a, int b);


    Quadruplet &m_q;
    Logger &m_logger;
};

#endif //STYLIT_SINGLE_SCALE_PAINTER_H
