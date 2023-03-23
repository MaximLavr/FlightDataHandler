#pragma once

//#include "entity/component/Component.h"
//#include "entity/component/SpriteComponent.h"
//#include "entity/component/TransformComponent.h"
//#include "entity/component/Physics2DComponent.h"
//#include "entity/Entity.h"
//#include "entity/Scene2D.h"

#include <memory>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/box2d.h>
#include <unordered_map>
#include <vector>

#include "audio/sound_manager.h"
#include "audio/sound.h"

#include "graphics/buffers/buffer.h"
#include "graphics/shadow.h"
#include "graphics/buffers/indexbuffer.h"
#include "graphics/buffers/vertexarray.h"
#include "graphics/layers/layer.h"
#include "graphics/layers/group.h"
#include "graphics/batchrenderer2d.h"
#include "graphics/font.h"
#include "graphics/font_manager.h"
#include "graphics/label.h"
#include "graphics/shader.h"
#include "graphics/renderer2d.h"
#include "graphics/renderable2d.h"
#include "graphics/shader.h"
#include "graphics/simple2drenderer.h"
#include "graphics/sprite.h"
#include "graphics/staticsprite.h"
#include "graphics/texture.h"
#include "graphics/texture_manager.h"
#include "graphics/window.h"
#include "graphics/camera.h"
#include "graphics/light.h"
#include "graphics/lightsetup.h"
#include "graphics/ray.h"
#include "graphics/particle.h"

#include "maths/mat4.h"
#include "maths/maths.h"
#include "maths/maths_func.h"
#include "maths/vec2.h"
#include "maths/vec3.h"
#include "maths/vec4.h"

#include "physics/physcis.h"

#include "utils/colorTransformer.h"
#include "utils/fileutils.h"
#include "utils/imageLoad.h"
#include "utils/stringutils.h"
#include "utils/timer.h"