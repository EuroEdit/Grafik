#ifndef __DINISURFACE_H__
#define __DINISURFACE_H__

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <vector>

#include "DIKUgrafik/glmutils.h"


/**
 * \class DiniSurface
 * implements the geometry of the surface defned by the Dini function.
 */
class DiniSurface {
public:
    /**
     * Default constructor creates a DiniSurface with some default parameters.
     */
    DiniSurface();

    /**
     * Parameterized constructor creates a DiniSurface with parameters given by the arguments.
     * \param a - coefficient of the Dini surface
     * \param b - coefficient of the Dini surface
     */
    DiniSurface(float a, float b);

    /**
     * Copy constructor creates a copy of its argument
     */
    DiniSurface(DiniSurface const& Src);

    /**
     * Destructor deletes the current instance of class DiniSurface.
     */
    virtual ~DiniSurface();

    /**
     * Assignment operator assigns its argument to the current instance.
     */
    DiniSurface& operator=(DiniSurface const& Src);

    /**
     * The number of subdivisions of the height angle, i.e. the phi-interval.
     * \return - The number of samples of the phi-interval.
     */
    int PhiSamples() const;

    /**
     * Changes the number of subdivisions of the height angle, i.e. the phi-interval.
     * \param newPhiSamples - the new number of samples of the phi-interval.
     * \return - the old number of samples of the phi-interval.
     */
    int PhiSamples(int newPhiSamples);

    /**
     * The number of subdivisions of the azimuth angle, i.e. the theta-interval.
     * \return - The number of samples of the theta-interval.
     */
    int ThetaSamples() const;

    /**
     * Changes the number of subdivisions of the azimuth angle, i.e. the theta-interval.
     * \param newThetaSamples - the new number of samples of the theta-interval.
     * \return - the old number of samples of the theta-interval.
     */
    int ThetaSamples(int newThetaSamples);


    /**
     * Computes a vertex on the Dini reflection surface given parameters phi and theta.
     * \param phi - the height angle of the view vector.
     * \param theta - the azimuth angle of the view vector.
     * \return - the 3D point corresponding to the parameters phi and theta.
     */
    glm::vec3 Vertex(float phi, float theta) const;

    /**
     * Computes the normal to the Dini reflection surface given parameters phi and theta.
     * \param phi - the height angle of the view vector.
     * \param theta - the azimuth angle of the view vector.
     * \return - the 3D normal to the phong surface corresponding to the parameters phi and theta.
     */
    glm::vec3 Normal(float phi, float theta) const;

    /**
     * Computes a number of vertices on the DiniSurface, depending on PhiSamples, and ThetaSamples.
     * \return - a std::vector<glm::vec3> containing the computed the vertices.
     */
    std::vector<glm::vec3> const& Vertices();

    /**
     * Computes a number of normals to the DiniSurface, depending on PhiSamples, and ThetaSamples.
     * \return - a std::vector<glm::vec3> containing the computed the normals.
     */
    std::vector<glm::vec3> const& Normals();

protected:

private:
    /**
     * Initializes the private variables.
     */
    void Initialize();

    /**
     * Computes a point on the Dini surface
     * \param phi - the height angle of the view vector.
     * \param theta - the azimuth angle of the view vector.
     * \return the 3D point on the surface.
     */
    glm::vec3 Dini(float phi, float theta) const;

    /**
     * The partial derivative of the Dini surface with respect to phi.
     * \param phi - the height angle of the view vector.
     * \param theta - the azimuth angle of the view vector.
     * \return - the partial derivative wrt. phi of the Dini surface.
     */
    glm::vec3 dDinidphi(float phi, float theta) const;

    /**
     * The partial derivative of the Dini surface with respect to theta.
     * \param phi - the height angle of the view vector.
     * \param theta - the azimuth angle of the view vector.
     * \return - the partial derivative wrt. theta of the Dini surface.
     */
    glm::vec3 dDinidtheta(float phi, float theta) const;


    // Private Variables

    // The vertices of the phong surface
    bool verticesOK;
    std::vector<glm::vec3> vertices;

    // The normals of the phong surface
    bool normalsOK;
    std::vector<glm::vec3> normals;

    // The coefficients of the Dini surface
    float a;
    float b;

    // The number of samples of the height angle phi
    float phi_start;

    // The stop value of the height angle phi.
    float phi_stop;

    // The number of samples of the height angle, i.e. the phi-angle.
    int N_phi;

    // The spacing between the samples of the height angle, i.e. the phi-interval
    float delta_phi;

    // The start value of the azimuth angle theta.
    float theta_start;

    // The stop value of the azimuth angle theta.
    float theta_stop;

    // The number of samples of the azimuth angle, i.e. the theta-angle.
    int N_theta;

    // The spacing between the samples of the azimuth angle, i.e. the theta-interval
    float delta_theta;
};

#endif
