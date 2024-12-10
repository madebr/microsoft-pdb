add_executable(cvdump
    cvdump.cpp

    dumppdb.cpp
    dumpsym6.cpp
    dumpsym7.cpp
    dumptyp6.cpp
    dumptyp7.cpp
    output.cpp
    type6.cpp
    type7.cpp
    utils.cpp
    utf8.cpp
    cvdump.rc
)
target_include_directories(cvdump PRIVATE "${PROJECT_SOURCE_DIR}/langapi/include")
target_include_directories(cvdump PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(cvdump PRIVATE "${PROJECT_SOURCE_DIR}/PDB/include")

target_compile_options(cvdump PRIVATE -Wno-multichar)
target_link_options(cvdump PRIVATE -municode)
target_link_libraries(cvdump PRIVATE mspdb)
