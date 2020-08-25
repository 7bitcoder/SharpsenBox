find_package(GTest)
#conan_basic_setup()

enable_testing(true)

add_executable(UpdateManagerTest
    AppInfoTest.cpp
    main.cpp "UpdateManagerMock.hpp" "ConfigMock.hpp")

add_test(AppInfoTest AppInfoTest)

qt5_use_modules(UpdateManagerTest 
    Quick 
    Core 
    Network)

target_link_libraries(UpdateManagerTest
    UpdateManager
    Config
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::gtest)

target_include_directories(UpdateManagerTest PUBLIC
    "../../QConfig/h"
    "../../QConfig"
    "../../JsonParsers"
    "../../Component/h"
    "../../Component"
    "../h"
    "..")

set_target_properties(UpdateManagerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Tests")