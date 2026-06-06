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
	
	static IntVec2 ConvertChessCoordToIntCoord( std::string chessCoord );

	static bool Event_ChessMove( EventArgs& args );

private:
	ChessBoard* m_chessBoard;
};