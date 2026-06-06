#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Entities/ChessPiece.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/StringUtils.hpp"

//-----------------------------------------------------------------------------------------------
ChessMatch::ChessMatch()
{
	m_chessBoard = new ChessBoard( this );

	PrintBoardState();
}

//-----------------------------------------------------------------------------------------------
ChessMatch::~ChessMatch()
{

}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Update()
{
	m_chessBoard->Update();
}

//-----------------------------------------------------------------------------------------------
void ChessMatch::Render() const
{
	m_chessBoard->Render();
}

//-----------------------------------------------------------------------------------------------
void ChessMatch::PrintBoardState()
{
	g_engine->m_devConsole->AddLine( Rgba8::ORANGE, "=====================" );

	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, "  ABCDEFGH  " );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " +--------+ " );

	for( int rowIndex = 8; rowIndex >= 1; rowIndex-- )
	{
		std::string rowString = Stringf( "%d|        |%d", rowIndex, rowIndex );
		GetPiecesOnRow( rowString, rowIndex - 1 );
		g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, rowString );
	}

	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, " +--------+ " );
	g_engine->m_devConsole->AddLine( DevConsole::INFO_MINOR, "  ABCDEFGH  " );
}

//-----------------------------------------------------------------------------------------------
void ChessMatch::GetPiecesOnRow( std::string& rowString, int rowNumber )
{
	for( int tileIndex = 0; tileIndex < 8; tileIndex++ )
	{
		ChessPiece* piece = m_chessBoard->GetPieceAtCoord( IntVec2( tileIndex, rowNumber ) );

		if( piece == nullptr )
		{
			rowString[ tileIndex + 2 ] = '.';
			continue;
		}

		ChessPieceType type = piece->m_definition->GetPieceType();
		ChessTeam team = piece->m_team;
		if( type == ChessPieceType::PAWN )
		{
			if( team == TEAM_PLAYER_0 )
			{
				rowString[ tileIndex + 2 ] = 'P';
			}
			else
			{
				rowString[ tileIndex + 2 ] = 'p';
			}
		}
		else if( type == ChessPieceType::ROOK )
		{
			if( team == TEAM_PLAYER_0 )
			{
				rowString[ tileIndex + 2 ] = 'R';
			}
			else
			{
				rowString[ tileIndex + 2 ] = 'r';
			}
		}
		else if( type == ChessPieceType::KNIGHT )
		{
			if( team == TEAM_PLAYER_0 )
			{
				rowString[ tileIndex + 2 ] = 'N';
			}
			else
			{
				rowString[ tileIndex + 2 ] = 'n';
			}
		}
		else if( type == ChessPieceType::BISHOP )
		{
			if( team == TEAM_PLAYER_0 )
			{
				rowString[ tileIndex + 2 ] = 'B';
			}
			else
			{
				rowString[ tileIndex + 2 ] = 'b';
			}
		}
		else if( type == ChessPieceType::QUEEN )
		{
			if( team == TEAM_PLAYER_0 )
			{
				rowString[ tileIndex + 2 ] = 'Q';
			}
			else
			{
				rowString[ tileIndex + 2 ] = 'q';
			}
		}
		else if( type == ChessPieceType::KING )
		{
			if( team == TEAM_PLAYER_0 )
			{
				rowString[ tileIndex + 2 ] = 'K';
			}
			else
			{
				rowString[ tileIndex + 2 ] = 'k';
			}
		}
	}
}
