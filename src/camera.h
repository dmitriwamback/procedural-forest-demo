//
// Created by Dmitri Wamback on 2026-02-01.
//

#ifndef PROCEDURAL_FORESTS_CAMERA_H
#define PROCEDURAL_FORESTS_CAMERA_H

class Camera {
public:
    glm::vec3 position, lookDirection;
    glm::mat4 projection, lookAt;

    float pitch, yaw = 2.0f * glm::pi<float>()/2.0f;
    float speed = 1.0f;

    float lastMouseX, lastMouseY;

    int mouseButton = GLFW_MOUSE_BUTTON_RIGHT;

    static void Initialize();
    void UpdateLookAtMatrix();
    void Update(glm::vec4 movement, float up, float down);

    glm::vec3 CalculateVelocity(glm::vec4 movement, float up, float down);
};

inline Camera camera;

inline void Camera::Initialize() {
    camera = Camera();

    camera.position = glm::vec3(0.0f, 2.0f, 0.0f);
    camera.lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    camera.projection = glm::perspective(glm::radians(60.0f), 3.0f / 2.0f, 0.1f, 1000.0f);
}

inline glm::vec3 Camera::CalculateVelocity(glm::vec4 movement, float up, float down) {

    float forward   = movement.x,
          backward  = movement.y,
          left      = movement.z,
          right     = movement.w;

    glm::vec3 motion = lookDirection;

    glm::vec3 rightMotion = glm::normalize(glm::cross(motion, glm::vec3(0.0f, 1.0f, 0.0f)));

    float forwardMotion = forward + backward, sidewaysMotion = left + right, verticalMotion = up + down;

    glm::vec3 vStraight = motion * forwardMotion * speed;
    glm::vec3 vRight = rightMotion * sidewaysMotion * speed;
    glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f) * verticalMotion * speed;

    return vStraight - vRight + vUp;
}

inline void Camera::Update(glm::vec4 movement, float up, float down) {

    position += CalculateVelocity(movement, up, down);

    lookDirection = glm::normalize(glm::vec3(cos(camera.yaw) * cos(camera.pitch),
                                               sin(camera.pitch),
                                               sin(camera.yaw) * cos(camera.pitch)));

    lookAt = glm::lookAt(camera.position, camera.position + camera.lookDirection, glm::vec3(0.0f, 1.0f, 0.0f));

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect = (float)width / (float)height;

    projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

inline void Camera::UpdateLookAtMatrix() {
    lookAt = glm::lookAt(camera.position, camera.position + camera.lookDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    if (glfwGetMouseButton(window, camera.mouseButton) == GLFW_PRESS) {

        float deltaX = xpos - camera.lastMouseX;
        float deltaY = ypos - camera.lastMouseY;

        camera.pitch -= deltaY * 0.005f;
        camera.yaw += deltaX * 0.005f;

        if (camera.pitch > 1.55f) camera.pitch = 1.55f;
        if (camera.pitch < -1.55f) camera.pitch = -1.55f;

        camera.lookDirection = glm::normalize(glm::vec3(cos(camera.yaw) * cos(camera.pitch),
                                                          sin(camera.pitch),
                                                          sin(camera.yaw) * cos(camera.pitch)));
    }

    camera.lastMouseX = xpos;
    camera.lastMouseY = ypos;
}

#endif //PROCEDURAL_FORESTS_CAMERA_H