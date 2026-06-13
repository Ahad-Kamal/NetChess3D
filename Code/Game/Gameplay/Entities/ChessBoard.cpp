#include "Game/Gameplay/Entities/ChessBoard.hpp"
#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//-----------------------------------------------------------------------------------------------
ChessBoard::ChessBoard( ChessMatch* owner )
	: m_owner( owner )
{
	m_boardBox = AABB3( Vec3(), Vec3( 8.5f, 8.5f, 1.f ) );
	m_boardBox.Translate( Vec3( -0.25f, -0.25f, -1.10f ) );
	AddVertsForCube( m_vertexes, m_indexes, m_boardBox, Rgba8( 110, 85, 0 ) );

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
				AddVertsForCube( m_vertexes, m_indexes, tile, Rgba8( 242, 242, 242 ) );
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
	for( unsigned int pieceIndex = 0; pieceIndex < m_p1ChessPieces.size(); pieceIndex++ )
	{
		m_p1ChessPieces[ pieceIndex ]->Update();
	}
	for( unsigned int pieceIndex = 0; pieceIndex < m_p2ChessPieces.size(); pieceIndex++ )
	{
		m_p2ChessPieces[ pieceIndex ]->Update();
	}
}

//-----------------------------------------------------------------------------------------------
void ChessBoard::Render() const
{
	Texture* diffuseTexture = g_engine->m_render->CreateOrGetTextureFromFile( "Data/Images/Woodfloor_d.png" );
	Texture* normalTexture = g_engine->m_render->CreateOrGetTextureFromFile( "Data/Images/Woodfloor_n.png" );
	g_engine->m_render->RenderSetup( diffuseTexture );

	g_engine->m_render->BindTexture( normalTexture, 1 );
	Vec3 normalizedLighting = g_game->m_sunDirection.GetNormalized();
	g_engine->m_render->SetLightConstants( normalizedLighting, g_game->m_sunIntensity, g_game->m_ambientIntensity );
	g_engine->m_render->DrawVertexArray( m_vertexes, m_indexes );
	g_engine->m_render->BindTexture( nullptr, 1 );

	for( unsigned int pieceIndex = 0; pieceIndex < m_p1ChessPieces.size(); pieceIndex++ )
	{
		if( m_p1ChessPieces[ pieceIndex ] )
		{
			m_p1ChessPieces[ pieceIndex ]->Render();
		}
	}
	for( unsigned int pieceIndex = 0; pieceIndex < m_p2ChessPieces.size(); pieceIndex++ )
	{
		if( m_p2ChessPieces[ pieceIndex ] )
		{
			m_p2ChessPieces[ pieceIndex ]->Render();
		}
	}
}

//-----------------------------------------------------------------------------------------------
IntVec2 ChessBoard::GetCoordFromIndex( int index ) const
{
	//if( m_piecesOnBoard[ index ] == nullptr )
	//{
	//	return IntVec2( -1, -1 );
	//}

	//return GetCoordFromPosition( Vec2( m_piecesOnBoard[ index ]->m_position ) );
	int yCoord = index / 8;
	int xCoord = index % 8;
	return IntVec2( xCoord, yCoord );
}

//-----------------------------------------------------------------------------------------------
IntVec2 ChessBoard::GetCoordFromPosition( Vec2 position ) const
{
	return IntVec2( RoundDownToInt( position.x ), RoundDownToInt( position.y ) );
}

//-----------------------------------------------------------------------------------------------
int ChessBoard::GetIndexFromCoord( IntVec2 coord ) const
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
Vec2 ChessBoard::GetTileCenterFromCoord( IntVec2 coord ) const
{
	float xPos = static_cast<float>( coord.x ) + 0.5f;
	float yPos = static_cast<float>( coord.y ) + 0.5f;

	return Vec2( xPos, yPos );
}

//-----------------------------------------------------------------------------------------------
void ChessBoard::MovePiece( ChessPiece*& pieceToMove, IntVec2 coord )
{
	int oldTileIndex = GetIndexFromCoord( GetCoordFromPosition( Vec2( pieceToMove->m_position ) ) );
	int newTileIndex = GetIndexFromCoord( coord );

	m_piecesOnBoard[ newTileIndex ] = pieceToMove;
	m_piecesOnBoard[ oldTileIndex ] = nullptr;

	pieceToMove->TranslatePieceToCoord( coord );
	pieceToMove->m_timesMoved++;
}

