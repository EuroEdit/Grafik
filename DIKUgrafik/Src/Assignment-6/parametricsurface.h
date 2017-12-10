#ifndef RM_DIKUGRAFIK_PARAMETRICSURFACE_H
#define RM_DIKUGRAFIK_PARAMETRICSURFACE_H

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>

#include "DIKUgrafik/glmutils.h"

class ParametricSurface {
public:
    ParametricSurface(float const& umin, float const& umax,
		      float const& vmin, float const& vmax,
                      bool frontfacing);
    
    ParametricSurface(ParametricSurface const& newparmsurface);

    bool FrontFacing() const;

    void FrontFacing(bool frontfacing);
    
    float Umin() const;
    float Umax() const;
    float Vmin() const;
    float Vmax() const;

    void Triangulate(unsigned int N, unsigned int M);

    ParametricSurface&       operator=(ParametricSurface const& newparmsurface);
    ParametricSurface const& operator=(ParametricSurface const& newparmsurface) const;

    std::vector<glm::vec3> const& Vertices();
    std::vector<glm::vec3> const& Normals();
    
protected:
    virtual glm::vec3 Vertex(float  const& u, float const& v) const = 0;
    virtual glm::vec3 Normal(float const& u, float const& v)  const = 0;
    
private:
    float umin;
    float umax;
    float vmin;
    float vmax;

    unsigned int Ncurrent;
    unsigned int Mcurrent;

    bool frontfacing;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
};

#endif
