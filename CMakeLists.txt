PROJECT(ParticleInitializer) 

FIND_PACKAGE(ITK) 
IF(ITK_FOUND)
	INCLUDE(${ITK_USE_FILE}) 
ELSE(ITK_FOUND)
	MESSAGE(FATAL_ERROR "ITK not found. Please set ITK_DIR.") 
ENDIF(ITK_FOUND) 

ADD_EXECUTABLE(ParticleInitializer main.cxx Computation.cxx)
TARGET_LINK_LIBRARIES(ParticleInitializer ${ITK_LIBRARIES})
