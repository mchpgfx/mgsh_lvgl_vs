# User customizations for LVGL integration

# Include the generated rule.cmake to get compile options
include("${CMAKE_CURRENT_SOURCE_DIR}/.generated/rule.cmake")

set(LVGL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../lvgl")

# Collect all LVGL core source files
file(GLOB_RECURSE LVGL_SOURCES
    "${LVGL_ROOT}/src/*.c"
)

# Collect all demos source files
file(GLOB_RECURSE LVGL_DEMO_SOURCES
    "${LVGL_ROOT}/demos/*.c"
)

# Combine all LVGL sources
set(ALL_LVGL_SOURCES
    ${LVGL_SOURCES}
    ${LVGL_DEMO_SOURCES}
)

# Create LVGL library
add_library(lvgl_lib OBJECT ${ALL_LVGL_SOURCES})

# Set include directories for LVGL library
target_include_directories(lvgl_lib
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../curiosity_wqvga_lcc/src"
    PRIVATE "${LVGL_ROOT}"
    PRIVATE "${LVGL_ROOT}/src"
    PRIVATE "${LVGL_ROOT}/demos"
)

# Apply same compile options as the main project using the generated rule
mgsh_pic32ck_sg_gc_lvgl_default_default_XC32_compile_rule(lvgl_lib)

# Add include directories to main compile targets
function(add_lvgl_includes target)
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../.."
        PRIVATE "${LVGL_ROOT}"
        PRIVATE "${LVGL_ROOT}/src"
        PRIVATE "${LVGL_ROOT}/demos"
    )
endfunction()

if(TARGET mgsh_pic32ck_sg_gc_lvgl_default_default_XC32_compile)
    add_lvgl_includes(mgsh_pic32ck_sg_gc_lvgl_default_default_XC32_compile)
endif()

if(TARGET mgsh_pic32ck_sg_gc_lvgl_default_default_XC32_compile_cpp)
    add_lvgl_includes(mgsh_pic32ck_sg_gc_lvgl_default_default_XC32_compile_cpp)
endif()

# Link LVGL objects to the main executable target
get_property(all_targets DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY BUILDSYSTEM_TARGETS)
foreach(target ${all_targets})
    if(target MATCHES "^mgsh_pic32ck_sg_gc_lvgl_default_image_")
        target_sources(${target} PRIVATE $<TARGET_OBJECTS:lvgl_lib>)
        break()
    endif()
endforeach()
