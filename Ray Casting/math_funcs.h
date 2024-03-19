

struct vec3
{
    float v[3];

    vec3();
    vec3(float, float, float);
    //Operator
    vec3 operator+(const vec3&);
    vec3 operator-(const vec3&);
    vec3 operator*(float);
    vec3 operator/(float);
    vec3 operator=(const vec3&);
};

vec3::vec3() 
{
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
}
vec3::vec3(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}
vec3 vec3::operator+(const vec3& other)
{
    vec3 tmp;
    tmp.v[0] = v[0] + other.v[0];
    tmp.v[1] = v[1] + other.v[1];
    tmp.v[2] = v[2] + other.v[2];
    return tmp;
}
vec3 vec3::operator-(const vec3& other)
{
    vec3 tmp;
    tmp.v[0] = v[0] - other.v[0];
    tmp.v[1] = v[1] - other.v[1];
    tmp.v[2] = v[2] - other.v[2];
    return tmp;
}
vec3 vec3::operator*(float scalar)
{
    vec3 tmp;
    tmp.v[0] = v[0] * scalar;
    tmp.v[1] = v[1] * scalar;
    tmp.v[2] = v[2] * scalar;
    return tmp;
}
vec3 vec3::operator=(const vec3& other)
{
    v[0] = other.v[0];
    v[1] = other.v[1];
    v[2] = other.v[2];
    return *this;
}
vec3 vec3::operator/(float k)
{
    for(int i = 0; i < 3; i++)
    {
        v[i] = v[i] / k;
    }
    return *this;
}




struct vec4
{
    float v[4];

    vec4();
    vec4(float, float, float, float);
    vec4(vec3, float);
    vec4 operator = (vec4);
};

vec4::vec4() {}
vec4::vec4(float x, float y, float z, float w)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}
vec4::vec4(vec3 v3, float w)
{
    v[0] = v3.v[0];
    v[1] = v3.v[1];
    v[2] = v3.v[2];
    v[3] = w;
}
vec4 vec4::operator = (vec4 tmp)
{
    for(int i = 0; i < 4; i++)
    {
        v[i] = tmp.v[i];
    }
    return *this;
}