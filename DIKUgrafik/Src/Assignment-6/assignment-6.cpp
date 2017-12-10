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
#include "DIKUgrafik/phongsurface.h"
#include "DIKUgrafik/dinisurface.h"
#include "DIKUgrafik/kleinbottle.h"
#include "DIKUgrafik/beziersurface.h"


#define BUFFER_OFFSET(bytes) ((GLvoid*) (bytes))


/**
 * Global Variables
 * \param WindowWidth - the current width of the window
 * \param WindowHeight - the current height of the window
 * \param WindowTitle - the current title of the window * \param NumberOfCurves - the total number of test curves
 * \param CurrentSurface - the number of the current surface to be drawn
 * \param NumberOfSurfaces - the number of surfaces
 * \param NVertices - the number of vertices of surfaces
 */
int WindowWidth  = 500;
int WindowHeight = 500;
std::string WindowTitle("Assignment 6: Parametric Surfaces");
int CurrentSurface   = 0;
int const NumberOfSurfaces = 12;
int NVertices[NumberOfSurfaces];

//#define COORDINATE_AXES 1
//#define BOX 1
#define TEAPOT 1
#define PHONGSURFACE 1
#define DINISURFACE 1
#define KLEINBOTTOM 1
#define KLEINHANDLE 1
#define KLEINTOP 1
#define KLEINMIDDLE 1
// In order to use the next define
// KLEINBOTTOM, KLEINHANDLE, and KLEINTOP must be defined
#define KLEINBOTTLE3SURFACES 1
// In order to use the next define
// KLEINBOTTOM, KLEINHANDLE, KLEINTOP, and KLEINMIDDLE must be defined
#define KLEINBOTTLE 1
#define ROCKET 1
#define PAIN 1
#define PATCHES 1

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
 * Prints the transformed vertices
 * \param Vertices - the vertices (a std::vector) of the figure.
 * \param CTM - the Current Transformation Matrix
 */
void printtransformedvertices(std::vector<glm::vec3> const& Vertices, glm::mat4x4 const& CTM)
{
    std::cout << "--> printtransformedvertices(...)" << std::endl;
    int size = Vertices.size();
    int width = 1;
    while (size > 10) { size /= 10; ++width; }
    std::cout << "width = " << width << std::endl;
    for (int i = 0; i < Vertices.size(); ++i) {
	std::cout << "point[" << std::setw(width) << i+1 << "]: [" << Vertices[i] << "] ---> ";
	glm::vec4 transformedvertex = CTM * glm::vec4(Vertices[i], 1.0f);
	transformedvertex /= transformedvertex.w;
	glm::vec3 eucledian(transformedvertex.x, transformedvertex.y, transformedvertex.z);
	std::cout << "[" << eucledian << "]" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "<-- printtransformedvertices(...)" << std::endl;
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
	    if (intkey >= 0 && intkey <= NumberOfSurfaces - 1) 
		CurrentSurface = intkey;
	    else {
		intkey = key - 'A' - 1 + 10;
		if (intkey >= 0 && intkey <= NumberOfSurfaces - 1)
		    CurrentSurface = intkey;
		else {
		    std::stringstream errormessage;
		    errormessage << "No such Surface: " << intkey + 1
				 << ": the key must be in the range [1,..., " << NumberOfSurfaces << "]";
		    std::cerr << errormessage.str() << std::endl;
		    CurrentSurface = 0;
		}
	    }
	}
    }

    // std::cout << "<---KeyboardCallback(...)" << std::endl;
}

