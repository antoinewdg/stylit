//
// Created by antoinewdg on 12/2/16.
//

#ifndef STYLIT_SINGLE_SCALE_PAINTER_H
#define STYLIT_SINGLE_SCALE_PAINTER_H

#include <fstream>

#include "logger.h"
#include "quadruplet.h"
#include "patch_distance.h"

#include "patch_match_core/patch_matcher.h"
#include "patch_match_opencv/adapters.h"

class SingleScalePainter {
public:


    typedef pm::opencv::WholeImagePatches PatchServer;
    typedef pm::opencv::OffsetMap2D OffsetMap;
    typedef pm::opencv::DistanceMap2d<float> DistanceMap;
    typedef pm::core::PatchMatcher<
            PatchServer, PatchServer, PatchDistance,
            OffsetMap, DistanceMap
    > Matcher;

    SingleScalePainter(Quadruplet &q, Logger &logger) : m_q(q), m_logger(logger) {}

    void iterate(vector<float> inv_mus) {
        for (float inv_mu : inv_mus) {
            auto nnf = _build_nnf(inv_mu);
            _build_b_from_nnf(nnf);
            m_logger.log_image(m_q.b_drawn);
            m_logger.next_iter();
        }
    }


private:

    Mat_<Vec2i> _build_nnf(float inv_mu = 0);

    void _build_b_from_nnf(const Mat_<Vec2i> &nnf);


    Quadruplet &m_q;
    Logger &m_logger;
};

#endif //STYLIT_SINGLE_SCALE_PAINTER_H
