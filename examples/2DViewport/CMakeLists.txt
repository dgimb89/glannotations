
set(target Example_2DViewport)
message(STATUS "example ${target}")

add_executable(${target} 
    main.cpp
)

target_link_libraries(${target}
	${GLOW_LIBRARIES}     
    ${GLEW_LIBRARY}
    glat
)
