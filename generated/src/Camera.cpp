#include "Camera.h"

Camera::Camera(const sf::Vector2u &windowSize) : currentY(0) {
    view.setSize(static_cast<double>(windowSize.x), static_cast<double>(windowSize.y));
    view.setCenter(view.getSize() / 2.0f);
}

void Camera::update(const sf::Vector2f &playerPosition) {
    currentY = std::min(playerPosition.y, view.getCenter().y);
    view.setCenter(view.getCenter().x, currentY);
}

const sf::View &Camera::getView() const {
    return view;
}

double Camera::getCurrentY() const {
    return currentY;
}

std::ostream& operator<<(std::ostream& os, const Camera& cam) {
    os << "Camera [Center: (" << cam.view.getCenter().x << ", " << cam.view.getCenter().y << ")]";
    return os;
}
