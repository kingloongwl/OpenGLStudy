
LINK_DIRECTORIES( ${GLUT_LIB_DIR} )
INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/GLTools )
ADD_EXECUTABLE( book1381 WIN32
	SPHEREWORLD32.cpp
	SPHEREWORLD32.h
	SPHEREWORLD32.ico
	SPHEREWORLD32.rc
	Resource.h
	small.ico
	stdafx.cpp
	stdafx.h
	targetver.h
	)
#ADD_DEFINITIONS( "-D_WINDOWS -DWIN32 -D__WIN__ -D_CRT_SECURE_NO_DEPRECATE" )
TARGET_LINK_LIBRARIES( book1381
	debug glu32 optimized glu32
	debug opengl32 optimized opengl32
	debug glut32D optimized glut32
	debug GLTools optimized GLTools
	)
