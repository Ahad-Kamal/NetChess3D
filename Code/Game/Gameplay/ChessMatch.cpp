#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"

//-----------------------------------------------------------------------------------------------
ChessMatch::ChessMatch()
{

}

//-----------------------------------------------------------------------------------------------
ChessMatch::~ChessMatch()
{

}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Update()
{
	m_chessBoard.Update();

	for( unsigned int pieceIndex = 0; pieceIndex < m_chessPieces.size(); pieceIndex++ )
	{
		m_chessPieces[ pieceIndex ]->Update();
	}
}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Render() const
{
	m_chessBoard.Render();

	for( unsigned int pieceIndex = 0; pieceIndex < m_chessPieces.size(); pieceIndex++ )
	{
		m_chessPieces[ pieceIndex ]->Render();
	}
}