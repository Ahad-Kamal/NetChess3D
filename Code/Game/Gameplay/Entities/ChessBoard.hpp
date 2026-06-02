#pragma once
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/AABB3.hpp"


//-----------------------------------------------------------------------------------------------
class ChessBoard
{
public:
	ChessBoard();
	~ChessBoard();

	void Update();
	void Render() const;

private:
	VertexList m_vertexes;
	IndexList m_indexes;

	AABB3 m_boardBox;
};