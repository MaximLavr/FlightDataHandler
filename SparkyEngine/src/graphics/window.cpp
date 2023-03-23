#define GL_SILENCE_DEPRECATION
#define BLENDING 0

#include "window.h"

namespace sparky { namespace graphics {

    void window_resize(GLFWwindow* m_window, int width, int height);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); 
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    Window::Window(const char* title, int width, int height)
    {
        glfwInit();
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        m_Title = title;
        m_Width = width;
        m_Height = height;

        if (!init()) 
            glfwTerminate();

        FontManager::add(new Font("SourceSansPro",
                                  "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/SourceSansPro-Light.ttf",
                                   28));
        audio::SoundManager::init();

        for (int i = 0; i < MAX_KEYS; i++) 
        {
            m_Keys[i] = false;
            m_KeyState[i] = false;
            m_KeyTyped[i] = false;
        }

        for (int i = 0; i < MAX_BUTTONS; i++) 
        {
            m_MouseButtons[i] = false;
            m_MouseState[i] = false;
            m_MouseClicked[i] = false;
        }
    }

    Window::~Window()
    {
        FontManager::clean();
        TextureManager::clean(); ///////AAAAAAAAAAAAAAAAAAAAADDDDDDDDDDDDDDDDD
        audio::SoundManager::clean();
        glfwTerminate();
    }

    bool Window::init()
    {
        if (!glfwInit()) 
        {
            std::cout << "Failed to initialize GLFW window!" << std::endl;
            return false;
        }

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);

        if (!m_Window) 
        {
            glfwTerminate();
            std::cout << "Failed to create GLFW window!" << std::endl; 
            return false;
        }

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, this);
        glfwSetFramebufferSizeCallback(m_Window, window_resize);
        glfwSetKeyCallback(m_Window, key_callback);
        glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
        glfwSetCursorPosCallback(m_Window, cursor_position_callback);
        glfwSwapInterval(0.0);

        glewExperimental = GL_TRUE; 
        glewInit();

        if (glewInit() != GLEW_OK)
        {
            std::cout << "Could not initialize GLEW!" << std::endl;
            return false;
        }

        //!!!FOR FONTS -> CREATES BLEND LAYERS!!!
#if BLENDING
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
        //!!!FOR FONTS -> CREATES BLEND LAYERS!!!

        return true;
    }

    bool Window::isKeyPressed(unsigned int keycode) const
    {
        if (keycode >= MAX_KEYS)
        return false;
         
        return m_Keys[keycode]; 
    }

    bool Window::isKeyTyped(unsigned int keycode) const
    {
        if (keycode >= MAX_KEYS)
            return false;

        return m_KeyTyped[keycode];
    }

    bool Window::isMouseButtonPressed(unsigned int button) const
    {
        if (button >= MAX_BUTTONS)
        return false;
         
        return m_MouseButtons[button]; 
    }

    bool Window::isMouseButtonClicked(unsigned int button) const
    {
        if (button >= MAX_BUTTONS)
            return false;

        return m_MouseClicked[button];
    }


    const maths::vec2& Window::getMousePosition() const
    {
        return m_MousePosition;
    }

    void Window::clear() const
    {
        //glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::update()
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL Error: " <<  error << std::endl;
        }

        glfwSwapBuffers(m_Window);
        glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
        glfwPollEvents();

        audio::SoundManager::update();
    }

    void Window::updateInput()
    {
        for (int i = 0; i < MAX_KEYS; i++)
            m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];

        for (int i = 0; i < MAX_BUTTONS; i++)
            m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

        memcpy(m_KeyState, m_Keys, MAX_KEYS);
        memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
    }

    bool Window::closed() const
    {
        return glfwWindowShouldClose(m_Window) == 1;
    }

    void window_resize(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        Window* win = (Window*) glfwGetWindowUserPointer(window);
        win->m_Width = width;
        win->m_Height = height;
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* win = (Window*) glfwGetWindowUserPointer(window);
        win->m_Keys[key] = action != GLFW_RELEASE;
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        Window* win = (Window*) glfwGetWindowUserPointer(window);
        win->m_MouseButtons[button] = action != GLFW_RELEASE;
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Window* win = (Window*) glfwGetWindowUserPointer(window);
        win->m_MousePosition.x = (float)xpos;
        win->m_MousePosition.y = (float)ypos;
    }

} }