// Project      : Weiss Engine
// Author       : Henry LE BERRE (PolarToCartesian)
// Repository   : https://github.com/PolarToCartesian/Weiss
// Requirements : Windows & DirectX 11 SDK & C++ 17
// Description  : Weiss is a 2D and 3D rendering engine for windows built with DirectX in c++.
//                It is intended to be a wrapper around low-level apis to make the developpement of 2D and 3D applications simpler and faster.

// LISCENCE     :

// Copyright (c) 2019 HENRY LE BERRE

// Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated documentation files(the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*

██          ██    ██████████████      ██████████      ██████████████    ██████████████
██          ██    ██                      ██          ██                ██
██          ██    ██                      ██          ██                ██
██    ██    ██    ██████████████          ██          ██████████████    ██████████████
██    ██    ██    ██                      ██                      ██                ██
██  ██  ██  ██    ██                      ██                      ██                ██
  ██      ██      ██████████████      ██████████      ██████████████    ██████████████
  
██████████████    ██          ██    ██████████████      ██████████      ██          ██    ██████████████
██                ████        ██    ██                      ██          ████        ██    ██
██                ██  ██      ██    ██                      ██          ██  ██      ██    ██
██████████████    ██    ██    ██    ██    ██████            ██          ██    ██    ██    ██████████████
██                ██      ██  ██    ██          ██          ██          ██      ██  ██    ██
██                ██        ████    ██          ██          ██          ██        ████    ██
██████████████    ██          ██    ████████████        ██████████      ██          ██    ██████████████

*/

#ifndef __WEISS__
#define __WEISS__
#ifndef _WIN32
#error Weiss Is Windows Only! (for now)
#endif // _WIN32

#include "math/Utils.h"
#include "engines/Engine.h"
#endif // __WEISS__

#ifdef __WEISS_LAST_INCLUDE

template <typename V>
Batch2DRenderer<V>::Batch2DRenderer(EngineCore& engine, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* vsSource, const char* psSource)
	: m_engine(engine)
{
	const VertexShaderDescriptor vsd = { ieds, ShaderLoadingMethod::FROM_SOURCE_CODE, nullptr, vsSource };
	this->m_vertexShaderIndex = this->m_engine.CreateVertexShader(vsd);

	const PixelShaderDescriptor psd = { ShaderLoadingMethod::FROM_SOURCE_CODE, nullptr, psSource };
	this->m_pixelShaderIndex = this->m_engine.CreatePixelShader(psd);
}

template <typename V>
void Batch2DRenderer<V>::CreateNewMeshesIfNeeded() {
	const size_t nMeshToAdd = static_cast<UINT>(std::ceil((this->m_triangles.size() * 3u) / (float)WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER) - this->m_meshes.size());

	for (size_t i = 0u; i < nMeshToAdd; i++)
	{
		V junk[WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER] = {  };

		const VertexBufferDescriptor vbd{ junk, WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER, sizeof(V), true };
		this->m_vertexBufferIndex = this->m_engine.CreateVertexBuffer(vbd);

		Mesh mesh{ this->m_vertexBufferIndex, this->m_vertexShaderIndex, this->m_pixelShaderIndex };

		this->m_meshes.push_back(this->m_engine.CreateMeshFromVertices(mesh));
	}
}

template <typename V>
void Batch2DRenderer<V>::FillMeshesIfNeeded()
{
	if (this->m_wasModified)
	{
		for (size_t i = 0u; i < this->m_meshes.size(); i++)
		{
			Mesh&         mesh         = this->m_engine.GetMesh(this->m_meshes[i]);
			VertexBuffer& vertexBuffer = this->m_engine.GetVertexBuffer(mesh.vertexBufferIndex);

			const void* srcPtr = this->m_triangles.data() + i * WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER;

			vertexBuffer.SetData(srcPtr, WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER);
		}
		
		this->m_wasModified = false;
	}
}

std::vector<Window> Window::m_s_windows = std::vector<Window>();

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::vector<Window>::iterator it = std::find_if(Window::m_s_windows.begin(), Window::m_s_windows.end(), [&hwnd](const Window& window)
		{
			return window.m_handle == hwnd;
		});

	if (it != Window::m_s_windows.end())
		return it->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif // __WEISS_LAST_INCLUDE