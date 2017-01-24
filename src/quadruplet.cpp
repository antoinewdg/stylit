#include "quadruplet.h"

vector<Quadruplet> Quadruplet::get_gaussian_pyramid(unsigned long size) {
    auto a_rendered_pyr = _compute_gaussian_pyramid(a_rendered);
    auto b_rendered_pyr = _compute_gaussian_pyramid(b_rendered);
    auto a_drawn_pyr = _compute_gaussian_pyramid(a_drawn);

    int i_max = int(std::min({size, a_rendered_pyr.size(), b_rendered_pyr.size()}));
    vector<Quadruplet> out;
    for (int i = 0; i < i_max; i++) {
        out.push_back(Quadruplet{
                a_rendered_pyr[i], b_rendered_pyr[i],
                a_drawn_pyr[i], Mat_<Vec3b>()
        });
    }

    return out;
}

Quadruplet Quadruplet::from_files(string style_name, string target_name) {
    return Quadruplet{
            _load_full_render(_file_root() + "sphere"),
            _load_full_render(_file_root() + target_name),
            load_color(_file_root() + "sphere/" + style_name + ".png"),
            Mat_<Vec3b>()
    };
}

Mat_<Vec3b> Quadruplet::_load_full_render(string name) {
    vector<Mat_<Vec3b>> lpes = {
            load_color(name + "/rendered.png"),
            load_color(name + "/lde.png"),
            load_color(name + "/lse.png"),
            load_color(name + "/ld12e.png"),
            load_color(name + "/l_dde.png")
    };

    Mat_<Vec15b> full(lpes[0].size());
    for (int i = 0; i < full.rows; i++) {
        for (int j = 0; j < full.cols; j++) {
            for (int c = 0; c < 15; c++) {
                full(i, j)[c] = lpes[c / 3](i, j)[c % 3];
            }
        }
    }


    return full;
}
