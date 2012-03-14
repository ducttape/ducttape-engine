
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef BEING_H
#define BEING_H

#include "Player.hpp"

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Logic/FollowPathComponent.hpp>

#include <OgreCommon.h>
#include <OgreAxisAlignedBox.h>

#include <map>
#include <vector>
#include "GameMap.hpp"


class Being;
typedef uint32_t BeingID;
typedef std::shared_ptr<Being> BeingPointer;
typedef std::map<BeingID, BeingPointer> MapCell;

class Being : public QObject, public std::enable_shared_from_this<Being> {
    Q_OBJECT
    
public:
    /**
     * Get the BeingID.
     * @return the BeingID.
     */
    BeingID GetBeingID();
    
    /**
     * The ctor is protected because being creation and deletion is 
     * managed by The Being class.
     * It allows to implement thread safety in the future.
     */
    virtual ~Being();
    
    /**
     * Get the node.
     * @return the node.
     */
    dt::Node* GetNode();
    
    /**
     * Get the bounding box of the mesh of the being.
     * @return the bounding box of the mesh of the being or
     * a zero bounding box if no mesh exist.
     * @throw runtime_error if no mesh exist.
     */
    Ogre::AxisAlignedBox GetBoundingBox();
    
    /**
     * Move the Being to a specific position.
     * It's virtual because each being may implement it's moving
     * process (e.g. path, flight, etc.)
     * @param position the position where the Being will move to.
     */
    virtual void MoveTo(const Ogre::Vector3& position);
    
    /**
     * Roteate the Being towards a specific position.
     * @param position the position where the Being will face to.
     */
    void LookAt(const Ogre::Vector3& position);
    
    /**
     * Make the Being stare towards an another Being.
     * The Being will follow the other being looking at him.
     * @param being the being where the Being will stare to.
     */
    void StareAtBeing(const BeingPointer& being);
    
    /**
     * Make the Being to stop staring;
     */
    void StopStaring();

    /**
     * Set the scene in which the nodes of the created Beings will be added.
     * This method should be called before the creation of any being.
     * @param scene the scene in which the nodes of the created Beings will be added.
     */
    static void SetScene(dt::Scene::SceneSP scene);
    
    /**
     * Set the mesh of the Being.
     * @param mesh the mesh that will be set.
     */
    void SetMesh(const QString& mesh_name);
    
    /**
     * Get the position in a cartesian plane parallel to x and z vectors.
     * @return a Ogre::Vector2, while the x component is the same of the node, the 
     * y component is the same of the z component of the node.
     * TODO: write this in a better way.
     */
    Ogre::Vector2 Get2DPosition();
    
    /**
     * Find a Being with a BeingID.
     * @param being_ID the BeingID of the Being to find.
     * @return a pointer to a Being, a null pointer if the Being with ID being_ID
     * doesn't exist.
     * @throw runtime_error if being with ID being_ID doesn't exist.
     */
    static BeingPointer FindBeing(const BeingID being_ID);
    
    /**
     * Get a reference to the container that holds all the Beings.
     * @return a const reference to the container that holds all the Beings.
     */
    static std::map<BeingID, BeingPointer>& GetAllBeings();

    /**
     * Destroy a Being
     * @param being_ID the BeingID of the being to destroy.
     */
    void _KillBeing();
    
    /**
     * Reduce the HP of a Being.
     * @param being_ID the BeingPointer of the being to reduce the HP.
     * @param hp the quantity of HP to subtract.
     * @return true if the HP of the Being goes under 0.
     */
    bool ReduceHP(uint32_t hp);
    
    /**
     * Destroy the Being.
     */
    void Destroy();
    
    /**
     * Update all the Beings during the game. 
     * This static method should be called once in each loop of the game.
     * @param time_diff time of the frame.
     */
    static void UpdateBeings(double time_diff);

    /**
     * Get the player ID of the player that has this Being.
     * @return the player ID of the player that has this Being.
     */
    PlayerID GetPlayerID();
    
    MapCell* GetMapCell();
    
public slots:  
  
    /**
     * The Being which this was staring at was killed,
     * remove its pointer.
     */
    void StopStareAtBeing();
    
protected:
    
    /**
     * The ctor is protected because being creation and deletion is 
     * managed by The Being class.
     * It allows to implement thread safety in the future.
     * @param position position of the Being.
     * @param player_id the player that will own this Being.
     */
    Being(const Ogre::Vector3& position, uint32_t hp, PlayerID player_id);
    
    /**
     * Initialize all resources needed for the creation of the Being.
     * Here each Being class could load meshes or texture.
     */
    virtual void InitializeResources();
    
    /**
     * Update the Being during the game. 
     * This method is called by the static method UpdateBeings. 
     * @param time_diff time of the frame.
     */
    void Update(double time_diff);
    
    /**
     * This is the first method called from OnUpdate().
     * @param time_diff time of the frame. 
     */ 
    virtual void OnUpdateStart(double time_diff);
    
    /**
     * This is the first method called from OnUpdate().
     * @param time_diff time of the frame. 
     */ 
    virtual void OnUpdate(double time_diff);
    
    /**
     * This is the last method called from OnUpdate().
     * @param time_diff time of the frame. 
     */ 
    virtual void OnUpdateEnd(double time_diff);
    
    /**
     * Update the cell in which the Being is.
     */
    void UpdateCell();
    
    float GetNowTime();
    
    /**
     * Whetere the Resources needed for the Being creation are 
     * initialized
     */
    static bool mResourceInitialized;
    BeingID mBeingID;                      //!< BeingID of the Being.
    double mHP;                            //!< Hit Points.
    dt::Node* mNode;                       //!< The node of the Being.
    dt::MeshComponent* mMeshComponent;     //!< The mesh component of the Being.
    Ogre::Vector3 mLookAtPosition;         //!< The position where the Being is looking at.
    BeingPointer mStareAtBeing;            //!< The other being which the Being is staring at.
    bool mStaringAtBeing;                  //!< Wheter the Being is staring to an another Being. 
    bool mLookingAt;                       //!< Wheter the Being is rotating to look to a position.
    static dt::Scene::SceneSP mScene;      //!< Scene where the Beings will be added.
    PlayerID mPlayerID;                    //!< The player ID that has this Being.
    MapCell* mMapCell;  //!< The cell in which the Being is.
    
//     std::deque<BeingPointer> CreateBeings(uint32_t number);

    static std::map<BeingID, BeingPointer> mBeingMap; //!< The container that holds all the Beings.
    
signals:
    void killed(BeingID being_id);
    

    
private:
    
//     /**
//      * Get an avaiable ID which doesn't match with any other Being.
//      * @return an avaiable BeingPointer.
//      */
//     static BeingPointer GetAvailableID();
    
//     static GameMap* mGameMap;           //!< The map of the game where position of the Beings are recorded.
    
    static uint32_t mCreatedCount;              //!< The count of the all created (alive or not) Beings.
    static double mNowTime;
    
};

#endif // BEING_H