//-----------------------------------------------------------------------------------------------
void ChessBoard::AddPiece( ChessPiece*& pieceToAdd, IntVec2 coord )
{
	ChessTeam team = pieceToAdd->m_team;
	if( team == TEAM_PLAYER_2 && pieceToAdd->m_definition->GetPieceType() == ChessPieceType::KNIGHT )
	{
		pieceToAdd->RotatePiece( EulerAngles( 180.f, 0.f, 0.f ) );
	}
	pieceToAdd->TranslatePieceToCoord( coord );

	if( team == TEAM_PLAYER_1 )
	{
		m_p1ChessPieces.push_back( pieceToAdd );
	}
	else
	{
		m_p2ChessPieces.push_back( pieceToAdd );
	}

	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = pieceToAdd;
}

//-----------------------------------------------------------------------------------------------
void ChessBoard::RemovePiece( ChessPiece*& pieceToRemove )
{
	int boardIndex = GetIndexFromCoord( GetCoordFromPosition( Vec2( pieceToRemove->m_position ) ) );
	int teamIndex = -1;
	ChessTeam pieceTeam = pieceToRemove->m_team;
	if( pieceTeam == TEAM_PLAYER_1 )
	{
		for( unsigned int pieceIndex = 0; pieceIndex < m_p1ChessPieces.size(); pieceIndex++ )
		{
			if( pieceToRemove == m_p1ChessPieces[ pieceIndex ] )
			{
				teamIndex = pieceIndex;
				break;
			}
		}
	}
	else
	{
		for( unsigned int pieceIndex = 0; pieceIndex < m_p2ChessPieces.size(); pieceIndex++ )
		{
			if( pieceToRemove == m_p2ChessPieces[ pieceIndex ] )
			{
				teamIndex = pieceIndex;
				break;
			}
		}
	}

	delete pieceToRemove;
	pieceToRemove = nullptr;

	m_piecesOnBoard[ boardIndex ] = nullptr;

	// This shouldn't happen, but this catch is here if it does
	if( teamIndex == -1 )
	{
		ERROR_AND_DIE( "Piece could not be removed");
	}

	if( pieceTeam == TEAM_PLAYER_1 )
	{
		//m_p1ChessPieces[ teamIndex ] = nullptr;
		m_p1ChessPieces.erase( m_p1ChessPieces.begin() + teamIndex);
	}
	else
	{
		//m_p2ChessPieces[ teamIndex ] = nullptr;
		m_p2ChessPieces.erase( m_p2ChessPieces.begin() + teamIndex);
	}
}

//-----------------------------------------------------------------------------------------------
bool ChessBoard::CheckForCapturedKing( ChessTeam currentPlayer )
{
	bool isKing = false;
	if( currentPlayer == TEAM_PLAYER_1 )
	{
		for( unsigned int pieceIndex = 0; pieceIndex < m_p2ChessPieces.size(); pieceIndex++ )
		{
			if( m_p2ChessPieces[ pieceIndex ] && m_p2ChessPieces[ pieceIndex ]->m_definition->GetPieceType() == ChessPieceType::KING )
			{
				isKing = true;
				break;
			}
		}
	}
	else
	{
		for( unsigned int pieceIndex = 0; pieceIndex < m_p1ChessPieces.size(); pieceIndex++ )
		{
			if( m_p1ChessPieces[ pieceIndex ] && m_p1ChessPieces[ pieceIndex ]->m_definition->GetPieceType() == ChessPieceType::KING )
			{
				isKing = true;
				break;
			}
		}
	}

	return isKing;
}

//-----------------------------------------------------------------------------------------------
void ChessBoard::SetPiecesOnBoard( std::string const& boardString )
{
	for( int tileIndex = 0; tileIndex < 64; tileIndex++ )
	{
		IntVec2 coord = GetCoordFromIndex( tileIndex );
		ChessPiece* currentPiece = GetPieceAtCoord( coord );

		ChessPieceDefinition* defOfPieceToAdd = ChessPieceDefinition::GetPieceDefFromChar( boardString[ tileIndex ] );
		ChessPieceType pieceTypeToAdd = defOfPieceToAdd->GetPieceType();
		ChessTeam pieceTeamToAdd = ChessPiece::GetTeamFromChar( boardString[ tileIndex ] );

		// Do nothing, cause the piece at this tile is the same as what we would like to add
		if( currentPiece != nullptr && currentPiece->m_team == pieceTeamToAdd && currentPiece->m_definition->GetPieceType() == pieceTypeToAdd )
		{
			continue;
		}
		// Do nothing, cause there is not a piece here and we are not adding one
		if( currentPiece == nullptr && pieceTypeToAdd == ChessPieceType::INVALID )
		{
			continue;
		}

		// Removing a piece on a tile
		if( pieceTypeToAdd == ChessPieceType::INVALID )
		{
			RemovePiece( currentPiece );
			continue;
		}

		// Placing a piece on an empty tile
		if( currentPiece == nullptr )
		{
			ChessPiece* piece = new ChessPiece( defOfPieceToAdd, pieceTeamToAdd, this );
			AddPiece( piece, coord );
			continue;
		}

		// Replacing a piece on a tile with another piece
		ChessPiece* piece = new ChessPiece( defOfPieceToAdd, pieceTeamToAdd, this );
		RemovePiece( currentPiece );
		AddPiece( piece, coord );
	}
}

