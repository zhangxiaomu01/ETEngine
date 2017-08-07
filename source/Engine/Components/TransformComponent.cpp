#include "stdafx.hpp"

#include "TransformComponent.hpp"
#include "LightComponent.hpp"
#include "../SceneGraph/Entity.hpp"

//Use lefthanded coordinate system
#ifndef GLM_LEFT_HANDED
	#define GLM_LEFT_HANDED
#endif
#include <gtx/transform.hpp>
#include <gtx\quaternion.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/euler_angles.hpp>



TransformComponent::TransformComponent()
{
	m_IsTransformChanged = TransformChanged::NONE;
}


TransformComponent::~TransformComponent()
{
}

void TransformComponent::Initialize()
{
	UpdateTransforms();
}

void TransformComponent::Update()
{
	UpdateTransforms();
}

void TransformComponent::UpdateTransforms()
{
	if (m_IsTransformChanged & TransformChanged::NONE)
		return;

	if (m_IsTransformChanged & TransformChanged::TRANSLATION | TransformChanged::ROTATION)
	{
		auto ligComp = m_pEntity->GetComponent<LightComponent>();
		if (!(ligComp == nullptr))
		{
			ligComp->m_PositionUpdated = true;
		}
	}

	//Calculate World Matrix
	//**********************
	m_World = glm::translate(m_Position)*glm::toMat4(m_Rotation)*glm::scale(m_Scale);

	Entity* parent = m_pEntity->GetParent();
	if (parent)
	{
		glm::mat4 parentWorld = parent->GetTransform()->m_World;
		m_World *= parentWorld;

		m_WorldPosition = glm::vec3(parentWorld * glm::vec4(m_Position, 0));
		m_WorldRotation = m_Rotation * parent->GetTransform()->GetWorldRotation();
		m_WorldScale = m_Scale * parent->GetTransform()->GetWorldScale();
	}
	else
	{
		m_WorldPosition = m_Position;
		m_WorldRotation = m_Rotation;
		m_WorldScale = m_Scale;
	}
	
	//Get World Transform
	//#todo there is an issue where glm decomposition does not have a left handed version, 
	//thus the resulting world rotation is mangled as if its input had been a right handed rotation
	//this problem has been overlooked for a large part of the engines development, resulting in code that is adapted to work with this bug
	//the correct solution would be to not use decomposition here at all, but because of the adapted code the correct solution causes incorrect behaviour
	//therefore we temporarily override the correct result with the faulty decomposited rotation
	glm::quat rot;
	if (glm::decompose(m_World, glm::vec3(), rot, glm::vec3(), glm::vec3(), glm::vec4()))
	{
		m_WorldRotation = rot;
	}

	m_Forward = m_WorldRotation*glm::vec3(0, 0, 1);
	m_Right = m_WorldRotation*glm::vec3(1, 0, 0);
	m_Up = glm::cross(m_Forward, m_Right);

	m_IsTransformChanged = TransformChanged::NONE;
}

void TransformComponent::Draw()
{
}
void TransformComponent::DrawForward()
{
}

void TransformComponent::Translate(float x, float y, float z)
{
	m_IsTransformChanged |= TransformChanged::TRANSLATION;
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
}
void TransformComponent::Translate(const glm::vec3& position)
{
	Translate(position.x, position.y, position.z);
}
void TransformComponent::SetPosition(float x, float y, float z)
{
	m_IsTransformChanged |= TransformChanged::TRANSLATION;
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
void TransformComponent::SetPosition(const glm::vec3& position)
{
	SetPosition(position.x, position.y, position.z);
}

void TransformComponent::RotateEuler(float x, float y, float z)
{
	using namespace glm;
	m_IsTransformChanged |= TransformChanged::ROTATION;
	
	m_Rotation = m_Rotation * quat_cast(eulerAngleYXZ(y, x, z));
}
void TransformComponent::RotateEuler(const glm::vec3& rotation)
{
	RotateEuler(rotation.x, rotation.y, rotation.z);
}
void TransformComponent::Rotate(const glm::quat& rotation)
{
	m_Rotation = m_Rotation * rotation;
}

void TransformComponent::Scale(float x, float y, float z)
{
	m_IsTransformChanged |= TransformChanged::SCALE;
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}
void TransformComponent::Scale(const glm::vec3& scale)
{
	Scale(scale.x, scale.y, scale.z);
}