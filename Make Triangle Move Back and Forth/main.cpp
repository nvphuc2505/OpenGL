#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

int main()
{
    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
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


    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &points, GL_STATIC_DRAW);

    GLuint colours_vbo = 0;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &colours, GL_STATIC_DRAW);

    //VAO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Vertex Shader
    const char* vertex_shader =  
    "#version 400\n"

    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_colour;"

    "uniform mat4 matrix;"

    "out vec3 colour;"

    "void main(){"
        "colour = vertex_colour;"
        "gl_Position = matrix * vec4(vertex_position, 1.0f);"
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

    // Translate Matrix
    float matrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f, // first column
        0.0f, 1.0f, 0.0f, 0.0f, // second column
        0.0f, 0.0f, 1.0f, 0.0f, // third column
        0.5f, 0.0f, 0.0f, 1.0f // fourth column
    };

    int matrix_location = glGetUniformLocation(shader_programme, "matrix");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

    float speed = 1.0f;
    float last_position = 0.0f;

    while(!glfwWindowShouldClose(window))
    {
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        // std::cout << "previous_seconds: " << previous_seconds << "\n";
        // std::cout << "current_seconds: " << current_seconds << "\n";
        // std::cout << "elapsed_seconds: " << elapsed_seconds << "\n";

        if(fabs(last_position) > 1.0f)
        {
            speed = -speed;
        }

        matrix[12] = elapsed_seconds * speed + last_position;
        last_position = matrix[12];
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