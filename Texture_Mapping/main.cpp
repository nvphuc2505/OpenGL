#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int gl_width = 640;
int gl_height = 480;

int main()
{
    GLfloat points[] = {
        -0.5f, -0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f, 
        0.5f, 0.5f, 0.0f, 
        0.5f, 0.5f, 0.0f, 
        -0.5f, 0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f
    };

    GLfloat textures[] = {
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 
        0.0f, 0.0f
    };

    if (!glfwInit ()) {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
    }
    
    GLFWwindow* window = glfwCreateWindow (gl_width, gl_height, "Rectangle", NULL, NULL);
    if (!window) {
    fprintf (stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
    }
    glfwMakeContextCurrent (window);
    
    glewExperimental = GL_TRUE;
    glewInit ();

    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);

    glEnable (GL_DEPTH_TEST); 
    glDepthFunc (GL_LESS); 

    GLuint points_vbo;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GL_FLOAT), &points, GL_STATIC_DRAW);

    GLuint textures_vbo;
    glGenBuffers(1, &textures_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, textures_vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GL_FLOAT), &textures, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, textures_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    const char* vertex_shader = 
    "#version 400\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec2 aTex;"

    "out vec2 texCoord;"

    "void main(){"
        "gl_Position = vec4(vertex_position, 1.0f);"
        "texCoord = aTex;"
    "}";

    const char* fragment_shader = 
    "#version 400\n"
    "out vec4 frag_colour;"

    "in vec2 texCoord;"

    "uniform sampler2D tex0;"

    "void main() {"
        "frag_colour = texture(tex0, texCoord);"
    "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);
    glBindAttribLocation(shader_programme, 0, "vertex_position");
    glLinkProgram(shader_programme);

                            // TEXTURES //
    int width_Imag, height_Imag, numColCh;
    unsigned char* bytes = stbi_load("skulluvmap.png", &width_Imag, &height_Imag, &numColCh, 4);



    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_Imag, height_Imag, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex = glGetUniformLocation(shader_programme, "tex0");
    glUseProgram(shader_programme);
    glUniform1i(tex, 0);

    //////////////////////////////////////////////////////////////////////

    while(!glfwWindowShouldClose(window))
    {

        // 6 steps
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport( 0, 0, gl_width, gl_height );
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, texture);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    glDeleteTextures(1, &texture);


    glfwTerminate();
    return 0;
}