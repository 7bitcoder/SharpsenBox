find_package(GTest)
#conan_basic_setup()

enable_testing(true)

add_executable(UpdateManagerTest
    AppInfoTest.cpp
    main.cpp "UpdateManagerMock.hpp" "ConfigMock.hpp")

add_test(AppInfoTest AppInfoTest)

qt6_use_modules(UpdateManagerTest
    Quick 
    Core 
    Network)

target_link_libraries(UpdateManagerTest
    UpdateManager
    Config
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
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
