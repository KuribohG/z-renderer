#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include "base.h"

class Camera {
private:
    Vec3 position;
    Vec3 center;
    Vec3 up;
    Float x_len, y_len;
public:
    Camera(Vec3 position, Vec3 center, Vec3 up, Float x_len, Float y_len);
    Vec3 get_position() const;
    Vec3 get_center() const;
    Vec3 get_up() const;
    void initialize();
    Ray get_ray_through_pixel(int x, int y, int width, int height) const;
};

#endif //RENDER_CAMERA_H
