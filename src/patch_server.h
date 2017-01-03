//
// Created by antoinewdg on 12/21/16.
//

#ifndef STYLIT_PATCH_SERVER_H
#define STYLIT_PATCH_SERVER_H

#include "patch_match_opencv/patch_servers/masked_patches.h"

#include "utils.h"

struct CenterOnly {
    bool operator()(const Mat_<bool> &mask, int i, int j, int P) {
        return mask(i, j);
    }
};

typedef pm::opencv::MaskedPatches<CenterOnly> RestrictedPatchServer;

#endif //STYLIT_PATCH_SERVER_H
