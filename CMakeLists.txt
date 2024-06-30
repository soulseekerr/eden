cmake_minimum_required(VERSION 3.28.1)
project(EdenProject)

add_subdirectory(edencore)
add_subdirectory(edenanalytics)
add_subdirectory(eden)
add_subdirectory(edentests)
add_subdirectory(edendaemon)
# add_subdirectory(edenpy)  # is built independently using setuptools