find_package(GTest)
#conan_basic_setup()

enable_testing(true)

add_executable(UpdateManagerTest
    AppInfoTest.cpp
    main.cpp)

add_test(AppInfoTest AppInfoTest)

qt5_use_modules(UpdateManagerTest 
    Quick 
    Core 
    Network)

target_link_libraries(UpdateManagerTest
    UpdateManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::gtest)

target_include_directories(UpdateManagerTest PUBLIC
    "../../QConfig/h"
    "../../JsonParsers"
    "../../Component/h"
    "../h"
    "..")