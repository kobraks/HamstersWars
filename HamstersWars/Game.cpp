// Darren brown the heist 1,2,3,4

#include "Game.h"
#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <sfml/System.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Defines.h"
#include "Log.h"


#include "GlewException.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Model.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Program.h"
#include <iostream>
#include "LinkException.h"
#include "CompileException.h"


glm::vec3 pos(0.f, 1.f, 4.f);
glm::vec3 dir (0.f, 0.f, -1.f);
glm::vec3 up(0.f, 1.f, 0.f);

const float speed = 0.05f;
bool flyingMode = true;
bool captureMouse = false;

float velocity_vertical;
float velocity_horizontal;

float window_height, window_width;
std::shared_ptr<game::model::Model> modela;

void update()
{
	pos.x += dir.x * speed * velocity_vertical;
	pos.z += dir.z * speed * velocity_vertical;

	pos.x += dir.z * speed * velocity_horizontal;
	pos.z -= dir.x * speed * velocity_horizontal;

	if (flyingMode)
		pos.y += dir.y * speed * velocity_vertical;
	else
		pos.y = 0.5;

	velocity_vertical /= 1.2;
	velocity_horizontal /= 1.2;
}

const float TIME_STEP = 1.f / 60.f;

void game::Game::draw()
{
	auto window = reinterpret_cast<sf::Window*>(window_);
	window->setActive();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader_->use();
	shader_->get_parameter("Projection").set_value(camera_->get_projection());
	shader_->get_parameter("View").set_value(camera_->get_view());
	shader_->get_parameter("mode").set_value(1);

	modela->set_rotation(0, 0, 0);
	for (size_t i = 0; i < modela->count(); ++i)
	{
		auto mesh = modela->get_mesh(i);
		auto transform = modela->get_transform() * mesh->get_transform();

		shader_->get_parameter("Model").set_value(transform);
		mesh->draw();
	}

	modela->set_rotation(45.f, 0, 0);
	for (size_t i = 0; i < modela->count(); ++i)
	{
		auto mesh = modela->get_mesh(i);
		auto transform = modela->get_transform() * mesh->get_transform();

		shader_->get_parameter("Model").set_value(transform);
		mesh->draw();
	}

	SceneManager::draw();

	glFlush();
	window->display();
}

void game::Game::on_reshape(int width, int height)
{
	auto window = reinterpret_cast<sf::Window*>(window_);
	window->setActive();
	glViewport(0, 0, width, height);
	camera_->set_width(width);
	camera_->set_height(height);

	shader_->use();
	shader_->get_parameter("Projection").set_value(camera_->get_projection());
}

void game::Game::main_loop()
{
	auto window = reinterpret_cast<sf::Window*>(window_);
	sf::Clock timer;
	timer.restart();
	auto elapsed_time = 0.f;
	sf::Event event;

	while(is_runing)
	{
		if (!window->isOpen())
		{
			is_runing = false;
			break;
		}

		while(window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::Resized)
			{
				auto size = window->getSize();
				on_reshape(size.x, size.y);
			}

			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
				Keyboard::parse_event(event);

			if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.
				type == sf::Event::MouseEntered || event.type == sf::Event::MouseLeft || event.type == sf::Event::
				MouseMoved || event.type == sf::Event::MouseWheelMoved || event.type == sf::Event::MouseWheelScrolled)
				Mouse::parse_event(event);

		}

		elapsed_time += timer.restart().asSeconds();
		while (elapsed_time >= TIME_STEP)
		{
			update(TIME_STEP);

			elapsed_time -= TIME_STEP;
		}

		draw();
	}
}

