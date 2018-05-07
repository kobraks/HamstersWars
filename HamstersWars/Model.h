#pragma once

#include <vector>
namespace model
{
	class Mesh;

	class Model
	{
	public:
		explicit Model(std::vector<Mesh*> meshes);
		~Model();

		void draw() const;
		void draw_mesh(unsigned mesh) const;

		Mesh* get_mesh(unsigned mesh) const;
		size_t size() const;

	private:
		std::vector<Mesh*> meshes_;
	};

}
