add_subdirectory(Common)
# add_subdirectory(DataStorage)
# add_subdirectory(Environment)
add_subdirectory(Modules)
# add_subdirectory(MainMediator)
# add_subdirectory(Settings)

# add_library(SourceLibraries STATIC
    # ${Common}
#  )

# target_link_libraries(SourceLibraries Common)

# set_target_properties(SourceLibraries PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17)

# set(PROJECT_SOURCES
#         main.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/../qml.qrc
#     )   