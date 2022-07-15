#pragma once
#include"Vec3.h"

class M4
{
private:
	

public:
	//�s��
	float m[4][4];

	//�֐�
	M4();
	M4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	M4(const float(*other)[4]);
	/*void Normalize();*/

	//����
	
	void TransposeM4();

	//�s��Z�b�g
	void SetScaleMatrix(const Vec3& v3);
	void SetRotationMatrix(const float& angle, const char& axis);
	void SetTranslationMatrix(const Vec3& v3);

	//
	/*M4& operator=(const float (*other)[4]);*/
	M4 operator*(const M4& other);
	M4& operator*=(const M4& other);
};

Vec3 operator*(const Vec3& v, const M4& m2);


const float normalM[4][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
};

const float papa[4][4] = {
		{1,1,1,1},
		{2,2,2,2},
		{3,3,3,3},
		{4,4,4,4}
};

