#pragma once

#include "../batch/BatchRenderer.h"

template <typename V>
BatchRenderer<V>::BatchRenderer(EngineCore& engine, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* vsSource, const char* psSource)
	: m_engine(engine)
{
	const VertexShaderDescriptor vsd = { ieds, ShaderLoadingMethod::FROM_SOURCE_CODE, nullptr, vsSource };
	this->m_vertexShaderIndex = this->m_engine.CreateVertexShader(vsd);

	const PixelShaderDescriptor psd = { ShaderLoadingMethod::FROM_SOURCE_CODE, nullptr, psSource };
	this->m_pixelShaderIndex = this->m_engine.CreatePixelShader(psd);
}

template <typename V>
void BatchRenderer<V>::CreateNewMeshesIfNeeded() {
	const size_t nMeshesToAdd = static_cast<UINT>(std::ceil((this->m_triangles.size() * 3u) / (float)WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER) - this->m_meshes.size());

	for (size_t i = 0u; i < nMeshesToAdd; i++)
	{
		V junk[WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER] = {  };

		const VertexBufferDescriptor vbd{ junk, WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER, sizeof(V), true };
		this->m_vertexBufferIndex = this->m_engine.CreateVertexBuffer(vbd);

		Drawable drawable{ this->m_vertexBufferIndex, this->m_vertexShaderIndex, this->m_pixelShaderIndex };

		this->m_meshes.push_back(this->m_engine.AddDrawable(drawable));
	}
}

template <typename V>
void BatchRenderer<V>::FillMeshesIfNeeded()
{
	if (this->m_wasModified)
	{
		for (size_t i = 0u; i < this->m_meshes.size(); i++)
		{
			Drawable& drawable = this->m_engine.GetDrawable(this->m_meshes[i]);
			VertexBuffer& vertexBuffer = this->m_engine.GetVertexBuffer(drawable.vertexBufferIndex);

			const void* srcPtr = this->m_triangles.data() + i * WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER;

			vertexBuffer.SetData(srcPtr, WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER);
		}

		this->m_wasModified = false;
	}
}