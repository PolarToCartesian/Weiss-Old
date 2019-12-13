#pragma once

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



//          //    //////////////      //////////      //////////////    //////////////
//          //    //                      //          //                //
//          //    //                      //          //                //
//    //    //    //////////////          //          //////////////    //////////////
//    //    //    //                      //                      //                //
//  //  //  //    //                      //                      //                //
  //      //      //////////////      //////////      //////////////    //////////////

//////////////    //          //    //////////////      //////////      //          //    //////////////
//                ////        //    //                      //          ////        //    //
//                //  //      //    //                      //          //  //      //    //
//////////////    //    //    //    //    //////            //          //    //    //    //////////////
//                //      //  //    //          //          //          //      //  //    //
//                //        ////    //          //          //          //        ////    //
//////////////    //          //    ////////////        //////////      //          //    //////////////



// --> INCLUDES START

#define NOMINMAX

// --> INCLUDES --> STANDARD LIBRARY START

#include <array>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <algorithm>
#include <functional>

// --> INCLUDES --> STANDARD LIBRARY END
// --> INCLUDES --> WINDOWS SPECIFIC START

#include <wrl.h>
#include <d3d11.h>
#include <strsafe.h>
#include <Windows.h>
#include <windowsx.h>
//#include <winhttp.h>
#include <wincodec.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// --> INCLUDES --> WINDOWS SPECIFIC END
// --> LIBRARY LINKING START

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "windowscodecs.lib")

#undef NOMINMAX

// --> LIBRARY LINKING END
// --> ERROR HANDLING START

#define MESSAGE_BOX_ERROR(errorMsg) {\
	MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__) + std::string("\nError: ") + std::string(errorMsg)).c_str(), "Weiss Engine Error", MB_ABORTRETRYIGNORE);\
}

#define ASSERT_ERROR(v, errorMsg) {\
	if (!(v))\
		MESSAGE_BOX_ERROR(errorMsg)\
}

#define HRESULT_ERROR(hr, errorMsg) {\
	if (hr != S_OK)\
		MESSAGE_BOX_ERROR(errorMsg)\
}

// --> ERROR HANDLING END

// --> WEISS DEFINES START

constexpr const size_t WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX = 0u;

// --> WEISS DEFINES END

// --> MATH START
// --> MATH --> CONSTANTS START

constexpr float PI             = 3.14159265359f;
constexpr float TWO_PI         = 2.0f * PI;
constexpr float HALF_PI        = PI / 2.0f;
constexpr float QUARTER_PI     = PI / 4.0f;
constexpr float PI_DIV_180     = PI / 180.0f;
constexpr float PI_DIV_180_INV = 180.0f / PI;

constexpr float TAU         = TWO_PI;
constexpr float HALF_TAU    = PI;
constexpr float QUARTER_TAU = HALF_PI;

// --> MATH --> CONSTANTS END

// --> MATH --> VECTORS START
// --> MATH --> VECTORS --> TYPEDEFS START

typedef std::array<float,    2> Vec2f;
typedef std::array<int8_t,   2> Vec2i8;
typedef std::array<int16_t,  2> Vec2i16;
typedef std::array<int32_t,  2> Vec2i32;
typedef std::array<int64_t,  2> Vec2i64;
typedef std::array<uint8_t,  2> Vec2u8;
typedef std::array<uint16_t, 2> Vec2u16;
typedef std::array<uint32_t, 2> Vec2u32;
typedef std::array<uint64_t, 2> Vec2u64;

typedef std::array<float,    3> Vec3f;
typedef std::array<int8_t,   3> Vec3i8;
typedef std::array<int16_t,  3> Vec3i16;
typedef std::array<int32_t,  3> Vec3i32;
typedef std::array<int64_t,  3> Vec3i64;
typedef std::array<uint8_t,  3> Vec3u8;
typedef std::array<uint16_t, 3> Vec3u16;
typedef std::array<uint32_t, 3> Vec3u32;
typedef std::array<uint64_t, 3> Vec3u64;

typedef std::array<float,    4> Vec4f;
typedef std::array<int8_t,   4> Vec4i8;
typedef std::array<int16_t,  4> Vec4i16;
typedef std::array<int32_t,  4> Vec4i32;
typedef std::array<int64_t,  4> Vec4i64;
typedef std::array<uint8_t,  4> Vec4u8;
typedef std::array<uint16_t, 4> Vec4u16;
typedef std::array<uint32_t, 4> Vec4u32;
typedef std::array<uint64_t, 4> Vec4u64;
typedef std::array<uint8_t,  4> Vec4u8;

// --> MATH --> VECTORS --> TYPEDEFS END
// --> MATH --> VECTORS --> OPERATOR OVERLOADING START

template <typename T, size_t S>
[[nodiscard]] std::array<T, S> operator+(const std::array<T, S>& a, const std::array<T, S>& b)
{
	std::array<T, S> result(a);
	
	for (size_t i = 0; i < S; i++)
		result[i] += b[i];
	
	return result;
}

template <typename T, size_t S>
[[nodiscard]] std::array<T, S> operator-(const std::array<T, S>& a, const std::array<T, S>& b)
{
	std::array<T, S> result(a);

	for (size_t i = 0; i < S; i++)
		result[i] -= b[i];

	return result;
}

template <typename T, size_t S>
[[nodiscard]] std::array<T, S> operator*(const std::array<T, S>& a, const std::array<T, S>& b)
{
	std::array<T, S> result(a);

	for (size_t i = 0; i < S; i++)
		result[i] *= b[i];

	return result;
}

template <typename T, size_t S>
[[nodiscard]] std::array<T, S> operator/(const std::array<T, S>& a, const std::array<T, S>& b)
{
	std::array<T, S> result(a);
	
	for (size_t i = 0; i < S; i++)
		result[i] /= b[i];
	
	return result;
}

template <typename T, size_t S>
[[nodiscard]] std::array<T, S> operator/(const std::array<T, S>& a, const float n)
{
	std::array<T, S> result(a);
	
	for (size_t i = 0; i < S; i++)
		result[i] /= n;
	
	return result;
}

template <typename T, size_t S>
void operator+=(std::array<T, S>& a, const std::array<T, S>& b)
{
	for (size_t i = 0; i < S; i++)
		a[i] += b[i];
}

template <typename T, size_t S>
void operator-=(std::array<T, S>& a, const std::array<T, S>& b)
{
	for (size_t i = 0; i < S; i++)
		a[i] -= b[i];
}

template <typename T, size_t S>
void operator*=(std::array<T, S>& a, const std::array<T, S>& b)
{
	for (size_t i = 0; i < S; i++)
		a[i] *= b[i];
}

template <typename T, size_t S>
void operator/=(std::array<T, S>& a, const std::array<T, S>& b)
{
	for (size_t i = 0; i < S; i++)
		a[i] /= b[i];
}

template <typename T, size_t S>
void operator+=(std::array<T, S>& a, const float n)
{
	for (size_t i = 0; i < S; i++)
		a[i] += n;
}

template <typename T, size_t S>
void operator-=(std::array<T, S>& a, const float n)
{
	for (size_t i = 0; i < S; i++)
		a[i] -= n;
}

template <typename T, size_t S>
void operator*=(std::array<T, S>& a, const float n)
{
	for (size_t i = 0; i < S; i++)
		a[i] *= n;
}

template <typename T, size_t S>
void operator/=(std::array<T, S>& a, const float n)
{
	for (size_t i = 0; i < S; i++)
		a[i] /= n;
}

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& stream, const std::array<T, S>& v)
{
	stream << '(';

	for (auto it = v.begin(); it != v.end() - 1; it++)
		stream << *it << ", ";

	return stream << v[S - 1] << ')';
}

// --> MATH --> VECTORS --> OPERATOR OVERLOADING END
// --> MATH --> VECTORS --> END

// --> MATH --> CONVERSIONS START

namespace Conversions
{
	[[nodiscard]] inline float degreesToRadians(const float degrees)
	{
		return degrees * PI_DIV_180;
	}

