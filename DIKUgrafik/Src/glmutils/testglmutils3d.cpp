#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <string>
#include <cctype>

#include "DIKUgrafik/glmutils.h"

int main()
{
    glm::mat4x4 translationmatrix1 = glm::translate(1.0f, 2.0f, 3.0f);
    std::cout << "translate(1.0f, 2.0f, 3.0f):" << std::endl;
    std::cout << translationmatrix1 << std::endl;

    glm::mat4x4 invtranslationmatrix1 = glm::invtranslate(1.0f, 2.0f, 3.0f);
    std::cout << "invtranslate(1.0f, 2.0f, 3.0f):" << std::endl;
    std::cout << invtranslationmatrix1 << std::endl;

    glm::vec3  transvec(3.0f, 2.0f, 1.0f);
    glm::mat4x4 translationmatrix2 = glm::translate(transvec);
    std::cout << "translate(transvec):" << std::endl;
    std::cout << translationmatrix2 << std::endl;

    glm::mat4x4 invtranslationmatrix2 = glm::invtranslate(transvec);
    std::cout << "invtranslate(transvec):" << std::endl;
    std::cout << invtranslationmatrix2 << std::endl;

    glm::mat4x4 scalematrix1 = glm::scale(3.0f, 3.0f, 4.0f);
    std::cout << "scale(2.0f, 3.0f, 4.0f)" << std::endl;
    std::cout << scalematrix1 << std::endl;

    glm::mat4x4 invscalematrix1 = glm::invscale(2.0f, 3.0f, 4.0f);
    std::cout << "invscale(2.0f, 3.0f, 4.0f)" << std::endl;
    std::cout << invscalematrix1 << std::endl;

    glm::vec3 scalevec(4.0f, 3.0f, 2.0f);
    glm::mat4x4 scalematrix2 = glm::scale(scalevec);
    std::cout << "scale(scalevec)" << std::endl;
    std::cout << scalematrix2 << std::endl;

    glm::mat4x4 invscalematrix2 = glm::invscale(scalevec);
    std::cout << "invscale(scalevec)" << std::endl;
    std::cout << invscalematrix2 << std::endl;

    glm::mat4x4 rotationXmatrix = glm::rotateX(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "rotateX(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << rotationXmatrix << std::endl;

    glm::mat4x4 invrotationXmatrix = glm::invrotateX(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "invrotateX(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << invrotationXmatrix << std::endl;

    glm::mat4x4 rotationYmatrix = glm::rotateY(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "rotateY(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << rotationYmatrix << std::endl;

    glm::mat4x4 invrotationYmatrix = glm::invrotateY(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "invrotateY(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << invrotationYmatrix << std::endl;

    glm::mat4x4 rotationZmatrix = glm::rotateZ(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "rotateZ(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << rotationZmatrix << std::endl;

    glm::mat4x4 invrotationZmatrix = glm::invrotateZ(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "invrotateZ(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << invrotationZmatrix << std::endl;

    glm::mat4x4 rotationmatrix = glm::rotate(45.0f * glm::pi<float>() / 180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    std::cout << "rotate(45.0f * glm::pi<float>() / 180.0f, glm::vec3(j1.0f, 1.0f, 1.0f))" << std::endl;
    std::cout << rotationmatrix << std::endl;

    glm::mat4x4 invrotationmatrix = glm::invrotate(45.0f * glm::pi<float>() / 180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    std::cout << "invrotate(45.0f * glm::pi<float>() / 180.0f, glm::vec3(j1.0f, 1.0f, 1.0f))" << std::endl;
    std::cout << invrotationmatrix << std::endl;
    
    glm::mat4x4 shearXYmatrix = glm::shearXY(5.0f, 2.5f);
    std::cout << "shearXY(5.0f, 2.5f)" << std::endl;
    std::cout << shearXYmatrix << std::endl;

    glm::mat4x4 invshearXYmatrix = glm::invshearXY(5.0f, 2.5f);
    std::cout << "invshearXY(5.0f, 2.5f)" << std::endl;
    std::cout << invshearXYmatrix << std::endl;
    
    glm::mat4x4 shearXZmatrix = glm::shearXZ(5.0f, 2.5f);
    std::cout << "shearXZ(5.0f, 2.5f)" << std::endl;
    std::cout << shearXZmatrix << std::endl;

    glm::mat4x4 invshearXZmatrix = glm::invshearXZ(5.0f, 2.5f);
    std::cout << "invshearXZ(5.0f, 2.5f)" << std::endl;
    std::cout << invshearXZmatrix << std::endl;

    glm::mat4x4 shearYZmatrix = glm::shearYZ(5.0f, 2.5f);
    std::cout << "shearYZ(5.0f, 2.5f)" << std::endl;
    std::cout << shearYZmatrix << std::endl;

    glm::mat4x4 invshearYZmatrix = glm::invshearYZ(5.0f, 2.5f);
    std::cout << "invshearYZ(5.0f, 2.5f)" << std::endl;
    std::cout << invshearYZmatrix << std::endl;

    return 0;
}


