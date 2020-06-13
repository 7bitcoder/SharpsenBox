
add_library(AppBackend STATIC
    AppBackend.cpp)

target_include_directories(AppBackend PUBLIC
    "../Dialog"
    "../InstalationManager"
    "../Config"
    "../LoadingBar"
    "../GameManager"
    "../GameExecutor")

qt5_use_modules(AppBackend 
    Quick 
    Core 
    Network)

target_link_libraries(AppBackend
    Dialog
    Config
    GameExecutor
    LoadingBar
    GameManager
    InstalationManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)