	[[nodiscard]] inline float radiansToDegrees(const float radians)
	{
		return radians * PI_DIV_180_INV;
	}

	// If a polar point is stored as (θ, r)
	[[nodiscard]] inline Vec2f polarToCartesian(const Vec2f polar)
	{
		const float x = polar[1] * std::cos(polar[0]);
		const float y = polar[1] * std::sin(polar[0]);

		return Vec2f{ x, y };
	}

	[[nodiscard]] inline Vec2f cartesianToPolar(const Vec2f cartesian)
	{
		float a = std::atan(cartesian[1] / cartesian[0]);

		if (cartesian[0] < 0) a += PI;
		else if (cartesian[1] < 0) a += TWO_PI;

		const float r = std::sqrt(std::pow(cartesian[0], 2) + std::pow(cartesian[1], 2));

		return Vec2f{ a, r };
	}
};

// --> MATH --> CONVERSIONS END
// --> MATH --> MISCELLANEOUS FUNCTIONS START 

namespace Misc
{
	[[nodiscard]] inline Vec3f getTriangleSurfaceNormal(const Vec3f& a, const Vec3f& b, const Vec3f& c)
	{
		const Vec3f U = b - a;
		const Vec3f V = c - a;

		return Vec3f{
			U[1] * V[2] - U[2] * V[1],
			U[2] * V[0] - U[0] * V[2],
			U[0] * V[1] - U[1] * V[0]
		};
	}
};

// --> MATH --> MISCELLANEOUS FUNCTIONS END
// --> MATH END

// --> D3D11 START
// --> D3D11 --> MISC STRUCTS START

enum class ShaderBindingType {
	VERTEX,
	PIXEL,
	BOTH
};

// --> D3D11 --> MISC STRUCTS END

// --> D3D11 --> BUFFERS START
// --> D3D11 --> BUFFERS --> INDEX BUFFER START

struct IndexBufferDescriptor
{
	const std::vector<uint32_t> indices;
};

class IndexBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	size_t m_nBytes;

public:
	IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const IndexBufferDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		this->m_nBytes = static_cast<uint64_t>(sizeof(uint32_t) * descriptor.indices.size());

		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = static_cast<UINT>(this->m_nBytes);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = descriptor.indices.data();

		HRESULT_ERROR(pDeviceRef->CreateBuffer(&ibd, &isd, &this->m_pIndexBuffer), "Unable To Create Index Buffer");
	}

	[[nodiscard]] size_t getSize() const
	{
		return this->m_nBytes;
	}

	void Bind() const noexcept
	{
		this->m_pDeviceContextRef->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}
};

// --> D3D11 --> BUFFERS --> INDEX  BUFFER END
// --> D3D11 --> BUFFERS --> VERTEX BUFFER START

struct VertexBufferDescriptor
{
	const void* memoryPtr    = nullptr;
	const size_t nElements   = 0;
	const size_t elementSize = 0;
	const bool isUpdatable   = false;
};

class VertexBuffer {
private:
	size_t nElements;
	size_t elementSize;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				 const VertexBufferDescriptor& descriptor)
		: nElements(descriptor.nElements), elementSize(descriptor.elementSize), m_pDeviceContextRef(pDeviceContextRef)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = (descriptor.isUpdatable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = (descriptor.isUpdatable) ? D3D11_CPU_ACCESS_WRITE : 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = static_cast<UINT>(this->elementSize * this->nElements);
		bd.StructureByteStride = static_cast<UINT>(this->elementSize);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = descriptor.memoryPtr;
		HRESULT_ERROR(pDeviceRef->CreateBuffer(&bd, &sd, &this->m_pVertexBuffer), "Unable To Create Vertex Buffer");
	}

	void SetData(const void* memoryPtr, const size_t nElements, const size_t elementSize) const noexcept {
		D3D11_MAPPED_SUBRESOURCE resource;
		this->m_pDeviceContextRef->Map(this->m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, memoryPtr, nElements * elementSize);
		this->m_pDeviceContextRef->Unmap(this->m_pVertexBuffer.Get(), 0);
	}

	void Bind() const noexcept
	{
		const UINT stride = static_cast<UINT>(this->elementSize);
		const UINT offset = 0u;

		this->m_pDeviceContextRef->IASetVertexBuffers(0u, 1u, this->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
};

// --> D3D11 --> BUFFERS --> VERTEX   BUFFER END
// --> D3D11 --> BUFFERS --> CONSTANT BUFFER START

struct ConstantBufferDescriptor
{
	const ShaderBindingType bindingType = ShaderBindingType::VERTEX;
	const size_t            objSize     = 0u;
	const UINT              slotVS      = 0u; // Ignored if ShaderBindingType is PIXEL
	const UINT              slotPS      = 0u; // Ignored if ShaderBindingType is VERTEX
};

class ConstantBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	ConstantBufferDescriptor m_descriptor;

public:
	ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				   const ConstantBufferDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.CPUAccessFlags = 0;
		cbd.ByteWidth = static_cast<UINT>(this->m_descriptor.objSize);

		cbd.StructureByteStride = 0u;
		HRESULT_ERROR(pDeviceRef->CreateBuffer(&cbd, nullptr, &this->m_pConstantBuffer), "Unable To Create Constant Buffer");
	}

	void setData(const void* objPtr)
	{
		this->m_pDeviceContextRef->UpdateSubresource(this->m_pConstantBuffer.Get(), 0, 0, objPtr, 0, 0);
	}

	void Bind() const noexcept
	{
		if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->VSSetConstantBuffers(this->m_descriptor.slotVS, 1u, this->m_pConstantBuffer.GetAddressOf());

		if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->PSSetConstantBuffers(this->m_descriptor.slotPS, 1u, this->m_pConstantBuffer.GetAddressOf());
	}
};

// --> D3D11 --> BUFFERS --> CONSTANT BUFFER END
// --> D3D11 --> BUFFERS END

// --> D3D11 --> SHADERS START
// --> D3D11 --> SHADER UTIL STRUCTS/ENUMS... START

enum class ShaderLoadingMethod {
	FROM_BINARY_FILE, // (.cso file)
	FROM_SOURCE_CODE  // (raw source code)
};

// --> D3D11 --> SHADER UTIL STRUCTS END
// --> D3D11 --> SHADERS --> PIXEL SHADER START

struct PixelShaderDescriptor
{
	const ShaderLoadingMethod loadingMethod;
	const wchar_t* binaryFilename;
	const char* sourceCode;
};

class PixelShader {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				const PixelShaderDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_BINARY_FILE)
		{
			HRESULT_ERROR(D3DReadFileToBlob(descriptor.binaryFilename, &pBlob), "Could Not Read Pixel Shader File");
		}
		else if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_SOURCE_CODE)
		{
			HRESULT_ERROR(D3DCompile(descriptor.sourceCode, strlen(descriptor.sourceCode), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pBlob, NULL), "Could Not Compile Pixel Shader");
		}
		else
		{
			MESSAGE_BOX_ERROR("Your Specified Shader Loading Method Is Not Supported From Pixel Shaders");
		}

		HRESULT_ERROR(pDeviceRef->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pPixelShader), "Could Not Create Pixel Shader");
	}

	void Bind() const noexcept
	{
		this->m_pDeviceContextRef->PSSetShader(this->m_pPixelShader.Get(), nullptr, 0u);
	}
};

// --> D3D11 --> SHADERS --> PIXEL  SHADER END
// --> D3D11 --> SHADERS --> VERTEX SHADER START

struct VertexShaderDescriptor
{
	const std::vector<std::pair<const char*, DXGI_FORMAT>> inputElementDescriptors = {};
	const ShaderLoadingMethod loadingMethod  = ShaderLoadingMethod::FROM_BINARY_FILE;
	const wchar_t*            binaryFilename = nullptr; // Ignore if ShaderLoadingMethod is FROM_SOURCE_CODE
	const char*               sourceCode     = nullptr; // Ignore if ShaderLoadingMethod is FROM_BINARY_FILE
};

