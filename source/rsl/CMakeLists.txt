project(rsl)

include_directories(${PROJECT_SOURCE_DIR}/../)
include_directories(${PROJECT_SOURCE_DIR}/../vendor)
include_directories(${PROJECT_SOURCE_DIR}/../plate/include)
include_directories(${PROJECT_SOURCE_DIR}/../plate/vendor)


add_library(rsl STATIC
  "FsDialog.cpp"
 "Defer.hpp" "DebugBreak.hpp" "Ranges.hpp" "Stb.cpp" "SafeReader.cpp" "Launch.cpp" "Download.cpp" "Zip.cpp" "Log.cpp"
 
 "Discord.cpp"
 )
target_link_libraries(rsl PUBLIC core range-v3 riistudio_rs vendor)
