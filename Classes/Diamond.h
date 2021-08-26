//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_DIAMOND_H
#define DUANG_DIAMOND_H

#include "Objects.h"

class Diamond : public Objects {
public :
    Diamond();

    static Diamond *create(const char *filename);
};


#endif //DUANG_DIAMOND_H
