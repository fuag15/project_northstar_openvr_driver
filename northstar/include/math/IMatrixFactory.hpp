# pragma once

#include "math/Types.hpp"

namespace northstar {
    namespace math {
        class IMatrixFactory {
        public:
            virtual types::AffineMatrix4d FromRowMajorArray(const std::array<double, 16>& pdArray) const = 0;
            virtual types::Translation3d TranslationFromVector3d(const types::Vector3d& v3dTranslation) const = 0;
            virtual types::AffineMatrix4d FromTraslationAndRotation(const types::Vector3d& v3dTranslation, const types::Quaterniond& qdRotation) const = 0;
            virtual types::ProjMatrix4d OpenGLProjectionFromFrustumExtents(
                const double & dLeft,
                const double & dRight,
                const double & dTop,
                const double & dBottom) const = 0;
        };
    }
}