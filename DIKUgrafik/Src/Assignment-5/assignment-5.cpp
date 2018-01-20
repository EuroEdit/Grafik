#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "DIKUgrafik/errorcheck.h"
#include "DIKUgrafik/ifile.h"
#include "DIKUgrafik/glmutils.h"
#include "DIKUgrafik/windowutils.h"
#include "DIKUgrafik/shaderutils.h"
#include "DIKUgrafik/camera.h"
#include "DIKUgrafik/bezierpatch.h"

// Sampling Type - only define one of them at a time - but do it for all of them
#define SAMPLE
// #define FORWARDDIFFERENCES
// #define SUBDIVISION
// #define FLATNESS

// Parameters for the different sampling methods
int   const Nsamples         = 100;
int   const Nsubdivisions    = 5;
float const Epsilon          = 0.1f;
int   const MaxFlatnessTests = 5;

#define BUFFER_OFFSET(bytes) ((GLvoid*) (bytes))

// Bezier matrices
glm::mat4x4 BasisMatrix;
glm::mat4x4 DLB;
glm::mat4x4 DRB;


/**
 * Samples a Bezier curve N times and returns the sample points in a vector.
 * \param G - the geometry matrix for the Bezier curve.
 * \param N - the number of sample points on the curve.
 * \param Vertices - a vector in which the sample points are returned.
 */
void Sample(BezierRow const& G, int N, std::vector<glm::vec3>& Vertices)
{   
	float t1 = 0.0f;
    float t2 = 0.0f;
    float t3 = 0.0f;
    float delta_t = 1.0f / float(N); //calculate step-size
    glm::vec4 T(0.0f);
    
    Vertices.push_back(G[1]); //Add start-point
    glm::vec3 vertex;
    
    for (int i = 2; i < N; ++i) {
		t1 += delta_t;	//calculate new t value
		t2 = t1 * t1; 	// t^2
		t3 = t2 * t1;	// t^3
		T = glm::vec4(t3, t2, t1, 1.0f); // T vector
		vertex = G * BasisMatrix * T; // Calculate point on curve
		Vertices.push_back(vertex); //Add new point to vertices
		Vertices.push_back(vertex); //Add again because same point is both start and endpoint for different lines
    }

    Vertices.push_back(G[4]); //Add end-point
}

/**
 * Subdivides a Bezier curve N times and returns the sample points in a vector.
 * \param G - the geometry matrix for the Bezier curve.
 * \param N - the number of times the curve should be subdivided.
 * \param Vertices - a vector in which the sample points are returned.
 */
void SubDivide(BezierRow const& G, int N, std::vector<glm::vec3>& Vertices)
{
    if (N == 0) {
		Vertices.push_back(G[1]); //No more recursion so simply add beginning and end point of current segment
		Vertices.push_back(G[4]);
    }
    else {
		SubDivide(G * DLB, N-1, Vertices); // Subdivide to Left segment, decrement N
		SubDivide(G * DRB, N-1, Vertices); // Subdivide to Right segment, decrement N
    }
}

/**
 * Global Variables
 * \param WindowWidth - the current width of the window
 * \param WindowHeight - the current height of the window
 * \param WindowTitle - the current title of the window * \param NumberOfCurves - the total number of test curves
 * \param CurrentCurve - the number of the current curve to be drawn
 * \param NVertices - the number of vertices on the current curve
 */
int WindowWidth  = 500;
int WindowHeight = 500;
std::string WindowTitle("Assignment 5: Bezier Curves");

int const NumberOfCurves = 5;
std::vector<glm::vec3> Vertices[NumberOfCurves];
int CurrentCurve         = 0;


/**
 * Prints out a std::vector<glm::vec3> to an output stream
 * \param s - The output stream that should be printed to
 * \param data - The data that should be printed
 * \return the output stream s
 */
std::ostream& operator<<(std::ostream& s, std::vector<glm::vec3> const& data)
{
    for (int i = 0; i < data.size(); ++i) {
	s << data[i] << std::endl;
    }
    return s;
}

/**
 * Callback function for window resize
 * \param Window - A pointer to the window beeing resized
 * \param width - The new width of the window
 * \param height - The new height of the window
 */
void ResizeCallback(GLFWwindow* Window, int width, int height)
{
    //std::cout << "-->ResizeCallback(GLFWwindow* Window, int width, int height)" << std::endl;

    //std::cout << "<--ResizeCallback(GLFWwindow* Window, int width, int height)" << std::endl;
}


/**
 * Callback function for keyboard shortcuts
 * \param Window - the window that created the callback
 * \param key - The key code
 * \param scancode - Don't know right now? It is badly documented in GLFW
 * \param action - Can be any of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
 * \param mods - Modifier keys pressed at the same time, like shift etc.
 */
