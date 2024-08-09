#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


int main()
{
	GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Fereastra", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* Initializing GLEW */
    if (glewInit() != GLEW_OK)
        std::cout << "Glew initializing error!\n";
    else
        std::cout << "Glew version: " << glGetString(GL_VERSION) << "\n";

    /* Declaring the positions for the Vertexes */
    float positions[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, //0
         0.5f, -0.5f, 1.0f, 0.0f, //1
         0.5f,  0.5f, 1.0f, 1.0f, //2
        -0.5f,  0.5f, 0.0f, 1.0f  //3 
    };

    /* Initializing Vertex Indices */
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
   
    VertexArray va;

    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    //layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    /* Creating and binding Index Buffer */
    IndexBuffer ib(indices, 6);
  
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();    
    shader.SetUniform4f("u_Color", 0.2f, 0.5f, 0.2f, 1.0f);

    //va.Bind();
    //ib.Bind();

    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    //va.Unbind();
    //vb.Unbind();
    //ib.Unbind();
    //shader.Unbind();

    Texture texture("res/pisica.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);
    
    //shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.01f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.SetUniform4f("u_Color", r, 0.5f, 0.2f, 1.0f);

        vb.Bind();

        renderer.Draw(va, ib, shader);
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            increment = -0.01f;
        else if (r < 0.0f)
            increment = 0.01f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 1;

}