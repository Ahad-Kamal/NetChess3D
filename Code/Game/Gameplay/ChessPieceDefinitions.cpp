#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Vertex.hpp"


//-----------------------------------------------------------------------------------------------
std::vector<ChessPieceDefinition> ChessPieceDefinition::s_pieceDefs;

//-----------------------------------------------------------------------------------------------
void ChessPieceDefinition::InitializePieceDefs()
{
	// Read in ActorDefinitions.xml
	XmlDocument pieceDefs;
	pieceDefs.LoadFile( "Data/Definitions/PieceDefinitions.xml" );
	int numPieceDefs = pieceDefs.FirstChildElement()->ChildElementCount();
	s_pieceDefs.resize( numPieceDefs );

	XmlElement* currentElement = pieceDefs.FirstChildElement()->FirstChildElement();
	for( int elementIndex = 0; elementIndex < numPieceDefs; elementIndex++ )
	{
		NamedStrings pieceDefBlackboard;
		pieceDefBlackboard.PopulateFromXmlElementAttributes( *currentElement );
		ChessPieceDefinition& currentPieceDef = s_pieceDefs[ elementIndex ];

		std::string name = pieceDefBlackboard.GetValue( "name", "" );
		if( name == "Pawn" )
		{
			currentPieceDef.m_pieceType = ChessPieceType::PAWN;
		}
		else if( name == "Rook" )
		{
			currentPieceDef.m_pieceType = ChessPieceType::ROOK;
		}
		else if( name == "Knight" )
		{
			currentPieceDef.m_pieceType = ChessPieceType::KNIGHT;
		}
		else if( name == "Bishop" )
		{
			currentPieceDef.m_pieceType = ChessPieceType::BISHOP;
		}
		else if( name == "Queen" )
		{
			currentPieceDef.m_pieceType = ChessPieceType::QUEEN;
		}
		else if( name == "King" )
		{
			currentPieceDef.m_pieceType = ChessPieceType::KING;
		}

		XmlElement* geoElement = currentElement->FirstChildElement();
		while( geoElement )
		{
			NamedStrings geoBlackboard;
			geoBlackboard.PopulateFromXmlElementAttributes( *geoElement );

			std::string shape = geoBlackboard.GetValue( "shape", "" );

			if( shape == "Sphere" )
			{
				ChessSphere* newSphere = new ChessSphere();

				Vec3 center = geoBlackboard.GetValue( "center", Vec3() );
				float radius = geoBlackboard.GetValue( "radius", 0.f );

				newSphere->m_shape = shape;
				newSphere->m_center = center;
				newSphere->m_radius = radius;

				currentPieceDef.m_geometry.push_back( newSphere );
			}
			else if( shape == "Cylinder" )
			{
				ChessCylinder* newCylinder = new ChessCylinder();

				Vec3 center = geoBlackboard.GetValue( "center", Vec3() );
				float radius = geoBlackboard.GetValue( "radius", 0.f );
				float height = geoBlackboard.GetValue( "height", 0.f );

				newCylinder->m_shape = shape;
				newCylinder->m_center = center;
				newCylinder->m_radius = radius;
				newCylinder->m_height = height;

				currentPieceDef.m_geometry.push_back( newCylinder );
			}
			else if( shape == "AABB3" )
			{
				ChessAABB3* newAABB3 = new ChessAABB3();

				Vec3 mins = geoBlackboard.GetValue( "mins", Vec3() );
				Vec3 maxs = geoBlackboard.GetValue( "maxs", Vec3() );

				newAABB3->m_shape = shape;
				newAABB3->m_abb3 = AABB3( mins, maxs );

				currentPieceDef.m_geometry.push_back( newAABB3 );
			}
			else if( shape == "OBB3" )
			{
				ChessOBB3* newOBB3 = new ChessOBB3();

				Vec3 center = geoBlackboard.GetValue( "center", Vec3() );
				Vec3 iBasis = geoBlackboard.GetValue( "iBasis", Vec3() );
				Vec3 jBasis = geoBlackboard.GetValue( "jBasis", Vec3() );
				Vec3 kBasis = geoBlackboard.GetValue( "kBasis", Vec3() );
				Vec3 halfDimensions = geoBlackboard.GetValue( "halfDimensions", Vec3() );

				newOBB3->m_shape = shape;
				newOBB3->m_obb3 = OBB3( center, iBasis, jBasis, kBasis, halfDimensions );

				currentPieceDef.m_geometry.push_back( newOBB3 );
			}

			geoElement = geoElement->NextSiblingElement();
		}

		// Create Vertex Buffers
		unsigned int vertexSize = sizeof( Vertex );
		currentPieceDef.m_vertexBufferP0 = g_engine->m_render->CreateVertexBuffer( 1, vertexSize );
		currentPieceDef.m_vertexBufferP1 = g_engine->m_render->CreateVertexBuffer( 1, vertexSize );

		// Create Index Buffers
		currentPieceDef.m_indexBufferP0 = g_engine->m_render->CreateIndexBuffer( 1 );
		currentPieceDef.m_indexBufferP1 = g_engine->m_render->CreateIndexBuffer( 1 );

		currentElement = currentElement->NextSiblingElement();
	}
}

//-----------------------------------------------------------------------------------------------
void ChessPieceDefinition::ClearPieceDefs()
{
	for( unsigned int defIndex = 0; defIndex < s_pieceDefs.size(); defIndex++ )
	{
		delete s_pieceDefs[ defIndex ].m_vertexBufferP0;
		s_pieceDefs[ defIndex ].m_vertexBufferP0 = nullptr;

		delete s_pieceDefs[ defIndex ].m_indexBufferP0;
		s_pieceDefs[ defIndex ].m_indexBufferP0 = nullptr;

		delete s_pieceDefs[ defIndex ].m_vertexBufferP1;
		s_pieceDefs[ defIndex ].m_vertexBufferP1 = nullptr;

		delete s_pieceDefs[ defIndex ].m_indexBufferP1;
		s_pieceDefs[ defIndex ].m_indexBufferP1 = nullptr;
	}

	s_pieceDefs.clear();
}

//-----------------------------------------------------------------------------------------------
ChessPieceType ChessPieceDefinition::GetPieceType() const
{
	return m_pieceType;
}

//-----------------------------------------------------------------------------------------------
std::vector<ChessGeometry*> ChessPieceDefinition::GetChessGeometry() const
{
	return m_geometry;
}

//-----------------------------------------------------------------------------------------------
VertexBuffer* ChessPieceDefinition::GetVertexBufferP0() const
{
	return m_vertexBufferP0;
}

IndexBuffer* ChessPieceDefinition::GetIndexBufferP0() const
{
	return m_indexBufferP0;
}

VertexBuffer* ChessPieceDefinition::GetVertexBufferP1() const
{
	return m_vertexBufferP1;
}

IndexBuffer* ChessPieceDefinition::GetIndexBufferP1() const
{
	return m_indexBufferP1;
}
