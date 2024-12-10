#ifndef _CVEXEFMT_H_
#define _CVEXEFMT_H_

typedef struct OMFDirEntry {
    unsigned short SubSection;
    unsigned short iMod;
    unsigned long lfo;
    unsigned long cb;
} OMFDirEntry;

typedef struct OMFSourceModule {
    unsigned short cFile;
    unsigned short cSeg;
    unsigned long baseSrcFile[];
} OMFSourceModule;

typedef struct OMFDirHeader {
    unsigned long field_0x0;
    unsigned long cDir;
    unsigned long lfoNextDir;
    unsigned long field_0xc;
} OMFDirHeader;

typedef struct DirEntry {
    unsigned short SubSectionType;
    unsigned short ModuleIndex;
    unsigned long  lfoStart;
    unsigned short Size;
} DirEntry;

typedef struct OMFSegDesc {
    unsigned short Seg;
    unsigned short field_0x2;
    unsigned long Off;
    unsigned long cbSeg;
} OMFSegDesc;

typedef struct OMFSourceFile {
    unsigned short cFile;
    unsigned short cSeg;
    unsigned char baseSrcLn[];
} OMFSourceFile;

typedef struct OMFSourceLine {
    unsigned short Seg;
    unsigned short cLnOff;
    unsigned long offset[];
} OMFSourceLine;

typedef struct COFF_GUID {
    unsigned __LONG32 Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} COFF_GUID;

// d1baa1c7-baee-4ba9-af20faf66aa4dcb8
// c7 a1 ba d1  ee ba  a9 4b  af 20 fa f6 6a a4 dc b8
static COFF_GUID EXTENDED_COFF_OBJ_GUID = { 0xd1baa1c7, 0xbaee, 0x4ba9, { 0xaf, 0x20, 0xfa, 0xf6, 0x6a, 0xa4, 0xdc, 0xb8 } };

typedef struct IMAGE_FILE_HEADER_EX {
    unsigned char field_0x0[0x2c];
    unsigned long NumberOfSections;
    unsigned char field_0x30[0x4];
} IMAGE_FILE_HEADER_EX;

typedef struct OMFSegMap {
    unsigned short cSeg;
    unsigned short cSegLog;
    struct OMFSeg {
        union {
            unsigned short fAll;
        } flags;
        unsigned short ovl;
        unsigned short group;
        unsigned short frame;
        unsigned short iSegName;
        unsigned short iClassName;
        unsigned short offset;
        unsigned char field_0xe;
        unsigned char field_0xf;
        unsigned long cbSeg;
    } rgDesc[];
} OMFSegMap;

typedef struct OMFSymHash {
    unsigned short symhash;
    unsigned short addrhash;
    unsigned long cbSymbol;
    unsigned long cbHSym;
    unsigned long cbHAddr;
} OMFSymHash;

enum {
    SSTMODULE = 0x101,
    SSTPUBLIC = 0x102,
    SSTTYPES = 0x103,
    SSTSYMBOLS = 0x104,
    SSTLIBRARIES = 0x106,
    SSTSRCLNSEG = 0x109,

    sstModule = 0x120,
    sstTypes = 0x121,
    sstPublic = 0x122,
    sstPublicSym = 0x123,
    sstAlignSym = 0x124,
    sstSymbols = 0x125,
    sstSrcLnSeg = 0x126,
    sstSrcModule = 0x127,
    sstLibraries = 0x128,
    sstGlobalSym = 0x129,
    sstGlobalPub = 0x12a,
    sstGlobalTypes = 0x12b,
    sstMPC = 0x12c,
    sstSegMap = 0x12d,
    sstSegName = 0x12e,
    sstPreComp = 0x12f,
    sstFileIndex = 0x133,
    sstStaticSym = 0x134,
};

#endif
