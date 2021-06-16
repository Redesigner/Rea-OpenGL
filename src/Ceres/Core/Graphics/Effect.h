#pragma once

#include "../Common/Matrix.h"
#include "../Common/Vector3.h"

#include <string>

extern "C"
{
    #include <SDL2/SDL.h>
    #include <gl/glew.h>
    #include <SDL2/SDL_opengl.h>
}

namespace Ceres
{
    class Effect
    {
        public:
            Effect(const char* vertFile, const char* fragFile);
            ~Effect();

            void Begin();
            void SetMatrix(std::string name, Matrix matrix);
            void SetVector3(std::string name, Vector3 vector);
            void SetViewMatrix(const Matrix& matrix);

        private:
            bool compileShader(GLuint shader, const char* filename, std::string source);
            void printGlShaderError(GLuint shader);

            GLuint _glProgram;
            GLuint _vertexShader;
            GLuint _fragmentShader;

            std::string _vertexShaderSource;
            std::string _fragmentShaderSource;

            Matrix _viewPerspective;
            Matrix _viewPosition;
    };
}