#--------------------------------------------------------------------------
# engine/CMakeLists.txt
#--------------------------------------------------------------------------

ADD_LIBRARY(engine INTERFACE)
TARGET_INCLUDE_DIRECTORIES(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#TARGET_LINK_LIBRARIES(engine INTERFACE ${OPENGL_LIBS})
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(render)
add_subdirectory(math)
add_subdirectory(system)
add_subdirectory(physics)
add_subdirectory(animation)

SET_TARGET_PROPERTIES(core PROPERTIES FOLDER "gscept/lab_env")
SET_TARGET_PROPERTIES(render PROPERTIES FOLDER "gscept/lab_env")
SET_TARGET_PROPERTIES(math PROPERTIES FOLDER "gscept/lab_env")
SET_TARGET_PROPERTIES(system PROPERTIES FOLDER "gscept/lab_env")
SET_TARGET_PROPERTIES(physics PROPERTIES FOLDER "gscept/lab_env")
SET_TARGET_PROPERTIES(animation PROPERTIES FOLDER "gscept/lab_env")