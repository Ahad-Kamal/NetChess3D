#pragma once
#include "Game/Gameplay/Entities/ChessBoard.hpp"


//-----------------------------------------------------------------------------------------------
class ChessMatch
{
public:
	ChessMatch();
	~ChessMatch();

	void Update();
	void Render() const;

private:
	ChessBoard m_chessBoard;
};