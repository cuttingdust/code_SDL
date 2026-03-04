#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
    /// 构造函数：接收必要参数
    Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float aspectRatio,
           float fov   = 45.0f, /// 默认值 45度
           float zNear = 0.1f,  /// 默认近平面
           float zFar  = 100.0f) /// 默认远平面
        : position_(position), front_(front), up_(up), aspectRatio_(aspectRatio), fov_(fov), zNear_(zNear), zFar_(zFar)
    {
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(position_, position_ + front_, up_);
    }

    glm::mat4 getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov_), aspectRatio_, zNear_, zFar_);
    }

    void setPosition(const glm::vec3& position)
    {
        position_ = position;
    }
    void setFront(const glm::vec3& front)
    {
        front_ = front;
    }
    void setUp(const glm::vec3& up)
    {
        up_ = up;
    }
    void setAspectRatio(float aspectRatio)
    {
        aspectRatio_ = aspectRatio;
    }
    void setFov(float fov)
    {
        fov_ = fov;
    }

private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    float     aspectRatio_;
    float     fov_;   /// 视野角度（度数）
    float     zNear_; /// 近平面
    float     zFar_;  /// 远平面
};
