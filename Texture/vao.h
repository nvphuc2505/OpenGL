class VAO
{
    private:
        GLuint m_ID;
    public:
        VAO();
        void bindVAO();
        void linkAttrib(VBO&, GLuint, GLuint, GLenum, GLsizeiptr, void*);
};

VAO::VAO()
{
    glGenVertexArrays(1, &m_ID);
    bindVAO();
}
void VAO::bindVAO()
{
    glBindVertexArray(m_ID);
}
void VAO::linkAttrib(VBO& vbo, GLuint layout, GLuint numberOfComponents, GLenum type, GLsizeiptr stride, void* offset)
{
    vbo.bindVBO();
    glVertexAttribPointer(layout, numberOfComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
}