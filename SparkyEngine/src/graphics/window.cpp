#define GL_SILENCE_DEPRECATION
#define BLENDING 0

#include "window.h"

namespace sparky { namespace graphics {

    void resize_callback(GLFWwindow* m_window, int width, int height);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); 
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xpos, double ypos);

    Window::Window(const char* title, int width, int height, float cameraLeft, float cameraRight, float cameraUp, float cameraBottom)
    {
        glfwInit();
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        m_Title = title;
        m_Width = width;
        m_Height = height;

        m_StartWidth = m_Width;
        m_StartHeight = m_Height;

        if (!init()) 
            glfwTerminate();

        FontManager::add(new Font("SourceSansPro",
                                  "../SparkyEngine/fonts/SourceSansPro-Light.ttf",
                                   28));

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

        this->cameraLeft = cameraLeft;// = -41.68f;   //default: -16.0f
        this->cameraRight =  cameraRight;//cameraRight = 41.68f;   //default:  16.0f
        this->cameraBottom =  cameraBottom;//cameraBottom = -26.05f;   //default: -10.0f
        this->cameraUp =  cameraUp;//cameraUp = 26.05f;
    }

    Window::~Window()
    {
        FontManager::clean();
        TextureManager::clean(); ///////AAAAAAAAAAAAAAAAAAAAADDDDDDDDDDDDDDDDD
        glfwTerminate();
    }

    bool Window::init()
    {
        if (!glfwInit()) 
        {
            std::cout << "Failed to initialize GLFW window!" << std::endl;
            return false;
        }

        GLFWmonitor* MyMonitor =  glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(MyMonitor);
        auto SCR_WIDTH = mode->width;
        auto SCR_HEIGHT = mode->height;

        //here I devide by 2 because window was created two times bigger before
        m_Window = glfwCreateWindow(m_Width / 2, m_Height / 2, m_Title, NULL, NULL);

        if (!m_Window) 
        {
            glfwTerminate();
            std::cout << "Failed to create GLFW window!" << std::endl; 
            return false;
        }

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, this);
        glfwSetFramebufferSizeCallback(m_Window, resize_callback);
        glfwSetKeyCallback(m_Window, key_callback);
        glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
        glfwSetCursorPosCallback(m_Window, cursor_position_callback);
        glfwSetScrollCallback(m_Window, scroll_callback);
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
        //glClearColor(0.85f, 0.0f, 0.2f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::update()
    {
        if (!m_Minimized)
        {
            if (isCursorDisabled)
                glfwSetInputMode((GLFWwindow*)m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            else
                glfwSetInputMode((GLFWwindow*)m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                std::cout << "OpenGL Error: " <<  error << std::endl;
            }

            glfwSwapBuffers(m_Window);
            glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
            glfwPollEvents();
        }
    }

    void Window::updateInput()
    {
        if (!m_Minimized)
        {
            for (int i = 0; i < MAX_KEYS; i++)
                m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];

            for (int i = 0; i < MAX_BUTTONS; i++)
                m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

            memcpy(m_KeyState, m_Keys, MAX_KEYS);
            memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
        }
    }

    bool Window::closed() const
    {
        return glfwWindowShouldClose(m_Window) == 1;
    }

    void resize_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
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
        win->m_MousePosition.x = (float)xpos * 2;
        win->m_MousePosition.y = (float)ypos * 2;
    }

    void scroll_callback(GLFWwindow *window, double xpos, double ypos) {
        //Window* win = (Window*) glfwGet(window);
    }

    bool Window::resized() {
        if (getWidth() == 0 || getHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;

        return false;
    }

} }