#include "DXRender.h"

using namespace DirectX;
using namespace std;

/// <summary>
/// Default constructor
/// </summary>
DXRender::DXRender() :
	mWidth(0),
	mHeight(0),
	mAntTweak(nullptr)
{
}

/// <summary>
/// Default destructor
/// </summary>
DXRender::~DXRender()
{
}

/// <summary>
/// Sets internal reference to the windows application window
/// </summary>
/// <param name="Wnd">Reference to windows application window</param>
void DXRender::SetWindow(const HWND& Wnd)
{
	mWnd = Wnd;
}

/// <summary>
/// Compiles shader from file
/// </summary>
/// <param name="szFileName">File name of shader to compile</param>
/// <param name="szEntryPoint">Entry point of shader</param>
/// <param name="szShaderModel">Model of shader</param>
/// <param name="ppBlobOut">Pointer to shader data</param>
/// <returns>Returns OK if compliation of shader is successful, or an error if unsuccessful</returns>
HRESULT DXRender::CompileShaderFromFile(const WCHAR* const szFileName, const LPCSTR szEntryPoint, const LPCSTR szShaderModel, ID3DBlob** const ppBlobOut) const
{
	HRESULT hr = static_cast<HRESULT>(0L);

	const DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, const_cast<ID3DBlob**>(ppBlobOut), &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return static_cast<HRESULT>(0L);
}

/// <summary>
/// Initialises DirectX device and resources needed for rendering with DirectX:
/// Swap Chain
/// Render target view
/// Viewport
/// Depth Stencil
/// Constant Buffer
/// Sampler State
/// Rasteriser State
/// </summary>
/// <returns>Returns OK if initialisation of DirectX device is successful, or an error if unsuccessful</returns>
HRESULT DXRender::InitDXDevice(AntTweakManager& antTweakManager)
{
	HRESULT hr = static_cast<HRESULT>(0L);

	RECT rc;
	GetClientRect(mWnd, &rc);
	mWidth = rc.right - rc.left;
	mHeight = rc.bottom - rc.top;

	const UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		mDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &mD3dDevice, &mFeatureLevel, &mImmediateContext);

		if (hr == static_cast<HRESULT>(0x80070057L))
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &mD3dDevice, &mFeatureLevel, &mImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = mD3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = mD3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mD3dDevice1));
		if (SUCCEEDED(hr))
		{
			static_cast<void>(mImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mImmediateContext1)));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = mWidth;
		sd.Height = mHeight;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(mD3dDevice, mWnd, &sd, nullptr, nullptr, &mSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = mSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = mWidth;
		sd.BufferDesc.Height = mHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = mWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(mD3dDevice, &sd, &mSwapChain);
	}

	//Disable full screen
	dxgiFactory->MakeWindowAssociation(mWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = mD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = mWidth;
	descDepth.Height = mHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3dDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3dDevice->CreateDepthStencilView(mDepthStencil, &descDSV, &mDepthStencilView);
	if (FAILED(hr))
		return hr;

	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(mWidth);
	vp.Height = static_cast<float>(mHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mImmediateContext->RSSetViewports(1, &vp);

	// Set primitive topology
	mImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	mBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	mBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufferDesc.CPUAccessFlags = 0;
	hr = mD3dDevice->CreateBuffer(&mBufferDesc, nullptr, &mConstantBuffer);
	if (FAILED(hr))
		return hr;

	//Set constant buffers
	mImmediateContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
	mImmediateContext->PSSetConstantBuffers(0, 1, &mConstantBuffer);

	//Create sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	hr = mD3dDevice->CreateSamplerState(&sampDesc, &mSampler);
	if (FAILED(hr))
		return hr;

	//Set sampler state
	mImmediateContext->PSSetSamplers(0, 1, &mSampler);

	//Create depth stencil for skybox drawing
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = mD3dDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilLessThanEqual);
	if (FAILED(hr))
		return hr;

	//Create rasteriser with no cull mode for skybox drawing
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;

	hr = mD3dDevice->CreateRasterizerState(&rastDesc, &mRasteriserStateNoCull);
	if (FAILED(hr))
		return hr;

	//Create rasteriser with back cull mode
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;

	hr = mD3dDevice->CreateRasterizerState(&rastDesc, &mRasteriserStateBackCull);
	if (FAILED(hr))
		return hr;

	//Create blend state for alpha blending
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = mD3dDevice->CreateBlendState(&blendDesc, &mBlendStateAlpha);
	if (FAILED(hr))
		return hr;

	//Create default blend state for disabling blend
	blendDesc.RenderTarget[0].BlendEnable = FALSE;

	hr = mD3dDevice->CreateBlendState(&blendDesc, &mBlendStateDisable);
	if (FAILED(hr))
		return hr;

	//Initialise AntTweak
	mAntTweak = &antTweakManager;
	mAntTweak->Init(TW_DIRECT3D11, mD3dDevice, mWidth, mHeight);

	return static_cast<HRESULT>(0L);
}

