#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "base.h"

class Texture2D {
public:
    virtual Vec3 at(Float x, Float y) const = 0;
};

class ConstantTexture2D : public Texture2D {
public:
    Vec3 constant;
    ConstantTexture2D(const Vec3 &x);
    virtual Vec3 at(Float x, Float y) const;
};

class BilinearTexture2D : public Texture2D {
public:
    Array2D<Vec3> texture;
    BilinearTexture2D(Array2D<Vec3> texture);
    virtual Vec3 at(Float x, Float y) const;
};

#endif //RENDER_TEXTURE_H
