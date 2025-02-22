#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <string>
#include <limits>
#include <regex>
#include <system_error>
#include <typeinfo>
#include <fstream>
#include <functional>
#include <optional>
#include <variant>

std::string createString(size_t size) {
    if (size > 1'000'000) throw std::length_error("String too large!");
    return std::string(size, 'A');
}

double safeSqrt(double x) {
    if (x < 0) throw std::domain_error("Cannot compute sqrt of negative number!");
    return std::sqrt(x);
}


void testAge(int age) {
    if (age < 0) throw std::invalid_argument("Age cannot be negative!");
}

void testRange(int index) {
    std::vector<int> vec = {1, 2, 3};
    if (index >= vec.size()) {
        throw std::out_of_range("Index out of range!");
    }
    std::cout << vec.at(index) << std::endl;
}

void checkRange(int value) {
    if (value > 1000) throw std::range_error("Value too large for this operation!");
}

int checkMultiply(int a, int b) {
    if (a > std::numeric_limits<int>::max() / b) 
        throw std::overflow_error("Multiplication overflow!");
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) throw std::underflow_error("Division by zero!");
    return a / b;
}

void openFile(const std::string& filename) {
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), "File not found!");
}

void checkRegex(const std::string& pattern) {
    try {
        std::regex regex(pattern);
    } catch (const std::regex_error& e) {
        throw std::runtime_error("Invalid regex pattern!");
    }
}

struct Base { virtual ~Base() {} };
struct Derived : Base {};

void testCast(Base* base) {
    Derived& d = dynamic_cast<Derived&>(*base); // Throws std::bad_cast if cast fails
    std::cout << "Casted to Derived!" << &d << std::endl;
}

void checkType(Base* base) {
    if (!base) throw std::bad_typeid();
    std::cout << typeid(*base).name() << std::endl;
}

void readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::ios_base::failure("File not found!");
}

std::function<void()> func;
void testFunction() {
    if (!func) throw std::bad_function_call();
    func();
}

std::optional<int> getValue(bool condition) {
    if (!condition) return std::nullopt;
    return 42;
}
void useOptional() {
    auto value = getValue(false);
    if (!value) throw std::bad_optional_access();
}


TEST(ExceptionsTest, BasicAssertions) {
    try {
        testRange(5);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        testAge(-4);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        safeSqrt(-2.33);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        createString(2'000'000);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        checkRange(100000);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        int a = 1'000'000'000;
        int b = 3;
        checkMultiply(a, b);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        divide(100, 0);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        openFile("no file");
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        checkRegex("[");
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    int* ptr = nullptr;
    try {
        ptr = new int[100'000'000'000'000]; // Too large!
        ptr[0] = 1;
        std::cout << "Allocated memory!" << ptr[0] << ". Deleting now." << std::endl;
        delete[] ptr;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        ptr = nullptr;
    }

    Base b;
    try {
        testCast(&b);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    Base* ptr_b = nullptr;
    try {
        checkType(ptr_b);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        readFile("no file");
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        testFunction();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        useOptional();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    std::variant<int, std::string> v = "Hello";
    try {
        std::cout << std::get<int>(v); // Throws std::bad_variant_access
    } catch (const std::bad_variant_access& e) {
        std::cout << "Variant error: " << e.what() << std::endl;
    }
}
