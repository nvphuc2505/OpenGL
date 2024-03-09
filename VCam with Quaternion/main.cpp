/*
pitch = W, S keys                                                
yaw = A, D keys                                                 
roll = Z, C keys   

move forward/back = w,s keys                                                
move left/right = a,d keys
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <math.h>

#include "math_func.h"

void normalise_quaternion(float (&q)[4]);
void create_quaternion(float (&q)[4], float degrees, float x, float y, float z);
void multiply_two_quaternion(float (&result)[4], float q[4], float r[4]);
void quat_to_mat4(float (&matrix)[16], float q[4]);

int main()
{
    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLfloat colours[] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    GLFWwindow* window = glfwCreateWindow (640, 480, "Triangle", NULL, NULL);
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
    /*========================================================*/

    GLuint points_vbo;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GL_FLOAT), &points, GL_STATIC_DRAW);

    GLuint colours_vbo;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GL_FLOAT), &colours, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    const char* vertex_shader = 
    "#version 400\n"

    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_colour;"

    "uniform mat4 proj;"
    "uniform mat4 view;"

    "out vec3 colour;"

    "void main()"
    "{"
        "colour = vertex_colour;"
        "gl_Position = proj * view * vec4(vertex_position, 1.0);"
    "}";

    const char* fragment_shader = 
    "#version 400\n"

    "in vec3 colour;"
    "out vec4 frag_colour;"

    "void main() {"
        "frag_colour = vec4 (colour, 1.0);"
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



    //Built Proj Matrix
    float aspect_ratio = 640.0f / 480.0f;
    float far          = 100.0f;
    float near         = 0.1f;
    float fov          = (67.0f * M_PI) / 180.0f;
    float range        = tanf(fov * 0.5f) * near;

    float Sx = (2 * near) / (range * aspect_ratio + range * aspect_ratio);
    float Sy = near / range;
    float Sz = -(far + near) / (far - near);
    float Pz = -(2 * far * near) / (far - near);

    float proj_mat[16] = {
        Sx, 0.0f, 0.0f, 0.0f,
        0.0f, Sy, 0.0f, 0.0f,
        0.0f, 0.0f, Sz, Pz,
        0.0f, 0.0f, -1.0f, 0.0f
    };
    transpose_matrix(proj_mat);

    // Built View Matrix
    float cam_speed = 5.0f;
    float cam_pos[3] = {0.0f, 0.0f, 2.0f};
    float T[16] = {
        1.0f, 0.0f, 0.0f, -cam_pos[0],
        0.0f, 1.0f, 0.0f, -cam_pos[1],
        0.0f, 0.0f, 1.0f, -cam_pos[2],
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    transpose_matrix(T);

    float cam_heading_speed = 100.0f;
    float cam_heading = 0.0f;
    float R[16];
    float quaternion[4];
    create_quaternion(quaternion, -cam_heading, 0.0f, 1.0f, 0.0f);
    quat_to_mat4(R, quaternion);

    float view_mat[16];
    matrix_multiplite_matrix(view_mat, R, T);

    // Load all Matrices to Shader
    int proj_mat_location = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat);

    int view_mat_location = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat);

    // Rotated Axis (x, y, z)
    float forward[4]  = {0.0f, 0.0f, -1.0f, 0.0f};
    float up[4]       = {0.0f, 1.0f, 0.0f, 0.0f};
    float right[4]    = {1.0f, 0.0f, 0.0f, 0.0f};

    while(!glfwWindowShouldClose(window))
    {
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();

        bool cam_moved = false;
        float cam_pitch = 0.0f;
        float cam_yaw = 0.0f;
        float cam_roll = 0.0f;

        // Rotation X-asix
        if(glfwGetKey(window, GLFW_KEY_W))
        {
            cam_pitch += cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_pitch[4];
            create_quaternion(q_pitch, cam_pitch, right[0], right[1], right[2]);
            multiply_two_quaternion(quaternion, quaternion, q_pitch);

            quat_to_mat4(R, quaternion);
        }
        if(glfwGetKey(window, GLFW_KEY_S))
        {
            cam_pitch -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_pitch[4];
            create_quaternion(q_pitch, cam_pitch, right[0], right[1], right[2]);
            multiply_two_quaternion(quaternion, quaternion, q_pitch);

            quat_to_mat4(R, quaternion);
        }

        // Rotation Y-asix
        if(glfwGetKey(window, GLFW_KEY_D))
        {
            cam_yaw -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_yaw[4];
            create_quaternion(q_yaw, cam_yaw, up[0], up[1], up[2]);
            multiply_two_quaternion(quaternion, quaternion, q_yaw);

            quat_to_mat4(R, quaternion);
        }
        if(glfwGetKey(window, GLFW_KEY_A))
        {
            cam_yaw += cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_yaw[4];
            create_quaternion(q_yaw, cam_yaw, up[0], up[1], up[2]);
            multiply_two_quaternion(quaternion, quaternion, q_yaw);

            quat_to_mat4(R, quaternion);
        }

        // Rotation Z-asix
        if(glfwGetKey(window, GLFW_KEY_Z))
        {
            cam_roll -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_roll[4];
            create_quaternion(q_roll, cam_roll, forward[0], forward[1], forward[2]);
            multiply_two_quaternion(quaternion, quaternion, q_roll);

            quat_to_mat4(R, quaternion);
        }
        if(glfwGetKey(window, GLFW_KEY_C))
        {
            cam_roll += cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_roll[4];
            create_quaternion(q_roll, cam_roll, forward[0], forward[1], forward[2]);
            multiply_two_quaternion(quaternion, quaternion, q_roll);

            quat_to_mat4(R, quaternion);
        }

        //=============================================================================
       
        if(glfwGetKey(window, GLFW_KEY_LEFT))
        {
            cam_pos[0] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            cam_pos[0] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_Q))
        {
            cam_pos[2] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_E))
        {
            cam_pos[2] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_UP))
        {
            cam_pos[1] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN))
        {
            cam_pos[1] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }

        if(cam_moved = true)
        {
            float Translation[16] = {
                1.0f, 0.0f, 0.0f, -cam_pos[0],
                0.0f, 1.0f, 0.0f, -cam_pos[1],
                0.0f, 0.0f, 1.0f, -cam_pos[2],
                0.0f, 0.0f, 0.0f, 1.0f,
            };

            transpose_matrix(Translation);
            
            matrix_multiplite_matrix(view_mat, R, Translation);
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat);
        }

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) 
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void create_quaternion(float (&q)[4], float degrees, float x, float y, float z){
    // q = [s, λ] {s = cos0, λ = sin0}
    // q = [cos(0/2), sin(0/2) * v]     Quay quanh truc V goc 0
    float rad = degrees * (M_PI / 180.0f);

    q[0] = cos(rad / 2.0f);
    q[1] = sin(rad / 2.0f) * x;
    q[2] = sin(rad / 2.0f) * y;
    q[3] = sin(rad / 2.0f) * z;
}
void normalise_quaternion(float (&q)[4])
{
    float sum = pow(q[0], 2) + pow(q[1], 2) + pow(q[2], 2) + pow(q[3], 2);
    if(sum != 1.0f)
    {
        float mag = sqrt(sum);
        for(int i = 0; i < 4; i++)
        {
            q[i] /= mag;
        }
    }
}
void multiply_two_quaternion(float (&result)[4], float q[4], float r[4])
{
    result[0] = r[0] * q[0] - r[1] * q[1] - r[2] * q[2] - r[3] * q[3];
    result[1] = r[0] * q[1] + r[1] * q[0] - r[2] * q[3] + r[3] * q[2];
    result[2] = r[0] * q[2] + r[1] * q[3] + r[2] * q[0] - r[3] * q[1];
    result[3] = r[0] * q[3] - r[1] * q[2] + r[2] * q[1] + r[3] * q[0];
    
    normalise_quaternion(result);
}
void quat_to_mat4(float (&matrix)[16], float q[4]) 
{
    float w = q[0];
    float x = q[1];
    float y = q[2];
    float z = q[3];

    matrix[0]    = 1.0f - 2.0f * y * y - 2.0f * z * z;
    matrix[1]    = 2.0f * x * y + 2.0f * w * z;
    matrix[2]    = 2.0f * x * z - 2.0f * w * y;
    matrix[3]    = 0.0f;
    matrix[4]    = 2.0f * x * y - 2.0f * w * z;
    matrix[5]    = 1.0f - 2.0f * x * x - 2.0f * z * z;
    matrix[6]    = 2.0f * y * z + 2.0f * w * x;
    matrix[7]    = 0.0f;
    matrix[8]    = 2.0f * x * z + 2.0f * w * y;
    matrix[9]    = 2.0f * y * z - 2.0f * w * x;
    matrix[10]   = 1.0f - 2.0f * x * x - 2.0f * y * y;
    matrix[11]   = 0.0f;
    matrix[12]   = 0.0f;
    matrix[13]   = 0.0f;
    matrix[14]   = 0.0f;
    matrix[15]   = 1.0f;

    transpose_matrix(matrix);
}