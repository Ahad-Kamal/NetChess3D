#include "Game/Gameplay//Entities/ChessPiece.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Math/Mat44.hpp"


//-----------------------------------------------------------------------------------------------
ChessPiece::ChessPiece( ChessPieceDefinition* definition, Team player, ChessMatch* owner )
	: m_definition( definition )
	, m_team( player )
	, m_owner( owner )
{
	std::vector<ChessGeometry*> geoList = m_definition->GetChessGeometry();

	Rgba8 color;
	if( m_team == TEAM_PLAYER_0 )
	{
		ChessCylinder* cylinderBase = new ChessCylinder();
		cylinderBase->m_radius = 0.35f;
		cylinderBase->m_height = 0.1f;
		m_base = cylinderBase;
		color = Rgba8( 220, 220, 220 );

		AddVertsForCylinder3D( m_vertexes, m_indexes, Vec3(), Vec3( 0.f, 0.f, cylinderBase->m_height ), cylinderBase->m_radius, 32, color );
	}
	else
	{
		ChessAABB3* boxBase = new ChessAABB3();
		boxBase->m_abb3 = AABB3( -0.3f, -0.3f, 0.f, 0.3f, 0.3f, 0.1f );
		m_base = boxBase;
		color = Rgba8( 60, 60, 60 );

		AddVertsForCube( m_vertexes, m_indexes, boxBase->m_abb3, color );
	}

	for( unsigned int geoIndex = 0; geoIndex < geoList.size(); geoIndex++ )
	{
		if( geoList[ geoIndex ]->m_shape == "Sphere" )
		{
			ChessSphere* sphere = static_cast<ChessSphere*>( geoList[ geoIndex ] );
			AddVertsForSphere( m_vertexes, m_indexes, sphere->m_center, sphere->m_radius, 32, 16, color );
		}
		else if( geoList[ geoIndex ]->m_shape == "Cylinder" )
		{
			ChessCylinder* cylinder = static_cast<ChessCylinder*>( geoList[ geoIndex ] );
			Vec3 center = cylinder->m_center;
			AddVertsForCylinder3D( m_vertexes, m_indexes, center, Vec3( center.x, center.y, center.z + cylinder->m_height ), cylinder->m_radius, 32, color );
		}
		else if( geoList[ geoIndex ]->m_shape == "AABB3" )
		{
			ChessAABB3* box = static_cast<ChessAABB3*>( geoList[ geoIndex ] );
			AddVertsForCube( m_vertexes, m_indexes, box->m_abb3, color );
		}
		else if( geoList[ geoIndex ]->m_shape == "OBB3" )
		{
			ChessOBB3* box = static_cast<ChessOBB3*>( geoList[ geoIndex ] );
			AddVertsForOBB3( m_vertexes, m_indexes, box->m_obb3, color );
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
	Vec3 normalizedLighting = g_game->m_sunDirection.GetNormalized();
	g_engine->m_render->SetLightConstants( normalizedLighting, g_game->m_sunIntensity, g_game->m_ambientIntensity );
	g_engine->m_render->DrawVertexArray( m_vertexes, m_indexes, m_definition->GetVertexBuffer(), m_definition->GetIndexBuffer() );
}

//-----------------------------------------------------------------------------------------------
void ChessPiece::TranslatePieceToCoord( IntVec2 coord )
{
	// NOTE: add logic to handle change in z?
	Vec2 newPosition = m_owner->GetTileCenterFromCoord( coord );
	Vec2 translation = newPosition - Vec2( m_position );

	Mat44 translationMatrix;
	translationMatrix.AppendTranslation2D( translation );

	TransformVertexArray3D( m_vertexes, translationMatrix );

	if( m_team == TEAM_PLAYER_0 )
	{
		ChessCylinder* cylinder = static_cast<ChessCylinder*>( m_base );
		cylinder->m_center += Vec3( translation );
	}
	else
	{
		ChessAABB3* box = static_cast<ChessAABB3*>( m_base );
		box->m_abb3.Translate( Vec3( translation ) );
	}
}

//-----------------------------------------------------------------------------------------------
void ChessPiece::RotatePiece( EulerAngles rotationAmt )
{
	Mat44 rotationMatrix;
	rotationMatrix.AppendXRotation( rotationAmt.m_rollDegrees );
	rotationMatrix.AppendYRotation( rotationAmt.m_pitchDegrees );
	rotationMatrix.AppendZRotation( rotationAmt.m_yawDegrees );

	TransformVertexArray3D( m_vertexes, rotationMatrix );
	RotateVertexArrayNormals3D( m_vertexes, rotationMatrix );
}
