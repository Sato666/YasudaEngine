#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <functional>
#include <locale.h>
#include <string>
#include <DirectXMath.h>
#include <WICTextureLoader.h>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>

#pragma warning(pop)

#pragma comment (lib,"winmm.lib")

constexpr uint32_t SCREEN_WIDTH = 1920;
constexpr uint32_t SCREEN_HEIGHT = 1080;

inline void Error(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	static char buf[1024];
	vsprintf_s(buf, sizeof(buf), format, arg);
	va_end(arg);
	MessageBox(NULL, buf, "Error", MB_OK);
}

//3840 x 2400		// rog flow z13 (my PC 16:10 4Kâëúìx)
//2560Å~1440
//1920 x 1080		// Full HD
//1280 x 720		// default