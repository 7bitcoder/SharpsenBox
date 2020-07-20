
find_package(GTest)
#conan_basic_setup(TARGETS)

include(GoogleTest)
enable_testing(true)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:

add_executable(UpdateManagerTest
    AppInfoTest.cpp)

add_test(AppInfoTest AppInfoTest)

target_link_libraries(UpdateManagerTest
    UpdateManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Test
    CONAN_PKG::gtest)

target_include_directories(UpdateManagerTest PUBLIC
    "../../QConfig/h"
    "../../JsonParsers"
    "../../Component/h"
    "../h"
    "..")

gtest_discover_tests(UpdateManagerTest)


