#pragma once
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/AABB3.hpp"


//-----------------------------------------------------------------------------------------------
class ChessPiece;
class ChessMatch;

//-----------------------------------------------------------------------------------------------
class ChessBoard
{
public:
	ChessBoard( ChessMatch* owner );
	~ChessBoard();

	void Update();
	void Render() const;

	Vec2 GetTileCenterFromCoord( IntVec2 coord );

private:
	void AddPiecesToBoard();

protected:
	ChessMatch* m_owner;

private:
	std::vector<ChessPiece*> m_p0ChessPieces;
	std::vector<ChessPiece*> m_p1ChessPieces;

	VertexList m_vertexes;
	IndexList m_indexes;

	AABB3 m_boardBox;
};