void transpose_matrix(float (&origin_matrix)[16])
{
    float tmp_matrix[16];

    for(int i = 0; i < 16; i++)
    {
        tmp_matrix[i] = origin_matrix[i];
    }

    origin_matrix[0]  =  tmp_matrix[0];
    origin_matrix[1]  =  tmp_matrix[4];
    origin_matrix[2]  =  tmp_matrix[8];
    origin_matrix[3]  =  tmp_matrix[12];
    origin_matrix[4]  =  tmp_matrix[1];
    origin_matrix[5]  =  tmp_matrix[5];
    origin_matrix[6]  =  tmp_matrix[9];
    origin_matrix[7]  =  tmp_matrix[13];
    origin_matrix[8]  =  tmp_matrix[2];
    origin_matrix[9]  =  tmp_matrix[6];
    origin_matrix[10] =  tmp_matrix[10];
    origin_matrix[11] =  tmp_matrix[14];
    origin_matrix[12] =  tmp_matrix[3];
    origin_matrix[13] =  tmp_matrix[7];
    origin_matrix[14] =  tmp_matrix[11];
    origin_matrix[15] =  tmp_matrix[15];
}

void matrix_multiplite_matrix(float (&c)[16], float a[16], float b[16])
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

void inverse_matrix(float (&matrix)[16])
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
    tmp_matrix[0]  =   ((matrix[5]  * matrix[10] * matrix[15] + matrix[9]  * matrix[14] * matrix[7] + matrix[6] * matrix[11] * matrix[13]) - (matrix[7] * matrix[10] * matrix[13] + matrix[5]  * matrix[14] * matrix[11] + matrix[6]  * matrix[15] * matrix[9])) ;
    tmp_matrix[1]  = - ((matrix[4]  * matrix[10] * matrix[15] + matrix[8]  * matrix[14] * matrix[7] + matrix[6] * matrix[11] * matrix[12]) - (matrix[7] * matrix[10] * matrix[12] + matrix[11] * matrix[14] * matrix[4]  + matrix[6]  * matrix[8]  * matrix[15]));
    tmp_matrix[2]  =   ((matrix[4]  * matrix[9]  * matrix[15] + matrix[8]  * matrix[13] * matrix[7] + matrix[5] * matrix[11] * matrix[12]) - (matrix[7] * matrix[9]  * matrix[12] + matrix[4]  * matrix[13] * matrix[11] + matrix[5]  * matrix[15] * matrix[8])) ;
    tmp_matrix[3]  = - ((matrix[4]  * matrix[9]  * matrix[14] + matrix[8]  * matrix[13] * matrix[6] + matrix[5] * matrix[10] * matrix[12]) - (matrix[6] * matrix[9]  * matrix[12] + matrix[4]  * matrix[13] * matrix[10] + matrix[5]  * matrix[14] * matrix[8])) ;
    tmp_matrix[4]  = - ((matrix[1]  * matrix[10] * matrix[15] + matrix[9]  * matrix[14] * matrix[3] + matrix[2] * matrix[11] * matrix[13]) - (matrix[3] * matrix[10] * matrix[13] + matrix[14] * matrix[1]  * matrix[11] + matrix[15] * matrix[9]  * matrix[2])) ;    
    tmp_matrix[5]  =   ((matrix[0]  * matrix[10] * matrix[15] + matrix[8]  * matrix[14] * matrix[3] + matrix[2] * matrix[11] * matrix[12]) - (matrix[3] * matrix[10] * matrix[12] + matrix[2]  * matrix[15] * matrix[8]  + matrix[11] * matrix[14] * matrix[0])) ;
    tmp_matrix[6]  = - ((matrix[0]  * matrix[9]  * matrix[15] + matrix[3]  * matrix[13] * matrix[8] + matrix[1] * matrix[11] * matrix[12]) - (matrix[3] * matrix[9]  * matrix[12] + matrix[11] * matrix[13] * matrix[0]  + matrix[1]  * matrix[15] * matrix[8])) ;
    tmp_matrix[7]  =   ((matrix[0]  * matrix[9]  * matrix[14] + matrix[8]  * matrix[13] * matrix[2] + matrix[1] * matrix[10] * matrix[12]) - (matrix[2] * matrix[9]  * matrix[12] + matrix[1]  * matrix[14] * matrix[8]  + matrix[0]  * matrix[13] * matrix[10]));
    tmp_matrix[8]  =   ((matrix[1]  * matrix[6]  * matrix[15] + matrix[14] * matrix[5]  * matrix[3] + matrix[2] * matrix[7]  * matrix[13]) - (matrix[6] * matrix[3]  * matrix[13] + matrix[14] * matrix[1]  * matrix[7]  + matrix[5]  * matrix[15] * matrix[2])) ;
    tmp_matrix[9]  = - ((matrix[0]  * matrix[6]  * matrix[15] + matrix[2]  * matrix[12] * matrix[7] + matrix[4] * matrix[14] * matrix[3])  - (matrix[6] * matrix[3]  * matrix[12] + matrix[4]  * matrix[2]  * matrix[15] + matrix[7]  * matrix[14] * matrix[0])) ;
    tmp_matrix[10] =   ((matrix[0]  * matrix[5]  * matrix[15] + matrix[3]  * matrix[13] * matrix[4] + matrix[1] * matrix[7]  * matrix[12]) - (matrix[3] * matrix[5]  * matrix[12] + matrix[1]  * matrix[4]  * matrix[15] + matrix[7]  * matrix[13] * matrix[0])) ;
    tmp_matrix[11] = - ((matrix[14] * matrix[5]  * matrix[0]  + matrix[13] * matrix[4]  * matrix[2] + matrix[1] * matrix[6]  * matrix[12]) - (matrix[2] * matrix[5]  * matrix[12] + matrix[1]  * matrix[14] * matrix[4]  + matrix[6]  * matrix[13] * matrix[0])) ;
    tmp_matrix[12] = - ((matrix[1]  * matrix[6]  * matrix[11] + matrix[5]  * matrix[10] * matrix[3] + matrix[2] * matrix[9]  * matrix[7])  - (matrix[6] * matrix[9]  * matrix[3]  + matrix[2]  * matrix[5]  * matrix[11] + matrix[7]  * matrix[10] * matrix[1])) ;
    tmp_matrix[13] =   ((matrix[0]  * matrix[6]  * matrix[11] + matrix[8]  * matrix[2]  * matrix[7] + matrix[4] * matrix[10] * matrix[3])  - (matrix[3] * matrix[6]  * matrix[8]  + matrix[4]  * matrix[11] * matrix[2]  + matrix[0]  * matrix[7]  * matrix[10]));
    tmp_matrix[14] = - ((matrix[0]  * matrix[5]  * matrix[11] + matrix[4]  * matrix[3]  * matrix[9] + matrix[1] * matrix[8]  * matrix[7])  - (matrix[3] * matrix[5]  * matrix[8]  + matrix[11] * matrix[1]  * matrix[4]  + matrix[0]  * matrix[7]  * matrix[9])) ;
    tmp_matrix[15] =   ((matrix[0]  * matrix[5]  * matrix[10] + matrix[4]  * matrix[9]  * matrix[2] + matrix[1] * matrix[6]  * matrix[8])  - (matrix[2] * matrix[8]  * matrix[5]  + matrix[4]  * matrix[1]  * matrix[10] + matrix[6]  * matrix[9]  * matrix[0])) ;

    matrix[0]  = (1 / detA) * tmp_matrix[0];
    matrix[1]  = (1 / detA) * tmp_matrix[4];
    matrix[2]  = (1 / detA) * tmp_matrix[8];
    matrix[3]  = (1 / detA) * tmp_matrix[12];
    matrix[4]  = (1 / detA) * tmp_matrix[1];
    matrix[5]  = (1 / detA) * tmp_matrix[5];
    matrix[6]  = (1 / detA) * tmp_matrix[9];
    matrix[7]  = (1 / detA) * tmp_matrix[13];
    matrix[8]  = (1 / detA) * tmp_matrix[2];
    matrix[9]  = (1 / detA) * tmp_matrix[6];
    matrix[10] = (1 / detA) * tmp_matrix[10];
    matrix[11] = (1 / detA) * tmp_matrix[14];
    matrix[12] = (1 / detA) * tmp_matrix[3];
    matrix[13] = (1 / detA) * tmp_matrix[7];
    matrix[14] = (1 / detA) * tmp_matrix[11];
    matrix[15] = (1 / detA) * tmp_matrix[15];
}

void multiply_matrix_to_vector(float matrix[16], float (&vec)[4])
{
    float tmp[4];
    for(int i = 0; i < 4; i++)
    {
        tmp[i] = vec[i];
    }

    vec[0] = matrix[0] * tmp[0] + matrix[1] * tmp[1] + matrix[2] * tmp[2] + matrix[3] * tmp[3];
    vec[1] = matrix[4] * tmp[0] + matrix[5] * tmp[1] + matrix[6] * tmp[2] + matrix[7] * tmp[3];
    vec[2] = matrix[8] * tmp[0] + matrix[9] * tmp[1] + matrix[10] * tmp[2] + matrix[11] * tmp[3];
    vec[3] = matrix[12] * tmp[0] + matrix[13] * tmp[1] + matrix[14] * tmp[2] + matrix[15] * tmp[3];
}