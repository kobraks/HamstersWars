#pragma once

#include <vector>
#include <memory>
#include <iterator>

#include "Vector3D.h"
#include "Transformable.h"

namespace game::model
{
	//TODO iterator

	class Mesh;

	class Model : public Transformable
	{
		typedef std::shared_ptr<Mesh> mesh_pointer;
		typedef std::vector<mesh_pointer> table_type;

	public:
		typedef size_t size_type;

		class ModelIterator
		{
		public:

			typedef ModelIterator self_type;
			typedef Mesh value_type;
			typedef Mesh& reference;
			typedef Mesh* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit ModelIterator(table_type::iterator iterator);

			self_type operator++();
			self_type operator++(int i);

			self_type operator--();
			self_type operator--(int i);

			reference operator*() const;
			pointer operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			table_type::iterator current_;
		};

		class ConstModelIterator
		{
		public:
			typedef ConstModelIterator self_type;
			typedef Mesh value_type;
			typedef Mesh& reference;
			typedef Mesh* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit ConstModelIterator(table_type::const_iterator iterator);

			self_type operator++();
			self_type operator++(int i);

			self_type operator--();
			self_type operator--(int i);

			const reference operator*() const;
			const pointer operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			table_type::const_iterator current_;
		};

		Model();
		explicit Model(std::vector<std::shared_ptr<Mesh>> meshes);
		Model(const Model& model);
		Model& operator=(const Model& model);

		~Model();

		ModelIterator begin() noexcept;
		ModelIterator end() noexcept;

		ConstModelIterator begin() const noexcept;
		ConstModelIterator end() const noexcept;

		void draw() const;
		void draw_mesh(unsigned mesh) const;

		mesh_pointer get_mesh(unsigned mesh) const;
		size_type count() const;

		void add_mesh(const Mesh& mesh);

		bool colide(const Model& model);

		gl::Vector3D size() const;
	private:
		table_type meshes_;

		mutable gl::Vector3D size_;
		mutable bool size_needs_update_ = true;
	};

}
