//
// Created by antoinewdg on 12/3/16.
//

#ifndef STYLIT_LOGGER_H
#define STYLIT_LOGGER_H

#include <ctime>
#include <boost/filesystem.hpp>

#include "utils.h"

namespace fs = boost::filesystem;

class Logger {
public:
    Logger(string style, string target) : scale(0), n_iter(0) {
        root = _file_root() + "out/" + std::to_string(time(0)) + "/" + target + "/" +
               style + "/";

        fs::create_directories(fs::path(root));
    }

    inline string slug() {
        return std::to_string(scale) + '_' + std::to_string(n_iter);
    }

    void next_scale() {
        scale++;
        n_iter = 0;
    }

    void next_iter() {
        n_iter++;
    }

    void log_image(Mat_<Vec3b> image) {
        string filename = root + slug() + ".png";

        cv::imwrite(filename, image);
    }

private:
    string root;
    int scale;
    int n_iter;
};

#endif //STYLIT_LOGGER_H
