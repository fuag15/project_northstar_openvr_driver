#pragma once

#include "math/IMatrixFactory.hpp"

namespace northstar {
    namespace math {
        class CMatrixFactory : public IMatrixFactory {
        public:
            virtual types::AffineMatrix4d FromRowMajorArray(const std::array<double, 16>& pdArray) const override final;
            virtual types::Translation3d TranslationFromVector3d(const types::Vector3d& v3dTranslation) const override final;
            virtual types::ProjMatrix4d OpenGLProjectionFromFrustumExtents(
                const double & dLeft,
                const double & dRight,
                const double & dTop,
                const double & dBottom) const override final;

        private:
            static constexpr double s_dZNear = 0.07;
            static constexpr double s_dZFar = 1000.0;
        };
    }
}