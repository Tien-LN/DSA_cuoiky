cmake_minimum_required(VERSION 3.10)
project(bai1)

set(CMAKE_CXX_STANDARD 17)

# Add compiler definition to disable std::byte
add_compile_definitions(_HAS_STD_BYTE=0)

# Include headers
include_directories(include)

# Package các thư viện cần thiết
find_package(fmt CONFIG REQUIRED)

# Tìm tất cả file cpp trong src/
file(GLOB SOURCES "src/*.cpp")

# Tạo file thực thi
add_executable(bai1 ${SOURCES})

# Liên kết thư viện sau khi đã tạo target
target_link_libraries(bai1 PRIVATE fmt::fmt)