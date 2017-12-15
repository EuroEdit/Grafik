#include "DIKUgrafik/camera.h"

/*
 * Default constructor, creates a camera with default values of the defining parameters.
 */
Camera::Camera()
{
    this->vrp = glm::vec3(0.0f, 0.0f, 0.0f);
    this->vpn = glm::vec3(0.0f, 0.0f, 1.0f);
    this->vup = glm::vec3(0.0f, 1.0f, 0.0f);
    this->prp = glm::vec3(8.0f, 6.0f, 84.0f);
    this->lower_left_window  = glm::vec2(-50.0f, -50.0f);
    this->upper_right_window = glm::vec2(50.0f, 50.0f);
    this->front_plane = 60.0f;
    this->back_plane  = 25.0f;
    this->x_viewport  = 0.0f;
    this->y_viewport  = 0.0f;
    this->viewport_width  = 2.0f;
    this->viewport_height = 2.0f;

    this->vieworientationmatrix    = glm::mat4x4(1.0f);
    this->invvieworientationmatrix = glm::mat4x4(1.0f);
    this->ComputeViewOrientation(this->vrp, this->vpn, this->vup);

    this->viewprojectionmatrix     = glm::mat4x4(1.0f);
    this->invviewprojectionmatrix  = glm::mat4x4(1.0f);
    this->ComputeViewProjection(this->prp, this->lower_left_window, this->upper_right_window,
				this->front_plane, this->back_plane);

    this->windowviewportmatrix     = glm::mat4x4(1.0f);
    this->invwindowviewportmatrix  = glm::mat4x4(1.0f);
    this->ComputeWindowViewport(this->x_viewport, this->y_viewport, this->viewport_width, this->viewport_height);

    this->currenttransformationmatrix    = glm::mat4x4(1.0f);
    this->invcurrenttransformationmatrix = glm::mat4x4(1.0f);
}

/* 
 * Parameterized constructor, creates a camere with the parameteres given.
 * \param vrp - a vector which specifies the View Reference Point in world coordinates.
 * \param vpn - a vector which specifies the View Plane Normal in world coordinates.
 * \param vup - a vector which specifies the View Up vector in world coordinates.
 * \param prp - a vector which specifies the Projection Reference Point in eye coordinates.
 * \param lower_left_window - a vector which specifies the lower left corner of the window in eye coordinates.
 * \param upper_right_window - a vector which specifies the upper right corner of the window in eye coordinates.
 * \param front_plane - the z-coordinate of the front clipping plane, relative to vrp, i.e. in eye coordinates.
 * \param back_plane - the z-coordinate of the back clipping plane, relative to vrp, i.e. in eye coordinates.
 * \param x_viewport - the x-coordinate of the viewport on the screen.
 * \param y_viewport - the y-coordinate of the viewport on the screen.
 * \param viewport_width - the width of the viewport on the screen.
 * \param viewport_height - the height of the viewport on the screen.
 */
Camera::Camera(glm::vec3 const& vrp, glm::vec3 const& vpn, glm::vec3 const& vup, glm::vec3 const& prp,
	       glm::vec2 const& lower_left_window, glm::vec2 const& upper_right_window,
	       float front_plane,    float back_plane,
	       float x_viewport,     float y_viewport,
	       float viewport_width, float viewport_height)
{
    this->vrp = vrp;
    this->vpn = vpn;
    this->vup = vup;
    this->prp = prp;

    this->lower_left_window  = lower_left_window;
    this->upper_right_window = upper_right_window;
    this->front_plane = front_plane;
    this->back_plane  = back_plane;

    this->x_viewport = x_viewport;
    this->y_viewport = y_viewport;
    this->viewport_width  = viewport_width;
    this->viewport_height = viewport_height;

    this->vieworientationmatrix    = glm::mat4x4(1.0f);
    this->invvieworientationmatrix = glm::mat4x4(1.0f);
    this->ComputeViewOrientation(this->vrp, this->vpn, this->vup);

    this->viewprojectionmatrix     = glm::mat4x4(1.0f);
    this->invviewprojectionmatrix  = glm::mat4x4(1.0f);
    this->ComputeViewProjection(this->prp, this->lower_left_window, this->upper_right_window, this->front_plane, this->back_plane);

    this->windowviewportmatrix     = glm::mat4x4(1.0f);
    this->invwindowviewportmatrix  = glm::mat4x4(1.0f);
    this->ComputeWindowViewport(this->x_viewport, this->y_viewport, this->viewport_width, this->viewport_height);

    this->currenttransformationmatrix    = glm::mat4x4(1.0f);
    this->invcurrenttransformationmatrix = glm::mat4x4(1.0f);
}
 
