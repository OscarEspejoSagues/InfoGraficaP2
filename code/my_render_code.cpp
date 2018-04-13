#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <time.h>
#include <SDL2\SDL.h>

#include "GL_framework.h"

#define ARRAY_SIZE 20

int w, h; //variables donde guardamos la witdh y la height

glm::mat4 RotMatX;
glm::mat4 RotMatY;
glm::mat4 RotMatZ;
glm::mat4 RotMatXZ;
glm::mat4 RotMatYZ;
glm::mat4 RotMatYX;
glm::mat4 scale;

glm::mat4 RotMatXCube;
glm::mat4 RotMatYCube;
glm::mat4 RotMatZCube;
glm::mat4 RotMatXZCube;
glm::mat4 RotMatYZCube;
glm::mat4 RotMatYXCube;
glm::mat4 scaleCube;


glm::vec4 RandomPositionsArray[ARRAY_SIZE];
float caida = 0.1f;
float sumatorio_caida = 0.f;

void InitMatsFall(double currentTime, bool fall);
void InitMatsFallCube(double currentTime, bool fall);

bool staticube = false;
bool staticocto = false;
bool falling = false;
bool fallingCube = false;
bool honeycombwireframe = false;

bool drawcube1 = false;
bool drawcube2 = false;
bool drawwireframe = false;
bool drawocta1 = false;
bool drawocta2 = false;
bool drawoctagreen = false;
bool rotwireframe = false;
bool vel6 = false;

bool greencolor;

///////// fw decl

glm::vec4 RandomPos()
{
	glm::vec4 pos = glm::vec4{ (float)((rand() % 15) - 7), (float)((rand() % 20) + 10), (float)((rand() % 15) - 7), 1.f };
	return pos;
}

void FillArrayPos()
{
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		RandomPositionsArray[i] = RandomPos();
	}
}


namespace ImGui {
	void Render();
}
namespace Box {
	void setupCube();
	void cleanupCube();
	void drawCube();
}
namespace Axis {
	void setupAxis();
	void cleanupAxis();
	void drawAxis();
}

//Dentro del namespace de cube tenemos nustras fuciones que nos permiten dibujar cuadrados
namespace Cube {
	void setupCube();
	void cleanupCube();
	void updateCube(const glm::mat4& transform);
	void drawCube();

	void updateColor(const glm::vec4 newColor);
	glm::vec4 myColor = { 0.0f, 0.3f, 1.4f, 1.0f };
}

namespace MyFirstShader {
	void myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	void myRenderCode(double currentTime, glm::vec4 position, glm::mat4 Rotation, bool fall);

	GLuint myRenderProgram;
	GLuint myVAO;
}

namespace Octahedron {
	void myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	void myRenderCode(double currentTime, glm::vec4 position, glm::mat4 rotation, glm::mat4 scale, bool fall);

	GLuint myRenderProgram;
	GLuint myVAO;
}

namespace OctahedronGreen {
	void myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	void myRenderCode(double currentTime, glm::vec4 position, glm::mat4 rotation, glm::mat4 scale, bool fall);

	GLuint myRenderProgram;
	GLuint myVAO;
}

namespace WireframeOcta {
	void myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	void myRenderCode(double currentTime, glm::vec4 position);

	GLuint myRenderProgram;
	GLuint myVAO;
}

namespace ShaderValues {
	glm::vec4 position = { 0.f, 5.f, 0.f, 0.f };
	glm::vec4 position1 = { 0.f, 3.f,2.f, 1.f };
	glm::vec4 position2 = { 1.f, 6.f, 0.f, 1.f };
	glm::vec4 position3 = { -1.f, 4.f, 1.f, 1.f };
	glm::vec4 position4 = { 2.f, 5.f, -6.f, 1.f };
	glm::vec4 position5 = { 4.f, 9.f,1.f, 1.f };
	glm::vec4 position6 = { -2.f, 5.f,2.f, 1.f };
	glm::vec4 position7 = { 1.f, 2.f,0.f, 1.f };
	glm::vec4 position8 = { -1.f, 2.f, 1.f, 1.f };
	glm::vec4 position9 = { 4.f, 8.f,1.f, 1.f };
	glm::vec4 position10 = { 7.f, 10.f,1.f, 1.f };

	glm::vec4 position_1_Honey = { 0.f, 0.f, 1.f, 1.f };
	glm::vec4 position_2_Honey = { 2.3f, 2.2f, 1.f, 1.f };
	glm::vec4 position_3_Honey = { -0.45f, 2.f, 1.f, 1.f };
	glm::vec4 position_4_Honey = { -0.4f, -0.17f, 1.f, 1.f };
	glm::vec4 position_5_Honey = { 0.98f, -1.f, 1.f, 1.f };
	glm::vec4 position_6_Honey = { 0.9f, 3.1f, 1.f, 1.f };
	glm::vec4 position_7_Honey = { 2.4f, -0.1f, 1.f, 1.f };

	glm::vec4 pos = { 0,0,0,0 };
	glm::vec4 pos1 = { 1.f,0,0,0 };
	glm::vec4 pos2 = { 0.5f,1.f,0,0 };
	glm::vec4 pos3 = { -0.5f, 0.5f, 0,0 };
	glm::vec4 pos4 = { 1.f, 2.f, 1.f, 0 };
	glm::vec4 pos5 = { 0.f, 3.f, 0.f,0 };
	glm::vec4 pos6 = { 1.f, 2.f, 0.f,0 };
	glm::vec4 pos7 = { 0.25f, 1.5f, 0.f,0 };
	glm::vec4 pos8 = { 0.6f, 2.f, 0.f,0 };
	glm::vec4 pos9 = { 0.1f, 1.7f, 0.f,0 };
	glm::vec4 pos10 = { -0.25f, 3.f, 0.f,0 };
	glm::vec4 pos11 = { -0.4f, 1.5f, 0.f,0 };
	glm::vec4 pos12 = { -0.7f, 2.f, 0.f,0 };
	glm::vec4 pos13 = { -0.6f, 3.f, 0.f,0 };
	glm::vec4 pos14 = { -0.2f, 2.f, 0.f,0 };
	glm::vec4 pos15 = { 0.4f, 3.5f, 0.f,0 };
	glm::vec4 pos16 = { -0.1f, 2.5f, 0.f,0 };
	glm::vec4 pos17 = { -0.1f, 2.2f, 0.f,0 };
	glm::vec4 pos18 = { 0.25,0.25,0,0 };
	glm::vec4 pos19 = { -0.25,0.25,0,0 };

	glm::vec4 Honey1 = { 0.f, 13.f, -15.f, 1.f };
	glm::vec4 Honey2 = { 1.7f, 13.f, -15.f, 1.f };
	glm::vec4 Honey3 = { -1.7f, 13.f, -15.f, 1.f };
	glm::vec4 Honey4 = { 1.7f, 14.4f, -15.f, 1.f };
	glm::vec4 Honey5 = { 0.f, 14.4f, -15.f, 1.f };
	glm::vec4 Honey6 = { -1.7f, 14.4f, -15.f, 1.f };
	glm::vec4 Honey7 = { 0.f, 15.8f, -15.f, 1.f };
	glm::vec4 Honey8 = { 1.7f, 15.8f, -15.f, 1.f };
	glm::vec4 Honey9 = { -1.7f, 15.8f, -15.f, 1.f };
	glm::vec4 Honey10 = { 0.f, 12.3f, -13.65f, 1.f };
	glm::vec4 Honey11 = { 1.7f, 12.3f, -13.65f, 1.f };
	glm::vec4 Honey12 = { -1.7f, 12.3f, -13.65f, 1.f };
	glm::vec4 Honey13 = { 1.7f, 13.6f, -13.65f, 1.f };
	glm::vec4 Honey14 = { 0.f, 13.6f, -13.65f, 1.f };
	glm::vec4 Honey15 = { -1.7f, 13.6f, -13.65f, 1.f };
	glm::vec4 Honey16 = { 0.f, 15.1f, -13.6f, 1.f };
	glm::vec4 Honey17 = { 1.7f, 15.1f, -13.6f, 1.f };
	glm::vec4 Honey18 = { -1.7f, 15.1f, -13.6f, 1.f };
	glm::vec4 Honey19 = { 0.f, 16.56f, -13.6f, 1.f };
	glm::vec4 Honey20 = { 1.7f, 16.56f, -13.6f, 1.f };
	glm::vec4 Honey21 = { -1.7f, 16.56f, -13.6f, 1.f };

