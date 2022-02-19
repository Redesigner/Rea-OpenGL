#include "CubeMap.h"

extern "C"
{
    #include "SDL_image.h"
}
#include "fmt/core.h"

#include <vector>

const std::vector<std::string> CUBE_FACE_NAME = {
    "left.png",
    "right.png",
    "top.png",
    "bottom.png",
    "front.png",
    "back.png"
};
const std::string DEBUG_PREFIX = "[texture]";

namespace Ceres
{
    CubeMap::CubeMap(const char* textureName)
    {
        GLuint texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        for (int i = 0; i < 6; i++)
        {
            SDL_Surface* surface = genSurface(fmt::format("{}{}", textureName, CUBE_FACE_NAME[i]).c_str());
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
            SDL_FreeSurface(surface);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        _textureID = texture;
        fmt::print("{} Loaded cubemap file successfully: '{}'.\n", DEBUG_PREFIX, textureName);
    }

    CubeMap::~CubeMap()
    {
        glDeleteTextures(1, &_textureID);
    }

    void CubeMap::Bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
    }

    SDL_Surface* CubeMap::genSurface(const char* textureName)
    {
        SDL_Surface* surface = IMG_Load(textureName);
        if (surface == nullptr)
        {
            fmt::print("{} Failed to load texture: '{}'\n", DEBUG_PREFIX, IMG_GetError());
            return nullptr;
        }
        // flipSurface(surface);
        return surface;
    }

    void CubeMap::flipSurface(SDL_Surface* surface)
    {
        SDL_LockSurface(surface);
        char* swap = new char[surface->pitch];
        char* pixels = static_cast<char*>(surface->pixels);

        for (int i = 0; i < surface->h / 2; i++)
        {
            // get pointers to the two rows to swap
            char* row1 = pixels + i * surface->pitch;
            char* row2 = pixels + (surface->h - i - 1) * surface->pitch;
            
            // swap rows
            memcpy(swap, row1, surface->pitch);
            memcpy(row1, row2, surface->pitch);
            memcpy(row2, swap, surface->pitch);
        }
        delete[] swap;
        SDL_UnlockSurface(surface);
    }
}