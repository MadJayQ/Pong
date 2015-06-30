#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <DirectXMath.h>

#include <SpriteBatch.h>
#include <SpriteFont.h>


#ifndef DXTK_AUDIO
#include <Dbt.h>
#include <Audio.h>
#endif 

#include <DDSTextureLoader.h>
#include <SpriteBatch.h>
#include <algorithm>
#include <memory>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "DirectXTK")



//Overlapped window - (Resize border and Maximize Button) 
#define WINSTYLE (WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX))



