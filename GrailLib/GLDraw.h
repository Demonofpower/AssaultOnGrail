#pragma once
#pragma comment(lib, "OpenGl32.lib")

#include <Windows.h>
#include <stdio.h>
#include <gl\GL.h>
#include "In.h"
#include "Util.h"

namespace RGB
{
	const GLubyte RED[3]{ 255,0,0 };
	const GLubyte GREEN[3]{ 0,255,0 };
	const GLubyte BLUE[3]{ 0,0,255 };
	const GLubyte GRAY[3]{ 55,55,55 };
	const GLubyte LIGHTGRAY[3]{ 192,192,192 };
	const GLubyte BLACK[3]{ 0,0,0 };
}

namespace GL
{
	void SetupOrtho();
	void RestoreGL();

	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutline(float x, float y, float width, float height, float inlineWidth, const GLubyte color[3]);

	class Font
	{
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void Build(int height);
		void Print(float x, float y, const unsigned char color[3], const char* format, ...);

		Vector3 CenterText(float x, float y, float width, float height, float textWidth, float textHeight);
		float CenterText(float x, float width, float textWidth);
	};
}