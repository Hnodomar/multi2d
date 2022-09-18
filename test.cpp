#include <glad.h>
#include <GLFW/glfw3.h>

#include <string_view>
#include <fstream>
#include <vector>

#include "scenes/shader.hpp"
#include "client/window.hpp"
#include "assets/bg_image.hpp"
#include "util/exception.hpp"
#include "assets/bitmap_font.hpp"


using namespace multi2d;

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    multi2d::window_t window(SCR_WIDTH, SCR_HEIGHT);

    // build and compile our shader zprogram
    // ------------------------------------
    const std::string v_code = 
      R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        void main()
        {
          gl_Position = vec4(aPos, 1.0);
          ourColor = aColor;
          TexCoord = vec2(aTexCoord.x, aTexCoord.y);
        }      
      )"
    ;

    const std::string f_code =
      R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;

        // texture sampler
        uniform sampler2D texture1;

        void main()
        {
          FragColor = texture(texture1, TexCoord);
        }
      )"
    ;
    
    multi2d::shader_t ourShader(v_code, f_code);

    multi2d::bg_image_t bg_image("../extern/textures/main_menu_bg.png", window);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window.window()))
    {
        // input
        // -----
        processInput(window.window());

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bg_image.draw(1, 1);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.window());
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}