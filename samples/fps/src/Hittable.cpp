#include "Hittable.hpp"

Hittable::Hittable(const QString& name)
    : dt::Node(name) {}

void Hittable::OnHit(int damage) {}