add_library(DATA_BACKEND_LIBRARIES STATIC)

add_subdirectory(audio)
add_subdirectory(images)
add_subdirectory(video)

target_link_libraries(DATA_BACKEND_LIBRARIES Qt5::Core )

set_target_properties(DATA_BACKEND_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
