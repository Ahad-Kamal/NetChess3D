#pragma once
#include <vector>


//-----------------------------------------------------------------------------------------------
enum class ChessPieceType
{
	INVALID = -1,
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

//-----------------------------------------------------------------------------------------------
class ChessPieceDefinition
{
public:
	static void InitializePieceDefs();
	static void ClearPieceDefs();

	ChessPieceType GetPieceType() const;

public:
	static std::vector<ChessPieceDefinition> s_pieceDefs;

private:
	ChessPieceType m_pieceType;
};