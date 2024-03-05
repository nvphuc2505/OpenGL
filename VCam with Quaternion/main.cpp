/*
controls:                                                                    |
| pitch = up,down arrow keys                                                   |
| yaw = left,right arrow keys                                                  |
| roll = z,c keys                                                              |
| move forward/back = w,s keys                                                 |
| move left/right = a,d keys   
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <math.h>

#include "quaternion.h"
#include "maths_funcs.h"

#define  VERTEX_SHADER_FILE  "test_vs.glsl"
#define FRAGMENT_SHADER_FILE "test_fs.glsl"
#define NUM_SPHERES 4

// a world position for each sphere in the scene
// float v1[3] = {-2.0f, 0.0f, 0.0f};
// float v2[3] = {2.0f, 0.0f, 0.0f};
// float v3[3] = {-2.0f, 0.0f, -2.0f};
// float v4[3] = {1.5f, 1.0f, -1.0f};
// float* sphere_pos_wor[4] = {v1, v2, v3, v4};

int g_gl_width  = 640;
int g_gl_height = 480;

int main()
{
    GLfloat points[] = {
        0.0f, 0.3f, 0.0f,
        0.3f, -0.3f, 0.0f,
        -0.3f, -0.3f, 0.0f
    };

    GLfloat colours[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    }; 

    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    GLFWwindow* window = glfwCreateWindow (g_gl_width, g_gl_height, "Triangle", NULL, NULL);
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

    // Vertex Buffer Object & Vertex Attribute Object
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

    
    // Shader
    const char* vertex_shader = 
    "#version 400\n"

    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_colour;"

    "uniform mat4 view;"
    "uniform mat4 proj;"

    "out vec3 colour;"
    "void main() {"
        "colour = vertex_colour;"
        "gl_Position = view * proj * vec4(vertex_position, 1.0f);"
    "}";

    const char* fragment_shader = 
    "#version 400\n"

    "in vec3 colour;"
    "out vec4 frag_colour;"

    "void main(){"
        "frag_colour = vec4(colour, 1.0f);"
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
    glBindAttribLocation(shader_programme, 1, "vertex_colour");

    glLinkProgram(shader_programme);


    // Project Matrix
    float near = 0.1f;
    float far = 100.0f;
    float fov = 67.0f * (M_PI / 180.0f);
    float aspect = (float)g_gl_width / (float)g_gl_height;


    float range = tanf(fov * 0.5f) * near;
    float Sx = (2.0f * near) / ((range * aspect + range * aspect));
    float Sy = near / range;
    float Sz = -(far + near) / (far - near);
    float Pz = -(2.0f * far * near) / (far - near);

    float proj_mat[16] = {
        Sx, 0.0f, 0.0f, 0.0f,
        0.0f, Sy, 0.0f, 0.0f,
        0.0f, 0.0f, Sz, Pz,
        0.0f, 0.0f, 1.0f, 0.0f
    };

    int proj_mat_location = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat);


    // View Matrix
    float cam_speed = 5.0f;
    float cam_pos[3] = {0.0f, 0.0f, 2.0f};
    float T[16] = {
        1.0f, 0.0f, 0.0f, -cam_pos[0],
        0.0f, 1.0f, 0.0f, -cam_pos[1],
        0.0f, 0.0f, 1.0f, -cam_pos[2],
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float cam_heading_speed = 100.0f;
    float cam_heading = 0.0f;
    float R[16];
    float quaternion[4];
    _create_versor(quaternion, -cam_heading, 0.0f, 1.0f, 0.0f);
    _versor_to_matrix(quaternion, R);

    float view_mat[16];
    _multiplication_matrix(view_mat, R, T); // V = T x R (ROW-MAJOR)
    
    int view_mat_location = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat);

    // Keep track of some useful vectors that can be used for keyboard movement
    float forward[4] {0.0f, 0.0f, -1.0f, 0.0f};
    float right[4]   {1.0f, 0.0f, 0.0f, 0.0f};
    float up[4]      {0.0f, 1.0f, 0.0f, 0.0f};


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
        float move[3] {0.0f, 0.0f, 0.0f};
        if(glfwGetKey(window, GLFW_KEY_A))
        {
            move[0] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_D))
        {
            move[0] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_Q))
        {
            move[1] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_E))
        {
            move[1] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_W))
        {
            move[2] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if(glfwGetKey(window, GLFW_KEY_S))
        {
            move[2] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }

        float cam_yaw = 0.0f;
        if(glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            cam_yaw -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_yaw[4];
            _create_versor(q_yaw, cam_yaw, up[0], up[1], up[2]);
            _multiplication_two_versor(quaternion, q_yaw, quaternion);
            _versor_to_matrix(quaternion, R);

            float v1[4] {0.0, 0.0, -1.0, 0.0};
            float v2[4] {1.0, 0.0, 0.0, 0.0};
            float v3[4] {0.0, 1.0, 0.0, 0.0};

            _matrix_multiply_vector(forward, R, v1);
            _matrix_multiply_vector(right, R, v2);
            _matrix_multiply_vector(up, R, v3);
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT))
        {
            cam_yaw += cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_yaw[4];
            _create_versor(q_yaw, cam_yaw, up[0], up[1], up[2]);
            _multiplication_two_versor(quaternion, q_yaw, quaternion);
            _versor_to_matrix(quaternion, R);

            float v1[4] {0.0, 0.0, -1.0, 0.0};
            float v2[4] {1.0, 0.0, 0.0, 0.0};
            float v3[4] {0.0, 1.0, 0.0, 0.0};

            _matrix_multiply_vector(forward, R, v1);
            _matrix_multiply_vector(right, R, v2);
            _matrix_multiply_vector(up, R, v3);
        }

        float cam_pictch = 0.0f;
        if(glfwGetKey(window, GLFW_KEY_UP))
        {
            cam_pictch += cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_pitch[4];
            _create_versor(q_pitch, cam_pictch, right[0], right[1], right[2]);
            _multiplication_two_versor(quaternion, q_pitch, quaternion);

            _versor_to_matrix(quaternion, R);

            float v1[4] {0.0, 0.0, -1.0, 0.0};
            float v2[4] {1.0, 0.0, 0.0, 0.0};
            float v3[4] {0.0, 1.0, 0.0, 0.0};

            _matrix_multiply_vector(forward, R, v1);
            _matrix_multiply_vector(right, R, v2);
            _matrix_multiply_vector(up, R, v3);
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN))
        {
            cam_pictch -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_pitch[4];
            _create_versor(q_pitch, cam_pictch, right[0], right[1], right[2]);
            _multiplication_two_versor(quaternion, q_pitch, quaternion);
            
            _versor_to_matrix(quaternion, R);

            float v1[4] {0.0, 0.0, -1.0, 0.0};
            float v2[4] {1.0, 0.0, 0.0, 0.0};
            float v3[4] {0.0, 1.0, 0.0, 0.0};

            _matrix_multiply_vector(forward, R, v1);
            _matrix_multiply_vector(right, R, v2);
            _matrix_multiply_vector(up, R, v3);
        }
        
        float cam_roll = 0.0f;
        if(glfwGetKey(window, GLFW_KEY_Z))
        {
            cam_roll -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_roll[4];
            _create_versor(q_roll, cam_roll, forward[0], forward[1], forward[2]);
            _multiplication_two_versor(quaternion, q_roll, quaternion);
            
            _versor_to_matrix(quaternion, R);

            float v1[4] {0.0, 0.0, -1.0, 0.0};
            float v2[4] {1.0, 0.0, 0.0, 0.0};
            float v3[4] {0.0, 1.0, 0.0, 0.0};

            _matrix_multiply_vector(forward, R, v1);
            _matrix_multiply_vector(right, R, v2);
            _matrix_multiply_vector(up, R, v3);
        }
        if(glfwGetKey(window, GLFW_KEY_Z))
        {
            cam_roll += cam_heading_speed * elapsed_seconds;
            cam_moved = true;

            float q_roll[4];
            _create_versor(q_roll, cam_roll, forward[0], forward[1], forward[2]);
            _multiplication_two_versor(quaternion, q_roll, quaternion);
            
            _versor_to_matrix(quaternion, R);

            float v1[4] {0.0, 0.0, -1.0, 0.0};
            float v2[4] {1.0, 0.0, 0.0, 0.0};
            float v3[4] {0.0, 1.0, 0.0, 0.0};

            _matrix_multiply_vector(forward, R, v1);
            _matrix_multiply_vector(right, R, v2);
            _matrix_multiply_vector(up, R, v3);
        }

        if(cam_moved)
        {
            _versor_to_matrix(quaternion, R);

            _recreate_vector(cam_pos, forward, move[2]);
            _recreate_vector(cam_pos, up, move[1]);
            _recreate_vector(cam_pos, right, move[0]);

            float T[16] = {
                1.0f, 0.0f, 0.0f, -cam_pos[0],
                0.0f, 1.0f, 0.0f, -cam_pos[1],
                0.0f, 0.0f, 1.0f, -cam_pos[2],
                0.0f, 0.0f, 0.0f, 1.0f
            };

            _inverse_matrix(T);
            _inverse_matrix(R);
            _multiplication_matrix(view_mat, R, T);

            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat);
        }

        //if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1); 

        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}