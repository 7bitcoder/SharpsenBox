
add_library(GameExecutor STATIC
    GameExecutor.cpp)

target_include_directories(GameExecutor PUBLIC
    "../AppBackend"
    "../Config"
    "../LoadingBar"
    "../InstalationManager")

qt5_use_modules(GameExecutor 
    Quick 
    Core 
    Network
    Widgets)

target_link_libraries(GameExecutor
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)