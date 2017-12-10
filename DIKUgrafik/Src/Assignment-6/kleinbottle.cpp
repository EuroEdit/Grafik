#include "DIKUgrafik/kleinbottle.h"



KleinBottom::KleinBottom()
           : ParametricSurface(0.0f, 2.0f * glm::pi<float>(), 0.0f, glm::pi<float>(), true)
{}


glm::vec3 KleinBottom::Vertex(float const& u, float const& v)  const
{
    return glm::vec3((2.5f + 1.5f * glm::cos(v)) * glm::cos(u),
		     (2.5f + 1.5f * glm::cos(v)) * glm::sin(u),
		     -2.5f * glm::sin(v));
}


glm::vec3 KleinBottom::Normal(float const& u, float const& v) const
{
    return glm::vec3((-6.25f - 3.75f * glm::cos(v)) * glm::cos(v) * glm::cos(u),
		     (-6.25f - 3.75f * glm::cos(v)) * glm::cos(v) * glm::sin(u),
		     ( 3.75f + 2.25f * glm::cos(v)) * glm::sin(v));
}


KleinHandle::KleinHandle()
           : ParametricSurface(0.0f, 2.0f * glm::pi<float>(), 0.0f, glm::pi<float>(), true)
{}


glm::vec3 KleinHandle::Vertex(float const& u, float const& v)  const
{
    return glm::vec3(2.0f - 2.0f * glm::cos(v) + glm::sin(u),
		     glm::cos(u),
		     3.0f * v);
}


glm::vec3 KleinHandle::Normal(float const& u, float const& v) const
{
    return glm::vec3(-3.0f * glm::sin(u),
		     -3.0f * glm::cos(u),
		     2.0f * glm::sin(u) * glm::sin(v));
}


KleinTop::KleinTop()
        : ParametricSurface(0.0f, 2.0f * glm::pi<float>(), 0.0f, glm::pi<float>(), true)
{}


glm::vec3 KleinTop::Vertex(float const& u, float const& v)  const
{
    return glm::vec3(2.0f + (2.0f + glm::cos(u)) * glm::cos(v),
		     glm::sin(u),
		     3.0f * glm::pi<float>() + (2.0f + glm::cos(u)) * glm::sin(v));
}


glm::vec3 KleinTop::Normal(float const& u, float const& v) const
{
    return glm::vec3((2.0f + glm::cos(u)) * glm::cos(u) * glm::cos(v),
		     (2.0f + glm::cos(u)) * glm::sin(u),
		     (2.0f + glm::cos(u)) * glm::cos(u) * glm::sin(v));
}


KleinMiddle::KleinMiddle()
           : ParametricSurface(0.0f, 2.0f * glm::pi<float>(), 0.0f, glm::pi<float>(), true)
{}


glm::vec3 KleinMiddle::Vertex(float const& u, float const& v)  const
{
    return glm::vec3((2.5f + 1.5f * glm::cos(v)) * glm::cos(u),
		     (2.5f + 1.5f * glm::cos(v)) * glm::sin(u),
		     3.0f * v);
}


glm::vec3 KleinMiddle::Normal(float const& u, float const& v) const
{
    return glm::vec3((7.5f + 4.5f * glm::cos(v)) * glm::cos(u),
		     (7.5f + 4.5f * glm::cos(v)) * glm::sin(u),
		     (3.75f + 2.25f * glm::cos(v)) * glm::sin(v));
}
