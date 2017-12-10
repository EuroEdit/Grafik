#include "dinisurface.h"

#include "DIKUgrafik/traceinfo.h"

/**
 * \class DiniSurface
 * implements the geometry of the surface defned by the Dini function.
 */

// Public member functions

/*
 * Default constructor creates a DiniSurface with some default parameters.
 */
DiniSurface::DiniSurface()
           : a(1.0f), b(0.2f), N_phi(20), N_theta(10)
	      
{
    this->Initialize();
}

/*
 * Parameterized constructor creates a DiniSurface with parameters given by the arguments.
* \param a - coefficient of the Dini surface
* \param b - coefficient of the Dini surface
 */
DiniSurface::DiniSurface(float a, float b)
           : a(a), b(b), N_phi(20), N_theta(10)
{
    this->Initialize();
}

/*
 * Copy constructor creates a copy of its argument
 */
DiniSurface::DiniSurface(DiniSurface const& Src)
{
    this->verticesOK = Src.verticesOK;
    this->vertices   = Src.vertices;

    this->normalsOK  = Src.normalsOK;
    this->normals    = Src.normals;

    this->a = Src.a;
    this->b = Src.b;
    
    this->phi_start = Src.phi_start;
    this->phi_stop  = Src.phi_stop;
    this->N_phi     = Src.N_phi;
    this->delta_phi = Src.delta_phi;

    this->theta_start = Src.theta_start;
    this->theta_stop  = Src.theta_stop;
    this->N_theta     = Src.N_theta;
    this->delta_theta = Src.delta_theta;
}

/*
 * Destructor deletes the current instance of class DiniSurface.
 */
DiniSurface::~DiniSurface()
{}

/*
 * Assignment operator assigns its argument to the current instance.
 */
DiniSurface& DiniSurface::operator=(DiniSurface const& Src)
{
    if (this != &Src) {
	this->verticesOK = Src.verticesOK;
	this->vertices   = Src.vertices;

	this->normalsOK  = Src.normalsOK;
	this->normals    = Src.normals;

	this->a = Src.a;
	this->b = Src.b;

	this->phi_start = Src.phi_start;
	this->phi_stop  = Src.phi_stop;
	this->N_phi     = Src.N_phi;
	this->delta_phi = Src.delta_phi;

	this->theta_start = Src.theta_start;
	this->theta_stop  = Src.theta_stop;
	this->N_theta     = Src.N_theta;
	this->delta_theta = Src.delta_theta;
    }
    return *this;
}

/*
 * The number of subdivisions of the height angle, i.e. the phi-interval.
 * \return - The number of samples of the phi-interval.
 */
int DiniSurface::PhiSamples() const
{
    return this->N_phi;
}

/*
 * Changes the number of subdivisions of the height angle, i.e. the phi-interval.
 * \param newPhiSamples - the new number of samples of the phi-interval.
 * \return - the old number of samples of the phi-interval.
 */
int DiniSurface::PhiSamples(int newPhiSamples)
{
    int old_N_phi = this->N_phi;
    this->N_phi = newPhiSamples;
    this->delta_phi = (this->phi_stop - this->phi_start) / float(this->N_phi);
    this->verticesOK = false;
    this->normalsOK  = false;
    return old_N_phi;
}

/*
 * The number of subdivisions of the azimuth angle, i.e. the theta-interval.
 * \return - The number of samples of the theta-interval.
 */
int DiniSurface::ThetaSamples() const
{
    return this->N_theta;
}

/*
 * Changes the number of subdivisions of the azimuth angle, i.e. the theta-interval.
 * \param newThetaSamples - the new number of samples of the theta-interval.
 * \return - the old number of samples of the theta-interval.
 */
int DiniSurface::ThetaSamples(int newThetaSamples)
{
    int old_N_theta = this->N_theta;
    this->N_theta = newThetaSamples;
    this->delta_theta = (this->theta_stop - this->theta_start) / float(this->N_theta);
    this->verticesOK = false;
    this->normalsOK  = false;
    return old_N_theta;
}

/*
 * Computes a vertex on the Dini reflection surface given parameters phi and theta.
 * \param phi - the height angle of the view vector.
 * \param theta - the azimuth angle of the view vector.
 * \return - the 3D point corresponding to the parameters phi and theta.
 */
glm::vec3 DiniSurface::Vertex(float phi, float theta) const
{
    glm::vec3 vertex = this->Dini(phi, theta);
    return vertex;
}

/*
 * Computes the normal to the Dini reflection surface given parameters phi and theta.
 * \param phi - the height angle of the view vector.
 * \param theta - the azimuth angle of the view vector.
 * \return - the 3D normal to the phong surface corresponding to the parameters phi and theta.
 */
glm::vec3 DiniSurface::Normal(float phi, float theta) const
{
    glm::vec3 Dphi = this->dDinidphi(phi, theta);
    glm::vec3 Dtheta = this->dDinidtheta(phi, theta);
    //glm::vec3 normal = glm::cross(Dphi, Dtheta);
    glm::vec3 normal = glm::cross(Dtheta, Dphi);

    if (normal != glm::vec3(0.0f)) {
	normal = glm::normalize(normal);
    }
    return normal;
}

