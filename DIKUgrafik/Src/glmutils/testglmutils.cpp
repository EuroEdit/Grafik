#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <string>
#include <cctype>

#include "DIKUgrafik/glmutils.h"


std::string horizline(60, '=');

glm::mat4x4 ComputeMperpar(glm::vec3 const& vrp, glm::vec3 const& vpn, glm::vec3 const& vup,
			   glm::vec3 const& prp,
			   glm::vec2 const& window_lower_left, glm::vec2 const& window_upper_right,
			   float front_clipping_plane, float back_clipping_plane)
{
    std::cout << "-->ComputeNper()" << std::endl;

    glm::vec3 n(glm::normalize(vpn));
    glm::vec3 u(glm::normalize(glm::cross(vup, vpn)));
    glm::vec3 v(glm::normalize(glm::cross(n, u)));

    std::cout << "Eye coordinate system:" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << "   vrp = " << vrp << std::endl;
    std::cout << "   vup = " << vup << std::endl;
    std::cout << std::endl;
    std::cout << "   u   = " << u << std::endl;
    std::cout << "   v   = " << v << std::endl;
    std::cout << "   n   = " << n << std::endl;
    std::cout << std::endl;
    std::cout << "   prp = " << prp << std::endl;
    std::cout << std::endl;
    std::cout << "   window lower left  = " << window_lower_left  << std::endl;
    std::cout << "   window upper right = " << window_upper_right << std::endl;
    std::cout << std::endl;
    std::cout << "   front clipping plane = " << std::setw(6) << std::setprecision(4)
	      << front_clipping_plane << std::endl;
    std::cout << "   back clipping plane  = " << std::setw(6) << std::setprecision(4)
	      << back_clipping_plane << std::endl;
    std::cout << std::endl;

    // Make the ViewOrientation Matrix
    std::cout << "Make the ViewOrientation Matrix" << std::endl;

    glm::mat4x4 T_vrp = glm::translate(-vrp);
    std::cout << "T(-vrp)" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << T_vrp << std::endl;

    glm::mat4x4 Rotation;
    Rotation = glm::row(Rotation, 0, glm::vec4(u, 0.0f));
    Rotation = glm::row(Rotation, 1, glm::vec4(v, 0.0f));
    Rotation = glm::row(Rotation, 2, glm::vec4(n, 0.0f));

    std::cout << "Rotation(u, v, n)" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << Rotation << std::endl;

    // Here is the ViewOrientation Matrix
    glm::mat4x4 ViewOrientation = Rotation * T_vrp;
    std::cout << "ViewOrientation = Rotation * T_vrp" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << ViewOrientation << std::endl;



    // Make the ViewProjection Matrix
    std::cout << "Make the ViewProjection Matrix" << std::endl;

    // Translate top of the view pyramid to the origin
    glm::mat4x4 T_prp = glm::translate(-prp);
    std::cout << "T(-prp)" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << T_prp << std::endl;

    // Make the shear matrix SHxy
    glm::vec3 CW((window_upper_right + window_lower_left) / 2.0f, 0.0f);
    glm::vec3 DOP(prp - CW);
    float shx = 0.0f;
    float shy = 0.0f;
    if (DOP.z != 0.0f) {
	shx = -DOP.x / DOP.z;
	shy = -DOP.y / DOP.z;
    }
    glm::mat4x4 SHxy(glm::shearXY(shx, shy));
    std::cout << "SHxy(DOP.u / DOP.n, DOP.v /DOP.n)" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << SHxy << std::endl;
    
    // Make the scale matrix S
    float sx = 2.0f * prp.z / (window_upper_right.x - window_lower_left.x);
    float sy = 2.0f * prp.z / (window_upper_right.y - window_lower_left.y);
    float s  = -1.0f / (back_clipping_plane - prp.z);

    glm::mat4x4 S(glm::scale(glm::vec3(sx * s, sy * s, s)));
    std::cout << "Scaling matrix" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << S << std::endl;

    glm::mat4x4 ViewProjection(S * SHxy * T_prp);
    std::cout << "ViewProjection = S * Shxy * T(-prp)" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << ViewProjection << std::endl;

    // Make the matrix Mperpar
    float Zmax = -(front_clipping_plane - prp.z) / (back_clipping_plane - prp.z);
    glm::mat4x4 Mperpar(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, 1.0f / (1 + Zmax), -1.0f),
			glm::vec4(0.0f, 0.0f, -Zmax / (1.0f + Zmax), 0.0f));
    std::cout << "Mperpar" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << Mperpar << std::endl;

    glm::mat4x4 Mtotal(Mperpar * ViewProjection * ViewOrientation);
    std::cout << "Mtotal = Mperpar * ViewProjection * ViewOrientation" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << Mtotal << std::endl;

