class VAO
{
    private:
        GLuint m_ID;
    public:
        VAO();
        void bindVAO();
        void setUpAttrib(VBO&, GLuint, GLint, GLenum, GLsizeiptr, void*);
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
void VAO::setUpAttrib(VBO& vbo, GLuint index, GLint number_of_component, GLenum type, GLsizeiptr stride, void* offset)
{
    vbo.bindVBO();
    glVertexAttribPointer(index, number_of_component, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}