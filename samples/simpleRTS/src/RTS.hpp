#include <Graphics/TextComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/State.hpp>

#include <OgreVector3.h>

#include <Logic/SimplePlayerComponent.hpp>
#include "Crowd.hpp"
#include <Graphics/MeshComponent.hpp>

namespace dt {
class FollowPathComponent;
}
class Main: public dt::State {
    Q_OBJECT
public:
    void ResetGame();

    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

private:
    std::shared_ptr<dt::Node> mGameNode;
    std::shared_ptr<dt::Node> mFieldNode;
    std::shared_ptr<dt::Node> mBallNode;
    std::shared_ptr<dt::Node> mPaddle1Node;
    std::shared_ptr<dt::Node> mPaddle2Node;
    dt::TextComponent* mScore1Text;
    dt::TextComponent* mScore2Text;
    int mScore1;
    int mScore2;

    Ogre::Vector3 mBallSpeed;
    
    float mTime; //! Time lenght from the game beggining
    
    dt::SimplePlayerComponent* cam_player;
    
    Crowd mCrowd;
    dt::MeshComponent* mesh;
    dt::FollowPathComponent* path;
    dt::Node * node;
};