#--------------------------------------------------------------------------
# math/CMakeLists.txt
#--------------------------------------------------------------------------

file(GLOB head *.h)
file(GLOB src *.cc)
list(APPEND files_math ${head} ${src})

source_group("math" FILES ${files_math})
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
add_library(math STATIC ${files_math} ${files_pch})
ADD_DEPENDENCIES(math core)
TARGET_LINK_LIBRARIES(math PUBLIC core system)
TARGET_PCH(core ../)


