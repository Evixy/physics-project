#--------------------------------------------------------------------------
# physics/CMakeLists.txt
#--------------------------------------------------------------------------

file(GLOB head *.h)
file(GLOB src *.cc)
list(APPEND files_physics ${head} ${src})

source_group("physics" FILES ${files_physics})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(physics STATIC ${files_physics} ${files_pch})
TARGET_PCH(physics ../)
ADD_DEPENDENCIES(physics math)
TARGET_LINK_LIBRARIES(physics PUBLIC engine exts glew)