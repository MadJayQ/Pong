#include "Pong.h"



Pong::Pong()
{

}


Pong::~Pong()
{

}

void Pong::OnActivate()
{

}

void Pong::OnDeactivate()
{

}

void Pong::Initialize(HWND wnd)
{
	hwnd = wnd;

	CreateDevice();
	CreateResources();

	Position.x = 400;
	Position.y = 300;

	Velocity.x = BALL_SPEED;
	Velocity.y = BALL_SPEED;

	PlayerTwoPosition.x = 800 - 32;
}

void Pong::Tick()
{
	//Do stuff
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

void Pong::Update(DX::StepTimer const& timer)
{
	float dt = float(timer.GetElapsedSeconds());
	
	RECT rc;
	GetClientRect(hwnd, &rc);

	if (Velocity.x)
	{
		Position.x += Velocity.x;
	}
	if (Velocity.y)
	{
		Position.y += Velocity.y;
	}
	if (PlayerOneVelocity.y)
	{
		PlayerOnePosition.y += PlayerOneVelocity.y;
		PlayerOneVelocity.y = 0;
	}
	if (PlayerTwoVelocity.y)
	{
		PlayerTwoPosition.y += PlayerTwoVelocity.y;
		PlayerTwoVelocity.y = 0;
	}
	if (Position.y > rc.bottom || Position.y < rc.top)
	{
		Velocity.y *= -1;
	}
	if (rcBall.right > rcPlayerTwo.left + 16)
	{
		playerOneScore++;
		Position.x = 400;
		Position.y = 300;

		Velocity.x = BALL_SPEED;
		if ((rand() % 2) + 1 == 2)
		{
			Velocity.x = -BALL_SPEED;
		}
		Velocity.y = BALL_SPEED;
		if ((rand() % 2) + 1 == 2)
		{
			Velocity.y = -BALL_SPEED;
		}

	}
	if (rcBall.left < rcPlayerOne.right - 16)
	{
		playerTwoScore++;
		Position.x = 400;
		Position.y = 300;
		Velocity.x = BALL_SPEED;
		if ((rand() % 2) + 1 == 2)
		{
			Velocity.x = -BALL_SPEED;
		}
		Velocity.y = 8;
		if ((rand() % 2) + 1 == 2)
		{
			Velocity.y = -BALL_SPEED;
		}
	}
	if (playerOneScore > 11 || playerTwoScore > 11)
	{
		playerOneScore = 0;
		playerTwoScore = 0;
	}
	rcBall.left = Position.x;
	rcBall.top = Position.y;
	rcBall.right = Position.x + 32;
	rcBall.bottom = Position.y + 32;

	rcPlayerOne.left = PlayerOnePosition.x;
	rcPlayerOne.top = PlayerOnePosition.y;
	rcPlayerOne.right = PlayerOnePosition.x + 32;
	rcPlayerOne.bottom = PlayerOnePosition.y + 160;

	rcPlayerTwo.left = PlayerTwoPosition.x;
	rcPlayerTwo.top = PlayerTwoPosition.y;
	rcPlayerTwo.right = PlayerTwoPosition.x + 32;
	rcPlayerTwo.bottom = PlayerTwoPosition.y + 160;

	if (rcBall.left >= rcPlayerOne.right || rcBall.right <= rcPlayerOne.left ||
		rcBall.top >= rcPlayerOne.bottom || rcBall.bottom <= rcPlayerOne.top)
	{
		//Nothing
	}
	else
	{
		Velocity.x *= -1;
		if (PlayerOneVelocity.y < 0)
		{
			Velocity.y = -BALL_SPEED;
		}
		if (PlayerOneVelocity.y > 0)
		{
			Velocity.y = BALL_SPEED;
		}

	}
	if (rcBall.left >= rcPlayerTwo.right || rcBall.right <= rcPlayerTwo.left ||
		rcBall.top >= rcPlayerTwo.bottom || rcBall.bottom <= rcPlayerTwo.top)
	{
		//Nothing
	}
	else
	{
		Velocity.x *= -1;
		if (PlayerTwoVelocity.y < 0)
		{
			Velocity.y = -BALL_SPEED;
		}
		if (PlayerTwoVelocity.y > 0)
		{
			Velocity.y = BALL_SPEED;
		}

	}

	if (rcBall.bottom > rcPlayerTwo.bottom)
	{
		PlayerTwoVelocity.y = EASY_SPEED;
	}
	if (rcBall.bottom  < rcPlayerTwo.top)
	{
		PlayerTwoVelocity.y = -EASY_SPEED;
	}
	if (keys[VK_DOWN] && rcPlayerOne.bottom < rc.bottom)
	{
		PlayerOneVelocity.y = PLAYER_SPEED;
	}
	if (keys[VK_UP] && rcPlayerOne.top > rc.top)
	{
		PlayerOneVelocity.y = -PLAYER_SPEED;
	}
}

void Pong::Render()
{
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}
	Clear();

	m_SpriteBatch->Begin(DirectX::SpriteSortMode_Deferred);
	m_SpriteBatch->Draw(m_texture, DirectX::XMFLOAT2(Position.x, Position.y), nullptr, DirectX::Colors::White);
	std::wstring p1s = std::to_wstring(playerOneScore);
	m_SpriteFont->DrawString(m_SpriteBatch, p1s.c_str(), DirectX::XMFLOAT2(400 - 75, 0), DirectX::Colors::White, 0, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1.5, 1.5), DirectX::SpriteEffects_None, 1.0f);
	std::wstring p2s = std::to_wstring(playerTwoScore);
	m_SpriteFont->DrawString(m_SpriteBatch, p2s.c_str(), DirectX::XMFLOAT2(390 + 40, 0), DirectX::Colors::White, 0, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1.5, 1.5), DirectX::SpriteEffects_None, 1.0f);
	for (int i = 0; i < 38; i++)
	{
		m_SpriteBatch->Draw(m_dividerTexture, DirectX::XMFLOAT2(400 - 4, (i * 8) + (16 * i)), DirectX::Colors::White);
	}

	m_SpriteBatch->Draw(m_paddleTexture, PlayerOnePosition, nullptr, DirectX::Colors::White);
	m_SpriteBatch->Draw(m_paddleTexture, PlayerTwoPosition, nullptr, DirectX::Colors::White);
	m_SpriteBatch->End();


	Present();
}