/// <summary>
/// Renders all entities currently in the scene
/// </summary>
/// <param name="entities">List of entities to render</param>
/// <param name="camera">Currently active camera</param>
/// <returns>Returns OK if rendering of the current frame is successful, or an error if unsuccessful</returns>
HRESULT DXRender::Render(const std::vector<Entity>& entities, const Camera* const camera, const float& time)
{
	HRESULT hr = static_cast<HRESULT>(0L);

	cb1.numOfLights = 0;
	cb1.time = time;

	// Clear the back buffer
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, Colors::MidnightBlue);

	// Clear the depth buffer to 1.0 (max depth)
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//Set Camera
	SetCamera(camera);

	//Set lighting values
	for (const auto& entity : entities)
	{
		for (const auto& light : entity.Lights())
		{
			//Set Lighting
			SetLights(light);
		}
	}

	//Render shapes
	for (const auto& entity : entities)
	{
		for (const auto& shape : entity.Shapes())
		{
			//Set shader
			hr = Shader(shape);
			if (FAILED(hr))
			{
				return hr;
			}

			//Set Texture
			hr = Texture(shape);
			if (FAILED(hr))
			{
				return hr;
			}

			//Set vertex and index buffers
			hr = VertexIndex(shape);
			if (FAILED(hr))
			{
				return hr;
			}

			//Set constant buffer value for world matrix and update constant buffer
			XMStoreFloat4x4(&cb1.mWorld, XMMatrixTranspose(XMLoadFloat4x4(&shape.Transform())));
			XMStoreFloat4x4(&cb1.mWorldInverse, XMMatrixInverse(NULL, XMLoadFloat4x4(&cb1.mWorld)));
			mImmediateContext->UpdateSubresource(mConstantBuffer, 0, nullptr, &cb1, 0, 0);

			//Set depth stencil, cull mode and blend state
			if (shape.ParticleSystem())
			{
				//Set blend state
				float blendFactor[] = { 0.5f, 0.0f, 0.0f, 0.0f };
				mImmediateContext->OMSetBlendState(mBlendStateAlpha, blendFactor, 0xffffffff);

				//Set depth stencil
				mImmediateContext->OMSetDepthStencilState(mDepthStencilLessThanEqual, 0);

				//Set cull mode
				mImmediateContext->RSSetState(mRasteriserStateNoCull);
			}
			else if (shape.SkyBox())
			{
				//Set blend state
				mImmediateContext->OMSetBlendState(mBlendStateDisable, nullptr, 0xffffffff);

				//Set depth stencil
				mImmediateContext->OMSetDepthStencilState(mDepthStencilLessThanEqual, 0);

				//Set cull mode
				mImmediateContext->RSSetState(mRasteriserStateNoCull);
			}
			else
			{
				//Set blend state
				mImmediateContext->OMSetBlendState(mBlendStateDisable, nullptr, 0xffffffff);

				//Set depth stencil
				mImmediateContext->OMSetDepthStencilState(NULL, 0);

				//Set cull mode
				mImmediateContext->RSSetState(mRasteriserStateBackCull);
			}

			//Draw shape
			//If shape has more than once instance, draws using instanced rendering
			if (shape.Instances().size() > 0)
			{
				mImmediateContext->DrawIndexedInstanced(shape.Indices().size(), shape.Instances().size(), 0, 0, 0);
			}
			//If only one instance of shape, draws without instancing
			else
			{
				mImmediateContext->DrawIndexed(shape.Indices().size(), 0, 0);
			}
		}
	}

	//Render AntTweak
	mAntTweak->Draw();

	// Present our back buffer to our front buffer
	mSwapChain->Present(1, 0);

	return hr;
}

/// <summary>
/// Sets constant buffer values for light
/// </summary>
/// <param name="lights">Light to calculate lighting for</param>
void DXRender::SetLights(const Light& light)
{
	cb1.vLightPos[static_cast<int>(cb1.numOfLights)] = light.Position();
	cb1.vLightColour[static_cast<int>(cb1.numOfLights)] = light.Colour();
	cb1.numOfLights = cb1.numOfLights + 1;
}

