//
// Created by antoinewdg on 11/15/16.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "single_scale_painter.h"

TEST_CASE("Random shit") {
//    auto i = load_styled_sphere("orange_aqua");
//    auto pyramid = compute_gaussian_pyramid(i);
//    for (auto im : pyramid) {
//        display_and_block(im);
//    }

    Quadruplet q = Quadruplet::from_files("victor", "helmet");
    display_and_block(q.a_drawn);
    vector<Quadruplet> pyramid = q.get_gaussian_pyramid();
    for (Quadruplet p : pyramid) {
        display_and_block(p.a_drawn);
    }
//    q = pyramid[1];
//    SingleScalePainter painter(q);
//
//    painter.iterate({0, 1});
//    display_and_block(q.a_drawn);
//    display_and_block(q.b_drawn);

//    auto a_rendered_im = load_full_render(string("sphere"));
//    auto b_rendered_im = load_full_render("helmet");
//    auto a_drawn_im = load_color("files/" + string("sphere") + "/" + string("victor") + ".png");
//
//    constexpr int P = 5;
//
//    auto a_rendered = compute_gaussian_pyramid(a_rendered_im, P);
//    auto b_rendered = compute_gaussian_pyramid(b_rendered_im, P);
//    auto a_drawn = compute_gaussian_pyramid(a_drawn_im, P);
//
//    auto max_k = std::min(a_rendered.size(), b_rendered.size()) - 1;
//    Mat_<Vec3b> b_drawn_im;
//    for (int k = int(max_k); k >= 0; k--) {
//        WARN(a_rendered[k].size() << "  " << b_rendered[k].size());
//        Copycat copycat(a_rendered[k], b_rendered[k], a_drawn[k], b_drawn_im);
//        float inv_mu = 0.5f;
//        if (k == max_k) {
//            inv_mu = 0;
//        }
////
//        WARN("ddd");
//        Mat_<Vec3b> temp;
//        for (int n = 0; n < 10; n++) {
//            auto nnf = copycat._build_nnf(inv_mu + n * (0.5f));
//            temp = copycat._build_b_from_nnf(nnf);
//            cv::imwrite("files/helmet/test_8_" + std::to_string(k) + "_" + std::to_string(n) + ".png", temp);
////            display_and_block(temp);
//        }
////        display_and_block(temp);
//        if (k > 0) {
//            cv::pyrUp(temp, b_drawn_im, b_rendered[k - 1].size());
////            display_and_block(b_drawn_im);
//        }
//    }

//    Copycat copycat("orange_aqua", "helmet");
//    auto nnf = copycat._build_nnf();
//    cv::imwrite("files/helmet/test_1.png", copycat._build_b_from_nnf(nnf));
////    display_and_block(copycat.build_b_from_nnf(nnf));
//
//    for (int k = 0; k < 5; k++) {
//        nnf = copycat._build_nnf(0.5 * (k + 1));
//        auto b = copycat._build_b_from_nnf(nnf);
//        cv::imwrite("files/helmet/test_4" + std::to_string(k) + ".png", b);
//    }

}