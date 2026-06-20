# Release build
cmake -B build_linux -D CMAKE_BUILD_TYPE=Release &&

# Dev build
# cmake -B build_linux -D CMAKE_BUILD_TYPE=Debug -D EXEC_OUTPUT_DIR="../src" &&

cmake --build build_linux -j8