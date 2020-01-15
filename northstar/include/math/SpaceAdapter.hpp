#pragma once

#include <memory>

#include "math/ISpaceAdapter.hpp"
#include "math/IVectorFactory.hpp"

namespace northstar {
    namespace  math {
        class CSpaceAdapter : public ISpaceAdapter {
        public:
            CSpaceAdapter(std::shared_ptr<IVectorFactory> pVectorFactory);
            virtual types::Vector3d FromUnityUVToUnityClipUnitForwardZ(const types::Vector2d& v2dUnityUV) override final;
            virtual types::Vector2d FromUnityClipNearPlaneToUnityUV(const types::Vector3d& v3dUnityClip) override final;
            virtual types::Vector3d ProjectPoint(const types::ProjMatrix4d& m4dProjection, const types::Vector3d& v3dPoint) override final;

        private:
            std::shared_ptr<IVectorFactory> m_pVectorFactory;
        };
    }
}