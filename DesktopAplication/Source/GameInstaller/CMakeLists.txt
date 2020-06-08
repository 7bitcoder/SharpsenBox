
add_library(GameInstaller STATIC
    GameInstaller.cpp)

target_include_directories(GameInstaller PUBLIC
    "../AppBackend"
    "../Config"
    "../LoadingBar"
    "../InstalationManager")

qt5_use_modules(GameInstaller 
    Quick 
    Core 
    Network
    Widgets)

target_link_libraries(GameInstaller
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)