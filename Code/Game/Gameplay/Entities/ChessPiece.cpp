#include "Game/Gameplay//Entities/ChessPiece.hpp"
#include "Game/Gameplay/Entities/ChessBoard.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Math/Mat44.hpp"


//-----------------------------------------------------------------------------------------------
ChessPiece::ChessPiece( ChessPieceDefinition* definition, ChessTeam player, ChessBoard* board )
	: m_definition( definition )
	, m_team( player )
	, m_board( board )
{
	std::vector<ChessGeometry*> geoList = m_definition->GetChessGeometry();

	if( m_team == TEAM_PLAYER_1 )
	{
		ChessCylinder* cylinderBase = new ChessCylinder();
		cylinderBase->m_radius = 0.35f;
		cylinderBase->m_height = 0.1f;
		m_base = cylinderBase;
		m_color = Rgba8( 255, 25, 25 );

		AddVertsForCylinder3D( m_vertexes, m_indexes, Vec3(), Vec3( 0.f, 0.f, cylinderBase->m_height ), cylinderBase->m_radius, 32, Rgba8::WHITE );
	}
	else
	{
		ChessAABB3* boxBase = new ChessAABB3();
		boxBase->m_abb3 = AABB3( -0.3f, -0.3f, 0.f, 0.3f, 0.3f, 0.1f );
		m_base = boxBase;
		m_color = Rgba8( 0, 100, 255 );

		AddVertsForCube( m_vertexes, m_indexes, boxBase->m_abb3, Rgba8::WHITE  );
	}

	for( unsigned int geoIndex = 0; geoIndex < geoList.size(); geoIndex++ )
	{
		if( geoList[ geoIndex ]->m_shape == "Sphere" )
		{
			ChessSphere* sphere = static_cast<ChessSphere*>( geoList[ geoIndex ] );
			AddVertsForSphere( m_vertexes, m_indexes, sphere->m_center, sphere->m_radius, 32, 16, Rgba8::WHITE );
		}
		else if( geoList[ geoIndex ]->m_shape == "Cylinder" )
		{
			ChessCylinder* cylinder = static_cast<ChessCylinder*>( geoList[ geoIndex ] );
			Vec3 center = cylinder->m_center;
			AddVertsForCylinder3D( m_vertexes, m_indexes, center, Vec3( center.x, center.y, center.z + cylinder->m_height ), cylinder->m_radius, 32, Rgba8::WHITE  );
		}
		else if( geoList[ geoIndex ]->m_shape == "AABB3" )
		{
			ChessAABB3* box = static_cast<ChessAABB3*>( geoList[ geoIndex ] );
			AddVertsForCube( m_vertexes, m_indexes, box->m_abb3, Rgba8::WHITE  );
		}
		else if( geoList[ geoIndex ]->m_shape == "OBB3" )
		{
			ChessOBB3* box = static_cast<ChessOBB3*>( geoList[ geoIndex ] );
			AddVertsForOBB3( m_vertexes, m_indexes, box->m_obb3, Rgba8::WHITE  );
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
	//Texture* texture = g_engine->m_render->CreateOrGetTextureFromFile( "Data/Images/Test_StbiFlippedAndOpenGL.png" );
	Texture* diffuseTexture = g_engine->m_render->CreateOrGetTextureFromFile( "Data/Images/Bricks_d.png" );
	Texture* normalTexture = g_engine->m_render->CreateOrGetTextureFromFile( "Data/Images/Bricks_n.png" );

	g_engine->m_render->RenderSetup( diffuseTexture, BlendMode::OPAQUE, GetModelToWorldTransform(), m_color );
	Vec3 normalizedLighting = g_game->m_sunDirection.GetNormalized();
	g_engine->m_render->SetLightConstants( normalizedLighting, g_game->m_sunIntensity, g_game->m_ambientIntensity );

	g_engine->m_render->BindTexture( normalTexture, 1 );
	if( m_team == TEAM_PLAYER_1 )
	{
		g_engine->m_render->DrawVertexArray( m_vertexes, m_indexes, m_definition->GetVertexBufferP1(), m_definition->GetIndexBufferP1() );
	}
	else
	{
		g_engine->m_render->DrawVertexArray( m_vertexes, m_indexes, m_definition->GetVertexBufferP2(), m_definition->GetIndexBufferP2() );
	}
	g_engine->m_render->BindTexture( nullptr, 1 );
}

//-----------------------------------------------------------------------------------------------
void ChessPiece::TranslatePieceToCoord( IntVec2 coord )
{
	// NOTE: add logic to handle change in z?
	Vec2 newPosition = m_board->GetTileCenterFromCoord( coord );

	// None of this code is necessary now cause of the model matrix now being passed in

	/*Vec2 translation = newPosition - Vec2( m_position );

	Mat44 translationMatrix;
	translationMatrix.AppendTranslation2D( translation );

	TransformVertexArray3D( m_vertexes, translationMatrix );

	if( m_team == TEAM_PLAYER_1 )
	{
		ChessCylinder* cylinder = static_cast<ChessCylinder*>( m_base );
		cylinder->m_center += Vec3( translation );
	}
	else
	{
		ChessAABB3* box = static_cast<ChessAABB3*>( m_base );
		box->m_abb3.Translate( Vec3( translation ) );
	}*/

	m_position = Vec3( newPosition );
}

//-----------------------------------------------------------------------------------------------
void ChessPiece::RotatePiece( EulerAngles rotationAmt )
{
	Mat44 rotationMatrix;
	rotationMatrix.AppendXRotation( rotationAmt.m_rollDegrees );
	rotationMatrix.AppendYRotation( rotationAmt.m_pitchDegrees );
	rotationMatrix.AppendZRotation( rotationAmt.m_yawDegrees );
	m_orientation = rotationMatrix.GetEulerAngles();

	/*TransformVertexArray3D( m_vertexes, rotationMatrix );
	RotateVertexArrayNormals3D( m_vertexes, rotationMatrix );*/
}

//-----------------------------------------------------------------------------------------------
Mat44 ChessPiece::GetModelToWorldTransform() const
{
	Mat44 modelMatrix;
	modelMatrix.AppendTranslation3D( m_position );
	modelMatrix.Append( m_orientation.GetAsMatrix_IFwd_JLeft_KUp() );
	return modelMatrix;
}

//-----------------------------------------------------------------------------------------------
bool ChessPiece::CheckMoveForMe( IntVec2 coordToMoveTo, bool isCapturing /*= false*/, bool isTeleporting /*= false*/ )
{
	IntVec2 currentCoord = m_board->GetCoordFromPosition( Vec2( m_position ) );
	switch( m_definition->GetPieceType() )
	{
		case ChessPieceType::PAWN:
			return CheckMoveForPawn( this, currentCoord, coordToMoveTo, isCapturing, isTeleporting );

		case ChessPieceType::ROOK:
			return CheckMoveForRook( this, currentCoord, coordToMoveTo, isTeleporting );

		case ChessPieceType::KNIGHT:
			return CheckMoveForKnight( this, currentCoord, coordToMoveTo );

		case ChessPieceType::BISHOP:
			return CheckMoveForBishop( this, currentCoord, coordToMoveTo, isTeleporting );

		case ChessPieceType::QUEEN:
			return CheckMoveForQueen( this, currentCoord, coordToMoveTo, isTeleporting );

		case ChessPieceType::KING:
			return CheckMoveForKing( this, currentCoord, coordToMoveTo );

		default:
			return false;
	}
}

//-----------------------------------------------------------------------------------------------
bool ChessPiece::CheckMoveForPawn( ChessPiece* pawn, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isCapturing, bool isTeleporting /*= false*/ )
{
	int xDifference = coordToMoveTo.x - currentCoord.x;
	int yDifference = coordToMoveTo.y - currentCoord.y;

	ChessTeam team = pawn->m_team;

	// Check x movement
	if( xDifference >= 2 || xDifference <= -2 )
	{
		return false;
	}
	// Check y movement
	if( team == TEAM_PLAYER_1 )
	{
		if( yDifference >= 3 )
		{
			return false;
		}
		if( pawn->m_timesMoved != 0 && yDifference == 2 )
		{
			return false;
		}

		// Check if blocked
		if( !isTeleporting )
		{
			ChessPiece* blockingPiece = pawn->m_board->GetPieceAtCoord( IntVec2( currentCoord.x, currentCoord.y + 1 ) );
			if( blockingPiece )
			{
				return false;
			}
		}
	}
	else
	{
		if( yDifference <= -3 )
		{
			return false;
		}
		if( pawn->m_timesMoved != 0 && yDifference == -2 )
		{
			return false;
		}

		// Check if blocked
		if( !isTeleporting )
		{
			ChessPiece* blockingPiece = pawn->m_board->GetPieceAtCoord( IntVec2( currentCoord.x, currentCoord.y - 1 ) );
			if( blockingPiece )
			{
				return false;
			}
		}
	}

	// Check for diagonal
	if( !isCapturing && ( xDifference == 1 || xDifference == -1 ) )
	{
		return false;
	}

	// Check if blocked
	if( isCapturing && xDifference == 0 )
	{
		return false;
	}


	return true;
}

//-----------------------------------------------------------------------------------------------
bool ChessPiece::CheckMoveForRook( ChessPiece* rook, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isTeleporting /*= false*/ )
{
	int xDifference = coordToMoveTo.x - currentCoord.x;
	int yDifference = coordToMoveTo.y - currentCoord.y;

	// Check for Diagonal movement
	if( xDifference != 0 && yDifference != 0 )
	{
		return false;
	}
	if( !isTeleporting )
	{
		if( xDifference > 0 )
		{
			for( int x = 1; x <= xDifference; x++ )
			{
				ChessPiece* blockingPiece = rook->m_board->GetPieceAtCoord( IntVec2( currentCoord.x + x, currentCoord.y ) );
				if( blockingPiece )
				{
					return false;
				}
			}
		}
		else if( xDifference < 0 )
		{
			for( int x = 1; x <= -xDifference; x++ )
			{
				ChessPiece* blockingPiece = rook->m_board->GetPieceAtCoord( IntVec2( currentCoord.x - x, currentCoord.y ) );
				if( blockingPiece )
				{
					return false;
				}
			}
		}
		else if( yDifference > 0 )
		{
			for( int y = 1; y <= yDifference; y++ )
			{
				ChessPiece* blockingPiece = rook->m_board->GetPieceAtCoord( IntVec2( currentCoord.x, currentCoord.y + y ) );
				if( blockingPiece )
				{
					return false;
				}
			}
		}
		else if( yDifference < 0 )
		{
			for( int y = 1; y <= -yDifference; y++ )
			{
				ChessPiece* blockingPiece = rook->m_board->GetPieceAtCoord( IntVec2( currentCoord.x, currentCoord.y - y ) );
				if( blockingPiece )
				{
					return false;
				}
			}
		}
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
bool ChessPiece::CheckMoveForKnight( [[maybe_unused]] ChessPiece* knight, IntVec2 currentCoord, IntVec2 coordToMoveTo )
{
	int xDifference = coordToMoveTo.x - currentCoord.x;
	int yDifference = coordToMoveTo.y - currentCoord.y;

	if( xDifference == 1 && yDifference == 2 )
	{
		return true;
	}
	if( xDifference == 2 && yDifference == 1 )
	{
		return true;
	}
	if( xDifference == 2 && yDifference == -1 )
	{
		return true;
	}
	if( xDifference == 1 && yDifference == -2 )
	{
		return true;
	}
	if( xDifference == -1 && yDifference == -2 )
	{
		return true;
	}
	if( xDifference == -2 && yDifference == -1 )
	{
		return true;
	}
	if( xDifference == -2 && yDifference == 1 )
	{
		return true;
	}
	if( xDifference == -1 && yDifference == 2 )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
bool ChessPiece::CheckMoveForBishop( ChessPiece* bishop, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isTeleporting /*= false*/ )
{
	return false;
}

//-----------------------------------------------------------------------------------------------
bool ChessPiece::CheckMoveForQueen( ChessPiece* queen, IntVec2 currentCoord, IntVec2 coordToMoveTo, bool isTeleporting /*= false*/ )
{
	return false;
}

//-----------------------------------------------------------------------------------------------
bool ChessPiece::CheckMoveForKing( ChessPiece* king, IntVec2 currentCoord, IntVec2 coordToMoveTo )
{
	return false;
}

//-----------------------------------------------------------------------------------------------
ChessTeam ChessPiece::GetTeamFromChar( char pieceChar )
{
	switch( pieceChar )
	{
		case 'p':
			return TEAM_PLAYER_2;

		case 'P':
			return TEAM_PLAYER_1;

		case 'r':
			return TEAM_PLAYER_2;

		case 'R':
			return TEAM_PLAYER_1;

		case 'n':
			return TEAM_PLAYER_2;

		case 'N':
			return TEAM_PLAYER_1;

		case 'b':
			return TEAM_PLAYER_2;

		case 'B':
			return TEAM_PLAYER_1;

		case 'q':
			return TEAM_PLAYER_2;

		case 'Q':
			return TEAM_PLAYER_1;

		case 'k':
			return TEAM_PLAYER_2;

		case 'K':
			return TEAM_PLAYER_1;

		default:
			return TEAM_INAVLID;
	}
}
