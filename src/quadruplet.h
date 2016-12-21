//
// Created by antoinewdg on 12/2/16.
//

#ifndef STYLIT_STRUCTS_H
#define STYLIT_STRUCTS_H

#include "utils.h"

constexpr int P = 5;

class Quadruplet {
public:
    Mat_<Vec15b> a_rendered, b_rendered;
    Mat_<Vec3b> a_drawn, b_drawn;

    static Quadruplet from_files(string style_name, string target_name);

    vector<Quadruplet> get_gaussian_pyramid();

private:
    static Mat_<Vec3b> _load_full_render(string name);

    template<class T>
    static vector<Mat_<T>> _compute_gaussian_pyramid(Mat_<T> m) {
        vector<Mat_<T>> pyramid = {m};
        while (std::min(pyramid.back().rows, pyramid.back().cols) >= 2 * P) {
            pyramid.emplace_back();
            cv::pyrDown(pyramid[pyramid.size() - 2], pyramid.back());
        }

        return pyramid;
    }

};

#endif //STYLIT_STRUCTS_H
