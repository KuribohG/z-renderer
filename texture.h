#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "base.h"

template <typename T>
class Texture2D {
public:
    virtual Vec3 at(Float x, Float y) const = 0;
    virtual ~Texture2D() {}
};

template <typename T>
class ConstantTexture2D : public Texture2D<T> {
public:
    T constant;
    ConstantTexture2D(const T &x)
            : constant(x) {}
    virtual T at(Float x, Float y) const {
        return constant;
    }
};

template <typename T>
class BilinearTexture2D : public Texture2D<T> {
public:
    Array2D<T> texture;
    BilinearTexture2D(Array2D<T> texture)
            : texture(texture) {}
    virtual T at(Float x, Float y) const {
        Float px = x * texture.shape[0], py = y * texture.shape[1];
        int x0 = (int)px, y0 = (int)py;
        Float dx = px - x0, dy = py - y0, omdx = 1 - dx, omdy = 1 - dy;
        T res = texture.get(x0, y0) * omdx * omdy +
                texture.get(x0 + 1, y0) * dx * omdy +
                texture.get(x0, y0 + 1) * omdx * dy +
                texture.get(x0 + 1, y0 + 1) * dx * dy;
        return res;
    }
};

#endif //RENDER_TEXTURE_H