//-----------------------------------------------------------------------------------------------
void ChessBoard::AddPiecesToBoard()
{
	IntVec2 coord;

	// Add player 1 pieces
	// Pawns
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_1, this );
		coord = IntVec2( pawnIndex, 1 );
		pawn->TranslatePieceToCoord( coord );
		m_p1ChessPieces.push_back( pawn );
		m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = pawn;
	}

	// Rooks
	ChessPiece* rook11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 0, 0 );
	rook11->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( rook11 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook11;

	ChessPiece* rook12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 7, 0 );
	rook12->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( rook12 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook12;

	// Knights
	ChessPiece* knight11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 1, 0 );
	knight11->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( knight11 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight11;

	ChessPiece* knight12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 6, 0 );
	knight12->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( knight12 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight12;

	// Bishops
	ChessPiece* bishop11 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 2, 0 );
	bishop11->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( bishop11 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop11;

	ChessPiece* bishop12 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 5, 0 );
	bishop12->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( bishop12 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop12;

	// Queen
	ChessPiece* queen1 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 4 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 3, 0 );
	queen1->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( queen1 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = queen1;

	// King
	ChessPiece* king1 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 5 ], TEAM_PLAYER_1, this );
	coord = IntVec2( 4, 0 );
	king1->TranslatePieceToCoord( coord );
	m_p1ChessPieces.push_back( king1 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = king1;

	// Add player 2 pieces
	// Pawns
	for( int pawnIndex = 0; pawnIndex < 8; pawnIndex++ )
	{
		ChessPiece* pawn = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 0 ], TEAM_PLAYER_2, this );
		coord = IntVec2( pawnIndex, 6 );
		pawn->TranslatePieceToCoord( coord );
		m_p2ChessPieces.push_back( pawn );
		m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = pawn;
	}

	// Rooks
	ChessPiece* rook21 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_2, this );
	coord = IntVec2( 0, 7 );
	rook21->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( rook21 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook21;

	ChessPiece* rook22 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 1 ], TEAM_PLAYER_2, this );
	coord = IntVec2( 7, 7 );
	rook22->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( rook22 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = rook22;

	// Knights
	ChessPiece* knight21 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_2, this );
	knight21->RotatePiece( EulerAngles( 180.f, 0.f, 0.f ) );
	coord = IntVec2( 1, 7 );
	knight21->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( knight21 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight21;

	ChessPiece* knight22 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 2 ], TEAM_PLAYER_2, this );
	knight22->RotatePiece( EulerAngles( 180.f, 0.f, 0.f ) );
	coord = IntVec2( 6, 7 );
	knight22->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( knight22 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = knight22;

	// Bishops
	ChessPiece* bishop21 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_2, this );
	coord = IntVec2( 2, 7 );
	bishop21->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( bishop21 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop21;

	ChessPiece* bishop22 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 3 ], TEAM_PLAYER_2, this );
	coord = IntVec2( 5, 7 );
	bishop22->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( bishop22 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = bishop22;

	// Queen
	ChessPiece* queen2 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 4 ], TEAM_PLAYER_2, this );
	coord = IntVec2( 3, 7 );
	queen2->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( queen2 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = queen2;

	// King
	ChessPiece* king2 = new ChessPiece( &ChessPieceDefinition::s_pieceDefs[ 5 ], TEAM_PLAYER_2, this );
	coord = IntVec2( 4, 7 );
	king2->TranslatePieceToCoord( coord );
	m_p2ChessPieces.push_back( king2 );
	m_piecesOnBoard[ GetIndexFromCoord( coord ) ] = king2;
}