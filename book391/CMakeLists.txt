LINK_DIRECTORIES( ${GLUT_LIB_DIR} )
INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
ADD_EXECUTABLE( book391 book391.cpp )
TARGET_LINK_LIBRARIES( book391
	debug glut32D optimized glut32
	)
