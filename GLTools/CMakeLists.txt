SET(LIBSRC
	GLTools.h
	GLEXT.H
	WGLEXT.H
	FrameMath.cpp
	GetExtensionPointer.cpp
	IsExtSupported.cpp
	LoadTGA.cpp
	MatrixMath.cpp
	sphere.cpp
	Stopwatch.cpp
	torus.cpp
	UnitAxes.cpp
	VectorMath.cpp
	WriteTGA.cpp
	)
ADD_LIBRARY(GLTools STATIC ${LIBSRC})
