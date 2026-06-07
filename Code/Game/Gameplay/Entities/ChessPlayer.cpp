#include "Game/Gameplay/Entities/ChessPlayer.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Game/Gameplay/Entity.hpp"
#include "Game/Gameplay/ChessMatch.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/MathUtils.hpp"


//-----------------------------------------------------------------------------------------------
ChessPlayer::ChessPlayer( Vec3 const& startingPosition, EulerAngles orientation )
	:	Entity( g_game, startingPosition )
{
	m_orientation = orientation;
	m_velocity = startingPosition;

	if( m_cameraMode == CameraMode::POV )
	{
		m_velocity = Vec3();
		m_position = Vec3( 4.f, -3.25f, 4.f );
		m_orientation = EulerAngles( 90.f, 30.f, 0.f );
	}
	else if( m_cameraMode == CameraMode::SPECTATOR )
	{
		m_velocity = Vec3();
		m_position = Vec3( 4.f, 3.3f, 8.f );
		m_orientation = EulerAngles( 90.f, 85.f, 0.f );
	}
}

//-----------------------------------------------------------------------------------------------
ChessPlayer::~ChessPlayer()
{

}

//-----------------------------------------------------------------------------------------------
void ChessPlayer::Update( [[maybe_unused]] float deltaSeconds )
{
	float systemDeltaSeconds = static_cast<float>( Clock::GetSystemClock().GetDeltaSeconds() );
	m_position += m_velocity;

	if( g_game->m_currentState == GAME_STATE_PLAY )
	{
		if( g_engine->m_input->WasKeyJustPressed( KEYCODE_F4 ) )
		{
			ToggleCameraMode();
		}

		CameraControlsKeyboard( systemDeltaSeconds );
		CameraControlsController( systemDeltaSeconds );
	}

	g_game->m_worldCamera->SetPosition( m_position );
	g_game->m_worldCamera->SetOrientation( m_orientation );
}

//-----------------------------------------------------------------------------------------------
void ChessPlayer::Render() const
{

}

//-----------------------------------------------------------------------------------------------
void ChessPlayer::ToggleCameraMode()
{
	switch( m_cameraMode )
	{
		case CameraMode::POV:
			m_cameraMode = CameraMode::SPECTATOR;
			break;

		case CameraMode::SPECTATOR:
			m_cameraMode = CameraMode::FREE;
			break;

		case CameraMode::FREE:
			m_cameraMode = CameraMode::POV;
			break;
	}

	if( m_cameraMode == CameraMode::POV )
	{
		if( g_game->m_chessMatch->m_currentPlayerTurn == TEAM_PLAYER_1 )
		{
			m_position = Vec3( 4.f, -3.25f, 4.f );
			m_orientation = EulerAngles( 90.f, 30.f, 0.f );
		}
		else
		{
			m_position = Vec3( 4.f, 11.25f, 4.f );
			m_orientation = EulerAngles( 90.f, 30.f, 270.f );
		}
	}
	else if( m_cameraMode == CameraMode::SPECTATOR )
	{
		m_position = Vec3( 4.f, 3.3f, 8.f );
		m_orientation = EulerAngles( 90.f, 85.f, 0.f );
	}
}

//-----------------------------------------------------------------------------------------------
void ChessPlayer::TogglePOVSide()
{
	if( g_game->m_chessMatch->m_currentPlayerTurn == TEAM_PLAYER_1 )
	{
		m_position = Vec3( 4.f, -3.25f, 4.f );
		m_orientation = EulerAngles( 90.f, 30.f, 0.f );
	}
	else
	{
		m_position = Vec3( 4.f, 11.25f, 4.f );
		m_orientation = EulerAngles( 270.f, 30.f, 0.f );
	}
}

//-----------------------------------------------------------------------------------------------
void ChessPlayer::CameraControlsKeyboard( float deltaSeconds )
{
	if( m_cameraMode != CameraMode::FREE )
	{
		m_velocity = Vec3();
		return;
	}

	Vec3 forwardVector = m_orientation.GetForwardDir_IFwd_JLeft_KUp();
	float speedFactor = 5.f;

	if( g_engine->m_input->IsKeyDown( KEYCODE_SHIFT ) )
	{
		speedFactor *= 10.f;
	}

	m_velocity = Vec3();

	// Left and Right
	if( g_engine->m_input->IsKeyDown( 'A' ) )
	{
		m_velocity.x += forwardVector.GetRotated90DegreesAboutZ().x * deltaSeconds * speedFactor;
		m_velocity.y += forwardVector.GetRotated90DegreesAboutZ().y * deltaSeconds * speedFactor;

	}
	if( g_engine->m_input->IsKeyDown( 'D' ) )
	{
		m_velocity.x -= forwardVector.GetRotated90DegreesAboutZ().x * deltaSeconds * speedFactor;
		m_velocity.y -= forwardVector.GetRotated90DegreesAboutZ().y * deltaSeconds * speedFactor;
	}

	// Forward and Back
	if( g_engine->m_input->IsKeyDown( 'W' ) )
	{
		m_velocity.x += forwardVector.x * deltaSeconds * speedFactor;
		m_velocity.y += forwardVector.y * deltaSeconds * speedFactor;
		m_velocity.z += forwardVector.z * deltaSeconds * speedFactor;
	}
	if( g_engine->m_input->IsKeyDown( 'S' ) )
	{
		m_velocity.x -= forwardVector.x * deltaSeconds * speedFactor;
		m_velocity.y -= forwardVector.y * deltaSeconds * speedFactor;
		m_velocity.z -= forwardVector.z * deltaSeconds * speedFactor;
	}

	// Up and Down
	if( g_engine->m_input->IsKeyDown( 'Z' ) )
	{
		m_velocity.z += deltaSeconds * speedFactor;
	}
	if( g_engine->m_input->IsKeyDown( 'C' ) )
	{
		m_velocity.z -= deltaSeconds * speedFactor;
	}

	// Yaw
	if( g_engine->m_input->m_cursorState.m_cursorMode == CursorMode::FPS )
	{
		m_orientation.m_yawDegrees -= g_engine->m_input->m_cursorState.m_cursorClientDelta.x * 0.075f;
	}

	// Pitch
	if( g_engine->m_input->m_cursorState.m_cursorMode == CursorMode::FPS )
	{
		m_orientation.m_pitchDegrees += g_engine->m_input->m_cursorState.m_cursorClientDelta.y * 0.075f;
		m_orientation.m_pitchDegrees = GetClamped( m_orientation.m_pitchDegrees, -85.f, 85.f );
	}

	//// Roll
	//if( g_engine->m_input->IsKeyDown( 'E' ) )
	//{
	//	m_orientation.m_rollDegrees += 90.f * deltaSeconds;
	//	m_orientation.m_rollDegrees = GetClamped( m_orientation.m_rollDegrees, -45.f, 45.f );
	//}
	//if( g_engine->m_input->IsKeyDown( 'Q' ) )
	//{
	//	m_orientation.m_rollDegrees -= 90.f * deltaSeconds;
	//	m_orientation.m_rollDegrees = GetClamped( m_orientation.m_rollDegrees, -45.f, 45.f );
	//}

	// Reset
	if( g_engine->m_input->WasKeyJustPressed( 'H' ) )
	{
		m_position = Vec3();
		m_velocity = Vec3();
		m_orientation = EulerAngles();
	}
}

