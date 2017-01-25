//
// Created by antoinewdg on 12/2/16.
//

#ifndef STYLIT_PATCH_DISTANCE_H
#define STYLIT_PATCH_DISTANCE_H

#include <boost/hana/if.hpp>

#include "quadruplet.h"

template<bool ASource>
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

        namespace hana = boost::hana;
        output_type d(0.f);

        for (int i = -P / 2; i <= P / 2; i++) {
            for (int j = -P / 2; j <= P / 2; j++) {
                Vec2i u = p + Vec2i(i, j);
                Vec2i v = q + Vec2i(i, j);

                if (m_inv_mu > 0) {
                    for (int c = 0; c < 3; c++) {
                        float temp = hana::if_(ASource,
                                               m_q.a_drawn(u)[c] - m_q.b_drawn(v)[c],
                                               m_q.a_drawn(v)[c] - m_q.b_drawn(u)[c]);
                        d += temp * temp * m_inv_mu;
                    }


                }

//                std::cout << "v:" << v << " u:" << u << std::endl;
                for (int c = 0; c < 15; c++) {
//                    std::cout << c << std::endl;
//                    cout << m_q.a_rendered.size() << endl;
//                    std::cout << m_q.a_rendered(v) << endl;
//                    cout << m_q.b_rendered(u) << endl;
//                    float temp = m_q.a_rendered(v)[c] - m_q.b_rendered(u)[c];
                    float temp = hana::if_(ASource,
                                           m_q.a_rendered(u)[c] - m_q.b_rendered(v)[c],
                                           m_q.a_rendered(v)[c] - m_q.b_rendered(u)[c]);
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

    Mat_<float> _compute_patch_variance(Mat_<Vec3b> m) {
        Mat_<float> variance(m.size(), 0);
        for (int i = P / 2; i < m.rows - 2; i++) {
            for (int j = 2; j < m.cols - 2; j++) {
                Rect r(j - 2, i - 2, P, P);
                cv::Scalar mean = cv::mean(m(r));
                variance(i, j) = float(cv::norm(m(r) - mean, cv::NORM_L2SQR));
            }
        }
        return variance;
    }

    const Quadruplet &m_q;
    float m_inv_mu;
};

#endif //STYLIT_PATCH_DISTANCE_H
