#include "pifantunerd.h"
#include <vector>
#include <string>

int main() {
    pifantunerd();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    pifantunerd_print_vector(vec);
}
