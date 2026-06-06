#pragma once
#include "Game/Framework/GameCommon.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Core/Vertex.hpp"


//-----------------------------------------------------------------------------------------------
class Game;
class App;
class Camera;
class Entity;
class Clock;
class Player;
class Prop;
class BitmapFont;
class ChessMatch;

//-----------------------------------------------------------------------------------------------
extern Game* g_game;
extern RandomNumberGenerator* g_rng;
extern BitmapFont* g_bitmapFont;

//-----------------------------------------------------------------------------------------------
class Game
{
public:
	Game();
	~Game();
	void Startup();
	void Update();
	void Render() const;
	void Shutdown();

	void SetGameMusicSpeed( float speed );

private:
	void UpdateStates();
	void UpdateAttractMode( float deltaSeconds );
	void RenderAttractMode() const;

	void UpdateNonChessEntities( float deltaSeconds );
	void RenderNonChessEntities() const;

	void UpdateKeyboardInput();
	void UpdateControllerInput();

	void InitializeStartTriangleVerts();
	void AddDebugObjects();

	void DeleteGarbageEntities();

	void AddCommandsToDevConsole();
	void AddControlsToDevConsole();

	void CreateCube( Prop* cube );
	void CreateGrid();
	void ObjTesting();

public:
	ChessMatch* m_chessMatch = nullptr;

	Player*		m_player = nullptr;
	Prop*		m_grid = nullptr;
	Prop*		m_testObj = nullptr;

	Camera*		m_worldCamera;
	Camera*		m_screenCamera;

	Clock*		m_gameClock;

	GameState	m_currentState = GAME_STATE_INVALID;
	GameState	m_nextState = GAME_STATE_ATTRACT;

	Vec3 m_sunDirection = Vec3( 3.f, 1.f, -2.f );
	float m_sunIntensity = 0.85f;
	float m_ambientIntensity = 0.35f;

private:
	Vertex m_startVerts[ 3 ];
	float m_startAlpha = 255.f;
	float m_time = 0.f;
	Rgba8 m_clearColor = Rgba8( 0, 0, 0, 1 );
	SoundPlaybackID m_music;
};