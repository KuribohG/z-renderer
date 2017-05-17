#include "camera.h"

Camera::Camera(Vec3 position, Vec3 center, Vec3 up, Float x_len, Float y_len)
        : position(position), center(center), up(up), x_len(x_len), y_len(y_len) {}

Vec3 Camera::get_position() const {
    return position;
}

Vec3 Camera::get_center() const {
    return center;
}

Vec3 Camera::get_up() const {
    return up;
}

void Camera::initialize() {
    up = up.normalize();
}

Ray Camera::get_ray_through_pixel(int x, int y, int width, int height) const {
    Vec3 direction = (center - position).normalize();
    Vec3 left = cross(up, direction);
    Float dx = x_len / width, dy = y_len / height;
    Vec3 ray_direction = (dx * (x + 0.5 - width / 2)) * left + (dy * (y + 0.5 - height / 2)) * up + direction;
    return Ray(position, ray_direction.normalize());
}
