
#include <iostream>
#include "matrix_proxy.hpp"
#include <assert.h>

int main()
{
    Matrix<int, -1> matrix;

    for (int i = 0; i < 10; ++i)
        matrix[i][i] = i; 

    for (int i = 0; i < 10; ++i)
        matrix[i][9 - i] = 9 - i; 

    std::cout << "Fragment [1,1]..[8,8]:" << std::endl;
    for (int y = 1; y <= 8; ++y)
    {
        for (int x = 1; x <= 8; ++x)
        {
            std::cout << static_cast<int>(matrix[y][x]);
            if (x < 8)
                std::cout << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Occupied cells count: " << matrix.size() << std::endl;
    assert(matrix.size() == 20);

    std::cout << "Occupied cells (x y v):" << std::endl;
    for (auto cell : matrix)
    {
        int x, y, v;
        std::tie(x, y, v) = cell;
        std::cout << x << ' ' << y << ' ' << v << std::endl;
    }

    return 0;
}
