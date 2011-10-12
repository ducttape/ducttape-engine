#include <Graphics/TextComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/State.hpp>

#include <OgreVector3.h>

class Main: public dt::State {
    Q_OBJECT
public:
    void ResetBall();

    void OnInitialize();

    private slots:
        void _GameLogic(double simulation_frame_time);

private:
    dt::Node* mGameNode;
    dt::Node* mFieldNode;
    dt::Node* mBallNode;
    dt::Node* mPaddle1Node;
    dt::Node* mPaddle2Node;
    dt::TextComponent* mScore1Text;
    dt::TextComponent* mScore2Text;
    int mScore1;
    int mScore2;

    Ogre::Vector3 mBallSpeed;
};
