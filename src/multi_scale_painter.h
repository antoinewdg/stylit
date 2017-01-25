//
// Created by antoinewdg on 12/2/16.
//

#ifndef STYLIT_MULTI_SCALE_PAINTER_H
#define STYLIT_MULTI_SCALE_PAINTER_H

#include "logger.h"
#include "quadruplet.h"
#include "single_scale_painter.h"

class MultiScalePainter {
public:
    MultiScalePainter(Quadruplet &q, Logger &logger) : m_q(q), m_logger(logger) {}

    void paint() {
        auto pyramid = m_q.get_gaussian_pyramid(6);

        for (auto it = pyramid.rbegin(); it != pyramid.rend(); it++) {
            m_logger.log_sphere(it->a_drawn);
            SingleScalePainter painter(*it, m_logger);
            if (it == pyramid.rbegin()) {
                painter.iterate({0, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f});
            } else {
                painter.iterate({0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f});
            }

            auto jt = it + 1;
            if (jt != pyramid.rend()) {
                cv::pyrUp(it->b_drawn, jt->b_drawn, jt->b_rendered.size());
            } else {
                m_q.b_drawn = it->b_drawn;
            }

            m_logger.next_scale();


        }
    }

private:
    Quadruplet &m_q;
    Logger &m_logger;
};

#endif //STYLIT_MULTI_SCALE_PAINTER_H
