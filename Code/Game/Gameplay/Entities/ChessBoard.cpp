#include "Game/Gameplay/Entities/ChessBoard.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"
#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Math/MathUtils.hpp"

//-----------------------------------------------------------------------------------------------
ChessBoard::ChessBoard( ChessMatch* owner )
	: m_owner( owner )
{
	m_boardBox = AABB3( Vec3(), Vec3( 8.5f, 8.5f, 1.f ) );
	m_boardBox.Translate( Vec3( -0.25f, -0.25f, -1.10f ) );
	AddVertsForCube( m_vertexes, m_indexes, m_boardBox, Rgba8( 220, 170, 15 ) );

	for( int xTileIndex = 0; xTileIndex < 8; xTileIndex++ )
	{
		for( int yTileIndex = 0; yTileIndex < 8; yTileIndex++ )
		{
			AABB3 tile = AABB3( Vec3(), Vec3( 1.f, 1.f, 0.10f ) );
			tile.Translate( Vec3( static_cast<float>( xTileIndex ), static_cast<float>( yTileIndex ), -0.10f ) );

			if( ( xTileIndex + yTileIndex ) % 2 == 0 )
			{
				AddVertsForCube( m_vertexes, m_indexes, tile, Rgba8( 48, 48, 48 ) );
			}
			else
			{
				AddVertsForCube( m_vertexes, m_indexes, tile, Rgba8( 232, 232, 232 ) );
			}
		}
	}

	m_piecesOnBoard.resize( 64 );
	AddPiecesToBoard();
}

//-----------------------------------------------------------------------------------------------
ChessBoard::~ChessBoard()
{

}

//-----------------------------------------------------------------------------------------------
void ChessBoard::Update()
{
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
void ChessBoard::Render() const
{
	g_engine->m_render->RenderSetup();
	Vec3 normalizedLighting = g_game->m_sunDirection.GetNormalized();
	g_engine->m_render->SetLightConstants( normalizedLighting, g_game->m_sunIntensity, g_game->m_ambientIntensity );
	g_engine->m_render->DrawVertexArray( m_vertexes, m_indexes );

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
int ChessBoard::GetIndexFromCoord( IntVec2 coord )
{
	return ( coord.y * 8 ) + coord.x;
}

//-----------------------------------------------------------------------------------------------
ChessPiece* ChessBoard::GetPieceAtCoord( IntVec2 coord )
{
	int index = ( coord.y * 8 ) + coord.x;
	return m_piecesOnBoard[ index ];
}

//-----------------------------------------------------------------------------------------------
//IntVec2 ChessBoard::GetTileCoordsFromIndex( int index ) const
//{
//	if( m_piecesOnBoard[ index ] == nullptr )
//	{
//		return IntVec2( -1, -1 );
//	}
//
//	return GetCoordFromPosition( Vec2( m_piecesOnBoard[ index ]->m_position ) );
//}

//-----------------------------------------------------------------------------------------------
Vec2 ChessBoard::GetTileCenterFromCoord( IntVec2 coord ) const
{
	float xPos = static_cast<float>( coord.x ) + 0.5f;
	float yPos = static_cast<float>( coord.y ) + 0.5f;

	return Vec2( xPos, yPos );
}

//-----------------------------------------------------------------------------------------------
//IntVec2 ChessBoard::GetCoordFromPosition( Vec2 position ) const
//{
//	return IntVec2( RoundDownToInt( position.x ), RoundDownToInt( position.y ) );
//}

//-----------------------------------------------------------------------------------------------
void ChessBoard::AddPiecesToBoard()
{
	IntVec2 coord;

	// Add player 0 pieces
	// Pawns
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_0, this );
		coord = IntVec2( pawnIndex, 1 );
		pawn->TranslatePieceToCoord( coord );
		m_p0ChessPieces.push_back( pawn );
		m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = pawn;
	}

	// Rooks
	ChessPiece* rook01 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 0, 0 );
	rook01->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( rook01 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook01;

	ChessPiece* rook02 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 7, 0 );
	rook02->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( rook02 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook02;

	// Knights
	ChessPiece* knight01 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 1, 0 );
	knight01->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( knight01 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight01;

	ChessPiece* knight02 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 6, 0 );
	knight02->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( knight02 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight02;

	// Bishops
	ChessPiece* bishop01 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 2, 0 );
	bishop01->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( bishop01 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop01;

	ChessPiece* bishop02 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 5, 0 );
	bishop02->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( bishop02 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop02;

	// Queen
	ChessPiece* queen0 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 4 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 3, 0 );
	queen0->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( queen0 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = queen0;

	// King
	ChessPiece* king0 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 5 ], TEAM_PLAYER_0, this );
	coord = IntVec2( 4, 0 );
	king0->TranslatePieceToCoord( coord );
	m_p0ChessPieces.push_back( king0 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = king0;

	// Add player 1 pieces
	// Pawns
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_1, this );
		coord = IntVec2( pawnIndex, 6 );
		pawn->TranslatePieceToCoord( coord );
		m_p1ChessPieces.push_back( pawn );
		m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = pawn;
	}

	// Rooks
	ChessPiece* rook11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 0, 7 );
	rook11->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( rook11 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook11;

	ChessPiece* rook12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 7, 7 );
	rook12->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( rook12 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook12;

	// Knights
	ChessPiece* knight11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_1, this );
	knight11->RotatePiece( EulerAngles( 180.f, 0.f, 0.f ) );
	coord = IntVec2( 1, 7 );
	knight11->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( knight11 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight11;

	ChessPiece* knight12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_1, this );
	knight12->RotatePiece( EulerAngles( 180.f, 0.f, 0.f ) );
	coord = IntVec2( 6, 7 );
	knight12->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( knight12 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight12;

	// Bishops
	ChessPiece* bishop11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 2, 7 );
	bishop11->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( bishop11 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop11;

	ChessPiece* bishop12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 5, 7 );
	bishop12->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( bishop12 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop12;

	// Queen
	ChessPiece* queen1 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 4 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 3, 7 );
	queen1->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( queen1 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = queen1;

	// King
	ChessPiece* king1 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 5 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 4, 7 );
	king1->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( king1 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = king1;
}