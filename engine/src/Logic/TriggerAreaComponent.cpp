#include <Logic/TriggerAreaComponent.hpp>

namespace dt
{

TriggerAreaComponent::TriggerAreaComponent(ScriptComponent* script, 
                                           Node* node, 
                                           Ogre::AxisAlignedBox area, 
                                           const QString& name) : 
    Component(name),
    mTriggeringNode(node),
    mArea(area),
    mScript(script),
    mOneShotMode(false)
{
    mScript->SetUpdateEnabled(false);
}

void TriggerAreaComponent::OnUpdate(double time_diff)
{
    if(mOneShotMode && mScript->IsUpdateEnabled())
        return;

    Ogre::AxisAlignedBox transformed_area = mArea;
    transformed_area.transform(Ogre::Matrix4::getTrans(mNode->GetPosition()));
    mScript->SetUpdateEnabled(transformed_area.contains(mTriggeringNode->GetPosition()));
}

void TriggerAreaComponent::OnCreate(){}
void TriggerAreaComponent::OnDestroy(){}
void TriggerAreaComponent::OnEnable(){}
void TriggerAreaComponent::OnDisable(){}

void TriggerAreaComponent::SetOneShotMode(bool oneShotMode)
{
    mOneShotMode = oneShotMode;
}

void TriggerAreaComponent::SetNode(Node* node)
{
    mTriggeringNode = node;
}

void TriggerAreaComponent::SetScript(ScriptComponent* script)
{
    mScript = script;
}

}