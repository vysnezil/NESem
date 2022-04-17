#include "glHelper.h"

void glHelper::error_callback(int error, const char* description) {
    Logger::log("OpenGL Error: ", std::string(description));
}

void glHelper::size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glHelper::glInit() {
    if (!initialized) {
        if (!glfwInit()) Logger::log("Failed to initialize GLFW!");
        window = glfwCreateWindow(768, 720, "NESem", NULL, NULL);
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) Logger::log("Failed to initialize GLAD!");
        glfwSetErrorCallback(this->error_callback);
        glfwSetFramebufferSizeCallback(window, this->size_callback);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        glfwSwapInterval(0); 
        initialized = true;
    }
}

void glHelper::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool glHelper::shouldClose() {
    bool close = glfwWindowShouldClose(this->window);
    if (close) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }
    return close;
}

void glHelper::setupRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}