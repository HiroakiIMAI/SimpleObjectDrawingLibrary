cmake_minimum_required(VERSION 3.0)


project(SimpleObjectDrawingLibrary CXX)


add_library(
	# libname
	SimpleObjectDrawingLibrary
	
	# static or dynamic
	STATIC
	
	#SourceCodeList
	assimpGlUtil.cpp
	CoordChain3dMdl.cpp
	CoordChainObj.cpp
	Drawable3dMdlClass.cpp
	DrawableObjectClass.cpp
	DrawingManager.cpp
	glDrawExt.cpp
	GraphObj.cpp
	IDrawableObjBase.cpp
	ISodlObjBase.cpp
	TimeSeriesGraph.cpp
	)

target_include_directories(
	# target name
	SimpleObjectDrawingLibrary

	# include dir
	# internal
	PUBLIC ${PROJECT_SOURCE_DIR}/../SimpleObjectDrawLib/include

	# external
	PUBLIC ${EIGEN_INC_PATH}
	PUBLIC ${ASSIMP_INC_PATH}
	PUBLIC ${FREEGLUT_INC_PATH}
	PUBLIC ${GLEW_INC_PATH}
	
  )


