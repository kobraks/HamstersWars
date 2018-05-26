#include "Game.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Defines.h"
#include "Log.h"


#include "GlewException.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Model.h"
#include "ModelLoader.h"
#include "Texture2d.h"
#include "TextureLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Program.h"
#include <iostream>


glm::vec3 pos(0.f, 1.f, 4.f);
glm::vec3 dir (0.f, 0.f, -1.f);
glm::vec2 mousePosition;

const float speed = 0.05f;
bool flyingMode = true;
bool captureMouse = false;

float velocity_vertical;
float velocity_horizontal;

float window_height, window_width;
model::Model* modela;

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

const float time_step = 1.f / 60.f;
const int timer_time = 14;

namespace game
{
	void on_draw_callback()
	{
		game::Game::get_instance()->on_draw();
	}

	void on_reshape(int widht, int height)
	{
		Game::get_instance()->on_reshape(widht, height);
	}

	void on_timer(int id)
	{
		Game::get_instance()->on_timer(id);

		glutTimerFunc(timer_time, on_timer, 0);
	}
}


void game::Game::on_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glm::mat4 model_view_matrix = camera_->get_view();
	glLoadMatrixf(&model_view_matrix[0][0]);

	shader_->get_parameter("mode")->set_value(1);

	
	for (size_t i = 0; i < modela->count(); ++i)
	{
		auto mesh = modela->get_mesh(i);
		shader_->get_parameter("model")->set_value(mesh->get_model_matrix());
		mesh->draw();

		/*
		shader_->get_parameter("mode")->set_value(-1);
		shader_->get_parameter("model")->set_value(mesh->bounding_box()->get_model_matrix());
		mesh->bounding_box()->draw();*/
	}

	manager_->draw();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void game::Game::on_reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	camera_->set_width(width);
	camera_->set_height(height);

	auto projection = camera_->get_projection();

	glLoadMatrixf(&projection[0][0]);

}

void game::Game::on_timer(int id)
{
	if (Keyboard::is_down(27))
		stop();

	if (Keyboard::is_down('w'))
		velocity_vertical = 1;
	if (Keyboard::is_down('s'))
		velocity_vertical = -1;

	if (Keyboard::is_down('a'))
		velocity_horizontal = 1;
	if (Keyboard::is_down('d'))
		velocity_horizontal = -1;

	if (captureMouse)
	{
		float theta = atan2(dir.z, dir.x);
		float phi = asin(dir.y);

		theta += (mousePosition.x - window_width / 2) * 0.01;
		phi -= (mousePosition.y - window_height / 2) * 0.01;

		dir.x = cos(theta) * cos(phi);
		dir.y = sin(phi);
		dir.z = sin(theta) * cos(phi);

		glutWarpPointer(window_width / 2, window_height / 2);
	}

	update();

	camera_->set_position(pos);
	camera_->set_target(pos + dir);

	manager_->update();
}

game::Game* game::Game::get_instance()
{
	static Game instance;
	return &instance;
}

void game::Game::initialize(int argc, char** argv, const char* window_name, const sf::Vector2i& position, const int& width, const int& height)
{
	Log::stream().rdbuf(get_instance()->log_file_.rdbuf());
	glutInit(&argc, argv);

	glutInitWindowPosition(position.x, position.y);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	get_instance()->window_handle_ = glutCreateWindow(window_name);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glewExperimental = GL_TRUE;
	GLenum glew_init_result;
	if (GLEW_OK != (glew_init_result = glewInit()))
		throw exception::GlewException(glew_init_result);

	glutDisplayFunc(game::on_draw_callback);
	glutReshapeFunc(game::on_reshape);
	glutTimerFunc(timer_time, game::on_timer, 0);
	
	Keyboard::initialize();
	Mouse::initialize();

	Mouse::set_position(width / 2, height / 2);
	Mouse::set_cursor(GLUT_CURSOR_NONE);
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
	modela->set_matrix(glm::mat4(1.f) * glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 2)) * glm::scale(glm::mat4(1.f), glm::vec3(0.005)));

	shader->set_attribute("inPosition", VERTEX_INDEX);
	shader->set_attribute("inColor", COLOR_INDEX);
	shader->set_attribute("inTexcoord", TEXCOORD_INDEX);
	shader->set_attribute("inNormal", NORMAL_INDEX);

	shader->link();
	shader->use();

	shader->set_uniform(shader->get_uniform("mytex"), GL_TEXTURE0);
	shader->set_uniform(shader->get_uniform("mode"), 1);

	get_instance()->manager_ = new game::SceneManager(*shader, [](gl::Program& shader, const model::Model*)
	{
		shader.get_parameter("mode")->set_value(1);

		for (size_t i = 0; i < modela->count(); ++i)
		{
			auto mesh = modela->get_mesh(i);
			shader.get_parameter("model")->set_value(mesh->get_model_matrix());
			mesh->draw();

			shader.get_parameter("mode")->set_value(-1);
			shader.get_parameter("model")->set_value(mesh->bounding_box()->get_model_matrix());
			mesh->bounding_box()->draw();
		}
	});
}

void game::Game::run()
{
	glutMainLoop();
}

void game::Game::stop()
{
	glutLeaveMainLoop();
}

void game::Game::close()
{
	glutDestroyWindow(get_instance()->window_handle_);
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
