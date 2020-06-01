
find_package(Curl REQUIRED)

conan_basic_setup(TARGETS)

add_library( AppUpdaterBackend STATIC
    AppUpdater.cpp)

target_include_directories(AppUpdaterBackend PUBLIC
    "../MainWindow" 
    "../AppBackend"
    "../InstalationManager"
    "../Config")

qt5_use_modules(AppUpdaterBackend 
    Quick 
    Core)

target_link_libraries(AppUpdaterBackend
    InstalationManager
    Config
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::libcurl)