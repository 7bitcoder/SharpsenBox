
find_package(curl)

add_library(AppUpdateChecker STATIC
    AppUpdateChecker.cpp)


target_include_directories(AppUpdateChecker PUBLIC
    "../MainWindow" 
    ${ZLIB_INCLUDE_DIRS} 
    ${OPENSSL_INCLUDE_DIRS} 
    ${CURL_INCLUDE_DIRS})
    

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
    ${CURL_LIBRARIES} 
    ${OPENSSL_LIBRARIES} 
    ${ZLIB_LIBRARIES})
