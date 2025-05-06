#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "ModelLoader.h"
#include "Shader.h"
//#include "PlayerCamera.h"


class Player {
private:
    glm::vec3 position_ = glm::vec3(0.0f);
    float rotationY_ = 0.0f;
    ModelLoader model_;
    //PlayerCamera* camera_;  // Zeiger auf die Kamera

public:
    // Konstruktor lädt das Modell
    Player(const std::string& modelPath);

    // Getter
    glm::vec3 getPosition() const;
    float getRotationY() const;

    // Setter
    void setPosition(const glm::vec3& pos);
    void setRotationY(float degrees);

    // Zeichnet das Modell
    void draw(Shader& shader);

    //PlayerCamera* getCamera() const { return camera_; }
};

#endif // PLAYER_H
