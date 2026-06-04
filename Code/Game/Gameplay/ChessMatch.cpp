#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"

//-----------------------------------------------------------------------------------------------
ChessMatch::ChessMatch()
{
	ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_0 );
	m_chessPieces.push_back( pawn );
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