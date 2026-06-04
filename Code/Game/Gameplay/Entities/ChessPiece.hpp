#pragma once
#include "Engine/Core/VertexUtils.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"


//-----------------------------------------------------------------------------------------------
enum Team
{
	TEAM_INAVLID = -1,
	TEAM_PLAYER_0,
	TEAM_PLAYER_1
};

//-----------------------------------------------------------------------------------------------
class ChessPiece
{
public:
	ChessPiece( ChessPieceDefinition* definition, Team player );
	~ChessPiece();

	void Update();
	void Render() const;

	

private:
	VertexList m_vertexes;
	IndexList m_indexes;

	Team m_team = TEAM_INAVLID;
	Vec3 m_position;
	ChessGeometry* m_base = nullptr;

	ChessPieceDefinition* m_definition = nullptr;
};