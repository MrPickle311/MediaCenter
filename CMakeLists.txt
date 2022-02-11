cmake_minimum_required(VERSION 3.14)

project(MediaCenter VERSION 0.1 LANGUAGES CXX)

set(QT_VERSION 6.2.3)

set(CMAKE_PROJECT_INCLUDE_BEFORE /home/$ENV{USER}/Qt/Tools/QtCreator/share/qtcreator/package-manager/auto-setup.cmake)
set(QT_QMAKE_EXECUTABLE /home/$ENV{USER}/Qt/${QT_VERSION}/gcc_64/bin/qmake )

list(APPEND CMAKE_PREFIX_PATH /home/$ENV{USER}/Qt/${QT_VERSION}/gcc_64)
add_compile_definitions(PROJECT_FOLDER_NAME="${CMAKE_CURRENT_SOURCE_DIR}")
add_compile_options(-Wall -Wextra)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Core Quick Multimedia SerialPort DBus  REQUIRED)
find_package(Qt6 COMPONENTS Core Quick Multimedia SerialPort DBus REQUIRED)
find_package(Boost 1.71.0)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(MediaCenter
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# else()
#     if(ANDROID)
#         add_library(MediaCenter SHARED
#             ${PROJECT_SOURCES}
#         )
#     else()
#         add_executable(MediaCenter
#           ${PROJECT_SOURCES}
#         )
#     endif()
# endif()

add_subdirectory(src)

target_compile_definitions(Common
    PUBLIC $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(Common
  	Qt${QT_VERSION_MAJOR}::Core 
  	Qt${QT_VERSION_MAJOR}::Quick 
  	Qt${QT_VERSION_MAJOR}::Multimedia
  	Qt${QT_VERSION_MAJOR}::DBus
)

# set_target_properties(MediaCenter PROPERTIES
#     MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
# )

set_target_properties(Common PROPERTIES
    CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
)


# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_import_qml_plugins(MediaCenter)
#     qt_finalize_executable(MediaCenter)
# endif()

# set(HEADERS_DIRS 
#     ${PROJECT_SOURCE_DIR}/src
#     ${PROJECT_SOURCE_DIR}/src/utilities
#     )

# include_directories(${HEADERS_DIRS})

add_subdirectory(tests)
