#ifndef _GRAPHICSDEVICE_DX11_H_
#define _GRAPHICSDEVICE_DX11_H_

#include "CommonInclude.h"
#include "wiGraphicsDevice.h"
#include "wiWindowRegistration.h"

struct ID3D11Device;
struct IDXGISwapChain1;
struct ID3D11DeviceContext;
struct ID3D11CommandList;
struct ID3DUserDefinedAnnotation;
enum D3D_DRIVER_TYPE;
enum D3D_FEATURE_LEVEL;

namespace wiGraphicsTypes
{

	class GraphicsDevice_DX11 : public GraphicsDevice
	{
	private:
		ID3D11Device*				device;
		D3D_DRIVER_TYPE				driverType;
		D3D_FEATURE_LEVEL			featureLevel;
		IDXGISwapChain1*			swapChain;
		ID3D11RenderTargetView*		renderTargetView;
		ID3D11Texture2D*			backBuffer;
		ViewPort					viewPort;
		ID3D11DeviceContext*		deviceContexts[GRAPHICSTHREAD_COUNT];
		ID3D11CommandList*			commandLists[GRAPHICSTHREAD_COUNT];
		ID3DUserDefinedAnnotation*	userDefinedAnnotations[GRAPHICSTHREAD_COUNT];

		UINT		stencilRef[GRAPHICSTHREAD_COUNT];
		XMFLOAT4	blendFactor[GRAPHICSTHREAD_COUNT];

		ID3D11VertexShader* prev_vs[GRAPHICSTHREAD_COUNT] = {};
		ID3D11PixelShader* prev_ps[GRAPHICSTHREAD_COUNT] = {};
		ID3D11HullShader* prev_hs[GRAPHICSTHREAD_COUNT] = {};
		ID3D11DomainShader* prev_ds[GRAPHICSTHREAD_COUNT] = {};
		ID3D11GeometryShader* prev_gs[GRAPHICSTHREAD_COUNT] = {};
		XMFLOAT4 prev_blendfactor[GRAPHICSTHREAD_COUNT] = {};
		UINT prev_samplemask[GRAPHICSTHREAD_COUNT] = {};
		ID3D11BlendState* prev_bs[GRAPHICSTHREAD_COUNT] = {};
		ID3D11RasterizerState* prev_rs[GRAPHICSTHREAD_COUNT] = {};
		UINT prev_stencilRef[GRAPHICSTHREAD_COUNT] = {};
		ID3D11DepthStencilState* prev_dss[GRAPHICSTHREAD_COUNT] = {};
		ID3D11InputLayout* prev_il[GRAPHICSTHREAD_COUNT] = {};
		PRIMITIVETOPOLOGY prev_pt[GRAPHICSTHREAD_COUNT] = {};

	public:
		GraphicsDevice_DX11(wiWindowRegistration::window_type window, bool fullscreen = false, bool debuglayer = false);

		~GraphicsDevice_DX11();