class VertexShader {
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>    m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	VertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				 const VertexShaderDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_BINARY_FILE)
		{
			HRESULT_ERROR(D3DReadFileToBlob(descriptor.binaryFilename, &pBlob), "Could Not Read Vertex Shader File");
		}
		else if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_SOURCE_CODE)
		{
			HRESULT_ERROR(D3DCompile(descriptor.sourceCode, strlen(descriptor.sourceCode), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &pBlob, NULL), "Could Not Compile Vertex Shader");
		}
		else
		{
			MESSAGE_BOX_ERROR("Your Specified Shader Loading Method Is Not Supported From Vertex Shaders");
		}

		HRESULT_ERROR(pDeviceRef->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pVertexShader), "Could Not Create Vertex Shader");

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptors(descriptor.inputElementDescriptors.size());
		for (uint32_t i = 0; i < inputElementDescriptors.size(); i++)
		{
			inputElementDescriptors[i] = D3D11_INPUT_ELEMENT_DESC{};
			inputElementDescriptors[i].SemanticName = descriptor.inputElementDescriptors[i].first;
			inputElementDescriptors[i].Format = descriptor.inputElementDescriptors[i].second;
			inputElementDescriptors[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			inputElementDescriptors[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		}

		HRESULT_ERROR(pDeviceRef->CreateInputLayout(
			inputElementDescriptors.data(), (UINT)inputElementDescriptors.size(),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&m_pInputLayout
		), "Could Not Create Input Layout");
	}

	void Bind() const noexcept
	{
		this->m_pDeviceContextRef->IASetInputLayout(this->m_pInputLayout.Get());
		this->m_pDeviceContextRef->VSSetShader(this->m_pVertexShader.Get(), nullptr, 0u);
	}
};

// --> D3D11 --> SHADERS --> VERTEX SHADER END
// --> D3D11 --> SHADERS END
// --> D3D11 END

// --> MESHES START
// --> MESHES --> MESH START

struct Mesh {
	size_t vertexBufferIndex;
	size_t indexBufferIndex;

	size_t vsIndex;
	size_t psIndex;

	std::vector<size_t> textureIndices;
	std::vector<size_t> textureSamplerIndices;

	std::vector<size_t> constantBufferIndices;

	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyType;
};

// --> MESHES --> MESH END
// --> MESHES END

// --> TIMERS START

class Timer
{
private:
	std::chrono::system_clock::time_point m_start;

public:
	Timer()
	{
		this->m_start = std::chrono::system_clock::now();
	}

	[[nodiscard]] float getElapsedTimeMs()
	{
		const auto end = std::chrono::system_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - this->m_start);

		return static_cast<float>(elapsed.count() * 0.001f);
	}
};

// --> TIMERS END

// --> AUDIO START

namespace AudioHandler {
	inline void playSoundFromFile(const char* filename)
	{
		HRESULT_ERROR(mciSendString(("play " + std::string(filename)).c_str(), 0, NULL, NULL), "Could Not Play Audio File");
	}
};

// --> AUDIO END

// --> PERIPHERAL DEVICES START
// --> PERIPHERAL DEVICES --> PERIPHERAL DEVICE START

class PeripheralDevice
{
public:
	virtual void __onWindowUpdateBegin() = 0;
	virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void __onWindowUpdateEnd() = 0;
};

// --> PERIPHERAL DEVICES --> PERIPHERAL DEVICE END

// --> PERIPHERAL DEVICES --> MOUSE EVENT INTERFACES START

class MouseEventInterface
{
protected:
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonDownFunctors;

	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonDownFunctors;

	std::vector<std::function<void(const int16_t)>> m_onWheelTurnFunctors;

	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onMouseMoveFunctors;
	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onCursorMoveFunctors;

public:
	void onLeftButtonUp(const std::function<void(Vec2u16)>& functor)
	{
		this->m_onLeftButtonUpFunctors.push_back(functor);
	}

	void onLeftButtonDown(const std::function<void(Vec2u16)>& functor)
	{
		this->m_onLeftButtonDownFunctors.push_back(functor);
	}

	void onRightButtonUp(const std::function<void(Vec2u16)>& functor)
	{
		this->m_onRightButtonUpFunctors.push_back(functor);
	}

	void onRightButtonDown(const std::function<void(Vec2u16)>& functor)
	{
		this->m_onRightButtonDownFunctors.push_back(functor);
	}

	void onWheelTurn(const std::function<void(const int16_t)>& functor)
	{
		this->m_onWheelTurnFunctors.push_back(functor);
	}

	void onMouseMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor)
	{
		this->m_onMouseMoveFunctors.push_back(functor);
	}
	
	void onCursorMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor)
	{
		this->m_onCursorMoveFunctors.push_back(functor);
	}
};

// --> PERIPHERAL DEVICES --> MOUSE EVENT INTERFACES END

// --> PERIPHERAL DEVICES --> MOUSE START

class Mouse : PeripheralDevice, public MouseEventInterface
{
private:
	Vec2u16 m_position{ 0, 0 };
	Vec2i16 m_deltaPosition{ 0, 0 };

	int16_t m_wheelDelta = 0;

	bool m_isLeftButtonDown = false;
	bool m_isRightButtonDown = false;

	bool m_wasMouseMovedDuringUpdate = false;
	bool m_wasCursorMovedDuringUpdate = false;

	bool m_isCursorInWindow = false;

public:
	Mouse()
	{
		RAWINPUTDEVICE mouseInputDevice;
		mouseInputDevice.usUsagePage = 0x01;
		mouseInputDevice.usUsage = 0x02;
		mouseInputDevice.dwFlags = 0;
		mouseInputDevice.hwndTarget = nullptr;

		RegisterRawInputDevices(&mouseInputDevice, 1, sizeof(RAWINPUTDEVICE));
	}

	[[nodiscard]] bool isLeftButtonUp()   const
	{
		return !this->m_isLeftButtonDown;
	}

	[[nodiscard]] bool isLeftButtonDown() const
	{ 
		return this->m_isLeftButtonDown;
	}

	[[nodiscard]] bool isRightButtonUp()   const
	{
		return !this->m_isRightButtonDown;
	}

	[[nodiscard]] bool isRightButtonDown() const
	{
		return this->m_isRightButtonDown;
	}

	[[nodiscard]] bool isCursorInWindow() const
	{
		return this->m_isCursorInWindow;
	}

	void show() const
	{
		ShowCursor(true);
	}

	void hide() const
	{
		ShowCursor(false);
	}

	void clip(const RECT& rect) const
	{
		ClipCursor(&rect);
	}

	virtual void __onWindowUpdateBegin() override
	{
		this->m_wheelDelta = 0;
		this->m_deltaPosition = { 0, 0 };
		this->m_wasMouseMovedDuringUpdate = false;
		this->m_wasCursorMovedDuringUpdate = false;
	}

	virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
	{
		switch (msg)
		{
		case WM_INPUT:
		{
			UINT size = 0;

			if (!GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)))
			{
				std::vector<char> rawBuffer(size);

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) == size)
				{
					const RAWINPUT& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());

