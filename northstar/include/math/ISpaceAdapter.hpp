# pragma once

#include "math/Types.hpp"

namespace northstar {
    namespace math {
        class ISpaceAdapter {
        public:
            virtual types::Vector3d FromUnityUVToUnityClipUnitForwardZ(const types::Vector2d& v2dUnityUV) = 0;
            virtual types::Vector2d FromUnityClipNearPlaneToUnityUV(const types::Vector3d& v3dUnityClip) = 0;
            virtual types::Vector3d ProjectPoint(const types::ProjMatrix4d& m4dProjection, const types::Vector3d& v3dPoint) = 0;
        };
    }
}