#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
    Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(),
              const glm::vec3& scale = glm::vec3(1.f, 1.f, 1.f))
    {
        this->pos_   = pos;
        this->rot_   = rot;
        this->scale_ = scale;
    }
    ~Transform()
    {
    }

public:
    inline auto getPos() const -> glm::vec3
    {
        return this->pos_;
    }

    inline auto getRot() const -> glm::vec3
    {
        return this->rot_;
    }

    inline auto getScale() const -> glm::vec3
    {
        return this->scale_;
    }

    inline auto setPos(const glm::vec3& pos) -> void
    {
        this->pos_ = pos;
    }

    inline auto setRot(const glm::vec3& rot) -> void
    {
        this->rot_ = rot;
    }

    inline auto setScale(const glm::vec3& scale) -> void
    {
        this->scale_ = scale;
    }

    inline auto getModel() const -> glm::mat4
    {
        /// 创建变换矩阵：T * R * S
        glm::mat4 translate = glm::translate(glm::mat4(1.f), this->pos_);
        glm::mat4 rotateX   = glm::rotate(glm::mat4(1.f), this->rot_.x, glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 rotateY   = glm::rotate(glm::mat4(1.f), this->rot_.y, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 rotateZ   = glm::rotate(glm::mat4(1.f), this->rot_.z, glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 scale     = glm::scale(glm::mat4(1.f), this->scale_);

        /// 组合矩阵：先缩放，再旋转，最后平移
        /// 注意矩阵乘法顺序：从右向左应用
        return translate * rotateZ * rotateY * rotateX * scale;
    }

public:
    glm::vec3 pos_;
    glm::vec3 rot_;
    glm::vec3 scale_;
};
