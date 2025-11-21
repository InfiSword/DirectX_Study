#pragma once
#include "Types.h"

struct Vertex
{
	Vec3 Pos;
	//Color color;
	Vec2 uv;
};

// 16바이트 정렬을 위한 더미로 쓰레기값을 넣어줌 ( dummy )
struct TransformData
{
	Vec3 offset;
	float dummy;
};