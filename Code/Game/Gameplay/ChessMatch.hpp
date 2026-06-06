#pragma once
#include "Game/Gameplay/Entities/ChessBoard.hpp"
#include "Engine/Core/EventSystem.hpp"
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

	static bool Event_ChessMove( EventArgs& args );

private:
	ChessBoard* m_chessBoard;
};