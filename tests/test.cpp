#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

std::string ReadFile(const fs::path& path)
{
    std::ifstream input(path);
    std::stringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

void RunRefactorAndCompare(const fs::path& input, 
                           const fs::path& expected, 
                           const fs::path& temp){
    fs::copy_file(input, temp, fs::copy_options::overwrite_existing);

    std::string command = 
        ".\\build\\refactor_tool.exe " + temp.string() +
        " -- -std=c++20 "
        "-isystem D:/Programm/MSYS2/mingw64/lib/clang/21/include "
        "-isystem D:/Programm/MSYS2/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include "
        "-isystem D:/Programm/MSYS2/mingw64/include/c++/15.2.0 "
        "-isystem D:/Programm/MSYS2/mingw64/include/c++/15.2.0/x86_64-w64-mingw32 "
        "-isystem D:/Programm/MSYS2/mingw64/include";

    int result = std::system(command.c_str());
    ASSERT_EQ(result, 0);

    EXPECT_EQ(ReadFile(temp), ReadFile(expected));

    fs::remove(temp);
}

TEST(VirtualDtorTest, AddsVirtualOnlyForBaseClasses) 
{
    RunRefactorAndCompare(
        "tests/tests_data/test1.cpp",
        "tests/tests_data/test1_ref.cpp",
        "tests/tests_data/test1_tmp.cpp"
    );
}

TEST(OverrideTest, AddsOverrideForOverriddenMethods) 
{
    RunRefactorAndCompare(
        "tests/tests_data/test2.cpp",
        "tests/tests_data/test2_ref.cpp",
        "tests/tests_data/test2_tmp.cpp"
    );
}

TEST(ConstReferenceTest, AddsReferenceForConstRangeLoopVariables) 
{
    RunRefactorAndCompare(
        "tests/tests_data/test3.cpp",
        "tests/tests_data/test3_ref.cpp",
        "tests/tests_data/test3_tmp.cpp"
    );
}