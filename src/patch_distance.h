//
// Created by antoinewdg on 12/2/16.
//

#ifndef STYLIT_PATCH_DISTANCE_H
#define STYLIT_PATCH_DISTANCE_H

#include "quadruplet.h"

class PatchDistance {
public:
    typedef Vec2i left_type;
    typedef Vec2i right_type;
    typedef float output_type;

    PatchDistance(Quadruplet &q, float inv_mu) : m_q(q), m_inv_mu(inv_mu) {

    }

    inline output_type operator()(const left_type &p, const right_type &q) {
        return operator()(p, q, std::numeric_limits<output_type>::max());
    }

    inline output_type operator()(const left_type &p, const right_type &q, output_type max_d) {
        output_type d(0.f);
        for (int i = -P / 2; i <= P / 2; i++) {
            for (int j = -P / 2; j <= P / 2; j++) {
                Vec2i u = p + Vec2i(i, j);
                Vec2i v = q + Vec2i(i, j);

                if (m_inv_mu > 0) {
                    for (int c = 0; c < 3; c++) {
                        float temp = m_q.a_drawn(v)[c] - m_q.b_drawn(u)[c];
                        d += temp * temp * m_inv_mu;
                    }
                }

                for (int c = 0; c < 15; c++) {
                    float temp = m_q.a_rendered(v)[c] - m_q.b_rendered(u)[c];
                    d += temp * temp;
                }

                if (d >= max_d) {
                    return d;
                }
            }
        }
        return d;
    }

private:


    const Quadruplet &m_q;
    float m_inv_mu;
};

#endif //STYLIT_PATCH_DISTANCE_H