void KeyboardCallback(GLFWwindow* Window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_RELEASE) return;

    // std::cout << "--->KeyboardCallback(...)" << std::endl;

    if (action == GLFW_PRESS) {
	if (key == GLFW_KEY_ESCAPE) {
	    glfwSetWindowShouldClose(Window, GL_TRUE);
	}
	else {
	    int intkey = key - '0' - 1;
	    if (intkey >= 0 && intkey <= NumberOfCurves - 1) 
		CurrentCurve = intkey;
	    else {
		std::stringstream errormessage;
		errormessage << "No such Curve: " << intkey + 1 << ": the key must be in the range [1,..., " << NumberOfCurves << "]";
		std::cerr << errormessage.str() << std::endl;
		CurrentCurve = 0;
	    }
	}
    }

    // std::cout << "<---KeyboardCallback(...)" << std::endl;
}

int main() 
{
    try {
	// Initialize the Bezier Basis Matris
	BasisMatrix = glm::mat4x4(glm::vec4(-1.0f,  3.0f, -3.0f, 1.0f),
				  glm::vec4 (3.0f, -6.0f,  3.0f, 0.0f),
				  glm::vec4(-3.0f,  3.0f,  0.0f, 0.0f),
				  glm::vec4( 1.0f,  0.0f,  0.0f, 0.0f));
	
	// Initialize DLB and DRB
	DLB = glm::mat4x4(glm::vec4(8.0f, 0.0f, 0.0f, 0.0f),
			  glm::vec4(4.0f, 4.0f, 0.0f, 0.0f),
			  glm::vec4(2.0f, 4.0f, 2.0f, 0.0f),
			  glm::vec4(1.0f, 3.0f, 3.0f, 1.0f));
	DLB /= 8.0f;

	DRB = glm::mat4x4(glm::vec4(1.0f, 3.0f, 3.0f, 1.0f),
			  glm::vec4(0.0f, 2.0f, 4.0f, 2.0f),
			  glm::vec4(0.0f, 0.0f, 4.0f, 4.0f),
			  glm::vec4(0.0f, 0.0f, 0.0f, 8.0f));
	DRB /= 8.0f;
	
	/*
	 * Viewing parameters
	 */
	glm::vec3 vrp(0.0f, 0.0f, 10.0f);
	glm::vec3 vpn(0.0f, 0.0f, 1.0f);
	glm::vec3 vup(0.0f, 1.0f, 0.0f);
	glm::vec3 prp(0.0f, 0.0f, 100.0f);
	float F =  10.0f;
	float B = -80.0f;
	glm::vec2 lower_left(-20.0f, -20.0f);
	glm::vec2 upper_right(20.0f,  20.0f);

	Camera camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	
	glm::mat4x4 CTM = camera.CurrentTransformationMatrix();

	GLenum Error = GL_NO_ERROR;

	// Initialize graphics
	InitializeGLFW();
	GLFWwindow* Window = CreateWindow(WindowWidth, WindowHeight, WindowTitle.c_str());
	if (!Window) {
	    throw std::runtime_error("Could not create window");
	}
	if (Window != NULL) {
	    glfwMakeContextCurrent(Window);
	}
	InitializeGLEW();
	InitializeOpenGL();
	glfwSwapBuffers(Window);

	// Read and Compile the vertex program vertextransform.vert
	GLuint vertexprogID = CreateGpuProgram("vertextransform.vert", GL_VERTEX_SHADER);

	// Read and Compile the fragment program linefragment.frag
	GLuint fragmentprogID = CreateGpuProgram("linefragment.frag", GL_FRAGMENT_SHADER);

	// Create a lineshader program and Link it with the vertex and fragment programs
	GLuint lineshaderID = CreateShaderProgram(vertexprogID, fragmentprogID);

	// Now comes the OpenGL core part
	BezierRow G[NumberOfCurves];

	G[0] = BezierRow(glm::vec3(-15.0f, -15.0f, 0.0f),
			 glm::vec3(-10.0f,  25.0f, 0.0f),
			 glm::vec3( 10.0f,  25.0f, 0.0f),
			 glm::vec3( 15.0f, -15.0f, 0.0f));
	
	G[1] = BezierRow(glm::vec3(-20.0f,   0.0f, 0.0f),
			 glm::vec3( -1.0f,  55.0f, 0.0f),
			 glm::vec3(  1.0f, -55.0f, 0.0f),
			 glm::vec3( 20.0f,   0.0f, 0.0f));

	G[2] = BezierRow(glm::vec3(-1.0f, -5.0f, 0.0f),
			 glm::vec3(-60.0f, 5.0f, 0.0f),
			 glm::vec3( 60.0f, 5.0f, 0.0f),
			 glm::vec3( 1.0f, -5.0f, 0.0f));

	G[3] = BezierRow(glm::vec3(-10.0f, -5.0f, 0.0f),
			 glm::vec3( 60.0f,  5.0f, 0.0f),
			 glm::vec3(-60.0f,  5.0f, 0.0f),
			 glm::vec3( 10.0f, -5.0f, 0.0f));

	G[4] = BezierRow(glm::vec3( 0.0f,   0.0f, 0.0f),
			 glm::vec3(-60.0f,  5.0f, 0.0f),
			 glm::vec3( 60.0f,  5.0f, 0.0f),
			 glm::vec3( 0.0f,   0.0f, 0.0f));

	
        // Make a VertexArrayObject - it is used by the VertexBufferObjects, and it must be declared!
	GLuint LineArrayID[NumberOfCurves];
	glGenVertexArrays(NumberOfCurves, LineArrayID);
	for (int i = 0; i < NumberOfCurves; ++i) {
	    if (LineArrayID[i] == 0) {
		std::stringstream errormessage;
		errormessage << "Could not create VertexArray[" << i << "]";
		throw std::runtime_error(errormessage.str());
	    }
	}

	GLuint vertexbuffer[NumberOfCurves];
	glGenBuffers(NumberOfCurves, vertexbuffer);
	for (int i = 0; i < NumberOfCurves; ++i) {
	    if (vertexbuffer[i] == 0) {
		std::stringstream errormessage;
		errormessage << "Could not create VertexBuffer[" << i << "]";
		throw std::runtime_error(errormessage.str());
	    }
	}

	for (int i = 0; i < NumberOfCurves; ++i) {
	    glBindVertexArray(LineArrayID[i]);

	    // Make a VertexbufferObject for the line segment vertices - it uses the current VertexArrayBuffer!
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);

	    // Generate vertices
#ifdef SAMPLE
	    Vertices[i].clear();
	    Sample(G[i], Nsamples, Vertices[i]);
#endif

#ifdef FORWARDDIFFERENCES
	    Vertices[i].clear();
	    SampleFWD(G[i], Nsamples, Vertices[i]);
#endif
	    
#ifdef SUBDIVISION
	    Vertices[i].clear();
	    SubDivide(G[i], Nsubdivisions, Vertices[i]);
#endif

#ifdef FLATNESS
	    Vertices[i].clear();
	    SubDivide(G[i], Epsilon, Vertices[i], MaxFlatnessTests);
#endif
	
	    // Give our vertices to OpenGL.
	    glBufferData(GL_ARRAY_BUFFER, Vertices[i].size() * 3 * sizeof(float),
			 glm::value_ptr(Vertices[i][0]), GL_STATIC_DRAW);

	    // Initialize line segment vertex Attributes
	    GLuint vertexattribute = glGetAttribLocation(lineshaderID, "VertexPosition");
	    glEnableVertexAttribArray(vertexattribute);
	    glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	    // Validate the line shader program
	    ValidateShader(lineshaderID);

	    glBindVertexArray(0);
	}
	
	// Get locations of Uniforms
	GLuint ctm  = glGetUniformLocation(lineshaderID, "CTM");
	GLuint color= glGetUniformLocation(lineshaderID, "Color");

	// The main loop
	std::cout << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	std::cout << "* Press ESC to finish the program                               *" << std::endl;
	std::cout << "* Press '1',..., '5' to choose different curves                 *" << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	std::cout << std::endl;

	while (!glfwWindowShouldClose(Window)) {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // Use the Lineshader
	    glUseProgram(lineshaderID);

            // Bind the underlying VertexArrayObject
	    glBindVertexArray(LineArrayID[CurrentCurve]);   // It is very important to bind the Vertex Array Object!

	    // Assign values to the uniforms
	    glUniform3f(color, 0.0f, 0.0f, 0.0f);
	    glUniformMatrix4fv(ctm, 1, GL_FALSE, glm::value_ptr(CTM));

	    glDrawArrays(GL_LINES, 0, Vertices[CurrentCurve].size());

	    glBindVertexArray(0);
	    glUseProgram(0);
	    
	    glfwSwapBuffers(Window);

#if 0
	    std::stringstream errormessage;
	    errormessage << "End of loop: " << "assignment-5.cpp" << ": " << __LINE__ << ": ";
	    ErrorCheck(errormessage.str());
#endif
	    glfwPollEvents();
	}
    }
    catch (std::exception const& exception) {
	std::cerr << "Exception: " << exception.what() << std::endl;
    }

    glfwTerminate();

    return 0;
}