		virtual HRESULT CreateBuffer(const GPUBufferDesc *pDesc, const SubresourceData* pInitialData, GPUBuffer *ppBuffer) override;
		virtual HRESULT CreateTexture1D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture1D **ppTexture1D) override;
		virtual HRESULT CreateTexture2D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture2D **ppTexture2D) override;
		virtual HRESULT CreateTexture3D(const TextureDesc* pDesc, const SubresourceData *pInitialData, Texture3D **ppTexture3D) override;
		virtual HRESULT CreateInputLayout(const VertexLayoutDesc *pInputElementDescs, UINT NumElements,
			const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, VertexLayout *pInputLayout) override;
		virtual HRESULT CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, VertexShader *pVertexShader) override;
		virtual HRESULT CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, PixelShader *pPixelShader) override;
		virtual HRESULT CreateGeometryShader(const void *pShaderBytecode, SIZE_T BytecodeLength, GeometryShader *pGeometryShader) override;
		virtual HRESULT CreateHullShader(const void *pShaderBytecode, SIZE_T BytecodeLength, HullShader *pHullShader) override;
		virtual HRESULT CreateDomainShader(const void *pShaderBytecode, SIZE_T BytecodeLength, DomainShader *pDomainShader) override;
		virtual HRESULT CreateComputeShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ComputeShader *pComputeShader) override;
		virtual HRESULT CreateBlendState(const BlendStateDesc *pBlendStateDesc, BlendState *pBlendState) override;
		virtual HRESULT CreateDepthStencilState(const DepthStencilStateDesc *pDepthStencilStateDesc, DepthStencilState *pDepthStencilState) override;
		virtual HRESULT CreateRasterizerState(const RasterizerStateDesc *pRasterizerStateDesc, RasterizerState *pRasterizerState) override;
		virtual HRESULT CreateSamplerState(const SamplerDesc *pSamplerDesc, Sampler *pSamplerState) override;
		virtual HRESULT CreateQuery(const GPUQueryDesc *pDesc, GPUQuery *pQuery) override;
		virtual HRESULT CreateGraphicsPSO(const GraphicsPSODesc* pDesc, GraphicsPSO* pso) override;
		virtual HRESULT CreateComputePSO(const ComputePSODesc* pDesc, ComputePSO* pso) override;

		virtual void PresentBegin() override;
		virtual void PresentEnd() override;

		virtual void ExecuteDeferredContexts() override;
		virtual void FinishCommandList(GRAPHICSTHREAD thread) override;

		virtual void SetResolution(int width, int height) override;

		virtual Texture2D GetBackBuffer() override;

		///////////////Thread-sensitive////////////////////////

		virtual void BindViewports(UINT NumViewports, const ViewPort *pViewports, GRAPHICSTHREAD threadID) override;
		virtual void BindRenderTargetsUAVs(UINT NumViews, Texture2D* const *ppRenderTargets, Texture2D* depthStencilTexture, GPUResource* const *ppUAVs, int slotUAV, int countUAV,
			GRAPHICSTHREAD threadID, int arrayIndex = -1) override;
		virtual void BindRenderTargets(UINT NumViews, Texture2D* const *ppRenderTargets, Texture2D* depthStencilTexture, GRAPHICSTHREAD threadID, int arrayIndex = -1) override;
		virtual void ClearRenderTarget(Texture* pTexture, const FLOAT ColorRGBA[4], GRAPHICSTHREAD threadID, int arrayIndex = -1) override;
		virtual void ClearDepthStencil(Texture2D* pTexture, UINT ClearFlags, FLOAT Depth, UINT8 Stencil, GRAPHICSTHREAD threadID, int arrayIndex = -1) override;
		virtual void BindResource(SHADERSTAGE stage, GPUResource* resource, int slot, GRAPHICSTHREAD threadID, int arrayIndex = -1) override;
		virtual void BindResources(SHADERSTAGE stage, GPUResource *const* resources, int slot, int count, GRAPHICSTHREAD threadID) override;
		virtual void BindUnorderedAccessResourceCS(GPUResource* resource, int slot, GRAPHICSTHREAD threadID, int arrayIndex = -1) override;
		virtual void BindUnorderedAccessResourcesCS(GPUResource *const* resources, int slot, int count, GRAPHICSTHREAD threadID) override;
		virtual void UnBindResources(int slot, int num, GRAPHICSTHREAD threadID) override;
		virtual void UnBindUnorderedAccessResources(int slot, int num, GRAPHICSTHREAD threadID) override;
		virtual void BindSampler(SHADERSTAGE stage, Sampler* sampler, int slot, GRAPHICSTHREAD threadID) override;
		virtual void BindConstantBuffer(SHADERSTAGE stage, GPUBuffer* buffer, int slot, GRAPHICSTHREAD threadID) override;
		virtual void BindVertexBuffers(GPUBuffer* const *vertexBuffers, int slot, int count, const UINT* strides, const UINT* offsets, GRAPHICSTHREAD threadID) override;
		virtual void BindIndexBuffer(GPUBuffer* indexBuffer, const INDEXBUFFER_FORMAT format, UINT offset, GRAPHICSTHREAD threadID) override;
		virtual void BindStencilRef(UINT value, GRAPHICSTHREAD threadID) override;
		virtual void BindBlendFactor(XMFLOAT4 value, GRAPHICSTHREAD threadID) override;
		virtual void BindGraphicsPSO(GraphicsPSO* pso, GRAPHICSTHREAD threadID) override;
		virtual void BindComputePSO(ComputePSO* pso, GRAPHICSTHREAD threadID) override;
		virtual void Draw(int vertexCount, UINT startVertexLocation, GRAPHICSTHREAD threadID) override;
		virtual void DrawIndexed(int indexCount, UINT startIndexLocation, UINT baseVertexLocation, GRAPHICSTHREAD threadID) override;
		virtual void DrawInstanced(int vertexCount, int instanceCount, UINT startVertexLocation, UINT startInstanceLocation, GRAPHICSTHREAD threadID) override;
		virtual void DrawIndexedInstanced(int indexCount, int instanceCount, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanceLocation, GRAPHICSTHREAD threadID) override;
		virtual void DrawInstancedIndirect(GPUBuffer* args, UINT args_offset, GRAPHICSTHREAD threadID) override;
		virtual void DrawIndexedInstancedIndirect(GPUBuffer* args, UINT args_offset, GRAPHICSTHREAD threadID) override;
		virtual void Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ, GRAPHICSTHREAD threadID) override;
		virtual void DispatchIndirect(GPUBuffer* args, UINT args_offset, GRAPHICSTHREAD threadID) override;
		virtual void GenerateMips(Texture* texture, GRAPHICSTHREAD threadID, int arrayIndex = -1) override;
		virtual void CopyTexture2D(Texture2D* pDst, Texture2D* pSrc, GRAPHICSTHREAD threadID) override;
		virtual void CopyTexture2D_Region(Texture2D* pDst, UINT dstMip, UINT dstX, UINT dstY, Texture2D* pSrc, UINT srcMip, GRAPHICSTHREAD threadID) override;
		virtual void MSAAResolve(Texture2D* pDst, Texture2D* pSrc, GRAPHICSTHREAD threadID) override;
		virtual void UpdateBuffer(GPUBuffer* buffer, const void* data, GRAPHICSTHREAD threadID, int dataSize = -1) override;
		virtual void* AllocateFromRingBuffer(GPURingBuffer* buffer, size_t dataSize, UINT& offsetIntoBuffer, GRAPHICSTHREAD threadID) override;
		virtual void InvalidateBufferAccess(GPUBuffer* buffer, GRAPHICSTHREAD threadID) override;
		virtual bool DownloadBuffer(GPUBuffer* bufferToDownload, GPUBuffer* bufferDest, void* dataDest, GRAPHICSTHREAD threadID) override;
		virtual void SetScissorRects(UINT numRects, const Rect* rects, GRAPHICSTHREAD threadID) override;
		virtual void QueryBegin(GPUQuery *query, GRAPHICSTHREAD threadID) override;
		virtual void QueryEnd(GPUQuery *query, GRAPHICSTHREAD threadID) override;
		virtual bool QueryRead(GPUQuery *query, GRAPHICSTHREAD threadID) override;
		virtual void UAVBarrier(GPUResource *const* uavs, UINT NumBarriers, GRAPHICSTHREAD threadID) override {};
		virtual void TransitionBarrier(GPUResource *const* resources, UINT NumBarriers, RESOURCE_STATES stateBefore, RESOURCE_STATES stateAfter, GRAPHICSTHREAD threadID) override {};

		virtual void WaitForGPU() override;

		virtual HRESULT CreateTextureFromFile(const std::string& fileName, Texture2D **ppTexture, bool mipMaps, GRAPHICSTHREAD threadID) override;
		virtual HRESULT SaveTexturePNG(const std::string& fileName, Texture2D *pTexture, GRAPHICSTHREAD threadID) override;
		virtual HRESULT SaveTextureDDS(const std::string& fileName, Texture *pTexture, GRAPHICSTHREAD threadID) override;

		virtual void EventBegin(const std::string& name, GRAPHICSTHREAD threadID) override;
		virtual void EventEnd(GRAPHICSTHREAD threadID) override;
		virtual void SetMarker(const std::string& name, GRAPHICSTHREAD threadID) override;

	private:
		HRESULT CreateShaderResourceView(Texture1D* pTexture);
		HRESULT CreateShaderResourceView(Texture2D* pTexture);
		HRESULT CreateShaderResourceView(Texture3D* pTexture);
		HRESULT CreateRenderTargetView(Texture2D* pTexture);
		HRESULT CreateRenderTargetView(Texture3D* pTexture);
		HRESULT CreateDepthStencilView(Texture2D* pTexture);
	};

}

#endif // _GRAPHICSDEVICE_DX11_H_
