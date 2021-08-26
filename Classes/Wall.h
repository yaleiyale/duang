//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_WALL_H
#define DUANG_WALL_H

#include "Objects.h"

class Wall : public Objects {
public:
    Wall();

    static Wall *create(const char *filename);

    static Wall *create();
};


#endif //DUANG_WALL_H
