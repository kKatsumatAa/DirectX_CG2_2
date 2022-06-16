#pragma once

#include "DirectX.h"
#include "WorldMat.h"
#include "ViewMat.h"
#include "ProjectionMat.h"
#include "Util.h"
#include "ConstBuffTransform.h"

/// <summary>
/// ���_�C���f�b�N�X�p
/// </summary>
static enum indices
{
	TRIANGLE,
	BOX,
	CUBE
};


class Draw
{
private:
	ConstBuffTransform cbt;
	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState[2] = { nullptr };
	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//04_01
		//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;   //xyz���W
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;    //uv���W
	};
	//���_�f�[�^
	Vertex vertices[24] = {
		//��O
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},//����
		{{-5.0f,5.0f, -5.0f},{},{0.0f,0.0f}},//����
		{{5.0f,-5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{5.0f,5.0f,  -5.0f},{},{1.0f,0.0f}},//�E��
		//��
		{{-5.0f,-5.0f,5.0f},{},{0.0f,1.0f}},//����
		{{-5.0f,5.0f, 5.0f},{},{0.0f,0.0f}},//����
		{{5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}},//�E��
		{{5.0f,5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
		//��
		{{5.0f,5.0f,-5.0f},{},{0.0f,1.0f}},//����
		{{5.0f,5.0f, 5.0f},{},{0.0f,0.0f}},//����
		{{-5.0f,5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{-5.0f,5.0f, 5.0f},{},{1.0f,0.0f}},//�E��
		//��
		{{5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},//����
		{{5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},//����
		{{-5.0f,-5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{-5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}},//�E��
		//��
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},//����
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},//����
		{{-5.0f,5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{-5.0f,5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
		//�E
		{{5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},//����
		{{5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},//����
		{{5.0f,5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{5.0f,5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
	};
	unsigned short indices[6] =
	{
		0,1,2,//�O�p�`1��
		2,1,3,//�O�p�`2��
	};
	unsigned short indices2[3] =
	{
		0,1,2//�O�p�`2��
	};
	unsigned short indicesCube[36] =
	{
		//�O
		0,1,2,//�O�p�`1��
		2,1,3,//�O�p�`2��
		//��
		6,5,4,//�O�p�`1��
		7,5,6,//�O�p�`2��
		//��
		10,9,8,//�O�p�`1��
		11,9,10,//�O�p�`2��
		//��
		12,13,14,//�O�p�`1��
		14,13,15,//�O�p�`2��
		//��
		16,17,18,//�O�p�`1��
		18,17,19,//�O�p�`2��
		//�E
		22,21,20,//�O�p�`1��
		23,21,22,//�O�p�`2��
	};
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
	{//xyz���W
	 "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	 D3D12_APPEND_ALIGNED_ELEMENT,
	 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}, // (1�s�ŏ������ق������₷��)

		{//�@���x�N�g��
	 "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	 D3D12_APPEND_ALIGNED_ELEMENT,
	 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}, // (1�s�ŏ������ق������₷��)

	{//uv���W
	 "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
	 D3D12_APPEND_ALIGNED_ELEMENT,
	 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	} // (1�s�ŏ������ق������₷��)
	};
	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@�̐���
	ID3D12Resource* constBuffMaterial = nullptr;
	//�萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;//�F(RGBA)
	};
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	XMFLOAT4 color2 = { 0,0,0,0 };
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};


	// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	
	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	HRESULT result;
	ID3D12Device* device = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	//04_02
		//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle2;
	int count2=0;


private:
	//--------------------
	void Update(const int& indexNum, const int& pipelineNum, const UINT64 textureHandle, const ConstBuffTransform& constBuffTransform,
		const bool& primitiveMode= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
public:
	

	//
	Draw();

	void DrawTriangle(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3,
		const WorldMat world, const ViewMat view, const ProjectionMat projection, const UINT64 textureHandle, const int& pipelineNum=0);
	void DrawBox(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4,
		const WorldMat world, const ViewMat view, const ProjectionMat projection, const UINT64 textureHandle, const int& pipelineNum=0);
	void DrawBoxSprite(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4, const UINT64 textureHandle, const int& pipelineNum = 0);
	void DrawCube3D(const WorldMat world, const ViewMat view, const ProjectionMat projection,
		const UINT64 textureHandle, const int& pipelineNum = 0);
	

	void PipeLineState(const D3D12_FILL_MODE& fillMode, ID3D12PipelineState** pipelineState);

	void Blend(const D3D12_BLEND_OP& blendMode,
		const bool& Inversion = 0, const bool& Translucent = 0);

	void constBuffTransfer(const XMFLOAT4& plusRGBA);

	void ResourceProperties(D3D12_RESOURCE_DESC& resDesc, const UINT& size);

	void BuffProperties(D3D12_HEAP_PROPERTIES& heap, D3D12_RESOURCE_DESC& resource,
		ID3D12Resource** buff);

	void SetNormDigitalMat(XMMATRIX& mat);

	void Error(const bool& filed);
};

void LoadGraph(const wchar_t* name, UINT64& textureHandle);