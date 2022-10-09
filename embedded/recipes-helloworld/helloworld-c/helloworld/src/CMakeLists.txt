add_executable (helloworldc helloworld.c)

install(TARGETS helloworldc DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