					if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
					{
						this->m_deltaPosition += Vec2i16{ static_cast<int16_t>(ri.data.mouse.lLastX), static_cast<int16_t>(ri.data.mouse.lLastY) };

						this->m_wasMouseMovedDuringUpdate = true;
					}
				}
			}

			return true;
		}

		break;
		case WM_MOUSELEAVE:
			this->m_isCursorInWindow = false;

			return true;
		case WM_MOUSEMOVE:
		{
			this->m_position = { static_cast<uint16_t>(GET_X_LPARAM(lParam)), static_cast<uint16_t>(GET_Y_LPARAM(lParam)) };
			this->m_wasCursorMovedDuringUpdate = true;
		}

		this->m_isCursorInWindow = true;

		return true;
		case WM_LBUTTONDOWN:
			this->m_isLeftButtonDown = true;

			for (auto& functor : this->m_onLeftButtonDownFunctors)
				functor(this->m_position);

			return true;
		case WM_LBUTTONUP:
			this->m_isLeftButtonDown = false;

			for (auto& functor : this->m_onLeftButtonUpFunctors)
				functor(this->m_position);

			return true;
		case WM_RBUTTONDOWN:
			this->m_isRightButtonDown = true;

			for (auto& functor : this->m_onRightButtonDownFunctors)
				functor(this->m_position);

			return true;
		case WM_RBUTTONUP:
			this->m_isRightButtonDown = false;

			for (auto& functor : this->m_onRightButtonUpFunctors)
				functor(this->m_position);

			return true;
		case WM_MOUSEWHEEL:
			this->m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
			return true;
		default:
			return false;
		}
	}

	virtual void __onWindowUpdateEnd() override
	{
		if (this->m_wasMouseMovedDuringUpdate)
			for (auto& functor : this->m_onMouseMoveFunctors)
				functor(this->m_position, this->m_deltaPosition);

		if (this->m_wasCursorMovedDuringUpdate)
			for (auto& functor : this->m_onCursorMoveFunctors)
				functor(this->m_position, this->m_deltaPosition);

		if (this->m_wheelDelta != 0)
			for (auto& functor : this->m_onWheelTurnFunctors)
				functor(this->m_wheelDelta);
	}
};

// --> PERIPHERAL DEVICES --> MOUSE END

// --> PERIPHERAL DEVICES --> KEYBOARD START

#define CHECK_BIT(var,pos) ((var) & ( 1 << (pos)))

class Keyboard : PeripheralDevice
{
private:
	std::vector<uint8_t> m_downKeys;

	std::vector<std::function<void(const uint8_t)>> m_onKeyUpFunctors;
	std::vector<std::function<void(const uint8_t)>> m_onKeyDownFunctors;

public:
	Keyboard()
	{

	}

	void onKeyUp(const std::function<void(const uint8_t)>& functor)
	{
		this->m_onKeyUpFunctors.push_back(functor);
	}

	void onKeyDown(const std::function<void(const uint8_t)>& functor)
	{
		this->m_onKeyDownFunctors.push_back(functor);
	}

	bool isKeyDown(const uint8_t key)
	{
		return this->m_downKeys.end() != std::find(this->m_downKeys.begin(), this->m_downKeys.end(), key);
	}

	virtual void __onWindowUpdateBegin() override
	{

	}

	virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
	{
		switch (msg)
		{
			case WM_KEYDOWN:
			{
				const uint8_t keyCode = static_cast<uint8_t>(wParam);

				// Verify that the key was not down before the message was sent
				if (!CHECK_BIT(lParam, 30))
				{
					this->m_downKeys.push_back(keyCode);

					for (auto& functor : this->m_onKeyDownFunctors)
						functor(keyCode);
				}

				return true;
			}
			case WM_KEYUP:
			{
				const uint8_t keyCode = static_cast<uint8_t>(wParam);

				this->m_downKeys.erase(std::remove(this->m_downKeys.begin(), this->m_downKeys.end(), keyCode), this->m_downKeys.end());

				for (auto& functor : this->m_onKeyUpFunctors)
					functor(keyCode);

				return true;
			}
		}

		return false;
	}

	virtual void __onWindowUpdateEnd()
	{

	}
};

// --> PERIPHERAL DEVICES --> KEYBOARD END

// --> PERIPHERAL DEVICES END
// --> WINDOW START

inline LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

struct WindowDescriptor
{
	const uint16_t windowPositionX = 0u;
	const uint16_t windowPositionY = 0u;
	const uint16_t width, height;
	const char* title      = nullptr;
	const char* iconPath   = nullptr;
	const bool isResizable = true;
	const HINSTANCE hInstance;
};

class Window
{
	friend LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

private:
	HWND m_handle = 0;
	HINSTANCE m_hinstance;

	Mouse m_mouse;
	Keyboard m_keyboard;

	bool m_isRunning = false;
	bool m_isMinimized = false;

	std::vector<std::function<void(const Vec2u16)>> m_onResizeFunctors;

public:
	Window(const WindowDescriptor& descriptor)
	{
		this->m_hinstance = descriptor.hInstance;

		const WNDCLASSA wc{
			CS_HREDRAW | CS_VREDRAW,
			WindowProcessMessages,
			0,
			0,
			this->m_hinstance,
			NULL,
			LoadCursor(nullptr, IDC_ARROW),
			(HBRUSH)COLOR_WINDOW, NULL,
			"WNDCLASSA"
		};

		if (RegisterClassA(&wc))
		{
			const uint32_t windowStyle = descriptor.isResizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

			RECT windowRect{ 0, 0, descriptor.width, descriptor.height };
			AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

			this->m_handle = CreateWindowA("WNDCLASSA", descriptor.title, windowStyle,
				descriptor.windowPositionX, descriptor.windowPositionY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
				NULL, NULL, descriptor.hInstance, NULL);

			ShowWindow(this->m_handle, SW_SHOW);
			UpdateWindow(this->m_handle);

			this->m_isRunning = true;

			if (descriptor.iconPath != nullptr)
				this->setIcon(descriptor.iconPath);
		}
	}

	// Misc
	void onResize(const std::function<void(const Vec2u16)>& functor)
	{
		this->m_onResizeFunctors.push_back(functor);
	}

	// Getters
	[[nodiscard]] bool isRunning() const
	{
		return this->m_isRunning;
	}

	Keyboard& getKeyboard()
	{
		return this->m_keyboard;
	}

	[[nodiscard]] Mouse&    getMouse()
	{
		return this->m_mouse;
	}

	[[nodiscard]] HWND      getHandle() const
	{
		return this->m_handle;
	}

	[[nodiscard]] RECT getWindowRect() const
	{
		RECT result; GetWindowRect(this->m_handle, &result);
		
		return result;
	}

	[[nodiscard]] RECT getClientRect() const
	{
		RECT result; GetClientRect(this->m_handle, &result);
		
		return result;
	}

	[[nodiscard]] uint16_t getWindowPositionX() const
	{
		const RECT rect = this->getWindowRect();
		
		return static_cast<uint16_t>(rect.left);
	}

	[[nodiscard]] uint16_t getWindowPositionY() const
	{
		const RECT rect = this->getWindowRect();
		
		return static_cast<uint16_t>(rect.top);
	}

	[[nodiscard]] uint16_t getWindowWidth()  const
	{
		const RECT rect = this->getWindowRect();
		
		return static_cast<uint16_t>(rect.right - rect.left);
	}

	[[nodiscard]] uint16_t getWindowHeight() const
	{
		const RECT rect = this->getWindowRect();
		
		return static_cast<uint16_t>(rect.bottom - rect.top);
	}

	[[nodiscard]] uint16_t getClientWidth()  const
	{
		const RECT rect = this->getClientRect();
		
		return static_cast<uint16_t>(rect.right);
	}
	
	[[nodiscard]] uint16_t getClientHeight() const
	{
		const RECT rect = this->getClientRect();
		
		return static_cast<uint16_t>(rect.bottom);
	}

