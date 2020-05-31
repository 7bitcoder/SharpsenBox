
add_library(AppBackend STATIC
    AppBackend.cpp)

target_include_directories(AppBackend PUBLIC
    "../MainWindow"
    "../AppUpdateChecker"
    "../DownloadManager"
    "../Config")

qt5_use_modules(AppBackend 
    Quick 
    Core 
    Network)

target_link_libraries(AppBackend
    Button
    Config
    AppUpdateChecker
    DownloadManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)
