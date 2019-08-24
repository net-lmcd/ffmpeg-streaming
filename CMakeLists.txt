cmake_minimum_required(VERSION 3.14)
project(youtube_streaming)

set(CMAKE_CXX_STANDARD 14)

include_directories(/home/sangmin/ffmpeg_build/include)
#include_directories(/home/sangmin/ffmpeg_sources/ffmpeg)

#set(CMAKE_LIBRARY_PATH /usr/local/lib)
#set(CMAKE_LIBRARY_PATH  /home/sangmin/ffmpeg_build/lib)

link_directories(/home/sangmin/ffmpeg_build/lib/)
#link_directories(/usr/lib/x86_64-linux-gnu/)

include_directories(/usr/lib/x86_64-linux-gnu/)
#include_directories(/home/sangmin/ffmpeg_build/include)
#include_directories(/usr/include/)
add_executable(youtube_streaming main.cpp)

target_link_libraries(
        youtube_streaming
        yasm
        avcodec
        avfilter
        avformat
        avutil
        fdk-aac
        va
        va-drm
        va-x11
        m
        mp3lame
        opus
        avdevice
        postproc
        swresample
        swscale
        vorbis
        vorbisenc
        vdpau
        vpx
        x264
        x265
        dl
        X11
        pthread
        z
        rt
)