#--------------------------------------------------------------------------
# animation/CMakeLists.txt
#--------------------------------------------------------------------------

file(GLOB head *.h)
file(GLOB src *.cc)
list(APPEND files_animation ${head} ${src})

source_group("animation" FILES ${files_animation})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(animation STATIC ${files_animation} ${files_pch})
TARGET_PCH(animation ../)
ADD_DEPENDENCIES(animation math tinyxml2)
TARGET_LINK_LIBRARIES(animation PUBLIC engine exts glew tinyxml2)