/// <summary>
/// Sets view matrix, projection matrix and camera position to active camera and applies them to constant buffer
/// </summary>
/// <param name="camera">Currently active camera</param>
void DXRender::SetCamera(const Camera* const camera)
{
	XMStoreFloat4x4(&cb1.mView, XMMatrixTranspose(XMLoadFloat4x4(&camera->View())));
	XMStoreFloat4x4(&cb1.mProjection, XMMatrixTranspose(XMLoadFloat4x4(&camera->Projection())));
	cb1.vCameraPos = XMFLOAT4(camera->Position().x, camera->Position().y, camera->Position().z, 0.0f);
}

/// <summary>
/// Checks if the vertex/index buffers have already been created for shape
/// Either creates new buffers or retrieves already existing buffers and sets them as active buffers for drawing
/// </summary>
/// <param name="shape">Current shape to render</param>
/// /// <returns>Returns OK if vertex/index buffers are loaded successfully, or an error code if unsuccessful</returns>
HRESULT DXRender::VertexIndex(const Shape& shape)
{
	HRESULT hr = static_cast<HRESULT>(0L);

	//Checks if a vertex buffer for the shapes geometry already exists
	if (mVertexBuffers.count(shape.Geometry()) == 0)
	{
		//Create vertex buffer
		ZeroMemory(&mBufferDesc, sizeof(mBufferDesc));
		mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mBufferDesc.ByteWidth = sizeof(SimpleVertex) * shape.Vertices().size();
		mBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		mBufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &(shape.Vertices()[0]);
		ID3D11Buffer* vertexBuffer = nullptr;
		hr = mD3dDevice->CreateBuffer(&mBufferDesc, &InitData, &vertexBuffer);
		if (FAILED(hr))
		{
			return hr;
		}
		//Store pointer to new vertex buffer in vertex buffers map
		mVertexBuffers.insert(pair<GeometryType, ID3D11Buffer*>(shape.Geometry(), vertexBuffer));

		//Set vertex buffer
		const UINT stride = sizeof(SimpleVertex);
		const UINT offset = 0;
		mImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	}
	else
	{
		//Set vertex buffer to already existing vertex buffer for the shapes geometry
		const UINT stride = sizeof(SimpleVertex);
		const UINT offset = 0;
		mImmediateContext->IASetVertexBuffers(0, 1, &mVertexBuffers[shape.Geometry()], &stride, &offset);
	}

	//Checks if an index buffer for the shapes geometry already exists
	if (mIndexBuffers.count(shape.Geometry()) == 0)
	{
		//Create index buffer
		mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mBufferDesc.ByteWidth = sizeof(WORD) * shape.Indices().size();
		mBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		mBufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &(shape.Indices()[0]);
		ID3D11Buffer* indexBuffer = nullptr;
		hr = mD3dDevice->CreateBuffer(&mBufferDesc, &InitData, &indexBuffer);
		if (FAILED(hr))
		{
			return hr;
		}

		//Store pointer to new index buffer in vertex buffers map
		mIndexBuffers.insert(pair<GeometryType, ID3D11Buffer*>(shape.Geometry(), indexBuffer));

		//Set index buffer
		mImmediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}
	else
	{
		// Set index buffer
		mImmediateContext->IASetIndexBuffer(mIndexBuffers[shape.Geometry()], DXGI_FORMAT_R16_UINT, 0);
	}

	//Checks if shape is rendered using instance rendering
	if (shape.Instances().size() > 0)
	{
		//Checks if an instance buffer for the shape already exists or if instance data has been changed
		if (mInstanceBuffers.count(shape.Name()) == 0)
		{
			//Create instance buffer
			ZeroMemory(&mBufferDesc, sizeof(mBufferDesc));
			mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			mBufferDesc.ByteWidth = sizeof(InstanceData) * shape.Instances().size();
			mBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			mBufferDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = &(shape.Instances()[0]);
			ID3D11Buffer* instanceBuffer = nullptr;
			hr = mD3dDevice->CreateBuffer(&mBufferDesc, &InitData, &instanceBuffer);
			if (FAILED(hr))
			{
				return hr;
			}
			//Store pointer to new instance buffer in vertex instance map
			mInstanceBuffers.insert(pair<string, ID3D11Buffer*>(shape.Name(), instanceBuffer));

			//Set vertex buffer
			const UINT stride = sizeof(InstanceData);
			const UINT offset = 0;
			mImmediateContext->IASetVertexBuffers(1, 1, &instanceBuffer, &stride, &offset);
		}
		else
		{
			//Set instance buffer to already existing instance buffer for the shapes geometry
			const UINT stride = sizeof(InstanceData);
			const UINT offset = 0;
			mImmediateContext->IASetVertexBuffers(1, 1, &mInstanceBuffers[shape.Name()], &stride, &offset);
		    mImmediateContext->UpdateSubresource(mInstanceBuffers[shape.Name()], 0, nullptr, &(shape.Instances()[0]), 0, 0);
		}
	}

	return hr;
}

