#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <math.h>

#include "vbo.h"
#include "vao.h"

#define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"

const int gl_width = 640;
const int gl_height = 480;


GLfloat points[] = {
   -0.5f, -0.5f, 0.0f, 
   0.5f, -0.5f, 0.0f, 
   0.5f, 0.5f, 0.0f, 
   0.5f, 0.5f, 0.0f, 
   -0.5f, 0.5f, 0.0f, 
   -0.5f, -0.5f, 0.0f
};
GLfloat texcoords[] = {
   0.0f, 0.0f, 
   1.0f, 0.0f, 
   1.0f, 1.0f, 
   1.0f, 1.0f, 
   0.0f, 1.0f, 
   0.0f, 0.0f
};

int main()
{
   if (!glfwInit ()) {
   fprintf (stderr, "ERROR: could not start GLFW3\n");
   return 1;
   }
   
   GLFWwindow* window = glfwCreateWindow (gl_width, gl_height, "Texture", NULL, NULL);
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

   //VBO
   VBO points_vbo(points, sizeof(points));
   VBO texcoords_vbo(texcoords, sizeof(texcoords));

   // VAO
   VAO vao;
   vao.bindVAO();
   vao.linkAttrib(points_vbo, 0, 3, GL_FLOAT, 0, NULL);
   vao.linkAttrib(texcoords_vbo, 1, 2, GL_FLOAT, 0, NULL);

   //===========================================================

   const char* vertex_shader =
   "#version 460\n"

   "layout(location = 0) in vec3 vertex_position;"
   "layout(location = 1) in vec2 vertex_texture;"
   "out vec2 texture_coordinates;"
   "void main(){"
      "texture_coordinates = vertex_texture;"
      "gl_Position = vec4(vertex_position, 1.0f);"
   "}";

   const char* fragment_shader = 
   "#version 460\n"

   "in vec2 texture_coordinates;"
   "uniform sampler2D basic_texture;"
   "out vec4 frag_colour;"
   "void main(){"
      "frag_colour = texture(basic_texture, texture_coordinates);"
   "}";

   GLuint vs = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vs, 1, &vertex_shader, NULL);
   glCompileShader(vs);

   GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fs, 1, &fragment_shader, NULL);
   glCompileShader(fs);

   //SHADER PROGRAM
   GLuint shader_programme = glCreateProgram();
   glAttachShader(shader_programme, vs);
   glAttachShader(shader_programme, fs);
   glBindAttribLocation(shader_programme, 0, "vertex_position");
   glBindAttribLocation(shader_programme, 1, "vertex_texture");
   glLinkProgram(shader_programme);

   //TEXTURE
   int width_image, height_image, n;
   stbi_set_flip_vertically_on_load(true);
   unsigned char* image_data = stbi_load("Lobuart.png", &width_image, &height_image, &n, 4);
   if(!image_data) 
   {
      std::cerr << "ERROR: could not load " << image_data << '\n';
      return 1;
   }

   GLuint tex = 0;
   glGenTextures(1, &tex);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, tex);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_image, height_image, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
   glGenerateMipmap(GL_TEXTURE_2D);

   // S, T, R <=> x, y, z axis
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
   stbi_image_free(image_data);
   glBindTexture(GL_TEXTURE_2D, 0);

   while(!glfwWindowShouldClose(window))
   {
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      glUseProgram(shader_programme);
      vao.bindVAO();
      
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindTexture(GL_TEXTURE_2D, tex);
      glfwPollEvents();
      glfwSwapBuffers(window);
   }

   points_vbo.deleteVBO();
   texcoords_vbo.deleteVBO();
   glDeleteTextures(1, &tex);
   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}  