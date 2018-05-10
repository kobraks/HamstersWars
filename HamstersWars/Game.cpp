#include "Game.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

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


glm::vec3 pos(0.f, 1.f, 4.f);
glm::vec3 dir (0.f, 0.f, -1.f);
glm::vec2 mousePosition;

const float speed = 0.05f;
bool flyingMode = true;
bool captureMouse = false;

float velocity_vertical;
float velocity_horizontal;

float window_height, window_width;

gl::Program* program;

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
float data[] = { 0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0 };
model::Model* modela = nullptr;
model::Model* modelb = nullptr;

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
	glLoadIdentity();


	gluLookAt(
		pos.x, pos.y, pos.z,
		pos.x + dir.x, pos.y + dir.y, pos.z + dir.z,
		0.0f, 1.0f, 0.0f
	);
	

	glPushMatrix();
	glTranslatef(0, 0, 2);
	//glTranslatef(0, -200, 1000);
	//glRotatef(90, 1, 0, 0);
	glScalef(0.005, 0.005, 0.005);
	modela->draw();

	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0, 0, -4);
	glScalef(0.05, 0.05, 0.05);
	modelb->draw();

	glPopMatrix();

	glPushMatrix();
	//glColor4f(0, 1, 1, 1);
	glTranslatef(0, 0, 4);
	//glScalef(1.5f, 0.2f, 0.5f);
	glutSolidCube(1.f);

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void game::Game::on_reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60.0f, static_cast<float>(width) / height, .01f, 100.0f);
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

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_CULL_FACE); // W³¹czenie face culling'u, czyli nie renderowanie dwóch stron prymitywów
	//glFrontFace(GL_CW); // Interpretacja jako przednich œcian tych, które s¹ rysowane w kierunku przeciwnym do ruchu zegara. Alternatywa : GL_CW
	//glCullFace(GL_BACK);

	glutCreateWindow(window_name);

	glutDisplayFunc(game::on_draw_callback);
	glutReshapeFunc(game::on_reshape);
	glutTimerFunc(timer_time, game::on_timer, 0);

	glewExperimental = GL_TRUE;
	GLenum glew_init_result;
	if (GLEW_OK != (glew_init_result = glewInit()))
		throw exception::GlewException(glew_init_result);

	//glClearColor(0.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	Keyboard::initialize();
	Mouse::initialize();


	Mouse::set_position(width / 2, height / 2);
	Mouse::set_cursor(GLUT_CURSOR_NONE);
	Mouse::set_event_on_mouse_move([](const int& x, const int& y) { captureMouse = true; mousePosition.x = x; mousePosition.y = y; });

	modela = model::ModelLoader::load(MODELS_PATH "cow.3DS");
	modelb = model::ModelLoader::load(MODELS_PATH "cube.ASE");

	window_width = width;
	window_height = height;

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

	program = new gl::Program(vertex, fragment);

	GLfloat light_position[3] = {1, 1, 1};

	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(*program, "mytex"), 0);

	program->set_attribute("inPosition", VERTEX_INDEX);
	program->set_attribute("inColor", COLOR_INDEX);
	program->set_attribute("inTexcoord", TEXCOORD_INDEX);

	program->link();
	program->use();

	/*
	 * attribute vec3 inPosition;
attribute vec4 inColor;
attribute vec2 inTexcoord; 
attribute vec3 inNormal; 
	 */

	//shader->use();
}

void game::Game::run()
{
	glutMainLoop();
}

void game::Game::stop()
{
	glutLeaveMainLoop();
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
