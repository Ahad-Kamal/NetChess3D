#pragma once
#include "Engine/Core/VertexUtils.hpp"

//-----------------------------------------------------------------------------------------------
enum TeamColor
{
	TEAM_COLOR_INAVLID = -1,
	TEAM_COLOR_WHITE,
	TEAM_COLOR_BLACK
};

//-----------------------------------------------------------------------------------------------
class ChessPiece
{
public:
	ChessPiece();
	~ChessPiece();

	void Update();
	void Render() const;

private:
	VertexList m_vertexes;
	TeamColor m_team = TEAM_COLOR_INAVLID;
};