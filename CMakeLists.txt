cmake_minimum_required(VERSION 3.0)

#---------------------------
# path of external libralies
#---------------------------

find_path(EIGEN_INC_PATH REQUIRED)

find_path(ASSIMP_INC_PATH REQUIRED)
find_path(ASSIMP_LIB_PATH REQUIRED)

find_path(FREEGLUT_INC_PATH REQUIRED)
find_path(FREEGLUT_LIB_PATH REQUIRED)

find_path(GLEW_INC_PATH REQUIRED)
find_path(GLEW_LIB_PATH REQUIRED)


project(SimpleObjectDrawingLibrary CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin_samples )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/bin_samples )

add_subdirectory( SimpleObjectDrawLib )
add_subdirectory( SODL_sample_01_LoadModel )
add_subdirectory( SODL_sample_02_6AxRobot )
add_subdirectory( SODL_sample_03_MultiViewPort )
add_subdirectory( SODL_sample_04_DrawGraph )
add_subdirectory( SODL_sample_05_DrawGraph3D )


	