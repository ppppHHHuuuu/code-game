#include <iostream>

int main() {
    int a[3][3];
    a[0][0] = 10;
    std::cout << &a[0][0] << std::endl <<&a;
}