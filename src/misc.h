//
// Created by antoinewdg on 1/8/17.
//

#ifndef STYLIT_MISC_H
#define STYLIT_MISC_H

#include <random>

#include "utils.h"

class RandomGlobals {
public:
    static std::default_random_engine GENERATOR;
};

template<class T, int C, class RandomEngine>
void add_noise(Mat_<cv::Vec<T, C>> &a, float sigma, RandomEngine &generator) {
    Mat_<cv::Vec<T, C>> noise(a.size());
    std::normal_distribution<float> dist(0.f, sigma);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            for (int c = 0; c < C; c++) {
                float f = dist(generator);
                a(i, j)[c] = cv::saturate_cast<T>(a(i, j)[c] + f);
            }
        }
    }
};

template<class T>
void add_noise(Mat_<T> &a, float sigma) {
    add_noise(a, sigma, RandomGlobals::GENERATOR);
}

#endif //STYLIT_MISC_H
