#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include "WindowsApp.h"
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "KeyboardInput.h"
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Directx
{
private:
	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[5];
	D3D_FEATURE_LEVEL featureLevel;
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	// �o�b�N�o�b�t�@
	std::vector<ID3D12Resource*> backBuffers;
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};
	// �t�F���X�̐���
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

public:
	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	// 2.�`���̕ύX
		// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;



	//�֐�
	Directx(const WindowsApp& win);

	void DrawInitialize(const WindowsApp& win);

	void DrawUpdate(const XMFLOAT4& winRGBA = { 0.1f,0.25f,0.5f,0.0f });

	void DrawUpdate2();

	void GraphicsCommand(const WindowsApp& win, const int& pipelineNum,
		const bool& primitiveMode);


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