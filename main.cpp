#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

// Use core profile only
// All gl-includes is included in shaderprogram.h, but added here also for clarity
#define GL3_PROTOTYPES 1
#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>

#include "matrix.h"
#include "shaderprogram.h"

using namespace std;

static struct {
	GLint color;
	GLuint vertices;
	GLuint mvpMatrix;

	const char * vertexCode =
		R"___(
		#version 330 core

		in vec4 vPosition;

		uniform vec4 uColor;
		uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix\n"
		out vec4 fColor;
		void main() {
		   gl_Position = mvp_matrix * vPosition;
		   fColor = uColor;
		}
	)___";


	const char * fragmentCode =
	    R"__(
	    #version 330 core
	    in vec4 fColor;
	    out vec4 FragColor;

	    void main() {
	      gl_FragColor = fColor;
	    })__";

} program1;



static Matrix<float> mvpMatrix;
static double screenWidth, screenHeight;
static ShaderProgram squareShaderProgram;

static GLuint vertexBuffer;

//Square
static const vector<float> squareVertices = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f };

static const vector<float> squareColors = {.8, .8, 1., 1};



static void setDimensions(double width, double height){
	screenWidth = width;
	screenHeight = height;
}



static bool initDrawModule(double width, double height) {
	GLuint vao; //The dumb way ouf declaring a VertexArray in the beginning and never change it
	glCall(glGenVertexArrays(1, &vao));
	glCall(glBindVertexArray(vao));

	glCall(glGenBuffers(1, &vertexBuffer));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	glCall(glBufferData(GL_ARRAY_BUFFER, sizeof (float) * squareVertices.size(), &squareVertices[0], GL_STATIC_DRAW));

	squareShaderProgram.initProgram(program1.vertexCode, program1.fragmentCode);
    if (!squareShaderProgram.getProgram()) {
        cerr << "Could not create shader program in " << __FILE__ << ":" << __LINE__ << endl;
        return false;
    }


	program1.vertices = squareShaderProgram.getAttribute("vPosition");
	program1.color = squareShaderProgram.getUniform("uColor");
	program1.mvpMatrix = squareShaderProgram.getUniform("mvp_matrix");

	setDimensions(width, height);

	return false;
}


//static void drawSquare(Vec p, double a, double sx, double sy){
static void render() {
	static float tmp = 0;

	//Calculate som random location
	Vec p(.1 + tmp + 50, 1);
	float a = 20. + tmp * 2;
	float sx = 100, sy = 100;
	tmp += 20;

	squareShaderProgram.use();

	{
		mvpMatrix = mvpMatrix.RotationZ(a / 180.);
		mvpMatrix.scale(sx, sy, 1);

		mvpMatrix.scale(1. / screenWidth, 1. / screenHeight, 1);
		mvpMatrix.setTranslation(
			p.x / screenWidth * 2 - 1.,
			p.y / screenHeight * 2 - 1.,
			p.z
		);

		glCall(glUniformMatrix4fv(program1.mvpMatrix, 1, GL_FALSE, mvpMatrix));
	}


    glCall(glUniform4fv(program1.color, 1, &squareColors[0]));

    glCall(glEnableVertexAttribArray(program1.vertices));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer))
    glCall(glVertexAttribPointer(
     		program1.vertices,
 			2,
 			GL_FLOAT,
 			GL_FALSE,
 			0,
 			(void *)0));


	glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));

    glDisableVertexAttribArray(program1.vertices);
}




bool init() {
	return initDrawModule(512, 512);
}




void die(string message);
void checkSDLError(int line = -1);



int main(int argc, char *argv[])
{
    SDL_Window *mainwindow;
    SDL_GLContext maincontext;

    //Request opengl 3.2 context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //May depend on system
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    mainwindow = SDL_CreateWindow("sdl-window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    checkSDLError(__LINE__);

    if (!mainwindow) { /* Die if creation failed */
        die("Unable to create window");
    }

    maincontext = SDL_GL_CreateContext(mainwindow);
    checkSDLError(__LINE__);

    init();

    // Sync buffer swap  with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    for (int i = 0; i < 10; ++i) {
    	glClearColor ( 0.1 * i, 0.0, 0.0, 1.0 );
    	glClear ( GL_COLOR_BUFFER_BIT );
    	render();
    	SDL_GL_SwapWindow(mainwindow);
    	// Wait
    	SDL_Delay(200);
    }

    // Delete our opengl context, destroy our window, and shutdown SDL
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();

    return 0;
}






void die(string message) {
	cout << message << endl;
	SDL_Quit();
	exit(1);
}

void checkSDLError(int line){
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		cout << "SDL_Error " << error;
		if (line != -1) {
			cout << " line: " << line;
		}
		cout << endl;
		SDL_ClearError();
	}
#endif
}


