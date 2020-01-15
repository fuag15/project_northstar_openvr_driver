#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <numeric>
#include <functional>

namespace northstar {
    namespace math {
        namespace types {
            typedef Eigen::Transform<double, 3, Eigen::TransformTraits::Affine, Eigen::ColMajor> AffineMatrix4d;
            typedef Eigen::Transform<double, 3, Eigen::TransformTraits::Isometry, Eigen::ColMajor> IsometryMatrix4d;
            typedef Eigen::Transform<double, 3, Eigen::TransformTraits::AffineCompact, Eigen::ColMajor> AffineMatrix4x3d;
            typedef Eigen::Transform<double, 3, Eigen::TransformTraits::Projective, Eigen::ColMajor> ProjMatrix4d;
            typedef Eigen::Translation<double, 3> Translation3d;
            typedef Eigen::Vector<double, 2> Vector2d;
            typedef Eigen::Vector<double, 3> Vector3d;
            typedef Eigen::Vector<double, 4> Vector4d;
            typedef Eigen::ParametrizedLine<double, 3> Ray3d;
            typedef Eigen::Hyperplane<double, 3> Plane3d;
            typedef Eigen::Quaternion<double> Quaterniond;
            typedef Eigen::Vector<int32_t, 2> Vector2i;
            typedef Eigen::AngleAxis<double> AngleAxisd;
            
            struct SPose {
                Vector3d position;
                Quaterniond orientation;
            };

            struct SSphere {
                Vector3d origin;
                double radius;
            };
            
            // NOTE: this hash function gets worse with the number of elements being iterated
            template<typename T>
            struct SHasher {
                std::size_t operator()(const T & IterativeEigen) const {
                    return std::transform_reduce(
                        IterativeEigen.begin(),
                        IterativeEigen.end(),
                        static_cast<double>(0.0),
                        std::bit_xor<std::size_t>(),
                        std::hash<typename T::Scalar>());
                }
            };
        }
    }
}