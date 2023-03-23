#pragma once

//#include "../sparky.h"
#include "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/src/utils/timestep.h"

#include "component/Components.h"
#include "../graphics/batchrenderer2d.h"

namespace sparky { namespace entity {

        class Entity
        {
        protected:
            std::vector<component::Component*> m_Components;
            const maths::AABB* m_BoundingBox;
        public:
            component::TransformComponent* transform;
        public:
            Entity();
            Entity(graphics::Sprite* sprite, const maths::mat4& transform = maths::mat4::identity());
            //Entity(graphics::Mesh* mesh, const maths::mat4& transform = maths::mat4::identity());

            void AddComponent(component::Component* component);
            void OnUpdate(const Timestep& ts);
            void OnRender(graphics::BatchRenderer2D& renderer);

            inline const maths::AABB& GetBoundingBox() const { return *m_BoundingBox; }

            template<typename T>
            T& CreateComponent()
            {
                T* c = new T(this);
                m_Components.push_back(c);
                return *c;
            }

            template<typename T, typename Param>
            T& CreateComponent(Param* p)
            {
                T* c = new T(this, p);
                m_Components.push_back(c);
                return *c;
            }

            template <typename T>
            const T* GetComponent() const
            {
                return GetComponentInternal<T>();
            }

            template <typename T>
            T* GetComponent()
            {
                return (T*)GetComponentInternal<T>();
            }
        private:
            void Init();

            template <typename T>
            const T* GetComponentInternal() const
            {
                component::ComponentType* type = T::GetStaticType();
                for (auto x : m_Components)
                {
                    if (x->GetType() == type)
                        return (const T*)x;
                }
                return nullptr;
            }
        private:
            static void StaticInit();
        };

    } }