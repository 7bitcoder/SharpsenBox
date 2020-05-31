
find_package(Curl REQUIRED)
#find_package(openssl REQUIRED)
#find_package(Zlib REQUIRED)

conan_basic_setup(TARGETS)

add_library(AppUpdateChecker STATIC
    AppUpdateChecker.cpp)


target_include_directories(AppUpdateChecker PRIVATE
    "../MainWindow" 
    "../AppBackend"
    "../DownloadManager"
    "../Config")

qt5_use_modules(AppUpdateChecker 
    Quick 
    Core 
    Network
    Widgets)

target_link_libraries(AppUpdateChecker
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    CONAN_PKG::libcurl)

