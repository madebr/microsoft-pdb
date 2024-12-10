add_executable(pdbtest pdbtest.cpp)
target_include_directories(pdbtest PRIVATE "${PROJECT_SOURCE_DIR}/langapi/include")
target_include_directories(pdbtest PRIVATE "${PROJECT_SOURCE_DIR}/include")