#if 0
    std::cout << "Test of the extreme points" << std::endl;
    std::cout << horizline << std::endl;
    glm::vec4 p(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
    glm::vec4 pt = Mperpar * p;
    std::cout << "p = [" << p << "] --> [" << pt << "] --> [" << p / p.w << "]" << std::endl;

    glm::vec4 q(glm::vec4(-Zmax, -Zmax, Zmax, 1.0f));
    glm::vec4 qt = Mperpar * q;
    std::cout << "q = [" << q << "] --> [" << qt << "] --> [" << qt / qt.w << "]" << std::endl;
#endif
    std::cout << std::endl;
    std::cout << "<--ComputeNper()" << std::endl;

    std::cout << std::endl;

    // return the final matrix Mperpar * Nper
    return Mtotal;
}


int main() 
{
    std::cout << "Test of glm::translate(glm::vec3(dx, dy, dz)):" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "glm::translate(glm::vec3(10.0f, 20.0f, 30.0f)) = " << std::endl
	      << glm::translate(glm::vec3(10.0f, 20.0f, 30.0f)) << std::endl;

    std::cout << "Test of glm::translate(dx, dy, dz):" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "glm::translate(10.0f, 20.0f, 30.0f) = " << std::endl
	      << glm::translate(10.0f, 20.0f, 30.0f) << std::endl;

    std::cout << "Test of glm::scale(glm::vec3(sx, sy, sz)):" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "glm::scale(glm::vec3(10.0f, 20.0f, 30.0f)) = " << std::endl
	      << glm::scale(glm::vec3(10.0f, 20.0f, 30.0f)) << std::endl;

    std::cout << "Test of glm::scale(sx, sy, sz):" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "glm::scale(10.0f, 20.0f, 30.0f) = " << std::endl
	      << glm::scale(10.0f, 20.0f, 30.0f) << std::endl;

    float theta = 30.0f / 180.0f * glm::pi<float>(); 

    std::cout << "Test of glm::rotateX(theta):" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "glm::rotateX(30) = " << std::endl
	      << glm::rotateX(theta) << std::endl;

    std::cout << "Test of glm::rotateY(theta):" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "glm::rotateY(30) = " << std::endl
	      << glm::rotateY(theta) << std::endl;

    std::cout << "Test of glm::rotateZ(theta):" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "glm::rotateZ(30) = " << std::endl
	      << glm::rotateZ(theta) << std::endl;

    std::cout << "Test of glm::shearXY(glm::vec2(20.0f, 30.0f)):" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "glm::shearXY(glm::vec2(20.0f, 30.0f)) =" << std::endl
	      << glm::shearXY(glm::vec2(20.0f, 30.0f)) << std::endl;

    std::cout << "Test of glm::shearXY(20.0f, 30.0f):" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "glm::shearXY(20.0f, 30.0f) =" << std::endl
	      << glm::shearXY(20.0f, 30.0f) << std::endl;

    std::cout << "Test of glm::shearXZ(glm::vec2(20.0f, 30.0f)):" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "glm::shearXZ(glm::vec2(20.0f, 30.0f)) =" << std::endl
	      << glm::shearXZ(glm::vec2(20.0f, 30.0f)) << std::endl;

    std::cout << "Test of glm::shearXZ(20.0f, 30.0f):" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "glm::shearXZ(20.0f, 30.0f) =" << std::endl
	      << glm::shearXZ(20.0f, 30.0f) << std::endl;

    std::cout << "Test of glm::shearYZ(glm::vec2(20.0f, 30.0f)):" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "glm::shearYZ(glm::vec2(20.0f, 30.0f)) =" << std::endl
	      << glm::shearYZ(glm::vec2(20.0f, 30.0f)) << std::endl;

    std::cout << "Test of glm::shearYZ(20.0f, 30.0f):" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "glm::shearYZ(20.0f, 30.0f) =" << std::endl
	      << glm::shearYZ(20.0f, 30.0f) << std::endl;


#if 0
    glm::vec3 VRP(0.0f, 0.0f, 0.0f);
    glm::vec3 VPN(0.0f, 0.0f, 1.0f);
    glm::vec3 VUP(0.0f, 1.0f, 0.0f);
    glm::vec3 PRP(0.0f, 0.0f, 5.0f);

    glm::vec2 window_lower_left(-4.0f, -3.0f);
    glm::vec2 window_upper_right(4.0f,  3.0f);

    float front_clipping_plane =  -5.0f;
    float back_clipping_plane  = -15.0f;

    int window_width  = 500;
    int window_height = 300;


    glm::mat4x4 Mtotal = ComputeMperpar(VRP, VPN, VUP,
					PRP,
					window_lower_left, window_upper_right,
					front_clipping_plane, back_clipping_plane);

    std::cout << "Mtotal" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << Mtotal << std::endl;

    std::cout << "Test of the extreme points" << std::endl;
    std::cout << horizline << std::endl;

    glm::vec4 Pxback(16.0f, 0.0f, -15.0f, 1.0f);
    glm::vec4 TPxback = Mtotal * Pxback;
    std::cout << "Pxback        = [" << Pxback  << "] --> [" << TPxback << "] --> ["
	      << TPxback / TPxback.w << "]" << std::endl;

    glm::vec4 Pyback(0.0f, 12.0f, -15.0f, 1.0f);
    glm::vec4 TPyback = Mtotal * Pyback;
    std::cout << "Pyback        = [" << Pyback   << "] --> [" << TPyback << "] --> ["
	      << TPyback / TPyback.w << "]" << std::endl;

    glm::vec4 Pbackcorner(16.0f, 12.0f, -15.0f, 1.0f);
    glm::vec4 TPbackcorner = Mtotal * Pbackcorner;
    std::cout << "Pbackcorner   = [" << Pbackcorner  << "] --> [" << TPbackcorner << "] --> ["
	      << TPbackcorner / TPbackcorner.w << "]" << std::endl;

    std::cout << std::endl;

    glm::vec4 Pxfront(8.0f, 0.0f, -5.0f, 1.0f);
    glm::vec4 TPxfront = Mtotal * Pxfront;
    std::cout << "Pxfront       = [" << Pxfront  << "] --> [" << TPxfront << "] --> ["
	      << TPxfront / TPxfront.w << "]" << std::endl;

    glm::vec4 Pyfront(0.0f, 6.0f, -5.0f, 1.0f);
    glm::vec4 TPyfront = Mtotal * Pyfront;
    std::cout << "Pyfront       = [" << Pyfront  << "] --> [" << TPyfront << "] --> [" 
	      << TPyfront / TPyfront.w << "]" << std::endl;

    glm::vec4 Pfrontcorner(8.0f, 6.0f, -5.0f, 1.0f);
    glm::vec4 TPfrontcorner = Mtotal * Pfrontcorner;
    std::cout << "Pfrontcorner  = [" << Pfrontcorner  << "] --> [" << TPfrontcorner << "] --> ["
	      << TPfrontcorner / TPfrontcorner.w << "]" << std::endl;
#endif

    return 0;
}
