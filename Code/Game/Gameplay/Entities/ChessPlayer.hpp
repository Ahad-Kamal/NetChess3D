#pragma once
#include "Game/Gameplay/Entity.hpp"


//-----------------------------------------------------------------------------------------------
enum class CameraMode
{
	POV,
	SPECTATOR,
	FREE
};

//-----------------------------------------------------------------------------------------------
struct EulerAngles;

//-----------------------------------------------------------------------------------------------
class ChessPlayer : public Entity
{
public:
	ChessPlayer( Vec3 const& startingPosition, EulerAngles orientation );
	~ChessPlayer();

	void Update( float deltaSeconds ) override;
	void Render() const override;

	void ToggleCameraMode();
	void TogglePOVSide();
	void CameraControlsKeyboard( float deltaSeconds );
	void CameraControlsController( float deltaSeconds );

private:
	CameraMode m_cameraMode = CameraMode::POV;
};