/*
 * Copy constructor, creates a copy of its parameter.
 * \param camera - the camera object which shuld be copied.
 */
Camera::Camera(Camera const& camera)
{
    this->vieworientationmatrix    = camera.vieworientationmatrix;
    this->invvieworientationmatrix = camera.invvieworientationmatrix;

    this->viewprojectionmatrix     = camera.viewprojectionmatrix;
    this->invviewprojectionmatrix  = camera.invviewprojectionmatrix;

    this->windowviewportmatrix     = camera.windowviewportmatrix;
    this->invwindowviewportmatrix  = camera.invwindowviewportmatrix;

    this->currenttransformationmatrix    = camera.currenttransformationmatrix;
    this->invcurrenttransformationmatrix = camera.invcurrenttransformationmatrix;

    this->vrp = camera.vrp;
    this->vpn = camera.vpn;
    this->vup = camera.vup;
    this->prp = camera.prp;

    this->lower_left_window  = camera.lower_left_window;
    this->upper_right_window = camera.upper_right_window;
    this->front_plane = camera.front_plane;
    this->back_plane  = camera.back_plane;

    this->x_viewport = camera.x_viewport;
    this->y_viewport = camera.y_viewport;
    this->viewport_width  = camera.viewport_width;
    this->viewport_height = camera.viewport_height;
}

/*
 * Destructor, cleans up the camere and destroys the object.
 */
Camera::~Camera()
{}

/*
 * Assignment operator, assigns its parameter to the current instance of class Camera
 * \param camera - the new value of class Camera.
 */
Camera& Camera::operator=(Camera const& camera)
{
    if (this != &camera) {
	this->vieworientationmatrix    = camera.vieworientationmatrix;
	this->invvieworientationmatrix = camera.invvieworientationmatrix;

	this->viewprojectionmatrix     = camera.viewprojectionmatrix;
	this->invviewprojectionmatrix  = camera.invviewprojectionmatrix;

	this->windowviewportmatrix     = camera.windowviewportmatrix;
	this->invwindowviewportmatrix  = camera.invwindowviewportmatrix;

	this->currenttransformationmatrix    = camera.currenttransformationmatrix;
	this->invcurrenttransformationmatrix = camera.invcurrenttransformationmatrix;

	this->vrp = camera.vrp;
	this->vpn = camera.vpn;
	this->vup = camera.vup;
	this->prp = camera.prp;

	this->lower_left_window  = camera.lower_left_window;
	this->upper_right_window = camera.upper_right_window;
	this->front_plane = camera.front_plane;
	this->back_plane  = camera.back_plane;

	this->x_viewport = camera.x_viewport;
	this->y_viewport = camera.y_viewport;
	this->viewport_width  = camera.viewport_width;
	this->viewport_height = camera.viewport_height;
    }
    return *this;
}

/*
 * \return the current ViewOrientation matrix.
 */
glm::mat4x4 Camera::ViewOrientation() const
{
    return this->vieworientationmatrix;
}

/*
 * \return the inverse of the current ViewOrientation matrix.
 */
glm::mat4x4 Camera::InvViewOrientation() const
{
    return this->invvieworientationmatrix;
}

/*
 * \return the current ViewProjection matrix.
 */
glm::mat4x4 Camera::ViewProjection() const
{
    return this->viewprojectionmatrix;
}

/*
 * \return the inverse of the current ViewProjection matrix.
 */
glm::mat4x4 Camera::InvViewProjection() const
{
    return this->invviewprojectionmatrix;
}

/*
 * \return the current WindowViewport matrix.
 */
glm::mat4x4 Camera::WindowViewport() const
{
    return this->windowviewportmatrix;
}

/*
 * \return the inverse of the current WindowViewport matrix.
 */
glm::mat4x4 Camera::InvWindowViewport() const
{
    return this->invwindowviewportmatrix;
}

