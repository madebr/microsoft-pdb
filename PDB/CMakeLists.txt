add_library(mspdb SHARED
    msf/mapfile.cpp
    msf/msf.cpp

    dbi/debug_s.cpp
    dbi/dbi.cpp
    dbi/locator.cpp
    dbi/mod.cpp
    dbi/modcommon.cpp
    dbi/pdbcommon.cpp
    dbi/pdbrtl.cpp
    dbi/pdb.cpp
    dbi/sha256.cpp
    dbi/tpi.cpp
    dbi/utf8.cpp
)
set_property(TARGET mspdb PROPERTY DEFINE_SYMBOL "PDB_SERVER")

target_include_directories(mspdb PRIVATE "${PROJECT_SOURCE_DIR}/PDB/dbi")
target_include_directories(mspdb PRIVATE "${PROJECT_SOURCE_DIR}/PDB/include")
target_include_directories(mspdb PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(mspdb PRIVATE "${PROJECT_SOURCE_DIR}/langapi/include")
target_include_directories(mspdb PRIVATE "${PROJECT_SOURCE_DIR}/cvdump")

#target_compile_definitions(mspdb PRIVATE PDB_LIBRARY)
target_compile_definitions(mspdb PRIVATE _CRT_ALTERNATIVE_INLINES)
target_compile_options(mspdb PRIVATE -Wno-multichar)
