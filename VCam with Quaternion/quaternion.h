void _create_versor(float (&q)[4], float degree, float x, float y, float z)
{
    float radian = (degree * M_PI) / 180.0f;

    q[0] = cosf(radian / 2.0f);
    q[1] = sinf(radian / 2.0f) * x;
    q[2] = sinf(radian / 2.0f) * y;
    q[3] = sinf(radian / 2.0f) * z;
}

void _normalise_versor(float (&q)[4])
{
    // float sum = pow(q[0], 2) + pow(q[1], 2) + pow(q[2], 2) + pow(q[3], 2);
    // if(sum <= 0.0f) return;

    // float length_of_versor = sqrt(sum);

    // for(int i = 0; i < 4; i++)
    // {
    //     q[i] = q[i] / length_of_versor;
    // }
    float sum = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];

    const float thresh = 0.0001f;
    if ( fabs( 1.0f - sum ) < thresh ) { return; }
    float mag = sqrt( sum );
    for ( int i = 0; i < 4; i++ ) { q[i] = q[i] / mag; }
}

void _multiplication_two_versor(float (&result)[4], float q[4], float r[4])
{
    result[0] = r[0] * q[0] - r[1] * q[1] - r[2] * q[2] - r[3] * q[3];
    result[1] = r[0] * q[1] + r[1] * q[0] - r[2] * q[3] + r[3] * q[2];
    result[2] = r[0] * q[2] + r[1] * q[3] + r[2] * q[0] - r[3] * q[1];
    result[3] = r[0] * q[3] - r[1] * q[2] + r[2] * q[1] + r[3] * q[0];
    
    _normalise_versor(result);
}

void _transposition_matrix(float matrix[16], float (&result)[16])
{
    result[0]  = matrix[0];
    result[1]  = matrix[4];
    result[2]  = matrix[8];
    result[3]  = matrix[12];
    result[4]  = matrix[1];
    result[5]  = matrix[5];
    result[6]  = matrix[9];
    result[7]  = matrix[13];
    result[8]  = matrix[2];
    result[9]  = matrix[6];
    result[10] = matrix[10];
    result[11] = matrix[14];
    result[12] = matrix[3];
    result[13] = matrix[7];
    result[14] = matrix[11];
    result[15] = matrix[15];
}

void _versor_to_matrix(float q[4], float (&mat)[16])
{
    float w = q[0];
    float x = q[1];
    float y = q[2];
    float z = q[3];

    //float mat[16];
    mat[0]  = 1.0f - 2.0f * pow(y, 2) - 2.0f * pow(z, 2);
    mat[1]  = 2.0f * x * y - 2.0f * w * z;
    mat[2]  = 2.0f * x * z + 2.0f * w * y;
    mat[3]  = 0.0f;
    mat[4]  = 2.0f * x * y + 2 * w * z;
    mat[5]  = 1.0f - 2.0f * pow(x, 2) - 2.0f * pow(z, 2);
    mat[6]  = 2.0f * y * z - 2.0f * w * x;
    mat[7]  = 0.0f;
    mat[8]  = 2.0f * x * z - 2.0f * w * y;
    mat[9]  = 2.0f * y * z + 2.0f * w * x;
    mat[10] = 1.0f - 2.0f * pow(x, 2) - 2.0f * pow(y, 2);
    mat[11] = 0.0f;
    mat[12] = 0.0f;
    mat[13] = 0.0f;
    mat[14] = 0.0f;
    mat[15] = 1.0f;

    //_transposition_matrix(mat, result);
}

