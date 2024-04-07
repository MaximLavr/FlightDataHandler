cmake_minimum_required(VERSION 3.17)

project(FlightDataHandler C CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/glew)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/FreeImage)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libpng/1.6.37)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/glm/0.9.9.8/include/glm)

if (!APPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/curl_windows/curl-8.0.1)
endif()

add_executable(FlightDataHandler
        #main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/DataLoader/FlightData.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/DataLoader/TrackLoader.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/GettingGraphics/Graphics.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/StringDrawer/StringDrawer.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Line/Line.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/UI/button.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/UI/SubWindow.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Track/Track.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Map/map.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Map/MapLayer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Map/Tile.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/DataLoader/BinaryLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/DataLoader/MaxZoomLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/DataLoader/TileData.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/DataLoader/TileLoader.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Utils/CoordsConverter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Utils/coords_converter2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Utils/graph_test.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/stb_img/stb_img.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/engine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/sparky.h

        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/Timestep/TimeStep.h


        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/timestep.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/DynamicArray.cpp

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
        ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/src/utils/VectorRemoval.cpp

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

        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Algorithm/PulseDataHandler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Application/Algorithm/PulseDataHandler.h

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

#target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/FreeImage/Source)
#matlab
target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libmatio/1.5.26/include)
#matlab
target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/include)
target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freetype-2.10.0/include)
target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/curl/8.0.1/include)

target_include_directories(FlightDataHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/matplotplusplus/1.2.0/include)
target_include_directories(FlightDataHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/opencv/4.7.0_7/include/opencv4)
target_include_directories(FlightDataHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/glm/0.9.9.8/include)
target_include_directories(FlightDataHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/stb_img)
if (!APPLE)
    target_include_directories(FlightDataHandler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/curl_windows/curl-8.0.1/include)
endif()

target_link_libraries(FlightDataHandler glfw)
target_link_libraries(FlightDataHandler libglew_static)
#target_link_libraries(FlightDataHandler FreeImage)
target_link_libraries(FlightDataHandler freetype)
if (APPLE)
    target_link_libraries(FlightDataHandler curl)
else()
    target_link_libraries(FlightDataHandler libcurl)
endif()

target_link_libraries(FlightDataHandler ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libpng/1.6.37/lib/libpng16.a)
target_link_libraries(FlightDataHandler  ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/matplotplusplus/1.2.0/lib/libmatplot.dylib)

target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimage.3.18.0.dylib)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimage.3.dylib)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimage.a)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimage.dylib)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimageplus.3.18.0.dylib)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimageplus.3.dylib)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimageplus.a)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/freeimage_brew/3.18.0/lib/libfreeimageplus.dylib)

#matlab
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libmatio/1.5.26/lib/libmatio.11.dylib)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libmatio/1.5.26/lib/libmatio.a)
target_link_libraries(FlightDataHandler   ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libmatio/1.5.26/lib/libmatio.dylib)
#matlab

if (APPLE)
    target_link_libraries(FlightDataHandler "-framework OpenGL")
endif()

find_package(opencv REQUIRED)
