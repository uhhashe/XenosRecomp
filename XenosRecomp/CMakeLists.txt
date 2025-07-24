project(XenosRecomp)

add_executable(XenosRecomp 
    constant_table.h
    fxc_compiler.cpp
    fxc_compiler.h
    main.cpp
    pch.h
    shader.h
    shader_code.h
    shader_recompiler.cpp
    shader_recompiler.h)

target_link_libraries(XenosRecomp PRIVATE
    d3dcompiler.lib
    fmt::fmt)

target_precompile_headers(XenosRecomp PRIVATE pch.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(XenosRecomp PRIVATE -Wno-switch -Wno-unused-variable -Wno-null-arithmetic -fms-extensions)

    include(CheckCXXSymbolExists)
    check_cxx_symbol_exists(_LIBCPP_VERSION version LIBCPP)
    if(LIBCPP)
        # Allows using std::execution
        target_compile_options(XenosRecomp PRIVATE -fexperimental-library)
    endif()
endif()

if (WIN32)
    target_compile_definitions(XenosRecomp PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()