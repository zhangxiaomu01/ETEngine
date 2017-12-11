#pragma once
#include "../Graphics/FrameBuffer.hpp"

class Planet;
class Atmosphere;

class AtmoPreComputer : public Singleton<AtmoPreComputer>
{
public:
	void Precalculate( Atmosphere* atmo );

private:
	//Textures - probably also need fbos
	TextureData* m_TransmittanceT;

	TextureData* m_deltaET;
	TextureData* m_deltaSRT;
	TextureData* m_deltaSMT;
	TextureData* m_deltaJT;

	std::vector<TextureData*> m_IrradianceT;
	std::vector<TextureData*> m_InscatterT;

	//Precomputation Shaders
	ShaderData* m_CopyInscatter1;
	ShaderData* m_CopyInscatterN;
	ShaderData* m_CopyIrradiance;

	ShaderData* m_Inscatter1;
	ShaderData* m_InscatterN;
	ShaderData* m_InscatterS;

	ShaderData* m_Irradiance1;
	ShaderData* m_IrradianceN;
	ShaderData* m_Transmittance;

	ShaderData* m_ReadData;

	//other
	int32 m_Step = 0;
	int32 m_Order = 0;

	bool m_Finished = false;

private:
	friend class Singleton<AtmoPreComputer>;

	AtmoPreComputer();
	~AtmoPreComputer();
};

class Atmosphere
{
public:
	Atmosphere();
	~Atmosphere();

	void Precalculate();
	void Initialize();
	void Draw(Planet* pPlanet, float radius);

private:
	friend class AtmoPreComputer;

	//Camera and pos reconstruction from gbuffer
	GLint m_uMatModel;
	GLint m_uMatWVP;

	GLint m_uCamPos;
	GLint m_uProjA;
	GLint m_uProjB;
	GLint m_uViewProjInv;

	GLint m_uPosition;
	GLint m_uRadius;
	GLint m_uSurfaceRadius;

	//textures for precomputed data
	static const int TRANSMITTANCE_W = 256;
	static const int TRANSMITTANCE_H = 64;
	TextureData* m_Transmittance;

	static const int IRRADIANCE_W = 64;
	static const int IRRADIANCE_H = 16;
	TextureData* m_Irradiance;

	static const int INSCATTER_R = 32;
	static const int INSCATTER_MU = 128;
	static const int INSCATTER_MU_S = 32;
	static const int INSCATTER_NU = 8;
	GLuint m_uTexInscatter;

	ShaderData* m_pShader;
};

