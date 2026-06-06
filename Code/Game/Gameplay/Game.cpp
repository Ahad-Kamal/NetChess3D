#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Entity.hpp"
#include "Game/Gameplay/Entities/Player.hpp"
#include "Game/Gameplay/Entities/Prop.hpp"
#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Framework/App.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ObjUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Renderer/SimpleTriangleFont.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "ChessPieceDefinitions.hpp"


//-----------------------------------------------------------------------------------------------
Game* g_game = nullptr;
RandomNumberGenerator* g_rng = nullptr;
BitmapFont* g_bitmapFont = nullptr;

//-----------------------------------------------------------------------------------------------
Game::Game()
{
	Startup();
}

//-----------------------------------------------------------------------------------------------
Game::~Game()
{
	ChessPieceDefinition::ClearPieceDefs();
}

//-----------------------------------------------------------------------------------------------
void Game::Startup()
{
	m_gameClock = new Clock( Clock::GetSystemClock() );

	Shader* diffuseShader = g_engine->m_render->CreateOrGetShader( "Data/Shaders/Diffuse", VertexType::VERTEX_PCUTBN );
	g_engine->m_render->BindShader( diffuseShader );

	g_engine->m_render->SetPerFrameConstants( 0 );

	m_worldCamera = new Camera();
	m_screenCamera = new Camera();

	//m_worldCamera->SetOrthoView( Vec2( -WORLD_CENTER_X, -WORLD_CENTER_Y ), Vec2( WORLD_CENTER_X, WORLD_CENTER_Y ) );
	m_worldCamera->SetPerspectiveView( 2.f, 60.f, 0.1f, 1000.f );
	m_worldCamera->SetCameraToRenderTransform( Mat44::DirectXCameraToRenderMatrix );

	m_screenCamera->SetOrthoView( Vec2( 0.f, 0.f ), Vec2( SCREEN_SIZE_X, SCREEN_SIZE_Y ) );

	g_bitmapFont = g_engine->m_render->CreateOrGetBitmapFont( "Data/Images/SquirrelFixedFont" );

	g_engine->m_devConsole->PrintCommandsToConsole();
	AddCommandsToDevConsole();
	//AddControlsToDevConsole();

	InitializeStartTriangleVerts();
	TransformVertexArrayXY3D( 3, m_startVerts, 1.f, 0.f, Vec2( SCREEN_CENTER_X, SCREEN_CENTER_Y ) );

	Vec2 worldCenter( WORLD_SIZE_X * 0.5f, WORLD_SIZE_Y * 0.5f );

 	m_player = new Player( Vec3( -2.f, 0.f, 1.5f ), EulerAngles() );
	m_worldCamera->SetPosition( m_player->m_position );
	m_worldCamera->SetOrientation( m_player->m_orientation );

	AddDebugObjects();
	ObjTesting();

	ChessPieceDefinition::InitializePieceDefs();
	m_chessMatch = new ChessMatch();
}

//-----------------------------------------------------------------------------------------------
void Game::Update()
{
	float deltaSeconds = static_cast<float>( m_gameClock->GetDeltaSeconds() );

	UpdateStates();
	UpdateKeyboardInput();
	UpdateControllerInput();

	if( m_currentState == GAME_STATE_ATTRACT )
	{
		UpdateAttractMode( deltaSeconds );
		return;
	}

	UpdateNonChessEntities( deltaSeconds );
	m_chessMatch->Update();

	DeleteGarbageEntities();

	m_screenCamera->SetOrthoView( Vec2( 0.f, 0.f ), Vec2( SCREEN_SIZE_X, SCREEN_SIZE_Y ) );
	m_worldCamera->SetPerspectiveView( 2.f, 60.f, 0.1f, 100.f );
}

//-----------------------------------------------------------------------------------------------
void Game::Render() const
{
	if( m_currentState == GAME_STATE_ATTRACT )
	{
		RenderAttractMode();
		return;
	}

	RenderNonChessEntities();
	m_chessMatch->Render();
}

//-----------------------------------------------------------------------------------------------
void Game::Shutdown()
{
	delete m_gameClock;
	m_gameClock = nullptr;

	/*g_engine->m_audio->StopSound( m_gameMusic );
	g_engine->m_audio->StopSound( m_menuMusic );*/

	g_engine->m_devConsole->ClearLog();
}

