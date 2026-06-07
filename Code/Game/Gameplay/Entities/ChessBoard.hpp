#pragma once
#include "Game/Gameplay/Entities/ChessPiece.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/AABB3.hpp"


//-----------------------------------------------------------------------------------------------
class ChessMatch;

//-----------------------------------------------------------------------------------------------
class ChessBoard
{
public:
	ChessBoard( ChessMatch* owner );
	~ChessBoard();

	void Update();
	void Render() const;

	//IntVec2 GetTileCoordsFromIndex( int index ) const;
	IntVec2 GetCoordFromPosition( Vec2 position ) const;
	int GetIndexFromCoord( IntVec2 coord ) const;
	ChessPiece* GetPieceAtCoord( IntVec2 coord );
	Vec2 GetTileCenterFromCoord( IntVec2 coord ) const;

	void MovePiece( ChessPiece*& pieceToMove, IntVec2 coord );
	void RemovePiece( ChessPiece*& pieceToRemove );
	bool CheckForCapturedKing( ChessTeam currentPlayer );

private:
	void AddPiecesToBoard();

protected:
	ChessMatch* m_owner;

private:
	std::vector<ChessPiece*> m_piecesOnBoard;
	std::vector<ChessPiece*> m_p1ChessPieces;
	std::vector<ChessPiece*> m_p2ChessPieces;

	VertexList m_vertexes;
	IndexList m_indexes;

	AABB3 m_boardBox;
};