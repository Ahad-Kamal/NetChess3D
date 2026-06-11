#pragma once
#include "Game/Gameplay/Entities/ChessBoard.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"
#include "Engine/Core/EventSystem.hpp"
#include <vector>


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

	static IntVec2 ConvertChessCoordToIntCoord( std::string const& chessCoord );
	static bool IsChessOverrideValid( std::string const& boardString );

	static bool Event_ChessMove( EventArgs& args );
	static bool Event_ChessOverride( EventArgs& args );

public:
	ChessTeam m_currentPlayerTurn = TEAM_PLAYER_1;

private:
	ChessBoard* m_chessBoard;
};