//-----------------------------------------------------------------------------------------------
void Game::SetGameMusicSpeed( float speed )
{
	g_engine->m_audio->SetSoundPlaybackSpeed( m_music, speed );
}

//-----------------------------------------------------------------------------------------------
void Game::UpdateStates()
{
	if( m_currentState != m_nextState )
	{
		if( m_currentState != GAME_STATE_INVALID )
		{
			g_engine->m_audio->StartSound( audio_testSound );
		}
		if( m_nextState == GAME_STATE_PLAY )
		{
			
		}
		if( m_nextState == GAME_STATE_ATTRACT )
		{
			
		}

		m_currentState = m_nextState;
	}
}

//-----------------------------------------------------------------------------------------------
void Game::UpdateAttractMode( [[maybe_unused]] float deltaSeconds )
{
	m_time += deltaSeconds * 1.25f;
	m_startAlpha = 127.5f * cosf( m_time * 2.0f ) + 127.5f;

	for( int startIndex = 0; startIndex < 3; startIndex++ )
	{
		m_startVerts[ startIndex ].m_color = Rgba8( 0, 255, 0, (unsigned int)m_startAlpha );
	}
}

//-----------------------------------------------------------------------------------------------
void Game::RenderAttractMode() const
{
	g_engine->m_render->BeginCamera( *m_screenCamera );

	//g_engine->m_render->ClearScreen( m_clearColor );

	// Draw Start Button
	g_engine->m_render->RenderSetup();
	g_engine->m_render->DrawVertexArray( 3, m_startVerts );

	// Draw Text
	std::vector<Vertex> textStarshipDropShadowVerts;
	AddVertsForTextTriangles2D( textStarshipDropShadowVerts, "Attract", Vec2( 600.f, 700.f ), 40.f, Rgba8( 255, 0, 0 ) );
	g_engine->m_render->RenderSetup();
	g_engine->m_render->DrawVertexArray( (int)textStarshipDropShadowVerts.size(), textStarshipDropShadowVerts.data() );

	std::vector<Vertex> textGoldDropShadowVerts;
	AddVertsForTextTriangles2D( textGoldDropShadowVerts, "Screen", Vec2( 800.f, 700.f ), 40.f, Rgba8( 0, 0, 255 ) );
	g_engine->m_render->RenderSetup();;
	g_engine->m_render->DrawVertexArray( (int)textGoldDropShadowVerts.size(), textGoldDropShadowVerts.data() );
}

//-----------------------------------------------------------------------------------------------
void Game::UpdateNonChessEntities( float deltaSeconds )
{
	m_time += deltaSeconds;
	m_startAlpha = 127.5f * cosf( m_time * 2.0f ) + 127.5f;

	m_player->Update( deltaSeconds );

	AABB2 positionBox = AABB2( 1.f, 784.f, 800.f, 799.f );
	std::string positionText = Stringf( "Position: %.2f, %.2f, %.2f", m_player->m_position.x, m_player->m_position.y, m_player->m_position.z );
	DebugAddScreenText( positionText, positionBox, 15.f, Vec2( 0.f, 1.f ), 0.f );

	AABB2 timeBox = AABB2( 800.f, 784.f, 1599.f, 799.f );
	float totalSeconds = static_cast<float>( Clock::GetSystemClock().GetTotalSeconds() );
	float framesPerSecond = static_cast<float>( Clock::GetSystemClock().GetFrameCount() ) / totalSeconds;
	float timeScale = static_cast<float>( Clock::GetSystemClock().GetTimeScale() );
	std::string timeText = Stringf( "Time: %.2f FPS: %.1f Scale: %.2f", totalSeconds, framesPerSecond, timeScale );
	DebugAddScreenText( timeText, timeBox, 15.f, Vec2( 1.f, 0.f ), 0.f );
}

//-----------------------------------------------------------------------------------------------
void Game::RenderNonChessEntities() const
{
	g_engine->m_render->BeginCamera( *m_worldCamera );

	g_engine->m_render->BindTexture( nullptr );

	m_grid->Render();

	m_testObj->Render();

	g_engine->m_render->EndCamera( *m_worldCamera );
}

