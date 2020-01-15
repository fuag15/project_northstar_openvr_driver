#pragma once

#include "math/Types.hpp"
#include <optional>

namespace northstar {
    namespace math {
        class IGeometry {
        public:
            virtual std::optional<types::Vector3d> Reflect(const types::Vector3d& v3dDirection, const types::Vector3d& v3dNormal) const = 0;
            virtual std::optional<types::Vector3d> IntersectWithPlane(const types::Ray3d& r3dRay, const types::Plane3d& p3dPlane) const = 0;
            virtual std::optional<types::Vector3d> IntersectWithSphereFromWithin(const types::Ray3d& r3dRay, const types::SSphere& sSphere) const = 0;
        };
    }
}