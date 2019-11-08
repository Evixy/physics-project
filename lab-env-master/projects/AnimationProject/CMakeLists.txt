#--------------------------------------------------------------------------
# project/AnimationProject
#--------------------------------------------------------------------------

PROJECT(AnimationProject)
FILE(GLOB head code/*.h)
FILE(GLOB src code/*.cc)

list(APPEND files_animation ${head} ${src})
SOURCE_GROUP("AnimationProject" FILES ${files_animation})

set(shaders 
	resources/RastBasicShader.vert 
	resources/RastBasicShader.frag 
	resources/BlinnPhongShader.vert 
	resources/BlinnPhongShader.frag 
	resources/LineShader.vert 
	resources/LineShader.frag 
	)


source_group("Shaders" FILES ${shaders})

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WORKING_DIR})

ADD_EXECUTABLE(AnimationProject ${files_animation} ${shaders})
set_target_properties(AnimationProject PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tempdir")
TARGET_LINK_LIBRARIES(AnimationProject core render math system physics animation)
ADD_DEPENDENCIES(AnimationProject core render math system physics animation)