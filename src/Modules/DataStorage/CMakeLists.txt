set(sources
    MultimediaEngine.cpp
    )

set(headers
    MultimediaEngine.hpp
    )

add_library(MULTIMEDIA_ENGINE STATIC
            ${sources}
            ${headers})

add_subdirectory(audio)
add_subdirectory(images)
add_subdirectory(video)

target_link_libraries(MULTIMEDIA_ENGINE Qt5::Core )

set_target_properties(MULTIMEDIA_ENGINE PROPERTIES LINKER_LANGUAGE CXX)
