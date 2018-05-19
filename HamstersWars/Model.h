#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace model
{
	class Mesh;

	class Model
	{
	public:
		Model();
		explicit Model(std::vector<Mesh*> meshes);
		Model(const Model& model);
		Model& operator=(const Model& model);

		~Model();

		void draw() const;
		void draw_mesh(unsigned mesh) const;

		Mesh* get_mesh(unsigned mesh) const;
		size_t count() const;

		void add_mesh(const Mesh& mesh);

		bool colide(const Model& model);

		void translate(const glm::vec3& axis);
		void translate(const float& x, const float& y, const float& z);
		void rotate(const float& angle, glm::vec3 axis);
		void rotate(const float& angle, const float& x, const float& y, const float& z);
		void scale(const glm::vec3& scale);
		void scale(const float& x, const float& y, const float& z);

		void set_matrix(const glm::mat4& matrix);
		glm::mat4 get_matrix() const;
	private:
		std::vector<Mesh*> meshes_;
	};

}
