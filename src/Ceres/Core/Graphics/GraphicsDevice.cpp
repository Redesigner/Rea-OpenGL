#include "GraphicsDevice.h"

#include "Context.h"
#include "Effect.h"
#include "Mesh.h"
#include "../Entities/Base/IEntity.h"

#include <stdexcept>
#include <fmt/core.h>

extern "C" 
{
	#include <SDL2/SDL.h>
    #include <gl/glew.h>
    #include <SDL2/SDL_opengl.h>
}

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

namespace Ceres
{
    GraphicsDevice::GraphicsDevice()
    {
        // TODO: properly initalize cameracomponent?


        _window = createWindow();
        _currentContext = new Context(_window);
        _screenSurface = nullptr;
        _currentEffect = LoadEffect("Shaders\\defaultVertex.GLSL", "Shaders\\defaultFragment.GLSL");

        _loadedMeshes = std::vector<MeshPtr>();
    }

    GraphicsDevice::~GraphicsDevice()
    {
        unloadEffects();
        unloadMeshes();
        delete _currentContext;
        if(_window != nullptr)
        {
            SDL_DestroyWindow(_window);
        }
    }

    void GraphicsDevice::BeginRender()
    {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    void GraphicsDevice::EndRender()
    {
        SDL_GL_SwapWindow(_window);
        printError();
    }

    void GraphicsDevice::Render(RenderComponent* renderComponent) const
    {
        MeshPtr componentMesh = _loadedMeshes[renderComponent->MeshId];
        componentMesh->GetVertexArray().Bind();
        _currentEffect->Begin();
        _currentEffect->SetViewMatrix(_currentCamera->GetMatrix());
        _currentEffect->SetMatrix("model", renderComponent->Transform.GetMatrix());
        glDrawElements(GL_TRIANGLES, componentMesh->Size(), GL_UNSIGNED_INT, NULL);
    }
    

    EffectPtr GraphicsDevice::LoadEffect(const char* vertexShaderName, const char* fragmentShaderName)
    {
        EffectPtr effect = EffectPtr(new Effect(vertexShaderName, fragmentShaderName));
        _loadedEffects.push_back(effect);
        return effect;
    }

    uint8_t GraphicsDevice::LoadMesh(const IVertexType vertexData[], const IVertexLayout& vertexLayout, const int vertexCount, const int indices[], int indexCount)
    {
        MeshPtr mesh = MeshPtr(new Mesh(vertexData, vertexLayout, vertexCount, indices, indexCount, _currentEffect));
        _loadedMeshes.push_back(mesh);
        return (uint8_t) (_loadedMeshes.size() - 1);
    }

    void GraphicsDevice::SetCamera(CameraComponent* camera)
    {
        _currentCamera = camera;
    }

    RenderComponent* GraphicsDevice::CreateRenderComponent(const IEntity& parent, uint8_t meshId) const
    {
       return new RenderComponent(parent, meshId);
    }


    // Private methods

    void GraphicsDevice::printError()
    {
        GLenum err;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            fmt::print("OpenGL error: {}\n", gluErrorString(err));
        }
    }

    void GraphicsDevice::unloadEffects()
    {
        _loadedEffects.clear();
    }

    void GraphicsDevice::unloadMeshes()
    {
        _loadedMeshes.clear();
    }

    SDL_Window* GraphicsDevice::createWindow()
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0) { return false; }
        return SDL_CreateWindow("Ceres",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    }

}