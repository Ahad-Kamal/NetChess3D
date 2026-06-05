#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"

//-----------------------------------------------------------------------------------------------
ChessMatch::ChessMatch()
{
	// Add player 0 pieces
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_0, this );
		pawn->TranslatePieceToCoord( IntVec2( pawnIndex, 1 ) );
		m_chessPieces.push_back( pawn );
	}

	// Add player 1 pieces
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_1, this );
		pawn->TranslatePieceToCoord( IntVec2( pawnIndex, 6 ) );
		m_chessPieces.push_back( pawn );
	}
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

//-----------------------------------------------------------------------------------------------
Vec2 ChessMatch::GetTileCenterFromCoord( IntVec2 coord )
{
	float xPos = static_cast<float>( coord.x ) + 0.5f;
	float yPos = static_cast<float>( coord.y ) + 0.5f;

	return Vec2( xPos, yPos );
}