	// Setters
	void setWindowSize(const uint16_t width, const uint16_t height)
	{
		SetWindowPos(this->m_handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
	}

	void setClientSize(const uint16_t width, const uint16_t height)
	{
		const uint16_t topBottomWindowPadding = this->getWindowHeight() - this->getClientHeight();
		const uint16_t leftRightWindowPadding = this->getWindowWidth() - this->getClientWidth();

		this->setWindowSize(width + leftRightWindowPadding, height + topBottomWindowPadding);
	}

	void setTitle(const char* title)
	{
		SetWindowTextA(this->m_handle, title);
	}

	void setIcon(const char* pathname)
	{
		const HICON hIcon = (HICON)LoadImage(NULL, pathname, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		ASSERT_ERROR(hIcon != NULL, "Could Not Load Icon");

		SendMessage(this->m_handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}

	// MESSAGE HANDLING
	void update()
	{
		this->m_mouse.__onWindowUpdateBegin();
		this->m_keyboard.__onWindowUpdateBegin();

		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		this->m_mouse.__onWindowUpdateEnd();
		this->m_keyboard.__onWindowUpdateEnd();
	}

	[[nodiscard]] LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_SIZE:
			{
				const Vec2u16 client_area_dimensions = {
					static_cast<uint16_t>(GET_X_LPARAM(lParam)),
					static_cast<uint16_t>(GET_Y_LPARAM(lParam))
				};

				this->m_isMinimized = (client_area_dimensions[0] == 0 && client_area_dimensions[1] == 0);

				for (auto& functor : this->m_onResizeFunctors)
					functor(client_area_dimensions);
			}

			return 0;
			case WM_DESTROY:
				this->destroy();
				
				return 0;
		}

		// Dispatch Message To Peripheral Devices
		if (this->m_mouse.__handleMessage(msg, wParam, lParam)) return 0;
		if (this->m_keyboard.__handleMessage(msg, wParam, lParam)) return 0;

		// Otherwise Let Windows Handle The Message
		return DefWindowProc(this->m_handle, msg, wParam, lParam);
	}

	// DESTROYING
	void destroy()
	{
		this->m_isRunning = !DestroyWindow(this->m_handle);
	}

	~Window()
	{
		this->destroy();
	}
};
// --> WINDOW END
// --> WINDOW MANAGER START

namespace WindowManager
{
	inline std::vector<Window> windows;

	inline Window* createWindow(const WindowDescriptor& descriptor)
	{
		windows.emplace_back(descriptor);

		return &windows[windows.size() - 1];
	}
};

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::vector<Window>::iterator it = std::find_if(WindowManager::windows.begin(), WindowManager::windows.end(), [&hwnd](const Window& window)
	{
		return window.m_handle == hwnd;
	});

	if (it != WindowManager::windows.end())
		return it->handleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// --> WINDOW MANAGER END

// --> CAMERAS START
// --> CAMERAS --> CAMERA START

class Camera
{
protected:
	DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	size_t m_transformConstantBufferIndex = 0;

public:
	Camera()
	{

	}

	[[nodiscard]] DirectX::XMMATRIX getTransform()           const noexcept
	{
		return this->m_transform;
	}

	[[nodiscard]] DirectX::XMMATRIX getTransposedTransform() const noexcept
	{
		return DirectX::XMMatrixTranspose(this->m_transform);
	}

	[[nodiscard]] Vec3f getPosition() const noexcept
	{
		return Vec3f{ this->m_position.m128_f32[0], this->m_position.m128_f32[1], this->m_position.m128_f32[2] };
	}

	[[nodiscard]] Vec3f getRotation() const noexcept
	{
		return Vec3f{ this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2] };
	}

	virtual void calculateTransform() = 0;
};

// --> CAMERAS --> CAMERA END
// --> CAMERAS --> ORTHOGRAPHIC CAMERA START 

struct OrthographicCameraDescriptor
{
	const Vec2f position;
	const float ratation;
};

class OrthographicCamera : public Camera
{
private:
	float m_InvAspectRatio = 0.0f;

public:
	OrthographicCamera(Window& window, const OrthographicCameraDescriptor& descriptor)
	{
		this->m_position = DirectX::XMVectorSet(descriptor.position[0], descriptor.position[1], 0.0f, 0.0f);
		this->m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, descriptor.ratation, 0.0f);

		auto recalculateInvAspectRatio = [this](const Vec2u16& clientDims)
		{
			this->m_InvAspectRatio = clientDims[1] / static_cast<float>(clientDims[0]);
		};

		recalculateInvAspectRatio({ window.getClientWidth(), window.getClientHeight() });

		window.onResize(recalculateInvAspectRatio);
	}

	void rotate(const float angle)
	{
		this->m_rotation.m128_f32[2] += angle;
	}

	void setPosition(const Vec2f& v)
	{
		this->m_position = DirectX::XMVectorSet(v[0], v[1], 0.0f, 0.0f);
	}

	void setRotation(const Vec2f& v)
	{
		this->m_rotation.m128_f32[0] += v[0];
		this->m_rotation.m128_f32[1] += v[1];
	}

	void translate(const Vec2f& v)
	{
		this->m_position.m128_f32[0] += v[0];
		this->m_position.m128_f32[1] += v[1];
	}

	virtual void calculateTransform() override
	{
		this->m_transform = DirectX::XMMatrixRotationZ(this->m_rotation.m128_f32[2]) // Rotate
			* DirectX::XMMatrixTranslation(-this->m_position.m128_f32[0], -this->m_position.m128_f32[1], 0.0f) // Translate
			* DirectX::XMMatrixScaling(this->m_InvAspectRatio, 1.0f, 1.0f); // Correct For Aspect Ratio
	}

	void handleKeyboardInputs(Keyboard& keyboard, const float speed, const char left, const char right, const char up, const char down)
	{
		if (keyboard.isKeyDown(right))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] + speed, this->m_position.m128_f32[1], 0.0f, 0.0f);
		if (keyboard.isKeyDown(left))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] - speed, this->m_position.m128_f32[1], 0.0f, 0.0f);

		if (keyboard.isKeyDown(up))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] + speed, 0.0f, 0.0f);
		if (keyboard.isKeyDown(down))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] - speed, 0.0f, 0.0f);
	}
};

// --> CAMERAS --> ORTHOGRAPHIC CAMERA END
// --> CAMERAS --> PERSPECTIVE  CAMERA START

struct PerspectiveCameraDescriptor
{
	const Vec3f position;
	const Vec3f rotation;

	const float fov;
	const float zNear;
	const float zFar;
};

class PerspectiveCamera : public Camera
{
private:
	const DirectX::XMVECTOR UP_VECTOR      = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR RIGHT_VECTOR   = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR m_forwardVector = FORWARD_VECTOR;
	DirectX::XMVECTOR m_rightVector   = RIGHT_VECTOR;

	float m_fov = 0.0f, m_aspectRatio = 0.0f, m_zNear = 0.0f, m_zFar = 0.0f;

public:
	PerspectiveCamera(Window& window, const PerspectiveCameraDescriptor& descriptor)
		: m_fov(descriptor.fov), m_zNear(descriptor.zNear), m_zFar(descriptor.zFar)
	{
		this->m_position = DirectX::XMVectorSet(descriptor.position[0], descriptor.position[1], descriptor.position[2], 0.0f);
		this->m_rotation = DirectX::XMVectorSet(descriptor.rotation[0], descriptor.rotation[1], descriptor.rotation[2], 0.0f);

		auto recalculateAspectRatio = [this](const Vec2u16& clientDims)
		{
			this->m_aspectRatio = clientDims[0] / static_cast<float>(clientDims[1]);
		};

		recalculateAspectRatio({ window.getClientWidth(), window.getClientHeight() });

		window.onResize(recalculateAspectRatio);
	}

