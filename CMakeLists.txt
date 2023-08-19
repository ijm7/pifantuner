cmake_minimum_required(VERSION 3.21)
project(pifantunerd C)

add_library(pifantuner SHARED src/pifantuner.c)
target_include_directories(pifantuner PUBLIC include)
set_target_properties(pifantuner PROPERTIES PUBLIC_HEADER "include/pifantuner.h")
set_property(TARGET pifantuner PROPERTY C_STANDARD 23)
#install(TARGETS pifantuner)

add_executable(pifantunerd src/main.c src/pifantuner.c)
set_property(TARGET pifantunerd PROPERTY C_STANDARD 23)
target_include_directories(pifantunerd PUBLIC include)
install(TARGETS pifantunerd DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )

