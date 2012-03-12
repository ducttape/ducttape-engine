#ifndef DUCTTAPE_SAMPLE_FPS_HITTABLE
#define DUCTTAPE_SAMPLE_FPS_HITTABLE

#include "Scene/Node.hpp"
#include "StatusComponent.hpp"
#include "Graphics/MeshComponent.hpp"

class Hittable : public dt::Node {
public:
    Hittable(const QString name = "");

    virtual void OnHit(int32_t damage);
};

#endif