	glm::vec4 Honey22 = { 0.f, 13.f, -12.3f, 1.f };
	glm::vec4 Honey23 = { 1.7f, 13.f, -12.3f, 1.f };
	glm::vec4 Honey24 = { -1.7f, 13.f, -12.3f, 1.f };
	glm::vec4 Honey25 = { 1.7f, 14.4f, -12.3f, 1.f };
	glm::vec4 Honey26 = { 0.f, 14.4f, -12.3f, 1.f };
	glm::vec4 Honey27 = { -1.7f, 14.4f, -12.3f, 1.f };
	glm::vec4 Honey28 = { 0.f, 15.8f, -12.3f, 1.f };
	glm::vec4 Honey29 = { 1.7f, 15.8f, -12.3f, 1.f };
	glm::vec4 Honey30 = { -1.7f, 15.8f, -12.3f, 1.f };

	glm::vec4 falling01 = { 0.f, 0.f, -1.f, 1.f };
	glm::vec4 falling02 = { 0.25f, 0.f, -0.1f, 1.f };
}





namespace RenderVars {
	float FOV = glm::radians(65.f); //Fov
	const float zNear = 2.f;
	const float zFar = 150.f;

	glm::mat4 _projection;
	glm::mat4 _modelView;
	glm::mat4 _MVP;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	float panv[3] = { 0.f, -5.f, -15.f };
	float rota[2] = { 0.f, 0.f };
}
namespace RV = RenderVars;

//Funcion que nos permite controlar los eventos de teclado, la llamamos en el MAIN
void myKeyController(SDL_Event eve) {//pasamos como parametro un evento SDL
	switch (eve.type) {
	case SDL_KEYDOWN://controlamos que se haya pulsado una tecla
		switch (eve.key.keysym.sym)
		{
		case SDLK_1://Si pulsamos la tecla 1
			FillArrayPos();
			vel6 = false;
			falling = false;
			fallingCube = false;
			honeycombwireframe = false;
			staticocto = false;

			drawcube1 = true;
			drawcube2 = false;
			drawwireframe = false;
			drawocta1 = false;
			drawocta2 = false;
			drawoctagreen = false;

			caida = 0.1f;
			break;
		case SDLK_2:
			vel6 = false;
			falling = false;
			fallingCube = false;
			honeycombwireframe = false;
			staticocto = false;

			drawcube1 = false;
			drawcube2 = false;
			drawwireframe = false;
			drawocta1 = true;
			drawocta2 = false;
			drawoctagreen = false;

			caida = 0.1f;

			break;
		case SDLK_3:
			vel6 = false;
			falling = false;
			fallingCube = true;
			honeycombwireframe = false;
			staticocto = false;

			drawcube1 = false;
			drawcube2 = true;
			drawwireframe = false;
			drawocta1 = false;
			drawocta2 = false;
			drawoctagreen = false;

			caida = 0.1f;
			break;
		case SDLK_6:
			falling = true;
			fallingCube = false;
			honeycombwireframe = true;
			staticocto = true;
			vel6 = true;

			drawcube1 = false;
			drawcube2 = false;
			drawocta1 = false;
			drawocta2 = false;
			drawwireframe = true;
			drawoctagreen = false;
			caida = 0.1f;
			break;

		case SDLK_4:
			FillArrayPos();
			vel6 = false;
			falling = true;
			fallingCube = false;
			honeycombwireframe = false;
			staticocto = false;

			drawcube1 = false;
			drawcube2 = false;
			drawwireframe = false;
			drawocta1 = false;
			drawocta2 = true;
			drawoctagreen = false;

			caida = 0.1f;
			break;
		case SDLK_5:
			FillArrayPos();
			vel6 = false;
			falling = true;
			fallingCube = false;
			honeycombwireframe = false;
			staticocto = false;
			drawcube1 = false;
			drawcube2 = false;
			drawwireframe = false;
			drawocta1 = false;
			drawocta2 = false;
			drawoctagreen = true;
			sumatorio_caida = 0.f;
			caida = 0.1f;
			break;
		case SDLK_7:

			break;

		}


	}
}


void GLmousecb(MouseEvent ev) {
	if (RV::prevMouse.waspressed && RV::prevMouse.button == ev.button) {
		float diffx = ev.posx - RV::prevMouse.lastx;
		float diffy = ev.posy - RV::prevMouse.lasty;
		switch (ev.button) {
		case MouseEvent::Button::Left: // ROTATE
			RV::rota[0] += diffx * 0.005f;
			RV::rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: // MOVE XY
			RV::panv[0] += diffx * 0.03f;
			RV::panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: // MOVE Z
			RV::panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	}
	else {
		RV::prevMouse.button = ev.button;
		RV::prevMouse.waspressed = true;
	}
	RV::prevMouse.lastx = ev.posx;
	RV::prevMouse.lasty = ev.posy;
}




void myInitCode(int width, int height)
{
	srand(time(NULL));
	FillArrayPos();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	w = width;//inicializamos nuestras variables witdh y height a sus valores correspondientes
	h = height;

	//Box::setupCube();
	//Axis::setupAxis();
	//Cube::setupCube();
	//Cube::setupCube();

	MyFirstShader::myInitCode();
	Octahedron::myInitCode();
	OctahedronGreen::myInitCode();
	WireframeOcta::myInitCode();
}

void myCleanupCode() {
	//Box::cleanupCube();
	//Axis::cleanupAxis();
	//Cube::cleanupCube();
	//Cube::cleanupCube();
	MyFirstShader::myCleanupCode();
	Octahedron::myCleanupCode();
	OctahedronGreen::myCleanupCode();
	WireframeOcta::myCleanupCode();
}

void myRenderCode(double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RV::_modelView = glm::mat4(1.f);
	RV::_modelView = glm::translate(RV::_modelView, glm::vec3(RV::panv[0], RV::panv[1], RV::panv[2]));

	RV::_projection = glm::perspective(RV::FOV, (float)w / (float)h, RV::zNear, RV::zFar);//si no es la escena 1 tenemos vision en perspectiva
	if (drawcube2 || drawocta2 || drawoctagreen)
	{
		float aux = 50.f;
		RV::_projection = glm::ortho((float)-w / aux, (float)w / aux, (float)h / aux, (float)-h / aux, 0.1f, 100.f); //camara orthonormal
	}
	if (drawwireframe)
	{
		float aux = 120.f;
		RV::_projection = glm::ortho((float)-w / aux, (float)w / aux, (float)h / aux, (float)-h / aux, 0.1f, 100.f); //camara orthonormal
	}

	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[1], glm::vec3(1.f, 0.f, 0.f));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[0], glm::vec3(0.f, 1.f, 0.f));

	RV::_MVP = RV::_projection * RV::_modelView;



	if (drawcube1)
	{
		InitMatsFallCube(currentTime, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[0], RotMatXCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[1], RotMatYCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[2], RotMatZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[3], RotMatYCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[4], RotMatZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[5], RotMatXCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[6], RotMatYCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[7], RotMatZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[8], RotMatXCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, RandomPositionsArray[9], RotMatXCube, fallingCube);
	}
	if (drawcube2)
	{
		InitMatsFallCube(currentTime, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position, RotMatXZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position1, RotMatYCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position2, RotMatYZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position3, RotMatXCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position4, RotMatZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position5, RotMatYX, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position6, RotMatYCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position7, RotMatYZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position8, RotMatZCube, fallingCube);
		MyFirstShader::myRenderCode(currentTime, ShaderValues::position9, RotMatXCube, fallingCube);
	}
	if (drawwireframe)
	{
		RotMatX = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, (float)cos(currentTime),(float)sin(currentTime), 0.f,
			0.f, (float)-sin(currentTime), (float)cos(currentTime), 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		scale = { 1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f, 
		};
		Octahedron::myRenderCode(currentTime, ShaderValues::falling01, RotMatX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::falling02, RotMatX, scale, falling);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey1);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey2);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey3);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey4);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey5);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey6);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey7);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey8);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey9);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey10);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey11);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey12);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey13);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey14);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey15);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey16);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey17);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey18);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey19);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey20);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey21);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey22);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey23);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey24);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey25);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey26);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey27);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey28);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey29);
		WireframeOcta::myRenderCode(currentTime, ShaderValues::Honey30);

	}
	if (drawocta1)
	{
		InitMatsFall(currentTime, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::position_2_Honey, RotMatYZ, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::position_3_Honey, RotMatYX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::position_4_Honey, RotMatY, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::position_5_Honey, RotMatYZ, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::position_6_Honey, RotMatY, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::position_7_Honey, RotMatX, scale, falling);
	}
	if (drawocta2) {
		InitMatsFall(currentTime, falling);
		scale = { 1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f, };
		Octahedron::myRenderCode(currentTime, ShaderValues::pos, RotMatXZ, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos1, RotMatY, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos2, RotMatX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos3, RotMatYZ, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos4, RotMatYX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos5, RotMatY, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos6, RotMatYX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos7, RotMatX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos8, RotMatYZ, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos9, RotMatYX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos10, RotMatY, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos11, RotMatYX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos12, RotMatX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos13, RotMatYZ, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos14, RotMatY, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos15, RotMatYX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos16, RotMatX, scale, falling);
		Octahedron::myRenderCode(currentTime, ShaderValues::pos17, RotMatYZ, scale, falling);
	}
	if (drawoctagreen)
	{
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos, RotMatX, scale, falling);
		scale = { 1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f, };
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos1, RotMatYZ, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos2, RotMatXZ, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos3, RotMatY, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos4, RotMatZ, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos5, RotMatYX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos6, RotMatYX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos7, RotMatX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos8, RotMatYZ, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos9, RotMatZ, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos10, RotMatYX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos11, RotMatYX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos12, RotMatX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos13, RotMatY, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos14, RotMatYX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos15, RotMatYX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos19, RotMatX, scale, falling);
		OctahedronGreen::myRenderCode(currentTime, ShaderValues::pos18, RotMatY, scale, falling);

		if (sumatorio_caida >= 250)
		{
			drawoctagreen = false;
		}


	}
	ImGui::Render();
}

GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name = "") {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStr, NULL);
	glCompileShader(shader);
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetShaderInfoLog(shader, res, &res, buff);
		fprintf(stderr, "Error Shader %s: %s", name, buff);
		delete[] buff;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

void linkProgram(GLuint program) {
	glLinkProgram(program);
	GLint res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (res == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetProgramInfoLog(program, res, &res, buff);
		fprintf(stderr, "Error Link: %s", buff);
		delete[] buff;
	}
}

namespace MyFirstShader {
	void myCleanupCode() {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}


	//EX0.2

	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330										\n\
		\n\
		void main() {\n\
		const vec4 vertices[4] = vec4[4](vec4( 0.25, -0.25, 0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4( -0.25,  0.25, 0.5, 1.0),\n\
										vec4( -0.25,  -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
			\n\
			out vec4 color;\n\
			\n\
			void main() {\n\
			const vec4 colors[6] = vec4[6](vec4( 0, 1, 0, 1.0),\n\
											vec4(0.25, 0.25, 0.5, 1.0),\n\
											vec4( 1, 0.25, 0.5, 1.0),\n\
											vec4(0.25, 0, 0, 1.0),\n\
											vec4( 1, 0, 0, 1.0),\n\
											vec4( 0.25, 0.25, 0.5, 1.0));\n\
			color = colors[gl_PrimitiveID ];\n\
			}"
		};



		static const GLchar * geom_shader_source[] = {
			"#version 330 \n\
			layout(triangles) in;\n\
			layout(triangle_strip, max_vertices = 24) out;\n\
			uniform mat4 vision;\n\
			uniform vec4 position;\n\
			uniform mat4 Rotation;\n\
			void main()\n\
			{\n\
				vec4 vertices[4] = vec4[4](vec4(0.25, -0.25, 0.25, 1.0),\n\
										vec4(0.25, 0.25, 0.25, 1.0),\n\
										vec4(-0.25, -0.25, 0.25, 1.0),\n\
										vec4(-0.25, 0.25, 0.25, 1.0));\n\
				\n\
				//CARA 1\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = vision*Rotation*vertices[i]+ (gl_in[0].gl_Position+position);\n\
					gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
				//CARA 2\n\
				vec4 vertices2[4]= vec4[4](vec4(0.25, 0.25, 0.25, 1.0),\n\
										vec4(0.25, 0.25, -0.25, 1.0),\n\
										vec4(-0.25, 0.25, 0.25, 1.0),\n\
										vec4(-0.25, 0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = vision*Rotation*vertices2[i]+ (gl_in[0].gl_Position+position);\n\
					gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 3\n\
				vec4 vertices3[4]= vec4[4](vec4(-0.25, -0.25, 0.25, 1.0),\n\
										vec4(-0.25, 0.25, 0.25, 1.0),\n\
										vec4(-0.25, -0.25, -0.25, 1.0),\n\
										vec4(-0.25, 0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = vision*Rotation*vertices3[i]+ (gl_in[0].gl_Position+position);\n\
					gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 4\n\
				vec4 vertices4[4]= vec4[4](vec4(-0.25, -0.25, -0.25, 1.0),\n\
										vec4(-0.25, 0.25, -0.25, 1.0),\n\
										vec4(0.25, -0.25, -0.25, 1.0),\n\
										vec4(0.25, 0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = vision*Rotation*vertices4[i]+ (gl_in[0].gl_Position+position);\n\
					gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 5\n\
				vec4 vertices5[4]= vec4[4](vec4(-0.25, -0.25, 0.25, 1.0),\n\
										vec4(-0.25, -0.25, -0.25, 1.0),\n\
										vec4(0.25, -0.25, 0.25, 1.0),\n\
										vec4(0.25, -0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = vision*Rotation*vertices5[i]+ (gl_in[0].gl_Position+position);\n\
					gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 6\n\
				 vec4 vertices6[4]= vec4[4](vec4(0.25, -0.25, -0.25, 1.0),\n\
										vec4(0.25, 0.25, -0.25, 1.0),\n\
										vec4(0.25, -0.25, 0.25, 1.0),\n\
										vec4(0.25, 0.25, 0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = vision*Rotation*vertices6[i]+ (gl_in[0].gl_Position+position);\n\
					gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};

		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint geom_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime, glm::vec4 position, glm::mat4 Rotation, bool fall) {
		glUseProgram(myRenderProgram);
		if (fall)
		{
			glm::mat4 fixvis = { 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };
			glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "vision"), 1, GL_FALSE, glm::value_ptr(fixvis));
			position.y -= -caida;
		}
		else
		{
			glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "vision"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		}
		caida -= 0.007;
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "Rotation"), 1, GL_FALSE, glm::value_ptr(Rotation));

		glUniform4fv(glGetUniformLocation(myRenderProgram, "position"), 1, (GLfloat*)&position);

		//glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotation"), 1, GL_FALSE, glm::value_ptr(rotation));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

namespace Octahedron {
	void myCleanupCode() {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}


	//EX0.2

	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330										\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4(0.0, 1.0, 0.0, 1.0),\n\
									   vec4(-1.0, 0.0, 1.0, 1.0),\n\
										vec4( 1.0, 0.0, 1.0, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};
		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
			\n\
			out vec4 color;\n\
			uniform float time;										\n\
			\n\
			void main() {\n\
			vec4 colors[10] = vec4[10](vec4( sin(time),1, sin(time), 1.0),\n\
											vec4(0,1,0, 1.0),\n\
											vec4(0, 0, 1, 1.0),\n\
											vec4(0.10, 0, 1, 1.0),\n\
											vec4( 1, 0.40, 0, 1.0),\n\
											vec4( 0, 0.40, 0.40, 1.0),\n\
											vec4( 0.1, 0.3, 0.1, 1.0),\n\
											vec4( 0.9, 0.2, 0.4, 1.0),\n\
											vec4( 0.6, 0.3, 0.5, 1.0),\n\
											vec4( 1.0, 1.0, 1.0, 1.0));\n\
			color = colors[gl_PrimitiveID ];\n\
			}"
		};

		static const GLchar * geom_shader_source[] = {
			"#version 330															\n\
			layout(triangles) in;													\n\
			layout(triangle_strip, max_vertices = 72) out;							\n\
			uniform mat4 rot;														\n\
			uniform mat4 scale;														\n\
            uniform mat4 RotMat;													\n\
			uniform vec4 position;													\n\
			uniform float color;													\n\
			void main()																\n\
			{																		\n\
				//ROJO                                                              \n\
				vec4 vertices[6] = vec4[6]( vec4(0.3, 0.7, 0.0, 1.0),				\n\
											vec4(-0.3, 0.7, 0.0, 1.0),				\n\
											vec4(0.65, 0.3, 0.3, 1.0),				\n\
											vec4(-0.65, 0.3, 0.3, 1.0),				\n\
											vec4(0.3, 0.0, 0.6, 1.0),				\n\
											vec4(-0.3, 0.0, 0.6, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//VERDE                                                             \n\
				vec4 vertices1[6] = vec4[6]( vec4(-0.3, 0.7, 0.0, 1.0),				\n\
											vec4(-0.3, 0.7, -0.7, 1.0),				\n\
											vec4(-0.65, 0.3, 0.3, 1.0),				\n\
											vec4(-0.65, 0.3, -1, 1.0),				\n\
											vec4(-0.9, 0.0, 0.0, 1.0),				\n\
											vec4(-0.9, 0.0, -0.7, 1.0));			\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices1[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////AZUL                                                            \n\
				vec4 vertices2[6] = vec4[6]( vec4(0.3, 0.7, -0.7, 1.0),			    \n\
											vec4(0.3, 0.7, 0.0, 1.0),		        \n\
											vec4(0.65, 0.3, -1, 1.0),		        \n\
											vec4(0.65, 0.3, 0.3, 1.0),		        \n\
											vec4(0.9, 0.0, -0.8, 1.0),		        \n\
											vec4(0.9, 0.0, 0.0, 1.0));		        \n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices2[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//AMARILLO                                                          \n\
				vec4 vertices3[6] = vec4[6]( vec4(-0.3, 0.7, -0.7, 1.0),			\n\
											vec4(0.3, 0.7, -0.7, 1.0),				\n\
											vec4(-0.65, 0.3, -1, 1.0),				\n\
											vec4(0.65, 0.3, -1, 1.0),				\n\
											vec4(-0.3, 0.0, -1.2, 1.0),				\n\
											vec4(0.3, 0.0, -1.2, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices3[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////PARTE INFERIOR													\n\
				//ROJO                                                              \n\
				vec4 vertices4[6] = vec4[6]( vec4(-0.3, -0.7, 0.0, 1.0),			\n\
											vec4(0.3, -0.7, 0.0, 1.0),				\n\
											vec4(-0.65, -0.3, 0.3, 1.0),			\n\
											vec4(0.65, -0.3, 0.3, 1.0),				\n\
											vec4(-0.3, 0.0, 0.6, 1.0),				\n\
											vec4(0.3, 0.0, 0.6, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices4[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//VERDE                                                             \n\
				vec4 vertices5[6] = vec4[6]( vec4(-0.3, -0.7, -0.7, 1.0),			\n\
											vec4(-0.3, -0.7, 0.0, 1.0),				\n\
											vec4(-0.65, -0.3, -1, 1.0),				\n\
											vec4(-0.65, -0.3, 0.3, 1.0),			\n\
											vec4(-0.9, 0.0, -0.7, 1.0),				\n\
											vec4(-0.9, 0.0, 0.0, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices5[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////AZUL                                                            \n\
				vec4 vertices6[6] = vec4[6]( vec4(0.3, -0.7, 0.0, 1.0),				\n\
											vec4(0.3, -0.7, -0.7, 1.0),		        \n\
											vec4(0.65, -0.3, 0.3, 1.0),		        \n\
											vec4(0.65, -0.3, -1, 1.0),		        \n\
											vec4(0.9, 0.0, 0.0, 1.0),		        \n\
											vec4(0.9, 0.0, -0.8, 1.0));		        \n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices6[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//AMARILLO                                                          \n\
				vec4 vertices7[6] = vec4[6]( vec4(0.3, -0.7, -0.7, 1.0),			\n\
											vec4(-0.3, -0.7, -0.7, 1.0),			\n\
											vec4(0.65, -0.3, -1, 1.0),				\n\
											vec4(-0.65, -0.3, -1, 1.0),				\n\
											vec4(0.3, 0.0, -1.2, 1.0),				\n\
											vec4(-0.3, 0.0, -1.2, 1.0));			\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices7[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//CUADRADOS															\n\
				//TOP                                                               \n\
					vec4 vertices8[4] = vec4[4]( vec4(0.3, 0.7, 0.0, 1.0),			\n\
										vec4(0.3, 0.7, -0.7, 1.0),					\n\
										vec4(-0.3, 0.7, 0.0, 1.0),					\n\
										vec4(-0.3, 0.7, -0.7, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices8[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//BOT                                                               \n\
					vec4 vertices9[4] = vec4[4]( vec4(-0.3, -0.7, 0.0, 1.0),		\n\
										vec4(-0.3, -0.7, -0.7, 1.0),				\n\
										vec4(0.3, -0.7, 0.0, 1.0),					\n\
										vec4(0.3, -0.7, -0.7, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices9[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT1                                                              \n\
					vec4 vertices10[4] = vec4[4]( vec4(-0.65, 0.3, 0.3, 1.0),		\n\
										vec4(-0.9, 0.0, 0.0, 1.0),					\n\
										vec4(-0.3, 0.0, 0.6, 1.0),					\n\
										vec4(-0.65, -0.3, 0.3, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices10[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT2                                                              \n\
					vec4 vertices11[4] = vec4[4]( vec4(0.65, 0.3, 0.3, 1.0),		\n\
										vec4(0.3, 0.0, 0.6, 1.0),					\n\
										vec4(0.9, 0.0, 0.0, 1.0),					\n\
										vec4(0.65, -0.3, 0.3, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices11[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT3                                                              \n\
					vec4 vertices12[4] = vec4[4]( vec4(0.65, 0.3, -1, 1.0),			\n\
										vec4(0.9, 0.0, -0.8, 1.0),					\n\
										vec4(0.3, 0.0, -1.2, 1.0),					\n\
										vec4(0.65, -0.3, -1, 1.0));					\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices12[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT4                                                              \n\
					vec4 vertices13[4] = vec4[4]( vec4(-0.65, 0.3, -1, 1.0),		\n\
										vec4(-0.3, 0.0, -1.2, 1.0),					\n\
										vec4(-0.9, 0.0, -0.7, 1.0),					\n\
										vec4(-0.65, -0.3, -1, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices13[i]+position;			\n\
					if(position.y<=-1.4){											\n\
					gl_PrimitiveID=9;												\n\
					}else{																\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
			}"
		};


		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint geom_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void)
	{

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);

	}

	void myRenderCode(double currentTime, glm::vec4 position, glm::mat4 rotation, glm::mat4 scale, bool fall) {
		glUseProgram(myRenderProgram);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rot"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "RotMat"), 1, GL_FALSE, glm::value_ptr(rotation));
		if (fall)
		{
			position.y -= -caida;
		}
		if (vel6)
		{
			caida -= 0.009;
		}
		caida -= 0.0009;
		glUniform4fv(glGetUniformLocation(myRenderProgram, "position"), 1, (GLfloat*)&position);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "scale"), 1, GL_FALSE, glm::value_ptr(scale));
		glUniform1f(glGetUniformLocation(myRenderProgram, "time"), (GLfloat)currentTime);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

namespace OctahedronGreen {
	void myCleanupCode() {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}


	//EX0.2

	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330										\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4(0.0, 1.0, 0.0, 1.0),\n\
									   vec4(-1.0, 0.0, 1.0, 1.0),\n\
										vec4( 1.0, 0.0, 1.0, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};
		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
			\n\
			out vec4 color;\n\
			uniform float time;										\n\
			\n\
			void main() {\n\
			vec4 colors[10] = vec4[10](vec4( 1,1,1, 1.0),\n\
											vec4(0,1,0, 1.0),\n\
											vec4(0, 0, 1, 1.0),\n\
											vec4(0.10, 0, 1, 1.0),\n\
											vec4( 1, 0.40, 0, 1.0),\n\
											vec4( 0, 0.40, 0.40, 1.0),\n\
											vec4( 0.1, 0.3, 0.1, 1.0),\n\
											vec4( 0.9, 0.2, 0.4, 1.0),\n\
											vec4( 0.6, 0.3, 0.5, 1.0),\n\
											vec4( 1.0, 1.0, 1.0, 1.0));\n\
			color = colors[gl_PrimitiveID ];\n\
			}"
		};

		static const GLchar * geom_shader_source[] = {
			"#version 330															\n\
			layout(triangles) in;													\n\
			layout(triangle_strip, max_vertices = 72) out;							\n\
			uniform mat4 rot;														\n\
			uniform mat4 scale;														\n\
            uniform mat4 RotMat;													\n\
			uniform vec4 position;													\n\
			uniform float color;													\n\
			uniform float sumatorio_caida;											\n\
			void main()																\n\
			{																		\n\
				//ROJO                                                              \n\
				vec4 vertices[6] = vec4[6]( vec4(0.3, 0.7, 0.0, 1.0),				\n\
											vec4(-0.3, 0.7, 0.0, 1.0),				\n\
											vec4(0.65, 0.3, 0.3, 1.0),				\n\
											vec4(-0.65, 0.3, 0.3, 1.0),				\n\
											vec4(0.3, 0.0, 0.6, 1.0),				\n\
											vec4(-0.3, 0.0, 0.6, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//VERDE                                                             \n\
				vec4 vertices1[6] = vec4[6]( vec4(-0.3, 0.7, 0.0, 1.0),				\n\
											vec4(-0.3, 0.7, -0.7, 1.0),				\n\
											vec4(-0.65, 0.3, 0.3, 1.0),				\n\
											vec4(-0.65, 0.3, -1, 1.0),				\n\
											vec4(-0.9, 0.0, 0.0, 1.0),				\n\
											vec4(-0.9, 0.0, -0.7, 1.0));			\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices1[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////AZUL                                                            \n\
				vec4 vertices2[6] = vec4[6]( vec4(0.3, 0.7, -0.7, 1.0),			    \n\
											vec4(0.3, 0.7, 0.0, 1.0),		        \n\
											vec4(0.65, 0.3, -1, 1.0),		        \n\
											vec4(0.65, 0.3, 0.3, 1.0),		        \n\
											vec4(0.9, 0.0, -0.8, 1.0),		        \n\
											vec4(0.9, 0.0, 0.0, 1.0));		        \n\
			for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices2[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//AMARILLO                                                          \n\
				vec4 vertices3[6] = vec4[6]( vec4(-0.3, 0.7, -0.7, 1.0),			\n\
											vec4(0.3, 0.7, -0.7, 1.0),				\n\
											vec4(-0.65, 0.3, -1, 1.0),				\n\
											vec4(0.65, 0.3, -1, 1.0),				\n\
											vec4(-0.3, 0.0, -1.2, 1.0),				\n\
											vec4(0.3, 0.0, -1.2, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices3[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////PARTE INFERIOR													\n\
				//ROJO                                                              \n\
				vec4 vertices4[6] = vec4[6]( vec4(-0.3, -0.7, 0.0, 1.0),			\n\
											vec4(0.3, -0.7, 0.0, 1.0),				\n\
											vec4(-0.65, -0.3, 0.3, 1.0),			\n\
											vec4(0.65, -0.3, 0.3, 1.0),				\n\
											vec4(-0.3, 0.0, 0.6, 1.0),				\n\
											vec4(0.3, 0.0, 0.6, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices4[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				//VERDE                                                             \n\
				vec4 vertices5[6] = vec4[6]( vec4(-0.3, -0.7, -0.7, 1.0),			\n\
											vec4(-0.3, -0.7, 0.0, 1.0),				\n\
											vec4(-0.65, -0.3, -1, 1.0),				\n\
											vec4(-0.65, -0.3, 0.3, 1.0),			\n\
											vec4(-0.9, 0.0, -0.7, 1.0),				\n\
											vec4(-0.9, 0.0, 0.0, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices5[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////AZUL                                                            \n\
				vec4 vertices6[6] = vec4[6]( vec4(0.3, -0.7, 0.0, 1.0),				\n\
											vec4(0.3, -0.7, -0.7, 1.0),		        \n\
											vec4(0.65, -0.3, 0.3, 1.0),		        \n\
											vec4(0.65, -0.3, -1, 1.0),		        \n\
											vec4(0.9, 0.0, 0.0, 1.0),		        \n\
											vec4(0.9, 0.0, -0.8, 1.0));		        \n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices6[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//AMARILLO                                                          \n\
				vec4 vertices7[6] = vec4[6]( vec4(0.3, -0.7, -0.7, 1.0),			\n\
											vec4(-0.3, -0.7, -0.7, 1.0),			\n\
											vec4(0.65, -0.3, -1, 1.0),				\n\
											vec4(-0.65, -0.3, -1, 1.0),				\n\
											vec4(0.3, 0.0, -1.2, 1.0),				\n\
											vec4(-0.3, 0.0, -1.2, 1.0));			\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices7[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//CUADRADOS															\n\
				//TOP                                                               \n\
					vec4 vertices8[4] = vec4[4]( vec4(0.3, 0.7, 0.0, 1.0),			\n\
										vec4(0.3, 0.7, -0.7, 1.0),					\n\
										vec4(-0.3, 0.7, 0.0, 1.0),					\n\
										vec4(-0.3, 0.7, -0.7, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices8[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//BOT                                                               \n\
					vec4 vertices9[4] = vec4[4]( vec4(-0.3, -0.7, 0.0, 1.0),		\n\
										vec4(-0.3, -0.7, -0.7, 1.0),				\n\
										vec4(0.3, -0.7, 0.0, 1.0),					\n\
										vec4(0.3, -0.7, -0.7, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices9[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT1                                                              \n\
					vec4 vertices10[4] = vec4[4]( vec4(-0.65, 0.3, 0.3, 1.0),		\n\
										vec4(-0.9, 0.0, 0.0, 1.0),					\n\
										vec4(-0.3, 0.0, 0.6, 1.0),					\n\
										vec4(-0.65, -0.3, 0.3, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices10[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT2                                                              \n\
					vec4 vertices11[4] = vec4[4]( vec4(0.65, 0.3, 0.3, 1.0),		\n\
										vec4(0.3, 0.0, 0.6, 1.0),					\n\
										vec4(0.9, 0.0, 0.0, 1.0),					\n\
										vec4(0.65, -0.3, 0.3, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices11[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT3                                                              \n\
					vec4 vertices12[4] = vec4[4]( vec4(0.65, 0.3, -1, 1.0),			\n\
										vec4(0.9, 0.0, -0.8, 1.0),					\n\
										vec4(0.3, 0.0, -1.2, 1.0),					\n\
										vec4(0.65, -0.3, -1, 1.0));					\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices12[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT4                                                              \n\
					vec4 vertices13[4] = vec4[4]( vec4(-0.65, 0.3, -1, 1.0),		\n\
										vec4(-0.3, 0.0, -1.2, 1.0),					\n\
										vec4(-0.9, 0.0, -0.7, 1.0),					\n\
										vec4(-0.65, -0.3, -1, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*RotMat*scale*vertices13[i]+position;			\n\
					if(sumatorio_caida>=200.f){									\n\
					gl_PrimitiveID = 0;												\n\
					}																\n\
					else{															\n\
					gl_PrimitiveID = 1;												\n\
					}																\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
			}"
		};


		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint geom_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void)
	{

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);

	}

	void myRenderCode(double currentTime, glm::vec4 position, glm::mat4 rotation, glm::mat4 scale, bool fall) {
		glUseProgram(myRenderProgram);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rot"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "RotMat"), 1, GL_FALSE, glm::value_ptr(rotation));
		if (fall)
		{
			position.y -= -caida;
			sumatorio_caida += -caida;
		}
		
		caida -= 0.0009;
		glUniform4fv(glGetUniformLocation(myRenderProgram, "position"), 1, (GLfloat*)&position);
		glUniform1fv(glGetUniformLocation(myRenderProgram, "sumatorio_caida"), 1, (GLfloat*)&sumatorio_caida);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "scale"), 1, GL_FALSE, glm::value_ptr(scale));
		glUniform1f(glGetUniformLocation(myRenderProgram, "time"), (GLfloat)currentTime);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

namespace WireframeOcta {
	void myCleanupCode() {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}


	//EX0.2

	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330										\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4(0.0, 1.0, 0.0, 1.0),\n\
									   vec4(-1.0, 0.0, 1.0, 1.0),\n\
										vec4( 1.0, 0.0, 1.0, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};
		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
			\n\
			out vec4 color;\n\
			\n\
			void main() {\n\
			const vec4 colors[10] = vec4[10](vec4( 1, 0, 0, 1.0),\n\
											vec4(0,1,0, 1.0),\n\
											vec4(0, 0, 1, 1.0),\n\
											vec4(0.10, 0, 1, 1.0),\n\
											vec4( 1, 0.40, 0, 1.0),\n\
											vec4( 0, 0.40, 0.40, 1.0),\n\
											vec4( 0.1, 0.3, 0.1, 1.0),\n\
											vec4( 0.9, 0.2, 0.4, 1.0),\n\
											vec4( 0.6, 0.3, 0.5, 1.0),\n\
											vec4( 1.0, 1.0, 1.0, 1.0));\n\
			color = colors[gl_PrimitiveID ];\n\
			}"
		};

		static const GLchar * geom_shader_source[] = {
			"#version 330															\n\
			layout(lines) in;														\n\
			layout(line_strip, max_vertices = 72) out;								\n\
			uniform mat4 rot;														\n\
			uniform mat4 scale;														\n\
            uniform mat4 RotMat;													\n\
			uniform vec4 position;													\n\
			void main()																\n\
			{																		\n\
				//ROJO                                                              \n\
				vec4 vertices[6] = vec4[6]( vec4(0.3, 0.0, 0.6, 1.0),				\n\
											vec4(-0.3, 0.0, 0.6, 1.0),				\n\
											vec4(-0.65, 0.3, 0.3, 1.0),				\n\
											vec4(-0.3, 0.7, 0.0, 1.0),				\n\
											vec4(0.3, 0.7, 0.0, 1.0),				\n\
											vec4(0.65, 0.3, 0.3, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//VERDE                                                             \n\
				vec4 vertices1[6] = vec4[6]( vec4(-0.9, 0.0, 0.0, 1.0),				\n\
											vec4(-0.9, 0.0, -0.7, 1.0),				\n\
											vec4(-0.65, 0.3, -1, 1.0),				\n\
											vec4(-0.3, 0.7, -0.7, 1.0),				\n\
											vec4(-0.3, 0.7, 0.0, 1.0),				\n\
											vec4(-0.65, 0.3, 0.3, 1.0));			\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices1[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////AZUL                                                            \n\
				vec4 vertices2[6] = vec4[6]( vec4(0.9, 0.0, -0.8, 1.0),			    \n\
											vec4(0.9, 0.0, 0.0, 1.0),		        \n\
											vec4(0.65, 0.3, 0.3, 1.0),		        \n\
											vec4(0.3, 0.7, 0.0, 1.0),		        \n\
											vec4(0.3, 0.7, -0.7, 1.0),		        \n\
											vec4(0.65, 0.3, -1, 1.0));		        \n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices2[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//AMARILLO                                                          \n\
				vec4 vertices3[7] = vec4[7]( vec4(-0.3, 0.0, -1.2, 1.0),			\n\
											vec4(0.3, 0.0, -1.2, 1.0),				\n\
											vec4(0.65, 0.3, -1, 1.0),				\n\
											vec4(0.3, 0.7, -0.7, 1.0),				\n\
											vec4(-0.3, 0.7, -0.7, 1.0),				\n\
											vec4(-0.65, 0.3, -1, 1.0),				\n\
											vec4(-0.3, 0.0, -1.2, 1.0));				\n\
				for (int i = 0; i <7; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices3[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////PARTE INFERIOR													\n\
				//ROJO                                                              \n\
				vec4 vertices4[6] = vec4[6]( vec4(0.3, 0.0, 0.6, 1.0),				\n\
											vec4(0.65, -0.3, 0.3, 1.0),				\n\
											vec4(0.3, -0.7, 0.0, 1.0),				\n\
											vec4(-0.3, -0.7, 0.0, 1.0),				\n\
											vec4(-0.65, -0.3, 0.3, 1.0),			\n\
											vec4(-0.3, 0.0, 0.6, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices4[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//VERDE                                                             \n\
				vec4 vertices5[6] = vec4[6]( vec4(-0.9, 0.0, 0.0, 1.0),				\n\
											vec4(-0.9, 0.0, -0.7, 1.0),				\n\
											vec4(-0.65, -0.3, -1, 1.0),				\n\
											vec4(-0.3, -0.7, -0.7, 1.0),				\n\
											vec4(-0.3, -0.7, 0.0, 1.0),				\n\
											vec4(-0.65, -0.3, 0.3, 1.0));			\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices5[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				////AZUL                                                            \n\
				vec4 vertices6[6] = vec4[6]( vec4(0.9, 0.0, -0.8, 1.0),			    \n\
											vec4(0.9, 0.0, 0.0, 1.0),		        \n\
											vec4(0.65, -0.3, 0.3, 1.0),		        \n\
											vec4(0.3, -0.7, 0.0, 1.0),		        \n\
											vec4(0.3, -0.7, -0.7, 1.0),		        \n\
											vec4(0.65, -0.3, -1, 1.0));		        \n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices6[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//AMARILLO                                                          \n\
				vec4 vertices7[6] = vec4[6]( vec4(0.3, 0.0, -1.2, 1.0),			\n\
											vec4(0.3, 0.0, -1.2, 1.0),				\n\
											vec4(0.65, -0.3, -1, 1.0),				\n\
											vec4(0.3, -0.7, -0.7, 1.0),				\n\
											vec4(-0.3, -0.7, -0.7, 1.0),				\n\
											vec4(-0.65, -0.3, -1, 1.0));				\n\
				for (int i = 0; i <6; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices7[i]+position);			\n\
					gl_PrimitiveID = 1;												\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//CUADRADOS															\n\
				//TOP                                                               \n\
					vec4 vertices8[4] = vec4[4]( vec4(0.3, 0.7, 0.0, 1.0),			\n\
										vec4(-0.3, 0.7, 0.0, 1.0),					\n\
										vec4(-0.3, 0.7, -0.7, 1.0),					\n\
										vec4(0.3, 0.7, -0.7, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices8[i]+position);			\n\
					gl_PrimitiveID = 1;											\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//BOT                                                               \n\
					vec4 vertices9[4] = vec4[4]( vec4(-0.3, -0.7, 0.0, 1.0),		\n\
										vec4(0.3, -0.7, 0.0, 1.0),				\n\
										vec4(0.3, -0.7, -0.7, 1.0),					\n\
										vec4(-0.3, -0.7, -0.7, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices9[i]+position);			\n\
					gl_PrimitiveID = 1;											\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT1                                                              \n\
					vec4 vertices10[4] = vec4[4]( vec4(-0.65, 0.3, 0.3, 1.0),		\n\
										vec4(-0.9, 0.0, 0.0, 1.0),					\n\
										vec4(-0.65, -0.3, 0.3, 1.0),					\n\
										vec4(-0.3, 0.0, 0.6, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices10[i]+position);			\n\
					gl_PrimitiveID = 1;											\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT2                                                              \n\
					vec4 vertices11[4] = vec4[4]( vec4(0.65, -0.3, 0.3, 1.0),		\n\
										vec4(0.3, 0.0, 0.6, 1.0),					\n\
										vec4(0.65, 0.3, 0.3, 1.0),					\n\
										vec4(0.9, 0.0, 0.0, 1.0));				\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices11[i]+position);			\n\
					gl_PrimitiveID = 1;											\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT3                                                              \n\
					vec4 vertices12[4] = vec4[4]( vec4(0.65, -0.3, -1, 1.0),			\n\
										vec4(0.9, 0.0, -0.8, 1.0),					\n\
										vec4(0.65, 0.3, -1, 1.0),					\n\
										vec4(0.3, 0.0, -1.2, 1.0));					\n\
				for (int i = 0; i <4; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices12[i]+position);			\n\
					gl_PrimitiveID = 1;											\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
				//LAT4                                                              \n\
					vec4 vertices13[3] = vec4[3](vec4(-0.3, 0.0, -1.2, 1.0),		\n\
										vec4(-0.65, -0.3, -1, 1.0),					\n\
										vec4(-0.9, 0.0, -0.7, 1.0));				\n\
				for (int i = 0; i <3; i++)											\n\
				{																	\n\
					gl_Position = rot*(vertices13[i]+position);			\n\
					gl_PrimitiveID = 1;											\n\
					EmitVertex();													\n\
				}																	\n\
				EndPrimitive();														\n\
			}"
		};


		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint geom_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void)
	{

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);

	}

	void myRenderCode(double currentTime, glm::vec4 position) {
		glUseProgram(myRenderProgram);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rot"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		//position.y -= currentTime * 2;
		glUniform4fv(glGetUniformLocation(myRenderProgram, "position"), 1, (GLfloat*)&position);
		glDrawArrays(GL_LINES, 0, 3);
	}
}

