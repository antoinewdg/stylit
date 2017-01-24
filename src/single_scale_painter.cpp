#include "single_scale_painter.h"

void SingleScalePainter::_build_b_from_nnf(const Mat_<Vec2i> &nnf) {
    Mat_<Vec3f> result(m_q.b_rendered.size(), Vec3f(0, 0, 0));
    Mat_<float> weights(m_q.b_rendered.size(), 0.f);
    for_each_patch(result, [this, &nnf, &weights, &result](int i, int j){
        auto q = nnf(i, j);
        for (int k = -P / 2; k <= P / 2; k++) {
            for (int l = -P / 2; l <= P / 2; l++) {
                result(k + i, l + j) = result(k + i, l + j) + Vec3f(m_q.a_drawn(q + Vec2i(k, l)));
                weights(k + i, l + j) += 1.f;
            }
        }
    });

    m_q.b_drawn = Mat_<Vec3b>(result.size());
    for_each_pixel(m_q.b_drawn, [&result, &weights](int i, int j, Vec3b &v) {
        v = result(i, j) / weights(i, j);
    });
}

Mat_<Vec2i> SingleScalePainter::_build_nnf(float inv_mu) {
    OffsetMap offset_map(m_q.b_rendered.size());
    DistanceMap distance_map(m_q.b_rendered.size());
    PatchServer a_patches(m_q.a_rendered.size(), P);
    PatchServer b_patches(m_q.b_rendered.size(), P);
    PatchDistance<false> patch_distance(m_q, inv_mu);

    Matcher matcher(b_patches, a_patches, patch_distance, offset_map, distance_map);
    matcher.initialize_offset_map_randomly();
    matcher.iterate_n_times(5);

    Mat_<Vec2i> nnf(offset_map.to_mat().size());
    for (auto p : b_patches) {
        nnf(p) = offset_map(p) + p;
    }

    return nnf;
//    RestrictedNNF rn(m_q, inv_mu, m_logger);
//    return rn.build_nnf();

}