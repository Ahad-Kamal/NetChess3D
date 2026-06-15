#pragma once
#include "Engine/Core/VertexUtils.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Engine/Math/EulerAngles.hpp"


//-----------------------------------------------------------------------------------------------
class ChessBoard;
class Timer;

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
	void SetPieceAtCoord( IntVec2 coord );
	void RotatePiece( EulerAngles rotationAmt );
	Mat44 GetModelToWorldTransform() const;

	bool CheckMoveForMe( IntVec2 coordToMoveTo, bool isCapturing = false, bool isTeleporting = false );

	static bool CheckMoveForPawn( ChessPiece* pawn, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isCapturing, bool isTeleporting = false  );
	static bool CheckMoveForRook( ChessPiece* rook, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isTeleporting = false );
	static bool CheckMoveForKnight( ChessPiece* knight, IntVec2 currentCoord, IntVec2 coordToMoveTo );
	static bool CheckMoveForBishop( ChessPiece* bishop, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isTeleporting = false );
	static bool CheckMoveForQueen( ChessPiece* queen, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isTeleporting = false );
	static bool CheckMoveForKing( ChessPiece* king, IntVec2 currentCoord, IntVec2 coordToMoveTo );

	static ChessTeam GetTeamFromChar( char pieceChar );

public:
	ChessTeam m_team = TEAM_INAVLID;
	unsigned char m_timesMoved = 0;
	unsigned char m_turnLastMoved = 0;
	bool m_isMoving = false;

	Vec3 m_position;
	Vec3 m_prevPosition;
	Vec3 m_nextPosition;
	EulerAngles m_orientation;

	Rgba8 m_color;
	ChessPieceDefinition* m_definition = nullptr;
	Timer* m_moveTimer = nullptr;

private:
	VertexList m_vertexes;
	IndexList m_indexes;

	ChessGeometry* m_base = nullptr;
	ChessBoard*	m_board = nullptr;
};