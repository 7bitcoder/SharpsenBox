

enable_testing(true)

add_executable(UpdateManagerTest
    AppInfoTest.cpp)

add_test(NAME UpdateManagerTest COMMAND UpdateManagerTest)

target_link_libraries(UpdateManagerTest
    UpdateManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)

target_include_directories(UpdateManagerTest PUBLIC
    "../../QConfig/h"
    "../../JsonParsers"
    "../../Component/h"
    "../h"
    "..")


