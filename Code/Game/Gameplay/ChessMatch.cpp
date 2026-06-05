#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"
#include "Engine/Math/EulerAngles.hpp"

//-----------------------------------------------------------------------------------------------
ChessMatch::ChessMatch()
{
	AddPiecesToBoard();
}

//-----------------------------------------------------------------------------------------------
ChessMatch::~ChessMatch()
{

}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Update()
{
	m_chessBoard.Update();

	for( unsigned int pieceIndex = 0; pieceIndex < m_p0ChessPieces.size(); pieceIndex++ )
	{
		m_p0ChessPieces[ pieceIndex ]->Update();
	}
	for( unsigned int pieceIndex = 0; pieceIndex < m_p1ChessPieces.size(); pieceIndex++ )
	{
		m_p1ChessPieces[ pieceIndex ]->Update();
	}
}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Render() const
{
	m_chessBoard.Render();

	for( unsigned int pieceIndex = 0; pieceIndex < m_p0ChessPieces.size(); pieceIndex++ )
	{
		m_p0ChessPieces[ pieceIndex ]->Render();
	}
	for( unsigned int pieceIndex = 0; pieceIndex < m_p1ChessPieces.size(); pieceIndex++ )
	{
		m_p1ChessPieces[ pieceIndex ]->Render();
	}
}

//-----------------------------------------------------------------------------------------------
Vec2 ChessMatch::GetTileCenterFromCoord( IntVec2 coord )
{
	float xPos = static_cast<float>( coord.x ) + 0.5f;
	float yPos = static_cast<float>( coord.y ) + 0.5f;

	return Vec2( xPos, yPos );
}

//-----------------------------------------------------------------------------------------------
void ChessMatch::AddPiecesToBoard()
{
	// Add player 0 pieces
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_0, this );
		pawn->TranslatePieceToCoord( IntVec2( pawnIndex, 1 ) );
		m_p0ChessPieces.push_back( pawn );
	}

	ChessPiece* rook01 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_0, this );
	rook01->TranslatePieceToCoord( IntVec2( 0, 0 ) );
	m_p0ChessPieces.push_back( rook01 );

	ChessPiece* rook02 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_0, this );
	rook02->TranslatePieceToCoord( IntVec2( 7, 0 ) );
	m_p0ChessPieces.push_back( rook02 );

	ChessPiece* knight01 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_0, this );
	knight01->TranslatePieceToCoord( IntVec2( 1, 0 ) );
	m_p0ChessPieces.push_back( knight01 );

	ChessPiece* knight02 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_0, this );
	knight02->TranslatePieceToCoord( IntVec2( 6, 0 ) );
	m_p0ChessPieces.push_back( knight02 );

	// Add player 1 pieces
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_1, this );
		pawn->TranslatePieceToCoord( IntVec2( pawnIndex, 6 ) );
		m_p1ChessPieces.push_back( pawn );
	}

	ChessPiece* rook11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_1, this );
	rook11->TranslatePieceToCoord( IntVec2( 0, 7 ) );
	m_p1ChessPieces.push_back( rook11 );

	ChessPiece* rook12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_1, this );
	rook12->TranslatePieceToCoord( IntVec2( 7, 7 ) );
	m_p1ChessPieces.push_back( rook12 );

	ChessPiece* knight11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_1, this );
	knight11->RotatePiece( EulerAngles( 180.f, 0.f, 0.f ) );
	knight11->TranslatePieceToCoord( IntVec2( 1, 7 ) );
	m_p1ChessPieces.push_back( knight11 );

	ChessPiece* knight12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_1, this );
	knight12->RotatePiece( EulerAngles( 180.f, 0.f, 0.f ) );
	knight12->TranslatePieceToCoord( IntVec2( 6, 7 ) );
	m_p1ChessPieces.push_back( knight12 );
}
