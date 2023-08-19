cmake_minimum_required(VERSION 3.21)
project(pifantuner C)

find_package(libgpiod REQUIRED)

add_library(pifantuner SHARED src/pifantuner.c)
target_link_libraries(pifantuner libgpiod::gpiod)
target_include_directories(pifantuner PUBLIC include)
set_target_properties(pifantuner PROPERTIES PUBLIC_HEADER "include/pifantuner.h")
set_property(TARGET pifantuner PROPERTY C_STANDARD 23)
install(TARGETS pifantuner)

add_executable(pifantunerd src/main.c)
set_property(TARGET pifantunerd PROPERTY C_STANDARD 23)
target_include_directories(pifantunerd PUBLIC include)
target_link_libraries(pifantunerd PRIVATE pifantuner ${LIB})
install(TARGETS pifantunerd DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )

