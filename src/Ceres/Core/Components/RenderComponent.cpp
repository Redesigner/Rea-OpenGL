#include "RenderComponent.h"

namespace Ceres
{
    RenderComponent::RenderComponent(const IEntity& parent, uint8_t meshId)
        :IComponent(parent, std::type_index(typeid(RenderComponent))), MeshId(meshId), Transform()
    {}

    RenderComponent::~RenderComponent()
    {}

    bool RenderComponent::RecieveMessage(Message* message)
    {
        if(message->Type == "Translate")
        {
            Vector3 position = message->GetData<Vector3>();
            Transform.SetPosition(Transform.GetPosition() + position);
            return true;
        }
        else if(message->Type == "Scale")
        {
            Vector3 scale = message->GetData<Vector3>();
            Transform.SetScale(scale);
            return true;
        }
        else if(message->Type == "Rotate")
        {
            Transform.SetRotation(Transform.GetRotation() + message->GetData<Vector3>());
            return true;
        }
        return false;
    }

}