void game::Game::update(const float& time_step)
{
	if (Keyboard::is_up(sf::Keyboard::Key::Escape))
		stop();

	if (Keyboard::is_down(sf::Keyboard::Key::W))
		velocity_vertical = 1;
	if (Keyboard::is_down(sf::Keyboard::Key::S))
		velocity_vertical = -1;

	if (Keyboard::is_down(sf::Keyboard::Key::A))
		velocity_horizontal = 1;
	if (Keyboard::is_down(sf::Keyboard::Key::D))
		velocity_horizontal = -1;
	if (Keyboard::is_up(sf::Keyboard::R))
	{
		delete shader_;
		auto shader = shader_ = load_program(SHADERS_PATH "texture");

		glActiveTexture(GL_TEXTURE0);

		shader->link();
		shader->use();

		shader->set_uniform(shader->get_uniform("my_texture"), GL_TEXTURE0);
		shader->set_uniform(shader->get_uniform("mode"), 1);
		shader_->get_parameter("Projection").set_value(camera_->get_projection());
	}


	if (Keyboard::is_up(sf::Keyboard::T))
	{
		if (captureMouse)
		{
			captureMouse = false;
			Mouse::set_visable(true);
			Mouse::set_grabbed(false);
		}
		else
		{
			captureMouse = true;
			Mouse::set_visable(false);
			Mouse::set_grabbed(true);
		}
	}

	if (captureMouse)
	{
		float theta = atan2(dir.z, dir.x);
		float phi = asin(dir.y);

		theta += (Mouse::get_position_x() - window_width / 2.f) * 0.01f;
		phi -= (Mouse::get_position_y() - window_height / 2.f) * 0.01f;

		dir.x = cos(theta) * cos(phi);
		dir.y = sin(phi);
		dir.z = sin(theta) * cos(phi);

		dir = glm::normalize(dir);
		auto right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
		up = glm::normalize(glm::cross(right, dir));

		Mouse::set_position(window_width / 2, window_height / 2);
	}

	::update();

	camera_->set_position(pos);
	camera_->set_target(pos + dir);
	camera_->set_up(up);

	SceneManager::update();

	Mouse::clear_buttons();
	Keyboard::clear_keys();
}

gl::Shader * game::Game::load_shader(const std::string & file, const TShader& type)
{
	gl::Shader* shader;

	switch(type)
	{
	case vertex:
		LOG(LOG_DEBUG, "Loading vertex shader form: %s", SHADERS_PATH "texture.vert");
		shader = new gl::Shader(gl::shader_type::Vertex);
		break;
	case fragment:
		LOG(LOG_DEBUG, "Loading fragment shader form: %s", SHADERS_PATH "texture.vert");
		shader = new gl::Shader(gl::shader_type::Fragment);
		break;
	case geometry:
		LOG(LOG_DEBUG, "Loading geometry shader form: %s", SHADERS_PATH "texture.vert");
		shader = new gl::Shader(gl::shader_type::Geometry);
		break;
	}

	try
	{
		shader->load_source_form_file(file);
		LOG(LOG_DEBUG, "Compiling shader");
		shader->compile();
	}
	catch(gl::exception::CompileException& ex)
	{
		LOG(LOG_ERROR, "Unable to compile shader: %s", ex.what());
		delete shader;
		return nullptr;
	}
	catch(std::exception& ex)
	{
		LOG(LOG_ERROR, "Unable to ready shader source: %s", ex.what());
		delete shader;
		return nullptr;
	}

	return shader;
}

gl::Program * game::Game::generate_program(gl::Shader * vertex, gl::Shader * fragment, gl::Shader * geometry)
{
	auto program = new gl::Program();

	if (vertex)
	{
		LOG(LOG_DEBUG, "Attaching vertex shader");
		program->attach(*vertex);
	}
	else
		LOG(LOG_WARNING, "No vertex shader");

	if (vertex)
	{
		LOG(LOG_DEBUG, "Attaching fragment shader");
		program->attach(*fragment);
	}
	else
		LOG(LOG_WARNING, "No fragment shader");

	if (vertex)
	{
		LOG(LOG_DEBUG, "Attaching geometry shader");
		program->attach(*geometry);
	}
	else
		LOG(LOG_WARNING, "No geometry shader");

	try
	{
		LOG(LOG_DEBUG, "Linking the shader");
		program->link();
	}
	catch (gl::exception::LinkException& ex)
	{
		LOG(LOG_ERROR, "Unable to link shaders %s", ex.what());

		delete program;
		program = nullptr;
	}

	return program;
}