/*
 * \return the current transformation matrix = WindowViewport() * ViewProjection() * ViewOrientation().
 */
glm::mat4x4 Camera::CurrentTransformationMatrix()
{
    this->currenttransformationmatrix = this->windowviewportmatrix * this->viewprojectionmatrix * this->vieworientationmatrix;
    return this->currenttransformationmatrix;
}

/*
 * \return the inverse of the current transformation 
 * matrix = InvViewOrientation() * InvViewProjection() * InvWindowViewport().
 */
glm::mat4x4 Camera::InvCurrentTransformationMatrix()
{
    Trace("Camera", "InvCurrentTransformationMatrix()");

    TraceMessage("InvViewOrientation() = " << std::endl << this->invvieworientationmatrix << std::endl;);
    TraceMessage("InvViewProjection() = " << std::endl << this->invviewprojectionmatrix << std::endl;);
    TraceMessage("InvWindowViewport() = " << std::endl << this->invwindowviewportmatrix << std::endl;);

    this->invcurrenttransformationmatrix = this->invvieworientationmatrix * this->invviewprojectionmatrix * this->invwindowviewportmatrix;
    return this->invcurrenttransformationmatrix;
}

/*
 * \return the current value of the View Reference Point.
 */
glm::vec3 const& Camera::VRP() const
{
    return this->vrp;
}

/*
 * Changes the value of the View Reference Point.
 * \param vrp - the new View Reference Point
 */
void Camera::VRP(glm::vec3 const& vrp)
{
    this->vrp = vrp;
    this->ComputeViewOrientation(this->vrp, this->vpn, this->vup);
}

/*
 * \return the current value of the View Plane Normal.
 */
glm::vec3 const& Camera::VPN() const
{
    return this->vpn;
}

/*
 * Changes the value of the View Plane Normal.
 * \param vpn - the new View Plane Normal.
 */
void Camera::VPN(glm::vec3 const& vpn)
{
    this->vpn = vpn;
    this->ComputeViewOrientation(this->vrp, this->vpn, this->vup);
}

/*
 * \return the current value of the View Up vector.
 */
glm::vec3 const& Camera::VUP() const
{
    return this->vup;
}

/*
 * Changes the value of the View Up vector.
 * \param vpn - the new View Up vector.
 */
void Camera::VUP(glm::vec3 const& vup)
{
    this->vup = vup;
    this->ComputeViewOrientation(this->vrp, this->vpn, this->vup);
}

/*
 * \return the current value of the Projection Reference Point.
 */
glm::vec3 const& Camera::PRP() const
{
    return this->prp;
}

/*
 * Changes the value of the Projection Reference Point.
 * \param prp - the new Projection Reference Point.
 */
void Camera::PRP(glm::vec3 const& prp)
{
    this->prp = prp;
    this->ComputeViewProjection(this->prp, this->lower_left_window, this->upper_right_window, this->front_plane, this->back_plane);
}

/*
 * \return the lower left corner of the current camera window.
 */
glm::vec2 const& Camera::WinLowerLeft() const
{
    return this->lower_left_window;
}

/*
 * Changes the lower left corner of the camera window.
 * \param lower_left_window - the new value of the lower left corner of the window.
 */
void Camera::WinLowerLeft(glm::vec2 const& lower_left_window)
{
    this->lower_left_window = lower_left_window;
    this->ComputeViewProjection(this->prp, this->lower_left_window, this->upper_right_window, this->front_plane, this->back_plane);
}

/*
 * \return the upper right corner of the current camera window.
 */
glm::vec2 const& Camera::WinUpperRight() const
{
    return this->upper_right_window;
}

/*
 * Changes the upper right corner of the camera window.
 * \param upper_right_window - the new value of the upper right corner of the window.
 */
void Camera::WinUpperRight(glm::vec2 const& upper_right_window)
{
    this->upper_right_window = upper_right_window;
    this->ComputeViewProjection(this->prp, this->lower_left_window, this->upper_right_window, this->front_plane, this->back_plane);
}

/*
 * \return the value of the z-coordinate of the Back Clipping Plane.
 */
float Camera::FrontClippingPlane() const
{
    return this->front_plane;
}

/*
 * Changes the z-value of the Front Clipping Plane.
 * \param front_plane - the new value of the z-coordinate of the Front Clipping Plane.
 */
