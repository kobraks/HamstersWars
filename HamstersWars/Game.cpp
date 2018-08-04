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


glm::vec3 pos(0.f, 1.f, 4.f);
glm::vec3 dir (0.f, 0.f, -1.f);
glm::vec3 up(0.f, 1.f, 0.f);
glm::vec2 mousePosition;

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

	glViewport(0, 0, width, height);
	camera_->set_width(width);
	camera_->set_height(height);

	shader_->use();
	shader_->get_parameter("Projection").set_value(camera_->get_projection());
}

void game::Game::on_timer(int id)
{
	if (Keyboard::is_up(27))
		stop();

	if (Keyboard::is_down('w'))
		velocity_vertical = 1;
	if (Keyboard::is_down('s'))
		velocity_vertical = -1;

	if (Keyboard::is_down('a'))
		velocity_horizontal = 1;
	if (Keyboard::is_down('d'))
		velocity_horizontal = -1;
	if (Keyboard::is_up('r'))
	{
		delete shader_;
		shader_ = nullptr;

		Log::level() = Log::log_info;
		Log::print("Loading vertex shader");
		gl::Shader vertex(gl::shader_type::Vertex);
		vertex.load_source_form_file(SHADERS_PATH "texture.vert");
		vertex.compile();

		Log::level() = Log::log_info;
		Log::print("Loading fragment shader");
		gl::Shader fragment(gl::shader_type::Fragment);
		fragment.load_source_form_file(SHADERS_PATH"texture.frag");
		fragment.compile();

		auto shader = get_instance()->shader_ = new gl::Program(vertex, fragment);

		glActiveTexture(GL_TEXTURE0);


		shader->link();
		shader->use();

		shader->set_uniform(shader->get_uniform("my_texture"), GL_TEXTURE0);
		shader->set_uniform(shader->get_uniform("mode"), 1);
		shader_->get_parameter("Projection").set_value(camera_->get_projection());
	}


	if (Keyboard::is_up('t'))
		if (captureMouse)
		{
			captureMouse = false;
			Mouse::set_event_on_mouse_move(nullptr);
			Mouse::set_cursor(GLUT_CURSOR_INHERIT);


		}
		else
		{
			captureMouse = true;
			Mouse::set_event_on_mouse_move([](const int& x, const int& y) { captureMouse = true; mousePosition.x = x; mousePosition.y = y; });
			Mouse::set_cursor(GLUT_CURSOR_NONE);

		}


	if (captureMouse)
	{
		float theta = atan2(dir.z, dir.x);
		float phi = asin(dir.y);

		theta += (mousePosition.x - window_width / 2) * 0.01;
		phi -= (mousePosition.y - window_height / 2) * 0.01;

		dir.x = cos(theta) * cos(phi);
		dir.y = sin(phi);
		dir.z = sin(theta) * cos(phi);

		dir = glm::normalize(dir);
		auto right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
		up = glm::normalize(glm::cross(right, dir));

		glutWarpPointer(window_width / 2, window_height / 2);
	}

	::update();

	camera_->set_position(pos);
	camera_->set_target(pos + dir);
	camera_->set_up(up);

	SceneManager::update();
	Mouse::clear_buttons();
	Keyboard::clear_keys();
}

void game::Game::main_loop()
{
	auto window = reinterpret_cast<sf::Window*>(window_);
	sf::Clock timer;
	auto elapsed_time = 0.f;

	while(window->isOpen())
	{
		sf::Event event;

		while(window->pollEvent(event))
		{
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

		elapsed_time += timer.restart().asMilliseconds();
		while (elapsed_time >= TIME_STEP)
		{
			update(TIME_STEP);

			elapsed_time -= TIME_STEP;
		}

		draw();
	}
}

game::Game* game::Game::get_instance()
{
	static Game instance;
	return &instance;
}

void game::Game::initialize(int argc, char** argv, const char* window_name, const sf::Vector2i& position, const int& width, const int& height)
{
	Log::stream().rdbuf(get_instance()->log_file_.rdbuf());
	
	auto window = new sf::Window(sf::VideoMode(width, height), window_name, sf::Style::Default, sf::ContextSettings(32));
	window->setPosition(position);

	get_instance()->window_ = window;
	
	window->setVerticalSyncEnabled(true);
	if (!window->setActive())
	{
		Log::level() = Log::log_error;
		Log::print("Unable to set valid opengl context");
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);

	glewExperimental = GL_TRUE;
	GLenum glew_init_result;
	if (GLEW_OK != (glew_init_result = glewInit()))
		throw gl::exception::GlewException(glew_init_result);

	Keyboard::initialize();
	Mouse::initialize(window, true, true);

	Mouse::set_position(width / 2, height / 2);
	Mouse::set_event_on_mouse_move([](const int& x, const int& y) { captureMouse = true; mousePosition.x = x; mousePosition.y = y; });

	window_width = width;
	window_height = height;

	get_instance()->camera_ = new gl::Camera(width, height, pos, pos + dir, glm::vec3(0, 1, 0), 60.f);

	Log::level() = Log::log_info;
	Log::print("Loading vertex shader");
	gl::Shader vertex(gl::shader_type::Vertex);
	vertex.load_source_form_file(SHADERS_PATH "texture.vert");
	vertex.compile();

	Log::level() = Log::log_info;
	Log::print("Loading fragment shader");
	gl::Shader fragment(gl::shader_type::Fragment);
	fragment.load_source_form_file(SHADERS_PATH"texture.frag");
	fragment.compile();

	auto shader = get_instance()->shader_ = new gl::Program(vertex, fragment);

	glActiveTexture(GL_TEXTURE0);

	modela = model::ModelLoader::load(MODELS_PATH"cow.3DS");
	modela->set_scale(glm::vec3(0.005f));

	shader->link();
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
	get_instance()->main_loop();
}

void game::Game::stop()
{
	
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
