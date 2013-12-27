
set(target Example_2DViewport)
message(STATUS "example ${target}")

add_executable(${target} 
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