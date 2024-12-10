#ifndef _PDB_RTL_H_
#define _PDB_RTL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "no_sdl.h"

enum RTL_INIT{
	Uninitialized,
	InitializeFault,
	InitializeSuccess
};






typedef BOOLEAN(
WINAPI
*pfnWow64EnableWow64FsRedirection )(
     BOOLEAN Wow64FsEnableRedirection
    );

typedef BOOL(
WINAPI
*pfnWow64DisableWow64FsRedirection)(
     PVOID *OldValue
    );

typedef BOOL(
WINAPI
*pfnWow64RevertWow64FsRedirection)(
_In_ PVOID OlValue
    );

#ifdef __cplusplus
}
#endif
	
	
#endif
