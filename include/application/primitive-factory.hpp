#pragma once

#include "pipeline/primitives/cube.hpp"
#include "pipeline/primitives/plane.hpp"
#include "pipeline/primitives/frustum.hpp"
#include "pipeline/primitives/sphere.hpp"
#include "pipeline/primitives/bezier_surface.hpp"
#include "pipeline/primitives/catmull-rom-curve.hpp"

class PrimitiveFactory {
    static PrimitivePtr createCube() {
        return std::make_shared<Cube>();
    }

    static PrimitivePtr createPlane() {
        return std::make_shared<Plane>();
    }

    static PrimitivePtr createCylinder() {
        return std::make_shared<Frustum>(1.0f, 1.0f);
    }

    static PrimitivePtr createCone() {
        return std::make_shared<Frustum>(1.0f, 0.0f);
    }

    static PrimitivePtr createSphere() {
        return std::make_shared<Sphere>(1.0f);
    }

    static PrimitivePtr createBezierSurface() {
        return std::make_shared<BezierSurface>();
    }

    static PrimitivePtr createCatmullRomCurve() {
        return std::make_shared<CatmullRomCurve>();
    }

public:
    static PrimitivePtr createPrimitive(const PrimitiveType type) {
        switch (type) {
            case CUBE:
                return createCube();
            case PLANE:
                return createPlane();
            case CYLINDER:
                return createCylinder();
            case CONE:
                return createCone();
            case SPHERE:
                return createSphere();
            case BEZIER_SURFACE:
                return createBezierSurface();
            case CATMULL_ROM_CURVE:
                return createCatmullRomCurve();
            default:
                return nullptr;
        }
    };
};
