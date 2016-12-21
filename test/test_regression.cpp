#include "catch.hpp"

#include "regression.h"

TEST_CASE("Linear regression on exact case", "[unit]") {
    vector<float> x, y;
    for (int i = 0; i < 100; i++) {
        x.push_back(float(i));
        y.push_back(3.22f * i - 99.54f);
    }

    auto p = linear_regression(x.begin(), x.end(), y.begin(), y.end());
    REQUIRE(p.first == Approx(3.22f));
    REQUIRE(p.second == Approx(-99.54f));
}