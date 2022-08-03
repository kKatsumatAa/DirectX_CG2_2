#pragma once
#include"ConstBuffTransform.h"
#include"DirectX.h"
#include <wrl.h>
using namespace Microsoft::WRL;

struct PipelineSet
{
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
};
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;//�F(RGBA)
};
struct SpriteBuff
{
	//���_�o�b�t�@
	ComPtr < ID3D12Resource> vertBuff = nullptr;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
		//�萔�o�b�t�@�̐���
};
struct VertexSprite
{
	XMFLOAT3 pos;//xyz���W
	XMFLOAT2 uv;//uv���W
};

class Sprite
{
private:
	

public:
	VertexSprite vertices[4] = {
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}}
	};
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@�̐���
	ComPtr < ID3D12Resource> constBuffMaterial = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	ConstBuffTransform cbt;
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

	

	void CreateSprite(D3D12_RESOURCE_DESC resDesc);
	void SpriteCommonBeginDraw();
	void SpriteDraw();
};

void ResourceProperties(D3D12_RESOURCE_DESC& resDesc, const UINT& size);

void BuffProperties(D3D12_HEAP_PROPERTIES& heap, D3D12_RESOURCE_DESC& resource,
	ID3D12Resource** buff);