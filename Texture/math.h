struct matrix4f
{
    float m[16];

    matrix4f();
    matrix4f(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    matrix4f operator=(matrix4f);
};

matrix4f::matrix4f()
{
    for(int i = 0; i < 16; i++)
    {
        this->m[i] = 0.0f;
    }
}
matrix4f::matrix4f(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
{
    this->m[0]  = a;    this->m[1]  = e;    this->m[2]  = i;    this->m[3]  = m;
    this->m[4]  = b;    this->m[5]  = f;    this->m[6]  = j;    this->m[7]  = n;
    this->m[8]  = c;    this->m[9]  = g;    this->m[10] = k;    this->m[11] = o;
    this->m[12] = d;    this->m[13] = h;    this->m[14] = l;    this->m[15] = p;
}
matrix4f matrix4f::operator=(matrix4f other)
{
    for(int i = 0; i < 16; i++)
    {
        this->m[i] = other.m[i];
    }
    return *this;
}