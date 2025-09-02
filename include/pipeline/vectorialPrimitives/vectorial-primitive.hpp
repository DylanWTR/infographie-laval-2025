#pragma once

#include "pipeline/primitives/primitive.hpp"

class VectorialPrimitive : public Primitive {
protected:
    explicit VectorialPrimitive(Shader &shader, Shader &glowShader) : Primitive(shader, glowShader) {
    }
};
