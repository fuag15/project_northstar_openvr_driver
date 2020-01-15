#pragma once

#include "math/IVectorFactory.hpp"

namespace northstar {
    namespace math {
        class CVectorFactory: public IVectorFactory {
        public:
            virtual types::Vector3d V3DFromArray(const std::array<double, 3>& pdArray) const override final;
            virtual types::Vector2d V2DFromArray(const std::array<double, 2>& pdArray) const override final;
            virtual types::Vector4d V4DFromV3DXYZandW(const types::Vector3d& v3dXYZ, const double& dW) const override final;
            virtual types::Vector4d V4DFromXYZWArray(const std::array<double, 4>& pdArray) const override final;
            virtual types::Vector3d V3DXYZFromV4D(const types::Vector4d& v4dSource) const override final;
            virtual types::Ray3d R3DFromOriginAndDirectionArray(const std::array<double, 3>& pdOrigin, const std::array<double, 3>& pdDirection) const override final;
            virtual types::Ray3d R3DFromOriginAndDirectionV3D(const types::Vector3d& pdOrigin, const types::Vector3d& pdDirection) const override final;
        };
    }
}
