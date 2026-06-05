#include "Game/Gameplay/Entities/ChessBoard.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/Engine.hpp"

//-----------------------------------------------------------------------------------------------
ChessBoard::ChessBoard()
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
}

//-----------------------------------------------------------------------------------------------
ChessBoard::~ChessBoard()
{

}

//-----------------------------------------------------------------------------------------------
void ChessBoard::Update()
{

}

//-----------------------------------------------------------------------------------------------
void ChessBoard::Render() const
{
	g_engine->m_render->RenderSetup();
	Vec3 normalizedLighting = g_game->m_sunDirection.GetNormalized();
	g_engine->m_render->SetLightConstants( normalizedLighting, g_game->m_sunIntensity, g_game->m_ambientIntensity );
	g_engine->m_render->DrawVertexArray( m_vertexes, m_indexes );
}