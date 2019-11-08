#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_display
	window.h
	window.cc)
SOURCE_GROUP("display" FILES ${files_render_display})

SET(files_render_system
	textureresource.h
	textureresource.cc
	meshresource.h
	meshresource.cc
	shaderobject.h
	shaderobject.cc
	vertex.h
	)
SOURCE_GROUP("system" FILES ${files_render_system})

SET(files_render
	${files_render_display}
	${files_render_pch}
	${files_render_system})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw imgui ${OPENGL_LIBS})