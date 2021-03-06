include(ExternalProject)

if(NOT DJV_THIRD_PARTY_DISABLE_BUILD)
    ExternalProject_Add(
        TIFFThirdParty
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/TIFF
        DEPENDS ZLIBThirdParty JPEGThirdParty
        URL http://download.osgeo.org/libtiff/tiff-4.0.10.tar.gz
        PATCH_COMMAND
            ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/third-party/tiff-patch/CMakeLists.txt
            ${CMAKE_CURRENT_BINARY_DIR}/TIFF/src/TIFFThirdParty/CMakeLists.txt
        CMAKE_ARGS
            -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
            -DCMAKE_INSTALL_LIBDIR=lib
            -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
            -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
            -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
            -DBUILD_SHARED_LIBS=${TIFF_SHARED_LIBS}
            -DZLIB_SHARED_LIBS=${ZLIB_SHARED_LIBS}
            -DJPEG_SHARED_LIBS=${JPEG_SHARED_LIBS}
            -Djbig=OFF
            -Dlzma=OFF
            -Dzstd=OFF
            -Dwebp=OFF
            -Djpeg12=OFF)
endif()

set(TIFF_FOUND TRUE)
set(TIFF_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
if(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
		set(TIFF_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/tiffd${CMAKE_STATIC_LIBRARY_SUFFIX})
	else()
		set(TIFF_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/tiff${CMAKE_STATIC_LIBRARY_SUFFIX})
	endif()
else()
	set(TIFF_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/libtiff${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()
set(TIFF_LIBRARIES ${TIFF_LIBRARY} ${ZLIB_LIBRARIES})

if(TIFF_FOUND AND NOT TARGET TIFF::TIFF)
    add_library(TIFF::TIFF UNKNOWN IMPORTED)
    add_dependencies(TIFF::TIFF TIFFThirdParty)
    set_target_properties(TIFF::TIFF PROPERTIES
        IMPORTED_LOCATION "${TIFF_LIBRARY}"
        INTERFACE_LINK_LIBRARIES "JPEG;ZLIB"
        INTERFACE_INCLUDE_DIRECTORIES "${TIFF_INCLUDE_DIRS}"
        INTERFACE_COMPILE_DEFINITIONS TIFF_FOUND)
endif()
if(TIFF_FOUND AND NOT TARGET TIFF)
    add_library(TIFF INTERFACE)
    target_link_libraries(TIFF INTERFACE TIFF::TIFF)
endif()

