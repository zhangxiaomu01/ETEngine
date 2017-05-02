#include "stdafx.hpp"
#include "Planet.h"

#include <limits>

#include "../Graphics/ShaderData.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Graphics/TextureData.hpp"

#include "../Graphics/Frustum.h"
#include "Triangulator.h"
#include "Patch.h"
#include "Atmosphere.hpp"
#include "../Content/TextureLoader.hpp"
#include "../GraphicsHelper/RenderPipeline.hpp"
#include "../GraphicsHelper/RenderState.hpp"

Planet::Planet()
{
	m_pTriangulator = new Triangulator(this);
	m_pPatch = new Patch(4);
	m_pPatch->SetPlanet(this);
}
Planet::~Planet()
{
	SafeDelete(m_pPatch);
	SafeDelete(m_pTriangulator);
}

void Planet::Initialize()
{
	GetTransform()->SetRotation(glm::rotate(GetTransform()->GetRotation(), glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f)));

	//LoadTextures
	TextureLoader* pTL = ContentManager::GetLoader<TextureLoader, TextureData>();
	pTL->UseSrgb(true);
	LoadPlanet();

	m_pDetail1 = CONTENT::Load<TextureData>("Resources/Textures/PlanetTextures/MoonDetail1.jpg");
	m_pDetail2 = CONTENT::Load<TextureData>("Resources/Textures/PlanetTextures/MoonDetail2.jpg");
	m_pHeightDetail = CONTENT::Load<TextureData>("Resources/Textures/PlanetTextures/MoonHeightDetail1.jpg");
	pTL->UseSrgb(false);

	m_pTriangulator->Init();
	m_pPatch->Init();
}

void Planet::Update()
{
	GetTransform()->SetPosition(0, 0, 0);
	if (INPUT->IsKeyboardKeyPressed('r'))m_Rotate = !m_Rotate;
	if(m_Rotate)GetTransform()->SetRotation(glm::rotate(GetTransform()->GetRotation(), -(GLfloat)TIME->DeltaTime() * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f)));

	// #todo should happen after transform update maybe at the beginning of draw

	//Change Planet Geometry
	//**********************
	if (m_pTriangulator->Update())
	{
		//Change the actual vertex positions
		m_pTriangulator->GenerateGeometry();
		//Bind patch instances
		m_pPatch->BindInstances(m_pTriangulator->m_Positions);
		m_pPatch->UploadDistanceLUT(m_pTriangulator->m_DistanceLUT);
	}
}

void Planet::Draw()
{
	STATE->SetCullEnabled(false);
	m_pPatch->Draw();
	STATE->SetCullEnabled(true);
}
void Planet::DrawForward()
{
	if (m_pAtmopshere)
	{
		m_pAtmopshere->Draw(this, m_AtmRadius);
	}
}

int Planet::GetVertexCount()
{
	return m_pTriangulator->GetVertexCount()*m_pPatch->GetVertexCount();
}