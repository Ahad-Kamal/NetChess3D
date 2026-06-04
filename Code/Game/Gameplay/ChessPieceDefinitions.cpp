#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/NamedStrings.hpp"


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

		currentElement = currentElement->NextSiblingElement();
	}
}

//-----------------------------------------------------------------------------------------------
void ChessPieceDefinition::ClearPieceDefs()
{
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
