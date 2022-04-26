#include "glHelper.h"

void glHelper::error_callback(int error, const char* description) {
    Logger::getInstance().log("OpenGL Error: ", std::string(description));
}

void glHelper::size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glHelper::glInit() {
    if (!initialized) {
        if (!glfwInit()) Logger::getInstance().log("Failed to initialize GLFW!");
        window = glfwCreateWindow(280*2, 295, "NESem", NULL, NULL);
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        if (primary) {
            const GLFWvidmode* desktop = glfwGetVideoMode(primary);
            if (desktop) glfwSetWindowPos(window, (desktop->width / 2) -280, (desktop->height/2) - (295/2));
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) Logger::getInstance().log("Failed to initialize GLAD!");
        glfwSetErrorCallback(this->error_callback);
        glfwSetFramebufferSizeCallback(window, this->size_callback);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL2_Init();

        glfwSwapInterval(0); 
        initialized = true;
    }
}

void glHelper::render() {
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool glHelper::shouldClose() {
    bool close = glfwWindowShouldClose(this->window);
    if (close) {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }
    return close;
}

void glHelper::setupRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void glHelper::resizeWindow(bool menu)
{
    glfwSetWindowSizeLimits(window, menu ? 560 : 0, menu ? 295 : 0, menu ? 560 : 0xffff, menu ? 295 : 0xffff);
    glfwSetWindowSize(window, menu ? 560 : 768, menu ? 295 : 720);  
}
