#--------------------------------------------------------------------------
# project/PhysicsProject
#--------------------------------------------------------------------------

PROJECT(PhysicsProject)
FILE(GLOB head code/*.h)
FILE(GLOB src code/*.cc)

list(APPEND files_physics ${head} ${src})
SOURCE_GROUP("PhysicsProject" FILES ${files_physics})

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

ADD_EXECUTABLE(PhysicsProject ${files_physics} ${shaders})
set_target_properties(PhysicsProject PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tempdir")
TARGET_LINK_LIBRARIES(PhysicsProject core render math system physics)
ADD_DEPENDENCIES(PhysicsProject core render math system physics)