gl::Program * game::Game::load_program(const std::string & file)
{
	gl::Shader* vertex_shader = load_shader(file + ".vert", vertex);
	gl::Shader* fragment_shader = load_shader(file + ".frag", fragment);
	gl::Shader* geometry_shader= load_shader(file + ".geo", geometry);

	gl::Program* program = generate_program(vertex_shader, fragment_shader, geometry_shader);

	delete vertex_shader;
	delete fragment_shader;
	delete geometry_shader;

	return program;
}

game::Game* game::Game::get_instance()
{
	static Game instance;
	return &instance;
}

void game::Game::initialize(int argc, char** argv, const char* window_name, const sf::Vector2i& position, const int& width, const int& height)
{
	if (fopen_s(&Output2File::stream(), LOG_FILE, "w"))
	{
		Output2File::stream() = stderr;
		LOG(LOG_ERROR, "Unable to open %s", LOG_FILE);
	}

	auto window = new sf::Window(sf::VideoMode(width, height), window_name, sf::Style::Default, sf::ContextSettings(32));
	window->setPosition(position);

	get_instance()->window_ = window;
	
	window->setVerticalSyncEnabled(true);
	if (!window->setActive())
		LOG(LOG_ERROR, "Unable to set valid opengl contex");


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);

	glewExperimental = GL_TRUE;
	GLenum glew_init_result;
	if (GLEW_OK != (glew_init_result = glewInit()))
		throw gl::exception::GlewException(glew_init_result);

	Keyboard::initialize(window);
	Mouse::initialize(window, true, false);
	Mouse::set_position(width / 2, height / 2);

	window_width = width;
	window_height = height;

	get_instance()->camera_ = new gl::Camera(width, height, pos, pos + dir, glm::vec3(0, 1, 0), 60.f);

	LOG(LOG_INFO, "Loading texture shader");
	auto shader = get_instance()->shader_ = load_program(SHADERS_PATH"texture");
	if (!shader)
		exit(EXIT_FAILURE);

	glActiveTexture(GL_TEXTURE0);

	modela = model::ModelLoader::load(MODELS_PATH"cow.3DS");
	modela->set_scale(glm::vec3(0.005f));

	shader->use();

	shader->set_uniform(shader->get_uniform("my_texture"), GL_TEXTURE0);
	shader->set_uniform(shader->get_uniform("mode"), 1);

	SceneManager::initialize(*shader, [](gl::Program& shader, const game::Drawable*)
	{
		/*shader.get_parameter("mode")->set_value(1);

		for (size_t i = 0; i < modela->count(); ++i)
		{
			auto mesh = modela->get_mesh(i);
			shader.get_parameter("model")->set_value(mesh->get_model_matrix());
			mesh->draw();

			shader.get_parameter("mode")->set_value(-1);
			shader.get_parameter("model")->set_value(mesh->bounding_box()->get_model_matrix());
			mesh->bounding_box()->draw();
		}*/
	});

	SceneManager::load_from_file(LUA_SCRIPTS_PATH"entities.lua");

}

void game::Game::run()
{
	get_instance()->is_runing = true;
	get_instance()->main_loop();
}

void game::Game::stop()
{
	get_instance()->is_runing = false;
}

void game::Game::close()
{
	reinterpret_cast<sf::Window*>(get_instance()->window_)->close();
}

game::Game::Game()
{
	log_file_.open(LOG_FILE, std::ios::trunc | std::ios::out);
	log_file_ << "Start logging" << std::endl;
}

game::Game::~Game()
{
	log_file_.close();
}
