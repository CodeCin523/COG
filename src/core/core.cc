#include <core/core.hh>

#include <print>

Core::Core() {
    std::printf("Core Create.\n");
}
Core::~Core() {
    std::print("Core Destroy.\n");
}