#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/Types.hpp"
#include "math/IVectorFactory.hpp"
#include "math/IGeometry.hpp"
#include "math/ISpaceAdapter.hpp"
#include "mock/MockIVectorFactory.hpp"
#include "mock/MockIMatrixFactory.hpp"
#include "mock/MockIGeometry.hpp"
#include "mock/MockIWorldAdapter.hpp"
#include "mock/MockISpaceAdapter.hpp"
#include "mock/MockIVRSettings.hpp"
#include "mock/MockILogger.hpp"
#include "driver/2DCalibratedOptics.hpp"
#include "driver/Settings.hpp"
#include "utility/Test.hpp"

#include <memory>

using trompeloeil::_;
using trompeloeil::re;
using namespace northstar::driver::settings::keys;
using namespace northstar::test::utils;
using namespace northstar::math::types;

TEST_SUITE("2DCalibratedOptics") {
    SCENARIO("Should provide eye projection information") {
        auto pMockVectorFactory = std::make_shared<northstar::math::MockIVectorFactory>();
        auto pMockMatrixFactory = std::make_shared<northstar::math::MockIMatrixFactory>();
        auto pMockWorldAdapter = std::make_shared<northstar::math::MockIWorldAdapter>();
        auto pMockSpaceAdapter = std::make_shared<northstar::math::MockISpaceAdapter>();
        auto pMockGeometry = std::make_shared<northstar::math::MockIGeometry>();
        auto pMockVRSettings = std::make_shared<northstar::openvr::MockIVRSettings>();
        auto pMockLogger = std::make_shared<northstar::utility::MockILogger>();

        ALLOW_CALL(*pMockVRSettings, GetFloat(_, _, _)).RETURN(1.0f);
        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_)).RETURN(Vector3d(_1[0], _1[1], _1[2]));
        ALLOW_CALL(*pMockVectorFactory, V4DFromXYZWArray(_)).RETURN(Vector4d(_1[0], _1[1], _1[2], _1[3]));
        ALLOW_CALL(*pMockWorldAdapter, FromUnityProjectionExtentsLRTBToOpenVRProjectionExtentsLRTB(_))
            .RETURN(Vector4d(_1.x(), _1.y(), _1.w(), _1.z()));

        ALLOW_CALL(*pMockMatrixFactory, FromRowMajorArray(_)).RETURN(AffineMatrix4d::Identity());
        ALLOW_CALL(*pMockMatrixFactory, TranslationFromVector3d(_)).RETURN(Translation3d(_1));
        ALLOW_CALL(*pMockMatrixFactory, OpenGLProjectionFromFrustumExtents(
            ANY(const double&),
            ANY(const double&),
            ANY(const double&),
            ANY(const double&)))
            .RETURN(ProjMatrix4d::Identity());

        auto Subject = northstar::driver::C2DCalibratedOptics(
            pMockVRSettings.get(),
            pMockWorldAdapter,
            pMockSpaceAdapter,
            pMockGeometry,
            pMockMatrixFactory,
            pMockVectorFactory,
            pMockLogger);

        WHEN("Left eye is requested") {
            auto result = Subject.GetEyeProjectionLRTB(vr::Eye_Left);
            Vector4d expected(1.0, 1.0, 1.0, 1.0);
            THEN("Left eye should be given") {
                CompareIndexed(result, expected, 0, 3);
            }
        }

        WHEN("Right eye is requested") {
            auto result = Subject.GetEyeProjectionLRTB(vr::Eye_Left);
            Vector4d expected(1.0, 1.0, 1.0, 1.0);
            THEN("Right eye should be given") {
                CompareIndexed(result, expected, 0, 3);
            }
        }
    }

    //TODO: test this
    SCENARIO("Should provide UV Mappings") {
        WHEN("Asked for a mapping") {
            THEN("It should provide a result") {}
        }
    }
}