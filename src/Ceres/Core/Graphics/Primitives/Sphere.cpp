#include "Sphere.h"

#include <cmath>
#include <fmt/core.h>

#define MATH_PI 3.141592654

namespace Ceres
{
    Sphere::Sphere(float radius, int segments, int rings)
    {
        VertexCount = segments * (rings - 1) + 2;
        Vertices = new VertexPositionNormal[VertexCount];
        IndexCount = segments * (rings - 1) * 6;
        Indices = new int[IndexCount];

        Vertices[0] = VertexPositionNormal(Vector3(0.0f, 0.0f, -radius), Vector3(0.0f, 0.0f, -1.0f));

        int currentIndex = 1;

        for (int ringIndex = 1; ringIndex < rings; ringIndex++)
        {
            const float theta = MATH_PI * static_cast<float>(ringIndex) / static_cast<float>(rings);

            float z = -std::cos(theta);
            float factor = std::sin(theta);
            for (int segmentIndex = 0 ; segmentIndex < segments; segmentIndex++)
            {
                const float phi = 2.0f * MATH_PI * static_cast<float>(segmentIndex) / static_cast<float>(segments);
                Vector3 point = Vector3(
                    std::cos(phi) * factor,
                    std::sin(phi) * factor,
                    z);
                Vertices[currentIndex++] = VertexPositionNormal(point * radius, point);
            }
        }
        Vertices[VertexCount - 1] = VertexPositionNormal(Vector3(0.0f, 0.0f, radius), Vector3(0.0f, 0.0f, 1.0f));

        generateRingBottom(segments);
        for (int i = 1; i < rings - 1; i++)
        {
            generateRing(segments, i);
        }
        generateRingTop(segments, rings);
    }

    Sphere::~Sphere()
    {
        delete[] Vertices;
        delete[] Indices;
    }

    void Sphere::generateTriangleFace(int a, int b, int c)
    {
        Indices[_currentIndexCount    ] = a;
        Indices[_currentIndexCount + 1] = b;
        Indices[_currentIndexCount + 2] = c;

        _currentIndexCount += 3;
    }

    void Sphere::generateSquareFace(int a, int b, int c, int d)
    {
        /*
            a___b
            |  /|
            c/__d
        */
        Indices[_currentIndexCount    ] = a;
        Indices[_currentIndexCount + 1] = b;
        Indices[_currentIndexCount + 2] = c;
        Indices[_currentIndexCount + 3] = b;
        Indices[_currentIndexCount + 4] = d;
        Indices[_currentIndexCount + 5] = c;
        _currentIndexCount += 6;
    }

    void Sphere::generateRingBottom(int segments)
    {
        for (int j = 0; j < segments; j++)
        {
            generateTriangleFace(  
                0,
                1 + j,
                1 + ((j + 1) % segments));
        }
    }
    void Sphere::generateRing(int segments, int ringNum)
    {
        const int verticesPerRing = segments;
        const int ringStartIndex = 1 + (verticesPerRing * (ringNum - 1));

        for (int i = 0; i < segments; i++)
        {
            const int faceStartIndex = ringStartIndex + i;

            generateSquareFace(
                ringStartIndex + i,
                ringStartIndex + ((i + 1) % verticesPerRing),
                ringStartIndex + verticesPerRing + i,
                ringStartIndex + verticesPerRing + ((i + 1) % verticesPerRing));
        }
    }
    void Sphere::generateRingTop(int segments, int ringCount)
    {
        const int startIndex = segments * (ringCount - 2) + 1;
        for (int j = 0; j < segments; j++)
        {
            generateTriangleFace(  
                startIndex + j,
                startIndex + ((j + 1) % segments),
                startIndex + segments);
        }
    }
}