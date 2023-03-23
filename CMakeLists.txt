cmake_minimum_required(VERSION 3.14)
project(NavigationSystem C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(SparkyEngine/lib/glfw)
add_subdirectory(SparkyEngine/lib/glew)
add_subdirectory(SparkyEngine/lib/FreeImage)
add_subdirectory(SparkyEngine/lib/libpng/1.6.37)
add_subdirectory(SparkyEngine/lib/freetype-2.10.0)

add_executable(NavigationSystem
                      app.cpp

                      SparkyEngine/src/engine.h
                      SparkyEngine/src/sparky.h


                      SparkyEngine/src/utils/timestep.h

                      SparkyEngine/src/maths/maths.h
                      SparkyEngine/src/maths/vec3.cpp
                      SparkyEngine/src/maths/vec2.cpp
                      SparkyEngine/src/maths/vec4.cpp
                      SparkyEngine/src/maths/mat4.cpp
                      SparkyEngine/src/maths/rectangle.cpp
                      SparkyEngine/src/maths/aabb.cpp

                      SparkyEngine/src/physics/physics.cpp

                      SparkyEngine/src/utils/fileutils.h
                      SparkyEngine/src/utils/timer.h
                      SparkyEngine/src/utils/imageLoad.h
                      SparkyEngine/src/utils/stringutils.h
                      SparkyEngine/src/utils/colorTransformer.h

                      SparkyEngine/src/audio/sound_manager.cpp
                      SparkyEngine/src/audio/sound.cpp

                      SparkyEngine/src/graphics/shadow.cpp
                      SparkyEngine/src/graphics/ray.cpp
                      SparkyEngine/src/graphics/particle.cpp
                      SparkyEngine/src/graphics/window.cpp
                      SparkyEngine/src/graphics/light.cpp
                      SparkyEngine/src/graphics/lightsetup.cpp
                      SparkyEngine/src/graphics/camera.cpp
                      SparkyEngine/src/graphics/shader.cpp
                      SparkyEngine/src/graphics/font.cpp
                      SparkyEngine/src/graphics/font_manager.cpp
                      SparkyEngine/src/graphics/buffers/buffer.cpp
                      SparkyEngine/src/graphics/buffers/indexbuffer.cpp
                      SparkyEngine/src/graphics/buffers/vertexarray.cpp
                      SparkyEngine/src/graphics/batchrenderer2d.cpp
                      SparkyEngine/src/graphics/sprite.cpp
                      SparkyEngine/src/graphics/layers/layer.cpp
                      SparkyEngine/src/graphics/layers/group.cpp
                      SparkyEngine/src/graphics/texture.cpp
                      SparkyEngine/src/graphics/texture.h
                      SparkyEngine/src/graphics/label.cpp
                      SparkyEngine/src/graphics/label.h
                      SparkyEngine/src/graphics/texture_manager.cpp
                      SparkyEngine/src/graphics/texture.cpp
                      SparkyEngine/src/graphics/renderer2d.h
                      SparkyEngine/src/graphics/renderable2d.h

                      SparkyEngine/lib/freetype-2.10.0/freetype_headers.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/common.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/freetype-gl.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/opengl.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/platform.c
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/platform.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-atlas.c
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-atlas.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-font.c
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/texture-font.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/vec234.h
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/vector.c
                      SparkyEngine/lib/freetype-2.10.0/freetype-gl/vector.h
                      SparkyEngine/lib/freetype-2.10.0/include/freetype/freetype.h

                      SparkyEngine/lib/gorilla-audio/include/gorilla/ga_internal.h
                      SparkyEngine/lib/gorilla-audio/src/ga_stream.c
                      SparkyEngine/lib/gorilla-audio/src/ga.c
                      SparkyEngine/lib/gorilla-audio/include/gorilla/ga.h
                      SparkyEngine/lib/gorilla-audio/src/gau.c
                      SparkyEngine/lib/gorilla-audio/include/gorilla/gau.h
                      SparkyEngine/lib/gorilla-audio/src/common/gc_common.c
                      SparkyEngine/lib/gorilla-audio/include/gorilla/common/gc_common.h
                      SparkyEngine/lib/gorilla-audio/src/common/gc_thread.c
                      SparkyEngine/lib/gorilla-audio/include/gorilla/common/gc_thread.h
                      SparkyEngine/lib/gorilla-audio/include/gorilla/common/gc_types.h
                      SparkyEngine/lib/gorilla-audio/src/devices/ga_openal.c
                      SparkyEngine/lib/gorilla-audio/include/gorilla/devices/ga_openal.h

                      SparkyEngine/lib/libvorbis/include/vorbis/codec.h
                      SparkyEngine/lib/libvorbis/include/vorbis/vorbisenc.h
                      SparkyEngine/lib/libvorbis/include/vorbis/vorbisfile.h

                      SparkyEngine/lib/libvorbis/lib/books/coupled/res_books_stereo.h
                      SparkyEngine/lib/libvorbis/lib/books/floor/floor_books.h
                      SparkyEngine/lib/libvorbis/lib/books/uncoupled/res_books_uncoupled.h

                      SparkyEngine/lib/libvorbis/lib/modes/floor_all.h
                      SparkyEngine/lib/libvorbis/lib/modes/psych_8.h
                      SparkyEngine/lib/libvorbis/lib/modes/psych_11.h
                      SparkyEngine/lib/libvorbis/lib/modes/psych_16.h
                      SparkyEngine/lib/libvorbis/lib/modes/psych_44.h
                      SparkyEngine/lib/libvorbis/lib/modes/residue_8.h
                      SparkyEngine/lib/libvorbis/lib/modes/residue_16.h
                      SparkyEngine/lib/libvorbis/lib/modes/residue_44.h
                      SparkyEngine/lib/libvorbis/lib/modes/residue_44u.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_8.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_11.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_16.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_22.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_32.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_44.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_44u.h
                      SparkyEngine/lib/libvorbis/lib/modes/setup_X.h

                      SparkyEngine/lib/libvorbis/lib/analysis.c
                      SparkyEngine/lib/libvorbis/lib/backends.h
                      SparkyEngine/lib/libvorbis/lib/barkmel.c
                      SparkyEngine/lib/libvorbis/lib/bitrate.c
                      SparkyEngine/lib/libvorbis/lib/bitrate.h
                      SparkyEngine/lib/libvorbis/lib/block.c
                      SparkyEngine/lib/libvorbis/lib/codebook.c
                      SparkyEngine/lib/libvorbis/lib/codebook.h
                      SparkyEngine/lib/libvorbis/lib/codec_internal.h
                      SparkyEngine/lib/libvorbis/lib/envelope.c
                      SparkyEngine/lib/libvorbis/lib/envelope.h
                      SparkyEngine/lib/libvorbis/lib/floor0.c
                      SparkyEngine/lib/libvorbis/lib/floor1.c
                      SparkyEngine/lib/libvorbis/lib/highlevel.h
                      SparkyEngine/lib/libvorbis/lib/info.c
                      SparkyEngine/lib/libvorbis/lib/lookup.h
                      SparkyEngine/lib/libvorbis/lib/lookup.c
                      SparkyEngine/lib/libvorbis/lib/lookup_data.h
                      SparkyEngine/lib/libvorbis/lib/lpc.c
                      SparkyEngine/lib/libvorbis/lib/lpc.h
                      SparkyEngine/lib/libvorbis/lib/lsp.c
                      SparkyEngine/lib/libvorbis/lib/lsp.h
                      SparkyEngine/lib/libvorbis/lib/mapping0.c
                      SparkyEngine/lib/libvorbis/lib/mdct.c
                      SparkyEngine/lib/libvorbis/lib/mdct.h
                      SparkyEngine/lib/libvorbis/lib/misc.h
                      SparkyEngine/lib/libvorbis/lib/os.h
                      SparkyEngine/lib/libvorbis/lib/psy.c
                      SparkyEngine/lib/libvorbis/lib/psy.h

                      SparkyEngine/lib/libvorbis/lib/registry.c
                      SparkyEngine/lib/libvorbis/lib/registry.h
                      SparkyEngine/lib/libvorbis/lib/res0.c
                      SparkyEngine/lib/libvorbis/lib/scales.h
                      SparkyEngine/lib/libvorbis/lib/sharedbook.c
                      SparkyEngine/lib/libvorbis/lib/smallft.c
                      SparkyEngine/lib/libvorbis/lib/smallft.h
                      SparkyEngine/lib/libvorbis/lib/synthesis.c
                      SparkyEngine/lib/libvorbis/lib/tone.c
                      SparkyEngine/lib/libvorbis/lib/vorbisenc.c
                      SparkyEngine/lib/libvorbis/lib/vorbisfile.c
                      SparkyEngine/lib/libvorbis/lib/window.c
                      SparkyEngine/lib/libvorbis/lib/window.h

                      SparkyEngine/lib/libogg/include/ogg/config_types.h
                      SparkyEngine/lib/libogg/include/ogg/ogg.h
                      SparkyEngine/lib/libogg/include/ogg/os_types.h
                      SparkyEngine/lib/libogg/src/bitwise.c
                      SparkyEngine/lib/libogg/src/framing.c
        )

target_include_directories(NavigationSystem PRIVATE SparkyEngine/lib/FreeImage/Source)
target_include_directories(NavigationSystem PRIVATE SparkyEngine/lib/freetype-2.10.0/include)
target_include_directories(NavigationSystem PRIVATE SparkyEngine/lib/box2d/2.4.1/include)


target_link_libraries(NavigationSystem glfw)
target_link_libraries(NavigationSystem libglew_static)
target_link_libraries(NavigationSystem FreeImage)
target_link_libraries(NavigationSystem freetype)
target_link_libraries(NavigationSystem ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/openal-soft/1.22.2/lib/libopenal.dylib)
target_link_libraries(NavigationSystem ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/libpng/1.6.37/lib/libpng16.a)
target_link_libraries(NavigationSystem ${CMAKE_CURRENT_SOURCE_DIR}/SparkyEngine/lib/box2d/2.4.1/lib/libbox2d.a)


find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenAL REQUIRED)
find_package(box2d REQUIRED)

if (APPLE)
  target_link_libraries(NavigationSystem "-framework OpenGL")
endif()