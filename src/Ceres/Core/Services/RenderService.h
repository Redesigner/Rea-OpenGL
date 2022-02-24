#pragma once

#include "../Graphics/GraphicsDevice.h"
#include "IService.h"
#include "../Common/VertexList.h"
#include "../Common/AssetPointer.h"

#include "../Components/Base/ComponentParameters.h"

namespace Ceres
{
    using EffectPtr = std::shared_ptr<Effect>;
    using MeshPtr = std::shared_ptr<Mesh>;

    class RenderService : public IService
    {
        public:
            RenderService(GraphicsDevice& graphicsDevice);
            ~RenderService();

            virtual ComponentRef GenerateComponent(std::string type, const IEntity& parent, ComponentPR& params) override;
            void CreatePacked(ComponentParams& params);

            void RenderComponents();
            
        private:
            GraphicsDevice& _parentDevice;
    };
}