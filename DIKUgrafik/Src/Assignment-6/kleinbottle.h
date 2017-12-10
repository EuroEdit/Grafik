#ifndef RM_KLEINBOTTLE_H
#define RM_KLEINBOTTLE_H

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>

#include "DIKUgrafik/glmutils.h"
#include "DIKUgrafik/parametricsurface.h"


class KleinBottom : public ParametricSurface {
public:
    KleinBottom();

protected:
    glm::vec3 Vertex(float const& u, float const& v) const;
    glm::vec3 Normal(float const& u, float const& v)  const;
    
private:

};


class KleinHandle : public ParametricSurface  {
public:
    KleinHandle();

protected:
    glm::vec3 Vertex(float const& u, float const& v) const;
    glm::vec3 Normal(float const& u, float const& v)  const;
    
private:
   
};


class KleinTop : public ParametricSurface  {
public:
    KleinTop();

protected:
    glm::vec3 Vertex(float const& u, float const& v) const;
    glm::vec3 Normal(float const& u, float const& v)  const;
    
private:

};


class KleinMiddle : public ParametricSurface  {
public:
    KleinMiddle();

protected:
    glm::vec3 Vertex(float const& u, float const& v) const;
    glm::vec3 Normal(float const& u, float const& v) const;
    
private:
    
};

#endif
