#pragma once

#include <memory>
#include <vector>

#include "CubeMap.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Texture.h"
#include "Window.h"

#include "Primitives/MeshPrimitive.h"
#include "VertexTypes/IVertexType.h"
#include "../Common/Matrix.h"
#include "../Components/RenderComponent.h"
#include "../Components/CameraComponent.h"

#include "VertexStream.h"
#include "VertexTypes/VertexPositionLayout.h"

struct SDL_Window;
struct SDL_Surface;

namespace Ceres
{
    class Context;
    class Effect;

    struct Vector3;

    using EffectPtr = std::shared_ptr<Effect>;
    using MeshPtr = std::shared_ptr<Mesh>;

    class GraphicsDevice
    {
        public:

            GraphicsDevice();
            ~GraphicsDevice();

            void BeginRender();
            void EndRender();

            void Render();
            void Render(RenderComponent* renderComponent) const;

            void ReceiveEvent(SDL_WindowEvent& windowEvent);
            void ToggleFullscreen();

            uint8_t LoadEffect(const char* vertexShaderName, const char* fragmentShaderName);
            uint8_t LoadMesh(const IVertexType vertexData[], const IVertexLayout& vertexLayout, const uint vertexCount, const uint indices[], const uint indexCount, uint8_t effectID = 0);
            uint8_t LoadMesh(const MeshPrimitiveBase& meshPrimitive, uint8_t effectID = 0);
            uint8_t LoadTexture(std::string textureName);

            void SetCamera(CameraComponent* camera);

            RenderComponent* CreateRenderComponent(const IEntity& parent, uint8_t meshId) const;
            RenderComponent* CreateRenderComponent(const IEntity& parent, uint8_t meshId, uint8_t texId) const;

            void LoadWireframeData(const IVertexType vertexData[], const uint indices[], const uint vertexCount);
            void ClearWireframe();

        private:

            void printError();

            void unloadEffects();
            void unloadMeshes();
            void unloadTextures();

            void renderWireframe();
            void renderSkybox();

            SDL_Window* createWindow();
            
            Window _window;
            SDL_Surface* _screenSurface;

            Context* _currentContext;
            uint8_t _currentEffect;
            CameraComponent* _currentCamera;

            std::vector<EffectPtr> _loadedEffects;
            std::vector<MeshPtr> _loadedMeshes;
            std::vector<TexturePtr> _loadedTextures;

            // Wireframe temp variables
            uint8_t _wireframeEffect;
            VertexStream* _wireframe;
            VertexPositionLayout* _wireframeLayout;

            Skybox* _skybox;
            CubeMap* _skyboxCubeMap;
            uint8_t _skyboxEffect;
    };
}