////////////////////////////////////////////////// BOX
namespace Box {
	GLuint cubeVao;
	GLuint cubeVbo[2];
	GLuint cubeShaders[2];
	GLuint cubeProgram;

	float cubeVerts[] = {
		// -5,0,-5 -- 5, 10, 5
		-5.f,  0.f, -5.f,
		5.f,  0.f, -5.f,
		5.f,  0.f,  5.f,
		-5.f,  0.f,  5.f,
		-5.f, 10.f, -5.f,
		5.f, 10.f, -5.f,
		5.f, 10.f,  5.f,
		-5.f, 10.f,  5.f,
	};
	GLubyte cubeIdx[] = {
		1, 0, 2, 3, // Floor - TriangleStrip
		0, 1, 5, 4, // Wall - Lines
		1, 2, 6, 5, // Wall - Lines
		2, 3, 7, 6, // Wall - Lines
		3, 0, 4, 7  // Wall - Lines
	};

	const char* vertShader_xform =
		"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* fragShader_flatColor =
		"#version 330\n\
out vec4 out_Color;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = color;\n\
}";

	void setupCube() {
		glGenVertexArrays(1, &cubeVao);
		glBindVertexArray(cubeVao);
		glGenBuffers(2, cubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 20, cubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cubeShaders[0] = compileShader(vertShader_xform, GL_VERTEX_SHADER, "cubeVert");
		cubeShaders[1] = compileShader(fragShader_flatColor, GL_FRAGMENT_SHADER, "cubeFrag");

		cubeProgram = glCreateProgram();
		glAttachShader(cubeProgram, cubeShaders[0]);
		glAttachShader(cubeProgram, cubeShaders[1]);
		glBindAttribLocation(cubeProgram, 0, "in_Position");
		linkProgram(cubeProgram);
	}
	void cleanupCube() {
		glDeleteBuffers(2, cubeVbo);
		glDeleteVertexArrays(1, &cubeVao);

		glDeleteProgram(cubeProgram);
		glDeleteShader(cubeShaders[0]);
		glDeleteShader(cubeShaders[1]);
	}
	void drawCube() {
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		// FLOOR
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.6f, 0.6f, 0.6f, 1.f);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
		// WALLS
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.f, 0.f, 0.f, 1.f);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 4));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 8));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 12));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 16));

		glUseProgram(0);
		glBindVertexArray(0);
	}
}


