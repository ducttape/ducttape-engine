#include <Graphics/TextComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/State.hpp>

#include <OgreVector3.h>

class Main: public dt::State {
    Q_OBJECT
public:
    void resetBall();

    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

private:
    std::shared_ptr<dt::Node> mGameNode;
    std::shared_ptr<dt::Node> mFieldNode;
    std::shared_ptr<dt::Node> mBallNode;
    std::shared_ptr<dt::Node> mPaddle1Node;
    std::shared_ptr<dt::Node> mPaddle2Node;
    std::shared_ptr<dt::TextComponent> mScore1Text;
    std::shared_ptr<dt::TextComponent> mScore2Text;
    int mScore1;
    int mScore2;

    Ogre::Vector3 mBallSpeed;
};
