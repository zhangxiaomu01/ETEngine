#pragma once

#include "AtmosphereSettings.h"

class Atmosphere;

class AtmospherePrecompute : public Singleton<AtmospherePrecompute>
{
public:
	//Separate from constructor so we can unload the resources if we don't need them anymore
	void Init();
	void Unload();

	void Precalculate(Atmosphere* atmo);
	void SetUniforms(ShaderData* shader, TextureData* transmittance,
		TextureData* scattering, TextureData* irradiance, TextureData* mie);

	double Interpolate(const std::vector<double>& wavelengths, const std::vector<double>& wavelength_function, double wavelength);

private:
	double CieColorMatchingFunctionTableValue(double wavelength, int column);
	void ComputeSpectralRadianceToLuminanceFactors(const std::vector<double>& wavelengths, const std::vector<double>& solar_irradiance, double lambda_power, double* k_r, double* k_g, double* k_b);
	void ConvertSpectrumToLinearSrgb(const std::vector<double>& wavelengths, const std::vector<double>& spectrum, double* r, double* g, double* b);


	//Textures - probably also need fbos
	TextureData* m_TexDeltaIrradiance;
	TextureData* m_TexDeltaRayleigh;
	TextureData* m_TexDeltaMultipleScattering;
	TextureData* m_TexDeltaMie;
	TextureData* m_TexDeltaScattering;

	GLuint m_FBO;

	AtmosphereSettings m_Settings;

	//Precomputation Shaders
	ShaderData* m_pComputeTransmittance;
	ShaderData* m_pComputeDirectIrradiance;
	ShaderData* m_pComputeSingleScattering;
	ShaderData* m_pComputeScatteringDensity;
	ShaderData* m_pComputeIndirectIrradiance;
	ShaderData* m_pComputeMultipleScattering;

	//other
	int32 m_Step = 0;
	int32 m_Order = 0;

	bool m_IsInitialized = false;
	bool m_Finished = false;

private:
	friend class Singleton<AtmospherePrecompute>;

	AtmospherePrecompute();
	~AtmospherePrecompute();
};
