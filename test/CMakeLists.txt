add_executable(test_acr122u test_acr122u.cpp)
target_link_libraries(test_acr122u ${LINK_LIBS})

add_executable(pcsc_demo pcsc_demo.cpp)
target_link_libraries(pcsc_demo ${LINK_LIBS})