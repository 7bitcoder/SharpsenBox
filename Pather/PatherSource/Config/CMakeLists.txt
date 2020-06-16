
add_library(Config STATIC
    Config.cpp "Game.cpp")


target_include_directories(Config PRIVATE
    "../MainWindow" 
    "../AppBackend"
    "../InstalationManager")

qt5_use_modules(Config 
    Quick 
    Core 
    Network
    Widgets)

target_link_libraries(Config
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets)

