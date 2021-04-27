#include "Components.hh"

namespace rym
{
    // Transform component
    static ComponentType TransformComponentType = typeid(TransformComponent).hash_code();
    ComponentType TransformComponent::GetType()
    {
        return TransformComponentType;
    }

    // Sprite component
    static ComponentType SpriteComponentType = typeid(SpriteComponent).hash_code();
    ComponentType SpriteComponent::GetType()
    {
        return SpriteComponentType;
    }

    // Camera component
    static ComponentType CameraComponentType = typeid(CameraComponent).hash_code();
    ComponentType CameraComponent::GetType()
    {
        return CameraComponentType;
    }

    // Native component
    static ComponentType NativeScriptComponentType = typeid(NativeScriptComponent).hash_code();
    ComponentType NativeScriptComponent::GetType()
    {
        return NativeScriptComponentType;
    }

	static ComponentType PyScriptComponentType = typeid(PyScriptComponent).hash_code();
	ComponentType PyScriptComponent::GetType()
	{
		return PyScriptComponentType;
	}
}