////////////////////////////////////////////////// AXIS
namespace Axis {
	GLuint AxisVao;
	GLuint AxisVbo[3];
	GLuint AxisShader[2];
	GLuint AxisProgram;

	float AxisVerts[] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0
	};
	float AxisColors[] = {
		1.0, 0.0, 0.0, 1.0,
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		0.0, 0.0, 1.0, 1.0
	};
	GLubyte AxisIdx[] = {
		0, 1,
		2, 3,
		4, 5
	};
	const char* Axis_vertShader =
		"#version 330\n\
in vec3 in_Position;\n\
in vec4 in_Color;\n\
out vec4 vert_color;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	vert_color = in_Color;\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* Axis_fragShader =
		"#version 330\n\
in vec4 vert_color;\n\
out vec4 out_Color;\n\
void main() {\n\
	out_Color = vert_color;\n\
}";

	void setupAxis() {
		glGenVertexArrays(1, &AxisVao);
		glBindVertexArray(AxisVao);
		glGenBuffers(3, AxisVbo);

		glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisColors, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AxisVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, AxisIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		AxisShader[0] = compileShader(Axis_vertShader, GL_VERTEX_SHADER, "AxisVert");
		AxisShader[1] = compileShader(Axis_fragShader, GL_FRAGMENT_SHADER, "AxisFrag");

		AxisProgram = glCreateProgram();
		glAttachShader(AxisProgram, AxisShader[0]);
		glAttachShader(AxisProgram, AxisShader[1]);
		glBindAttribLocation(AxisProgram, 0, "in_Position");
		glBindAttribLocation(AxisProgram, 1, "in_Color");
		linkProgram(AxisProgram);
	}
	void cleanupAxis() {
		glDeleteBuffers(3, AxisVbo);
		glDeleteVertexArrays(1, &AxisVao);

		glDeleteProgram(AxisProgram);
		glDeleteShader(AxisShader[0]);
		glDeleteShader(AxisShader[1]);
	}
	void drawAxis() {
		glBindVertexArray(AxisVao);
		glUseProgram(AxisProgram);
		glUniformMatrix4fv(glGetUniformLocation(AxisProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
	}
}


////////////////////////////////////////////////// CUBE
namespace Cube {
	GLuint cubeVao;
	GLuint cubeVbo[3];
	GLuint cubeShaders[2];
	GLuint cubeProgram;
	glm::mat4 objMat = glm::mat4(1.f);

	extern const float halfW = 0.5f;
	int numVerts = 24 + 6; // 4 vertex/face * 6 faces + 6 PRIMITIVE RESTART

						   //   4---------7
						   //  /|        /|
						   // / |       / |
						   //5---------6  |
						   //|  0------|--3
						   //| /       | /
						   //|/        |/
						   //1---------2
	glm::vec3 verts[] = {
		glm::vec3(-halfW, -halfW, -halfW),
		glm::vec3(-halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW, -halfW),
		glm::vec3(-halfW,  halfW, -halfW),
		glm::vec3(-halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW, -halfW)
	};
	glm::vec3 norms[] = {
		glm::vec3(0.f, -1.f,  0.f),
		glm::vec3(0.f,  1.f,  0.f),
		glm::vec3(-1.f,  0.f,  0.f),
		glm::vec3(1.f,  0.f,  0.f),
		glm::vec3(0.f,  0.f, -1.f),
		glm::vec3(0.f,  0.f,  1.f)
	};

	glm::vec3 cubeVerts[] = {
		verts[1], verts[0], verts[2], verts[3],
		verts[5], verts[6], verts[4], verts[7],
		verts[1], verts[5], verts[0], verts[4],
		verts[2], verts[3], verts[6], verts[7],
		verts[0], verts[4], verts[3], verts[7],
		verts[1], verts[2], verts[5], verts[6]
	};
	glm::vec3 cubeNorms[] = {
		norms[0], norms[0], norms[0], norms[0],
		norms[1], norms[1], norms[1], norms[1],
		norms[2], norms[2], norms[2], norms[2],
		norms[3], norms[3], norms[3], norms[3],
		norms[4], norms[4], norms[4], norms[4],
		norms[5], norms[5], norms[5], norms[5]
	};
	GLubyte cubeIdx[] = {
		0, 1, 2, 3, UCHAR_MAX,
		4, 5, 6, 7, UCHAR_MAX,
		8, 9, 10, 11, UCHAR_MAX,
		12, 13, 14, 15, UCHAR_MAX,
		16, 17, 18, 19, UCHAR_MAX,
		20, 21, 22, 23, UCHAR_MAX
	};




	const char* cube_vertShader =
		"#version 330\n\
	in vec3 in_Position;\n\
	in vec3 in_Normal;\n\
	out vec4 vert_Normal;\n\
	uniform mat4 objMat;\n\
	uniform mat4 mv_Mat;\n\
	uniform mat4 mvpMat;\n\
	void main() {\n\
		gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);\n\
		vert_Normal = mv_Mat * objMat * vec4(in_Normal, 0.0);\n\
	}";


	const char* cube_fragShader =
		"#version 330\n\
in vec4 vert_Normal;\n\
out vec4 out_Color;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = vec4(color.xyz * dot(vert_Normal, mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)) + color.xyz * 0.3, 1.0 );\n\
}";
	void setupCube() {
		glGenVertexArrays(1, &cubeVao);
		glBindVertexArray(cubeVao);
		glGenBuffers(3, cubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNorms), cubeNorms, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glPrimitiveRestartIndex(UCHAR_MAX);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIdx), cubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cubeShaders[0] = compileShader(cube_vertShader, GL_VERTEX_SHADER, "cubeVert");
		cubeShaders[1] = compileShader(cube_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		cubeProgram = glCreateProgram();
		glAttachShader(cubeProgram, cubeShaders[0]);
		glAttachShader(cubeProgram, cubeShaders[1]);
		glBindAttribLocation(cubeProgram, 0, "in_Position");
		glBindAttribLocation(cubeProgram, 1, "in_Normal");
		linkProgram(cubeProgram);
	}
	void cleanupCube() {
		glDeleteBuffers(3, cubeVbo);
		glDeleteVertexArrays(1, &cubeVao);

		glDeleteProgram(cubeProgram);
		glDeleteShader(cubeShaders[0]);
		glDeleteShader(cubeShaders[1]);
	}
	void updateCube(const glm::mat4& transform) {
		objMat = transform;
	}
	void drawCube() {
		glEnable(GL_PRIMITIVE_RESTART);
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.1f, 1.f, 1.f, 0.f);
		glDrawElements(GL_TRIANGLE_STRIP, numVerts, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_PRIMITIVE_RESTART);
	}


	void updateColor(const glm::vec4 newColor) {
		myColor = newColor;
	}
}

