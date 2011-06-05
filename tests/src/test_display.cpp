#include "Root.hpp"
#include "game/Game.hpp"
#include "scene/Scene.hpp"
#include "scene/Node.hpp"
#include "component/TriggerComponent.hpp"

class Game : public dt::Game {
public:
    Game()
        : mScene("gamescene") {

    }

    void OnInitialize() {
        dt::Node* node = new dt::Node("camnode");
        mScene.AddChildNode(node);
        mScene.FindChildNode("camnode", false)->AddComponent(new dt::CameraComponent("cam"));
        mScene.FindChildNode("camnode", false)->AddComponent(new dt::TriggerComponent("trigger"));
    }

private:
    dt::Scene mScene;

};

int main() {
    Game g;
    g.Run();
    return 0;
}
