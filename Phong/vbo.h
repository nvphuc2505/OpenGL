class VBO
{
    private:
        GLuint m_ID;
    public:
        VBO(GLfloat*, GLsizeiptr);
        void bindVBO();
        void clearVBO();
};

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &m_ID);
    bindVBO();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VBO::bindVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}
void VBO::clearVBO()
{
    glDeleteBuffers(1, &m_ID);
}