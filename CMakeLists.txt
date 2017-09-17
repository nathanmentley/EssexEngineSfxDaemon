#files

file(GLOB EssexEngineSfxDaemon_HEADERS
    "src/Daemons/Sfx/*.h"
)
file(GLOB EssexEngineSfxDaemon_SRC
    ${EssexEngineSfxDaemon_HEADERS}
    "src/Daemons/Sfx/*.cpp"
)

#setup headers

file(COPY ${EssexEngineSfxDaemon_HEADERS} DESTINATION "${CMAKE_BINARY_DIR}/lib/include/EssexEngineSfxDaemon/")

#setup build

add_library(EssexEngineSfxDaemon SHARED ${EssexEngineSfxDaemon_SRC})
target_link_libraries(EssexEngineSfxDaemon EssexEngineCore)