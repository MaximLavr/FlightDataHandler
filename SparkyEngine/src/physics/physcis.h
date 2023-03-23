#pragma once
#include <memory>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include "../maths/vec2.h"

namespace sparky { namespace physics {

    class Box
    {
    private:
        b2Body* m_Body;
        b2Fixture* m_Fixture;
        maths::vec2 m_Dimensions;
    public:
        Box();
        ~Box();

        void init(b2World* world, const maths::vec2& position, const maths::vec2& dimensions);

        b2Body* getBody() const { return m_Body; }
        b2Fixture* getFixture() const { return m_Fixture; }
        const maths::vec2& getDimensions() const { return m_Dimensions; }
    };

} }
