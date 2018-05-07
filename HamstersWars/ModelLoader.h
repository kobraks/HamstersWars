#pragma once
#include <string>

#include "Model.h"

namespace model
{
	class ModelLoader
	{
	public:
		ModelLoader() = delete;
		ModelLoader(const ModelLoader&) = delete;
		ModelLoader(ModelLoader&&) = delete;

		ModelLoader& operator=(const ModelLoader&) = delete;
		ModelLoader& operator=(ModelLoader&&) = delete;

		static Model* load(const std::string& file);
	private:
	};
}

