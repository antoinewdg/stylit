#include <ctime>

#include <boost/filesystem.hpp>
#include "catch.hpp"

#include "multi_scale_painter.h"
#include "restricted_nnf.h"
#include "misc.h"

vector<string> targets = {
        "helmet", "shark", "dino",
        "donut", "crow", "giant"
};

vector<string> styles = {
        "dreamy", "paint_noob", "fragments", "victor", "orange_aqua"
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