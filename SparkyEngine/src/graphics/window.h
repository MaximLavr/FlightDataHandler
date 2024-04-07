#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "font_manager.h"
#include "texture_manager.h"
#include "../maths/vec2.h"

namespace sparky { namespace graphics {

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

    class Window
    {
    private: 
        const char *m_Title;
        GLFWwindow* m_Window;
        bool m_Closed();
        int m_Width;
        int m_Height;


        bool m_Keys[MAX_KEYS];
        bool m_MouseButtons[MAX_BUTTONS];
        bool m_KeyState[MAX_KEYS];
        bool m_KeyTyped[MAX_KEYS];
        bool m_MouseState[MAX_BUTTONS];
        bool m_MouseClicked[MAX_BUTTONS];

        maths::vec2 m_MousePosition;
        
    public:
        bool isCursorDisabled;
        float cameraLeft;
        float cameraRight;
        float cameraBottom;
        float cameraUp;
        Window(const char* title, int width, int height, float cameraLeft, float cameraRight, float cameraUp, float cameraBottom);
        ~Window();
        void clear() const;
        void update();
        void updateInput();
        bool closed() const;
        bool resized();

        inline const int getWidth() { return m_Width; } //960
        inline const int getHeight() { return m_Height; } //540

        bool isKeyPressed(unsigned int keycode) const;
        bool isKeyTyped(unsigned int keycode) const;
        bool isMouseButtonPressed(unsigned int button) const;
        bool isMouseButtonReleasedAfterPressed(unsigned int button) const;
        bool isMouseButtonClicked(unsigned int button) const;
        const maths::vec2& getMousePosition() const;

        int m_StartWidth, m_StartHeight;
        bool m_Minimized = false;
    private:
        bool init();
        friend void resize_callback(GLFWwindow* window, int width, int height);
        friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); 
        friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        friend void scroll_callback(GLFWwindow* window, double xpos, double ypos);
    };
} } 