//-----------------------------------------------------------------------------------------------
void Game::UpdateKeyboardInput()
{
	if( m_currentState != GAME_STATE_PLAY )
	{
		return;
	}

	if( g_engine->m_input->WasKeyJustPressed( '0' ) )
	{
		g_engine->m_render->SetPerFrameConstants( 0 );
	}

	if( g_engine->m_input->WasKeyJustPressed( '1' ) )
	{
		g_engine->m_render->SetPerFrameConstants( 1 );
	}

	/*if( m_currentState == GAME_STATE_PLAY && g_engine->m_input->WasKeyJustPressed( '1' ) )
	{
		Vec3 endPosition = m_player->m_position + ( m_player->GetModelToWorldTransform().GetIBasis3D() * 20.f );
		DebugAddWorldCylinder( Vec3( m_player->m_position ), endPosition,
			0.0625f, 10.f, Rgba8::YELLOW, Rgba8::YELLOW, DebugRenderMode::X_RAY );
	}

	if( m_currentState == GAME_STATE_PLAY && g_engine->m_input->IsKeyDown( '2' ) )
	{
		DebugAddWorldSphere( Vec3( m_player->m_position.x, m_player->m_position.y, 0.f ), 0.5f, 60.f, Rgba8( 150, 75, 0 ), Rgba8( 150, 75, 0 ) );
	}

	if( m_currentState == GAME_STATE_PLAY && g_engine->m_input->WasKeyJustPressed( '3' ) )
	{
		Mat44 playerMat = m_player->GetModelToWorldTransform();
		playerMat.AppendTranslation2D( Vec2( 2.f, 0.f ) );
		DebugAddWorldWireSphere( playerMat.GetTranslation3D(), 1.f, 5.f, Rgba8::GREEN, Rgba8::RED );
	}

	if( m_currentState == GAME_STATE_PLAY && g_engine->m_input->WasKeyJustPressed( '4' ) )
	{
		DebugAddBasis( m_player->GetModelToWorldTransform(), 20.f, 1.f, 0.125f );
	}

	if( m_currentState == GAME_STATE_PLAY && g_engine->m_input->WasKeyJustPressed( '5' ) )
	{
		Vec3 position = m_player->GetModelToWorldTransform().GetTranslation3D() + m_player->GetModelToWorldTransform().GetIBasis3D() * 10.f;
		std::string text = Stringf("Position: %.2f, %.2f, %.2f", m_player->m_position.x, m_player->m_position.y, m_player->m_position.z );
		DebugAddWorldBillboardText( text, position, 1.f, Vec2( 0.5f, 0.5f ), 5.f, Rgba8::WHITE, Rgba8::RED );
	}

	if( m_currentState == GAME_STATE_PLAY && g_engine->m_input->WasKeyJustPressed( '6' ) )
	{
		DebugAddWorldWireCylinder( Vec3( m_player->m_position.x, m_player->m_position.y, m_player->m_position.z - 0.5f ), 
			Vec3( m_player->m_position.x, m_player->m_position.y, m_player->m_position.z + 0.5f ), 0.5f, 10.f, Rgba8::WHITE, Rgba8::RED );
	}

	if( m_currentState == GAME_STATE_PLAY && g_engine->m_input->WasKeyJustPressed( '7' ) )
	{
		AABB2 textBox = AABB2( 1.f, 770.f, 800.f, 785.f );
		std::string text = Stringf( "Camera Orientation: %.2f, %.2f, %.2f", m_worldCamera->GetOrientation().m_rollDegrees, m_worldCamera->GetOrientation().m_pitchDegrees, m_worldCamera->GetOrientation().m_yawDegrees );
		DebugAddScreenText( text, textBox, 15.f, Vec2( 0.f, 1.f ), 5.f );
	}*/
}

//-----------------------------------------------------------------------------------------------
void Game::UpdateControllerInput()
{
	XboxController const& controller = g_engine->m_input->m_controllers[ 0 ];

	if( m_currentState == GAME_STATE_ATTRACT && ( controller.WasButtonJustPressed( XboxButtonID::START ) || controller.WasButtonJustPressed( XboxButtonID::A ) ) )
	{
		m_nextState = GAME_STATE_PLAY;
	}

	if( m_currentState == GAME_STATE_ATTRACT && controller.WasButtonJustPressed( XboxButtonID::SELECT ) )
	{
		g_app->m_isQuitting = true;
	}

	if( m_currentState == GAME_STATE_PLAY && controller.WasButtonJustPressed( XboxButtonID::SELECT ) )
	{
		m_nextState = GAME_STATE_ATTRACT;
	}
}

