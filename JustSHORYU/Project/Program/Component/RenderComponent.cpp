#include"RenderComponent.h"

void RenderComponent::SetTransform(TransformComponent& transform)
{

	transformComponent = &transform;

}

void RenderComponent::SetMaterial(MaterialComponent& material)
{

	materialComponent = &material;

}

void RenderComponent::SetModel(ModelComponent& model)
{

	modelComponent = &model;

}