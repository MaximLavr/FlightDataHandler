cmake_minimum_required(VERSION 3.14)

project(FlightDataHandler C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/glew)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/FreeImage)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libpng/1.6.37)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0)

if (!APPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/curl_windows/curl-8.0.1)
endif()

add_executable(FlightDataHandler
        #${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/UI/button.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Map/map_part.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Map/map.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Utils/CoordsConverter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Utils/coords_converter2.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/engine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/sparky.h


        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/timestep.h

        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/maths/maths.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/maths/vec3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/maths/vec2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/maths/vec4.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/maths/mat4.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/maths/rectangle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/maths/aabb.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/fileutils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/timer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/imageLoad.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/stringutils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/colorTransformer.h

        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/shadow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/ray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/particle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/light.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/lightsetup.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/font.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/font_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/buffers/buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/buffers/indexbuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/buffers/vertexarray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/batchrenderer2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/sprite.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/layers/layer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/layers/group.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/texture.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/label.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/label.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/texture_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/renderer2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/graphics/renderable2d.h

        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype_headers.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/freetype-gl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/opengl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/platform.c
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/platform.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-atlas.c
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-atlas.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-font.c
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-font.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/vec234.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/vector.c
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/freetype-gl/vector.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/include/freetype/freetype.h
        )

target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/FreeImage/Source)
target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/include)
target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/curl/8.0.1/include)
if (!APPLE)
    target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/curl_windows/curl-8.0.1/include)
endif()

target_link_libraries(FlightDataHandler glfw)
target_link_libraries(FlightDataHandler libglew_static)
target_link_libraries(FlightDataHandler FreeImage)
target_link_libraries(FlightDataHandler freetype)
if (APPLE)
    target_link_libraries(FlightDataHandler curl)
else()
    target_link_libraries(FlightDataHandler libcurl)
endif()

target_link_libraries(FlightDataHandler ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libpng/1.6.37/lib/libpng16.a)

if (APPLE)
    target_link_libraries(FlightDataHandler "-framework OpenGL")
endif()
