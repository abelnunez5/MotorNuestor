#include <glad/glad.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
};

int main(int argc, char* argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO))
		return -1;

	//Opengl attributes required to know before we create the window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer (it is the default)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with minimum 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with minimum 8 bits

	Uint32 flags = SDL_WINDOW_OPENGL;

	//window start params
	//#define WINDOW_FULLSCREEN
#define WINDOW_RESIZEABLE

#ifdef WINDOW_FULLSCREEN
	flags |= SDL_WINDOW_FULLSCREEN;
#endif
#ifdef WINDOW_RESIZEABLE
	flags |= SDL_WINDOW_RESIZABLE;
#endif
	unsigned int wWidth = 1280;
	unsigned int wHeight = 720;
	SDL_Window* wdn = SDL_CreateWindow("Engine", wWidth, wHeight, flags);

	if (wdn == nullptr)
		return -1;

	SDL_GLContext ctx = SDL_GL_CreateContext(wdn);
	if (!ctx)
		return -1;

	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0)
		return -1;

	glViewport(0, 0, wWidth, wHeight);
	glClearColor(0.1f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	Vertex cubeVertices[] = {
		// Cara de delante
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 0.0f) },

		// Cara deatras
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 1.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.5f, 0.0f) }
	};

	GLuint qVBuff;
	glGenBuffers(1, &qVBuff);
	glBindBuffer(GL_ARRAY_BUFFER, qVBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	unsigned int indices[] = {
		0,1,2,2,1,3,

		1,5,3,3,5,7,

		5,4,7,7,4,6,

		4,0,6,6,0,2,

		2,3,6,6,3,7,

		4,5,0,0,5,1


	};
	GLuint iBuff;
	glGenBuffers(1, &iBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotationAxis = glm::vec3(0.5f, 1.0f, 0.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float angleDegrees = 0.0f;
	Uint64 lastTicks = SDL_GetTicks();

	bool running = true;
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_EVENT_QUIT)
				running = false;
			else if (e.type == SDL_EVENT_WINDOW_RESIZED)
			{
				wWidth = e.window.data1;
				wHeight = e.window.data2;
				if (wHeight == 0) wHeight = 1;
				glViewport(0, 0, wWidth, wHeight);
			}
		}

		Uint64 currentTicks = SDL_GetTicks();
		float deltaTime = (currentTicks - lastTicks) / 1000.0f;
		lastTicks = currentTicks;

		angleDegrees += 45.0f * deltaTime;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePosition);
		model = glm::rotate(model, glm::radians(angleDegrees), rotationAxis);
		model = glm::scale(model, cubeScale);

		glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

		float aspectRatio = static_cast<float>(wWidth) / static_cast<float>(wHeight);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(projection));

		glm::mat4 modelView = view * model;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(modelView));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, qVBuff);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		glColorPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuff);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		SDL_GL_SwapWindow(wdn);
	}
	return 0;
}