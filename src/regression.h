//
// Created by antoinewdg on 12/21/16.
//

#ifndef STYLIT_REGRESSION_H
#define STYLIT_REGRESSION_H

#include <algorithm>
#include <numeric>
#include <utility>

#include "utils.h"

template<class XIterator, class YIterator>
std::pair<float, float> linear_regression(XIterator x_begin, XIterator x_end,
                                          YIterator y_begin, YIterator y_end) {
    long n = std::distance(x_begin, x_end);
    float x_mean = std::accumulate(x_begin, x_end, 0.f) / n;
    float y_mean = std::accumulate(y_begin, y_end, 0.f) / n;

    float var_x = 0.f, cov_xy = 0.f;
    YIterator y_it = y_begin;
    for (XIterator x_it = x_begin; x_it != x_end; x_it++) {
        var_x += (*x_it) * (*x_it);
        cov_xy += (*x_it) * (*y_it);
        y_it++;
    }

    var_x = (var_x / n) - (x_mean * x_mean);
    cov_xy = (cov_xy / n) - (x_mean * y_mean);

    float beta = cov_xy / var_x;
    float alpha = y_mean - beta * x_mean;

    return std::make_pair(beta, alpha);
};

#endif //STYLIT_REGRESSION_H
