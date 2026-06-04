#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB3.hpp"
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
struct ChessGeometry
{

};

//-----------------------------------------------------------------------------------------------
struct ChessSphere : public ChessGeometry
{
	Vec3 m_center;
	float m_radius = 0.f;
};

//-----------------------------------------------------------------------------------------------
struct ChessAABB3 : public ChessGeometry
{
	AABB3 m_abb3;
};

//-----------------------------------------------------------------------------------------------
struct ChessCylinder : public ChessGeometry
{
	Vec2 m_center;
	float m_radius = 0.f;
	float m_height = 0.f;
};

//-----------------------------------------------------------------------------------------------
struct ChessOBB3 : public ChessGeometry
{
	OBB3 m_obb3;
};

//-----------------------------------------------------------------------------------------------
class ChessPieceDefinition
{
public:
	static void InitializePieceDefs();
	static void ClearPieceDefs();

	ChessPieceType GetPieceType() const;
	std::vector<ChessGeometry*> GetChessGeometry() const;

public:
	static std::vector<ChessPieceDefinition> s_pieceDefs;

private:
	std::vector<ChessGeometry*> m_geometry;
	ChessPieceType m_pieceType;
};