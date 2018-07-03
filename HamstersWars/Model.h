#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Transformable.h"

namespace game::model
{
	//TODO iterator

	class Mesh;

	class Model : public Transformable
	{
	public:
		Model();
		explicit Model(std::vector<std::shared_ptr<Mesh>> meshes);
		Model(const Model& model);
		Model& operator=(const Model& model);

		~Model();

		void draw() const;
		void draw_mesh(unsigned mesh) const;

		std::shared_ptr<Mesh> get_mesh(unsigned mesh) const;
		size_t count() const;

		void add_mesh(const Mesh& mesh);

		bool colide(const Model& model);

		glm::vec3 size() const;
	private:
		std::vector<std::shared_ptr<Mesh>> meshes_;

		mutable glm::vec3 size_;
		mutable bool size_needs_update_ = true;
	};

}
