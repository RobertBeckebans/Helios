#pragma once

#include <glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

namespace lumen
{
	class Material;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	struct Triangle
	{
		uint32_t v0;
		uint32_t v1;
		uint32_t v2;
		uint32_t mat_id;
	};

	struct SubMesh
    {
        uint32_t  material_index;
        uint32_t  index_count;
        uint32_t  base_vertex;
        uint32_t  base_index;
    };

	class Mesh
	{
	public:
		static std::shared_ptr<Mesh> create(const std::string& path);

		std::vector<Vertex>					   m_vertices;
		std::vector<uint32_t>				   m_indices;
		std::vector<SubMesh>				   m_sub_meshes;
		std::vector<std::shared_ptr<Material>> m_materials;

		static std::unordered_map<std::string, std::weak_ptr<Mesh>> m_cache;
	};
}