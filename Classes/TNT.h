//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_TNT_H
#define DUANG_TNT_H

#include "Objects.h"

class TNT : public Objects {
    bool destoryed = false;
public:
    TNT();

    void leave() override;

    static TNT *create(const char *filename);

};


#endif //DUANG_TNT_H
