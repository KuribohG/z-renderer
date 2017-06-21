#include "texture.h"

ConstantTexture2D::ConstantTexture2D(const Vec3 &x)
        : constant(x) {}

Vec3 ConstantTexture2D::at(Float x, Float y) const {
    return constant;
}

BilinearTexture2D::BilinearTexture2D(Array2D<Vec3> texture)
        : texture(texture) {}

Vec3 BilinearTexture2D::at(Float x, Float y) const {
    Float px = x * texture.shape[0], py = y * texture.shape[1];
    int x0 = (int)px, y0 = (int)py;
    Float dx = px - x0, dy = py - y0, omdx = 1 - dx, omdy = 1 - dy;
    Vec3 res = texture.get(x0, y0) * omdx * omdy +
               texture.get(x0 + 1, y0) * dx * omdy +
               texture.get(x0, y0 + 1) * omdx * dy +
               texture.get(x0 + 1, y0 + 1) * dx * dy;
    return res;
}