//
// Created by antoinewdg on 12/3/16.
//

#ifndef STYLIT_LOGGER_H
#define STYLIT_LOGGER_H

#include <boost/filesystem.hpp>
#include <fstream>
#include "utils.h"

namespace fs = boost::filesystem;

class Logger {
public:
    Logger(string style, string target, time_t t) : scale(0), n_iter(0) {
        root = _file_root() + "out/" + std::to_string(t) + "/" + target + "/" +
               style + "/";

        fs::create_directories(fs::path(root + "distances"));

        cout << endl << endl << "==========================" << endl;
        cout << "Processing style " << style << " for target " << target << endl;
        cout << "- processing scale " << 0 << " iter " << 0 << endl;
    }

    inline string int_to_str(int n) {
        if (n < 10) {
            return "0" + std::to_string(n);
        }
        return std::to_string(n);
    }

    inline string slug() {
        return std::to_string(scale) + "_" + int_to_str(n_iter);
    }

    void next_scale() {
        scale++;
        n_iter = 0;
    }

    void next_iter() {
        n_iter++;
        cout << "- processing scale " << scale << " iter " << n_iter << endl;
    }

    void log_image(Mat_<Vec3b> image) {
        string filename = root + slug() + ".png";

        cv::imwrite(filename, image);
    }

    void log_image_bad(Mat_<Vec3b> image, int n) {
        string filename = root + slug() + "_" + int_to_str(n) + ".png";

        cv::imwrite(filename, image);
    }

    void log_sphere(Mat_<Vec3b> sphere) {
        string filename = root + "sphere_" + slug() + ".png";
        cv::imwrite(filename, sphere);
    }

    void log_distance_map(Mat_<float> d) {
        Mat_<float> copy(d.size(), 0.f);
        Rect r(2, 2, d.cols - P + 1, d.rows - P + 1);
        d(r).copyTo(copy);
        stretch_histogram(copy);
        copy *= 255.f;
        string filename = root + "distances_" + slug() + ".png";
        cv::imwrite(filename, copy);
    }

    void log_distances(const vector<float> &distances, float a, float b) {
        write_to_file(root + "distances/" + slug() + ".txt", distances, a, b);
    }

    template<class T>
    static void write_to_file(string filename, const std::vector<T> &data, float a, float b) {
        std::ofstream out(filename);
        out << a << " " << b;
        for (const T &d : data) {
            out << " " << d;
        }
        out.close();
    }

private:
    string root;
    int scale;
    int n_iter;
};

#endif //STYLIT_LOGGER_H