/// <summary>
/// Checks if textures for the shape have already been loaded
/// Either loads textures in or retrieves already loaded textures and sets them in shader resources
/// </summary>
/// <param name="shape">Current shape to render</param>
/// <returns>Returns OK if textures are loaded successfully, or an error code if unsuccessful</returns>
HRESULT DXRender::Texture(const Shape& shape)
{
	HRESULT hr = static_cast<HRESULT>(0L);
	ID3D11ShaderResourceView* textureRV = nullptr;

	//Checks if shape has diffuse map
	if (!shape.DiffuseTexture().empty())
	{
		//Checks if diffuse map for the shape already exists
		if (mTextures.count(shape.DiffuseTexture()) == 0)
		{
			//Create diffuse map
			hr = CreateDDSTextureFromFile(mD3dDevice, shape.DiffuseTexture().c_str(), nullptr, &textureRV);
			if (FAILED(hr))
				return hr;

			//Store pointer to new texture resource in textures map
			mTextures.insert(pair<wstring, ID3D11ShaderResourceView*>(shape.DiffuseTexture(), textureRV));

			//Set shader resource
			mImmediateContext->PSSetShaderResources(0, 1, &textureRV);
		}
		else
		{
			//Set shader resource
			mImmediateContext->PSSetShaderResources(0, 1, &mTextures[shape.DiffuseTexture()]);
		}
	}

	//Checks if shape has normal map
	if (!shape.NormalMap().empty())
	{
		//Checks if normal map for the shape already exists
		if (mTextures.count(shape.NormalMap()) == 0)
		{
			//Create normal map
			hr = CreateDDSTextureFromFile(mD3dDevice, shape.NormalMap().c_str(), nullptr, &textureRV);
			if (FAILED(hr))
				return hr;

			//Store pointer to new texture resource in textures map
			mTextures.insert(pair<wstring, ID3D11ShaderResourceView*>(shape.NormalMap(), textureRV));

			//Set shader resource
			mImmediateContext->PSSetShaderResources(1, 1, &textureRV);
		}
		else
		{
			//Set shader resource
			mImmediateContext->PSSetShaderResources(1, 1, &mTextures[shape.NormalMap()]);
		}
	}

	//Checks if shape has displacement map
	if (!shape.DisplacementMap().empty())
	{
		//Check if displacement map for the shape already exists
		if (mTextures.count(shape.DisplacementMap()) == 0)
		{
			hr = CreateDDSTextureFromFile(mD3dDevice, shape.DisplacementMap().c_str(), nullptr, &textureRV);
			if (FAILED(hr))
				return hr;

			//Store pointer to new texture resource in textures map
			mTextures.insert(pair<wstring, ID3D11ShaderResourceView*>(shape.DisplacementMap(), textureRV));

			//Set shader resource
			mImmediateContext->PSSetShaderResources(2, 1, &textureRV);
		}
		else
		{
			//Set shader resource
			mImmediateContext->PSSetShaderResources(2, 1, &mTextures[shape.DisplacementMap()]);
		}
	}



	return static_cast<HRESULT>(0L);
}

