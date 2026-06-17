#include "Game/Framework/GameCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Vertex.hpp"


//-----------------------------------------------------------------------------------------------
SoundID audio_music;
SoundID audio_selectSound;
SoundID audio_testSound;

//-----------------------------------------------------------------------------------------------
ConstantBuffer* g_lightCBO = nullptr;

//-----------------------------------------------------------------------------------------------
void SetLightConstants( const Vec3& sunDirection, float sunIntensity, float ambientIntensity )
{
	LightConstants lightConstants;
	lightConstants.SunDirection[ 0 ] = sunDirection.x;
	lightConstants.SunDirection[ 1 ] = sunDirection.y;
	lightConstants.SunDirection[ 2 ] = sunDirection.z;
	lightConstants.SunIntensity = sunIntensity;
	lightConstants.AmbientIntensity = ambientIntensity;
	g_engine->m_render->CopyCPUToGPU( &lightConstants, sizeof( LightConstants ), g_lightCBO );
	g_engine->m_render->BindConstantBuffer( k_lightConstantSlot, g_lightCBO );
}

//-----------------------------------------------------------------------------------------------
void CreateLightCBO()
{
	g_lightCBO = g_engine->m_render->CreateConstantBuffer( sizeof( LightConstants ) );
}

//-----------------------------------------------------------------------------------------------
void DeleteLightCBO()
{
	delete g_lightCBO;
	g_lightCBO = nullptr;
}

//-----------------------------------------------------------------------------------------------
void CreateSounds()
{

}
