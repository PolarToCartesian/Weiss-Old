#pragma once

#include "../objects/Mesh.h"
#include "../objects/Triangles.h"
#include "../buffers/VertexBuffer.h"

class EngineCore;

template <typename V>
class BatchRenderer
{
protected:
	std::vector<Triangle<V>> m_triangles;

	size_t m_vertexShaderIndex = WEISS_NO_RESOURCE_INDEX;
	size_t m_pixelShaderIndex  = WEISS_NO_RESOURCE_INDEX;
	size_t m_vertexBufferIndex = WEISS_NO_RESOURCE_INDEX;

	std::vector<size_t> m_meshes;
	
	EngineCore& m_engine;

protected:
	// Defined Later (__WEISS_LAST_INCLUDE) Because It Uses The Engine Class Before Its Declaration
	void CreateNewMeshesIfNeeded();

	// Defined Later (__WEISS_LAST_INCLUDE) Because It Uses The Engine Class Before Its Declaration
	void FillMeshesIfNeeded();

private:
	bool m_wasModified = false;

public:
	// Defined Later (__WEISS_LAST_INCLUDE) Because It Uses The Engine Class Before Its Declaration
	BatchRenderer(EngineCore& engine, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* vsSource, const char* psSource);

	Triangle<V>& getTriangle(const size_t index) noexcept
	{
		this->m_wasModified = true;
		
		return this->m_triangles[index];
	}

	size_t addTriangle(const Triangle<V>& tr) noexcept
	{
		this->m_triangles.push_back(tr);

		this->m_wasModified = true;

		return this->m_triangles.size() - 1u;
	}

	void Draw()
	{
		this->CreateNewMeshesIfNeeded();
		this->FillMeshesIfNeeded();

		for (size_t i = 0u; i < this->m_meshes.size(); i++)
		{
			const UINT nVerticesToDraw = static_cast<UINT>((i != this->m_meshes.size() - 1u) ? WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER : this->m_triangles.size() * 3u - i * WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER);

			this->m_engine.DrawMesh(this->m_meshes[i], nVerticesToDraw);
		}
	}
};