LINK_DIRECTORIES( ${GLUT_LIB_DIR} )
INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
ADD_EXECUTABLE( book3102 book3102.cpp )
TARGET_LINK_LIBRARIES( book3102
	debug glut32D optimized glut32
	)