	void translate(const Vec3f& v)
	{
		this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f));
	}

	void setPosition(const Vec3f& v)
	{
		this->m_position = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f);
	}

	void rotate(const Vec3f& v)
	{
		const DirectX::XMVECTOR rotationDeltaVector = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f);

		this->m_rotation = DirectX::XMVectorAdd(this->m_rotation, rotationDeltaVector);

		// UP-DOWN Rotation Limit
		if (this->m_rotation.m128_f32[0] > HALF_PI)
			this->m_rotation.m128_f32[0] = HALF_PI;

		if (this->m_rotation.m128_f32[0] < -HALF_PI)
			this->m_rotation.m128_f32[0] = -HALF_PI;
	}

	void setRotation(const Vec3f& v)
	{
		this->m_rotation = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f);

		// UP-DOWN Rotation Limit
		if (this->m_rotation.m128_f32[0] > HALF_PI)
			this->m_rotation.m128_f32[0] = HALF_PI;

		if (this->m_rotation.m128_f32[0] < -HALF_PI)
			this->m_rotation.m128_f32[0] = -HALF_PI;
	}

	virtual void calculateTransform() override
	{
		this->m_transform = getViewMatrix() * getProjectionMatrix();
	}

	[[nodiscard]] DirectX::XMMATRIX getViewMatrix()
	{
		const DirectX::XMMATRIX rotationYMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->m_rotation.m128_f32[1], 0.0f);

		this->m_forwardVector = DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationYMatrix);
		this->m_rightVector = DirectX::XMVector3TransformCoord(RIGHT_VECTOR, rotationYMatrix);

		const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2]);
		const DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorAdd(this->m_position, DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationMatrix));
		const DirectX::XMVECTOR upDirectionVec = DirectX::XMVector3TransformCoord(UP_VECTOR, rotationMatrix);

		return DirectX::XMMatrixLookAtLH(this->m_position, lookAtPosition, upDirectionVec);
	}

	[[nodiscard]] DirectX::XMMATRIX getProjectionMatrix()
	{
		return DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_zNear, m_zFar);
	}

	void handleMouseMovements(Mouse& mouse, const float sensitivity)
	{
		mouse.onMouseMove([sensitivity, this, &mouse](const Vec2u16 position, const Vec2i16 delta)
			{
				if (mouse.isCursorInWindow())
					this->rotate({ sensitivity * delta[1], sensitivity * delta[0], 0.0f });
			});
	}

	void handleKeyboardInputs(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
	{
		if (keyboard.isKeyDown(forward))
			this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
		if (keyboard.isKeyDown(backward))
			this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

		if (keyboard.isKeyDown(right))
			this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
		if (keyboard.isKeyDown(left))
			this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

		if (keyboard.isKeyDown(up))
			this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
		if (keyboard.isKeyDown(down))
			this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	}

	[[nodiscard]] Vec3f getKeyboardInputsDelta(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
	{
		DirectX::XMVECTOR delta = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		if (keyboard.isKeyDown(forward))
			delta = DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
		if (keyboard.isKeyDown(backward))
			delta = DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

		if (keyboard.isKeyDown(right))
			delta = DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
		if (keyboard.isKeyDown(left))
			delta = DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

		if (keyboard.isKeyDown(up))
			delta = DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
		if (keyboard.isKeyDown(down))
			delta = DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

		return Vec3f{ delta.m128_f32[0], delta.m128_f32[1], delta.m128_f32[2] };
	}
};

// --> CAMERAS --> PERSPECTIVE CAMERA END
// --> CAMERAS END

// --> IMAGES START
// --> IMAGES --> COLOR STRUCTS START

struct Coloru8
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

struct Colorf16
{
	float red;
	float green;
	float blue;
	float alpha;
};

// --> IMAGES --> COLOR STRUCTS END
// --> IMAGES --> IMAGE START

class Image {
private:
	std::unique_ptr<Coloru8[]> m_buff;

	uint16_t m_width   = 0, m_height = 0;
	uint32_t m_nPixels = 0;

public:
	Image(const wchar_t* filename)
	{
		Microsoft::WRL::ComPtr<IWICBitmapSource>      decodedConvertedFrame;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder>     bitmapDecoder;
		Microsoft::WRL::ComPtr<IWICImagingFactory>    factory;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frameDecoder;

		HRESULT_ERROR(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&factory)
		), "Could Not Create IWICImagingFactory");

		HRESULT_ERROR(factory->CreateDecoderFromFilename(
			filename,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&bitmapDecoder
		), "Could Not Read/Open Image");

		HRESULT_ERROR(bitmapDecoder->GetFrame(0, &frameDecoder), "Could Not Get First Frame Of Image");

		HRESULT_ERROR(frameDecoder->GetSize(
			(UINT*)&this->m_width,
			(UINT*)&this->m_height
		), "Could Not Get Image Width/Height");

		this->m_nPixels = this->m_width * this->m_height;

		HRESULT_ERROR(WICConvertBitmapSource(
			GUID_WICPixelFormat32bppRGBA,
			frameDecoder.Get(),
			&decodedConvertedFrame
		), "Could Not Create Bitmap Converter");

		this->m_buff = std::make_unique<Coloru8[]>(this->m_nPixels * sizeof(Coloru8));

		const WICRect sampleRect{ 0, 0, this->m_width, this->m_height };

		HRESULT_ERROR(decodedConvertedFrame->CopyPixels(
			&sampleRect,
			this->m_width * sizeof(Coloru8),
			this->m_nPixels * sizeof(Coloru8),
			(BYTE*)this->m_buff.get()
		), "Could Not Copy Pixels From Bitmap");
	}

	[[nodiscard]] uint16_t getWidth()   const {
		return this->m_width;
	}

	[[nodiscard]] uint16_t getHeight()  const {
		return this->m_height;
	}

	[[nodiscard]] uint32_t getNPixels() const {
		return this->m_nPixels;
	}

	[[nodiscard]] Coloru8 sample(const uint16_t x, const uint16_t y) const
	{
		return this->m_buff[y * this->m_width + x];
	}

	void    set(const uint16_t x, const uint16_t y, const Coloru8& color)
	{
		this->m_buff[y * this->m_width + x] = color;
	}

	[[nodiscard]] Coloru8* getBuffer() const
	{
		return this->m_buff.get();
	}
};

// --> IMAGES --> IMAGE END
// --> IMAGES --> TEXTURE 2D START

struct Texture2DDescriptor
{
	const ShaderBindingType bindingType;
	const Image& image;
	const UINT slotVS;
	const UINT slotPS;
	const bool useMipMaps;
};

class Texture2D {
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	Texture2DDescriptor m_descriptor;

public:
	Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const Texture2DDescriptor& descriptor) : m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
	{
		D3D11_SUBRESOURCE_DATA subResourceData;
		subResourceData.pSysMem = static_cast<void*>(descriptor.image.getBuffer());
		subResourceData.SysMemPitch = this->m_descriptor.image.getWidth() * sizeof(Coloru8);
		subResourceData.SysMemSlicePitch = 0;

		D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
		texture2DDescriptor.Width = this->m_descriptor.image.getWidth();
		texture2DDescriptor.Height = this->m_descriptor.image.getHeight();
		texture2DDescriptor.MipLevels = this->m_descriptor.useMipMaps ? 0 : 1;
		texture2DDescriptor.ArraySize = 1;
		texture2DDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDescriptor.SampleDesc.Count = 1;
		texture2DDescriptor.SampleDesc.Quality = 0;
		texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
		texture2DDescriptor.BindFlags = this->m_descriptor.useMipMaps ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
		texture2DDescriptor.MiscFlags = this->m_descriptor.useMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
		HRESULT_ERROR(pDeviceRef->CreateTexture2D(&texture2DDescriptor, this->m_descriptor.useMipMaps ? nullptr : &subResourceData, &texture2D), "Could Not Create Texture 2D");

		if (this->m_descriptor.useMipMaps)
			this->m_pDeviceContextRef->UpdateSubresource(texture2D.Get(), 0u, nullptr, subResourceData.pSysMem, subResourceData.SysMemPitch, 0u);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor = {};
		SRVDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDescriptor.Texture2D.MostDetailedMip = 0;
		SRVDescriptor.Texture2D.MipLevels = this->m_descriptor.useMipMaps ? -1 : 1;

		HRESULT_ERROR(pDeviceRef->CreateShaderResourceView(texture2D.Get(), &SRVDescriptor, this->m_pShaderResourceView.GetAddressOf()), "Could Not Create ShaderResourceView");

		if (this->m_descriptor.useMipMaps)
			this->m_pDeviceContextRef->GenerateMips(m_pShaderResourceView.Get());
	}

	void bind() const noexcept
	{
		if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->VSSetShaderResources(this->m_descriptor.slotVS, 1u, this->m_pShaderResourceView.GetAddressOf());

		if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->PSSetShaderResources(this->m_descriptor.slotPS, 1u, this->m_pShaderResourceView.GetAddressOf());
	}
};

