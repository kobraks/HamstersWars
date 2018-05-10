#include "ModelLoader.h"
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TextureLoader.h"

#include <vector>

#include "Log.h"
#include "Mesh.h"
#include "Defines.h"

model::Model* model::ModelLoader::load(const std::string& file)
{
	auto importer = new Assimp::Importer();
	const aiScene* scene = importer->ReadFile(file, aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenNormals | aiProcess_SplitLargeMeshes);


	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::level() = Log::log_error;
		Log::print(importer->GetErrorString());
		return nullptr;
	}

	Log::level() = Log::log_info;
	printf("%s", file.c_str());
	Log::print("%s", file.c_str());
	Log::print("Import of scene %s succeeded", file.c_str());

	const auto meshes = scene->mNumMeshes;

	Log::print("meshes: %u", meshes);

	std::vector<Mesh*> model_meshes;

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		auto mesh = new Mesh(*scene->mMeshes[i]);
		//mesh->set_local_world(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)));

		const auto& material = *scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

		Texture2d* texture = nullptr;
		aiColor4D albedo;
		float specular_intensity;
		float specular_power;

		material.Get(AI_MATKEY_COLOR_DIFFUSE, albedo);
		material.Get(AI_MATKEY_SHININESS_STRENGTH, specular_intensity);
		material.Get(AI_MATKEY_SHININESS, specular_power);

		auto texture_file = new aiString();
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, texture_file) == AI_SUCCESS)
			texture = TextureLoader::load_texture(TEXTURES_PATH + std::string(texture_file->C_Str()));
		delete texture_file;

		mesh->set_material(Material(albedo, specular_intensity, specular_power, texture));
		model_meshes.push_back(mesh);
	}

	delete importer;

	return new Model(model_meshes);
}