void Pong::Present()
{
	HRESULT hr = m_swapChain->Present(1, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		//Device was lost
		OnDeviceLost();
	}
}

void Pong::Clear()
{
	m_d3dContext->ClearRenderTargetView(m_renderTargetView, DirectX::Colors::Black);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_d3dContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void Pong::CreateDevice()
{
	UINT DeviceFlags = 0;
#ifdef _DEBUG
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT hr = D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		DeviceFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&m_d3dDevice,
		&m_featureLevel,
		&m_d3dContext);

	if (hr == E_INVALIDARG) //Fucking shitty DirectX
	{
		HRESULT hr = D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			DeviceFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			&m_d3dDevice,
			&m_featureLevel,
			&m_d3dContext);
	}

#ifndef NDEBUG
	ID3D11Debug* d3dDebug;
	hr = m_d3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug);
	if (SUCCEEDED(hr))
	{
		ID3D11InfoQueue* d3dInfoQueue;
		hr = d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue);
		if (SUCCEEDED(hr))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
			};
			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

}

void Pong::CreateResources()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	Memory::SafeRelease(&m_renderTargetView);
	Memory::SafeRelease(&m_depthStencilView);
	m_d3dContext->Flush();

	RECT rc;
	GetWindowRect(hwnd, &rc);

	UINT backBufferWidth = std::max<UINT>(rc.right - rc.left, 1);
	UINT backBufferHeight = std::max<UINT>(rc.bottom - rc.top, 1);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = (m_featureLevel >= D3D_FEATURE_LEVEL_10_0) ? DXGI_FORMAT_D32_FLOAT : DXGI_FORMAT_D16_UNORM;

	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(2, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			OnDeviceLost();

			return;
		}
	}
	else
	{
		IDXGIDevice1* dxgiDevice;
		IDXGIAdapter* dxgiAdapter;
		IDXGIFactory1* dxgiFactory;
		IDXGIFactory2* dxgiFactory2;

		HRESULT hr = m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
		if (SUCCEEDED(hr))
		{
			hr = dxgiDevice->GetAdapter(&dxgiAdapter);
			if (SUCCEEDED(hr))
			{
				hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), (void**)&dxgiFactory);
				if (SUCCEEDED(hr))
				{
					hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), (void**)&dxgiFactory2);
					if (SUCCEEDED(hr))
					{
						hr = m_d3dDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_d3dDevice1);
						if (SUCCEEDED(hr))
						{
							hr = m_d3dContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_d3dContext1);
							if (SUCCEEDED(hr))
							{
								DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
								swapChainDesc.Width = backBufferWidth;
								swapChainDesc.Height = backBufferHeight;
								swapChainDesc.Format = backBufferFormat;
								swapChainDesc.SampleDesc.Count = 1;
								swapChainDesc.SampleDesc.Quality = 0;
								swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
								swapChainDesc.BufferCount = 2;

								DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
								fsSwapChainDesc.Windowed = TRUE;
								
								hr = dxgiFactory2->CreateSwapChainForHwnd(m_d3dDevice, hwnd, &swapChainDesc,
									&fsSwapChainDesc, nullptr, &m_swapChain1);
								if (SUCCEEDED(hr))
								{
									m_swapChain1->QueryInterface(__uuidof(IDXGISwapChain), (void**)&m_swapChain);
								}
							}
						}
					}
					else
					{
						DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
						swapChainDesc.BufferCount = 2;
						swapChainDesc.BufferDesc.Width = backBufferWidth;
						swapChainDesc.BufferDesc.Height = backBufferHeight;
						swapChainDesc.BufferDesc.Format = backBufferFormat;
						swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
						swapChainDesc.OutputWindow = hwnd;
						swapChainDesc.SampleDesc.Count = 1;
						swapChainDesc.SampleDesc.Quality = 0;
						swapChainDesc.Windowed = TRUE;

						dxgiFactory->CreateSwapChain(m_d3dDevice, &swapChainDesc, &m_swapChain);
					}
					dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
				}
			}
		}
	}
	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	m_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);

	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ID3D11Texture2D* depthStencil;
	m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	m_d3dDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &m_depthStencilView);

	CD3D11_VIEWPORT viewPort(0.0f, 0.0f, static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight));

	m_d3dContext->RSSetViewports(1, &viewPort);

	DirectX::CreateDDSTextureFromFile(m_d3dDevice, L"ball.dds", nullptr, &m_texture);
	DirectX::CreateDDSTextureFromFile(m_d3dDevice, L"divider.dds", nullptr, &m_dividerTexture);
	DirectX::CreateDDSTextureFromFile(m_d3dDevice, L"paddle.dds", nullptr, &m_paddleTexture);

	m_SpriteBatch = new DirectX::SpriteBatch(m_d3dContext);
	m_SpriteFont = new DirectX::SpriteFont(m_d3dDevice, L"impact.spritefont");

}

void Pong::OnDeviceLost()
{
	Memory::SafeRelease(&m_depthStencil);
	Memory::SafeRelease(&m_depthStencilView);
	Memory::SafeRelease(&m_swapChain1);
	Memory::SafeRelease(&m_swapChain);
	Memory::SafeRelease(&m_d3dContext1);
	Memory::SafeRelease(&m_d3dContext1);
	Memory::SafeRelease(&m_d3dDevice1);
	Memory::SafeRelease(&m_d3dDevice);

	CreateDevice();
	CreateResources();
}