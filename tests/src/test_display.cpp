#include "Root.hpp"
#include "game/Game.hpp"
#include "scene/Scene.hpp"
#include "scene/Node.hpp"

class Game : public dt::Game {
public:
    void OnInitialize() {
        /*
        dt::Node* node = new dt::Node("camnode");
        node->AddComponent(new dt::CameraComponent("cam"));
        mScene.AddChildNode(node);*/
    }

private:
    dt::Scene mScene;

};

int main() {
    return 0;
}
