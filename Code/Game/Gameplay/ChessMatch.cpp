#include "Game/Gameplay/ChessMatch.hpp"
#include "Game/Gameplay/ChessPieceDefinitions.hpp"
#include "Game/Gameplay/Game.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Math/EulerAngles.hpp"

//-----------------------------------------------------------------------------------------------
ChessMatch::ChessMatch()
{
	m_chessBoard = new ChessBoard( this );

	PrintBoardState();
	SubscribeEventCallbackFunction( "ChessMove", Event_ChessMove );
	g_engine->m_devConsole->AddValidCommand( "ChessMove" );
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
	if( m_currentPlayerTurn == TEAM_PLAYER_1 )
	{
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, "It is currently player 1 (White)'s turn" );
	}
	else
	{
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, "It is currently player 2 (Black)'s turn" );
	}

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
			if( team == TEAM_PLAYER_1 )
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
			if( team == TEAM_PLAYER_1 )
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
			if( team == TEAM_PLAYER_1 )
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
			if( team == TEAM_PLAYER_1 )
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
			if( team == TEAM_PLAYER_1 )
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
			if( team == TEAM_PLAYER_1 )
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

//-----------------------------------------------------------------------------------------------
IntVec2 ChessMatch::ConvertChessCoordToIntCoord( std::string chessCoord )
{
	char letter = chessCoord[ 0 ];
	int row = atoi( &chessCoord[ 1 ] ) - 1;

	if( letter == 'a' || letter == 'A' )
	{
		return IntVec2( 0, row );
	}
	if( letter == 'b' || letter == 'B' )
	{
		return IntVec2( 1, row );
	}
	if( letter == 'c' || letter == 'C' )
	{
		return IntVec2( 2, row );
	}
	if( letter == 'd' || letter == 'D' )
	{
		return IntVec2( 3, row );
	}
	if( letter == 'e' || letter == 'E' )
	{
		return IntVec2( 4, row );
	}
	if( letter == 'f' || letter == 'F' )
	{
		return IntVec2( 5, row );
	}
	if( letter == 'g' || letter == 'G' )
	{
		return IntVec2( 6, row );
	}
	if( letter == 'h' || letter == 'H' )
	{
		return IntVec2( 7, row );
	}

	return IntVec2();
}

bool ChessMatch::Event_ChessMove( EventArgs& args )
{
	// Number of args check
	if( args.GetNumPairs() < 2 )
	{
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, "Illegal chess move; must have from= and to= arguments!" );
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, " Example: ChessMove from=a2 to a4" );
		return true;
	}

	std::string fromChessCoord = args.GetValue( "from", "" );
	std::string toChessCoord = args.GetValue( "to", "" );

	// Valid coords checks
	// Coords length check
	if( fromChessCoord.size() != 2 )
	{
		std::string errorString = "Illegal \"from=\" square \"" + fromChessCoord + "\"; must be a two-letter [Column][Rank] ";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, " Examples: C2, F5; A1 is bottom left and H8 is top-right" );
		return true;
	}
	if( toChessCoord.size() != 2 )
	{
		std::string errorString = "Illegal \"to=\" square \"" + toChessCoord + "\"; must be a two-letter [Column][Rank] ";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, " Examples: C2, F5; A1 is bottom left and H8 is top-right" );
		return true;
	}
	// Coords letter check
	if( !( fromChessCoord[ 0 ] >= 65 && fromChessCoord[ 0 ] <= 72 ) && !( fromChessCoord[ 0 ] >= 97 && fromChessCoord[ 0 ] <= 104 ) )
	{
		std::string errorString = "Illegal \"from=\" square \"" + fromChessCoord + "\"; must be a two-letter [Column][Rank] ";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, " Examples: C2, F5; A1 is bottom left and H8 is top-right" );
		return true;
	}
	if( !( toChessCoord[ 0 ] >= 65 && toChessCoord[ 0 ] <= 72 ) && !( toChessCoord[ 0 ] >= 97 && toChessCoord[ 0 ] <= 104 ) )
	{
		std::string errorString = "Illegal \"to=\" square \"" + toChessCoord + "\"; must be a two-letter [Column][Rank] ";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, " Examples: C2, F5; A1 is bottom left and H8 is top-right" );
		return true;
	}
	// Coords number check
	if( !( fromChessCoord[ 1 ] >= 49 && fromChessCoord[ 1 ] <= 56 ) )
	{
		std::string errorString = "Illegal \"from=\" square \"" + fromChessCoord + "\"; must be a two-letter [Column][Rank] ";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, " Examples: C2, F5; A1 is bottom left and H8 is top-right" );
		return true;
	}
	if( !( toChessCoord[ 1 ] >= 49 && toChessCoord[ 1 ] <= 56 ) )
	{
		std::string errorString = "Illegal \"to=\" square \"" + toChessCoord + "\"; must be a two-letter [Column][Rank] ";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		g_engine->m_devConsole->AddLine( Rgba8::ORANGE, " Examples: C2, F5; A1 is bottom left and H8 is top-right" );
		return true;
	}
	// Different coords check
	if( fromChessCoord == toChessCoord )
	{
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, "The \"from\" and \"to\" coordinates can't be the same tile!" );
		return true;
	}

	IntVec2 fromCoord = ConvertChessCoordToIntCoord( fromChessCoord );
	IntVec2 toCoord = ConvertChessCoordToIntCoord( toChessCoord );

	ChessMatch* chessMatch = g_game->m_chessMatch;

	ChessPiece* fromPiece = chessMatch->m_chessBoard->GetPieceAtCoord( fromCoord );
	ChessPiece* toPiece = chessMatch->m_chessBoard->GetPieceAtCoord( toCoord );

	// Check for if there is a piece at from coord
	if( fromPiece == nullptr )
	{
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, "There is no piece at " + fromChessCoord + "!" );
		return true;
	}
	// Check for if piece at from coord belongs to the current player
	if( fromPiece->m_team != chessMatch->m_currentPlayerTurn )
	{
		int playerInt = static_cast<int>( chessMatch->m_currentPlayerTurn ) + 1;
		char currentPlayer, nextPlayer;
		if( playerInt == 1 )
		{
			currentPlayer = '1';
			nextPlayer = '2';
		}
		else
		{
			currentPlayer = '2';
			nextPlayer = '1';
		}
		std::string errorString = "The " + fromPiece->m_definition->GetPieceName() + " at " + fromChessCoord + " belongs to player " + currentPlayer + " (White); it is currently player " + nextPlayer + " (Black)'s turn";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		return true;
	}
	// Check if from and to pieces are on different teams
	if( fromPiece->m_team == toPiece->m_team )
	{
		std::string errorString = "Can't move to " + fromChessCoord + ", since it is occupied by your own " + toPiece->m_definition->GetPieceName() + "!";
		g_engine->m_devConsole->AddLine( DevConsole::ERRORS, errorString );
		return true;
	}



	return true;
}