// --> IMAGES --> TEXTURE 2D END
// --> IMAGES --> TEXTURE SAMPLER START

struct TextureSamplerDescriptor
{
	const ShaderBindingType bindingType;
	const UINT slotVS;
	const UINT slotPS;
	const D3D11_FILTER filter;
	const D3D11_TEXTURE_ADDRESS_MODE addressMode;
};

class TextureSampler {
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	TextureSamplerDescriptor m_descriptor;

public:
	TextureSampler(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const TextureSamplerDescriptor& descriptor) : m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
	{
		D3D11_SAMPLER_DESC samplerDescriptor;
		samplerDescriptor.Filter = descriptor.filter;
		samplerDescriptor.AddressU = descriptor.addressMode;
		samplerDescriptor.AddressV = descriptor.addressMode;
		samplerDescriptor.AddressW = descriptor.addressMode;
		samplerDescriptor.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
		samplerDescriptor.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDescriptor.MipLODBias = 0.0f;
		samplerDescriptor.MinLOD = 0.0f;
		samplerDescriptor.MaxLOD = D3D11_FLOAT32_MAX;

		pDeviceRef->CreateSamplerState(&samplerDescriptor, &this->m_pSamplerState);
	}

	void bind() const noexcept
	{
		if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->VSSetSamplers(this->m_descriptor.slotVS, 1u, this->m_pSamplerState.GetAddressOf());

		if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->PSSetSamplers(this->m_descriptor.slotPS, 1u, this->m_pSamplerState.GetAddressOf());
	}
};

// --> IMAGES --> TEXTURE SAMPLER END
// --> IMAGES END

// --> PHYSICS START
// --> PHYSICS --> PHYSICS OBJECT START

struct PhysicsObject
{
	Vec3f position{ 0.f, 0.f, 0.f };
	Vec3f velocity{ 0.f, 0.f, 0.f };
	Vec3f acceleration{ 0.f, 0.f, 0.f };

	void updatePhysicsObject(const float friction = 0.9f)
	{
		this->velocity += this->acceleration;
		this->velocity *= 0.9f;
		this->acceleration = Vec3f{ 0.f, 0.f, 0.f };

		this->position += this->velocity;
	}
};

// --> PHYSICS --> PHYSICS OBJECT END
// --> PHYSICS END

// --> ENGINE START
// --> ENGINE --> ENGINE DESCRIPTORS START
struct MeshDescriptorFromVertices
{
	const size_t vertexBufferIndex;
	const size_t indexBufferIndex;
	const size_t vertexShaderIndex;
	const size_t pixelShaderIndex;
	const std::vector<size_t> t2dIndices;
	const std::vector<size_t> tsIndices;
	const std::vector<size_t> constantBufferIndices;
	const D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

struct DataFromMeshFile
{
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
};

struct EngineDescriptor
{
	WindowDescriptor windowDesc;
	OrthographicCameraDescriptor orthographicCameraDesc;
	PerspectiveCameraDescriptor  perspectiveCameraDesc;
};

// --> ENGINE --> ENGINE DESCRIPTORS END
// --> ENGINE --> ENGINE CLASS START

class Engine
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOn;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOff;

	std::vector<size_t> renderPool2D;
	std::vector<size_t> renderPool3D;

	std::vector<Mesh>           meshes;
		
	std::vector<PixelShader>    pixelShaders;
	std::vector<VertexShader>   vertexShaders;

	std::vector<VertexBuffer>   vertexBuffers;
	std::vector<IndexBuffer>    indexBuffers;
	std::vector<ConstantBuffer> constantBuffers;

	std::vector<Texture2D>      textures;
	std::vector<TextureSampler> textureSamplers;

	Window*   window   = nullptr;
	Mouse*    mouse    = nullptr;
	Keyboard* keyboard = nullptr;
	
	OrthographicCamera* orthographicCamera = nullptr;
	PerspectiveCamera*  perspectiveCamera  = nullptr;

private:
	void createDeviceAndSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferDesc.Width  = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator   = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count   = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount  = 1;
		scd.OutputWindow = this->window->getHandle();
		scd.Windowed     = TRUE;
		scd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags        = 0;

		HRESULT_ERROR(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&scd,
			&m_pSwapChain,
			&m_pDevice,
			nullptr,
			&m_pDeviceContext
		), "Could Not Create Device And SwapChain");
	}

	void createRenderTarget()
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

		HRESULT_ERROR(this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer), "Could Not Get BackBuffer");
		HRESULT_ERROR(this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget), "Could Not Create RenderTargetView");
	}

	void createViewport()
	{
		D3D11_VIEWPORT vp;
		vp.Width    = static_cast<FLOAT>(this->window->getClientWidth());
		vp.Height   = static_cast<FLOAT>(this->window->getClientHeight());
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		this->m_pDeviceContext->RSSetViewports(1u, &vp);
	}

	void createDepthStencilStates()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc     = {};
		dsDesc.DepthEnable                  = TRUE;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS;
		dsDesc.StencilReadMask              = 0xFF;
		dsDesc.StencilWriteMask             = 0xFF;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
		
		HRESULT_ERROR(this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOn), "Could Not Create DepthStencilState");
		
		dsDesc.DepthEnable = FALSE;
		
		HRESULT_ERROR(this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOff), "Could Not Create EmptyDepthStencilState");
	}

	void createDepthStencil()
	{
		// Create Detph Texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width     = this->window->getClientWidth();
		descDepth.Height    = this->window->getClientHeight();
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format    = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count   = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage     = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		HRESULT_ERROR(this->m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil), "Could Not Create Texture2D");

		// Create Depth Stencil Texture View
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;

		HRESULT_ERROR(this->m_pDevice->CreateDepthStencilView(
			pDepthStencil.Get(), &descDSV, &this->m_pDepthStencilView
		), "Could Not Create DepthStencilView");
	}

	void bindDepthStencil()
	{
		this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
	}

	/*
	 * This function initiliazes directx and creates key components
	*/
	void initGraphics()
	{
		this->createDeviceAndSwapChain();
		this->createRenderTarget();
		this->createViewport();
		this->createDepthStencilStates();
		this->createDepthStencil();
		this->bindDepthStencil();
	}

	void turnZBufferOn()
	{
		this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOn.Get(), 1u);
	}

	void turnZBufferOff()
	{
		this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOff.Get(), 1u);
	}

	void drawMesh(const size_t meshIndex)
	{
		Mesh& mesh = this->meshes[meshIndex];

		this->vertexBuffers[mesh.vertexBufferIndex].Bind();
		this->indexBuffers[mesh.indexBufferIndex].Bind();
		this->vertexShaders[mesh.vsIndex].Bind();
		this->pixelShaders[mesh.psIndex].Bind();

		for (const size_t textureIndex : mesh.textureIndices)
			this->textures[textureIndex].bind();

		for (const size_t textureSamplerIndex : mesh.textureSamplerIndices)
			this->textureSamplers[textureSamplerIndex].bind();

		for (const size_t cbIndex : mesh.constantBufferIndices)
			this->constantBuffers[cbIndex].Bind();

		this->m_pDeviceContext->IASetPrimitiveTopology(mesh.primitiveTopologyType);
		this->m_pDeviceContext->DrawIndexed(static_cast<UINT>(this->indexBuffers[mesh.indexBufferIndex].getSize()), 0u, 0u);
	}

	/*
	 * This functions swaps the back buffer and the front buffer to show the frame to the user
	 * It also clears the depthStencilView
	 */
	void presentFrame(const bool useVSync)
	{
		HRESULT_ERROR(this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u), "Could Not Present Frame");

		// Clear Depth Stencil
		this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void createDefaultConstantBuffers()
	{
		const ConstantBufferDescriptor cbd = { ShaderBindingType::VERTEX, sizeof(DirectX::XMMATRIX), 0u, 0u };
		ASSERT_ERROR(WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX == this->createConstantBuffer(cbd), "Could Not Create Default Constant Buffer #0 In Target Position");
	}

