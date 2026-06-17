#pragma once
#include "Engine/Audio/AudioSystem.hpp"


//-----------------------------------------------------------------------------------------------
struct Vec2;
struct Rgba8;
class ConstantBuffer;

//-----------------------------------------------------------------------------------------------
struct LightConstants
{
	float SunDirection[ 3 ];
	float SunIntensity;
	float AmbientIntensity;
	float EMPTY_PADDING[ 3 ];
};
static const int k_lightConstantSlot = 8;
extern ConstantBuffer* g_lightCBO;

//-----------------------------------------------------------------------------------------------
enum GameState
{
	GAME_STATE_INVALID = -1,
	GAME_STATE_ATTRACT,
	GAME_STATE_PLAY,
	GAME_STATE_VICTORY
};

//-----------------------------------------------------------------------------------------------
constexpr char FUNCTION_KEY_1 = 'p';
constexpr char FUNCTION_KEY_8 = 'w';
constexpr float WORLD_SIZE_X = 2.f;
constexpr float WORLD_SIZE_Y = 2.f;
constexpr float WORLD_SIZE_Z = 2.f;
constexpr float WORLD_VIEW_SIZE_X = 2.f;
constexpr float WORLD_VIEW_SIZE_Y = 2.f;
constexpr float SCREEN_SIZE_X = 1600.f;
constexpr float SCREEN_SIZE_Y = 800.f;
constexpr float WORLD_CENTER_X = WORLD_SIZE_X / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_SIZE_Y / 2.f;
constexpr float WORLD_CENTER_Z = WORLD_SIZE_Z / 2.f;
constexpr float SCREEN_CENTER_X = SCREEN_SIZE_X / 2.f;
constexpr float SCREEN_CENTER_Y = SCREEN_SIZE_Y / 2.f;
constexpr float DEBUG_LINE_THICKNESS = 0.2f;

//-----------------------------------------------------------------------------------------------
extern SoundID audio_music;
extern SoundID audio_selectSound;
extern SoundID audio_testSound;

//-----------------------------------------------------------------------------------------------
void SetLightConstants( const Vec3& sunDirection, float sunIntensity, float ambientIntensity );
void CreateLightCBO();
void DeleteLightCBO();

//-----------------------------------------------------------------------------------------------
void CreateSounds();