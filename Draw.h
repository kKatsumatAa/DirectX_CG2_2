#pragma once

#include "DirectX.h"
#include "WorldMat.h"
#include "ViewMat.h"
#include "ProjectionMat.h"
#include "Util.h"
#include "ConstBuffTransform.h"

/// <summary>
/// 頂点インデックス用
/// </summary>
static enum indices
{
	TRIANGLE,
	BOX,
	CUBE,
	LINE,
	CIRCLE
};
struct Vertex
{
	XMFLOAT3 pos;   //xyz座標
	XMFLOAT3 normal;//法線ベクトル
	XMFLOAT2 uv;    //uv座標
};
//頂点データ

class Draw
{
private:
	//リソース設定
	//D3D12_RESOURCE_DESC resDesc{};
	ConstBuffTransform cbt;//ここをどうにかすれば、インスタンス一つでも色々描画
	// パイプランステートの生成
	ID3D12PipelineState* pipelineState[2] = { nullptr };
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//04_01
		//頂点データ構造体
	
	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
	
	//定数バッファの生成
	ID3D12Resource* constBuffMaterial = nullptr;
	//定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;//色(RGBA)
	};
	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	
	


	// 2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	
	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	HRESULT result;
	ID3D12Device* device = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	//04_02
	
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle2;
	int count2=0;
	

	

private:
	//--------------------
	void Update(const int& indexNum, const int& pipelineNum, const UINT64 textureHandle, const ConstBuffTransform& constBuffTransform,
		const bool& primitiveMode= true);

public://変数
	WorldMat* worldMat;
	ViewMat* view;
	ProjectionMat* projection;
	bool isWireFrame = 0;
	XMFLOAT4 color2 = { 0,0,0,0 };

public:
	

	//
	Draw();

	void DrawTriangle(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3,
		WorldMat* world, ViewMat* view, ProjectionMat* projection, XMFLOAT4 color={NULL,NULL,NULL,NULL},
		const UINT64 textureHandle = NULL, const int& pipelineNum = 0);
	void DrawBox(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4,
		WorldMat* world, ViewMat* view, ProjectionMat* projection, XMFLOAT4 color = { NULL,NULL,NULL,NULL },
		const UINT64 textureHandle = NULL, const int& pipelineNum=0);
	void DrawBoxSprite(XMFLOAT3& pos1, XMFLOAT3& pos2, XMFLOAT3& pos3, XMFLOAT3& pos4, XMFLOAT4 color = { NULL,NULL,NULL,NULL }
		, const UINT64 textureHandle = NULL, const int& pipelineNum = 0);
	void DrawCube3D(WorldMat* world, ViewMat* view, ProjectionMat* projection,
		XMFLOAT4 color = { NULL,NULL,NULL,NULL }, const UINT64 textureHandle = NULL, const int& pipelineNum = 0);
	void DrawLine(XMFLOAT3& pos1, XMFLOAT3& pos2, WorldMat* world, ViewMat* view, ProjectionMat* projection,XMFLOAT4& color
	, const UINT64 textureHandle=NULL, const int& pipelineNum=0);
	void DrawCircle(float radius, WorldMat* world, ViewMat* view, ProjectionMat* projection,
		XMFLOAT4 color = { NULL,NULL,NULL,NULL }, const UINT64 textureHandle = NULL, const int& pipelineNum = 0);
	
private:
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