void Camera::FrontClippingPlane(float const front_plane)
{
    this->front_plane = front_plane;
    this->ComputeViewProjection(this->prp, this->lower_left_window, this->upper_right_window, this->front_plane, this->back_plane);
}

/*
 * \return the value of the z-coordinate of the Back Clipping Plane.
 */
float Camera::BackClippingPlane() const
{
    return this->back_plane;
}

/*
 * Changes the z-value of the Back Clipping Plane.
 * \param front_plane - the new value of the z-coordinate of the Back Clipping Plane.
 */
void Camera::BackClippingPlane(float const back_plane)
{
    this->back_plane = back_plane;
    this->ComputeViewProjection(this->prp, this->lower_left_window, this->upper_right_window, this->front_plane, this->back_plane);
}

/*
 * \return - the value of the x-coordinate of the viewport on the screen
 */
float Camera::XPosition() const
{
    return this->x_viewport;
}

/*
 * Changes the x-coordinate of the viewport on the screen
 * \param new_x_position - the new x-coordinate of the viewport
 */
void Camera::XPosition(float new_x_position)
{
    this->x_viewport = new_x_position;
}

/*
 * \return - the value of the y-coordinate of the viewport on the screen
 */
float Camera::YPosition() const
{
    return this->y_viewport;
}

/*
 * Changes the y-coordinate of the viewport on the screen
 * \param new_y_position - the new y-coordinate of the viewport
 */
void Camera::YPosition(float new_y_position)
{
    this->y_viewport = new_y_position;
}

/*
 * \return the value of the Viewport width on the screen.
 */
float Camera::ViewportWidth() const
{
    return this->viewport_width;
}

/*
 * Changes the Viewport width
 * \param viewport_width - the new value of the Viewport width on the screen.
 */
void Camera::ViewportWidth(int new_viewport_width)
{
    this->viewport_width = new_viewport_width;
    this->ComputeWindowViewport(this->x_viewport, this->y_viewport, this->viewport_width, this->viewport_height);
}

/*
 * \return the value of the Viewport height on the screen.
 */
float Camera::ViewportHeight() const
{
    return this->viewport_height;
}

/*
 * Changes the Viewport height on the screen.
 * \param viewport_height - the new value of the Viewport height
 */
void Camera::ViewportHeight(int new_viewport_height)
{
    this->viewport_height = new_viewport_height;
    this->ComputeWindowViewport(this->x_viewport, this->y_viewport, this->viewport_width, this->viewport_height);
}

/**
 * Private Functions
 */

/*
 * Compute the View Orientation Matrix
 * \param vrp - the View Reference Point.
 * \param vpn - the View Plane Normal.
 * \param vup - the View Up vector.
 * \return - the computed ViewOrientation matrix.
 */
void Camera::ComputeViewOrientation(glm::vec3& vrp, glm::vec3& vpn, glm::vec3& vup)
{
    Trace("Camera", "ComputeViewOrientation(vec3&, vec3&, vec3&)");



    /////////////////////////////// Step 1 ///////////////////////////////////////
    // Translate to the origin
    glm::mat4x4 T_vrp = glm::translate(-vrp);


    ////////////////////////////// Step 2 ////////////////////////////////////////

    // Construct eye coordinate frame (u, v, n)

    // VPN / || VPN ||
    glm::vec3 n(glm::normalize(vpn));
    // VUP x VPN / || VUP x VPN ||
    glm::vec3 u(glm::normalize(glm::cross(vup, vpn)));
    // Rz x Rx / || Rz x Rx || -> n x u / || n x u ||
    glm::vec3 v(glm::normalize(glm::cross(n, u)));


    // Rotate (u, v, n) into (X, Y, Z) axes
    // We use a  unit vector to fill in our new n, u and v
    glm::mat4x4 Rotation(1.0f);
    Rotation = glm::row(Rotation, 0, glm::vec4(u, 0.0f));
    Rotation = glm::row(Rotation, 1, glm::vec4(v, 0.0f));
    Rotation = glm::row(Rotation, 2, glm::vec4(n, 0.0f));


    // The ViewOrientation matrix and its inverse
    // Rotation * T_vrp -> R * T(-VRP) from expression we are mimicking on slide 183
    this->vieworientationmatrix    = Rotation * T_vrp;
    this->invvieworientationmatrix = glm::translate(vrp) * glm::transpose(Rotation);    

}

