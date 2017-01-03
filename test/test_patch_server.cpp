#include "catch.hpp"

#include "patch_server.h"
#include "utils.h"

TEST_CASE("Patch server", "[unit]") {
    Mat_<bool> mask(7, 6, false);
    vector<Vec2i> patches = {Vec2i(1, 3), Vec2i(1, 4),
                             Vec2i(2, 2), Vec2i(2, 3),
                             Vec2i(4, 2)};
    for (Vec2i p : patches) {
        mask(p) = true;
    }
    RestrictedPatchServer server(mask, 3);

    SECTION("iter") {
        auto it = patches.begin();
        for (Vec2i p : server) {
            if (p != *it) {
                FAIL("Error in iteration: " << p << " instead of " << *it);
            }
            it++;
        }
        REQUIRE(patches.size() == std::distance(server.begin(), server.end()));
    }

    SECTION("iter reverse") {
        auto it = patches.rbegin();
        for (auto jt = server.rbegin(); jt != server.rend(); jt++) {
            if (*jt != *it) {
                FAIL("Error in iteration: " << *jt << " instead of " << *it);
            }
            it++;
        }
        REQUIRE(patches.size() == std::distance(server.rbegin(), server.rend()));
    }
}