//-----------------------------------------------------------------------------------------------
void Game::InitializeStartTriangleVerts()
{
	m_startVerts[ 0 ].m_position = Vec3( -200.f, 200.f, 0.f );
	m_startVerts[ 1 ].m_position = Vec3( -200.f, -200.f, 0.f );
	m_startVerts[ 2 ].m_position = Vec3( 200.f, 0.f, 0.f );

	for( int vertIndex = 0; vertIndex < 3; vertIndex++ )
	{
		m_startVerts[ vertIndex ].m_color = Rgba8( 0, 255, 0 );
	}
}

//-----------------------------------------------------------------------------------------------
void Game::AddDebugObjects()
{
	// Grid
	m_grid = new Prop( Vec3(), EulerAngles() );
	//CreateGrid();

	// World Basis
	DebugAddWorldBasis( Mat44(), -1.f, 0.05f, 1.f );

	//// X axis text
	//Mat44 xMatrix = Mat44();
	//xMatrix.AppendTranslation3D( Vec3( 6.f, 0.f, 1.f ) );
	//xMatrix.AppendZRotation( -90.f );
	//std::string xAxisText = "x - forward";
	//DebugAddWorldText( xAxisText, xMatrix, 1.f, Vec2( 0.5f, 0.5f ), -1.f, Rgba8::RED, Rgba8::RED );

	//// Y axis text
	//Mat44 yMatrix = Mat44();
	//yMatrix.AppendTranslation3D( Vec3( 0.f, 4.f, 1.f ) );
	//yMatrix.AppendZRotation( 180.f );
	//std::string yAxisText = "y - left ";
	//DebugAddWorldText( yAxisText, yMatrix, 1.f, Vec2( 0.5f, 0.5f ), -1.f, Rgba8::GREEN, Rgba8::GREEN );

	//// Z axis text
	//Mat44 zMatrix = Mat44();
	//zMatrix.AppendTranslation3D( Vec3( 0.f, -1.f, 3.f ) );
	//zMatrix.AppendXRotation( 90.f );
	//zMatrix.AppendYRotation( 180.f );
	//std::string zAxisText = " z - up";
	//DebugAddWorldText( zAxisText, zMatrix, 1.f, Vec2( 0.5f, 0.5f ), -1.f, Rgba8::BLUE, Rgba8::BLUE );
}

//-----------------------------------------------------------------------------------------------
void Game::DeleteGarbageEntities()
{

}

//-----------------------------------------------------------------------------------------------
void Game::AddCommandsToDevConsole()
{
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " ChessMove" );
}

//-----------------------------------------------------------------------------------------------
void Game::AddControlsToDevConsole()
{
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MAJOR, "Controls:" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Yaw and Pitch: Mouse, Right Stick" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Roll: Q/E, Left/Right Trigger" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Move left and right: A/D, Left Stick" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Move forward and back: W/S, Left Stick" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Move up and down: C/Z, Left/Right Shoulder" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Reset position and orientation: H, Start" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Increase speed: Shift, A button" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Pause: P" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Step Frame: O" );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " Slow motion: T" );
}

