// Camera.h
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

// ========== 抽象基类 ==========
class Camera
{
public:
    virtual ~Camera() = default;

    // 纯虚接口
    virtual glm::mat4 getViewProjection() const   = 0;
    virtual glm::mat4 getViewMatrix() const       = 0;
    virtual glm::mat4 getProjectionMatrix() const = 0;

    // 公共接口
    virtual void      setPosition(const glm::vec3& pos) = 0;
    virtual glm::vec3 getPosition() const               = 0;

    // 类型识别（可选，用于需要区分类型的场景）
    enum class Type
    {
        Perspective,
        Orthographic
    };
    virtual Type getType() const = 0;

    // 统一的更新视图方法（子类实现）
    virtual void updateViewMatrix() = 0;
};

// ========== 透视摄像机 ==========
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float aspectRatio,
                      float fov = 45.0f, float nearPlane = 0.1f, float farPlane = 1000.0f) :
        position_(position), front_(front), up_(up), fov_(fov), aspectRatio_(aspectRatio), nearPlane_(nearPlane),
        farPlane_(farPlane)
    {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    // Camera 接口实现
    glm::mat4 getViewProjection() const override
    {
        return projection_ * view_;
    }

    glm::mat4 getViewMatrix() const override
    {
        return view_;
    }

    glm::mat4 getProjectionMatrix() const override
    {
        return projection_;
    }

    void setPosition(const glm::vec3& pos) override
    {
        position_ = pos;
        updateViewMatrix();
    }

    glm::vec3 getPosition() const override
    {
        return position_;
    }

    Type getType() const override
    {
        return Type::Perspective;
    }

    void updateViewMatrix() override
    {
        view_ = glm::lookAt(position_, position_ + front_, up_);
    }

    // 透视摄像机特有方法
    void setLookAt(const glm::vec3& target)
    {
        front_ = glm::normalize(target - position_);
        updateViewMatrix();
    }

    void setFov(float fov)
    {
        fov_ = fov;
        updateProjectionMatrix();
    }

    void setAspectRatio(float aspect)
    {
        aspectRatio_ = aspect;
        updateProjectionMatrix();
    }

    float getFov() const
    {
        return fov_;
    }
    float getAspectRatio() const
    {
        return aspectRatio_;
    }

private:
    void updateProjectionMatrix()
    {
        projection_ = glm::perspective(glm::radians(fov_), aspectRatio_, nearPlane_, farPlane_);
    }

private:
    glm::mat4 projection_;
    glm::mat4 view_;
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;

    float fov_;
    float aspectRatio_;
    float nearPlane_;
    float farPlane_;
};

// ========== 正交摄像机 ==========
class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top, float nearPlane = 0.1f,
                       float farPlane = 1000.0f) :
        left_(left), right_(right), bottom_(bottom), top_(top), nearPlane_(nearPlane), farPlane_(farPlane)
    {
        updateProjectionMatrix();
        position_ = glm::vec3(0.0f, 0.0f, 0.0f);
        front_    = glm::vec3(0.0f, 0.0f, -1.0f);
        up_       = glm::vec3(0.0f, 1.0f, 0.0f);
        updateViewMatrix();
    }

    // 便捷构造函数：通过大小和宽高比创建
    static OrthographicCamera createWithSize(float size,   // 视口高度的一半
                                             float aspect, // 宽高比 (width/height)
                                             float nearPlane = 0.1f, float farPlane = 1000.0f)
    {
        float halfHeight = size;
        float halfWidth  = size * aspect;
        return OrthographicCamera(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
    }

    // Camera 接口实现
    glm::mat4 getViewProjection() const override
    {
        return projection_ * view_;
    }

    glm::mat4 getViewMatrix() const override
    {
        return view_;
    }

    glm::mat4 getProjectionMatrix() const override
    {
        return projection_;
    }

    void setPosition(const glm::vec3& pos) override
    {
        position_ = pos;
        updateViewMatrix();
    }

    glm::vec3 getPosition() const override
    {
        return position_;
    }

    Type getType() const override
    {
        return Type::Orthographic;
    }

    void updateViewMatrix() override
    {
        view_ = glm::lookAt(position_, position_ + front_, up_);
    }

    // 正交摄像机特有方法
    void move(float dx, float dy, float dz)
    {
        position_ += glm::vec3(dx, dy, dz);
        updateViewMatrix();
    }

    void setSize(float size, float aspect)
    {
        float halfHeight = size;
        float halfWidth  = size * aspect;
        left_            = -halfWidth;
        right_           = halfWidth;
        bottom_          = -halfHeight;
        top_             = halfHeight;
        updateProjectionMatrix();
    }

    void setBoundaries(float left, float right, float bottom, float top)
    {
        left_   = left;
        right_  = right;
        bottom_ = bottom;
        top_    = top;
        updateProjectionMatrix();
    }

    float getLeft() const
    {
        return left_;
    }
    float getRight() const
    {
        return right_;
    }
    float getBottom() const
    {
        return bottom_;
    }
    float getTop() const
    {
        return top_;
    }

private:
    void updateProjectionMatrix()
    {
        projection_ = glm::ortho(left_, right_, bottom_, top_, nearPlane_, farPlane_);
    }

private:
    glm::mat4 projection_;
    glm::mat4 view_;
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;

    float left_, right_, bottom_, top_;
    float nearPlane_, farPlane_;
};
