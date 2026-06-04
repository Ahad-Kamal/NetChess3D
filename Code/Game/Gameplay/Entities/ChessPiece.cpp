#include "Game/Gameplay//Entities/ChessPiece.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Engine/Core/Engine.hpp"


//-----------------------------------------------------------------------------------------------
ChessPiece::ChessPiece( ChessPieceDefinition* definition, Team player )
	: m_definition( definition )
	, m_team( player )
{
	std::vector<ChessGeometry*> geoList = m_definition->GetChessGeometry();

	if( m_team == TEAM_PLAYER_0 )
	{
		ChessCylinder* cylinderBase = new ChessCylinder();
		cylinderBase->m_radius = 0.4f;
		cylinderBase->m_height = 0.1f;
		m_base = cylinderBase;

		AddVertsForCylinder3D( m_vertexes, m_indexes, Vec3(), Vec3( 0.f, 0.f, cylinderBase->m_height ), cylinderBase->m_radius );
	}
	else
	{
		ChessAABB3* boxBase = new ChessAABB3();
		boxBase->m_abb3 = AABB3( -0.4f, -0.4f, -0.4f, 0.4f, 0.4f, 0.4f );
		m_base = boxBase;

		AddVertsForCube( m_vertexes, m_indexes, boxBase->m_abb3 );
	}

	for( unsigned int geoIndex = 0; geoIndex < geoList.size(); geoIndex++ )
	{
		if( geoList[ geoIndex ]->m_shape == "Sphere" )
		{
			ChessSphere* sphere = static_cast<ChessSphere*>( geoList[ geoIndex ] );
			AddVertsForSphere( m_vertexes, m_indexes, sphere->m_center, sphere->m_radius );
		}
		else if( geoList[ geoIndex ]->m_shape == "Cylinder" )
		{
			ChessCylinder* cylinder = static_cast<ChessCylinder*>( geoList[ geoIndex ] );
			Vec3 center = cylinder->m_center;
			AddVertsForCylinder3D( m_vertexes, m_indexes, center, Vec3( center.x, center.y, center.z + cylinder->m_height ), cylinder->m_radius );
		}
		else if( geoList[ geoIndex ]->m_shape == "AABB3" )
		{
			ChessAABB3* box = static_cast<ChessAABB3*>( geoList[ geoIndex ] );
			AddVertsForCube( m_vertexes, m_indexes, box->m_abb3 );
		}
		else if( geoList[ geoIndex ]->m_shape == "OBB3" )
		{
			ChessOBB3* box = static_cast<ChessOBB3*>( geoList[ geoIndex ] );
			AddVertsForOBB3( m_vertexes, m_indexes, box->m_obb3 );
		}
	}
}

//-----------------------------------------------------------------------------------------------
ChessPiece::~ChessPiece()
{
	delete m_base;
	m_base = nullptr;
}

//-----------------------------------------------------------------------------------------------
void ChessPiece::Update()
{

}

//-----------------------------------------------------------------------------------------------
void ChessPiece::Render() const
{
	g_engine->m_render->RenderSetup();
	g_engine->m_render->DrawVertexArray( m_vertexes, m_indexes );
}