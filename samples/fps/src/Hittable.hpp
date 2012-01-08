#ifndef HITTABLE_H
#define HITTABLE_H
#pragma once

#include "Scene/Node.hpp"
#include "StatusComponent.hpp"
#include "Graphics/MeshComponent.hpp"

class Hittable : public dt::Node {
public:
    Hittable(const QString& name = "");

    virtual void OnHit(int damage);
};

#endif