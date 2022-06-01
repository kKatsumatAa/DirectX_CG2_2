#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include "M4.h"

//����
void Vec4xM4(Vec4& v, const M4& m4);

//���W�A���n
const float pi = 3.141592f;
float AngletoRadi(const float& angle);
float RaditoAngle(const float& radian);
