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
#include <iostream>

inline glm::vec3 convert(aiColor3D color)
{
	return glm::vec3(color.r, color.g, color.b);
}

inline glm::vec4 convert(aiColor4D color)
{
	return glm::vec4(color.r, color.g, color.b, color.a);
}

std::shared_ptr<game::model::Model> game::model::ModelLoader::load(const std::string& file)
{
	auto importer = new Assimp::Importer();
	const aiScene* scene = importer->ReadFile(
		file, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenNormals
		| aiProcess_SplitLargeMeshes);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::level() = Log::log_error;
		Log::print(importer->GetErrorString());
		return nullptr;
	}

	Log::level() = Log::log_info;
	Log::print("%s", file.c_str());
	Log::print("Import of scene %s succeeded", file.c_str());

	const auto meshes = scene->mNumMeshes;
	Log::print("meshes: %u", meshes);
	std::vector<std::shared_ptr<Mesh>> model_meshes(meshes);


	for (size_t i = 0; i < scene->mNumMeshes; ++i)
		model_meshes[i] = process_mesh(scene, i);

	delete importer;

	return std::make_shared<Model>(model_meshes);
}

std::shared_ptr<game::model::Mesh> game::model::ModelLoader::process_mesh(const void* scene, const size_t& index)
{
	aiMesh* mesh = static_cast<const aiScene*>(scene)->mMeshes[index];
	auto result = std::make_shared<Mesh>(*mesh);

	result->set_material(process_material(mesh, scene));

	return result;
}

game::model::Material game::model::ModelLoader::process_material(void* mesh, const void* scene)
{
	Material result;
	const aiScene* s = static_cast<const aiScene*>(scene);
	auto m = static_cast<aiMesh*>(mesh);

	const auto& material = *s->mMaterials[m->mMaterialIndex];
	aiColor4D ambient_color;
	aiColor4D diffuse_color;
	aiColor4D specular_color;
	float shininess;

	material.Get(AI_MATKEY_COLOR_AMBIENT, ambient_color);
	material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
	material.Get(AI_MATKEY_COLOR_SPECULAR, specular_color);
	material.Get(AI_MATKEY_SHININESS, shininess);

	result.ambient = convert(ambient_color);
	result.diffuse = convert(diffuse_color);
	result.specular = convert(specular_color);
	result.shininess = shininess;

	aiString texture_file;
	if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texture_file) == AI_SUCCESS)
		result.texture = TextureLoader::load_texture(TEXTURES_PATH + std::string(texture_file.C_Str()));
	else
		result.texture = TextureLoader::load_texture(DEFAULT_TEXTURE);

	return result;
}
