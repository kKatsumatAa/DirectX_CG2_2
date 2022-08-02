#pragma once
#include"DirectX.h"
#include <wrl.h>
using namespace Microsoft::WRL;

//struct PipelineSet
//{
//	ComPtr<ID3D12PipelineState> pipelineState;
//	ComPtr<ID3D12RootSignature> rootSignature;
//};

class Sprite
{
private:


public:
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g

	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv;//uv���W
	};

};

