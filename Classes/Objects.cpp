//
// Created by zeyuz on 2021/6/13.
//

#include "Objects.h"


bool Objects::isRemoveable() {
    if (this->removeable)
        return true;
    return false;
}

void Objects::leave() {
    this->removeFromParent();
}

int Objects::getValue() {
    return value;
}

void Objects::setWeight(float w) {
    weight = w;
}

void Objects::setValue(int v) {
    value = v;
}

float Objects::getWeight() {
    return weight;
}
