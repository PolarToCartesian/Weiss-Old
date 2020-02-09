#pragma once

#include "../misc/Pch.h"

constexpr const size_t WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE      = 1024u;
constexpr const size_t WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER  = 600u;
constexpr const size_t WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER   = 3u * WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER;
constexpr const size_t WEISS_NO_RESOURCE_INDEX                      = std::numeric_limits<size_t>::max();

constexpr const size_t WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX = 0u;
constexpr const size_t WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_SLOT  = 0u;

constexpr const size_t WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_INDEX = 1u;
constexpr const size_t WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_SLOT  = 1u;

constexpr const uint32_t WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_TRANSFORM = 1u << 0u;
constexpr const uint32_t WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_LIGHTING  = 1u << 1u;

constexpr const uint32_t WEISS_COLORED_BATCH_RENDERER_2D_ALL_FLAGS = WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_TRANSFORM | WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_LIGHTING;

constexpr const uint16_t WEISS_SPRITE_SHEET_SIDE_LENGTH = 1024u; // 2^10

constexpr const char* WEISS_COLORED_BATCH_2D_RENDERER_VS_SOURCE = ""
"cbuffer cbuff0 { matrix transform; }\n"
"cbuffer cbuff1 { float4 ambiant; }\n"
"struct VSoutput { float4 out_color : Color; float4 out_positionSV : SV_Position; };\n"
"VSoutput main(float2 in_position : Position, float4 in_color : Color, uint in_flags : Flags) {"
	"VSoutput output;\n"
	"output.out_color = in_color;\n"
	"output.out_positionSV = float4(in_position, 0.0f, 1.0f);\n"
	"if ((in_flags & 2) > 0) { output.out_color = output.out_color + ambiant; }\n"
	"if ((in_flags & 1) > 0) { output.out_positionSV = mul(output.out_positionSV, transform); }\n"
	"return output;\n"
"}";

constexpr const char* WEISS_COLORED_BATCH_2D_RENDERER_PS_SOURCE = ""
"float4 main(float4 color : Color) : SV_TARGET {"
	"return color;\n"
"}";

const std::vector<std::pair<const char*, DXGI_FORMAT>> WEISS_COLORED_BATCH_2D_RENDERER_IEDS =
{
	{ "Position", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT   },
	{ "Color",    DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM },
	{ "Flags",    DXGI_FORMAT::DXGI_FORMAT_R32_UINT       },
};