/*
 * Computes a number of vertices on the DiniSurface, depending on PhiSamples, and ThetaSamples.
 * \return - a std::vector<glm::vec3> containing the computed the vertices.
 */
std::vector<glm::vec3> const& DiniSurface::Vertices()
{
    if (!this->verticesOK) {
	this->vertices.clear();
	for (float phi = this->phi_start; phi < this->phi_stop; phi += this->delta_phi) {
	    for (float theta = this->theta_start; theta < this->theta_stop; theta += delta_theta) {

		glm::vec3 v_11 = this->Vertex(phi, theta);
		glm::vec3 v_12 = this->Vertex(phi, theta + this->delta_theta);
		glm::vec3 v_21 = this->Vertex(phi + this->delta_phi, theta);
		glm::vec3 v_22 = this->Vertex(phi + this->delta_phi, theta + this->delta_theta);

		// The first triangle
		this->vertices.push_back(v_11);
		this->vertices.push_back(v_12);
		this->vertices.push_back(v_22);
	    
		// The second triangle
		this->vertices.push_back(v_11);
		this->vertices.push_back(v_22);
		this->vertices.push_back(v_21);
	    }
	}
	this->verticesOK = true;
    }
    return this->vertices;
}

/*
 * Computes a number of normals to the DiniSurface, depending on PhiSamples, and ThetaSamples.
 * \return - a std::vector<glm::vec3> containing the computed the normals.
 */
std::vector<glm::vec3> const& DiniSurface::Normals()
{
    if (!this->normalsOK) {
	this->normals.clear();
	for (float phi = this->phi_start; phi < this->phi_stop; phi += this->delta_phi) {
	    for (float theta = this->theta_start; theta < this->theta_stop; theta += delta_theta) {
		glm::vec3 n_11 = this->Normal(phi, theta);
		glm::vec3 n_12 = this->Normal(phi, theta + delta_theta);
		glm::vec3 n_21 = this->Normal(phi + delta_phi, theta);
		glm::vec3 n_22 = this->Normal(phi + delta_phi, theta + delta_theta);

		// The first triangle
		this->normals.push_back(n_11);
		this->normals.push_back(n_12);
		this->normals.push_back(n_22);

		// The second triangle
		this->normals.push_back(n_11);
		this->normals.push_back(n_22);
		this->normals.push_back(n_21);
	    }
	}
	this->normalsOK = true;
    }
    return this->normals;
}

// Protected member functions

// Private member functions

/*
 * Initializes the private variables.
 */
void DiniSurface::Initialize()
{
    // Define the range and steps of the height angle phi
    this->phi_start = 0.0f;
    this->phi_stop = 6.0f * M_PI;
    this->delta_phi   = (this->phi_stop - this->phi_start) / float(this->N_phi);

    // Define the range and steps of the azimuth angle theta
    this->theta_start= 0.01f; // this->theta_start= 0.001f;
    this->theta_stop = 2.0f;
    this->delta_theta = (this->theta_stop - this->theta_start) / float(this->N_theta);

    // Neither the Vertices nor the Normals are computed yet, so they are not OK.
    this->verticesOK = false;
    this->normalsOK  = false;
}

/*
 * The view vector used to visualize the phong surface.
 * \param phi - the height angle of the view vector.
 * \param theta - the azimuth angle of the view vector.
 * \return - the 3D view vector.
 */
glm::vec3 DiniSurface::Dini(float phi, float theta) const
{
    glm::vec3 dini(this->a * glm::cos(phi) * glm::sin(theta),
		   this->a * glm::sin(phi) * glm::sin(theta),
		   this->a * (glm::cos(theta) + glm::log(glm::tan(0.5 * theta))) + this->b * phi);
    return dini;
}

/*
 * The partial derivative of the view vector with respect to phi.
 * \param phi - the height angle of the view vector.
 * \param theta - the azimuth angle of the view vector.
 * \return - the partial derivative wrt. phi of the 3D view vector.
 */
glm::vec3 DiniSurface::dDinidphi(float phi, float theta) const
{
    glm::vec3 Ddini_Dphi(-this->a * glm::sin(phi) * glm::sin(theta),
			  this->a * glm::cos(phi) * glm::sin(theta),
			  this->b);
    return Ddini_Dphi;
}

/*
 * The partial derivative of the view vector with respect to theta.
 * \param phi - the height angle of the view vector.
 * \param theta - the azimuth angle of the view vector.
 * \return - the partial derivative wrt. theta of the 3D view vector.
 */
glm::vec3 DiniSurface::dDinidtheta(float phi, float theta) const
{
    glm::vec3 Ddini_Dtheta(this->a * glm::cos(phi) * glm::cos(theta),
			   this->a * glm::sin(phi) * glm::cos(theta),
			   0.5 / (glm::sin(0.5 * theta) * glm::cos(0.5 * theta)) - glm::sin(theta));
    return Ddini_Dtheta;
}

