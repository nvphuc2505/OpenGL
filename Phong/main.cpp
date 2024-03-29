#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/normalize_dot.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <fstream>
#include <cstring>

#include "vbo.h"
#include "vao.h"

int g_gl_width = 640;
int g_gl_height = 480;

glm::mat4 model_mat = glm::mat4(1.0f);
glm::mat4 proj_mat  = glm::mat4(1.0f);
glm::mat4 view_mat  = glm::mat4(1.0f);

GLfloat points[] = 
{
    0.0f,  0.5f,  0.0f, 
    0.5f, -0.5f,  0.0f, 
    -0.5f, -0.5f,  0.0f
};

GLfloat normals[] =
{
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f
};

bool parse_file_into_str(const char* file_name, char* shader_str, int max_len)
{
    std::ifstream file(file_name);
    if(!file.is_open()) 
    {    
        std::cerr << "ERROR: opening file for reading: " << file_name << std::endl;
        return false;
    }

    file.read(shader_str, max_len - 1);

    if (file.gcount() >= max_len - 1) std::cerr << "WARNING: file " << file_name << " too big - truncated." << std::endl;
    
    if (file.fail() && !file.eof()) 
    {
        std::cerr << "ERROR: reading shader file " << file_name << std::endl;
        file.close();
        return false;
    }

    shader_str[file.gcount()] = '\0';

    file.close();
    return true;
}

glm::quat create_Quaternion(float degree, float x, float y, float z) {
    float rad = glm::radians(degree);
    return glm::normalize(glm::quat(cos(rad / 2), sin(rad / 2) * x, sin(rad / 2) * y, sin(rad / 2) * z));
}

int main()
{
    if (!glfwInit ()) {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
    }
   
    GLFWwindow* window = glfwCreateWindow (g_gl_width, g_gl_height, "Texture", NULL, NULL);
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
    
    VBO points_vbo(points, sizeof(points));
    VBO normals_vbo(normals, sizeof(normals));

    VAO vao; 
    vao.setUpAttrib(points_vbo, 0, 3, GL_FLOAT, 0, NULL);
    vao.setUpAttrib(normals_vbo, 1, 3, GL_FLOAT, 0, NULL);

    //=====================================================

    // Vertex Shader
    char vertex_shader[1024 * 256];
    parse_file_into_str("test_vs.glsl", vertex_shader, 1024 * 256);
    const GLchar* v = (const GLchar*) vertex_shader;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &v, NULL);
    glCompileShader(vs);
    
    // Fragment Shader
    char fragment_shader[1024 * 256];
    parse_file_into_str("test_fs.glsl", fragment_shader, 1024 * 256);
    const GLchar* f = (const GLchar*)fragment_shader;

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &f, NULL);
    glCompileShader(fs);

    // Shader Porgram
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);

    glBindAttribLocation(shader_programme, 0, "vertex_position");
    glBindAttribLocation(shader_programme, 1, "vertex_normal");

    glLinkProgram(shader_programme);

    // PROJECT MATRIX
    float aspect = (float) (g_gl_width / g_gl_height);
    float fov = 67.0f;
    float near = 0.1f; 
    float far = 100.0f;
    proj_mat = glm::perspective(glm::radians(fov), aspect, near, far);

    // VIEW MATRIX;
    float cam_speed = 5.0f;
    glm::vec3 cam_pos(0.0f, 0.0f, 2.0f);
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), -cam_pos);
    
    float cam_heading_speed = 100.0f;
    float cam_heading = 0.0f;
    // glm::quat quaternion(w, x, y, z);
    glm::quat quaternion;
    quaternion = create_Quaternion(-cam_heading, 0.0f, 1.0f, 0.0f);
    glm::mat4 rotate_matrix = glm::toMat4(quaternion);

    view_mat = rotate_matrix * translate_matrix;

    // Matrix Shader
    int proj_mat_location = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(proj_mat));    

    int view_mat_location = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view_mat));

    int model_mat_location = glGetUniformLocation(shader_programme, "model");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, glm::value_ptr(model_mat));

    while(!glfwWindowShouldClose(window))
    {
        // Time will be run when program start
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);
        vao.bindVAO();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();

        bool cam_move = false;

        // TRANSLATION
        if(glfwGetKey(window, GLFW_KEY_UP))
        {
            cam_pos.y -= elapsed_seconds * cam_speed;
            cam_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN))
        {
            cam_pos.y += elapsed_seconds * cam_speed;
            cam_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            cam_pos.x -= elapsed_seconds * cam_speed;
            cam_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT))
        {
            cam_pos.x += elapsed_seconds * cam_speed;
            cam_move = true;
        }

        // ROTATION
        /*
            cam_yaw   ----  rotate y axis
            cam_pitch ----  rotate x axis
            cam_roll  ----  rotate z axis
        */
        float cam_yaw = 0.0f;
        glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

        if(glfwGetKey(window, GLFW_KEY_C))
        {
            cam_yaw += elapsed_seconds * cam_heading_speed;
            cam_move = true;

            glm::quat q_yaw = create_Quaternion(-cam_yaw, up.x, up.y, up.z);
            quaternion *= q_yaw;

            rotate_matrix = glm::toMat4(quaternion);
            up = rotate_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        }
        if(glfwGetKey(window, GLFW_KEY_Z))
        {
            cam_yaw -= elapsed_seconds * cam_heading_speed;
            cam_move = true;

            glm::quat q_yaw = create_Quaternion(-cam_yaw, up.x, up.y, up.z);
            quaternion *= q_yaw;

            rotate_matrix = glm::toMat4(quaternion);
            up = rotate_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        }
        

        if(cam_move == true)
        {
            translate_matrix = glm::translate(glm::mat4(1.0f), -cam_pos);
            
            view_mat = rotate_matrix * translate_matrix;
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view_mat));
        }

        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwWindowShouldClose(window) == GL_TRUE;
        glfwSwapBuffers(window);
    }

    points_vbo.clearVBO();
    normals_vbo.clearVBO();
    glfwTerminate();
    return 0;
}