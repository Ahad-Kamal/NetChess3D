#pragma once
#include "Game/Gameplay/Entities/ChessBoard.hpp"
#include <vector>


//-----------------------------------------------------------------------------------------------
class ChessPiece;

//-----------------------------------------------------------------------------------------------
class ChessMatch
{
public:
	ChessMatch();
	~ChessMatch();

	void Update();
	void Render() const;

private:
	void PrintBoardState();
	void GetPiecesOnRow( std::string& rowString, int rowNumber );

private:
	ChessBoard* m_chessBoard;
};