#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <string>
#include <cctype>

#include "DIKUgrafik/glmutils.h"

int main()
{
    glm::mat3x3 translatiommatrix1 = glm::translate(1.0f, 2.0f);
    std::cout << "translate(1.0f, 2.0f):" << std::endl;
    std::cout << translatiommatrix1 << std::endl;

    glm::mat3x3 invtranslatiommatrix1 = glm::invtranslate(1.0f, 2.0f);
    std::cout << "invtranslate(1.0f, 2.0f):" << std::endl;
    std::cout << invtranslatiommatrix1 << std::endl;

    glm::vec2 transvec(2.0f, 1.0f);
    glm::mat3x3 translatiommatrix2 = glm::translate(transvec);
    std::cout << "translate(transvec):" << std::endl;
    std::cout << translatiommatrix2 << std::endl;

    glm::mat3x3 invtranslatiommatrix2 = glm::invtranslate(transvec);
    std::cout << "invtranslate(transvec):" << std::endl;
    std::cout << invtranslatiommatrix2 << std::endl;

    glm::mat3x3 scalematrix1 = glm::scale(3.0f, 4.0f);
    std::cout << "scale(3.0f, 4.0f)" << std::endl;
    std::cout << scalematrix1 << std::endl;

    glm::mat3x3 invscalematrix1 = glm::invscale(3.0f, 4.0f);
    std::cout << "invscale(3.0f, 4.0f)" << std::endl;
    std::cout << invscalematrix1 << std::endl;

    glm::vec2 scalevec(4.0f, 3.0f);
    glm::mat3x3 scalematrix2 = glm::scale(scalevec);
    std::cout << "scale(scalevec)" << std::endl;
    std::cout << scalematrix2 << std::endl;

    glm::mat3x3 invscalematrix2 = glm::invscale(scalevec);
    std::cout << "invscale(scalevec)" << std::endl;
    std::cout << invscalematrix2 << std::endl;

    glm::mat3x3 rotationmatrix = glm::rotate(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "rotate(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << rotationmatrix << std::endl;

    glm::mat3x3 invrotationmatrix = glm::invrotate(45.0f * glm::pi<float>() / 180.0f);
    std::cout << "invrotate(45.0f * glm::pi<float>() / 180.0f)" << std::endl;
    std::cout << invrotationmatrix << std::endl;

    glm::mat3x3 shearXmatrix = glm::shearX(5.0f);
    std::cout << "shearX(5.0f)" << std::endl;
    std::cout << shearXmatrix << std::endl;

    glm::mat3x3 invshearXmatrix = glm::invshearX(5.0f);
    std::cout << "invshearX(5.0f)" << std::endl;
    std::cout << invshearXmatrix << std::endl;
    
    glm::mat3x3 shearYmatrix = glm::shearY(6.0f);
    std::cout << "shearY(6.0f)" << std::endl;
    std::cout << shearYmatrix << std::endl;

    glm::mat3x3 invshearYmatrix = glm::invshearY(6.0f);
    std::cout << "invshearY(6.0f)" << std::endl;
    std::cout << invshearYmatrix << std::endl;

    return 0;
}


