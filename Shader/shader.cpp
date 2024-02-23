#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

void _print_shader_info_log(GLuint shader_index)
{
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetShaderInfoLog(shader_index, maxLength, &actualLength, log);
    std::cout << "Shader info log for GL index " << shader_index << "\n" << log << "\n";
}

void _print_programme_info_log(GLuint programme)
{
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetProgramInfoLog(programme, maxLength, &actualLength, log); 
    std::cout << "Program info log for GL index " << programme << "\n" << log << "\n";
}


bool _is_validate(GLuint programme)
{
    glValidateProgram(programme);

    int params = -1;
    glGetProgramiv(programme, GL_VALIDATE_STATUS, &params);
    std::cout << "Program " << programme << " GL_VALIDATE_STATUS = " << params << "\n";
    if(GL_TRUE != params)
    {
        _print_programme_info_log(programme);
        return false;
    }
    return true;
}


int main()
{
    GLfloat points[] = {
         0.0f,  0.5f,  0.0f, 
         0.5f, -0.5f,  0.0f, 
        -0.5f, -0.5f,  0.0f
    };




    if (!glfwInit ()) {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
    }
    
    GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Triangle", NULL, NULL);
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











    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_VERTEX_ARRAY, vbo);
    glBufferData(GL_VERTEX_ARRAY, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_VERTEX_ARRAY, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    



    const char* vertex_shader = 
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
        "gl_Position = vec4(vp, 1.0f);"
    "}";

    const char* fragment_shader = 
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
        "frag_colour = vec4(0.5f, 0.0f, 0.5f, 1.0f);"
    "}";

    // Load * Compile Shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

     // Check for compile Errors (Right after calling glCompileShader)
    int params = -1;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
    if(GL_TRUE != params)
    {
        std::cerr << "ERROR: GL shader index " << fs << " did not compile\n";
        _print_shader_info_log(fs);
        return 1;
    }

    // Link Shader
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);

    
    // Check for Linking Errors
    glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
    if(GL_TRUE != shader_programme)
    {
        std::cerr << "ERROR: could not link shader programme GL index " << shader_programme << "\n";
        _print_programme_info_log(shader_programme);
        return 1;
    }

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}