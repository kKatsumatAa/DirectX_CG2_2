#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//VSOutput output;//�s�N�Z���V�F�[�_�[�ɓn���l(hlsli�̍\����)
	////output.svpos = pos;
	//output.svpos = mul(mat, pos);//���W�ɍs�����Z
	//output.normal = normal;
	//output.uv = uv;

	//�����o�[�g
	float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);
	float3 lightcolor = float3(1, 1, 1);
	VSOutput output;
	output.svpos = mul(mat, pos);

	output.color.rgb = dot(-lightdir, normal) * 1.5f * lightcolor/*+(0.1,0.1,0.1)*/;
	output.color.a = 1.0f;
	output.uv = uv;

	return output;
}