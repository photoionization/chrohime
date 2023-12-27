cmake_minimum_required(VERSION 3.0)
project(Gallery)

add_executable(gallery gallery.c)
if(WIN32)
  target_sources(gallery PRIVATE gallery.manifest)
endif()

string(CONCAT WITH_CONTENT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/"
                               "${CMAKE_SHARED_LIBRARY_PREFIX}"
                               "chrohime_with_content"
                               "${CMAKE_SHARED_LIBRARY_SUFFIX}"
                               "${CMAKE_IMPORT_LIBRARY_SUFFIX}")
string(CONCAT NO_CONTENT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/"
                             "${CMAKE_SHARED_LIBRARY_PREFIX}"
                             "chrohime"
                             "${CMAKE_SHARED_LIBRARY_SUFFIX}"
                             "${CMAKE_IMPORT_LIBRARY_SUFFIX}")

if (EXISTS "${WITH_CONTENT_LIB}")
  target_link_libraries(gallery LINK_PRIVATE "${WITH_CONTENT_LIB}")
  target_compile_definitions(gallery PRIVATE CHROHIME_WITH_CONTENT)
elseif (EXISTS "${NO_CONTENT_LIB}")
  target_link_libraries(gallery LINK_PRIVATE "${NO_CONTENT_LIB}")
else()
  message(FATAL_ERROR "Unable to find libchrohime.")
endif()
