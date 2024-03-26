class VBO
{
    private:
        GLuint m_ID;
    public:
        VBO(GLfloat*, GLsizeiptr);
        void bindVBO();
        void deleteVBO();
};

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VBO::bindVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
void VBO::deleteVBO()
{
    glDeleteBuffers(1, &m_ID);
}