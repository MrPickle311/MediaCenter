set(audio_headers
    Audio.hpp
)

set(audio_sources
    Audio.cpp
)

add_library(AUDIO_LIBRARIES STATIC
            ${audio_headers}
            ${audio_sources})

target_link_libraries(AUDIO_LIBRARIES Qt5::Core )
set_target_properties(AUDIO_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
