#------------------------------#
# Real-Time Deformations - Setup
#------------------------------#
cmake_minimum_required(VERSION 3.3)
project(Real-TimeDeformations LANGUAGES  CXX)

#------------------------------#
#Include atlas
#------------------------------#
add_subdirectory(atlas)
include_directories(${ATLAS_INCLUDE_DIRS})

add_subdirectory(real_time_deformations)
