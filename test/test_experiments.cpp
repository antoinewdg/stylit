#include <ctime>

#include <boost/filesystem.hpp>
#include "catch.hpp"

#include "multi_scale_painter.h"
#include "restricted_nnf.h"
#include "misc.h"

vector<string> targets = {
//        "train", "hat", "duck"
        "helmet", "shark", "dino",
//        "donut", "crow", "giant"
};

vector<string> styles = {
        "dreamy", "orange_aqua", "comics", "pen_gray", "paint_noob",
        "fragments", "victor",
};

typedef std::pair<string, string> SPair;

vector<SPair> todo = {
        SPair("crow","comics"),
        SPair("dino", "red_green"),
        SPair("shark", "fragments")
};

TEST_CASE("Experiments", "[exp]") {
    time_t a = time(0);
//    for (auto t : targets) {
//        for (auto s : styles) {
//            Quadruplet q = Quadruplet::from_files(s, t);
//            Logger logger(s, t, a);
//            MultiScalePainter painter(q, logger);
//            painter.paint();
//        }
//    }

    for (auto p : todo) {
        Quadruplet q = Quadruplet::from_files(p.second, p.first);
        Logger logger(p.second, p.first, a);
        MultiScalePainter painter(q, logger);
        painter.paint();
    }
}

TEST_CASE("all_patches", "[exp]") {
    vector<Mat_<Vec3b>> images;
    for (int i = 0; i < 25; i++) {
        string n = std::to_string(i);
        if (i < 10) {
            n = "0" + n;
        }
        images.push_back(load_color("../files/all_patches/4_02_" + n + ".png"));

    }

    Mat_<float> distances = load_grayscale<float>("../files/all_patches/distances_4_02.png");
//    cout << distances << endl;
//    display_blocking(Mat_<bool>(distances <= 0.5));


    Mat_<Vec3b> out(images.back().size());
//    for_each_patch<P>(out, [&out, &distances, &images](int i, int j) {
//        if (i % P != 2 || j % P != 2 || distances(i, j) > 0.1f) {
//            return;
//        }
//        for (int k = -2; k <= 2; k++) {
//            for (int l = -2; l <= 2; l++) {
////                int n = (k + 2) * 5 + l + 2;
//                out(i + k, j + l) = images[0](i + k, j + l);
//            }
//        }
//    });

    typedef cv::Vec<float, 24> Vec24f;
    auto to_vec3f = [](Vec3b v, float w) {
        return Vec3f(w * v[0], w * v[1], w * v[2]);
    };

    auto to_vec3b = [](Vec3f v) {
        Vec3b b;
        for (int c = 0; c < 3; c++)
            b[c] = std::round(v[c]);

        return b;
    };



//    Mat_<Vec24f> distances_full(out.size());

    for (int g = 1; g < 10000000000; g *= 10) {
        float sigma2 = 1.f / g;
        for_each_patch<P>(out, [&](int i, int j) {
//        vector<float> d;
//        for (int m = 0; m < 24; m++) {
//            int k = (m / 5), l = (m % 5);
//            int a = ((i + k - 2) / P) * P + 2, b = ((k + l - 2) / P) * P + 2;
//            d.emplace_back(distances(a, b));
//        }
//        auto it = std::min_element(d.begin(), d.end());
//        auto m = std::distance(d.begin(), it);
//        out(i, j) = images[m](i, j)
//


            Vec3f total(0, 0, 0);
            float weights = 0.f;
            for (int k = -2; k <= 2; k++) {
                for (int l = -2; l <= 2; l++) {
                    int m = 5 * ((i + k) % 5) + ((j + l) % 5);
//                int a = (i + k) % 5 - 2, b = (j+k) % 5 - 2;
                    float w = std::max(std::exp(-distances(i + k, j + l) / (2 * sigma2)), 0.0001f);
                    total = total + to_vec3f(images[m](i, j), w);
                    weights += w;
                }
            }

            out(i, j) = to_vec3b(total / weights);
//        for (int m = 0; m < 24; m++) {
//            int k = m / 5, l = m % 5;
//
//            cout << "i,j: " << i << "," << j << "  m=" << m << " a,b:" << a << "," << b << endl;
//            out(i, j) = out(i, j) + (images[m](i, j) / 24);
//        }


        });

//    std::uniform_int_distribution<int> dist(0, 24);
//    for_each_pixel(out, [&dist, &images, &distances](int i, int j, Vec3b &o) {
//        vector<float> d;
//        for (int k = -2; k <= 2; k++) {
//            for (int l = -2; l <= 2; l++) {
//                d.push_back(float(distances(i + k, j + l)));
//            }
//        }
//        auto it = std::min_element(d.begin(), d.end());
//        auto k = std::distance(d.begin(), it);
//        o = images[k](i, j);
//    });z

//    for_ea_patch(out, [&dist, &images, &distances](int i, int j) {
//        ch
//    });
//
//    cv::medianBlur(out, out, 5);
        display_blocking(out);
    }

}