
set(target Example_Presentation_Scene)
message(STATUS "example ${target}")

add_executable(${target} 
	building.h
	building.cpp
    main.cpp
)

target_link_libraries(${target}
	general
		${GLEW_LIBRARY}
    	glat
	debug
		${GLOW_LIBRARIES_DEBUG}
	optimized
		${GLOW_LIBRARIES}
)