void InitMatsFall(double currentTime, bool fall) {
	if (fall)
	{
		RotMatX = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, (float)cos(currentTime),(float)sin(currentTime), 0.f,
			0.f, (float)-sin(currentTime), (float)cos(currentTime), 0.f,
			0.f, 0.f, 0.f, 1.f
		};

		RotMatY = glm::mat4{
			(float)cos(currentTime), 0.f, (float)-sin(currentTime), 0.f,
			0.f, 1.f, 0.f, 0.f,
			(float)sin(currentTime), 0.f, (float)cos(currentTime), 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		RotMatZ = glm::mat4{
			(float)cos(currentTime), (float)-sin(currentTime), 0.f, 0.f,
			(float)sin(currentTime), (float)cos(currentTime), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};

		RotMatXZ = RotMatX * RotMatZ;
		RotMatYZ = RotMatY * RotMatZ;
		RotMatYX = RotMatY * RotMatX;

		scale = glm::mat4{
			0.5f, 0.f, 0.f, 0.f,
			0.0f, 0.5f, 0.f, 0.f,
			0.0f, 0.f, 0.5f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};


	}
	else
	{
		RotMatX = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f,0.f,0.f,
			0.f,0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};

		RotMatY = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f,0.f,0.f,
			0.f,0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		RotMatZ = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f,0.f,0.f,
			0.f,0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		scale = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};

		RotMatXZ = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};

		RotMatYZ = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};

		RotMatYX = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};
	}
}

