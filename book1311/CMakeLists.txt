
ADD_EXECUTABLE( book1311 WIN32
	GLRECT.cpp
	GLRECT.h
	GLRECT.ico
	GLRECT.rc
	Resource.h
	small.ico
	stdafx.cpp
	stdafx.h
	targetver.h
	)
#ADD_DEFINITIONS( "-D_WINDOWS -DWIN32 -D__WIN__ -D_CRT_SECURE_NO_DEPRECATE" )
TARGET_LINK_LIBRARIES( book1311
	debug opengl32 optimized opengl32
	)
