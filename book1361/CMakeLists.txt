
ADD_EXECUTABLE( book1361 WIN32
	FSCREEN.cpp
	FSCREEN.h
	FSCREEN.ico
	FSCREEN.rc
	Resource.h
	small.ico
	stdafx.cpp
	stdafx.h
	targetver.h
	)
#ADD_DEFINITIONS( "-D_WINDOWS -DWIN32 -D__WIN__ -D_CRT_SECURE_NO_DEPRECATE" )
TARGET_LINK_LIBRARIES( book1361
	debug opengl32 optimized opengl32
	)
