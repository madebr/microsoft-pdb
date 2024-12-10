#include "cvdump.h"
#include "cvinfo.h"

#include <windows.h>

void DumpFieldListT7(DWORD length, LPCVOID data);
void StdOutClassFieldAttributesT7(bool b, CV_fldattr_t attr);
void StdOutHexBlockT7(unsigned char *buffer, WORD size);
void StdOutClassProperties(CV_prop_t prop);
const wchar_t *SzCallConvention(DWORD t);
const wchar_t *SzFunctionAttr(CV_funcattr_t attr);
void DumpCobolT7(DWORD *pLength, const unsigned char *data);
void DumpCobolCompileFlagsT7(DWORD length, const unsigned char *data);

typedef union TypeRecord {
    unsigned short leaf;
    lfMFuncId lf_mfuncid;
    lfBuildInfo lf_buildinfo;
    lfArgList lf_arglist;
    lfStringId lf_stringid;
    lfUdtSrcLine lf_udtsrcline;
    lfUdtModSrcLine lf_udtmodsrcline;
    lfFuncId lf_funcid;
    lfManaged lf_managed;
    lfTypeServer lf_typeserver;
    lfTypeServer2 lf_typeserver2;
    lfStridedArray lf_stridedarray;
    lfHLSL lf_hlsl;
    lfModifierEx lf_modifierex;
    lfVector lf_vector;
    lfMatrix lf_matrix;
    lfVftable lf_vftable;
    lfDefArg lf_defarg;
    lfFieldList lf_fieldlist;
    lfDerived lf_derived;
    lfBitfield lf_bitfield;
    lfMethodList lf_methodlist;
    lfDimCon lf_dimcon;
    lfDimVar lf_dimvar;
    lfSkip lf_skip;
    lfPointer lf_pointer;
    lfArray lf_array;
    lfClass lf_class;
    lfUnion lf_union;
    lfEnum lf_enum;
    lfProc lf_proc;
    lfMFunc lf_mfunc;
    lfCobol0 lf_cobol0;
    lfBArray lf_barray;
    lfDimArray lf_dimarray;
    lfPreComp lf_precomp;
    lfOEM lf_oem;
    lfAlias lf_alias;
    lfModifier lf_modifier;
    lfArgList_16t lf_arglist_16t;
    lfDefArg_16t lf_defarg_16t;
    lfList lf_list;
    lfFieldList_16t lf_fieldlist_16t;
    lfDerived_16t lf_derived_16t;
    lfBitfield_16t lf_bitfield_16t;
    lfMethodList_16t lf_methodlist_16t;
    lfDimCon_16t lf_dimcon_16t;
    lfDimVar_16t lf_dimvar_16t;
    lfRefSym lf_refsym;
    lfSkip_16t lf_skip_16t;
    lfModifier_16t lf_modifier_16t;
    lfPointer_16t lf_pointer_16t;
    lfArray_16t lf_array_16t;
    lfClass_16t lf_class_16t;
    lfUnion_16t lf_union_16t;
    lfEnum_16t lf_enum_16t;
    lfProc_16t lf_proc_16t;
    lfMFunc_16t lf_mfunc_16t;
    lfVTShape lf_vtshape;
    lfCobol0_16t lf_cobol0_16t;
    lfCobol1 lf_cobol1;
    lfBArray_16t lf_barray_16t;
    lfLabel lf_label;
    lfDimArray_16t lf_dimarray_16t;
    lfPreComp_16t lf_precomp_16t;
    lfEndPreComp lf_endprecomp;
    lfOEM_16t lf_oem_16t;
} TypeRecord;

typedef union FieldListElement {
    unsigned short leaf;
    lfVBClass lf_vbclass;
    lfFriendFcn lf_friendfcn;
    lfIndex lf_index;
    lfMember lf_member;
    lfSTMember lf_stmember;
    lfMethod lf_method;
    lfNestType lf_nesttype;
    lfVFuncTab lf_vfunctab;
    lfFriendCls lf_friendcls;
    lfOneMethod lf_onemethod;
    lfNestTypeEx lf_nesttypeex;
    lfBClass lf_bclass;
    lfBClass_16t lf_bclass_16t;
    lfVBClass_16t lf_vbclass_16t;
    lfEnumerate lf_enumerate;
    lfFriendFcn_16t lf_friendfcn_16t;
    lfIndex_16t lf_index_16t;
    lfMember_16t lf_member_16t;
    lfSTMember_16t lf_stmember_16t;
    lfMethod_16t lf_method_16t;
    lfNestType_16t lf_nesttype_16t;
    lfVFuncTab_16t lf_vfunctab_16t;
    lfFriendCls_16t lf_friendcls_16t;
    lfOneMethod_16t lf_onemethod_16t;
} FieldListElement;

NameMap *GNameMapC7;

const wchar_t *const rgszCallConventionNames[] =
{
    L"C Near",
    L"C Far",
    L"Pascal Near",
    L"Pascal Far",
    L"Fast Near",
    L"Fast Far",
    L"???",
    L"STD Near",
    L"STD Far",
    L"SYS Near",
    L"SYS Far",
    L"ThisCall",
    L"MIPS CALL",
    L"Generic",
    L"Alpha Call",
    L"PPC Call",
    L"SH Call",
    L"ARM Call",
    L"AM33 Call",
    L"TriCore Call",
    L"SH5 Call",
    L"M32R Call",
    L"CLR Call",
    L"No callconv; always Inlined",
    L"Vector Near",
};

const wchar_t *const rgszMethodProp[] =
{
    L"VANILLA",
    L"VIRTUAL",
    L"STATIC",
    L"FRIEND",
    L"INTRODUCING VIRTUAL",
    L"PURE VIRTUAL",
    L"PURE INTRO",
};

const wchar_t *const rgszHlslTypeNames[] =
{
    L"INTERFACE_POINTER"
    L"TEXTURE1D"
    L"TEXTURE1D_ARRAY"
    L"TEXTURE2D"
    L"TEXTURE2D_ARRAY"
    L"TEXTURE3D"
    L"TEXTURECUBE"
    L"TEXTURECUBE_ARRAY"
    L"TEXTURE2DMS"
    L"TEXTURE2DMS_ARRAY"
    L"SAMPLER"
    L"SAMPLERCOMPARISON"
    L"BUFFER"
    L"POINTSTREAM"
    L"LINESTREAM"
    L"TRIANGLESTREAM"
    L"INPUTPATCH"
    L"OUTPUTPATCH"
    L"RWTEXTURE1D"
    L"RWTEXTURE1D_ARRAY"
    L"RWTEXTURE2D"
    L"RWTEXTURE2D_ARRAY"
    L"RWTEXTURE3D"
    L"RWBUFFER"
    L"BYTEADDRESS_BUFFER"
    L"RWBYTEADDRESS_BUFFER"
    L"STRUCTURED_BUFFER"
    L"RWSTRUCTURED_BUFFER"
    L"APPEND_STRUCTURED_BUFFER"
    L"CONSUME_STRUCTURED_BUFFER"
    L"MIN8FLOAT"
    L"MIN10FLOAT"
    L"MIN16FLOAT"
    L"MIN12INT"
    L"MIN16INT"
    L"MIN16UINT"
    L"CONSTANT_BUFFER",
};

const wchar_t *const rgszPointerMode[] =
{
    L"Pointer",
    L"L-value Reference",
    L"Pointer to member",
    L"Pointer to member function",
    L"R-value Reference",
    L"???",
    L"???",
    L"???",
};

const wchar_t *const rgszPointerType[] =
{
    L"__far",
    L"__huge",
    L"BasedSeg",
    L"BasedVal",
    L"BasedSegVal",
    L"BasedAddr",
    L"BasedSegAdr",
    L"BasedOnType",
    L"BasedOnSelf",
    L"NEAR32",
    L"FAR32",
    L"__ptr64",
    L"???",
    L"???",
    L"???",
};

const wchar_t *const rgszPointerToMemberType[] = {
    L"Not specified",
    L"Data, Single inheritance",
    L"Data, Multiple inheritance",
    L"Data, Virtual inheritance",
    L"Data, Most general",
    L"Function, Single inheritance",
    L"Function, Multiple inheritance",
    L"Function, Virtual inheritance",
    L"Function, Most general",
};

const wchar_t *const rgszVtFuncType[] =
{
    L"NEAR",
    L"FAR",
    L"THIN",
    L"ADDRESS POINT DISPLACEMENT",
    L"POINTER TO METACLASS DESCRIPTOR",
    L"NEAR32",
    L"FAR32",
    L"??? (0x7)",
    L"??? (0x8)",
    L"??? (0x9)",
    L"??? (0xa)",
    L"??? (0xb)",
    L"??? (0xc)",
    L"??? (0xd)",
    L"??? (0xe)",
    L"??? (0xf)",
};

static const wchar_t * const rgszAccess[] =
{
        L"none",
        L"private",
        L"protected",
        L"public"
};