void _multiplication_matrix(float (&c)[16], float a[16], float b[16])
{ 
    c[0]  = a[0]  * b[0] + a[1]  * b[4] + a[2]  * b[8]  + a[3]  * b[12];
    c[1]  = a[0]  * b[1] + a[1]  * b[5] + a[2]  * b[9]  + a[3]  * b[13];
    c[2]  = a[0]  * b[2] + a[1]  * b[6] + a[2]  * b[10] + a[3]  * b[14];
    c[3]  = a[0]  * b[3] + a[1]  * b[7] + a[2]  * b[11] + a[3]  * b[15];
    c[4]  = a[4]  * b[0] + a[5]  * b[4] + a[6]  * b[8]  + a[7]  * b[12];
    c[5]  = a[4]  * b[1] + a[5]  * b[5] + a[6]  * b[9]  + a[7]  * b[13];
    c[6]  = a[4]  * b[2] + a[5]  * b[6] + a[6]  * b[10] + a[7]  * b[14];
    c[7]  = a[4]  * b[3] + a[5]  * b[7] + a[6]  * b[11] + a[7]  * b[15];
    c[8]  = a[8]  * b[0] + a[9]  * b[4] + a[10] * b[8]  + a[11] * b[12];
    c[9]  = a[8]  * b[1] + a[9]  * b[5] + a[10] * b[9]  + a[11] * b[13];
    c[10] = a[8]  * b[2] + a[9]  * b[6] + a[10] * b[10] + a[11] * b[14];
    c[11] = a[8]  * b[3] + a[9]  * b[7] + a[10] * b[11] + a[11] * b[15];
    c[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8]  + a[15] * b[12];
    c[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9]  + a[15] * b[13];
    c[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
    c[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}

void _matrix_multiply_vector(float (&result)[4], float matrix[16], float v[4])
{
    result[0] = matrix[0]  * v[0] + matrix[1]  * v[1] + matrix[2]  * v[2] + matrix[3]  * v[3];
    result[1] = matrix[4]  * v[0] + matrix[5]  * v[1] + matrix[6]  * v[2] + matrix[7]  * v[3];
    result[2] = matrix[8]  * v[0] + matrix[9]  * v[1] + matrix[10] * v[2] + matrix[11] * v[3];
    result[3] = matrix[12] * v[0] + matrix[13] * v[1] + matrix[14] * v[2] + matrix[15] * v[3];
}

void _recreate_vector(float (&cam_pos)[3], float vec3[4], float scales)
{
    //cam_pos = cam_pos + vec3( fwd ) * -move.v[2];
    cam_pos[0] += vec3[0] - scales;
    cam_pos[1] += vec3[1] - scales;
    cam_pos[2] += vec3[2] - scales;
}

void _inverse_matrix(float (&matrix)[16])
{
    float detA = matrix[0] * ((matrix[5] * matrix[10] * matrix[15] + matrix[9] * matrix[14] * matrix[7] + matrix[6] * matrix[11] * matrix[13]) - (matrix[7] * matrix[10] * matrix[13] + matrix[5] * matrix[14] * matrix[11] + matrix[6] * matrix[15] * matrix[9])) 
               - matrix[1] * ((matrix[4] * matrix[10] * matrix[15] + matrix[8] * matrix[14] * matrix[7] + matrix[6] * matrix[11] * matrix[12]) - (matrix[7] * matrix[10] * matrix[12] + matrix[11] * matrix[14] * matrix[4] + matrix[6] * matrix[8] * matrix[15]))
               + matrix[2] * ((matrix[4] * matrix[9] * matrix[15] + matrix[8] * matrix[13] * matrix[7] + matrix[5] * matrix[11] * matrix[12]) - (matrix[7] * matrix[9] * matrix[12] + matrix[4] * matrix[13] * matrix[11] + matrix[5] * matrix[15] * matrix[8]))
               - matrix[3] * ((matrix[4] * matrix[9] * matrix[14] + matrix[8] * matrix[13] * matrix[6] + matrix[5] * matrix[10] * matrix[12]) - (matrix[6] * matrix[9] * matrix[12] + matrix[4] * matrix[13] * matrix[10] + matrix[5] * matrix[14] * matrix[8]));

    if(detA == 0) 
    {
        std::cerr << "The matrix has not inverse matrix!\n";
        return;
    }

    float tmp_matrix[16];
    tmp_matrix[0]  = ((matrix[5] * matrix[10] * matrix[15] + matrix[9] * matrix[14] * matrix[7] + matrix[6] * matrix[11] * matrix[13]) - (matrix[7] * matrix[10] * matrix[13] + matrix[5] * matrix[14] * matrix[11] + matrix[6] * matrix[15] * matrix[9]));
    tmp_matrix[1]  = ((matrix[4] * matrix[10] * matrix[15] + matrix[8] * matrix[14] * matrix[7] + matrix[6] * matrix[11] * matrix[12]) - (matrix[7] * matrix[10] * matrix[12] + matrix[11] * matrix[14] * matrix[4] + matrix[6] * matrix[8] * matrix[15]));
    tmp_matrix[2]  = ((matrix[4] * matrix[9] * matrix[15] + matrix[8] * matrix[13] * matrix[7] + matrix[5] * matrix[11] * matrix[12])  - (matrix[7] * matrix[9] * matrix[12] + matrix[4] * matrix[13] * matrix[11] + matrix[5] * matrix[15] * matrix[8]));
    tmp_matrix[3]  = ((matrix[4] * matrix[9] * matrix[14] + matrix[8] * matrix[13] * matrix[6] + matrix[5] * matrix[10] * matrix[12])  - (matrix[6] * matrix[9] * matrix[12] + matrix[4] * matrix[13] * matrix[10] + matrix[5] * matrix[14] * matrix[8]));
    tmp_matrix[4]  = ((matrix[1] * matrix[10] * matrix[15] + matrix[9] * matrix[14] * matrix[3] + matrix[2] * matrix[11] * matrix[13]) - (matrix[3] * matrix[10] * matrix[13] + matrix[14] * matrix[1] * matrix[11] + matrix[15] * matrix[9] * matrix[2]));    
    tmp_matrix[5]  = ((matrix[0] * matrix[10] * matrix[15] + matrix[8] * matrix[14] * matrix[3] + matrix[2] * matrix[11] * matrix[12]) - (matrix[3] * matrix[10] * matrix[12] + matrix[2] * matrix[15] * matrix[8] + matrix[11] * matrix[14] * matrix[0]));
    tmp_matrix[6]  = ((matrix[0] * matrix[9] * matrix[15] + matrix[3] * matrix[13] * matrix[8] + matrix[1] * matrix[11] * matrix[12])  - (matrix[3] * matrix[9] * matrix[12] + matrix[11] * matrix[13] * matrix[0] + matrix[1] * matrix[15] * matrix[8]));
    tmp_matrix[7]  = ((matrix[0] * matrix[9] * matrix[14] + matrix[8] * matrix[13] * matrix[2] + matrix[1] * matrix[10] * matrix[12])  - (matrix[2] * matrix[9] * matrix[12] + matrix[1] * matrix[14] * matrix[8] + matrix[0] * matrix[13] * matrix[10]));
    tmp_matrix[8]  = ((matrix[1] * matrix[6] * matrix[15] + matrix[14] * matrix[5] * matrix[3] + matrix[2] * matrix[7] * matrix[13])   - (matrix[6] * matrix[3] * matrix[13] + matrix[4] * matrix[1] * matrix[7] + matrix[5] * matrix[15] * matrix[2]));
    tmp_matrix[9]  = ((matrix[0] * matrix[6] * matrix[15] + matrix[2] * matrix[12] * matrix[7] + matrix[4] * matrix[14] * matrix[3])   - (matrix[6] * matrix[3] * matrix[12] + matrix[4] * matrix[2] * matrix[15] + matrix[7] * matrix[14] * matrix[0]));
    tmp_matrix[10] = ((matrix[0] * matrix[5] * matrix[15] + matrix[3] * matrix[13] * matrix[4] + matrix[1] * matrix[7] * matrix[12])   - (matrix[3] * matrix[5] * matrix[12] + matrix[1] * matrix[4] * matrix[15] + matrix[7] * matrix[3] * matrix[0]));
    tmp_matrix[11] = ((matrix[14] * matrix[5] * matrix[0] + matrix[13] * matrix[4] * matrix[2] + matrix[1] * matrix[6] * matrix[12])   - (matrix[2] * matrix[5] * matrix[12] + matrix[1] * matrix[14] * matrix[4] + matrix[6] * matrix[13] * matrix[0]));
    tmp_matrix[12] = ((matrix[1] * matrix[6] * matrix[11] + matrix[5] * matrix[10] * matrix[3] + matrix[2] * matrix[9] * matrix[7])    - (matrix[6] * matrix[9] * matrix[3] + matrix[2]* matrix[5] * matrix[11] + matrix[7] * matrix[10] * matrix[1]));
    tmp_matrix[13] = ((matrix[0] * matrix[6] * matrix[11] + matrix[8] * matrix[2] * matrix[7] + matrix[4] * matrix[10] * matrix[3])    - (matrix[3] * matrix[6] * matrix[8] + matrix[4] * matrix[11] * matrix[2] + matrix[0] * matrix[7] * matrix[10]));
    tmp_matrix[14] = ((matrix[0] * matrix[5] * matrix[11] + matrix[4] * matrix[3] * matrix[9] + matrix[1] * matrix[8] * matrix[7])     - (matrix[3] * matrix[5] * matrix[8] + matrix[11] * matrix[1] * matrix[4] + matrix[0] * matrix[7] * matrix[9]));
    tmp_matrix[15] = ((matrix[0] * matrix[5] * matrix[10] + matrix[4] * matrix[9] * matrix[2] + matrix[1] * matrix[6] * matrix[8])     - (matrix[2] * matrix[8] * matrix[5] + matrix[4] * matrix[1] * matrix[10] + matrix[6] * matrix[9] * matrix[0]));

    matrix[0]  = detA * tmp_matrix[0];
    matrix[1]  = detA * tmp_matrix[4];
    matrix[2]  = detA * tmp_matrix[8];
    matrix[3]  = detA * tmp_matrix[12];
    matrix[4]  = detA * tmp_matrix[1];
    matrix[5]  = detA * tmp_matrix[5];
    matrix[6]  = detA * tmp_matrix[9];
    matrix[7]  = detA * tmp_matrix[13];
    matrix[8]  = detA * tmp_matrix[2];
    matrix[9]  = detA * tmp_matrix[6];
    matrix[10] = detA * tmp_matrix[10];
    matrix[11] = detA * tmp_matrix[14];
    matrix[12] = detA * tmp_matrix[3];
    matrix[13] = detA * tmp_matrix[7];
    matrix[14] = detA * tmp_matrix[11];
    matrix[15] = detA * tmp_matrix[15];
    
}