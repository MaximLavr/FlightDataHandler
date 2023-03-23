#include "physcis.h"

namespace sparky { namespace physics {

    Box::Box()
    {

    }

    Box::~Box()
    {

    }

    void Box::init(b2World *world, const maths::vec2 &position, const maths::vec2 &dimensions)
    {
        //Create a body
        b2BodyDef bodyDef;
        m_Dimensions = dimensions;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(position.x, position.y);
        m_Body = world->CreateBody(&bodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        m_Fixture = m_Body->CreateFixture(&fixtureDef);
    }

} }