int main() 
{
    try {
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

	// Now comes the OpenGL core part
	// Generate the VertexArrays and the Buffers
	
        // Make a VertexArrayObject - it is used by the VertexBufferObjects, and it must be declared!
	GLuint SurfaceArrayID[NumberOfSurfaces];
	glGenVertexArrays(NumberOfSurfaces, SurfaceArrayID);
	for (int i = 0; i < NumberOfSurfaces; ++i) {
	    if (SurfaceArrayID[i] == 0) {
		std::stringstream errormessage;
		errormessage << "Could not create VertexArray[" << i << "]";
		throw std::runtime_error(errormessage.str());
	    }
	}

	// Generate vertex buffers
	GLuint vertexbuffer[NumberOfSurfaces];
	glGenBuffers(NumberOfSurfaces, vertexbuffer);
	for (int i = 0; i < NumberOfSurfaces; ++i) {
	    if (vertexbuffer[i] == 0) {
		std::stringstream errormessage;
		errormessage << "Could not create VertexBuffer[" << i << "]";
		throw std::runtime_error(errormessage.str());
	    }
	}

	// Generate normal buffers
	GLuint normalbuffer[NumberOfSurfaces];
	glGenBuffers(NumberOfSurfaces, normalbuffer);
	for (int i = 0; i < NumberOfSurfaces; ++i) {
	    if (normalbuffer[i] == 0) {
		std::stringstream errormessage;
		errormessage << "Could not create NormalBuffer[" << i << "]";
		throw std::runtime_error(errormessage.str());
	    }
	}
	
	// Camera
	Camera camera[NumberOfSurfaces];

	// Transformations
	glm::mat4x4 CTM[NumberOfSurfaces];
	glm::mat4x4 invCTMtranspose[NumberOfSurfaces];
	glm::mat4x4 VertexTransformation[NumberOfSurfaces];
	glm::mat4x4 NormalTransformation[NumberOfSurfaces];
	glm::vec3   LightPosition[NumberOfSurfaces];
	glm::vec3   LightColor[NumberOfSurfaces];
	glm::vec4   TmpEyePosition[NumberOfSurfaces];
	glm::vec3   EyePosition[NumberOfSurfaces];
	
	// Setup all the shaders
	
	// Read and Compile the vertex program linevertex.vert
	GLuint linevertexprogID   = CreateGpuProgram("linevertex.vert", GL_VERTEX_SHADER);

	// Read and Compile the fragment program linefragment.frag
	GLuint linefragmentprogID = CreateGpuProgram("linefragment.frag", GL_FRAGMENT_SHADER);

	// Create a lineshader program and Link it with the vertex and fragment programs
	GLuint lineshaderID = CreateShaderProgram(linevertexprogID, linefragmentprogID);

	// Get location of Uniforms for the line shader
	GLint linectmuniform      = glGetUniformLocation(lineshaderID, "CTM");
	GLint linecoloruniform    = glGetUniformLocation(lineshaderID, "Color");

	// Get location of the Vertex Attributes for the line shader
	GLint linevertexattribute = glGetAttribLocation(lineshaderID,  "VertexPosition");;
	
	// Read and Compile the vertex program vertextransform.vert
	GLuint vertexprogID = CreateGpuProgram("vertextransform.vert", GL_VERTEX_SHADER);

	// Read and Compile the fragment program phong.frag
	GLuint fragmentprogID = CreateGpuProgram("phong.frag", GL_FRAGMENT_SHADER);

	// Create a surfaceshader program and Link it with the vertex and fragment programs
	GLuint surfaceshaderID = CreateShaderProgram(vertexprogID, fragmentprogID);

	// Get locations of Uniforms for the surface shader
	// Transformations
	GLint ctm                    = glGetUniformLocation(surfaceshaderID, "CTM");
	GLint invctmtranspose        = glGetUniformLocation(surfaceshaderID, "invCTMtranspose");
	GLint vertextransform        = glGetUniformLocation(surfaceshaderID, "VertexTransformation");
	GLint normaltransform        = glGetUniformLocation(surfaceshaderID, "NormalTransformation");

	// Light Colors
	GLint lightposition          = glGetUniformLocation(surfaceshaderID, "LightPosition");
	GLint lightcolor             = glGetUniformLocation(surfaceshaderID, "LightColor");

	// Eye Position
	GLint eyeposition            = glGetUniformLocation(surfaceshaderID, "EyePosition");

	// Front material properties
	GLint frontambientcolor      = glGetUniformLocation(surfaceshaderID, "FrontAmbientColor");
	GLint frontdiffusecolor      = glGetUniformLocation(surfaceshaderID, "FrontDiffuseColor");
	GLint frontspecularcolor     = glGetUniformLocation(surfaceshaderID, "FrontSpecularColor");
	GLint frontshininess         = glGetUniformLocation(surfaceshaderID, "FrontShininess");

	// Back material properties
	GLint backambientcolor       = glGetUniformLocation(surfaceshaderID, "BackAmbientColor");
	GLint backdiffusecolor       = glGetUniformLocation(surfaceshaderID, "BackDiffuseColor");
	GLint backspecularcolor      = glGetUniformLocation(surfaceshaderID, "SBackpecularColor");
	GLint backshininess          = glGetUniformLocation(surfaceshaderID, "BackShininess");

	// Get location of the Vertex Attributes for the surface shader
	GLint vertexattribute = glGetAttribLocation(surfaceshaderID, "Vertex");
	GLint normalattribute = glGetAttribLocation(surfaceshaderID, "Normal");

	// Front colors
	float     frontKa = 0.5f;
	glm::vec3 frontOa(0.0f, 1.0f, 0.0f);
	glm::vec3 FrontAmbientColor(frontKa * frontOa);

	float     frontKd = 0.75f;
	glm::vec3 frontOd(1.0f, 0.843f, 0.0f); // frontOd(0.0f, 1.0f, 0.0f);
	glm::vec3 FrontDiffuseColor(frontKd * frontOd);

	float     frontKs = 0.9f;
	glm::vec3 frontOs(1.0f, 1.0f, 1.0f);
	glm::vec3 FrontSpecularColor(frontKs * frontOs);

	float FrontShininess = 20.0f;

	// Back colors
	float     backKa = 0.5f;
	glm::vec3 backOa(1.0f, 0.0f, 0.0f);
	glm::vec3 BackAmbientColor(backKa * backOa);

	float     backKd = 0.75f;
	glm::vec3 backOd(1.0f, 0.0f, 0.0f);
	glm::vec3 BackDiffuseColor(backKd * backOd);

	float     backKs = 0.9f;
	glm::vec3 backOs(1.0f, 1.0f, 1.0f);
	glm::vec3 BackSpecularColor(backKs * backOs);

	float BackShininess = 20.0f;
	
#ifdef COORDINATE_AXES
	// Generate a VertexArrayObjects for each axis
	GLuint CoordinateAxes[3];
	glGenVertexArrays(3, CoordinateAxes);
	for (int i = 0; i < 3; ++i) {
	    if (CoordinateAxes[i] == 0) {
		std::stringstream errormessage;
		errormessage << "Could not create Coordinate Axes VertexArray[" << i << "]";
		throw std::runtime_error(errormessage.str());
	    }
	}

	// Generate vertex buffers
	GLuint AxisBuffer[3];
	glGenBuffers(3, AxisBuffer);
	for (int i = 0; i < 3; ++i) {
	    if (AxisBuffer[i] == 0) {
		std::stringstream errormessage;
		errormessage << "Could not create VertexBuffer[" << i << "] for the coorsinate axes";
		throw std::runtime_error(errormessage.str());
	    }
	}

	std::vector<glm::vec3> AxisData[3];
	AxisData[0].push_back(glm::vec3(-1.0f,  0.0f,  0.0f));
	AxisData[0].push_back(glm::vec3( 4.0f,  0.0f,  0.0f));
	AxisData[1].push_back(glm::vec3( 0.0f, -1.0f,  0.0f));
	AxisData[1].push_back(glm::vec3( 0.0f,  4.0f,  0.0f));
	AxisData[2].push_back(glm::vec3( 0.0f,  0.0f, -1.0f));
	AxisData[2].push_back(glm::vec3( 0.0f,  0.0f,  4.0f));

	for (int axis = 0; axis < 3; ++axis) {
	    glBindVertexArray(CoordinateAxes[axis]);
	        glBindBuffer(GL_ARRAY_BUFFER, AxisBuffer[axis]);
	            glBufferData(GL_ARRAY_BUFFER, AxisData[axis].size() * 3 * sizeof(float),
				 glm::value_ptr(AxisData[axis][0]), GL_STATIC_DRAW);
		    // Initialize the vertex Attributes
		    linevertexattribute = glGetAttribLocation(lineshaderID, "VertexPosition");
		    glEnableVertexAttribArray(linevertexattribute);
		    glVertexAttribPointer(linevertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	    glBindVertexArray(0);
	}
#endif

#ifdef BOX
	// Generate a VertexArrayObject for the Box
	GLuint Box;
	glGenVertexArrays(1, &Box);
	if (Box == 0) {
	    std::stringstream errormessage;
	    errormessage << "Could not create Box VertexArray";
	    throw std::runtime_error(errormessage.str());
	}

	// Generate a vertex buffer for the Box
	GLuint BoxBuffer;
	glGenBuffers(1, &BoxBuffer);
	if (BoxBuffer == 0) {
	    std::stringstream errormessage;
	    errormessage << "Could not create Box VertexBuffer";
	    throw std::runtime_error(errormessage.str());
	}

	std::vector<glm::vec3> BoxCorners;
	BoxCorners.push_back(glm::vec3(-2.0f, -2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3( 2.0f, -2.0f, -0.5f));
	
	BoxCorners.push_back(glm::vec3( 2.0f, -2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3( 2.0f,  2.0f, -0.5f));

	BoxCorners.push_back(glm::vec3( 2.0f,  2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3(-2.0f,  2.0f, -0.5f));

	BoxCorners.push_back(glm::vec3(-2.0f,  2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3(-2.0f, -2.0f, -0.5f));

	BoxCorners.push_back(glm::vec3(-2.0f, -2.0f,  3.0f));
	BoxCorners.push_back(glm::vec3( 2.0f, -2.0f,  3.0f));
	
	BoxCorners.push_back(glm::vec3( 2.0f, -2.0f,  3.0f));
	BoxCorners.push_back(glm::vec3( 2.0f,  2.0f,  3.0f));

	BoxCorners.push_back(glm::vec3( 2.0f,  2.0f,  3.0f));
	BoxCorners.push_back(glm::vec3(-2.0f,  2.0f,  3.0f));

	BoxCorners.push_back(glm::vec3(-2.0f,  2.0f,  3.0f));
	BoxCorners.push_back(glm::vec3(-2.0f, -2.0f,  3.0f));

	BoxCorners.push_back(glm::vec3(-2.0f, -2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3(-2.0f, -2.0f,  3.0f));

	BoxCorners.push_back(glm::vec3( 2.0f, -2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3( 2.0f, -2.0f,  3.0f));

	BoxCorners.push_back(glm::vec3( 2.0f,  2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3( 2.0f,  2.0f,  3.0f));

	BoxCorners.push_back(glm::vec3(-2.0f,  2.0f, -0.5f));
	BoxCorners.push_back(glm::vec3(-2.0f,  2.0f,  3.0f));

	glBindVertexArray(Box);
	    glBindBuffer(GL_ARRAY_BUFFER, BoxBuffer);
	    glBufferData(GL_ARRAY_BUFFER, BoxCorners.size() * 3 * sizeof(float),
			 glm::value_ptr(BoxCorners[0]), GL_STATIC_DRAW);
	    // Initialize the vertex Attributes
	    linevertexattribute = glGetAttribLocation(lineshaderID, "VertexPosition");
	    glEnableVertexAttribArray(linevertexattribute);
	    glVertexAttribPointer(linevertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glBindVertexArray(0);
#endif
	
#ifdef TEAPOT
	CurrentSurface = 0;
	BezierSurface teapot("../../Data/teapot.data");
	teapot.FrontFacing(false);
	teapot.NumberOfSubdivisions(3);
	NVertices[CurrentSurface] = teapot.Vertices().size();

	//std::cout << "BezierPatches:" << std::endl;
	//std::cout << teapot.Vertices();
	//std::cout << std::endl;
	//std::cout << "teapot.Vertices().size() = " << NVertices[CurrentSurface] << std::endl;

	// Viewing parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 5.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	
	    float F =   5.0f;
	    float B = -10.0f;

	    glm::vec2 lower_left(-4.0f, -4.0f);
	    glm::vec2 upper_right(4.0f,  4.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    float rotation_angle = 45.0f * glm::pi<float>() / 180.0f;
	    glm::mat4x4 R(glm::rotateZ(rotation_angle));
	    glm::mat4x4 invR(R);

	    float scale = 1.5f;
	    glm::mat4x4 S(glm::scale(scale, scale, scale));
	    
	    float invscale = 1.0f / scale;
	    glm::mat4x4 invS(glm::scale(invscale, invscale, invscale));
	    
	    VertexTransformation[CurrentSurface] = S * R;
	    NormalTransformation[CurrentSurface] = invR * invS;
	}
	
	//std::cout << "VertexTransformation =" << std::endl;
	//std::cout << VertexTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, 300.0f, 200.0f);
	LightColor[CurrentSurface] = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x, TmpEyePosition[CurrentSurface].y, TmpEyePosition[CurrentSurface].z);
	//std::cout << "EyePosition = " << EyePosition[CurrentSurface] << std::endl;

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, teapot.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(teapot.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, teapot.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(teapot.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//std::cout << "Validate TeaPot" << std::endl;
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);

	// printtransformedvertices(teapot.Vertices(), CTM * VertexTransformation);
#endif

#ifdef PHONGSURFACE
	++CurrentSurface;
	PhongSurface phongsurface;
	phongsurface.PhiSamples(150);
	phongsurface.ThetaSamples(150);
	NVertices[CurrentSurface] = phongsurface.Vertices().size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    float scalefactor    = 20.0f;
	    float invscalefactor = 1.0f / scalefactor;
	
	    VertexTransformation[CurrentSurface] = glm::scale(scalefactor, scalefactor, scalefactor);
	    NormalTransformation[CurrentSurface] = glm::scale(invscalefactor, invscalefactor, invscalefactor);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, phongsurface.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(phongsurface.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, phongsurface.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(phongsurface.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef DINISURFACE
	++CurrentSurface;
	DiniSurface dinisurface;
	dinisurface.PhiSamples(100);
	dinisurface.ThetaSamples(20);
	NVertices[CurrentSurface] = dinisurface.Vertices().size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    float x_scalefactor     = 6.0f;
	    float inv_x_scalefactor = 1.0f / x_scalefactor;

	    float y_scalefactor     = 6.0f;
	    float inv_y_scalefactor = 1.0f / y_scalefactor;

	    float z_scalefactor     = 2.5f;
	    float inv_z_scalefactor = 1.0f / z_scalefactor;
	
	    VertexTransformation[CurrentSurface] = glm::scale(x_scalefactor, y_scalefactor, z_scalefactor);
	    NormalTransformation[CurrentSurface] = glm::scale(inv_x_scalefactor, inv_y_scalefactor, inv_z_scalefactor);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, dinisurface.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(dinisurface.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, dinisurface.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(dinisurface.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef KLEINBOTTOM
	++CurrentSurface;
	KleinBottom kleinbottom;
	kleinbottom.FrontFacing(false);
	kleinbottom.Triangulate(20, 20);
	NVertices[CurrentSurface] = kleinbottom.Vertices().size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
            kleinbottom.FrontFacing(false);

	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    VertexTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	    NormalTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, kleinbottom.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(kleinbottom.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, kleinbottom.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(kleinbottom.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef KLEINHANDLE
	++CurrentSurface;
	KleinHandle kleinhandle;
	kleinhandle.FrontFacing(false);
	kleinhandle.Triangulate(20, 20);
	NVertices[CurrentSurface] = kleinhandle.Vertices().size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    VertexTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	    NormalTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, kleinhandle.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(kleinhandle.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, kleinhandle.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(kleinhandle.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef KLEINTOP
	++CurrentSurface;
	KleinTop kleintop;
	kleintop.Triangulate(20, 20);
	NVertices[CurrentSurface] = kleintop.Vertices().size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));	kleinbottom.FrontFacing(false);

	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    VertexTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	    NormalTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, kleintop.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(kleintop.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, kleintop.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(kleintop.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef KLEINMIDDLE
	++CurrentSurface;
	KleinMiddle kleinmiddle;
	kleinmiddle.Triangulate(20, 20);
	NVertices[CurrentSurface] = kleinmiddle.Vertices().size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));	kleinbottom.FrontFacing(false);

	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    VertexTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	    NormalTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, kleinmiddle.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(kleinmiddle.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, kleinmiddle.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(kleinmiddle.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef KLEINBOTTLE3SURFACES
	++CurrentSurface;
	std::vector<glm::vec3> kleinvertices3;
	kleinvertices3.insert(kleinvertices3.end(), kleinbottom.Vertices().begin(), kleinbottom.Vertices().end());
	kleinvertices3.insert(kleinvertices3.end(), kleinhandle.Vertices().begin(), kleinhandle.Vertices().end());
	kleinvertices3.insert(kleinvertices3.end(), kleintop.Vertices().begin(), kleintop.Vertices().end());
	
	std::vector<glm::vec3> kleinnormals3;
	kleinnormals3.insert(kleinnormals3.end(), kleinbottom.Normals().begin(), kleinbottom.Normals().end());
	kleinnormals3.insert(kleinnormals3.end(), kleinhandle.Normals().begin(), kleinhandle.Normals().end());
	kleinnormals3.insert(kleinnormals3.end(), kleintop.Normals().begin(), kleintop.Normals().end());
	
	NVertices[CurrentSurface] = kleinvertices3.size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));	kleinbottom.FrontFacing(false);

	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    VertexTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	    NormalTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, kleinvertices3.size() * 3 * sizeof(float),
			     glm::value_ptr(kleinvertices3[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, kleinnormals3.size() * 3 * sizeof(float),
			     glm::value_ptr(kleinnormals3[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef KLEINBOTTLE
	++CurrentSurface;
	std::vector<glm::vec3> kleinvertices;
	kleinvertices.insert(kleinvertices.end(), kleinvertices3.begin(), kleinvertices3.end());
	kleinvertices.insert(kleinvertices.end(), kleinmiddle.Vertices().begin(), kleinmiddle.Vertices().end());
	
	std::vector<glm::vec3> kleinnormals;
	kleinnormals.insert(kleinnormals.end(), kleinnormals3.begin(), kleinnormals3.end());
	kleinnormals.insert(kleinnormals.end(), kleinmiddle.Normals().begin(), kleinmiddle.Normals().end());
	
	NVertices[CurrentSurface] = kleinvertices.size();

	// Vieving parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 6.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));	kleinbottom.FrontFacing(false);

	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	    float F =   5.0f;
	    float B = -15.0f;
	    glm::vec2 lower_left(-10.0f, -10.0f);
	    glm::vec2 upper_right(10.0f,  10.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    VertexTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	    NormalTransformation[CurrentSurface] = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
	}
	//std::cout << "ModelTransformation = " << std::endl << VertexTransformation[CurrentSurface] << std::endl;
	//std::cout << "NormalTrnsformation = " << std::endl << NormalTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, -300.0f, 300.0f);
	LightColor[CurrentSurface]    = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x,
						TmpEyePosition[CurrentSurface].y,
						TmpEyePosition[CurrentSurface].z);

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, kleinvertices.size() * 3 * sizeof(float),
			     glm::value_ptr(kleinvertices[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, kleinnormals.size() * 3 * sizeof(float),
			     glm::value_ptr(kleinnormals[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);
	
	//printtransformedvertices(phongsurface.Vertices(), CTM[CurrentSurface] * VertexTransformation[CurrentSurface]);
#endif

#ifdef ROCKET
	++CurrentSurface;
	BezierSurface rocket("../../Data/rocket.data");
	rocket.FrontFacing(false);
	rocket.NumberOfSubdivisions(3);
	NVertices[CurrentSurface] = rocket.Vertices().size();

	//std::cout << "BezierPatches:" << std::endl;
	//std::cout << rocket.Vertices();
	//std::cout << std::endl;
	//std::cout << "rocket.Vertices().size() = " << NVertices[CurrentSurface] << std::endl;

	// Viewing parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 5.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	
	    float F =   5.0f;
	    float B = -10.0f;

	    glm::vec2 lower_left(-4.0f, -4.0f);
	    glm::vec2 upper_right(4.0f,  4.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;

	// Model Transformation
	{
	    float rotation_angle = 90.0f * glm::pi<float>() / 180.0f;
	    glm::mat4x4 R(glm::rotateX(rotation_angle));
	    glm::mat4x4 invR(R);
	    
	    float scale_factor = 3.0f;
	    glm::mat4x4 S(glm::scale(scale_factor, scale_factor, scale_factor));
	    
	    float inv_scale_factor = 1.0f / scale_factor;
	    glm::mat4x4 invS(glm::scale(inv_scale_factor, inv_scale_factor, inv_scale_factor));

	    float dz = 2.0f;
	    glm::mat4x4 T(glm::translate(0.0f, 0.0f, dz));
	    glm::mat4x4 invT(glm::translate(0.0f, 0.0f, -dz));
			  
	    VertexTransformation[CurrentSurface] = T * S * R;
	    NormalTransformation[CurrentSurface] = invR * invS * invT;
	}

	//std::cout << "VertexTransformation =" << std::endl;
	//std::cout << VertexTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, 300.0f, 200.0f);
	LightColor[CurrentSurface] = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x, TmpEyePosition[CurrentSurface].y, TmpEyePosition[CurrentSurface].z);
	//std::cout << "EyePosition = " << EyePosition[CurrentSurface] << std::endl;

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, rocket.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(rocket.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, rocket.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(rocket.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//std::cout << "Validate Rocket" << std::endl;
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);

	// printtransformedvertices(rocket.Vertices(), CTM * VertexTransformation);
#endif


#ifdef PAIN
	++CurrentSurface;
	BezierSurface pain("../../Data/pain.data");
	pain.NumberOfSubdivisions(5);
	NVertices[CurrentSurface] = pain.Vertices().size();

	//std::cout << "BezierPatches:" << std::endl;
	//std::cout << pain.Vertices();
	//std::cout << std::endl;
	//std::cout << "pain.Vertices().size() = " << NVertices[CurrentSurface] << std::endl;

	// Viewing parameters
	{
	    glm::vec3 vrp(5.0f, 0.0f, 5.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
	    glm::vec3 vup(0.0f, 0.0f, 1.0f);
	    glm::vec3 prp(0.0f, 0.0f, 50.0f);
	
	    float F =   5.0f;
	    float B = -10.0f;

	    glm::vec2 lower_left(-4.0f, -4.0f);
	    glm::vec2 upper_right(4.0f,  4.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;

	// Model Transformation
	{
	    float z_rotation_angle = 30.0f * glm::pi<float>() / 180.0f;
	    glm::mat4x4 Rz(glm::rotateZ(z_rotation_angle));
	    glm::mat4x4 invRz(Rz);

	    float y_rotation_angle = -40.0f * glm::pi<float>() / 180.0f;
	    glm::mat4x4 Ry(glm::rotateY(y_rotation_angle));
	    glm::mat4x4 invRy(Ry);
	    
	    float dx = -0.5f;
	    float dy = -0.5f;
	    glm::mat4x4 T(glm::translate(dx, dy, 0.0f));
	    glm::mat4x4 invT(glm::translate(-dx, -dy, 0.0f));

	    float scale_factor = 3.5f;
	    glm::mat4x4 S(glm::scale(scale_factor, scale_factor, scale_factor));
	    
	    float inv_scale_factor = 1.0f / scale_factor;
	    glm::mat4x4 invS(glm::scale(inv_scale_factor, inv_scale_factor, inv_scale_factor));
	  
	    VertexTransformation[CurrentSurface] = Ry * Rz * S * T;
	    NormalTransformation[CurrentSurface] = invT * invS * invRz * invRy;
	}

	//std::cout << "VertexTransformation =" << std::endl;
	//std::cout << VertexTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, 300.0f, 200.0f);
	LightColor[CurrentSurface] = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x, TmpEyePosition[CurrentSurface].y, TmpEyePosition[CurrentSurface].z);
	//std::cout << "EyePosition = " << EyePosition[CurrentSurface] << std::endl;

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, pain.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(pain.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, pain.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(pain.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//std::cout << "Validate Pain" << std::endl;
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);

	// printtransformedvertices(pain.Vertices(), CTM * VertexTransformation);
#endif

#ifdef PATCHES
	++CurrentSurface;
	BezierSurface patches("../../Data/patches.data");
	patches.FrontFacing(false);
	patches.NumberOfSubdivisions(4);
	NVertices[CurrentSurface] = patches.Vertices().size();

	//std::cout << "BezierPatches:" << std::endl;
	//std::cout << patches.Vertices();
	//std::cout << std::endl;
	//std::cout << "patches.Vertices().size() = " << NVertices[CurrentSurface] << std::endl;

	// Viewing parameters
	{
	    glm::vec3 vrp(0.0f, 3.0f, 0.0f);
	    glm::vec3 vpn(glm::cos(30.0f * glm::pi<float>() / 180.0f), 0.0f, glm::sin(30.0f * glm::pi<float>() / 180.0f));
	    glm::vec3 vup(0.0f, 1.0f, 0.0f);
	    glm::vec3 prp(0.0f, 0.0f, 10.0f);
	
	    float F =  5.0f;
	    float B = -5.0f;

	    glm::vec2 lower_left(-5.0f, -5.0f);
	    glm::vec2 upper_right(5.0f,  5.0f);

	    camera[CurrentSurface] = Camera(vrp, vpn, vup, prp, lower_left, upper_right, F, B);
	}
	
	// Shader parameters
	CTM[CurrentSurface] = camera[CurrentSurface].CurrentTransformationMatrix();
	//std::cout << "CTM = " << std::endl << CTM[CurrentSurface] << std::endl;

	invCTMtranspose[CurrentSurface] = glm::transpose(camera[CurrentSurface].InvCurrentTransformationMatrix());
	//std::cout << "invCTMtranspose = " << std::endl << invCTMtranspose[CurrentSurface] << std::endl;
	{
	    float rotation_angle = 90.0f * glm::pi<float>() / 180.0f;
	    glm::mat4x4 R(glm::rotateY(rotation_angle));
	    glm::mat4x4 invR(R);

	    float scale = 0.45f;
	    glm::mat4x4 S(glm::scale(scale, scale, scale));
	    
	    float invscale = 1.0f / scale;
	    glm::mat4x4 invS(glm::scale(invscale, invscale, invscale));
	    
	    VertexTransformation[CurrentSurface] = S * R;
	    NormalTransformation[CurrentSurface] = invR * invS;
	}
	
	//std::cout << "VertexTransformation =" << std::endl;
	//std::cout << VertexTransformation[CurrentSurface] << std::endl;

	LightPosition[CurrentSurface] = glm::vec3(300.0f, 300.0f, 200.0f);
	LightColor[CurrentSurface] = glm::vec3(1.0f, 1.0f, 1.0f);

	TmpEyePosition[CurrentSurface] = camera[CurrentSurface].InvViewOrientation() * glm::vec4(camera[CurrentSurface].PRP(), 1.0f);
	TmpEyePosition[CurrentSurface] = TmpEyePosition[CurrentSurface] / TmpEyePosition[CurrentSurface].w;
	EyePosition[CurrentSurface] = glm::vec3(TmpEyePosition[CurrentSurface].x, TmpEyePosition[CurrentSurface].y, TmpEyePosition[CurrentSurface].z);
	//std::cout << "EyePosition = " << EyePosition[CurrentSurface] << std::endl;

	// Generate the data
	glBindVertexArray(SurfaceArrayID[CurrentSurface]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[CurrentSurface]);
	
	        // Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, patches.Vertices().size() * 3 * sizeof(float),
			     glm::value_ptr(patches.Vertices()[0]), GL_STATIC_DRAW);

		// Initialize the vertex Attributes
		glEnableVertexAttribArray(vertexattribute);
		glVertexAttribPointer(vertexattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Give our normals to OpenGL.
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[CurrentSurface]);
		glBufferData(GL_ARRAY_BUFFER, patches.Normals().size() * 3 * sizeof(float),
			     glm::value_ptr(patches.Normals()[0]), GL_STATIC_DRAW);

		// Initialize the normal Attributes
		glEnableVertexAttribArray(normalattribute);
		glVertexAttribPointer(normalattribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Validate the line shader program
		//std::cout << "Validate Patches" << std::endl;
		//ValidateShader(surfaceshaderID);
	glBindVertexArray(0);

	// printtransformedvertices(patches.Vertices(), CTM * VertexTransformation);
#endif


	// Almost ready to visualize the surfaces

	// The main loop
	std::cout << std::endl;
	
	std::cout << "*****************************************************************" << std::endl;
	std::cout << "* Press ESC to finish the program                               *" << std::endl;
	std::cout << "* Press '1',..., '9', 'A',..., 'C' to choose different surfaces *" << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	std::cout << std::endl;

	CurrentSurface = 0;
	while (!glfwWindowShouldClose(Window)) {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef COORDINATE_AXES
	    glUseProgram(lineshaderID);
	        glBindVertexArray(CoordinateAxes[0]);
		glUniformMatrix4fv(linectmuniform, 1, GL_FALSE, glm::value_ptr(CTM[CurrentSurface]));
		    glUniform3fv(linecoloruniform,    1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
		    glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);

		glBindVertexArray(CoordinateAxes[1]);
		glUniformMatrix4fv(linectmuniform, 1, GL_FALSE, glm::value_ptr(CTM[CurrentSurface]));
		    glUniform3fv(linecoloruniform,    1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		    glDrawArrays(GL_LINES, 0, 2);
	        glBindVertexArray(0);
	    
		glBindVertexArray(CoordinateAxes[2]);
	            glUniformMatrix4fv(linectmuniform, 1, GL_FALSE, glm::value_ptr(CTM[CurrentSurface]));
		    glUniform3fv(linecoloruniform,    1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
		    glDrawArrays(GL_LINES, 0, 2);
	        glBindVertexArray(0);
	    glUseProgram(0);
#endif

#ifdef BOX
	    // Use the lineshader
	    glUseProgram(lineshaderID);
	        glBindVertexArray(Box);
		    glUniformMatrix4fv(linectmuniform, 1, GL_FALSE, glm::value_ptr(CTM));
		    glUniform3fv(linecoloruniform,    1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
		    glDrawArrays(GL_LINES, 0, BoxCorners.size());
		glBindVertexArray(0);
	    glUseProgram(0);
#endif
	    
	    // Use the surfaceshader
	    glUseProgram(surfaceshaderID);
                // Bind the underlying VertexArrayObject
	        glBindVertexArray(SurfaceArrayID[CurrentSurface]);

		    // Assign values to the uniforms
		    // Transformations
		    glUniformMatrix4fv(ctm, 1, GL_FALSE, glm::value_ptr(CTM[CurrentSurface]));
		    glUniformMatrix4fv(invctmtranspose, 1, GL_FALSE, glm::value_ptr(invCTMtranspose[CurrentSurface]));
		    glUniformMatrix4fv(vertextransform, 1, GL_FALSE, glm::value_ptr(VertexTransformation[CurrentSurface]));
		    glUniformMatrix4fv(normaltransform, 1, GL_FALSE, glm::value_ptr(NormalTransformation[CurrentSurface]));

		    // Light Colors
		    //glUniform3fv(ambientlightcolor,   1, glm::value_ptr(AmbientLightColor));
		    glUniform3fv(lightposition,         1, glm::value_ptr(LightPosition[CurrentSurface]));
		    glUniform3fv(lightcolor,            1, glm::value_ptr(LightColor[CurrentSurface]));

		    // Eye Position
		    glUniform3fv(eyeposition,           1, glm::value_ptr(EyePosition[CurrentSurface]));

		    // Front material properties
		    glUniform3fv(frontambientcolor,     1, glm::value_ptr(FrontAmbientColor));
		    glUniform3fv(frontdiffusecolor,     1, glm::value_ptr(FrontDiffuseColor));
		    glUniform3fv(frontspecularcolor,    1, glm::value_ptr(FrontSpecularColor));
		    glUniform1f(frontshininess, FrontShininess);

		    // Back material properties
		    glUniform3fv(backambientcolor,      1, glm::value_ptr(BackAmbientColor));
		    glUniform3fv(backdiffusecolor,      1, glm::value_ptr(BackDiffuseColor));
		    glUniform3fv(backspecularcolor,     1, glm::value_ptr(BackSpecularColor));
		    glUniform1f(backshininess, BackShininess);

		    glFrontFace(GL_CCW);
#if 0
		    glCullFace(GL_FRONT);
		    glEnable(GL_CULL_FACE);
#endif
		    
		    // Draw the surfaces
		    glDrawArrays(GL_TRIANGLES, 0, NVertices[CurrentSurface]);
#if 0
		    // Just for the pain surface
		    glCullFace(GL_FRONT);
		    glEnable(GL_CULL_FACE);

		    glDrawArrays(GL_TRIANGLES, 0, NVertices[CurrentSurface]);
#endif
		glBindVertexArray(0);
	    glUseProgram(0);
	    
	    glfwSwapBuffers(Window);

#if 0
	    std::stringstream errormessage;
	    errormessage << "End of loop: " << "assignment-6.cpp" << ": " << __LINE__ << ": ";
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

