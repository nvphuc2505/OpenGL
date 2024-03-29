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

glm::vec3 get_ray_form_mouse(float mouse_x, float mouse_y)
{
    // NORMALISED DEVICE SPACE
    float x = (2.0f * mouse_x) / g_gl_width - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / g_gl_height;
    float z = 1.0f;
    glm::vec3 ray_NDC(x, y, z);

    // HOMOGENEOUS CLIP SPACE
    z = -1.0f;
    float w = 1.0f;
    glm::vec4 ray_HCL(ray_NDC.x, ray_NDC.y, z, w);

    // EYE SPACE
    glm::vec4 ray_ES = glm::inverse(proj_mat) * ray_HCL;
    ray_ES = glm::vec4(ray_ES.x, ray_ES.y, -1.0f, 0.0f);

    // WORLD SPACE
    glm::vec3 tmp = glm::inverse(view_mat) * ray_ES;
    glm::vec3 ray_WS(tmp.x, tmp.y, tmp.z);
    return glm::normalize(ray_WS);
}
void glfw_mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(GLFW_PRESS == action)
    {
        // Take the Position of Cursor
        double xPos;
        double yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        glm::vec3 ray_WS = get_ray_form_mouse(xPos, yPos);
        std::cout << ray_WS.x << " " << ray_WS.y << " " << ray_WS.z << '\n';

        
    }
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

    // Vertex Shader & Fragment Shader
    const char* vertex_shader = 
    "#version 460\n"

    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_normal;"

    "out vec3 position_eye, normal_eye;"

    "uniform mat4 model;"
    "uniform mat4 proj;"
    "uniform mat4 view;"

    "void main(){"
        "position_eye = vec3(view * model * vec4(vertex_position, 1.0f));"
        "normal_eye   = vec3(view * model * vec4(vertex_normal, 0.0f));"
        "gl_Position  = proj * vec4(position_eye, 1.0f);"
    "}";

    const char* fragment_shader = 
    "#version 460\n"

    "in vec3 normal_eye;"
    "in vec3 position_eye;"

    "uniform mat4 view_mat;"

    "vec3 light_position_world = vec3(0.0f, 0.0f, 2.0f);"
    "vec3 Ld = vec3(0.7f, 0.7f, 0.7f);"
    "vec3 Ls = vec3(1.0f, 1.0f, 1.0f);"
    "vec3 La = vec3(0.2f, 0.2f, 0.2f);"
    
    "vec3 Kd = vec3(1.0f, 0.5f, 0.0f);"
    "vec3 Ks = vec3(1.0f, 1.0f, 1.0f);"
    "vec3 Ka = vec3(1.0f, 1.0f, 1.0f);"
    "float specular_exponent = 100.0f;"

    "out vec4 frag_colour;"

    "void main(){"
        // Diffuse light intensity Id
        "vec3 light_position_eye = vec3(view_mat * vec4(light_position_world, 1.0f));"
        "vec3 distance_to_light_eye = light_position_eye - position_eye;"
        "vec3 direction_to_light_eye = normalize(distance_to_light_eye);"
        "float dot_prod = dot(direction_to_light_eye, normal_eye);"
        "dot_prod = max(dot_prod, 0.0f);"
        "vec3 Id = Ld * Kd * dot_prod;"

        // Specular light intensity (Is)
        "vec3 surface_to_viewer_eye = normalize(-position_eye);"

        // "vec3 reflection_eye = reflect(-direction_to_light_eye, normal_eye);"
        // "float dot_prod_specular = dot(reflection_eye, surface_to_viewer_eye);"
        // "dot_prod_specular = max(dot_prod_specular, 0.0f);"
        // "float specular_factor = pow(dot_prod_specular, specular_exponent);"

        // Blinn Phong
        "vec3 half_way_eye = normalize(surface_to_viewer_eye + direction_to_light_eye);"
        "float dot_prod_specular = max(dot(half_way_eye, normal_eye), 0.0f);"
        "float specular_factor = pow(dot_prod_specular, specular_exponent);"
        "vec3 Is = Ls * Ks * specular_factor;"

        "vec3 Ia = La * Ka;"
        
        "frag_colour = vec4(Id + Is + Ia, 1.0f);"
    "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
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