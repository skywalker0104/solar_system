#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Sphere.h"


#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
//spheraRadius: Ban kinh cua hinh cau
//sphereOrigin: Tam cua hinh cau
//sphereSectors: So luong duong tron dong tam theo chieu ngang
//sphereStacks: So luong duong tron dong tam theo chieu doc

Sphere::Sphere(float sphereRadius, glm::vec3 sphereOrigin, int sphereSectors, int sphereStacks) :
    origin(sphereOrigin),
    radius(sphereRadius)
{
    if (sphereSectors < 3)
    {
        return;
    }
    if (sphereStacks < 4)
    {
        return;
    }

    sectorCount = sphereSectors;
    stackCount = sphereStacks; 

    this->makeVertices();
}


void Sphere::makeVertices()
{
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = float(2 * M_PI / sectorCount);
    float stackStep = float(M_PI / stackCount);
    float sectorAngle, stackAngle;
    
    //Tao ra cac diem trong khong gian ba chieu
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = float(M_PI / 2 - i * stackStep);
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle);          // r * sin(u)
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);  // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);  // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    int k1, k2;
    //Ghep cac diem lai thanh cac tam giac, cac tam giac nay se duoc render thanh hinh cau 
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}
//radius: khoang cach cua hanh tinh toi mat troi
//orbitalSpeed: toc do quay quanh mat troi
//selfRotationSpeed: toc do quay quanh truc
//point: toa do tam cua hanh tinh me (Vi du: Trai Dat la hanh tinh me, Mat Trang la hanh tinh con)
void Sphere::addRotation(GLfloat radius, GLfloat orbitalSpeed, GLfloat selfRotationSpeed, glm::vec3 point)
{
    this->orbitalSpeed = orbitalSpeed;
    this->selfRotationSpeed = selfRotationSpeed;

    float x = sin(glfwGetTime() * orbitalSpeed / 5) * radius;
    float z = cos(glfwGetTime() * orbitalSpeed / 5) * radius;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, point);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0.0f, z));
    origin = glm::vec3(x + point.x, 0.0f, z + point.z);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, (GLfloat)glfwGetTime() * glm::radians(selfRotationSpeed), glm::vec3(0.0f, 0.0f, 1.f));
}

void Sphere::generateBuffers()
{
    VBO.createBO(vertices.size());
    IBO.createBO(indices.size());

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    VBO.bindBO(GL_ARRAY_BUFFER);
    VBO.addData(vertices);
    VBO.uploadDataToGPU(GL_DYNAMIC_DRAW);

    IBO.bindBO(GL_ELEMENT_ARRAY_BUFFER);
    IBO.addData(indices);
    IBO.uploadDataToGPU(GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);// Unbind VBO
    glBindVertexArray(0); // Unbind VAO
}

//Cac tam giac duoc tao ra o ham makeVertices() se duoc ghep lai va render thanh hinh cau
void Sphere::draw()
{
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

Sphere::~Sphere()
{
    VBO.deleteBO();
}