CV_typ_t DumpTypRecC7(CV_typ_t ti, WORD length, BYTE *arg3, TPI *ptpi, PDB *ppdb) {
    TypeRecord *r = (TypeRecord *) arg3;
    StdOutPrintf(L"0x%0*x : Length = %u, Leaf = 0x%04x ", ti & 0x8000 ? 4 : 8, ti, length, r->leaf);
    switch (r->leaf) {
    default:
        StdOutPrintf(L"%s", L"\"UNRECOGNIZED TYPE\"");
        break;
    case LF_MFUNC_ID:
        StdOutPrintf(L"%s\n", L"LF_MFUNC_ID");
        StdOutPrintf(L"\tType = %s\t", SzNameC7Type(r->lf_mfuncid.type));
        StdOutPrintf(L"\tParent = %s\t", SzNameC7Type(r->lf_mfuncid.parentType));
        PrintSt(true, r->lf_mfuncid.name);
        break;
    case LF_BUILDINFO:
        StdOutPrintf(L"%s\n", L"LF_BUILDINFO");
        StdOutPrintf(L"\tString ID's (count = %u):", r->lf_buildinfo.count);
        for (int i = 0; i < r->lf_buildinfo.count; i++) {
            StdOutPrintf(L" %s", SzNameC7Type(r->lf_buildinfo.arg[i]));
        }
        StdOutPuts(L"\n");
        break;
    case LF_SUBSTR_LIST:
        StdOutPrintf(L"%s\n", L"LF_SUBSTR_LIST");
        StdOutPrintf(L"\tString ID's (count = %u):", r->lf_arglist.count);
        for (int i = 0; i < r->lf_arglist.count; i++) {
            StdOutPrintf(L" %s", SzNameC7Type(r->lf_arglist.arg[i]));
        }
        StdOutPrintf(L"\n");
        break;
    case LF_STRING_ID:
        StdOutPrintf(L"%s\n", L"LF_STRING_ID");
        StdOutPuts(L"\t");
        PrintSt(true, r->lf_stringid.name);
        if (r->lf_stringid.id) {
            StdOutPrintf(L"\tList of sub string ID's = %s\n", SzNameC7Type(r->lf_stringid.id));
        } else {
            StdOutPrintf(L"\tNo sub string\n");
        }
        break;
    case LF_UDT_SRC_LINE:
        StdOutPrintf(L"%s\n", L"LF_UDT_SRC_LINE");
        StdOutPrintf(L"\ttype = %s, ", SzNameC7Type(r->lf_udtsrcline.type));
        StdOutPrintf(L"source file = %s, line = %u\n", SzNameC7Type(r->lf_udtsrcline.src), r->lf_udtsrcline.line);
        break;
    case LF_UDT_MOD_SRC_LINE:
        StdOutPrintf(L"%s\n", L"LF_UDT_MOD_SRC_LINE");
        if (GNameMapC7 == NULL && !NameMap::open(ppdb, FALSE, &GNameMapC7)) {
            StdOutPuts(L"Error no namemap\n");
        } else {
            wchar_t source_file[0x400] = L"<Error: Invalid NI>";
            if (GNameMapC7->isValidNi(r->lf_udtmodsrcline.src)) {
                size_t s = _countof(source_file);
                GNameMapC7->getNameW(r->lf_udtmodsrcline.src, source_file, &s);
            }
            StdOutPrintf(L"\ttype = %s, mod = %u, source file = %s, line = %u\n",
                r->lf_udtmodsrcline.type, r->lf_udtmodsrcline.imod, source_file, r->lf_udtmodsrcline.line);
        }
        break;
    case LF_FUNC_ID:
        StdOutPrintf(L"%s\n", L"LF_FUNC_ID");
        StdOutPrintf(L"\tType = %s\t", SzNameC7Type(r->lf_funcid.type));
        if (r->lf_funcid.scopeId == 0) {
            StdOutPrintf(L"\tScope = global\t");
        } else {
            StdOutPrintf(L"\tScope = %s\t",SzNameC7Type(r->lf_funcid.scopeId));
        }
        PrintSt(true, r->lf_funcid.name);
        break;
    case LF_MANAGED:
        StdOutPrintf(L"%s\n", L"LF_MANAGED");
        StdOutPrintf(L"%s", L"\t managed name = ");
        PrintSt(true, r->lf_managed.Name, false);
        StdOutPutc(L'\n');
        break;
    case LF_TYPESERVER2:
        {
            LPOLESTR ole_str;
            StringFromIID(r->lf_typeserver2.sig70, &ole_str);
            StdOutPrintf(L"%s\n", L"LF_TYPESERVER2");
            StdOutPrintf(L"\t\tGUID=%s, age = 0x%08lx", ole_str, r->lf_typeserver2.age);
            CoTaskMemFree(ole_str);
            StdOutPrintf(L"%s", L", PDB name = '");
            PrintSt(true, r->lf_typeserver2.name);
        }
        break;
    case LF_STRIDED_ARRAY:
        {
            StdOutPrintf(L"%s\n", L"LF_STRIDED_ARRAY");
            StdOutPrintf(L"\tElement type = %s\n", SzNameC7Type(r->lf_stridedarray.elemtype));
            StdOutPrintf(L"\tIndex type = %s\n",SzNameC7Type(r->lf_stridedarray.idxtype));
            StdOutPrintf(L"\tStride = %u\n", r->lf_stridedarray.stride);
            StdOutPuts(L"\tlength = ");
            int l = PrintNumeric(r->lf_stridedarray.data);
            StdOutPrintf(L"%s", L"\n\tName = ");
            PrintSt(true, r->lf_stridedarray.data + l);
        }
        break;
    case LF_HLSL:
        {
            StdOutPrintf(L"%s\n", L"LF_HLSL");
            StdOutPrintf(L"\tSub type = %s, kind = ", SzNameC7Type(r->lf_hlsl.subtype));
            if (r->lf_hlsl.kind - CV_BI_HLSL_start < _countof(rgszHlslTypeNames)) {
                StdOutPrintf(L"%s", rgszHlslTypeNames[r->lf_hlsl.kind - CV_BI_HLSL_start]);
            } else {
                StdOutPrintf(L"****Warning**** unrecognized HLSL type kind value: %04X\n", r->lf_hlsl.kind);
            }
            StdOutPrintf(L"\tNumeric properties:");
            int o = 0;
            if (r->lf_hlsl.numprops == 0) {
                StdOutPrintf(L" (none)");
            } else {
                for (int i = 0; i < r->lf_hlsl.numprops; i++) {
                    StdOutPrintf(L" ");
                    o += PrintNumeric(r->lf_hlsl.data + o);
                }
            }
            StdOutPuts(L"\n\tlength = ");
            PrintNumeric(r->lf_hlsl.data + o);
            StdOutPuts(L"\n");
        }
        break;
    case LF_MODIFIER_EX:
        StdOutPrintf(L"%s\n", L"LF_MODIFIER_EX");
        StdOutPuts(L"\t");
        for (int i = 0; i < r->lf_modifierex.count; i++) {
            switch (r->lf_modifierex.mods[i]) {
            case CV_MOD_CONST:
                StdOutPuts(L" const");
                break;
            case CV_MOD_VOLATILE:
                StdOutPuts(L" volatile");
                break;
            case CV_MOD_UNALIGNED:
                StdOutPuts(L" __unaligned");
                break;
            default:
                if (r->lf_modifierex.mods[i] - CV_MOD_HLSL_UNIFORM < _countof(rgszHlslTypeNames)) {
                    StdOutPrintf(L" %s", rgszHlslTypeNames[r->lf_modifierex.mods[i] - CV_MOD_HLSL_UNIFORM]);;
                } else {
                    StdOutPrintf(L" ****Warning**** invalid HLSL modifier value: %04X", r->lf_modifierex.mods[i]);
                    break;
                }
            }
        }
        StdOutPrintf(L"\n\tmodifies type %s\n", SzNameC7Type(r->lf_modifierex.type));
        break;
    case LF_VECTOR:
        {
            StdOutPrintf(L"%s\n", L"LF_VECTOR");
            StdOutPrintf(L"\tElement type = %s\n", SzNameC7Type(r->lf_vector.elemtype));
            StdOutPrintf(L"\tCount of elements = %u\n", r->lf_vector.count);
            StdOutPuts(L"\tlength = ");
            int o = PrintNumeric(r->lf_vector.data);
            StdOutPrintf(L"%s",L"\n\tName = ");
            PrintSt(true, r->lf_vector.data + o);
        }
        break;
    case LF_MATRIX:
        {
            StdOutPrintf(L"%s\n", L"LF_MATRIX");
            StdOutPrintf(L"\tElement type = %s\n", SzNameC7Type(r->lf_matrix.elemtype));
            StdOutPrintf(L"\tNumber of rows = %u\n", r->lf_matrix.rows);
            StdOutPrintf(L"\tNumber of columns = %u\n", r->lf_matrix.cols);
            if (r->lf_matrix.matattr.row_major) {
                StdOutPuts(L"\trow-major layout\n");
            }
            StdOutPuts(L"\tlength = ");
            int o = PrintNumeric(r->lf_matrix.data);
            StdOutPrintf(L"%s", L"\n\tName = ");
            PrintSt(true, r->lf_matrix.data + o);
        }
        break;
    case LF_VFTABLE:
    {
        StdOutPrintf(L"%s\n", L"LF_VFTABLE");
        StdOutPrintf(L"\tType = 0x%04x, ", r->lf_vftable.type);
        StdOutPrintf(L"base vftable = 0x%04x, ", r->lf_vftable.baseVftable);
        StdOutPrintf(L"offset in object layout = %u, ", r->lf_vftable.offsetInObjectLayout);
        StdOutPrintf(L"len of contents = %u\n", r->lf_vftable.len);
        StdOutPrintf(L"\tUnique name = ");
        const unsigned char *n = PrintSt(true, r->lf_vftable.Names);
        for (int i = 0; n < &r->lf_vftable.Names[r->lf_vftable.len]; i++) {
            StdOutPrintf(L"\t%3d   ", i);
            n = PrintSt(true, n);
        }
        break;
    }
    case LF_ARGLIST:
        StdOutPrintf(L"LF_ARGLIST argument count = %d\n", r->lf_arglist.count);
        for (int i = 0; i < r->lf_arglist.count; i++) {
            StdOutPrintf(L"\tlist[%d] = %s\n", i, SzNameC7Type(r->lf_arglist.arg[i]));
        }
        break;
    case LF_DEFARG:
        StdOutPrintf(L"%s\n", L"LF_DEFARG");
        StdOutPrintf(L"type = %s, ", SzNameC7Type(r->lf_defarg.type));
        PrintSt(false, r->lf_defarg.expr);
        break;
    case LF_FIELDLIST:
        StdOutPrintf(L"%s\n", L"LF_FIELDLIST");
        DumpFieldListT7(length - offsetof(lfFieldList, data), r->lf_fieldlist.data);
        break;
    case LF_DERIVED:
        StdOutPrintf(L"%s\n", L"LF_DERIVED");
        for (int i = 0; i < r->lf_derived.count; i++) {
            StdOutPrintf(L"\tderived[%d] = %s\n", i, SzNameC7Type(r->lf_derived.drvdcls[i]));
        }
        break;
    case LF_BITFIELD:
        StdOutPrintf(L"%s\n", L"LF_BITFIELD");
        StdOutPrintf(L"\tbits = %d, ", r->lf_bitfield.length);
        StdOutPrintf(L"starting position = %d", r->lf_bitfield.position);
        StdOutPrintf(L", Type = %s\n", SzNameC7Type(r->lf_bitfield.type));
        break;
    case LF_METHODLIST:
        StdOutPrintf(L"%s\n", L"LF_METHODLIST");
        for (int i = 0, o = 0; o + offsetof(lfMethodList, mList) < length; i++) {
            mlMethod *m = (mlMethod *) r->lf_methodlist.mList + o;
            StdOutPrintf(L"\tlist[%u] = ",i);
            StdOutClassFieldAttributesT7(true, m->attr);
            StdOutPrintf(L"%s, ", SzNameC7Type(m->index));
            int l = offsetof(mlMethod, vbaseoff);
            if (m->attr.mprop == CV_MTintro || m->attr.mprop == CV_MTpureintro) {
                StdOutPrintf(L" vfptr offset = %ld", m->vbaseoff[0]);
                l += sizeof(long);
            }
            if (m->pad0 != 0) {
                StdOutPuts(L"***Warning, pad bytes are non-zero!\n");
            }
            StdOutPutc(L'\n');
            o += l;
        }
        break;
    case LF_DIMCONU:
        {
            StdOutPrintf(L"%s\n", L"LF_DIMCONU");
            StdOutPrintf(L"\tRank = %d\n", r->lf_dimcon.rank);
            StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimcon.typ));
            StdOutPuts(L"\tBounds = (");
            int s = 1 << (r->lf_dimcon.typ % 16);
            for (int i = 0, o = 0; i < r->lf_dimcon.rank;) {
                unsigned char buf[512];
                memcpy(buf, r->lf_dimcon.dim + o, s);
                o += s;
                StdOutPrintf(L"%d", buf);

                i += 1;
                if (i < r->lf_dimcon.rank) {
                    StdOutPuts(L", ");
                }
            }
            StdOutPuts(L")\n");
        }
        break;
    case LF_DIMCONLU:
        {
            StdOutPrintf(L"%s\n", L"LF_DIMCONLU");
            StdOutPrintf(L"\tRank = %d\n", r->lf_dimcon.rank);
            StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimcon.typ));
            StdOutPuts(L"\tBounds = (");
            int s = 1 << (r->lf_dimcon.typ & 0xf);
            for (int i = 0, o = 0; i < r->lf_dimcon.rank;) {
                unsigned char buf[512];
                memcpy(buf, r->lf_dimcon.dim + o, s);
                o += s;
                StdOutPrintf(L"%d", buf);

                StdOutPuts(L":");

                memcpy(buf, r->lf_dimcon.dim + o, s);
                o += s;
                StdOutPrintf(L"%d", buf);

                i += 1;
                if (i < r->lf_dimcon.rank) {
                    StdOutPuts(L", ");
                }
            }
            StdOutPuts(L")\n");
        }
        break;
    case LF_DIMVARU:
        StdOutPrintf(L"%s\n", L"LF_DIMVARU");
        StdOutPrintf(L"\tRank = %d\n", r->lf_dimvar.rank);
        StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimvar.typ));
        StdOutPuts(L"\tBounds = (");
        for (int i = 0; i < r->lf_dimvar.rank;) {
            StdOutPrintf(L"@%s",SzNameC7Type(r->lf_dimvar.dim[i]));

            i++;
            if (i < r->lf_dimvar.rank) {
                break;
            }
        }
        StdOutPuts(L")\n");
        break;
    case LF_DIMVARLU:
        StdOutPrintf(L"%s\n", L"LF_DIMVARLU");
        StdOutPrintf(L"\tRank = %d\n", r->lf_dimvar.rank);
        StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimvar.typ));
        StdOutPuts(L"\tBounds = (");
        for (int i = 0; i < r->lf_dimvar.rank;) {
            StdOutPrintf(L"@%s", SzNameC7Type(r->lf_dimvar.dim[2 * i + 0]));
            StdOutPuts(L":");
            StdOutPrintf(L"@%s", SzNameC7Type(r->lf_dimvar.dim[2 * i + 1]));

            i++;
            if (i < r->lf_dimvar.rank) {
                break;
            }
        }
        StdOutPuts(L")\n");
        break;
    case LF_SKIP:
        StdOutPrintf(L"%s\n", L"LF_SKIP");
        StdOutPrintf(L"\tNext effective type index : 0x%04x.\n", r->lf_skip.type);
        StdOutPuts(L"\tBytes Skipped :\n");
        StdOutHexBlockT7(r->lf_skip.data, length - offsetof(lfSkip, data));
        StdOutPuts(L"LF_LIST ignored\n");
        break;
    case LF_POINTER:
        StdOutPrintf(L"%s\n", L"LF_POINTER");
        StdOutPuts(L"\t");
        if (r->lf_pointer.attr.isvolatile) {
            StdOutPuts(L"volatile ");
        }
        if (r->lf_pointer.attr.isconst) {
            StdOutPuts(L"const ");
        }
        if (r->lf_pointer.attr.isunaligned) {
            StdOutPuts(L"__unaligned ");
        }
        if (r->lf_pointer.attr.isrestrict) {
            StdOutPuts(L"__restrict ");
        }
        StdOutPrintf(L"%s (%s)", rgszPointerMode[r->lf_pointer.attr.ptrmode], rgszPointerType[r->lf_pointer.attr.ptrtype]);
        StdOutPrintf(L", Size: %u", r->lf_pointer.attr.size);
        if (r->lf_pointer.attr.isflat32) {
            StdOutPuts(L" 16:32");
        }
        if (r->lf_pointer.attr.ismocom) {
            StdOutPuts(L" MoCOM");
        }
        StdOutPrintf(L"\n\tElement type : %s", SzNameC7Type(r->lf_pointer.utype));
        if (r->lf_pointer.attr.ptrmode != 0) {
            switch (r->lf_pointer.attr.ptrmode) {
            case CV_PTR_MODE_PMEM:
            case CV_PTR_MODE_PMFUNC:
                StdOutPrintf(L", Containing class = %s,\n", SzNameC7Type(r->lf_pointer.pbase.pm.pmclass));
                StdOutPrintf(L"\tType of pointer to member = %s", rgszPointerToMemberType[r->lf_pointer.pbase.pm.pmenum]);
            }
            StdOutPutc(L'\n');
            break;
        }
        switch (r->lf_pointer.attr.ptrtype) {
        case CV_PTR_BASE_SEG:
            StdOutPrintf(L", Segment#: 0x%04x", r->lf_pointer.pbase.bseg);
            StdOutPutc(L'\n');
            break;
        case CV_PTR_BASE_VAL:
            StdOutPuts(L", Based on value in symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer.pbase.pm, 0);
            break;
        case CV_PTR_BASE_SEGVAL:
            StdOutPuts(L", Based on segment in symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer.pbase.pm, 0);
            break;
        case CV_PTR_BASE_ADDR:
            StdOutPuts(L", Based on address of symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer.pbase.pm, 0);
            break;
        case CV_PTR_BASE_SEGADDR:
            StdOutPuts(L", Based on segment of symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer.pbase.pm, 0);
            break;
        case CV_PTR_BASE_TYPE:
            StdOutPrintf(L", base symbol type = %s", SzNameC7Type(r->lf_pointer.pbase.btype.index));
            StdOutPrintf(L"%s",L", name = '");
            PrintSt(false, r->lf_pointer.pbase.btype.name, false);
            StdOutPuts(L"'");
            break;
        case CV_PTR_BASE_SELF:
            StdOutPuts(L", Based on self");
            StdOutPutc(L'\n');
            break;
        }
        break;
    case LF_ARRAY_ST:
    case LF_ARRAY:
        {
            StdOutPrintf(L"%s\n", L"LF_ARRAY");
            StdOutPrintf(L"\tElement type = %s\n", SzNameC7Type(r->lf_array.elemtype));
            StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_array.idxtype));
            StdOutPuts(L"\tlength = ");
            int o = PrintNumeric(r->lf_array.data);
            StdOutPrintf(L"%s", L"\n\tName = ");
            PrintSt(r->leaf == LF_ARRAY, r->lf_array.data + o);
            break;
        }
    case LF_CLASS_ST:
    case LF_STRUCTURE_ST:
    case LF_CLASS:
    case LF_STRUCTURE:
    case LF_INTERFACE:
        {
            switch (r->leaf) {
            case LF_CLASS:
                StdOutPrintf(L"%s\n", L"LF_CLASS");
                break;
            case LF_STRUCTURE:
                StdOutPrintf(L"%s\n", L"LF_STRUCTURE");
                break;
            default:
                StdOutPrintf(L"%s\n", L"LF_INTERFACE");
                break;
            }
            int o = 0;
            StdOutPrintf(L"\t# members = %d, ", r->lf_class.count);
            StdOutPrintf(L" field list type 0x%04x, ", r->lf_class.field);
            StdOutClassProperties(r->lf_class.property);
            StdOutPutc(L'\n');
            StdOutPrintf(L"\tDerivation list type 0x%04x, ", r->lf_class.derived);
            StdOutPrintf(L"VT shape type 0x%04x\n", r->lf_class.vshape);
            StdOutPuts(L"\tSize = ");
            o += PrintNumeric(r->lf_class.data + o);
            StdOutPrintf(L"%s", L" class name = ");
            const unsigned char *e = PrintSt(r->leaf == LF_CLASS || r->leaf == LF_STRUCTURE, r->lf_class.data + o, false);
            if (r->lf_class.property.hasuniquename) {
                StdOutPrintf(L"%s", L", unique name = ");
                PrintSt(r->leaf == LF_CLASS || r->leaf == LF_STRUCTURE, e, false);
            }
            if (fSupportsQueryUDT && ptpi != NULL) {
                ShowUdtTypeId(ptpi, (const char *)r->lf_class.data + o);
            }
            StdOutPutc(L'\n');
        }
        break;
    case LF_UNION_ST:
    case LF_UNION:
        {
            StdOutPrintf(L"%s\n", L"LF_UNION");
            StdOutPrintf(L"\t# members = %d, ", r->lf_union.count);
            StdOutPrintf(L" field list type 0x%04x, ", r->lf_union.field);
            StdOutClassProperties(r->lf_union.property);
            StdOutPuts(L"Size = ");
            int o = 0;
            o += PrintNumeric(r->lf_union.data + o);
            const unsigned char *s = r->lf_union.data + o;
            StdOutPrintf(L"%s",L"\t,class name = ");
            s = PrintSt(r->leaf == LF_UNION, s, false);
            if (r->lf_union.property.hasuniquename) {
                StdOutPrintf(L"%s", L", unique name = ");
                PrintSt(r->leaf == LF_UNION, s, false);
            }
            if (fSupportsQueryUDT && ptpi != NULL) {
                ShowUdtTypeId(ptpi, (const char *) s);
            }
            StdOutPutc(L'\n');
        }
        break;
    case LF_ENUM_ST:
    case LF_ENUM:
        StdOutPrintf(L"%s\n", L"LF_ENUM");
        StdOutPrintf(L"\t# members = %d, ", r->lf_enum.count);
        StdOutPrintf(L" type = %s", SzNameC7Type(r->lf_enum.utype));
        StdOutPrintf(L" field list type 0x%04x\n", r->lf_enum.field);
        StdOutClassProperties(r->lf_enum.property);
        StdOutPrintf(L"%s", L"\tenum name = ");
        PrintSt(r->leaf == LF_ENUM, r->lf_enum.Name, false);
        if (fSupportsQueryUDT && ptpi != NULL) {
            ShowUdtTypeId(ptpi, (const char *) r->lf_enum.Name);
        }
        StdOutPutc(L'\n');
        break;
    case LF_PROCEDURE:
        StdOutPrintf(L"%s\n", L"LF_PROCEDURE");
        StdOutPrintf(L"\tReturn type = %s, ", SzNameC7Type(r->lf_proc.rvtype));
        StdOutPrintf(L"Call type = %s\n", SzCallConvention(r->lf_proc.calltype));
        StdOutPrintf(L"\tFunc attr = %s\n", SzFunctionAttr(r->lf_proc.funcattr));
        StdOutPrintf(L"\t# Parms = %d, ", r->lf_proc.parmcount);
        StdOutPrintf(L"Arg list type = 0x%04x\n", r->lf_proc.arglist);
        break;
    case LF_MFUNCTION:
        StdOutPrintf(L"%s\n", L"LF_MFUNCTION");
        StdOutPrintf(L"\tReturn type = %s, ", SzNameC7Type(r->lf_mfunc.rvtype));
        StdOutPrintf(L"Class type = %s, ", SzNameC7Type(r->lf_mfunc.classtype));
        StdOutPrintf(L"This type = %s, \n", SzNameC7Type(r->lf_mfunc.thistype));
        StdOutPrintf(L"\tCall type = %s, ", SzCallConvention(r->lf_mfunc.calltype));
        StdOutPrintf(L"Func attr = %s\n", SzFunctionAttr(r->lf_mfunc.funcattr));
        StdOutPrintf(L"\tParms = %d, ", r->lf_mfunc.parmcount);
        StdOutPrintf(L"Arg list type = 0x%04x, ", r->lf_mfunc.arglist);
        StdOutPrintf(L"This adjust = %lx\n", r->lf_mfunc.thisadjust);
        break;
    case LF_COBOL0:
        {
            StdOutPrintf(L"%s\n", L"LF_COBOL0");
            StdOutPrintf(L"\tParent index : %d", r->lf_cobol0.type);
            DWORD l = length - offsetof(lfCobol0, data);
            if (r->lf_cobol0.data[0]) {
                DumpCobolT7(&l, r->lf_cobol0.data);
            } else {
                DumpCobolCompileFlagsT7(l, r->lf_cobol0.data);
            }
        }
        break;
    case LF_BARRAY:
        StdOutPrintf(L"%s\n", L"LF_BARRAY");
        StdOutPrintf(L"    Element type %s\n", SzNameC7Type(r->lf_barray.utype));
        break;
    case LF_DIMARRAY_ST:
    case LF_DIMARRAY:
        StdOutPrintf(L"%s\n", L"LF_DIMARRAY");
        StdOutPrintf(L"\tElement type = %s\n", SzNameC7Type(r->lf_dimarray.utype));
        StdOutPrintf(L"\tDimension info = %s\n", SzNameC7Type(r->lf_dimarray.diminfo));
        StdOutPrintf(L"%s",L"\tName = ");
        PrintSt(r->lf_dimarray.leaf == LF_DIMARRAY, r->lf_dimarray.name);
        break;
    case LF_PRECOMP_ST:
    case LF_PRECOMP:
        StdOutPrintf(L"%s\n", L"LF_PRECOMP");
        StdOutPrintf(L"\t\tstart = 0x%04x, count = 0x%04x, signature = 0x%08lx\n ", r->lf_precomp.start, r->lf_precomp.count, r->lf_precomp.signature);
        StdOutPuts(L"\t\tIncluded file = ");
        PrintSt(r->lf_precomp.leaf == LF_PRECOMP, r->lf_precomp.name);
        break;
    case LF_OEM:
        {
            StdOutPrintf(L"%s\n", L"LF_OEM");
            StdOutPrintf(L"\tOEM = 0x%04X,\trecOEM = 0x%04X", r->lf_oem.cvOEM, r->lf_oem.recOEM);
            int count = r->lf_oem.count;
            if (count != 0) {
                StdOutPrintf(L"\n\tTypes (count = 0x%04X):\n", count);
                int count_lines = count / 4;
                int line = 0;
                int i = 0;
                for (int line = 0; line < count_lines - 1; line++) {
                    StdOutPrintf(L"\t\t%s", SzNameC7Type(r->lf_oem.index[i])); i++;
                    StdOutPrintf(L"\t\t%s", SzNameC7Type(r->lf_oem.index[i])); i++;
                    StdOutPrintf(L"\t\t%s", SzNameC7Type(r->lf_oem.index[i])); i++;
                    StdOutPrintf(L"\t\t%s\n", SzNameC7Type(r->lf_oem.index[i])); i++;
                }
                for (; i < count; i++) {
                    StdOutPrintf(L"%s\t",SzNameC7Type(r->lf_oem.index[i]));
                }
            }
            int count_bytes = length - 4 * r->lf_oem.count - offsetof(lfOEM, index);
            if (count_bytes != 0) {
                StdOutPrintf(L"\n\tData : (byte count = 0x%04x):\n", count_bytes);
                unsigned char *data = (unsigned char *)(r->lf_oem.index + r->lf_oem.count);
                int index = 0;
                while (count_bytes != 0) {
                    count_bytes -= 8;
                    StdOutPrintf(L"\t\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\n",
                        data[index + 0], data[index + 1],
                        data[index + 2], data[index + 3],
                        data[index + 4], data[index + 5],
                        data[index + 6], data[index + 7]);
                    count_bytes -= 8;
                    index += 8;
                }
                if (count_bytes != 0) {
                    while (count_bytes != 0) {
                        StdOutPrintf(L"0x%02X\t", data[index]);
                        count_bytes -= 1;
                        index += 1;
                    }
                    StdOutPutc(L'\n');
                }
            } else {
                StdOutPutc(L'\n');
            }        }
        break;
    case LF_ALIAS_ST:
    case LF_ALIAS:
        StdOutPrintf(L"%s\n", L"LF_ALIAS");
        StdOutPrintf(L"\ttype = %s,", SzNameC7Type(r->lf_alias.utype));
        StdOutPrintf(L"%s",L" Name = ");
        PrintSt(r->lf_alias.leaf == LF_ALIAS, r->lf_alias.Name);
        break;
    case LF_MODIFIER:
        StdOutPrintf(L"%s\n", L"LF_MODIFIER");
        StdOutPrintf(L"\t");
        if (r->lf_modifier.attr.MOD_const) {
            StdOutPrintf(L"const, ");
        }
        if (r->lf_modifier.attr.MOD_volatile) {
            StdOutPrintf(L"volatile, ");
        }
        if (r->lf_modifier.attr.MOD_unaligned) {
            StdOutPrintf(L"__unaligned, ");
        }
        StdOutPrintf(L"modifies type %s\n", SzNameC7Type(r->lf_modifier.type));
        break;
    case LF_ARGLIST_16t:
        StdOutPrintf(L"LF_ARGLIST_16t argument count = %d\n", r->lf_arglist_16t.count);
        for (int i = 0; i < r->lf_arglist_16t.count; i++) {
            StdOutPrintf(L"\tlist[%d] = %s\n", i, SzNameC7Type(r->lf_arglist_16t.arg[i]));
        }
        break;
    case LF_DEFARG_16t:
        StdOutPrintf(L"%s\n",L"LF_DEFARG_16t");
        StdOutPrintf(L"type = %s, ", SzNameC7Type(r->lf_defarg_16t.type));
        PrintSt(false, r->lf_defarg_16t.expr);
        break;
    case LF_LIST:
        StdOutPuts(L"LF_LIST ignored\n");
        break;
    case LF_FIELDLIST_16t:
        StdOutPrintf(L"%s\n", L"LF_FIELDLIST_16t");
        DumpFieldListT7(length - offsetof(lfFieldList_16t, data), r->lf_fieldlist_16t.data);
        break;
    case LF_DERIVED_16t:
        StdOutPrintf(L"%s\n", L"LF_DERIVED_16t");
        for (int i = 0; i < r->lf_derived_16t.count; i++) {
            StdOutPrintf(L"\tderived[%d] = %s\n", i, SzNameC7Type(r->lf_derived_16t.drvdcls[i]));
        }
        break;
    case LF_BITFIELD_16t:
        StdOutPrintf(L"%s\n", L"LF_BITFIELD_16t");
        StdOutPrintf(L"\tbits = %d, ", r->lf_bitfield_16t.length);
        StdOutPrintf(L"starting position = %d", r->lf_bitfield_16t.position);
        StdOutPrintf(L", Type = %s\n", SzNameC7Type(r->lf_bitfield_16t.type));
        break;
    case LF_METHODLIST_16t:
        {
            StdOutPrintf(L"%s\n", L"LF_METHODLIST_16t");
            int o = offsetof(lfMethodList_16t, mList);
            for (int i = 0; o < length; i++) {
                StdOutPrintf(L"\tlist[%u] = ", i);
                CV_fldattr_t attr = *(CV_fldattr_t*)(r->lf_methodlist_16t.mList + o);
                o += 2;
                StdOutClassFieldAttributesT7(true, attr);
                StdOutPrintf(L"%s, ", SzNameC7Type(*(unsigned short*)(r->lf_methodlist_16t.mList + o)));
                o += 2;
                if (attr.mprop == CV_MTintro || attr.mprop == CV_MTpureintro) {
                    StdOutPrintf(L" vfptr offset = %ld", *(unsigned int *)(r->lf_methodlist_16t.mList + o));
                    o += 4;
                }
                StdOutPutc(L'\n');
            }
        }
        break;
    case LF_DIMCONU_16t:
        {
            StdOutPrintf(L"%s\n", L"LF_DIMCONU");
            StdOutPrintf(L"\tRank = %d\n", r->lf_dimcon_16t.rank);
            StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimcon.typ));
            StdOutPuts(L"\tBounds = (");
            int s = 1 << (r->lf_dimcon.typ % 16);
            for (int i = 0, o = 0; i < r->lf_dimcon.rank;) {
                unsigned char buf[512];
                memcpy(buf, r->lf_dimcon.dim + o, s);
                o += s;
                StdOutPrintf(L"%d", buf);

                i += 1;
                if (i < r->lf_dimcon.rank) {
                    StdOutPuts(L", ");
                }
            }
            StdOutPuts(L")\n");
        }
    break;
    case LF_DIMCONLU_16t:
        {
            StdOutPrintf(L"%s\n", L"LF_DIMCONLU_16t");
            StdOutPrintf(L"\tRank = %d\n", r->lf_dimcon_16t.rank);
            StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimcon_16t.typ));
            StdOutPuts(L"\tBounds = (");
            int s = 1 << (r->lf_dimcon.typ % 16);
            for (int i = 0, o = 0; i < r->lf_dimcon.rank;) {
                unsigned char buf[512];
                memcpy(buf, r->lf_dimcon.dim + o, s);
                o += s;
                StdOutPrintf(L"%d", buf);
                StdOutPuts(L":");
                memcpy(buf, r->lf_dimcon.dim + o, s);
                o += s;
                StdOutPrintf(L"%d", buf);

                i += 1;
                if (i < r->lf_dimcon.rank) {
                    StdOutPuts(L", ");
                }
            }
            StdOutPuts(L")\n");
        }
        break;
    case LF_DIMVARU_16t:
        StdOutPrintf(L"%s\n", L"LF_DIMVARU_16t");
        StdOutPrintf(L"\tRank = %d\n", r->lf_dimvar_16t.rank);
        StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimvar_16t.typ));
        StdOutPuts(L"\tBounds = (");
        for (int i = 0; i < r->lf_dimvar_16t.rank;) {
            StdOutPrintf(L"@%s", SzNameC7Type(r->lf_dimvar_16t.dim[i]));
            i++;
            if (i < r->lf_dimvar_16t.rank) {
                StdOutPuts(L", ");
            }
        }
        StdOutPuts(L")\n");
        break;
    case LF_DIMVARLU_16t:
        // FIXME: cvdump.exe switched lfDimVar_16t.rank and lfDImVar_16t.type here, and used 32-bit int
        StdOutPrintf(L"%s\n", L"LF_DIMVARLU_16t");
        StdOutPrintf(L"\tRank = %d\n", r->lf_dimvar_16t.rank);
        StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_dimvar_16t.typ));
        StdOutPuts(L"\tBounds = (");
        for (int i = 0; i < r->lf_dimvar_16t.rank;) {
            StdOutPrintf(L"@%s", SzNameC7Type(r->lf_dimvar_16t.dim[2 * i + 0]));
            StdOutPuts(L":");
            StdOutPrintf(L"@%s",SzNameC7Type(r->lf_dimvar_16t.dim[2 * i + 1]));
            i++;
            if (i < r->lf_dimvar_16t.rank) {
                StdOutPuts(L", ");
            }
        }
        StdOutPuts(L")\n");
        break;
    case LF_REFSYM:
        StdOutPrintf(L"%s\n", L"LF_REFSYM");
        DumpOneSymC7(NULL, r->lf_refsym.Sym, 0);
        break;
    case LF_SKIP_16t:
        StdOutPrintf(L"%s\n", L"LF_SKIP_16t");
        StdOutPrintf(L"\tNext effective type index : 0x%04x.\n", r->lf_skip_16t.type);
        StdOutPuts(L"\tBytes Skipped :\n");
        StdOutHexBlockT7(r->lf_skip_16t.data, length - offsetof(lfSkip_16t, data));
        // FIXME: cvdump.exe jumps to LF_DERIVED_16T here
        break;
    case LF_MODIFIER_16t:
        StdOutPrintf(L"%s\n", L"LF_MODIFIER_16t");
        StdOutPuts(L"\t");
        if (r->lf_modifier_16t.attr.MOD_const) {
            StdOutPuts(L"const, ");
        }
        if (r->lf_modifier_16t.attr.MOD_volatile) {
            StdOutPuts(L"volatile, ");
        }
        if (r->lf_modifier_16t.attr.MOD_unaligned) {
            StdOutPuts(L"__unaligned, ");
        }
        StdOutPrintf(L"modifies type %s\n", SzNameC7Type(r->lf_modifier_16t.type));
        break;
    case LF_POINTER_16t:
        StdOutPrintf(L"%s\n",L"LF_POINTER_16t");
        StdOutPuts(L"\t");
        if (r->lf_pointer_16t.attr.isvolatile) {
            StdOutPuts(L"volatile ");
        }
        if (r->lf_pointer_16t.attr.isconst) {
            StdOutPuts(L"const ");
        }
        if (r->lf_pointer_16t.attr.isunaligned) {
            StdOutPuts(L"__unaligned ");
        }
        StdOutPrintf(L"%s (%s)",
            rgszPointerMode[r->lf_pointer_16t.attr.ptrmode],
            rgszPointerType[r->lf_pointer_16t.attr.ptrtype]);
        if (r->lf_pointer_16t.attr.isflat32) {
            StdOutPuts(L" 16:32");
        }
        StdOutPrintf(L"\n\tElement type : %s", SzNameC7Type(r->lf_pointer_16t.utype));
        if (r->lf_pointer_16t.attr.ptrmode != 0) {
            switch (r->lf_pointer_16t.attr.ptrmode) {
            case CV_PTR_MODE_PMEM:
            case CV_PTR_MODE_PMFUNC:
                StdOutPrintf(L", Containing class = %s,\n", SzNameC7Type(r->lf_pointer_16t.pbase.pm.pmclass));
                StdOutPrintf(L"\tType of pointer to member = %s", rgszPointerToMemberType[r->lf_pointer_16t.pbase.pm.pmenum]);
                break;
            }
            StdOutPutc(L'\n');
            break;
        }
        switch (r->lf_pointer_16t.attr.ptrtype) {
        case CV_PTR_BASE_SEG:
            StdOutPrintf(L", Segment#: 0x%04x", r->lf_pointer_16t.pbase.bseg);
            StdOutPutc(L'\n');
            break;
        case CV_PTR_BASE_VAL:
            StdOutPuts(L", Based on value in symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer_16t.pbase.pm, 0);
            break;
        case CV_PTR_BASE_SEGVAL:
            StdOutPuts(L", Based on segment in symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer_16t.pbase.pm, 0);
            break;
        case CV_PTR_BASE_ADDR:
            StdOutPuts(L", Based on address of symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer_16t.pbase.pm, 0);
            break;
        case CV_PTR_BASE_SEGADDR:
            StdOutPuts(L", Based on segment of symbol :\n\t");
            DumpOneSymC7(NULL, &r->lf_pointer_16t.pbase.pm, 0);
            break;
        case CV_PTR_BASE_TYPE:
            StdOutPrintf(L", base symbol type = %s", SzNameC7Type(r->lf_pointer_16t.pbase.btype.index));
            StdOutPrintf(L"%s",L", name = '");
            PrintSt(false, r->lf_pointer_16t.pbase.btype.name, false);
            StdOutPuts(L"'");
            break;
        case CV_PTR_BASE_SELF:
            StdOutPuts(L", Based on self");
            StdOutPutc(L'\n');
            break;
        }
        break;
    case LF_ARRAY_16t:
        {
            StdOutPrintf(L"%s\n",L"LF_ARRAY_16t");
            StdOutPrintf(L"\tElement type = %s\n", SzNameC7Type(r->lf_array_16t.elemtype));
            StdOutPrintf(L"\tIndex type = %s\n", SzNameC7Type(r->lf_array_16t.idxtype));
            StdOutPuts(L"\tlength = ");
            int o = 0;
            o += PrintNumeric(r->lf_array_16t.data + o);
            StdOutPrintf(L"%s",L"\n\tName = ");
            PrintSt(false, r->lf_array_16t.data + o);
        }
        break;
    case LF_CLASS_16t:
    case LF_STRUCTURE_16t:
        {
            if (r->lf_class_16t.leaf == LF_CLASS) {
                StdOutPrintf(L"%s\n", L"LF_CLASS_16t");
            } else {
                StdOutPrintf(L"%s\n",  L"LF_STRUCTURE_16t");
            }
            StdOutPrintf(L"\t# members = %d, ", r->lf_class_16t.count);
            StdOutPrintf(L" field list type 0x%04x, ", r->lf_class_16t.field);
            StdOutClassProperties(r->lf_class_16t.property);
            StdOutPutc(L'\n');
            StdOutPrintf(L"\tDerivation list type 0x%04x, ", r->lf_class_16t.derived);
            StdOutPrintf(L"VT shape type 0x%04x\n", r->lf_class_16t.vshape);
            StdOutPuts(L"\tSize = ");
            int o = PrintNumeric(r->lf_class_16t.data);
            StdOutPrintf(L"%s", L", class name = ");
            PrintSt(false, r->lf_class_16t.data + o, false);
            if (fSupportsQueryUDT && ptpi != NULL) {
                ShowUdtTypeId(ptpi, (const char *) (r->lf_class_16t.data + o));
            }
            StdOutPutc(L'\n');
        }
        break;
    case LF_UNION_16t:
        {
            StdOutPrintf(L"%s\n",L"LF_UNION_16t");
            StdOutPrintf(L"\t# members = %d, ", r->lf_union_16t.count);
            StdOutPrintf(L" field list type 0x%04x, ", r->lf_union_16t.field);
            StdOutClassProperties(r->lf_union_16t.property);
            StdOutPuts(L"Size = ");
            int o = PrintNumeric(r->lf_union_16t.data);
            StdOutPrintf(L"%s",L"\t,class name = ");
            PrintSt(false, r->lf_union_16t.data + o, false);
            if (fSupportsQueryUDT && ptpi != NULL) {
                ShowUdtTypeId(ptpi, (const char *) (r->lf_union_16t.data + o));
            }
            StdOutPutc(L'\n');
        }
        break;
    case LF_ENUM_16t:
        StdOutPrintf(L"%s\n", L"LF_ENUM_16t");
        StdOutPrintf(L"\t# members = %d, ", r->lf_enum_16t.count);
        StdOutPrintf(L" type = %s", SzNameC7Type(r->lf_enum_16t.utype));
        StdOutPrintf(L" field list type 0x%04x\n", r->lf_enum_16t.field);
        StdOutClassProperties(r->lf_enum_16t.property);
        StdOutPrintf(L"%s", L"\tenum name = ");
        PrintSt(false, r->lf_enum_16t.Name, false);
        if (fSupportsQueryUDT && ptpi != NULL) {
            ShowUdtTypeId(ptpi, (const char *) r->lf_enum_16t.Name);
        }
        StdOutPutc(L'\n');
        break;
    case LF_PROCEDURE_16t:
        StdOutPrintf(L"%s\n", L"LF_PROCEDURE_16t");
        StdOutPrintf(L"\tReturn type = %s, ", SzNameC7Type(r->lf_proc_16t.rvtype));
        StdOutPrintf(L"Call type = %s\n", SzCallConvention(r->lf_proc_16t.calltype));
        StdOutPrintf(L"\tFunc attr = %s\n", SzFunctionAttr(r->lf_proc_16t.funcattr));
        StdOutPrintf(L"\t# Parms = %d, ", r->lf_proc_16t.parmcount);
        StdOutPrintf(L"Arg list type = 0x%04x\n", r->lf_proc_16t.arglist);
        break;
    case LF_MFUNCTION_16t:
        StdOutPrintf(L"%s\n",L"LF_MFUNCTION_16t");
        StdOutPrintf(L"\tReturn type = %s, ", SzNameC7Type(r->lf_mfunc_16t.rvtype));
        StdOutPrintf(L"Class type = %s, ", SzNameC7Type(r->lf_mfunc_16t.classtype));
        StdOutPrintf(L"This type = %s, \n", SzNameC7Type(r->lf_mfunc_16t.thistype));
        StdOutPrintf(L"\tCall type = %s, ", SzCallConvention(r->lf_mfunc_16t.calltype));
        StdOutPrintf(L"Func attr = %s\n", SzFunctionAttr(r->lf_mfunc_16t.funcattr));
        StdOutPrintf(L"\tParms = %d, ", r->lf_mfunc_16t.parmcount);
        StdOutPrintf(L"Arg list type = 0x%04x, ", r->lf_mfunc_16t.arglist);
        StdOutPrintf(L"This adjust = %lx\n", r->lf_mfunc_16t.thisadjust);
        break;
    case LF_VTSHAPE:
        StdOutPrintf(L"%s\n",L"LF_VTSHAPE");
        StdOutPrintf(L"\tNumber of entries : %u\n", r->lf_vtshape.count);
        for (int i = 0, idx = 0; i < r->lf_vtshape.count; i++) {
            unsigned char t = (r->lf_vtshape.desc[idx] >> (4 * (i % 2))) % 16;
            StdOutPrintf(L"\t\t[%u]: %s\n", i, rgszVtFuncType[t]);
            if (i % 2) {
                idx++;
            }
        }
        break;
    case LF_COBOL0_16t:
        {
            StdOutPrintf(L"%s\n", L"LF_COBOL0_16t");
            StdOutPrintf(L"\tParent index : %d", r->lf_cobol0_16t.type);
            DWORD l = length - 6;
            if (r->lf_cobol0_16t.data[0]) {
                DumpCobolT7(&l, r->lf_cobol0_16t.data);
            } else {
                DumpCobolCompileFlagsT7(l, r->lf_cobol0_16t.data);
            }
        }
        break;
    case LF_COBOL1:
        {
            DWORD l = length - 2;
            if (r->lf_cobol1.data[0]) {
                DumpCobolT7(&l, r->lf_cobol1.data);
            } else {
                DumpCobolCompileFlagsT7(l, r->lf_cobol1.data);
            }
        }
        break;
    case LF_BARRAY_16t:
        StdOutPrintf(L"%s\n", L"LF_BARRAY_16t");
        StdOutPrintf(L"    Element type %s\n", SzNameC7Type(r->lf_barray_16t.utype));
        break;
    case LF_LABEL:
        StdOutPrintf(L"%s\n",L"LF_LABEL");
        switch (r->lf_label.mode) {
        case CV_LABEL_NEAR:
            StdOutPrintf(L"\tmode = NEAR(0x%04x)\n", r->lf_label.mode);
            break;
        case CV_LABEL_FAR:
            StdOutPrintf(L"\tmode = FAR(0x%04x)\n", r->lf_label.mode);
            break;
        default:
            StdOutPrintf(L"\tmode = ???" L"(0x%04x)\n", r->lf_label.mode);
            break;
        }
        break;
    case LF_NULL:
        StdOutPrintf(L"%s\n", L"LF_NULL");
        break;
    case LF_NOTTRAN:
        StdOutPrintf(L"%s\n", L"LF_NOTTRANS");
        break;
    case LF_DIMARRAY_16t:
        StdOutPrintf(L"%s\n",L"LF_DIMARRAY_16t");
        StdOutPrintf(L"\tElement type = %s\n", SzNameC7Type(r->lf_dimarray_16t.utype));
        StdOutPrintf(L"\tDimension info = %s\n", SzNameC7Type(r->lf_dimarray_16t.diminfo));
        StdOutPrintf(L"%s",L"\tName = ");
        PrintSt(false, r->lf_dimarray_16t.name);
        break;
    case LF_PRECOMP_16t:
        StdOutPrintf(L"%s\n", L"LF_PRECOMP_16t");
        StdOutPrintf(L"\t\tstart = 0x%04x, count = 0x%04x, signature = 0x%08lx\n ",
            r->lf_precomp_16t.start, r->lf_precomp_16t.count,r->lf_precomp_16t.signature);
        StdOutPuts(L"\t\tIncluded file = ");
        PrintSt(false,r->lf_precomp_16t.name);
        break;
    case LF_ENDPRECOMP:
        StdOutPrintf(L"%s\n",L"LF_ENDPRECOMP");
        StdOutPrintf(L"\t\tsignature = 0x%08x\n ", r->lf_endprecomp.signature);
        break;
    case LF_OEM_16t:
        {
            StdOutPrintf(L"%s\n", L"LF_OEM_16t");
            StdOutPrintf(L"\tOEM = 0x%04X,\trecOEM = 0x%04X", r->lf_oem_16t.cvOEM, r->lf_oem_16t.recOEM);
            int count = r->lf_oem_16t.count;
            if (count != 0) {
                StdOutPrintf(L"\n\tTypes (count = 0x%04X):\n", count);
                int count_lines = count / 4;
                int line = 0;
                int i = 0;
                for (int line = 0; line < count_lines - 1; line++) {
                    StdOutPrintf(L"\t\t%s", SzNameC7Type(r->lf_oem_16t.index[i])); i++;
                    StdOutPrintf(L"\t\t%s", SzNameC7Type(r->lf_oem_16t.index[i])); i++;
                    StdOutPrintf(L"\t\t%s", SzNameC7Type(r->lf_oem_16t.index[i])); i++;
                    StdOutPrintf(L"\t\t%s\n", SzNameC7Type(r->lf_oem_16t.index[i])); i++;
                }
                for (; i < count; i++) {
                    StdOutPrintf(L"%s\t", SzNameC7Type(r->lf_oem_16t.index[i]));
                }
            }
            int count_bytes = length - 2 * r->lf_oem_16t.count - offsetof(lfOEM, index);
            if (count_bytes != 0) {
                StdOutPrintf(L"\n\tData : (byte count = 0x%04x):\n", count_bytes);
                int index = 0;
                unsigned char *data = (unsigned char *)(r->lf_oem_16t.index + r->lf_oem_16t.count);
                while (count_bytes != 0) {
                    count_bytes -= 8;
                    StdOutPrintf(L"\t\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\t0x%02X\n",
                        data[index + 0], data[index + 1],
                        data[index + 2], data[index + 3],
                        data[index + 4], data[index + 5],
                        data[index + 6], data[index + 7]);
                    count_bytes -= 8;
                    index += 8;
                }
                if (count_bytes != 0) {
                    while (count_bytes != 0) {
                        StdOutPrintf(L"0x%02X\t", data[index]);
                        count_bytes -= 1;
                        index += 1;
                    }
                    StdOutPutc(L'\n');
                }
            } else {
                StdOutPutc(L'\n');
            }
        }
        break;
    case LF_TYPESERVER:
    case LF_TYPESERVER_ST:
        StdOutPrintf(L"%s\n",L"LF_TYPESERVER");
        StdOutPrintf(L"\t\tsignature = 0x%08lx, age = 0x%08x", r->lf_typeserver.signature, r->lf_typeserver.age);
        StdOutPrintf(L"%s",L", PDB name = '");
        PrintSt(r->leaf == LF_TYPESERVER, r->lf_typeserver.name);
        break;
    }
    StdOutPutc(L'\n');
    return r->leaf;
}

void DumpFieldListT7(DWORD length, LPCVOID data)
{
    const BYTE *current_data = (const BYTE *)data;
    for (int i = 0, offset = 0; ; i++) {
        FieldListElement *r = (FieldListElement *) current_data;
        StdOutPrintf(L"\tlist[%d] = ", i);
        DWORD elem_length;
        switch (r->leaf) {
            default:
                StdOutPrintf(L"unknown leaf %x\n", r->leaf);
                elem_length = 4;
                break;
            case LF_VBCLASS:
                {
                    StdOutPuts(L"LF_VBCLASS, ");
                    StdOutClassFieldAttributesT7(false, r->lf_vbclass.attr);
                    StdOutPrintf(L"direct base type = %s\n", SzNameC7Type(r->lf_vbclass.index));
                    StdOutPrintf(L"\t\tvirtual base ptr = %s, vbpoff = ", SzNameC7Type(r->lf_vbclass.vbptr));
                    int o = PrintNumeric(r->lf_vbclass.vbpoff);
                    StdOutPuts(L", vbind = ");
                    o += PrintNumeric(r->lf_vbclass.vbpoff + o);
                    elem_length = offsetof(lfVBClass, vbpoff) + o;
                    StdOutPutc(L'\n');
                }
                break;
            case LF_IVBCLASS:
                {
                    StdOutPuts(L"LF_IVBCLASS, ");
                    StdOutClassFieldAttributesT7(false, r->lf_vbclass.attr);
                    StdOutPrintf(L"indirect base type = %s\n", SzNameC7Type(r->lf_vbclass.index));
                    StdOutPrintf(L"\t\tvirtual base ptr = %s, vbpoff = ", SzNameC7Type(r->lf_vbclass.vbptr));
                    int o = PrintNumeric(r->lf_vbclass.vbpoff);
                    StdOutPuts(L", vbind = ");
                    o += PrintNumeric(r->lf_vbclass.vbpoff + o);
                    elem_length = offsetof(lfVBClass, vbpoff) + o;
                    StdOutPutc(L'\n');
                }
                break;
            case LF_FRIENDFCN_ST:
            case LF_FRIENDFCN:
                StdOutPuts(L"LF_FRIENDFCN, ");
                StdOutPrintf(L"type = %s", SzNameC7Type(r->lf_friendfcn.index));
                StdOutPrintf(L"%s", L"\tfunction name = ");
                PrintSt(r->leaf == LF_FRIENDFCN, r->lf_friendfcn.Name);
                elem_length = offsetof(lfFriendFcn, Name);
                if (r->leaf == LF_FRIENDFCN) {
                    for (const unsigned char *n = r->lf_friendfcn.Name; *n != '\0'; n++) {
                        elem_length += 1;
                    }
                    elem_length += 1;
                } else {
                    elem_length += r->lf_friendfcn.Name[0] + 1;
                }
                if (r->lf_friendfcn.pad0 != 0) {
                    StdOutPuts(L"***Warning, pad bytes are non-zero!\n");
                }
                break;
            case LF_INDEX:
                StdOutPrintf(L"LF_INDEX, Type Index = %s\n", SzNameC7Type(r->lf_index.index));
                if (r->lf_index.pad0 != 0) {
                    StdOutPuts(L"***Warning, pad bytes are non-zero!\n");
                }
                elem_length = sizeof(lfIndex);
                break;
            case LF_MEMBER_ST:
            case LF_MEMBER:
                {
                    StdOutPuts(L"LF_MEMBER, ");
                    StdOutClassFieldAttributesT7(false, r->lf_member.attr);
                    StdOutPrintf(L"type = %s, offset = ", SzNameC7Type(r->lf_member.index));
                    int o = PrintNumeric(r->lf_member.offset);
                    StdOutPrintf(L"%s", L"\n\t\tmember name = '");
                    PrintSt(r->lf_member.leaf == LF_MEMBER, r->lf_member.offset + o, false);
                    elem_length = offsetof(lfMember, offset) + o;
                    if (r->lf_member.leaf == LF_MEMBER) {
                        for (const unsigned char *n = r->lf_member.offset + o; *n != '\0'; n++) {
                            elem_length += 1;
                        }
                        elem_length += 1;
                    } else {
                        elem_length += r->lf_member.offset[o] + 1;
                    }
                    StdOutPuts(L"'\n");
                }
                break;
            case LF_STMEMBER_ST:
            case LF_STMEMBER:
                StdOutPuts(L"LF_STATICMEMBER, ");
                StdOutClassFieldAttributesT7(false, r->lf_stmember.attr);
                StdOutPrintf(L"type = %s", SzNameC7Type(r->lf_stmember.index));
                StdOutPrintf(L"%s", L"\t\tmember name = '");
                PrintSt(r->lf_stmember.leaf == LF_STMEMBER, r->lf_stmember.Name, false);
                elem_length = offsetof(lfSTMember, Name);
                StdOutPuts(L";\n");
                if (r->lf_stmember.leaf == LF_STMEMBER) {
                    for (const unsigned char *n = r->lf_stmember.Name; *n != '\0'; n++) {
                        elem_length += 1;
                    }
                    elem_length += 1;
                } else {
                    elem_length += r->lf_stmember.Name[0] + 1;
                }
                break;
            case LF_METHOD_ST:
            case LF_METHOD:
                StdOutPuts(L"LF_METHOD, ");
                StdOutPrintf(L"count = %d, ", r->lf_method.count);
                StdOutPrintf(L"list = %s, ", SzNameC7Type(r->lf_method.mList));
                StdOutPrintf(L"%s", L"name = '");
                PrintSt(r->lf_method.leaf == LF_METHOD, r->lf_method.Name, false);
                StdOutPuts(L"'\n");
                elem_length = offsetof(lfMethod, Name);
                if (r->lf_method.leaf == LF_METHOD) {
                    for (const unsigned char *n = r->lf_method.Name; *n != '\0'; n++) {
                        elem_length += 1;
                    }
                    elem_length += 1;
                } else {
                    elem_length += r->lf_method.Name[0] + 1;
                }
                break;
            case LF_NESTTYPE_ST:
            case LF_NESTTYPE:
                StdOutPuts(L"LF_NESTTYPE, ");
                StdOutPrintf(L"type = %s, ", SzNameC7Type(r->lf_nesttype.index));
                PrintSt(r->lf_nesttype.leaf == LF_NESTTYPE, r->lf_nesttype.Name);
                elem_length = offsetof(lfNestType, Name);
                if (r->lf_nesttype.leaf == LF_NESTTYPE) {
                    for (const unsigned char *n = r->lf_nesttype.Name; *n != '\0'; n++) {
                        elem_length += 1;
                    }
                    elem_length += 1;
                } else {
                    elem_length += r->lf_nesttype.Name[0] + 1;
                }
                if (r->lf_nesttype.pad0 != 0) {
                    StdOutPuts(L"***Warning, pad bytes are non-zero!\n");
                }
                break;
            case LF_VFUNCTAB:
                StdOutPuts(L"LF_VFUNCTAB, ");
                StdOutPrintf(L"type = %s\n", SzNameC7Type(r->lf_vfunctab.type));
                elem_length = sizeof(lfVFuncTab);
                if (r->lf_vfunctab.pad0 != 0) {
                    StdOutPuts(L"***Warning, pad bytes are non-zero!\n");
                }
                break;
            case LF_FRIENDCLS:
                StdOutPuts(L"LF_FRIENDCLS, ");
                StdOutPrintf(L"type = %s\n", SzNameC7Type(r->lf_friendcls.index));
                elem_length = sizeof(lfFriendCls);
                if (r->lf_friendcls.pad0 != 0) {
                    StdOutPuts(L"***Warning, pad bytes are non-zero!\n");
                }
                break;
            case LF_ONEMETHOD_ST:
            case LF_ONEMETHOD:
                {
                    StdOutPuts(L"LF_ONEMETHOD, ");
                    StdOutClassFieldAttributesT7(true, r->lf_onemethod.attr);
                    StdOutPrintf(L"index = %s, ", SzNameC7Type(r->lf_onemethod.index));
                    int o = 0;
                    if (r->lf_onemethod.attr.mprop == CV_MTintro || r->lf_onemethod.attr.mprop == CV_MTpureintro) {
                        StdOutPrintf(L"\n\t\tvfptr offset = %ld, ", r->lf_onemethod.vbaseoff[0]);
                        o += sizeof(long);
                    }
                    StdOutPrintf(L"%s", L"name = '");
                    const unsigned char *name = ((const unsigned char *) r->lf_onemethod.vbaseoff) + o;
                    PrintSt(r->lf_onemethod.leaf == LF_ONEMETHOD, name, false);
                    StdOutPuts(L"'\n");
                    elem_length = offsetof(lfOneMethod, vbaseoff) + offset;
                    if (r->lf_onemethod.leaf == LF_ONEMETHOD) {
                        for (const unsigned char *n = name; *n != '\0'; n++) {
                            elem_length += 1;
                        }
                        elem_length += 1;
                    } else {
                        elem_length += name[0] + 1;
                    }
                }
                break;
            case LF_NESTTYPEEX_ST:
            case LF_NESTTYPEEX:
                StdOutPuts(L"LF_NESTTYPEEX, ");
                StdOutPrintf(L"type = %s, ", SzNameC7Type(r->lf_nesttypeex.index));
                StdOutClassFieldAttributesT7(false, r->lf_nesttypeex.attr);
                PrintSt(r->lf_nesttypeex.leaf == LF_NESTTYPEEX, r->lf_nesttypeex.Name);
                elem_length = offsetof(lfNestTypeEx, Name);
                if (r->lf_nesttypeex.leaf == LF_NESTTYPEEX) {
                    for (const unsigned char *n = r->lf_nesttypeex.Name; *n != '\0'; n++) {
                        elem_length += 1;
                    }
                    elem_length += 1;
                } else {
                    elem_length += r->lf_nesttypeex.Name[0] + 1;
                }
                break;
            case LF_BINTERFACE:
            case LF_BCLASS: {
                StdOutPuts(r->leaf == LF_BINTERFACE ? L"LF_BINTERFACE, " : L"LF_BCLASS, ");
                StdOutPrintf(L"%s, ", rgszAccess[r->lf_bclass.attr.access]);
                if (r->lf_bclass.attr.noinherit) {
                    StdOutPuts(L"(noinherit), ");
                }
                if (r->lf_bclass.attr.noconstruct) {
                    StdOutPuts(L"(noconstruct), ");
                }
                StdOutPrintf(L"type = %s", SzNameC7Type(r->lf_bclass.index));
                StdOutPuts(L", offset = ");
                int o = PrintNumeric(r->lf_bclass.offset);
                elem_length = offsetof(lfBClass, offset) + o;
                StdOutPutc(L'\n');
            }
                break;
            case LF_BCLASS_16t:
                {
                    StdOutPuts(L"LF_BCLASS_16t, ");
                    StdOutClassFieldAttributesT7(false, r->lf_bclass_16t.attr);
                    StdOutPrintf(L"type = %s", SzNameC7Type(r->lf_bclass_16t.index));
                    StdOutPuts(L", offset = ");
                    int o = PrintNumeric(r->lf_bclass_16t.offset);
                    elem_length = offsetof(lfBClass_16t, offset) + o;
                    StdOutPutc(L'\n');
                }
                break;
            case LF_VBCLASS_16t:
                {
                    StdOutPuts(L"LF_VBCLASS_16t, ");
                    StdOutClassFieldAttributesT7(false, r->lf_vbclass_16t.attr);
                    StdOutPrintf(L"direct base type = %s\n", SzNameC7Type(r->lf_vbclass_16t.index));
                    StdOutPrintf(L"\t\tvirtual base ptr = %s, vbpoff = ", SzNameC7Type(r->lf_vbclass_16t.vbptr));
                    int o = PrintNumeric(r->lf_vbclass_16t.vbpoff);
                    StdOutPuts(L", vbind = ");
                    o += PrintNumeric(r->lf_vbclass_16t.vbpoff + o);
                    elem_length = offsetof(lfVBClass_16t, vbpoff) + o;
                    StdOutPutc(L'\n');
                }
                break;
            case LF_IVBCLASS_16t:
                {
                    StdOutPuts(L"LF_IVBCLASS_16t, ");
                    StdOutClassFieldAttributesT7(false, r->lf_vbclass_16t.attr);
                    StdOutPrintf(L"indirect base type = %s\n", SzNameC7Type(r->lf_vbclass_16t.index));
                    StdOutPrintf(L"\t\tvirtual base ptr = %s, vbpoff = ", SzNameC7Type(r->lf_vbclass_16t.vbptr));
                    int o = PrintNumeric(r->lf_vbclass_16t.vbpoff);
                    StdOutPuts(L", vbind = ");
                    o += PrintNumeric(r->lf_vbclass_16t.vbpoff + o);
                    elem_length = offsetof(lfVBClass_16t, vbpoff) + o;
                    StdOutPutc(L'\n');
                }
                break;
            case LF_ENUMERATE_ST:
            case LF_ENUMERATE:
                {
                    StdOutPuts(L"LF_ENUMERATE, ");
                    StdOutPrintf(L"%s, ", rgszAccess[r->lf_enumerate.attr.access]);
                    if (r->lf_enumerate.attr.noinherit) {
                        StdOutPuts(L"(noinherit), ");
                    }
                    if (r->lf_enumerate.attr.noconstruct) {
                        StdOutPuts(L"(noconstruct), ");
                    }
                    StdOutPuts(L"value = ");
                    int o = PrintNumeric(r->lf_enumerate.value);
                    StdOutPrintf(L"%s", L", name = '");
                    const unsigned char *name = r->lf_enumerate.value + o;
                    PrintSt(r->lf_enumerate.leaf == LF_ENUMERATE, name, false);
                    StdOutPuts(L"'\n");
                    elem_length = offsetof(lfEnumerate, value) + o;
                    if (r->lf_enumerate.leaf == LF_ENUMERATE) {
                        for (const unsigned char *n = name; *n != '\0'; n++) {
                            elem_length += 1;
                        }
                        elem_length += 1;
                    } else {
                        elem_length += name[0] + 1;
                    }
                    break;
                }
        case LF_FRIENDFCN_16t:
            StdOutPuts(L"LF_FRIENDFCN_16t, ");
            StdOutPrintf(L"type = %s", SzNameC7Type(r->lf_friendfcn_16t.index));
            StdOutPrintf(L"%s",L"\tfunction name = ");
            PrintSt(false,  r->lf_friendfcn_16t.Name);
            elem_length = offsetof(lfFriendFcn_16t, Name) + r->lf_friendfcn_16t.Name[0] + 1;;
            break;
        case LF_INDEX_16t:
            StdOutPrintf(L"Type Index = %s\n", SzNameC7Type(r->lf_index_16t.index));
            elem_length = sizeof(lfIndex_16t);
            break;
        case LF_MEMBER_16t:
            {
                StdOutPuts(L"LF_MEMBER_16t, ");
                StdOutClassFieldAttributesT7(false, r->lf_member_16t.attr);
                StdOutPrintf(L"type = %s, offset = ", SzNameC7Type(r->lf_member_16t.index));
                int o = PrintNumeric(r->lf_member_16t.offset);
                StdOutPrintf(L"%s", L"\n\t\tmember name = '");
                const unsigned char *name = r->lf_member_16t.offset + o;
                PrintSt(false, name, false);
                StdOutPuts(L";\n");
                elem_length = offsetof(lfMember_16t, offset) + o + name[0] + 1;
            }
            break;
        case LF_STMEMBER_16t:
            StdOutPuts(L"LF_STATICMEMBER_16t, ");
            StdOutClassFieldAttributesT7(false, r->lf_stmember_16t.attr);
            StdOutPrintf(L"type = %s", SzNameC7Type(r->lf_stmember_16t.index));
            StdOutPrintf(L"%s", L"\t\tmember name = ");
            PrintSt(false, r->lf_stmember_16t.Name);
            elem_length = offsetof(lfSTMember_16t, Name) + r->lf_stmember_16t.Name[0];
            break;
        case LF_METHOD_16t:
            StdOutPuts(L"LF_METHOD_16t, ");
            StdOutPrintf(L"count = %d, ", r->lf_method_16t.count);
            StdOutPrintf(L"list = %s, ", SzNameC7Type(r->lf_method_16t.mList));
            StdOutPrintf(L"%s", L"name = '");
            PrintSt(false, r->lf_method_16t.Name, false);
            StdOutPuts(L"'\n");
            elem_length = offsetof(lfMethod_16t, Name) + r->lf_method_16t.Name[0] + 1;
            break;
        case LF_NESTTYPE_16t:
            StdOutPuts(L"LF_NESTTYPE_16t, ");
            StdOutPrintf(L"type = %s, ", SzNameC7Type(r->lf_nesttype_16t.index));
            PrintSt(false, r->lf_nesttype_16t.Name);
            elem_length = offsetof(lfNestType_16t, Name) + r->lf_nesttype_16t.Name[0] + 1;
            break;
        case LF_VFUNCTAB_16t:
            StdOutPuts(L"LF_VFUNCTAB_16t, ");
            StdOutPrintf(L"type = %s\n", SzNameC7Type(r->lf_vfunctab_16t.type));
            elem_length = sizeof(lfVFuncTab);
            break;
        case LF_FRIENDCLS_16t:
            StdOutPuts(L"LF_FRIENDCLS_16t, ");
            StdOutPrintf(L"type = %s\n", SzNameC7Type(r->lf_friendcls_16t.index));
            elem_length = sizeof(lfFriendCls_16t);
            break;
        case LF_ONEMETHOD_16t:
            {
                StdOutPuts(L"LF_ONEMETHOD_16t, ");
                StdOutClassFieldAttributesT7(true, r->lf_onemethod_16t.attr);
                StdOutPrintf(L"index = %s, ", SzNameC7Type(r->lf_onemethod_16t.index));
                int o = 0;
                if (r->lf_onemethod_16t.attr.mprop == CV_MTintro || r->lf_onemethod_16t.attr.mprop == CV_MTpureintro) {
                    StdOutPrintf(L"\n\t\tvfptr offset = %ld, ", r->lf_onemethod_16t.vbaseoff[0]);
                    o += sizeof(long);
                }
                StdOutPrintf(L"%s", L"name = '");
                const unsigned char *name = ((const unsigned char *) r->lf_onemethod_16t.vbaseoff) + o;
                PrintSt(false, name, false);
                StdOutPuts(L"'\n");
                elem_length = offsetof(lfOneMethod, vbaseoff) + offset + name[0] + 1;
            }
            break;
        }

        current_data += elem_length;
        offset += elem_length;
        if (offset >= length) {
            break;
        }
        if ((*current_data & 0xf0) == 0xf0) {
            offset += *current_data & 0xf;
            current_data += *current_data & 0xf;
        }
        if (offset >= length) {
            break;
        }
        if (((current_data - (const unsigned char *)data) & 0x3) != 0) {
            StdOutPuts(L"Error : Leaf is not aligned on a 4 byte boundary\n");
        }
    }
}

void DumpModTypC7(size_t cbTyp)
{
    DWORD l = 4096;
    while (cbTyp != 0) {
        if (_read(exefile, RecBuf, sizeof(unsigned short)) != sizeof(unsigned short)) {
            Fatal(L"Types subsection wrong length");
        }
        WORD len_type = *(unsigned short *)RecBuf;
        if (len_type > 0xfffc) {
            Fatal(L"Type string too long");
        }
        DWORD actual_read = _read(exefile, RecBuf + sizeof(unsigned short), len_type);
        if (actual_read != len_type) {
            Fatal(L"Types subsection wrong length");
        }
        if (fRaw) {
            if (len_type != 0xfffe) {
                for (int i = 0; i < len_type + 2; i += 2) {
                    StdOutPrintf(L"  %02x  %02x", RecBuf[i], RecBuf[i + 1]);
                }
            }
            StdOutPutc(L'\n');
        }
        DumpTypRecC7(l, len_type, RecBuf + 2, NULL, NULL);
        cbTyp -= sizeof(unsigned short) + len_type;
        l = actual_read;
    }
}

void DumpCobolCompileFlagsT7(DWORD length, const unsigned char *data)
{
    StdOutPuts(L" Level = 0 ");
    if (length == 0x101) {
        StdOutPuts(L"dump of cobol compiler flags :");
        const unsigned char *slice = data + 1;
        for (int i = 0; i < 256; i++) {
            if (i % 16 == 0) {
                StdOutPrintf(L"\n\t%02x  ", i);
            }
            StdOutPrintf(L" %02x", slice[i]);
            if (i % 8 == 7) {
                StdOutPuts(L" ");
            }
        }
    } else if (*(unsigned short *)(data + 1) == 0) {
        StdOutPuts(L"name algorithm is decimal with ");
        StdOutPrintf(L"root = \"%*S\"", data[3], data + 4);
    } else {
        StdOutPuts(L"name algorithm - unknown ");
    }
    StdOutPutc(L'\n');
}

void StdOutClassProperties(CV_prop_t prop)
{
#define PRINT_PROP(NAME)           \
    do {                           \
        if (items_on_line == 4) {  \
            StdOutPuts(L"\n\t\t"); \
            items_on_line = 0;     \
        }                          \
        StdOutPuts(NAME);          \
        items_on_line += 1;        \
    } while (0)

    int items_on_line = 0;

    if (prop.packed) {
        PRINT_PROP(L"PACKED, ");
    }
    if (prop.ctor) {
        PRINT_PROP(L"CONSTRUCTOR, ");
    }
    if (prop.ovlops) {
        PRINT_PROP(L"OVERLOAD, ");
    }
    if (prop.isnested) {
        PRINT_PROP(L"NESTED, ");
    }
    if (prop.scoped) {
        PRINT_PROP(L"LOCAL, ");
    }
    if (prop.cnested) {
        PRINT_PROP(L"CONTAINS NESTED, ");
    }
    if (prop.opassign) {
        PRINT_PROP(L"OVERLOADED ASSIGNMENT, ");
    }
    if (prop.opcast) {
        PRINT_PROP(L"CASTING, ");
    }
    if (prop.fwdref) {
        PRINT_PROP(L"FORWARD REF, ");
    }
    if (prop.sealed) {
        PRINT_PROP(L"SEALED, ");
    }
    if (prop.intrinsic) {
        PRINT_PROP(L"INTRINSIC TYPE, ");
    }
    if (prop.hfa) {
        if (items_on_line == 4) {
            StdOutPuts(L"\n\t\t");
            items_on_line = 0;
        }
        switch (prop.hfa) {
        case CV_HFA_float:
            StdOutPuts(L"HFA float, ");
            break;
        case CV_HFA_double:
            StdOutPuts(L"HFA double, ");
            break;
        case CV_HFA_other:
            StdOutPuts(L"HFA other, ");
            break;
        default:
            StdOutPuts(L"****Warning**** HFA field contains invalid value!");
            break;
        }
        items_on_line += 1;
    }
    if (prop.mocom) {
        if (items_on_line == 4) {
            StdOutPuts(L"\n\t\t");
        }
        switch (prop.mocom) {
        case CV_MOCOM_UDT_ref:
            StdOutPuts(L"REF");
            break;
        case CV_MOCOM_UDT_value:
            StdOutPuts(L"VALUE");
            break;
        case CV_MOCOM_UDT_interface:
            StdOutPuts(L"INTERFACE");
            break;
        }
        items_on_line += 1;
    }
#undef PRINT_PROP
}

const wchar_t *SzCallConvention(DWORD t) {
    if (t < _countof(rgszCallConventionNames)) {
        return rgszCallConventionNames[t];
    }
    return  L"???";
}

void StdOutClassFieldAttributesT7(bool b, CV_fldattr_t attr) {
    StdOutPrintf(L"%s, ", rgszAccess[attr.access]);
    if (b) {
        StdOutPrintf(L"%s, ", rgszMethodProp[attr.mprop]);
        if (attr.pseudo) {
            StdOutPuts(L"(pseudo), ");
        }
        if (attr.compgenx) {
            StdOutPuts(L"(compgenx), ");
        }
        if (attr.sealed) {
            StdOutPuts(L"(sealed), ");
        }
    }
    if (attr.noinherit) {
        StdOutPuts(L"(noinherit), ");
    }
    if (attr.noconstruct) {
        StdOutPuts(L"(noconstruct), ");
    }
}

const wchar_t *SzFunctionAttr(CV_funcattr_t attr)
{
    if (attr.cxxreturnudt) {
        return L"return UDT (C++ style)";
    }
    if (attr.ctor) {
        return L"instance constructor";
    }
    if (attr.ctorvbase) {
        return L"instance constructor of a class with virtual base";
    }
    if (attr.unused) {
        return L"none";
    }
    return L"****Warning**** unused field non-zero!";;
}

void StdOutHexBlockT7(const unsigned char *data, WORD size)
{
    for (int i = 0; i < size; ) {
        StdOutPrintf(L" 0x%02x", data[i]);
        i += 1;
        if (i >= size) {
            break;
        }
        if (i % 8 == 0) {
            StdOutPuts(L"\n\t");
        }
    }
}

void DumpCobolT7(DWORD *pLength, const unsigned char *data)
{
    // FIXME: Original accesses pLength as data?
    abort();
}

void StdOutHexBlockT7(unsigned char *buffer, WORD size)
{
    StdOutPuts(L"\t");
    for (int i = 0; i < size; ) {
        StdOutPrintf(L" 0x%02x", buffer[i]);
        i++;
        if ((i % 8) == 0) {
            if (i >= size) {
                break;
            }
            StdOutPuts(L"\n\t");
        }
    }
}
