#include "DIKUgrafik/parametricsurface.h"


ParametricSurface::ParametricSurface(float const& umin, float const& umax,
				     float const& vmin, float const& vmax,
				     bool frontfacing)
                 : umin(umin), umax(umax), vmin(vmin), vmax(vmax),
		   Ncurrent(0), Mcurrent(0),
                   frontfacing(frontfacing)
{}


ParametricSurface::ParametricSurface(ParametricSurface const& newparmsurface)
                 : umin(newparmsurface.umin), umax(newparmsurface.umax),
		   vmin(newparmsurface.vmin), vmax(newparmsurface.vmax),
		   Ncurrent(newparmsurface.Ncurrent), Mcurrent(newparmsurface.Mcurrent),
                   frontfacing(newparmsurface.frontfacing)
{}


bool ParametricSurface::FrontFacing() const
{
    return this->frontfacing;
}


void ParametricSurface::FrontFacing(bool frontfacing)
{
    this->frontfacing = frontfacing;
}


float ParametricSurface::Umin() const
{
    return this->umin;
}


float ParametricSurface::Umax() const
{
    return this->umax;
}


float ParametricSurface::Vmin() const
{
    return this->vmin;
}


float ParametricSurface::Vmax() const
{
    return this->vmax;
}


ParametricSurface& ParametricSurface::operator=(ParametricSurface const& newparmsurface)
{
    if (this != &newparmsurface) {
	this->frontfacing = newparmsurface.frontfacing;
	this->umin = newparmsurface.umin;
	this->umax = newparmsurface.umax;
	this->vmin = newparmsurface.vmin;
	this->vmax = newparmsurface.vmax;
	this->Ncurrent = newparmsurface.Ncurrent;
	this->Mcurrent = newparmsurface.Mcurrent;
    }
    return *this;
}


void ParametricSurface::Triangulate(unsigned int N, unsigned int M)
{
    if ((N != this->Ncurrent) && (M != this->Mcurrent)) {
	this->vertices.clear();
	this->normals.clear();
	
	float delta_u = (this->umax - this->umin) / N;
	float delta_v = (this->vmax - this->vmin) / M;

	glm::vec3 V1;
	glm::vec3 N1;

	glm::vec3 V2;
	glm::vec3 N2;

	glm::vec3 V3;
	glm::vec3 N3;

	glm::vec3 V4;
	glm::vec3 N4;

	for (float u = this->umin; u < this->umax; u += delta_u) {
	    for (float v = this->vmin; v < this->vmax; v += delta_v) {
		V1 = this->Vertex(u, v);
		N1 = this->Normal(u, v);

		V2 = this->Vertex(u + delta_u, v);
		N2 = this->Normal(u + delta_u, v);

		V3 = this->Vertex(u + delta_u, v + delta_v);
		N3 = this->Normal(u + delta_u, v + delta_v);

		V4 = this->Vertex(u, v + delta_v);
		N4 = this->Normal(u, v + delta_v);

		if (this->frontfacing) {
		    this->vertices.push_back(V1);
		    this->normals.push_back(N1);

		    this->vertices.push_back(V2);
		    this->normals.push_back(N2);

		    this->vertices.push_back(V3);
		    this->normals.push_back(N3);

		    this->vertices.push_back(V1);
		    this->normals.push_back(N1);

		    this->vertices.push_back(V3);
		    this->normals.push_back(N3);

		    this->vertices.push_back(V4);
		    this->normals.push_back(N4);
		}
		else {
		    this->vertices.push_back(V1);
		    this->normals.push_back(-N1);

		    this->vertices.push_back(V3);
		    this->normals.push_back(-N3);

		    this->vertices.push_back(V2);
		    this->normals.push_back(-N2);

		    this->vertices.push_back(V1);
		    this->normals.push_back(-N1);

		    this->vertices.push_back(V4);
		    this->normals.push_back(-N4);

		    this->vertices.push_back(V3);
		    this->normals.push_back(-N3);
		}
#if 0
		std::cout << "Triangle(" << std::setw(13) << V1 << ", " << std::endl;
		std::cout << "         " << std::setw(13) << V2 << ", " << std::endl;
		std::cout << "         " << std::setw(13) << P3 << ")"  << std::endl;

		std::cout << "Triangle(" << std::setw(13) << V1 << ", " << std::endl;
		std::cout << "         " << std::setw(13) << V3 << ", " << std::endl;
		std::cout << "         " << std::setw(13) << V4 << ")"  << std::endl;
#endif
	    }
	}
    }
}


std::vector<glm::vec3> const& ParametricSurface::Vertices()
{
    return this->vertices;
}


std::vector<glm::vec3> const& ParametricSurface::Normals()
{
    return this->normals;
}
