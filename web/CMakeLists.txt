cmake_minimum_required(VERSION 3.20)
project(Proyecto1942WebPOC LANGUAGES CXX)

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "This proof of concept requires Emscripten. Configure with emcmake cmake.")
endif()

add_executable(proyecto1942 main.cpp)
target_compile_features(proyecto1942 PRIVATE cxx_std_17)
set_target_properties(proyecto1942 PROPERTIES SUFFIX ".html")
target_compile_options(proyecto1942 PRIVATE -Wall -Wextra -Wpedantic -sUSE_SDL=2)
target_link_options(proyecto1942 PRIVATE
  -sUSE_SDL=2
  -sALLOW_MEMORY_GROWTH=1
  -sASSERTIONS=1
  -sEXIT_RUNTIME=0
)
