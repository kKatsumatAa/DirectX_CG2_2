#pragma once
#include"DirectX.h"
#include <wrl.h>
using namespace Microsoft::WRL;

struct PipelineSet
{
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
};

class Sprite
{
private:


public:
	PipelineSet pipelineSet;
		// ���_���C�A�E�g�X�v���C�g�p
	D3D12_INPUT_ELEMENT_DESC inputLayoutSprite[2] = {
	{//xyz���W
	 "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	 D3D12_APPEND_ALIGNED_ELEMENT,
	 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}, // (1�s�ŏ������ق������₷��)

	{//uv���W
	 "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
	 D3D12_APPEND_ALIGNED_ELEMENT,
	 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	} // (1�s�ŏ������ق������₷��)
	};

	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv;//uv���W
	};

};

