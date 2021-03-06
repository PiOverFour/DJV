include(ExternalProject)

if(NOT DJV_THIRD_PARTY_DISABLE_BUILD)
    ExternalProject_Add(
        ZLIBThirdParty
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ZLIB
        URL http://www.zlib.net/zlib-1.2.11.tar.gz
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/third-party/zlib-patch/CMakeLists.txt
            ${CMAKE_CURRENT_BINARY_DIR}/ZLIB/src/ZLIBThirdParty/CMakeLists.txt
        CMAKE_ARGS
            -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
            -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
            -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
            -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
            -DBUILD_SHARED_LIBS=${ZLIB_SHARED_LIBS})
endif()

set(ZLIB_FOUND TRUE)
set(ZLIB_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
if(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
		set(ZLIB_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/zlibd${CMAKE_STATIC_LIBRARY_SUFFIX})
	else()
		set(ZLIB_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/zlib${CMAKE_STATIC_LIBRARY_SUFFIX})
	endif()
else()
	set(ZLIB_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libz${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()
set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})

if(ZLIB_FOUND AND NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB UNKNOWN IMPORTED)
    add_dependencies(ZLIB::ZLIB ZLIBThirdParty)
    set_target_properties(ZLIB::ZLIB PROPERTIES
        IMPORTED_LOCATION "${ZLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIR}")
endif()
if(ZLIB_FOUND AND NOT TARGET ZLIB)
    add_library(ZLIB INTERFACE)
    target_link_libraries(ZLIB INTERFACE ZLIB::ZLIB)
endif()

