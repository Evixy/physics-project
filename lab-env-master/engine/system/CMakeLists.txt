#--------------------------------------------------------------------------
# system/CMakeLists.txt
#--------------------------------------------------------------------------

file(GLOB head *.h)
file(GLOB src *.cc)
list(APPEND files_system ${head} ${src})

source_group("system" FILES ${files_system})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(system STATIC ${files_system} ${files_pch})
TARGET_PCH(system ../)
ADD_DEPENDENCIES(system math)
TARGET_LINK_LIBRARIES(system PUBLIC engine exts glew)