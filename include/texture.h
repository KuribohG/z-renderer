#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "base.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

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
    BilinearTexture2D() {}
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
    void read_from_file(const char *filename) {
        Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
        image.convertTo(image, CV_32F);
        texture.init(image.rows, image.cols);
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                Vec3f x = image.at<Vec3f>(i, j) / 255.0;
                texture.set(i, j, Vec3(x.val[2], x.val[1], x.val[0]));
            }
        }
    }
};

#endif //RENDER_TEXTURE_H
