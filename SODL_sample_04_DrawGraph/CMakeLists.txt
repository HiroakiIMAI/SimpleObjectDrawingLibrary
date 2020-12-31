cmake_minimum_required(VERSION 3.0)

#-------------------------------------
set( TARGET_NAME_LOCAL sample_04_DrawGraph )
#-------------------------------------


add_executable(
	# exe name
	${TARGET_NAME_LOCAL}
	
	#SourceCodeList
	SODL_sample_04_DrawGraph_main.cpp
	)

target_include_directories(
	# target name
	${TARGET_NAME_LOCAL}

	# include dir
	# internal
	PUBLIC ${CMAKE_SOURCE_DIR}/SimpleObjectDrawLib/include

	# external
	PUBLIC ${EIGEN_INC_PATH}
	PUBLIC ${ASSIMP_INC_PATH}
	PUBLIC ${FLEEGLUT_INC_PATH}
	PUBLIC ${GLEW_INC_PATH}
	
  )

target_link_libraries(
	# target name
	${TARGET_NAME_LOCAL}

	# libraries
	SimpleObjectDrawingLibrary
	${ASSIMP_LIB_PATH}/assimp.lib
	${FREEGLUT_LIB_PATH}/freeglut.lib
	${GLEW_LIB_PATH}/glew32.lib
	)

