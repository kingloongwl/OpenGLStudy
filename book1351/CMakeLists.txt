
ADD_EXECUTABLE( book1351 WIN32
	GLTEXT.cpp
	GLTEXT.h
	GLTEXT.ico
	GLTEXT.rc
	Resource.h
	small.ico
	stdafx.cpp
	stdafx.h
	targetver.h
	)
#ADD_DEFINITIONS( "-D_WINDOWS -DWIN32 -D__WIN__ -D_CRT_SECURE_NO_DEPRECATE" )
TARGET_LINK_LIBRARIES( book1351
	debug glu32 optimized glu32
	debug opengl32 optimized opengl32
	)
