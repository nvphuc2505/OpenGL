#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

GLFWwindow* window = NULL;

int main () 
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

    
    // Initialize ID of VBO
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, points, GL_STATIC_DRAW);

    // Initialize ID of VBO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    
    // Shader 
    const char* vertex_shader = 
    "#version 400\n"
    "in vec3 vp;"

    "uniform mat4 proj;"
    "uniform mat4 view;"

    "void main() {"
        "gl_Position = view * proj * vec4(vp, 1.0f);"
    "}";

    const char* fragment_shader = 
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
        "frag_colour = vec4(0.5f, 0.0f, 0.5f, 1.0f);"
    "}";

    // Load and Compile Shader Objects
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);



    //-------------------------CREATE VIRTUAL CAMERA---------------------------------
    float cam_speed = 1.0f;
    float cam_yaw_speed = 10.0f;


    //--------------------------------VIEW MATRIX------------------------------------
    /* MATRIX COMPONENTS OF VIEW MATRIX */
    float cam_pos[] = {0.0f, 0.0f, 2.0f}; 
    float cam_yaw = 0.0f;       // Y-rotation in degrees

    float view_mat[] = {
        cos(-cam_yaw),  0.0f,       sin(-cam_yaw),          -cam_pos[0] * cos(-cam_yaw) + sin(-cam_yaw) * -cam_pos[2],
        0.0f,           1.0f,       0.0f,                   -cam_pos[1],
        -sin(-cam_yaw), 0.0f,       cos(-cam_yaw),          -sin(-cam_yaw) * -cam_pos[1] + cos(-cam_yaw) * -cam_pos[2],
        0.0f,           0.0f,       0.0f,                    1.0f
    };

    int view_mat_location = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat);


    //--------------------------------PROJECTION MATRIX------------------------------------

    float near = 0.1f;                      // Near clipping plane
    float far = 100.0f;                     // Far clipping plane
    float fov = (67.0f * M_PI) / 180.0f;    // Field of View by Radian
    float aspect = 640.0f / 480.0f;         // Aspect Ratio

    /* MATRIX COMPONENTS OF PROJECTION MATRIX */
    float range = tan(fov * 0.5f) * near;
    float Sx = (2.0f * near) / (range * aspect + range * aspect);
    float Sy = near / range;
    float Sz = - (far + near) / (far - near);
    float Pz = - (2.0f * far * near) / (far - near);

    float proj_mat[] = {
        Sx, 0.0f, 0.0f, 0.0f,
        0.0f, Sy, 0.0f, 0.0f,
        0.0f, 0.0f, Sz, Pz,
        0.0f, 0.0f, -1.0f, 0.0f
    };

    int proj_mat_location = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat);

    

    


    while(!glfwWindowShouldClose(window))
    {
        // Time
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;
        
        bool can_move = false;
        if(glfwGetKey(window, GLFW_KEY_A))
        {
            cam_pos[0] -= cam_speed * elapsed_seconds;
            can_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_D))
        {
            cam_pos[0] += cam_speed * elapsed_seconds;
            can_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_UP))
        {
            cam_pos[1] += cam_speed * elapsed_seconds;
            can_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN))
        {
            cam_pos[1] -= cam_speed * elapsed_seconds;
            can_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_W))
        {
            cam_pos[2] -= cam_speed * elapsed_seconds;
            can_move = true;
        }
        if(glfwGetKey(window, GLFW_KEY_S))
        {
            cam_pos[2] += cam_speed * elapsed_seconds;
            can_move = true;
        }
        if (glfwGetKey (window, GLFW_KEY_LEFT)) {
            cam_yaw += cam_yaw_speed * elapsed_seconds;
            can_move = true;
        }
        if (glfwGetKey (window, GLFW_KEY_RIGHT)) {
            cam_yaw -= cam_yaw_speed * elapsed_seconds;
            can_move = true;
        }

        // Update View Matrix
        if(can_move == true)
        {
            view_mat[0] = cos(-cam_yaw);
            view_mat[2] = sin(-cam_yaw);
            view_mat[3] = -cam_pos[0] * cos(-cam_yaw) + sin(-cam_yaw) * -cam_pos[2];
            view_mat[7] = -cam_pos[1];
            view_mat[8] = -sin(-cam_yaw);
            view_mat[10] = cos(-cam_yaw);
            view_mat[11] = -sin(cam_yaw) * -cam_pos[1] + cos(-cam_yaw) * -cam_pos[2];
        
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat);
        }


        // 6 steps
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