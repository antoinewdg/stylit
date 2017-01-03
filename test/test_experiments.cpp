#include <ctime>

#include <boost/filesystem.hpp>
#include "catch.hpp"

#include "multi_scale_painter.h"
#include "restricted_nnf.h"

vector<string> targets = {
        "helmet", "shark", "dino",
        "donut", "crow", "giant"
};

vector<string> styles = {
        "dreamy", "fragments", "victor", "orange_aqua"
};

TEST_CASE("Experiments", "[exp]") {
    time_t a = time(0);
    for (auto t : targets) {
        for (auto s : styles) {
            Quadruplet q = Quadruplet::from_files(s, t);
            Logger logger(s, t, a);
            MultiScalePainter painter(q, logger);
            painter.paint();
        }
    }
}

//TEST_CASE("Exp bis", "[exp]") {
//    Quadruplet q = Quadruplet::from_files("victor", "crow");
//    auto pyr = q.get_gaussian_pyramid();
//    RestrictedNNF rn(pyr[3], 0.f);
//    rn.build_nnf();
////    rn.perform_step(0.f);
//}