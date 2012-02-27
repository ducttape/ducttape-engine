#include <Graphics/TextComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/State.hpp>

#include <OgreVector3.h>

class Main: public dt::State {
    Q_OBJECT
public:
    void ResetBall();

    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

public slots:
	void BallCollided(dt::PhysicsBodyComponent* collider);

private:
    dt::Node* mGameNode;
    dt::Node* mFieldNode;
	dt::Node* mFieldWallX1Node;
	dt::Node* mFieldWallX2Node;
	dt::Node* mFieldWallY1Node;
	dt::Node* mFieldWallY2Node;
    dt::Node* mBallNode;
    dt::Node* mPaddle1Node;
    dt::Node* mPaddle2Node;
    dt::TextComponent* mScore1Text;
    dt::TextComponent* mScore2Text;
    int mScore1;
    int mScore2;
};