//-----------------------------------------------------------------------------------------------
void Game::CreateCube( Prop* cube )
{
	// -Y Magenta Face
	AddVertsForQuad3D( cube->m_vertexes, Vec3( -0.5f, -0.5f, -0.5f ), Vec3( 0.5f, -0.5f, -0.5f ), Vec3( 0.5f, -0.5f, 0.5f ), Vec3( -0.5f, -0.5f, 0.5f ), Rgba8::MAGENTA );
	// +Y Green Face
	AddVertsForQuad3D( cube->m_vertexes, Vec3( 0.5f, 0.5f, -0.5f ), Vec3( -0.5f, 0.5f, -0.5f ), Vec3( -0.5f, 0.5f, 0.5f ), Vec3( 0.5f, 0.5f, 0.5f ), Rgba8::GREEN );
	// +Z Blue Face
	AddVertsForQuad3D( cube->m_vertexes, Vec3( -0.5f, 0.5f, 0.5f ), Vec3( -0.5f, -0.5f, 0.5f ), Vec3( 0.5f, -0.5f, 0.5f ), Vec3( 0.5f, 0.5f, 0.5f ), Rgba8::BLUE );
	// -Z Yellow Face
	AddVertsForQuad3D( cube->m_vertexes, Vec3( 0.5f, 0.5f, -0.5f ), Vec3( 0.5f, -0.5f, -0.5f ), Vec3( -0.5f, -0.5f, -0.5f ), Vec3( -0.5f, 0.5f, -0.5f ), Rgba8::YELLOW );
	// +X Red Face
	AddVertsForQuad3D( cube->m_vertexes, Vec3( 0.5f, -0.5f, -0.5f ), Vec3( 0.5f, 0.5f, -0.5f ), Vec3( 0.5f, 0.5f, 0.5f ), Vec3( 0.5f, -0.5f, 0.5f ), Rgba8::RED );
	// -X Cyan Face
	AddVertsForQuad3D( cube->m_vertexes, Vec3( -0.5f, 0.5f, -0.5f ), Vec3( -0.5f, -0.5f, -0.5f ), Vec3( -0.5f, -0.5f, 0.5f ), Vec3( -0.5f, 0.5f, 0.5f ), Rgba8::CYAN );
}

//-----------------------------------------------------------------------------------------------
void Game::CreateGrid()
{
	// X Lines
	float yPos = -100.f;
	for( int i = 0; i < 200; i++ )
	{
		if( yPos == 0.f )
		{
			AABB3 bounds = AABB3( -100.f, -0.1f, -0.1f, 100.f, 0.1f, 0.1f );
			AddVertsForCube( m_grid->m_vertexes, bounds, Rgba8::RED );
		}
		else if( fmodf( yPos, 5.f ) == 0.f )
		{
			AABB3 bounds = AABB3( -100.f, -0.05f, -0.05f, 100.f, 0.05f, 0.05f );
			bounds.Translate( Vec3( 0.f, yPos, 0.f ) );
			AddVertsForCube( m_grid->m_vertexes, bounds, Rgba8( 150, 0, 0 ) );
		}
		else
		{
			AABB3 bounds = AABB3( -100.f, -0.025f, -0.025f, 100.f, 0.025f, 0.025f );
			bounds.Translate( Vec3( 0.f, yPos, 0.f ) );
			AddVertsForCube( m_grid->m_vertexes, bounds, Rgba8::DARK_GREY );
		}
		
		yPos += 1.f;
	}

	// Green Lines
	float xPos = -100.f;
	for( int j = 0; j < 200; j++ )
	{
		if( xPos == 0.f )
		{
			AABB3 bounds = AABB3( -0.1f, -100.f, -0.1f, 0.1f, 100.f, 0.1f );
			AddVertsForCube( m_grid->m_vertexes, bounds, Rgba8::GREEN );
		}
		else if( fmodf( xPos, 5.f ) == 0.f )
		{
			AABB3 bounds = AABB3(-0.05f, -100.f, -0.05f, 0.05f, 100.f, 0.05f );
			bounds.Translate( Vec3( xPos, 0.f, 0.f ) );
			AddVertsForCube( m_grid->m_vertexes, bounds, Rgba8( 0, 150, 0 ) );
		}
		else
		{
			AABB3 bounds = AABB3( -0.025f, -100.f, -0.025f, 0.025f, 100.f, 0.025f );
			bounds.Translate( Vec3( xPos, 0.f, 0.f ) );
			AddVertsForCube( m_grid->m_vertexes, bounds, Rgba8::DARK_GREY );
		}

		xPos += 1.f;
	}
}

//-----------------------------------------------------------------------------------------------
void Game::ObjTesting()
{
	VertexList verts;
	IndexList indexes;
	LoadObjFromFile( verts, indexes, "Data/Models/luigi.obj" );

	m_testObj = new Prop( Vec3( 50.f, 10.f, 17.5f ), EulerAngles( 180.f, 0.f, 90.f ) );
	m_testObj->m_vertexes = verts;
 	m_testObj->m_texture = g_engine->m_render->CreateOrGetTextureFromFile( "Data/Images/luigi_grp.png" );
}