void InitMatsFallCube(double currentTime, bool fall) {
	if (fall)
	{
		RotMatXCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, (float)cos(currentTime),(float)sin(currentTime), 0.f,
			0.f, (float)-sin(currentTime), (float)cos(currentTime), 0.f,
			0.f, 0.f, 0.f, 1.f
		};

		RotMatYCube = glm::mat4{
			(float)cos(currentTime), 0.f, (float)-sin(currentTime), 0.f,
			0.f, 1.f, 0.f, 0.f,
			(float)sin(currentTime), 0.f, (float)cos(currentTime), 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		RotMatZCube = glm::mat4{
			(float)cos(currentTime), (float)-sin(currentTime), 0.f, 0.f,
			(float)sin(currentTime), (float)cos(currentTime), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};

		scaleCube = glm::mat4{
			0.5f, 0.f, 0.f, 0.f,
			0.0f, 0.5f, 0.f, 0.f,
			0.0f, 0.f, 0.5f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};

		RotMatXZCube = RotMatXCube * RotMatZCube;
		RotMatYZCube = RotMatYCube * RotMatZCube;
		RotMatYXCube = RotMatYCube * RotMatXCube;
	}
	else
	{
		RotMatXCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f,0.f,0.f,
			0.f,0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};

		RotMatYCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f,0.f,0.f,
			0.f,0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		RotMatZCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f,0.f,0.f,
			0.f,0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
		scaleCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};

		RotMatXZCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};

		RotMatYZCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};

		RotMatYXCube = glm::mat4{
			1.f, 0.f, 0.f, 0.f,
			0.0f, 1.f, 0.f, 0.f,
			0.0f, 0.f, 1.f, 0.f,
			0.0f, 0.f, 0.f, 1.f,
		};
	}
}