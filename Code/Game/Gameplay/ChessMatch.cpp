#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"
#include "Engine/Math/EulerAngles.hpp"

//-----------------------------------------------------------------------------------------------
ChessMatch::ChessMatch()
{
	m_chessBoard = new ChessBoard( this );
}

//-----------------------------------------------------------------------------------------------
ChessMatch::~ChessMatch()
{

}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Update()
{
	m_chessBoard->Update();
}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Render() const
{
	m_chessBoard->Render();
}