public:
	Engine()
	{
		HRESULT_ERROR(CoInitialize(NULL), "Could Not Initialize COM");
	}

	~Engine()
	{
		// No Need To Destroy Window Because It Destroys Itself
		delete this->orthographicCamera;
		delete this->perspectiveCamera;
	}

	[[nodiscard]] Mesh&           getMesh          (const size_t index) noexcept { return this->meshes[index];          }
	[[nodiscard]] Texture2D&      getTexture       (const size_t index) noexcept { return this->textures[index];        }
	[[nodiscard]] VertexShader&   getVertexShader  (const size_t index) noexcept { return this->vertexShaders[index];   }
	[[nodiscard]] PixelShader&    getPixelShader   (const size_t index) noexcept { return this->pixelShaders[index];    }
	[[nodiscard]] ConstantBuffer& getConstantBuffer(const size_t index) noexcept { return this->constantBuffers[index]; }
	[[nodiscard]] VertexBuffer&   getVertexBuffer  (const size_t index) noexcept { return this->vertexBuffers[index];   }
	[[nodiscard]] IndexBuffer&    getIndexBuffer   (const size_t index) noexcept { return this->indexBuffers[index];    }
	[[nodiscard]] TextureSampler& getTextureSampler(const size_t index) noexcept { return this->textureSamplers[index]; }

	[[nodiscard]] Window&   getWindow()   noexcept { return *this->window;   }
	[[nodiscard]] Mouse&    getMouse()    noexcept { return *this->mouse;    }
	[[nodiscard]] Keyboard& getKeybaord() noexcept { return *this->keyboard; }
	[[nodiscard]] Engine&   getEngine()   noexcept { return *this;           }

	[[nodiscard]] OrthographicCamera& getOrthographicCamera() noexcept { return *this->orthographicCamera; }
	[[nodiscard]] PerspectiveCamera&  getPerspectiveCamera()  noexcept { return *this->perspectiveCamera;  }

	void init(const EngineDescriptor& descriptor)
	{
		this->window = WindowManager::createWindow(descriptor.windowDesc);

		this->mouse    = &(this->window->getMouse());
		this->keyboard = &(this->window->getKeyboard());

		this->orthographicCamera = new OrthographicCamera(*this->window, descriptor.orthographicCameraDesc);
		this->perspectiveCamera  = new PerspectiveCamera (*this->window, descriptor.perspectiveCameraDesc);

		this->initGraphics();
		this->createDefaultConstantBuffers();

		this->window->onResize([this](const Vec2u16 dimensions)
		{
			this->initGraphics();
		});
	}

	virtual void onRender(const float elapsed) = 0;

	void captureCursor()
	{
		// Clip Cursor
		const RECT windowRect = this->window->getWindowRect();
		const RECT clientRect = this->window->getClientRect();

		RECT boundingRect = windowRect;

		boundingRect.top    += (windowRect.bottom - windowRect.top) - clientRect.bottom + 10; // padding
		boundingRect.left   += 10; // padding
		boundingRect.right  -= 10; // padding
		boundingRect.bottom -= 10; // padding

		this->window->getMouse().clip(boundingRect);

		// Hide Cursor

		this->mouse->hide();
	}

	[[nodiscard]] size_t loadVertexShader(const VertexShaderDescriptor& descriptor)
	{
		this->vertexShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->vertexShaders.size() - 1;
	}

	[[nodiscard]] size_t loadPixelShader(const PixelShaderDescriptor& descriptor)
	{
		this->pixelShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->pixelShaders.size() - 1;
	}

	[[nodiscard]] size_t createTextureFromImage(const Texture2DDescriptor& descriptor)
	{
		this->textures.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->textures.size() - 1;
	}

	[[nodiscard]] size_t createTextureSampler(const TextureSamplerDescriptor& descriptor)
	{
		this->textureSamplers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->textureSamplers.size() - 1;
	}

	[[nodiscard]] size_t createConstantBuffer(const ConstantBufferDescriptor& descriptor)
	{
		this->constantBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->constantBuffers.size() - 1;
	}

	[[nodiscard]] size_t createVertexBuffer(const VertexBufferDescriptor& descriptor)
	{
		this->vertexBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);
	
		return this->vertexBuffers.size() - 1;
	}

	[[nodiscard]] size_t createIndexBuffer(const IndexBufferDescriptor& descriptor)
	{
		this->indexBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->indexBuffers.size() - 1;
	}

	[[nodiscard]] size_t createMeshFromVertices(const MeshDescriptorFromVertices& descriptor)
	{
		this->meshes.emplace_back(Mesh{
			descriptor.vertexBufferIndex,
			descriptor.indexBufferIndex,
			descriptor.vertexShaderIndex,
			descriptor.pixelShaderIndex,
			descriptor.t2dIndices,
			descriptor.tsIndices,
			descriptor.constantBufferIndices,
			descriptor.primitiveTopology
		});

		return this->meshes.size() - 1;
	}

	[[nodiscard]] DataFromMeshFile loadDataFromMeshFile(const char* filename)
	{
		std::ifstream infile(filename);

		std::vector<Vec3f>    vertices;
		std::vector<uint32_t> indices;

		std::string opcode;
		char junkChar;
		float junkFloat;
		std::string line;

		while (std::getline(infile, line))
		{
			std::istringstream iss(line);

			iss >> opcode;

			if (opcode == "v")
			{
				Vec3f vertex{ 0.f, 0.f, 0.f };

				iss >> vertex[0] >> vertex[1] >> vertex[2];

				vertices.push_back(vertex);
			}
			else if (opcode == "f")
			{
				const uint16_t spaceCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), ' '));
				const uint16_t slashCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), '/'));

				if (spaceCount == 3)
				{
					uint32_t i1 = 0, i2 = 0, i3 = 0;

					if (slashCount == 0)
						iss >> i1 >> i2 >> i3;
					else if (slashCount == 3)
						iss >> i1 >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> i3;
					else if (slashCount == 6)
						iss >> i1 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i3;

					indices.push_back(i1 - 1);
					indices.push_back(i2 - 1);
					indices.push_back(i3 - 1);
				}
			}
		}

		return { vertices, indices };
	}

	void queueMeshFor2dRendering(const size_t meshIndex)
	{
		this->renderPool2D.push_back(meshIndex);
	}

	void queueMeshFor3dRendering(const size_t meshIndex)
	{
		this->renderPool3D.push_back(meshIndex);
	}

	void fill(const Coloru8& color)
	{
		float colorf[4] = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f };

		this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&colorf);
	}

	void run(const bool useVSync = true, const uint16_t fps = 60)
	{
		ConstantBuffer& cameraTransformConstantBuffer = this->constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
		cameraTransformConstantBuffer.Bind();

		Timer timer;
		uint32_t frames = 0;

		while (this->window->isRunning())
		{
			const float elapsed = timer.getElapsedTimeMs();

			if (!useVSync)
			{
				if (elapsed >= 1 / static_cast<float>(fps) * 1000)
				{
					timer = Timer();
				}
				else
				{
					std::this_thread::yield();
					continue;
				}
			}

			this->window->update();

			this->renderPool2D.clear();
			this->renderPool3D.clear();

			this->onRender(elapsed);

			const DirectX::XMMATRIX perspectiveCameraTransposedTransform  = this->perspectiveCamera->getTransposedTransform();
			const DirectX::XMMATRIX orthographicCameraTransposedTransform = this->orthographicCamera->getTransposedTransform();

			this->turnZBufferOn();
			cameraTransformConstantBuffer.setData(&perspectiveCameraTransposedTransform);

			for (const size_t mesh3D : this->renderPool3D)
				this->drawMesh(mesh3D);

			this->turnZBufferOff();
			cameraTransformConstantBuffer.setData(&orthographicCameraTransposedTransform);

			for (const size_t mesh2D : this->renderPool2D)
				this->drawMesh(mesh2D);

			this->presentFrame(useVSync);
		}
	}
};
// --> ENGINE --> ENGINE CLASS END
// --> ENGINE END