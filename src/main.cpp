// main.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Scene.h"
#include "Texture.h"
#include "Constants.h"
#include <iostream>
#include <memory>

// OpenGL Debug Output Callback
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cerr << "---------------" << std::endl;
    std::cerr << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cerr << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cerr << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cerr << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cerr << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cerr << "Source: Other"; break;
    } std::cerr << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cerr << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cerr << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cerr << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cerr << "Type: Other"; break;
    } std::cerr << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Severity: notification"; break;
    } std::cerr << std::endl;
    std::cerr << std::endl;
}

// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Static callback function for mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Retrieve the Camera pointer from the window's user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera)
    {
        // Static variables to track the last mouse position and initialization
        static float lastX = WINDOW_WIDTH / 2.0f;
        static float lastY = WINDOW_HEIGHT / 2.0f;
        static bool firstMouse = true;

        if (firstMouse)
        {
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos) - lastX;
        float yoffset = lastY - static_cast<float>(ypos); // Reversed since y-coordinates go from bottom to top
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

// Callback function for scroll input
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Retrieve the Camera pointer from the window's user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera)
    {
        camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

// processInput function remains largely the same, optimized for const correctness
void processInput(GLFWwindow *window, Camera &camera, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);

    // Toggle cursor capture with 'C' key
    static bool cKeyPressed = false;
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed)
    {
        static bool cursorDisabled = true;
        cursorDisabled = !cursorDisabled;
        if(cursorDisabled)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cKeyPressed = true;
    }
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    {
        cKeyPressed = false;
    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Configure GLFW - OpenGL version 4.6 Core Profile for Debug Output
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
#endif

    // Create Window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Optimized OpenGL Lighthouse Scene", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    // Set callback for window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD (Removed GLEW initialization)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    } else {
        std::cout << "GLAD initialized successfully. OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    }

    // Enable OpenGL Debug Output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Enable depth testing and backface culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // Counter-clockwise wound polygons are front-facing

    // Build and compile shaders
    Shader phongShader("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/shaders/phong_vertex_shader.glsl", 
                       "C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/shaders/phong_fragment_shader.glsl");
    Shader skyboxShader("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/shaders/skybox_vertex_shader.glsl", 
                        "C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/shaders/skybox_fragment_shader.glsl");

    // Initialize Camera
    Camera camera(glm::vec3(0.0f, 15.0f, 30.0f)); // Positioned to view the lighthouse

    // Initialize Scene using smart pointer
    auto scene = std::make_unique<Scene>();
    scene->Setup();

    // Setup mouse callbacks with camera reference
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Timing variables
    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f;

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrame;
        lastFrame = currentFrameTime;

        // Input
        processInput(window, camera, deltaTime);

        // Clear the color and depth buffers
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Dark background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        phongShader.use();

        // Set view and projection matrices
        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
                                                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 
                                                0.1f, 
                                                100.0f);
        phongShader.setMat4("view", view);
        phongShader.setMat4("projection", projection);

        // Render Scene
        scene->Render(phongShader, camera, skyboxShader);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return EXIT_SUCCESS;
}
