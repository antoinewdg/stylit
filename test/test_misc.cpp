#include "catch.hpp"

#include "misc.h"

TEST_CASE("add_noise", "[unit]") {
    Mat_<Vec3b> img_b = load_color("../files/sphere/rendered.png");
    Mat_<Vec3f> img;
    img_b.convertTo(img, CV_32FC3, 1.f / 255);
    Mat_<Vec3f> noisy = img.clone();
    float sigma = 0.025f;
    std::default_random_engine generator(52);
    add_noise(noisy, sigma, generator);

    float sigma_sqr_hat = cv::norm(noisy - img, cv::NORM_L2SQR) / (3 * img.rows * img.cols);
    float err = sigma_sqr_hat - (sigma * sigma);
    REQUIRE(err < 1e-6);
}