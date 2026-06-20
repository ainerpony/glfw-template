# For Release build
cmake -B build_win;

# For dev build
# cmake -B build_win -DEXEC_OUTPUT_DIR="../src";

if ($LASTEXITCODE -eq 0) {
    cmake --build build_win --config Release -j8;
}