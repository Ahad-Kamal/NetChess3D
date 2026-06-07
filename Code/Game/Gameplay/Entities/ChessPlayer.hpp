#pragma once
#include "Game/Gameplay/Entity.hpp"


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

	void CameraControlsKeyboard( float deltaSeconds );
	void CameraControlsController( float deltaSeconds );
};