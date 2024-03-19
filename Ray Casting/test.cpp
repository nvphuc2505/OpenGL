#include <iostream>
#include "math_funcs.h"

int main()
{
    vec3 vp(1, 5, 3);
    vec3 vp1(1, 2, 3);

    vp = vp * 2;
    std::cout << vp.v[1] << '\n';
    return 0;
}