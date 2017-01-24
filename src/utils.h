//
// Created by antoinewdg on 11/26/16.
//

#ifndef STYLIT_UTIL_H
#define STYLIT_UTIL_H

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <array>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cv_utils/cv_utils.h"

using namespace cvu;

using std::vector;

typedef cv::Vec<uchar, 15> Vec15b;

template<typename TimeT = std::chrono::milliseconds>
struct measure {
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F &&func, Args &&... args) {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast<TimeT>
                (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};


inline string _file_root() {
    return "../files/";
}

template<class T, class Func>
inline void for_each_patch(Mat_<T> &m, Func f) {
    for (int i = 2; i < m.rows - 2; i++) {
        for (int j = 2; j < m.cols - 2; j++) {
            f(i, j);
        }
    }
};


#endif //STYLIT_UTIL_H
