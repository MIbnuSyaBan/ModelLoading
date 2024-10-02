//------- Ignore this ----------
#include <filesystem>
namespace fs = std::filesystem;
//------------------------------

#include "Model.h"
#include <chrono>  // For timing calculations (delta time)
#include <cmath>   // For sine wave (sin)

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
    // Initialize GLFW
    glfwInit();

    // Specify OpenGL version (3.3) and profile (core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, "ModelLoading", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    gladLoadGL();
    glViewport(0, 0, width, height);

    // Set up shaders
    Shader shaderProgram("default.vert", "default.frag");

    // Set up lighting (white light at a specific position)
    glm::vec4 lightColor = glm::vec4(1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.5f, 1.5f);

    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Create the camera
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 3.0f));

    // Load the model
    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
    std::string modelPath = "/WCG/Models/keyboard6/scene.gltf";
    Model model((parentDir + modelPath).c_str());

    // Set the initial model position and scale
    model.setPosition(glm::vec3(0.0f, -0.5f, 0.0f)); // Initial position
    model.setScale(glm::vec3(1.0f));                 // Initial scale (normal)

    // Variables for controlling rotation, scale, and position oscillation
    float rotationSpeed = 50.0f;        // Rotation speed in degrees per second
    float scaleSpeed = 2.0f;            // Speed of scale oscillation
    float scaleAmplitude = 0.5f;        // Amplitude of scale oscillation (max scale = 1.5)
    float minScale = 1.0f;              // Minimum scale value

    float positionSpeed = 1.0f;         // Speed of position oscillation
    float positionAmplitude = 1.0f;     // Amplitude of position oscillation (movement range)

    // Timing variables for delta time calculation
    auto lastTime = std::chrono::high_resolution_clock::now();

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastTime;
        float deltaTime = elapsedTime.count();
        lastTime = currentTime;

        // Clear the screen (color and depth buffer)
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process camera input
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Update model rotation (continuously rotates around Y-axis)
        model.rotate(glm::vec3(0.0f, rotationSpeed * deltaTime, 0.0f));

        // Update model scale (oscillates between minScale and minScale + scaleAmplitude)
        float scaleFactor = minScale + scaleAmplitude * (std::sin(glfwGetTime() * scaleSpeed) * 0.5f + 0.5f);
        model.setScale(glm::vec3(scaleFactor));

        // Update model position (oscillates along the Z-axis)
        float oscillatingZ = positionAmplitude * std::sin(glfwGetTime() * positionSpeed);
        model.setPosition(glm::vec3(0.0f, -0.5f, oscillatingZ));

        // Draw the model
        model.Draw(shaderProgram, camera);

        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
