#pragma once

#include "math/IGeometry.hpp"
#include "math/IVectorFactory.hpp"

#include <memory>

namespace northstar {
    namespace math {
        class CGeometry : public IGeometry {
        public:
            CGeometry(std::shared_ptr<IVectorFactory> pVectorFactory);
            virtual std::optional<types::Vector3d> Reflect(const types::Vector3d& v3dDirection, const types::Vector3d& v3dNormal) const override final;
            virtual std::optional<types::Vector3d> IntersectWithPlane(const types::Ray3d& r3dRay, const types::Plane3d& p3dPlane) const override final;
            virtual std::optional<types::Vector3d> IntersectWithSphereFromWithin(const types::Ray3d& r3dRay, const types::SSphere& sSphere) const override final;
        private:
            std::shared_ptr<IVectorFactory> m_pVectorFactory;
        };
    }
}