/*
 * Compute the View Projection Matrix.
 * \param prp - the Projection Reference Point.
 * \param lower_left_window - the lower left corner of the window.
 * \param upper_right_window - the upper right corner of the window.
 * \param front_clipping_plane - the z-coordinate of the front clipping plane.
 * \param back_clipping_plane - the z-coordinate of the back clipping plane.
 * \return - the computed ViewProjection matrix.
 */
void Camera::ComputeViewProjection(glm::vec3& prp, 
				   glm::vec2& lower_left_window, glm::vec2& upper_right_window,
				   float front_clipping_plane, float back_clipping_plane)
{
    // Trace("Camera", "ComputeViewProjection(vec3&, vec2&, vec2&, float, float)");



    //////////////////////// Step 3 ////////////////////////////

    // Translate top of the view pyramid to the origin
    // Is the T(−PRP) part of expression on 183
    glm::mat4x4 T_prp = glm::translate(-prp);





////////////////////////// Step 4 //////////////////////////////

    // Compute the Center of Window and the Direction of Projection
    glm::vec3 CW((lower_left_window + upper_right_window) / 2.0f, 0.0f);
    glm::vec3 DOP(prp - CW);


    // Compute the shear factors
    float shx = 0.0f;
    float shy = 0.0f;
    if (DOP.z != 0.0f) {
	shx = -DOP.x / DOP.z;
	shy = -DOP.y / DOP.z;
    }

    // Generate the shear matrix SHxy
    // Is Sh_per from expression on 183
    glm::mat4x4 SHxy(glm::shearXY(shx, shy));





//////////////////////////// Step 5 ////////////////////////////////

    // Compute the scale factors and the scaling matrix
    float s_x =  -2.0f * prp.z / ((upper_right_window.x - lower_left_window.x) * (back_clipping_plane - prp.z));
    float s_y =  -2.0f * prp.z / ((upper_right_window.y - lower_left_window.y) * (back_clipping_plane - prp.z));
    float s  = -1.0f / (back_clipping_plane - prp.z);

    // Is the scale matrix S_per from 183 expression
    glm::mat4x4 S(glm::scale(glm::vec3(s_x, s_y, s)));



/////////////////////////////// Step "6" ////////////////////////////
    

    //Calculate Zmax using expression on 180 for use in finding Mperpar
    float Zmax = -(front_clipping_plane - prp.z) / (back_clipping_plane - prp.z);
    glm::mat4x4 Mperpar(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 1.0f / (1.0f + Zmax), -1.0f),
			glm::vec4(0.0f, 0.0f, -Zmax / (1.0f + Zmax), 0.0f));


    // Compute the ViewProjection matrix that is the Mperpar matrix slide 187
    this->viewprojectionmatrix    = Mperpar * S * SHxy * T_prp;

    glm::mat4x4 invS = glm::scale(glm::vec3(1.0f / s_x, 1.0f / s_y, 1.0f / s));
    glm::mat4x4 invMperpar(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
			   glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
			   glm::vec4(0.0f, 0.0f, 0.0f, -(1 + Zmax) / Zmax),
			   glm::vec4(0.0f, 0.0f, -1.0f, -1.0f / Zmax));
    this->invviewprojectionmatrix = glm::translate(prp) * glm::shearXY(-shx, -shy) * invS * invMperpar;

}

/*
 * Compute the Window Viewport Matrix.
 * \param x_viewport - the x-coordinate of the viewport.
 * \param y_viewport - the y-coordinate of the viewport.
 * \param viewport_width - the width of the screen viewport.
 * \param viewport_height - the height of the screen viewport.
 * \return - the computed ViewportViewport matrix.
 */
void Camera::ComputeWindowViewport(float x_viewport, float y_viewport, float viewport_width, float viewport_height)
{
    Trace("Camera", "ComputeWindowViewport(float, float, float, float)");

    // The view volume is the canonical parallel view volume: [-1, 1] x [-1, 1] x [-1, 0]
    // OpenGL does this, so just return the identity matrix.
    this->windowviewportmatrix    = glm::mat4x4(1.0f);
    this->invwindowviewportmatrix = glm::mat4x4(1.0f);

    return;
}