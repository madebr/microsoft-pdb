#pragma once

#define __PDBIMPL_INCLUDED__

#include "prefast.h"

namespace pdb_internal { }

using namespace pdb_internal;

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#define INCR_COMPILE

#ifndef PDB_MT
//#define PDB_TYPESERVER                  // uncomment this to get rid of looking up types 
                                        // from additional type servers
#endif

#include "SysPage.h"
#include <cvinfo.h>

#define MR_ENGINE_IMPL
#include <pdb.h>                        // includes PDB and MR definitions

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <tchar.h>
#include <limits.h>

#include "pdbtypdefs.h"
#include "assert_.h"

#include "trace.h"

#define UTF8_IMP
#include "utf8.h"

#define precondition(x) assert(x)
#define postcondition(x) assert(x)
#define notReached() assert(0)

inline bool implies(int a, int b) {
    // a implies b: if a is true, b must be true
    return a <= b;
}

#if defined(INSTRUMENTED)
#define instrumentation(x) x
#else
#define instrumentation(x)
#endif

#include "crtwrap.h"

#include "mdalign.h"
#include "pdbheap.h"
#include "buffer.h"
#include "pool.h"
#include "safestk.h"
#include "szst.h"

#include "ptr.h"
#include "mts.h"

typedef struct OMFSegMapDesc {
    union {
        WORD  fAll;
    } flags;           // descriptor flags bit field
    WORD  ovl;         // the logical overlay number
    WORD  group;       // group index into the descriptor array
    WORD  frame;       // logical segment index - interpreted via flags
    WORD  iSegName;    // segment or group name - index into sstSegName
    WORD  iClassName;  // class name - index into sstSegName
    DWORD offset;      // byte offset of the logical within the physical segment
    DWORD cbSeg;       // byte count of the logical segment or group
} OMFSegMapDesc;

#if defined(_WIN64)
const size_t s_cbMaxAlloc = 0xFFFFFFFFFFFFFFFFUI64 - 0xF;
#else
const size_t s_cbMaxAlloc = 0xFFFFFFFF - 0x7;
#endif
