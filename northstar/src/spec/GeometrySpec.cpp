#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

#include "math/IVectorFactory.hpp"
#include "mock/MockIVectorFactory.hpp"
#include "math/Geometry.hpp"
#include "math/Types.hpp"
#include "utility/Test.hpp"

using trompeloeil::_;
using namespace northstar::test::utils;
using namespace northstar::math::types;
using northstar::math::MockIVectorFactory;
using northstar::math::IVectorFactory;

TEST_SUITE("Geometry") {
    SCENARIO("Should be able to reflect directions given a normal") {
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CGeometry(pMockVectorFactory);

        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        ALLOW_CALL(*pMockVectorFactory, V2DFromArray(_))
            .RETURN(Vector2d(_1[0], _1[1]));

        ALLOW_CALL(*pMockVectorFactory, R3DFromOriginAndDirectionArray(_, _))
            .RETURN(Ray3d(
                { _1[0], _1[1], _1[2] },
                { _2[0], _2[1], _2[2] }));

        WHEN("Given a normal and an incoming ray in the same direction") {
            Vector3d normal(0, 1, 0);
            Vector3d direction(0, 1, 0);

            THEN("The result should be empty") {
                auto result = Subject.Reflect(direction, normal);
                CHECK(result.has_value() == false);
            }
        }

        WHEN("Given a normal that is orthogonal to the ray") {
            Vector3d normal(0, 1, 0);
            Vector3d direction(1, 0, 0);

            THEN("The result should be empty") {
                auto result = Subject.Reflect(direction, normal);
                CHECK(result.has_value() == false);
            }
        }

        GIVEN("a normal and an incoming ray in opposite directions") {
            WHEN("The ray is exactly on the normal") {
                Vector3d normal(0, 1, 0);
                Vector3d direction(0, -1, 0);
                Vector3d expectedDirection(0, 1, 0);

                THEN("The outcoming ray should be correct") {
                    auto result = Subject.Reflect(direction, normal);
                    CHECK(result.has_value() == true);
                    if (result)
                        CompareIndexed(result.value(), expectedDirection, 0, 2);
                }
            }

            WHEN("The ray is at some normal angle") {
                Vector3d normal(0, 1, 0);
                auto direction = Vector3d(0.5, -0.5, 0.5).normalized();
                auto expectedDirection = Vector3d(0.5, 0.5, 0.5).normalized();

                THEN("The outcoming ray should be correct") {
                    auto result = Subject.Reflect(direction, normal);
                    CHECK(result.has_value() == true);
                    if (result)
                        CompareIndexed(result.value(), expectedDirection, 0, 2);
                }
            }
        }
    }

    SCENARIO("Should be able to intersect a ray with a plane") {
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CGeometry(pMockVectorFactory);

        ALLOW_CALL(*pMockVectorFactory, V3DFromArray(_))
            .RETURN(Vector3d(_1[0], _1[1], _1[2]));

        ALLOW_CALL(*pMockVectorFactory, V2DFromArray(_))
            .RETURN(Vector2d(_1[0], _1[1]));

        ALLOW_CALL(*pMockVectorFactory, R3DFromOriginAndDirectionArray(_, _))
            .RETURN(Ray3d(
                { _1[0], _1[1], _1[2] },
                { _2[0], _2[1], _2[2] }));

        WHEN("called with a ray going away from the plane") {
            auto plane = Plane3d({ 0, 1, 0 }, { 0, 0, 0 });
            auto ray = Ray3d({ 1, 1, 1 }, { 0, 1, 0 });

            THEN("it should return nothing") {
                auto result = Subject.IntersectWithPlane(ray, plane);
                CHECK(result.has_value() == false);
            }
        }

        WHEN("called with an orthoganal ray") {
            auto plane = Plane3d({ 0, 1, 0 }, { 0, 0, 0 });
            auto ray = Ray3d({ 1, 1, 1 }, { 1, 0, 0 });

            THEN("it should return nothing") {
                auto result = Subject.IntersectWithPlane(ray, plane);
                CHECK(result.has_value() == false);
            }
        }


        WHEN("called with a normal intersecting ray") {
            auto plane = Plane3d({ 0, 1, 0 }, { 0, 0, 0 });
            auto ray = Ray3d({ 1, 1, 1 }, { 0, -1, 0 });
            auto expectedIntersection = Vector3d({ 1, 0, 1 });

            THEN("it should provide the intersection point") {
                auto result = Subject.IntersectWithPlane(ray, plane);
                CHECK(result.has_value() == true);
                if (result)
                    CompareIndexed(*result, expectedIntersection, 0, 2);
            }
        }
    }

    SCENARIO("Should be able to intersect a ray with a sphere") {
        auto pMockVectorFactory = std::make_shared<MockIVectorFactory>();
        auto Subject = northstar::math::CGeometry(pMockVectorFactory);

        SSphere sphere;
        sphere.origin << 0, 0, 0;
        sphere.radius = 1;
        GIVEN("the ray intersects the sphere") {
            WHEN("The ray is from within the sphere at the center pointing out") {
                auto ray = Ray3d({ 0, 0, 0 }, { 0, 1, 0 });
                Vector3d expectedIntersection(0, 1, 0);
                auto result = Subject.IntersectWithSphereFromWithin(ray, sphere);
                THEN("It should provide the right intersect") {
                    CHECK(result.has_value() == true);
                    if (result)
                        CompareIndexed(*result, expectedIntersection, 0, 2);
                }
            }

            WHEN("The ray is from within the sphere pointing  toward the opposite side") {
                auto ray = Ray3d({ 0, 0.5, 0 }, { 0, -1, 0 });
                Vector3d expectedIntersection(0, -1, 0);
                auto result = Subject.IntersectWithSphereFromWithin(ray, sphere);
                THEN("It should provide the right intersect") {
                    CHECK(result.has_value() == true);
                    if (result)
                        CompareIndexed(*result, expectedIntersection, 0, 2);
                }
            }
            
            WHEN("The ray is outside the sphere") {
                auto ray = Ray3d({ 3, 3, 0 }, { 0, 0, -1 });
                auto result = Subject.IntersectWithSphereFromWithin(ray, sphere);
                THEN("It should return nothing") {
                    CHECK(result.has_value() == false);
                }
            }
        }

        WHEN("the ray does not intersect the sphere") {
            auto ray = Ray3d({ 3, 3, 0 }, { 0, 0, 1 });
            auto result = Subject.IntersectWithSphereFromWithin(ray, sphere);
            THEN("The function should indicate this") {
                CHECK(result.has_value() == false);
            }
        }
    }
}
