#pragma once

#include "pipeline/vectorialPrimitives/point.hpp"
#include "pipeline/vectorialPrimitives/line.hpp"
#include "pipeline/vectorialPrimitives/square.hpp"
#include "pipeline/vectorialPrimitives/rectangle.hpp"
#include "pipeline/vectorialPrimitives/triangle.hpp"

enum VectorialPrimitiveType {
    POINT,
    LINE,
    SQUARE,
    RECTANGLE,
    TRIANGLE
};

constexpr std::string vectorialPrimitiveTypeNames[5] = {"Point", "Line", "Square", "Rectangle", "Triangle"};

class VectorialPrimitiveFactory {
    static VectorialPrimitivePtr createPoint() {
        return std::make_shared<Point>();
    }

    static VectorialPrimitivePtr createLine() {
        return std::make_shared<Line>();
    }

    static VectorialPrimitivePtr createSquare() {
        return std::make_shared<Square>();
    }

    static VectorialPrimitivePtr createRectangle() {
        return std::make_shared<Rectangle>();
    }

    static VectorialPrimitivePtr createTriangle() {
        return std::make_shared<Triangle>();
    }

public:
    static VectorialPrimitivePtr createPrimitive(const VectorialPrimitiveType type) {
        switch (type) {
            case POINT:
                return createPoint();
            case LINE:
                return createLine();
            case SQUARE:
                return createSquare();
            case RECTANGLE:
                return createRectangle();
            case TRIANGLE:
                return createTriangle();
            default:
                return nullptr;
        }
    };
};
