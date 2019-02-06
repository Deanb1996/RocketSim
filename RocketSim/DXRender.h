#pragma once

#include <vector>
#include <map>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "Entity.h"
#include "Shape.h"
#include "Camera.h"
#include "Light.h"
#include <Keyboard.h>
#include "AntTweakManager.h"

//Constant buffer for passing data to the shader
struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mWorldInverse;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProjection;
	DirectX::XMFLOAT4 vLightPos[4];
	DirectX::XMFLOAT4 vLightColour[4];
	DirectX::XMFLOAT4 vCameraPos;
	float numOfLights;
	float time;
	DirectX::XMFLOAT2 padding;
};


class DXRender 
{
private:
	std::map<std::wstring, ID3D11VertexShader*>			mVertexShaders;
	std::map<std::wstring, ID3D11PixelShader*>			mPixelShaders;
	std::map<GeometryType, ID3D11Buffer*>				mVertexBuffers;
	std::map<GeometryType, ID3D11Buffer*>				mIndexBuffers;
	std::map<std::string, ID3D11Buffer*>				mInstanceBuffers;
	std::map<std::wstring, ID3D11ShaderResourceView*>	mTextures;
	std::map<std::wstring, ID3D11InputLayout*>			mLayouts;
	int													mWidth;
	int													mHeight;
	D3D_DRIVER_TYPE										mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL									mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	HWND												mWnd = nullptr;
	ID3D11Device*										mD3dDevice = nullptr;
	ID3D11Device1*										mD3dDevice1 = nullptr;
	ID3D11DeviceContext*								mImmediateContext = nullptr;
	ID3D11DeviceContext1*								mImmediateContext1 = nullptr;
	IDXGISwapChain*										mSwapChain = nullptr;
	IDXGISwapChain1*									mSwapChain1 = nullptr;
	ID3D11RenderTargetView*								mRenderTargetView = nullptr;
	ID3D11Texture2D*									mDepthStencil = nullptr;
	ID3D11DepthStencilView*								mDepthStencilView = nullptr;
	ID3D11DepthStencilState*							mDepthStencilLessThanEqual = nullptr;
	ID3D11RasterizerState*								mRasteriserStateNoCull = nullptr;
	ID3D11RasterizerState*								mRasteriserStateBackCull = nullptr;
	ID3D11BlendState*									mBlendStateAlpha = nullptr;
	ID3D11BlendState*									mBlendStateDisable = nullptr;
	ID3D11Buffer*										mConstantBuffer = nullptr;
	D3D11_BUFFER_DESC									mBufferDesc;
	ID3D11SamplerState*									mSampler = nullptr;
	ConstantBuffer										cb1;
	AntTweakManager*									mAntTweak;

	void SetLights(const Light& light);
	void SetCamera(const Camera* camera);
	HRESULT VertexIndex(const Shape& shape);
	HRESULT Texture(const Shape& shape);
	HRESULT Shader(const Shape& shape);
	HRESULT CompileShaderFromFile(const WCHAR* const szFileName, const LPCSTR szEntryPoint, const LPCSTR szShaderModel, ID3DBlob** const ppBlobOut) const;

public:
	DXRender();
	~DXRender();

	const int& Width() const;
	const int& Height() const;

	void SetWindow(const HWND& Wnd);
	HRESULT InitDXDevice(AntTweakManager& antTweakManager);
	HRESULT Render(const std::vector<Entity>& entities, const Camera* const camera, const float& time);
	void CleanUpDevice();

	DXRender& operator=(const DXRender&) = delete;
	DXRender(const DXRender&) = delete;
};