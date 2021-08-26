//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_ANIMAL_H
#define DUANG_ANIMAL_H

#include "Objects.h "

class Animal : public Objects {

public:
    Animal();

    void act(const std::string &);

    static Animal *create(const char *filename);
};


#endif //DUANG_ANIMAL_H
