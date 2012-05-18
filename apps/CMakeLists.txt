########### Find MRCore
IF (WIN32)
	SET(CMAKE_INSTALL_PREFIX "C:/mrcore" CACHE PATH "FOO install prefix" FORCE)
ELSE(WIN32)
	SET(CMAKE_INSTALL_PREFIX /usr/local/mrcore) 
ENDIF(WIN32)
MESSAGE(STATUS "Default install directory is: " ${CMAKE_INSTALL_PREFIX}) 

find_path(	MRCORE_DIR include/mrcore/mrcore.h  ${CMAKE_INSTALL_PREFIX} )

INCLUDE_DIRECTORIES(${MRCORE_DIR}/include)
INCLUDE_DIRECTORIES(${MRCORE_DIR}/include/mrcore)
MESSAGE (STATUS "MRCore include folder is "${MRCORE_DIR}/include)
LINK_DIRECTORIES(${MRCORE_DIR}/lib)
#¡¡¡¡¡¡¡!!!!!!!!!!!¡¡¡!!!

ADD_DEFINITIONS(-DFREEGLUT_STATIC -D_CRT_SECURE_NO_WARNINGS)
ADD_EXECUTABLE(simulator simulator.cpp glutapp.cpp)
ADD_EXECUTABLE(	teleop
		teleop.cpp
		glutapp.cpp
		PathControl/Control.cpp 
		PathControl/Angular.cpp
		PathControl/AngDistToSeg.cpp 
		PathControl/CalculoError.cpp
		PathControl/ADSK.cpp
		ReactiveControl/Vision2D.cpp
		ReactiveControl/CinematicMap.cpp
		ReactiveControl/ControlReactivo.cpp
		ReactiveControl/ReactiveControl.cpp
		)

IF(WIN32)
	TARGET_LINK_LIBRARIES(simulator debug mrcore_d freeglut_static)
	TARGET_LINK_LIBRARIES(teleop debug mrcore_d freeglut_static)
	TARGET_LINK_LIBRARIES(simulator optimized mrcore freeglut_static)
	TARGET_LINK_LIBRARIES(teleop optimized mrcore freeglut_static)
ELSE(WIN32) #LINUX	
	TARGET_LINK_LIBRARIES(simulator mrcore glut GL GLU pthread)
	TARGET_LINK_LIBRARIES(teleop mrcore glut GL GLU pthread)
ENDIF(WIN32)
		
##################################
##   INSTALL MY APPS
##################################
INSTALL(TARGETS simulator teleop DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)



