
#define GL3_PROTOTYPES 1

#include "draw.h"
#include "matrix.h"
#include <math.h>
#include <vector>

#include "shaderprogram.h"

using namespace std;

static struct {
	GLint color;
	GLuint vertices;
	GLuint mvpMatrix;

	const char * vertexCode =
		R"___(
		#version 330 core
		uniform vec4 uColor;
		uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix\n"
		out vec4 fColor;
		in vec4 vPosition;
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



//static GLfloat transformMatrix[16];
static Matrix<float> mvpMatrix;
static double screenWidth, screenHeight;
ShaderProgram *squareShaderProgram = 0;



//Square
static const vector<float> squareVertices = { 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f };
static const vector<float> squareColors = {.8, .8, 1., 1};


void drawSquare(Vec p, double a, double sx, double sy, DrawStyle drawStyle){
	squareShaderProgram->useProgram();

	{
		mvpMatrix = mvpMatrix.RotationZ(a / 180.);
		mvpMatrix.scale(sx, sy, 1);

		mvpMatrix.scale(1. / screenWidth, 1. / screenHeight, 1);
		mvpMatrix.setTranslation(
			p.x / screenWidth * 2 - 1.,
			p.y / screenHeight * 2 - 1.,
			p.z
		);

		glUniformMatrix4fv(program1.mvpMatrix, 1, GL_FALSE, mvpMatrix);
	}


    glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, &squareVertices[0]);
    glEnableVertexAttribArray(program1.vertices);


    //Set up uniform i.e data that is costant for all vertices
    glUniform4fv(program1.color, 1, &squareColors[0]);

    glDrawArrays((drawStyle == DRAW_STYLE_LINES)? GL_LINE_LOOP: GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(program1.vertices);
}

void setDimensions(double width, double height){
	screenWidth = width;
	screenHeight = height;
}

bool initDrawModule(double width, double height) {
	if (squareShaderProgram){
		delete squareShaderProgram;
	}
    squareShaderProgram = new ShaderProgram(program1.vertexCode, program1.fragmentCode);

    if (!squareShaderProgram->getProgram()) {
        debug_print("Could not create program.");
        return false;
    }

    squareShaderProgram->useProgram();
    checkGlError("glUseProgram");

	program1.vertices = squareShaderProgram->getAttribute("vPosition");
	program1.color = squareShaderProgram->getUniform("uColor");
	program1.mvpMatrix = squareShaderProgram->getUniform("mvp_matrix");


	setDimensions(width, height);
	return false;
}

void QuitDrawModule() {
	delete squareShaderProgram;
}
