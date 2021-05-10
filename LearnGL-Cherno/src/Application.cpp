#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <intrin.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CE", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR: GLEW couldn't be initialised!\n";

    std::cout << glGetString(GL_VERSION) << '\n';

#if _DEBUG
    std::cout << "Cherno Engine v1.0\n" "[DEBUG MODE] (Warning: Performace compromised)\n" "::[DEBUG CONSOLE]::\n\n"; // Disabled in Release
#else
    std::cout << "Cherno Engine v1.0\n" "[RELEASE MODE]\n\n";
#endif // _DEBUG

    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        float positions[6] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
        };


        unsigned int indices[3] = {
            0, 1, 2
        };

        float positions_glm[8] = {
            100.0f, 100.0f,
            200.0f, 100.0f,
            200.0f, 200.0f,
            100.0f, 200.0f
        };

        unsigned int indices_glm[6] = {
            0, 1, 2,
            2, 3, 0
        };

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -100.0f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        VertexArray va;
        VertexBuffer vb(positions_glm, 2 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices_glm, 6);

        Shader shader("res/shaders/QuadProjection.shader");
        shader.Bind();

        Renderer renderer;
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2DTexture");

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }


    std::cout << "Shutting engine down\n\n";

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    std::cout << "Shutdown successful\n";

    return 0;
}