#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

Player::Player(const std::string& modelPath) : model_(modelPath) {}

glm::vec3 Player::getPosition() const { return position_; }
float Player::getRotationY() const { return rotationY_; }
void Player::setPosition(const glm::vec3& pos) { position_ = pos; }

void Player::setRotationY(float degrees) {
    // Berechne den Unterschied zur aktuellen Rotation und wende ihn auf die Kamera an
    float deltaRotation = degrees - rotationY_;
    rotationY_ = degrees;
    //camera_->addAngleAroundPlayer(deltaRotation);  // Kamera mitrotieren lassen
}

void Player::draw(Shader& shader) {
    shader.use();
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position_);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY_), glm::vec3(0, 1, 0));
    shader.setUniform("modelMatrix", modelMatrix);

    model_.Draw(shader);
}