#include <fstream>
#include <iostream>

#include "Vec2.hpp"

// Main entry point
int main(int argc, char *argv[]) {
    // std::string config_file = "config.txt";
    auto *vec_a = new Vec2<double>(2, 3);
    const auto *vec_b = new Vec2<double>(3, 2);
    auto my_result = *vec_a + *vec_b;

    std::cout << "***Add***" << std::endl;
    std::cout << "x: " << my_result.x() << "  y: " << my_result.y() << '\n';
    my_result = *vec_a - *vec_b;
    std::cout << "***Subtract***" << std::endl;
    std::cout << "x: " << my_result.x() << "  y: " << my_result.y() << '\n';
    my_result = *vec_a * *vec_b;
    std::cout << "***Multiply***" << std::endl;
    std::cout << "x: " << my_result.x() << "  y: " << my_result.y() << '\n';
    my_result = *vec_a / *vec_b;
    std::cout << "***Divide***" << std::endl;
    std::cout << "x: " << my_result.x() << "  y: " << my_result.y() << '\n';

    return 0;
}
