#pragma once
#include "pch.h"
#include "StepTimer.h"
#include "Memutils.h"

#define PLAYER_SPEED  10;
#define EASY_SPEED  5;
#define MEDIUM_SPEED  10;
#define HARD_SPEED  15;

#define BALL_SPEED  10;

class Pong
{
public:
	Pong();
	~Pong();

	void Initialize(HWND hwnd);
	void Tick();
	void Update(DX::StepTimer const& timer);
	void Render();

	void Present();
	void Clear();

	void OnActivate();
	void OnDeactivate();

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	bool keys[65336];

private:

	HWND hwnd;

	D3D_FEATURE_LEVEL			m_featureLevel;
	ID3D11Device*				m_d3dDevice;
	ID3D11Device1*				m_d3dDevice1;
	ID3D11DeviceContext*		m_d3dContext;
	ID3D11DeviceContext1*		m_d3dContext1;

	IDXGISwapChain*				m_swapChain;
	IDXGISwapChain1*			m_swapChain1;
	ID3D11RenderTargetView*		m_renderTargetView;
	ID3D11DepthStencilView*		m_depthStencilView;
	ID3D11Texture2D*			m_depthStencil;
	ID3D11ShaderResourceView*	m_texture;
	ID3D11ShaderResourceView*	m_dividerTexture;
	ID3D11ShaderResourceView*	m_paddleTexture;

	DX::StepTimer				m_timer;

	DirectX::XMFLOAT2 Position;
	DirectX::XMFLOAT2 Velocity;
	RECT rcBall;

	DirectX::XMFLOAT2 PlayerOnePosition;
	DirectX::XMFLOAT2 PlayerOneVelocity;
	RECT rcPlayerOne;

	DirectX::XMFLOAT2 PlayerTwoPosition;
	DirectX::XMFLOAT2 PlayerTwoVelocity;
	RECT rcPlayerTwo;

	DirectX::SpriteBatch* m_SpriteBatch;
	DirectX::SpriteFont* m_SpriteFont;

	UINT playerOneScore, playerTwoScore;

};

