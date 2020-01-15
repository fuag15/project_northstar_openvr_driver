#include "math/VectorFactory.hpp"

using namespace northstar::math::types;

Vector3d northstar::math::CVectorFactory::V3DFromArray(const std::array<double, 3>& pdArray) const {
    return Vector3d(pdArray[0], pdArray[1], pdArray[2]);
}

Vector2d northstar::math::CVectorFactory::V2DFromArray(const std::array<double, 2>& pdArray) const {
    return Vector2d(pdArray[0], pdArray[1]);
}

Vector4d northstar::math::CVectorFactory::V4DFromV3DXYZandW(const Vector3d& v3dXYZ, const double& dW) const {
    Vector4d v;
    v << v3dXYZ, dW;
    return v;
}

Vector4d northstar::math::CVectorFactory::V4DFromXYZWArray(const std::array<double, 4>& pdArray) const {
    return Vector4d(pdArray[0], pdArray[1], pdArray[2], pdArray[3]);
}

Vector3d northstar::math::CVectorFactory::V3DXYZFromV4D(const Vector4d& v4dSource) const {
    return Vector3d(v4dSource.x(), v4dSource.y(), v4dSource.z());
}

Ray3d northstar::math::CVectorFactory::R3DFromOriginAndDirectionArray(
    const std::array<double, 3>& pdOrigin, 
    const std::array<double, 3>& pdDirection) const {
    return Ray3d(
        Vector3d(pdOrigin[0], pdOrigin[1], pdOrigin[2]),
        Vector3d(pdDirection[0], pdDirection[1], pdDirection[2]));
}

Ray3d northstar::math::CVectorFactory::R3DFromOriginAndDirectionV3D(const types::Vector3d& pdOrigin, const types::Vector3d& pdDirection) const {
    return Ray3d(pdOrigin, pdDirection);
}
