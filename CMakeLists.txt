cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(ProjName PBRTests)
set(CMAKE_CXX_STADNARD 17)

project(${ProjName})

add_executable(${ProjName} 
	src/main.cpp 
	src/vec3.h
	src/color.h
	src/ray.h
	src/hittable.h
	src/sphere.h
	src/hittable_list.h
	src/utility.h
	src/interval.h
)