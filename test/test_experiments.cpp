#include <boost/filesystem.hpp>
#include "catch.hpp"

#include "multi_scale_painter.h"

vector<string> targets = {
        "helmet", "shark", "dino",
        "donut", "crow", "giant"
};

vector<string> styles = {
        "victor", "fragments", "orange_aqua"
};

TEST_CASE("Experiments") {
    for (auto t : targets) {
        for (auto s : styles) {
            Quadruplet q = Quadruplet::from_files(s, t);
            Logger logger(s, t);
            MultiScalePainter painter(q, logger);
            painter.paint();
        }
    }
}