/// <summary>
/// Checks if shaders for the shape have already been compiled
/// Either compiles shaders or retrieves already compiled shaders and sets them as active shaders
/// </summary>
/// <param name="shape">Current shape to render</param>
/// <returns>Returns OK if shaders are compiled successfully, or an error code if unsuccessful</returns>
HRESULT DXRender::Shader(const Shape& shape)
{
	HRESULT hr = static_cast<HRESULT>(0L);

	//Checks if shader for the shape already exists
	if (mVertexShaders.count(shape.Shader()) == 0)
	{
		// Compiles the vertex shader
		ID3DBlob* pVSBlob = nullptr;
		hr = CompileShaderFromFile(shape.Shader().c_str(), "VS", "vs_4_0", &pVSBlob);
		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}

		// Creates the vertex shader
		ID3D11VertexShader* vertexShader = nullptr;
		hr = mD3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &vertexShader);
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return hr;
		}

		//Store pointer to new shader in vertex shaders map
		mVertexShaders.insert(pair<wstring, ID3D11VertexShader*>(shape.Shader(), vertexShader));

		//Set vertex shader
		mImmediateContext->VSSetShader(vertexShader, nullptr, 0);

		//Checks if input layout for shape already exists
		if (mLayouts.count(shape.Shader()) == 0)
		{
			// Define the shader input layout
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "INSTANCEPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			};
			UINT numElements = ARRAYSIZE(layout);

			// Create the shader input layout
			ID3D11InputLayout* vertexLayout = nullptr;
			hr = mD3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
				pVSBlob->GetBufferSize(), &vertexLayout);
			pVSBlob->Release();
			if (FAILED(hr))
				return hr;

			//Store pointer to new input layout in layouts map
			mLayouts.insert(pair<wstring, ID3D11InputLayout*>(shape.Shader(), vertexLayout));

			// Set the shader input layout
			mImmediateContext->IASetInputLayout(vertexLayout);
		}
		else
		{
			// Set the shader input layout
			mImmediateContext->IASetInputLayout(mLayouts[shape.Shader()]);
		}
	}
	else
	{
		//Set vertex shader
		mImmediateContext->VSSetShader(mVertexShaders[shape.Shader()], nullptr, 0);
	}

	//Checks if a pixel shader for the shape already exists
	if (mPixelShaders.count(shape.Shader()) == 0)
	{
		// Compile the pixel shader
		ID3DBlob* pPSBlob = nullptr;
		hr = CompileShaderFromFile(shape.Shader().c_str(), "PS", "ps_4_0", &pPSBlob);
		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}

		// Creates the pixel shader
		ID3D11PixelShader* pixelShader = nullptr;
		hr = mD3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;

		//Stores pointer to new shader in pixel shaders map
		mPixelShaders.insert(pair<wstring, ID3D11PixelShader*>(shape.Shader(), pixelShader));

		//Set pixel shader
		mImmediateContext->PSSetShader(pixelShader, nullptr, 0);
	}
	else
	{
		//Set pixel shader
		mImmediateContext->PSSetShader(mPixelShaders[shape.Shader()], nullptr, 0);
	}

	return static_cast<HRESULT>(0L);
}

/// <summary>
/// Releases all resources allocated for use by DirectX
/// </summary>
void DXRender::CleanUpDevice()
{
	if (mImmediateContext) mImmediateContext->ClearState();

	//Releases all resources used by vertex buffers
	for (const auto& buffer : mVertexBuffers)
	{
		buffer.second->Release();
	}

	//Releases all resources used by index buffers
	for (const auto& buffer : mIndexBuffers)
	{
		buffer.second->Release();
	}

	//Release all resources used by instance buffers
	for (const auto& buffer : mInstanceBuffers)
	{
		buffer.second->Release();
	}

	//Releases all resources used by vertex shaders
	for (const auto& shader : mVertexShaders)
	{
		shader.second->Release();
	}

	//Releases all resources used by pixel shaders
	for (const auto& shader : mPixelShaders)
	{
		shader.second->Release();
	}

	//Releases all resources used by textures
	for (const auto& texture : mTextures)
	{
		texture.second->Release();
	}

	//Release all resources used by vertex layouts
	for (const auto& layout : mLayouts)
	{
		layout.second->Release();
	}

	if (mConstantBuffer) mConstantBuffer->Release();
	if (mDepthStencil) mDepthStencil->Release();
	if (mDepthStencilView) mDepthStencilView->Release();
	if (mRenderTargetView) mRenderTargetView->Release();
	if (mSwapChain1) mSwapChain1->Release();
	if (mSwapChain) mSwapChain->Release();
	if (mImmediateContext1) mImmediateContext1->Release();
	if (mImmediateContext) mImmediateContext->Release();
	if (mD3dDevice1) mD3dDevice1->Release();
	if (mD3dDevice) mD3dDevice->Release();
	if (mSampler) mSampler->Release();
	if (mRasteriserStateNoCull) mRasteriserStateNoCull->Release();
	if (mRasteriserStateBackCull) mRasteriserStateBackCull->Release();
	if (mDepthStencilLessThanEqual) mDepthStencilLessThanEqual->Release();
	if (mBlendStateAlpha) mBlendStateAlpha->Release();
	if (mBlendStateDisable) mBlendStateDisable->Release();

	//Uninitialise ant tweak
	mAntTweak->Cleanup();
}

/// <summary>
/// Retrieves width of window
/// </summary>
/// <returns>Width of window</returns>
const int& DXRender::Width() const
{
	return mWidth;
}

/// <summary>
/// Retrieves height of window
/// </summary>
/// <returns>Height of window</returns>
const int& DXRender::Height() const
{
	return mHeight;
}
