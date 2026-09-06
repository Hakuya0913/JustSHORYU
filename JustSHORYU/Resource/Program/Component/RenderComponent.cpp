#include"RenderComponent.h"

RenderComponent::RenderComponent(
	TransformComponent& transform,
	ModelComponent& model,
	MaterialComponent& material
)
	:transformComponent(transform),
	modelComponent(model),
	materialComponent(material)
{

	//特に処理なし

}

