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

	XmlElement* currentElement = pieceDefs.FirstChildElement()->FirstChildElement();
	for( int elementIndex = 0; elementIndex < numPieceDefs; elementIndex++ )
	{
		NamedStrings pieceDefBlackboard;
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