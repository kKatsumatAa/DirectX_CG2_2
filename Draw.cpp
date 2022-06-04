#include "Draw.h"

//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
ID3D12DescriptorHeap* srvHeap = nullptr;
D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

Draw::Draw(const WindowsApp& win, Directx& directx):
	directx(directx),win(win)
{
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//���_�o�b�t�@�̐ݒ�		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p
	
	ResourceProperties(resDesc, sizeVB);

	//���_�o�b�t�@�̐���
	BuffProperties(heapProp, resDesc, &vertBuff);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);


	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	directx.result = D3DCompileFromFile(
		L"BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	Error(FAILED(directx.result));

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	directx.result = D3DCompileFromFile(
		L"BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	Error(FAILED(directx.result));

	//04_02
	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;      //��x�̕`��Ɏg���e�N�X�`���̖���
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;  //�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//���[�g�p�����[�^�̐ݒ�
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�(b0)
	rootParams[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//�f�X�N���v�^
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;//�V��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�(b1)
	rootParams[2].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����


	// �p�C�v�����X�e�[�g�̐���
	PipeLineState(D3D12_FILL_MODE_SOLID, pipelineState);

	PipeLineState(D3D12_FILL_MODE_WIREFRAME, pipelineState + 1);

	//03_02
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	ResourceProperties(cbResourceDesc,
		((UINT)sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff/*256�o�C�g�A���C�����g*/);
	//�萔�o�b�t�@�̐���
	BuffProperties(cbHeapProp, cbResourceDesc, &constBuffMaterial);
	//�萔�o�b�t�@�̃}�b�s���O
	directx.result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(directx.result));
	constBuffTransfer({ 1.0f,0,0,1.0f });

	//03_04
	//�C���f�b�N�X�f�[�^
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//���\�[�X�ݒ�
	ResourceProperties(resDesc, sizeIB);
	//�C���f�b�N�X�o�b�t�@�̍쐬
	ID3D12Resource* indexBuff = nullptr;//GPU���̃�����
	BuffProperties(heapProp, resDesc, &indexBuff);
	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	directx.result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];//�C���f�b�N�X���R�s�[
	}
	//�}�b�s���O������
	indexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//04_02
	////�������s�N�Z����
	//const size_t textureWidth = 256;
	////�c�����s�N�Z����
	//const size_t textureHeight = 256;
	////�z��̗v�f��
	//const size_t imageDataCount = textureWidth * textureHeight;
	////�摜�C���[�W�f�[�^�z��
	//XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];//���ƂŕK������I�I
	////�S�s�N�Z���̐F��������
	//for (size_t i = 0; i < imageDataCount; i++)
	//{
	//	imageData[i].x = 1.0f;//R
	//	imageData[i].y = 0.0f;//G
	//	imageData[i].z = 0.0f;//B
	//	imageData[i].w = 1.0f;//A
	//}

	


	//05_02
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		ResourceProperties(cbResourceDesc, (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff/*256�o�C�g�A���C�����g*/);
		//�萔�o�b�t�@�̐���
		BuffProperties(cbHeapProp, cbResourceDesc, &constBuffTransform);
	}
	//�萔�o�b�t�@�̃}�b�s���O
	directx.result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//�}�b�s���O
	assert(SUCCEEDED(directx.result));
	//�P�ʍs�����
	//SetNormDigitalMat(constMapTransform->mat, win);

	////05_03
	////���s���e�ϊ��i�X�v���C�g�`��?�j
	//constMapTransform->mat =
	//	XMMatrixOrthographicOffCenterLH(0.0, win.window_width, win.window_height, 0.0, 0.0f, 1.0f);


	////05_04
	//ViewMat view ( 0.0f, 0.0f, -100.0f
	//	, 0.0f, 0.0f, 0.0f
	//	, 0.f, 1.f, 0.f );

	////�萔�o�b�t�@�ɓ]��
	//constMapTransform->mat = view.matView * matProjection;

	//05_05
	/*XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	constMapTransform=matWorld**/
}

