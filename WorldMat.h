#pragma once
#include <DirectXMath.h>
#include "Vec3.h"
using namespace DirectX;

class WorldMat
{
private:
	void SetScale();
	void SetRot();
	void SetTrans();
	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;

public:
	XMMATRIX matWorld;
	Vec3 scale = { 1.0f,1.0f,1.0f };
	Vec3 rot = { 0,0,0 };
	Vec3 trans = { 0,0,0 };


	//
	WorldMat();
	void SetWorld();
};