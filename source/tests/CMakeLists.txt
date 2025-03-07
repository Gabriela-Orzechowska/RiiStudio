project(tests)

include_directories(${PROJECT_SOURCE_DIR}/../)
include_directories(${PROJECT_SOURCE_DIR}/../vendor)
include_directories(${PROJECT_SOURCE_DIR}/../plate/include)
include_directories(${PROJECT_SOURCE_DIR}/../plate/vendor)

add_executable(tests
	tests.cpp
)

set(ASSIMP_DIR, ${PROJECT_SOURCE_DIR}/../vendor/assimp)

target_link_libraries(tests PUBLIC
	core
  librii
	oishii
	rsl
	plate
	plugins
	vendor
  rsmeshopt
)
# Since we rely on rszst.exe
add_dependencies(tests cli)
# ???
add_dependencies(tests plugins)

add_dependencies(tests rsl)

if (WIN32)
  set(LINK_LIBS
		${PROJECT_SOURCE_DIR}/../plate/vendor/glfw/lib-vc2017/glfw3dll.lib
		${PROJECT_SOURCE_DIR}/../vendor/assimp/assimp-vc141-mt.lib
		opengl32.lib
    ntdll Crypt32 Secur32 Ncrypt # Rust needs this now?
		${PROJECT_SOURCE_DIR}/../vendor/freetype.lib
	)
  if (ASAN)
    set(LINK_LIBS ${LINK_LIBS} "C:\\Program Files\\LLVM\\lib\\clang\\10.0.0\\lib\\windows\\clang_rt.asan-x86_64.lib")
  endif()
  
	target_link_libraries(tests PUBLIC ${LINK_LIBS})
elseif (APPLE)
    execute_process(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    message(STATUS "Architecture: ${ARCHITECTURE}")

    if (${ARCHITECTURE} STREQUAL "arm64")
      set(HOMEBREW_CELLAR "/opt/homebrew/Cellar")
    else()
      set(HOMEBREW_CELLAR "/usr/local/Cellar")
    endif()

    set(ASSIMP_VERSION "5.2.5")
    set(GLFW_VERSION "3.3.8")
	set(FREETYPE_VERSION "2.13.0_1")

    SET_TARGET_PROPERTIES(tests PROPERTIES LINK_FLAGS "-framework CoreFoundation -ldl -lcurl ${HOMEBREW_CELLAR}/assimp/${ASSIMP_VERSION}/lib/libassimp.dylib ${HOMEBREW_CELLAR}/glfw/${GLFW_VERSION}/lib/libglfw.dylib ${HOMEBREW_CELLAR}/freetype/${FREETYPE_VERSION}/lib/libfreetype.dylib")
elseif (NOT UNIX)
	target_link_libraries(tests PUBLIC
		${PROJECT_SOURCE_DIR}/../vendor/assimp/libassimp.a
	  ${PROJECT_SOURCE_DIR}/../vendor/assimp/libIrrXML.a
	  ${PROJECT_SOURCE_DIR}/../vendor/assimp/libzlib.a
	  ${PROJECT_SOURCE_DIR}/../vendor/assimp/libzlibstatic.a
	)
elseif(UNIX)
	# TODO: code duplication, extract FindBzip2.cmake to top level CMakeLists
	find_library(BZIP2_LIBRARY NAMES libbz2.so PATHS /usr/lib)
	target_link_libraries(tests PUBLIC ${BZIP2_LIBRARY})
endif()

if (UNIX)
  # --start-group, --end-group allows circular dependencies among object files
  SET_TARGET_PROPERTIES(tests PROPERTIES LINK_FLAGS "-Wl,--start-group -ldl -lassimp -lglfw -lfreetype -lstdc++ -lm -lpthread")
  # This is a hack to append a final link arg
  target_link_libraries(tests PUBLIC "-Wl,--end-group")
endif()

# DLLs for windows
# if (WINDOWS)
	add_custom_command(
	  TARGET tests 
	  POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy
		  ${PROJECT_SOURCE_DIR}/../vendor/assimp/assimp-vc141-mt.dll
		  $<TARGET_FILE_DIR:tests>/assimp-vc141-mt.dll
	)
	add_custom_command(
	  TARGET tests 
	  POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy
		  ${PROJECT_SOURCE_DIR}/../plate/vendor/glfw/lib-vc2017/glfw3.dll
		  $<TARGET_FILE_DIR:tests>/glfw3.dll
	)
	add_custom_command(
	  TARGET tests 
	  POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy_directory
		  ${PROJECT_SOURCE_DIR}/../../fonts
		  $<TARGET_FILE_DIR:tests>/fonts
	)
  add_custom_command(
	  TARGET tests 
	  POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy_directory
		  ${PROJECT_SOURCE_DIR}/../vendor/dll
		  $<TARGET_FILE_DIR:tests>
  )
add_custom_command(TARGET tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:tests> $<TARGET_FILE_DIR:tests> 
  COMMAND_EXPAND_LISTS
)

if (WINDOWS)
  add_custom_command(
	  TARGET tests 
	  POST_BUILD
	  COMMAND python.exe ${PROJECT_SOURCE_DIR}/../../tests/tests.py
		  $<TARGET_FILE_DIR:tests>/tests.exe
		  ${PROJECT_SOURCE_DIR}/../../tests/samples
      ${PROJECT_SOURCE_DIR}/../../tests/out
	)
else()
  add_custom_command(
	  TARGET tests 
	  POST_BUILD
	  COMMAND python3 ${PROJECT_SOURCE_DIR}/../../tests/tests.py
		  $<TARGET_FILE_DIR:tests>/tests
      $<TARGET_FILE_DIR:cli>/rszst
		  ${PROJECT_SOURCE_DIR}/../../tests/samples
      ${PROJECT_SOURCE_DIR}/../../tests/out
	)
endif()
# endif()
