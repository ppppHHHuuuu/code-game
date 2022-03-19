#include <iostream>

int main() {
    int a[3][3];
    a[0][0] = 1000;
    std::cout << &a[0][0] << std::endl <<&a;
}