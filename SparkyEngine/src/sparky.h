#if 1
#pragma once

#include "engine.h"

//class b2World;
namespace sparky {

    class Sparky {
    private:
        graphics::Window *m_Window;
        Timer *m_Timer;
        unsigned int m_FramesPerSecond, m_UpdatesPerSecond;
        audio::Sound* m_Sound;
        std::unique_ptr<b2World> m_World;
        std::vector<physics::Box> m_Boxes;

    protected:
        //b2World* m_PhysicsWorld;
        Sparky()
                : m_FramesPerSecond(0), m_UpdatesPerSecond(0) {
        }

        virtual ~Sparky() {
            delete m_Window;
            delete m_Timer;
        }

        graphics::Window *createWindow(const char *title, int width, int height) {
            m_Window = new graphics::Window(title, width, height);
            return m_Window;
        }

        //void InitPhysics();
        //void Add(entity::Entity* entity);

        //std::vector<entity::Entity*> m_Entities;
    public:
        void start() {
            init();
            //initPhysics();
            run();
        }

    protected:
        virtual void init() = 0;

        //virtual void initPhysics() = 0;

        virtual void tick() {}

        virtual void update() {}

        virtual void glim() {}

        virtual void render() = 0;

        const unsigned int getFPS() const { return m_FramesPerSecond; }

        const unsigned int getUPS() const { return m_UpdatesPerSecond; }

    private:
        void run() {
            m_Timer = new Timer();

            float timer = 0.0f;
            float updateTimer = 0.0f;
            float updateTick = 1.0f / 60.0f;
            unsigned int frames = 0;
            unsigned int updates = 0;
            float gain = 0.5f;

            std::string music_path = "../music/test.wav";
            m_Sound = new audio::Sound("Music",music_path);
            audio::SoundManager::add(m_Sound);
            audio::SoundManager::get("Music")->setGain(10.0f);

            while (!m_Window->closed()) {
                m_Window->clear();
                if (m_Timer->elapsed() - updateTimer > updateTick) {
                    m_Window->updateInput();
                    update();
                    updates++;
                    updateTimer += updateTick;
                }
                render();

                glim();

                frames++;

                m_Window->update();
                if (m_Timer->elapsed() - timer > 1.0f) {
                    timer += 1.0f;
                    m_FramesPerSecond = frames;
                    m_UpdatesPerSecond = updates;
                    frames = 0;
                    updates = 0;
                    tick();
                }
            }
        }
    };
}
#endif

#if 0
#pragma once
//#include "graphics/camera/OrthographicCamera.h"
#include "graphics/BatchRenderer2D.h"
//#include "entity/Entity.h"
#include "engine.h"
#include "../src/utils/timer.h"
class b2World;

namespace sparky {

    class Scene2D
    {
    private:
        //graphics::OrthographicCamera* m_Camera;
        graphics::BatchRenderer2D* m_Renderer;

        std::vector<entity::Entity*> m_Entities;
        b2World* m_PhysicsWorld;
    public:
        Scene2D();
        Scene2D(const maths::mat4& projectionMatrix);
        ~Scene2D();

        void Init();
        void Add(entity::Entity* entity);

        virtual void OnUpdate(const Timer& ts);
        virtual void OnRender(graphics::BatchRenderer2D& renderer);

        void OnRender();

        inline graphics::BatchRenderer2D* GetRenderer() { return m_Renderer; }
        //inline graphics::OrthographicCamera* GetCamera() { return m_Camera; }
    };

}
#endif