void Draw::Update( unsigned short* indices, const int& pipelineNum,
	const bool& primitiveMode)
{
	//constBuffTransfer({ -0.001f,0.001f,0,0 });

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	directx.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(directx.result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(this->vertices); i++) {
		vertMap[i] = this->vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// �r���[�|�[�g�ݒ�R�}���h
	viewport = { 0, 0, win.window_width, win.window_height, 0.0f, 1.0f };
	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	directx.commandList->RSSetViewports(1, &viewport);

	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + win.window_width; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + win.window_height; // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	directx.commandList->RSSetScissorRects(1, &scissorRect);

	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	directx.commandList->SetPipelineState(pipelineState[pipelineNum]);
	directx.commandList->SetGraphicsRootSignature(rootSignature);

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	directx.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p���l�p

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directx.commandList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directx.commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//04_02
	//SRV�q�[�v�̐ݒ�R�}���h
	directx.commandList->SetDescriptorHeaps(1, &srvHeap);
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	directx.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directx.commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	directx.commandList->IASetIndexBuffer(&ibView);

	// �`��R�}���h
	directx.commandList->DrawIndexedInstanced(sizeof(indices), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void Draw::DrawTriangle(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, const int& pipelineNum)
{
	vertices[0] = { pos1,{0.0f,1.0f} };//����
	vertices[1] = { pos2,{0.5f,0.0f} };//��
	vertices[2] = { pos3,{1.0f,1.0f} };//�E��
	vertices[3] = vertices[1];//�E��
	
	Update(indices, pipelineNum);
}

void Draw::DrawBox(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4, const int& pipelineNum)
{
	vertices[0] = { pos1,{0.0f,1.0f} };//����
	vertices[1] = { pos2,{0.0f,0.0f} };//����
	vertices[2] = { pos3,{1.0f,1.0f} };//�E��
	vertices[3] = { pos4,{1.0f,0.0f} };//�E��

	Update(indices2, pipelineNum);
}
void Draw::DrawBoxSprite(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4, const int& pipelineNum)
{
	vertices[0] = { pos1,{0.0f,1.0f} };//����
	vertices[1] = { pos2,{0.0f,0.0f} };//����
	vertices[2] = { pos3,{1.0f,1.0f} };//�E��
	vertices[3] = { pos4,{1.0f,0.0f} };//�E��

	//05_03
	//���s���e�ϊ��i�X�v���C�g�`��?�j
	constMapTransform->mat =
		XMMatrixOrthographicOffCenterLH(0.0, win.window_width, win.window_height, 0.0, 0.0f, 1.0f);

	Update(indices2, pipelineNum);
}

void Draw::LoadGraph(const wchar_t* name)
{
	// 04_03
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WIC�̃e�N�X�`���̃��[�h
	directx.result = LoadFromWICFile(
		name,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	assert(SUCCEEDED(directx.result));

	ScratchImage mipChain{};
	//mipmap����
	directx.result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(directx.result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	////���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//textureResourceDesc.Width = textureWidth;						//���_�f�[�^�S�̂̃T�C�Y
	//textureResourceDesc.Height = textureHeight;
	//textureResourceDesc.DepthOrArraySize = 1;
	//textureResourceDesc.MipLevels = 1;
	//textureResourceDesc.SampleDesc.Count = 1;
	// 04_03
	
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;						//���_�f�[�^�S�̂̃T�C�Y
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
	//�e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff = nullptr;
	BuffProperties(textureHeapProp, textureResourceDesc, &texBuff);
	////�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//directx.result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,//�S�̈�փR�s�[
	//	imageData,//���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * textureWidth,//1���C���T�C�Y
	//	sizeof(XMFLOAT4) * imageDataCount//�S�T�C�Y
	//);

	//04_03
	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		directx.result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,//�S�̈�փR�s�[
			img->pixels,//���f�[�^�A�h���X
			(UINT)img->rowPitch,//1���C���T�C�Y
			(UINT)img->slicePitch//�S�T�C�Y
		);
		assert(SUCCEEDED(directx.result));
	}
	////���f�[�^���
	//delete[] imageData;

	
	
	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	if(srvHandle.ptr == 0) directx.result = directx.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(directx.result));
	//SRV�q�[�v�̐擪�n���h�����擾
	if (srvHandle.ptr == 0)srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	else srvHandle.ptr += directx.device->GetDescriptorHandleIncrementSize(srvHeapDesc.Type);

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//srvDesc.Shader4ComponentMapping =
	//	D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	//srvDesc.Texture2D.MipLevels = 1;
	////�n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	//device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	//04_03
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;
	//�n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directx.device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	
}

void Draw::PipeLineState(const D3D12_FILL_MODE& fillMode, ID3D12PipelineState** pipelineState)
{
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = fillMode; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	Blend(D3D12_BLEND_OP_ADD, false, true);

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//04_02
	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  //���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  //�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;  //���s���J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;    //�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                  //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                               //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	directx.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(directx.result));
	directx.result = directx.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(directx.result));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	directx.result = directx.device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState));
	assert(SUCCEEDED(directx.result));
}

void Draw::Blend(const D3D12_BLEND_OP& blendMode, const bool& Inversion, const bool& Translucent)
{
	//���ʐݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��
	blendDesc.BlendEnable = true;//�u�����h��L��
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��
	//��������
	blendDesc.BlendOp = blendMode;
	if (Inversion)//���]
	{
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blendDesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�
	}
	else if (Translucent)//������
	{
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//1.0f-�f�X�g�J���[�̒l
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}
	else
	{
		blendDesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
		blendDesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��
	}
}

void Draw::constBuffTransfer(const XMFLOAT4& plusRGBA)
{
	color2.x += plusRGBA.x;
	color2.y += plusRGBA.y;
	color2.z += plusRGBA.z;
	color2.w += plusRGBA.w;

	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = color2;//�������̐�
}

void Draw::ResourceProperties(D3D12_RESOURCE_DESC& resDesc, const UINT& size)
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = size;						//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void Draw::BuffProperties(D3D12_HEAP_PROPERTIES& heap, D3D12_RESOURCE_DESC& resource, ID3D12Resource** buff)
{
	directx.result = directx.device->CreateCommittedResource(
		&heap,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resource,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(buff));
	assert(SUCCEEDED(directx.result));
}

void Draw::SetNormDigitalMat(XMMATRIX& mat, const WindowsApp& win)
{
	mat.r[0].m128_f32[0] = 2.0f / win.window_width;
	mat.r[1].m128_f32[1] = -2.0f / win.window_height;
	mat.r[3].m128_f32[0] = -1.0f;
	mat.r[3].m128_f32[1] = 1.0f;
}

void Draw::Error(const bool& filed)
{
	if (filed)
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

