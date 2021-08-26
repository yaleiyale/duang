//
// Created by zeyuz on 2021/6/16.
//

#ifndef DUANG_GIFT_H
#define DUANG_GIFT_H

#include "Objects.h"

class Gift : public Objects {

public:
    Gift();

    static Gift *create(const char *filename);
};


#endif //DUANG_GIFT_H
