
ADD_EXECUTABLE( book1352 WIN32
	GLTEXT2D.cpp
	GLTEXT2D.h
	GLTEXT2D.ico
	GLTEXT2D.rc
	Resource.h
	small.ico
	stdafx.cpp
	stdafx.h
	targetver.h
	)
#ADD_DEFINITIONS( "-D_WINDOWS -DWIN32 -D__WIN__ -D_CRT_SECURE_NO_DEPRECATE" )
TARGET_LINK_LIBRARIES( book1352
	debug glu32 optimized glu32
	debug opengl32 optimized opengl32
	)
