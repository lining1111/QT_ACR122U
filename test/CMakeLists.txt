#pcsc demo
add_executable(pcsc_demo pcsc_demo.cpp)
target_link_libraries(pcsc_demo ${LINK_LIBS})

#test_nfc
add_executable(test_nfc test_nfc.cpp
                        ../src/nfc/nfc-utils.cpp
                        ../src/nfc/mifare.cpp
                        ../src/nfc/NFC_Operate.cpp)
target_link_libraries(test_nfc ${LINK_LIBS})

#test_nfc
add_executable(test_cjson test_cjson.cpp)
target_link_libraries(test_cjson cjson)