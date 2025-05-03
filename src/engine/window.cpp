#include "window.hpp"

void Window::sizeCallback(GLFWwindow* window, [[maybe_unused]] int w, [[maybe_unused]] int h) {
    Window* handler = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    handler->width = w;
    handler->height = h;

    handler->sizeCallback();
}

void Window::sizeCallback() {
    glViewport(0, 0, width, height);
}

void Window::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    Window* handler = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    handler->keyboardCallback(key, scancode, action, mode);
}

void Window::keyboardCallback(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mode) {
    if (action == GLFW_PRESS) {
        keyPressed.insert(key);
    } else if (action == GLFW_RELEASE) {
        keyPressed.erase(key);
    }
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mode) {
    Window* handler = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    handler->mouseCallback(button, action, mode);
}

void Window::mouseCallback(int button, int action, int) {
    if (action == GLFW_PRESS) {
        mousePressed.insert(button);
    } else if (action == GLFW_RELEASE) {
        mousePressed.erase(button);
    }
}

Window::Window(std::string title, int width, int height) : title{title}, width{width}, height{height}, lastFrame{0.0f}, dt{0.0f} {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(m_window, this);

    if (m_window == NULL) {
        std::cerr << "Failed to create GLFW window!\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(m_window);

    // Load all OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!\n";
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glfwSetFramebufferSizeCallback(m_window, sizeCallback);
    glfwSetKeyCallback(m_window, keyboardCallback);
    glfwSetMouseButtonCallback(m_window, mouseCallback);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::restoreState() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::update() {
    float currentFrame = glfwGetTime();
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwGetCursorPos(m_window, &mouseX, &mouseY);

    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

bool Window::keyDown(int key) {
    return keyPressed.find(key) != keyPressed.end();
}

bool Window::mouseDown(int button) {
    return mousePressed.find(button) != mousePressed.end();
}