//-----------------------------------------------------------------------------------------------
void ChessPlayer::CameraControlsController( float deltaSeconds )
{
	if( m_cameraMode != CameraMode::FREE )
	{
		m_velocity = Vec3();
		return;
	}

	XboxController const& controller = g_engine->m_input->m_controllers[ 0 ];
	Vec3 forwardVector = m_orientation.GetForwardDir_IFwd_JLeft_KUp();
	float speedFactor = 5.f;

	if( controller.GetButton( XboxButtonID::A ).m_isPressed )
	{
		speedFactor *= 10.f;
	}

	// Yaw
	if( g_engine->m_input->m_cursorState.m_cursorMode == CursorMode::FPS )
	{
		m_orientation.m_yawDegrees -= controller.GetRightStick().GetPosition().x * 0.125f;
	}

	// Pitch
	if( g_engine->m_input->m_cursorState.m_cursorMode == CursorMode::FPS )
	{
		m_orientation.m_pitchDegrees -= controller.GetRightStick().GetPosition().y * 0.125f;
		m_orientation.m_pitchDegrees = GetClamped( m_orientation.m_pitchDegrees, -85.f, 85.f );
	}

	// Roll
	if( controller.GetLeftTrigger() )
	{
		m_orientation.m_rollDegrees += 90.f * deltaSeconds;
		m_orientation.m_rollDegrees = GetClamped( m_orientation.m_rollDegrees, -45.f, 45.f );
	}
	if( controller.GetRightTrigger() )
	{
		m_orientation.m_rollDegrees -= 90.f * deltaSeconds;
		m_orientation.m_rollDegrees = GetClamped( m_orientation.m_rollDegrees, -45.f, 45.f );
	}

	// Up and Down
	if( controller.GetButton( XboxButtonID::RIGHT_BUMPER ).m_isPressed )
	{
		m_velocity.z += deltaSeconds * speedFactor;
	}
	if( controller.GetButton( XboxButtonID::LEFT_BUMPER ).m_isPressed )
	{
		m_velocity.z -= deltaSeconds * speedFactor;
	}

	// Left and Right
	if( controller.GetLeftStick().GetPosition().x > 0.5f )
	{
		m_velocity.x += forwardVector.GetRotated90DegreesAboutZ().x * deltaSeconds * speedFactor;
		m_velocity.y += forwardVector.GetRotated90DegreesAboutZ().y * deltaSeconds * speedFactor;

	}
	if( controller.GetLeftStick().GetPosition().x < -0.5f )
	{
		m_velocity.x -= forwardVector.GetRotated90DegreesAboutZ().x * deltaSeconds * speedFactor;
		m_velocity.y -= forwardVector.GetRotated90DegreesAboutZ().y * deltaSeconds * speedFactor;
	}

	// Forward and Back
	if( controller.GetLeftStick().GetPosition().y > 0.5f )
	{
		m_velocity.x += forwardVector.x * deltaSeconds * speedFactor;
		m_velocity.y += forwardVector.y * deltaSeconds * speedFactor;
		m_velocity.z += forwardVector.z * deltaSeconds * speedFactor;
	}
	if( controller.GetLeftStick().GetPosition().y < -0.5f ) 
	{
		m_velocity.x -= forwardVector.x * deltaSeconds * speedFactor;
		m_velocity.y -= forwardVector.y * deltaSeconds * speedFactor;
		m_velocity.z -= forwardVector.z * deltaSeconds * speedFactor;
	}

	if( controller.GetButton( XboxButtonID::START ).m_isPressed && ! controller.GetButton( XboxButtonID::START ).m_wasPressedLastFrame )
	{
		m_position = Vec3();
		m_velocity = Vec3();
		m_orientation = EulerAngles();
	}
}

