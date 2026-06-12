#pragma once
#include "Engine/Core/VertexUtils.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Engine/Math/EulerAngles.hpp"


//-----------------------------------------------------------------------------------------------
class ChessBoard;

//-----------------------------------------------------------------------------------------------
enum ChessTeam
{
	TEAM_INAVLID = -1,
	TEAM_PLAYER_1,
	TEAM_PLAYER_2
};

//-----------------------------------------------------------------------------------------------
class ChessPiece
{
public:
	ChessPiece( ChessPieceDefinition* definition, ChessTeam player, ChessBoard* board );
	~ChessPiece();

	void Update();
	void Render() const;

	void TranslatePieceToCoord( IntVec2 coord );
	void RotatePiece( EulerAngles rotationAmt );
	Mat44 GetModelToWorldTransform() const;

	static ChessTeam GetTeamFromChar( char pieceChar );

public:
	ChessTeam m_team = TEAM_INAVLID;
	Vec3 m_position;
	EulerAngles m_orientation;
	ChessPieceDefinition* m_definition = nullptr;

private:
	VertexList m_vertexes;
	IndexList m_indexes;

	ChessGeometry* m_base = nullptr;

	ChessBoard*	m_board = nullptr;
};