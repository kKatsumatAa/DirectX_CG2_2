#pragma once

#include "DirectX.h"


class Draw
{
private:
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
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv;//uv���W
	};
	//���_�f�[�^
	Vertex vertices[4] = {
		{{-50.0f,-50.0f,0.0f},{0.0f,1.0f}},//����
		{{-50.0f,50.0f, 0.0f},{0.0f,0.0f}},//����
		{{50.0f,-50.0f, 0.0f},{1.0f,1.0f}},//�E��
		{{50.0f,50.0f,  0.0f},{1.0f,0.0f}},//�E��
	};
	unsigned short indices[6] =
	{
		0,1,2,//�O�p�`1��
		1,2,3,//�O�p�`2��
	};
	unsigned short indices2[3] =
	{
		0,1,2//�O�p�`2��
	};
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[2] = {
	{//xyz���W
	"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}, // (1�s�ŏ������ق������₷��)

	{//uv���W
	"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}, // (1�s�ŏ������ق������₷��)
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

	//�萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;//3D�ϊ��s��
	};
	//05_02
	ID3D12Resource* constBuffTransform = nullptr;//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	KeyboardInput key;

	// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	Directx directx;
	WindowsApp win;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;
	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	srvHeapDesc.NumDescriptors = kMaxSRVCount,
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE//�V�F�[�_�[���猩����悤��
	};
	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;


	//--------------------
	void Update( unsigned short* indices,  const int& pipelineNum,
		const bool& primitiveMode= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
public:
	ConstBufferDataTransform* constMapTransform = nullptr;//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^

	//
	Draw(const WindowsApp& win, Directx& directx);

	void DrawTriangle(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, const int& pipelineNum=0);
	void DrawBox(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4, const int& pipelineNum=0);
	void DrawBoxSprite(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4, const int& pipelineNum = 0);
	void LoadGraph(const wchar_t* name);

	void PipeLineState(const D3D12_FILL_MODE& fillMode, ID3D12PipelineState** pipelineState);

	void Blend(const D3D12_BLEND_OP& blendMode,
		const bool& Inversion = 0, const bool& Translucent = 0);

	void constBuffTransfer(const XMFLOAT4& plusRGBA);

	void ResourceProperties(D3D12_RESOURCE_DESC& resDesc, const UINT& size);

	void BuffProperties(D3D12_HEAP_PROPERTIES& heap, D3D12_RESOURCE_DESC& resource,
		ID3D12Resource** buff);

	void SetNormDigitalMat(XMMATRIX& mat, const WindowsApp& win);

	void Error(const bool& filed);
};