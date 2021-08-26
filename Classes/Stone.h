//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_STONE_H
#define DUANG_STONE_H

#include "Objects.h"
#include "Stone.h"

class Stone : public Objects {
public:
    Stone();

    static Stone *create(const char *filename);
};


#endif //DUANG_STONE_H
