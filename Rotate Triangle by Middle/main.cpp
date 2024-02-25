#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <math.h>

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

    GLuint points_vbo;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, &points, GL_STATIC_DRAW);

    GLuint colours_vbo;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, &colours, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    const char* vertex_shader = 
    "#version 400\n"

    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_colour;"

    "uniform mat4 matrix;"

    "out vec3 colour;"

    "void main()"
    "{"
        "colour = vertex_colour;"
        "gl_Position = matrix * vec4(vertex_position, 1.0f);"
    "}";

    const char* fragment_shader =
    "#version 400\n"

    "in vec3 colour;"
    "out vec4 frag_colour;"

    "void main()"
    "{"
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

    float matrix[] {
        pow(cos(45), 2),                                cos(45)*sin(45),                            -sin(45),           0,
        pow(sin(45), 2)*cos(45) - cos(45)*sin(45),      pow(sin(45), 3) + pow(cos(45), 2),          sin(45)*cos(45),    0,
        pow(cos(45), 2)*sin(45) + pow(sin(45), 2),      cos(45)*pow(sin(45), 2) - sin(45)*cos(45),  pow(cos(45), 2),    0,
        0.5f,                                              0,                                          0,               1
    };

    int matrix_location = glGetUniformLocation(shader_programme, "matrix");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

    // Data of Rotation
    float rotate_speed = 1.0f;
    float last_rotation = 0.0f;

    // Data of Translation
    float translate_speed = 0.5f;
    float last_translation = 0.0f;
    
    while(!glfwWindowShouldClose(window))
    {
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        double tmp = rotate_speed * elapsed_seconds + last_rotation;

        matrix[0] = pow(cos(tmp), 2);
        matrix[1] = cos(tmp)*sin(tmp);
        matrix[2] = -sin(tmp);
        matrix[4] = pow(sin(tmp), 2)*cos(tmp) - cos(tmp)*sin(tmp);
        matrix[5] = pow(sin(tmp), 3) + pow(cos(tmp), 2);
        matrix[6] = sin(tmp)*cos(tmp);
        matrix[8] = pow(cos(tmp), 2)*sin(tmp) + pow(sin(tmp), 2);
        matrix[9] = cos(tmp)*pow(sin(tmp), 2) - sin(tmp)*cos(tmp);
        matrix[10] = pow(cos(tmp), 2);
        matrix[12] = translate_speed * elapsed_seconds + last_translation;
        
        if(fabs(last_translation) > 1.0f)
        {
            translate_speed = -translate_speed;
        }

        last_translation = translate_speed * elapsed_seconds + last_translation;
        last_rotation = tmp;

        glUseProgram(shader_programme);
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}