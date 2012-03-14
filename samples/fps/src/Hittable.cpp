#include "Hittable.hpp"

Hittable::Hittable(const QString name)
    : dt::Node(name) {}

void Hittable::onHit(int32_t damage) {}
