#include "math/Geometry.hpp"
#include <cmath>

using northstar::math::types::Vector2d;
using northstar::math::types::Vector3d;
using northstar::math::types::Plane3d;
using northstar::math::types::Ray3d;
using northstar::math::types::SSphere;

northstar::math::CGeometry::CGeometry(std::shared_ptr<IVectorFactory> pVectorFactory) {
    m_pVectorFactory = pVectorFactory;
}

/*
    This can be throught of as projecting the negation of the incoming ray onto the normal and adding an
    offset beta equal to the offset from the inverted incoming ray and the normal.

    let r' = the reflected ray
    let r_i = the incoming ray
    let r = -r_i
    let n = the normal
    let . = dot product
    let b = beta offset

    then these two equations have to hold:
    1) r' = n(r.n) + b
    2) r + b = n(r.n)

    solving for beta they become:
    1') -b = -r' + n(r.n) 
      -> b = r' - n(r.n)
    2') b = n(r.n) - r

    substituting 1' for b in 2' and solving for r' yields:
       r' - n(r.n) = n(r.n) - r
    -> r' = 2n(r.n) - r

    substituting -r_i for r yields:
    r' = r_i - 2n(r.n)

    this function specifically returns nothing if the rays dont reflect as opposed to returning the orig ray
*/
std::optional<Vector3d> northstar::math::CGeometry::Reflect(const Vector3d& v3dDirection, const Vector3d& v3dNormal) const {
    double dDotProd = v3dDirection.normalized().dot(v3dNormal.normalized());
    // if rays face away from eachother / orthogonal
    if (dDotProd >= 0)
        return std::nullopt;
    
    return v3dDirection.normalized() - (2.0 * v3dNormal.normalized() * dDotProd);
}

std::optional<Vector3d> northstar::math::CGeometry::IntersectWithPlane(const Ray3d& r3dRay, const Plane3d& p3dPlane) const {
    // if ray faces away / orthogonal
    if (r3dRay.direction().normalized().dot(p3dPlane.normal().normalized()) >= 0)
        return std::nullopt;

    return r3dRay.intersectionPoint(p3dPlane);
}

/*
    given a ray and a sphere determine the intersection (if any)

    if the the length of the ray to sphere origin the radius
    this ray starts outisde the sphere and therefore should return nothing;

    else we can get the point along the ray that is orthogonal to the normal of the sphere by projecting the rayToSphere vector onto the ray normal.

    Given this we can compute the distance between that point and the sphere origin then use the pythagoras theorem with the radius to get
    the distance from the normal to the edge of the sphere.

    Finally we can get the actual point of intersection using the parameterized form of the line

    TODO: make this less fragile, it does not work if the ray's origin is at the spheres origin nor
    if the ray's direction is directly with or against a normal of the sphere. the analytical solution would be 
    safer
*/
std::optional<Vector3d> northstar::math::CGeometry::IntersectWithSphereFromWithin(const Ray3d& r3dRay, const SSphere& sSphere) const {
    auto v3dRayToSphere = sSphere.origin - r3dRay.origin();
    if (v3dRayToSphere.norm() >= sSphere.radius)
        return std::nullopt;

    auto v3dOrthogonalProjection = r3dRay.projection(sSphere.origin);
    auto v3dSphereOriginToOrtogonalProjection = v3dOrthogonalProjection - sSphere.origin; //this is rayToSphere
    auto lengthAlongRayFromProjectionToSphere = sqrt(pow(sSphere.radius, 2.0) - v3dSphereOriginToOrtogonalProjection.squaredNorm());
    return v3dOrthogonalProjection + (lengthAlongRayFromProjectionToSphere * r3dRay.direction().normalized());
}