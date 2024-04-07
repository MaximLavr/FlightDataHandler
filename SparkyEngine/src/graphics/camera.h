#pragma once

#include "../maths/maths.h"

namespace sparky { namespace graphics {

    class Camera
    {
    protected:
        maths::mat4 m_ProjectionMatrix, m_ViewMatrix;
        maths::vec3 m_Position, m_Rotation, m_FocalPoint;
    public:
        Camera(const maths::mat4& projectionMatrix);

        virtual void focus() { }
        virtual void update() { }

        inline const maths::vec3& getPosition() const { return m_Position; }
        inline void setPosition(const maths::vec3& position) { m_Position = position; }

        inline const maths::vec3& getRotation() const { return m_Rotation; }
        inline void setRotation(const maths::vec3& rotation) { m_Rotation = rotation; }

        inline const maths::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
        inline void setProjectionMatrix(const maths::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }

        inline void translate(const maths::vec3& translation) { m_Position += translation; }
        inline void rotate(const maths::vec3& rotation) { m_Rotation += rotation; }

        inline void translate(float x, float y, float z) { m_Position += maths::vec3(x, y, z); }
        inline void rotate(float x, float y, float z) { m_Rotation += maths::vec3(x, y, z); }

        inline const maths::vec3& getFocalPoint() const { return m_FocalPoint; }

        inline const maths::mat4& getViewMatrix() { return m_ViewMatrix; }

    };

} }