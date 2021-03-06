cmake_minimum_required(VERSION 3.19)
project(QT_ACR122U)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH "/home/lining/Qt5.14.2/5.14.2/gcc_64/bin")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)

include_directories(/usr/local/include/PCSC)
include_directories(include)

set(SRC_DIRS src
        src/nfc
        )

foreach (src_dir ${SRC_DIRS})
    aux_source_directory(${src_dir} src_dir_file)
    set(SRC_FILES ${SRC_FILES} ${src_dir_file})
endforeach (src_dir)


set(LINK_LIBS m pthread pcsclite nfc)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} ${LINK_LIBS})

add_subdirectory(test)