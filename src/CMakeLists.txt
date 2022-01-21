add_subdirectory(Common)
# add_subdirectory(DataStorage)
# add_subdirectory(Environment)
add_subdirectory(Modules)
# add_subdirectory(MainMediator)
# add_subdirectory(Settings)

# add_library(SourceLibraries STATIC
    # ${Common}
#  )


# set_target_properties(SourceLibraries PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
        main.cpp
    )   

qt6_add_resources(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../qml.qrc)

add_executable(FrontendApp ${PROJECT_SOURCES})


target_link_libraries(FrontendApp Qt6::Core Qt6::Quick Qt6::Multimedia)
