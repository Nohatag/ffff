
/*

Index of this file:
// Header mess
// Forward declarations
// STB libraries includes
// Context pointer
// Generic helpers
// Misc data structures
// Main imgui context
// Tab bar, tab item
// Internal API

*/

#pragma once

#ifndef IMGUI_VERSION
#error Must include imgui.h before imgui_internal.h
#endif

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>       
#include <limits.h>     

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251) 
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"        
#pragma clang diagnostic ignored "-Wmissing-prototypes"     
#pragma clang diagnostic ignored "-Wold-style-cast"
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                  
#pragma GCC diagnostic ignored "-Wclass-memaccess"          
#endif

#ifdef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS                
#error Use IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#endif
#ifdef IMGUI_DISABLE_MATH_FUNCTIONS                         
#error Use IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#endif

struct ImBoolVector;                
struct ImRect;                      
struct ImDrawDataBuilder;           
struct ImDrawListSharedData;        
struct ImGuiColorMod;               
struct ImGuiColumnData;             
struct ImGuiColumns;                
struct ImGuiContext;                
struct ImGuiDataTypeInfo;           
struct ImGuiGroupData;              
struct ImGuiInputTextState;         
struct ImGuiItemHoveredDataBackup;  
struct ImGuiMenuColumns;            
struct ImGuiNavMoveResult;          
struct ImGuiNextWindowData;         
struct ImGuiNextItemData;           
struct ImGuiPopupData;              
struct ImGuiSettingsHandler;        
struct ImGuiStyleMod;               
struct ImGuiTabBar;                 
struct ImGuiTabItem;                
struct ImGuiWindow;                 
struct ImGuiWindowTempData;         
struct ImGuiWindowSettings;         

typedef int ImGuiLayoutType;            
typedef int ImGuiButtonFlags;           
typedef int ImGuiColumnsFlags;          
typedef int ImGuiDragFlags;             
typedef int ImGuiItemFlags;             
typedef int ImGuiItemStatusFlags;       
typedef int ImGuiNavHighlightFlags;     
typedef int ImGuiNavDirSourceFlags;     
typedef int ImGuiNavMoveFlags;          
typedef int ImGuiNextItemDataFlags;     
typedef int ImGuiNextWindowDataFlags;   
typedef int ImGuiSeparatorFlags;        
typedef int ImGuiSliderFlags;           
typedef int ImGuiTextFlags;             

namespace ImStb
{

#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_STRING             ImGuiInputTextState
#define STB_TEXTEDIT_CHARTYPE           ImWchar
#define STB_TEXTEDIT_GETWIDTH_NEWLINE   -1.0f
#define STB_TEXTEDIT_UNDOSTATECOUNT     99
#define STB_TEXTEDIT_UNDOCHARCOUNT      999
#include "imstb_textedit.h"

} 

#ifndef GImGui
extern IMGUI_API ImGuiContext* GImGui;  
#endif

#ifndef IMGUI_DEBUG_LOG
#define IMGUI_DEBUG_LOG(_FMT,...)       printf("[%05d] " _FMT, GImGui->FrameCount, __VA_ARGS__)
#endif

#if (__cplusplus >= 201100)
#define IM_STATIC_ASSERT(_COND)         static_assert(_COND, "")
#else
#define IM_STATIC_ASSERT(_COND)         typedef char static_assertion_##__line__[(_COND)?1:-1]
#endif

#ifdef IMGUI_DEBUG_PARANOID
#define IM_ASSERT_PARANOID(_EXPR)       IM_ASSERT(_EXPR)
#else
#define IM_ASSERT_PARANOID(_EXPR)   
#endif

#ifndef IM_ASSERT_USER_ERROR
#define IM_ASSERT_USER_ERROR(_EXP,_MSG) IM_ASSERT((_EXP) && _MSG)   
#endif

#define IM_PI                           3.14159265358979323846f
#ifdef _WIN32
#define IM_NEWLINE                      "\r\n"   
#else
#define IM_NEWLINE                      "\n"
#endif
#define IM_TABSIZE                      (4)
#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))   
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))               
#define IM_FLOOR(_VAL)                  ((float)(int)(_VAL))                                    
#define IM_ROUND(_VAL)                  ((float)(int)((_VAL) + 0.5f))                           

#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif

#define ImQsort         qsort
IMGUI_API ImU32         ImHashData(const void* data, size_t data_size, ImU32 seed = 0);
IMGUI_API ImU32         ImHashStr(const char* data, size_t data_size = 0, ImU32 seed = 0);
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
static inline ImU32     ImHash(const void* data, int size, ImU32 seed = 0) { return size ? ImHashData(data, (size_t)size, seed) : ImHashStr((const char*)data, 0, seed); } 
#endif

static inline bool      ImIsPowerOfTwo(int v)           { return v != 0 && (v & (v - 1)) == 0; }
static inline int       ImUpperPowerOfTwo(int v)        { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }

IMGUI_API int           ImStricmp(const char* str1, const char* str2);
IMGUI_API int           ImStrnicmp(const char* str1, const char* str2, size_t count);
IMGUI_API void          ImStrncpy(char* dst, const char* src, size_t count);
IMGUI_API char*         ImStrdup(const char* str);
IMGUI_API char*         ImStrdupcpy(char* dst, size_t* p_dst_size, const char* str);
IMGUI_API const char*   ImStrchrRange(const char* str_begin, const char* str_end, char c);
IMGUI_API int           ImStrlenW(const ImWchar* str);
IMGUI_API const char*   ImStreolRange(const char* str, const char* str_end);                
IMGUI_API const ImWchar*ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin);   
IMGUI_API const char*   ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end);
IMGUI_API void          ImStrTrimBlanks(char* str);
IMGUI_API const char*   ImStrSkipBlank(const char* str);
IMGUI_API int           ImFormatString(char* buf, size_t buf_size, const char* fmt, ...) IM_FMTARGS(3);
IMGUI_API int           ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args) IM_FMTLIST(3);
IMGUI_API const char*   ImParseFormatFindStart(const char* format);
IMGUI_API const char*   ImParseFormatFindEnd(const char* format);
IMGUI_API const char*   ImParseFormatTrimDecorations(const char* format, char* buf, size_t buf_size);
IMGUI_API int           ImParseFormatPrecision(const char* format, int default_value);
static inline bool      ImCharIsBlankA(char c)          { return c == ' ' || c == '\t'; }
static inline bool      ImCharIsBlankW(unsigned int c)  { return c == ' ' || c == '\t' || c == 0x3000; }

IMGUI_API int           ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end);      
IMGUI_API int           ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);          
IMGUI_API int           ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);   
IMGUI_API int           ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                            
IMGUI_API int           ImTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end);                        
IMGUI_API int           ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end);                   

#ifdef IMGUI_DEFINE_MATH_OPERATORS
static inline ImVec2 operator*(const ImVec2& lhs, const float rhs)              { return ImVec2(lhs.x*rhs, lhs.y*rhs); }
static inline ImVec2 operator/(const ImVec2& lhs, const float rhs)              { return ImVec2(lhs.x/rhs, lhs.y/rhs); }
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x*rhs.x, lhs.y*rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x/rhs.x, lhs.y/rhs.y); }
static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs)                { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs)                { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2& lhs, const float rhs)                  { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const float rhs)                  { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z, lhs.w+rhs.w); }
static inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z, lhs.w-rhs.w); }
static inline ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z, lhs.w*rhs.w); }
#endif

#if defined(__EMSCRIPTEN__) && !defined(IMGUI_DISABLE_FILE_FUNCTIONS)
#define IMGUI_DISABLE_FILE_FUNCTIONS
#endif
#ifdef IMGUI_DISABLE_FILE_FUNCTIONS
#define IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef void* ImFileHandle;
static inline ImFileHandle  ImFileOpen(const char*, const char*)                    { return NULL; }
static inline bool          ImFileClose(ImFileHandle)                               { return false; }
static inline ImU64         ImFileGetSize(ImFileHandle)                             { return (ImU64)-1; }
static inline ImU64         ImFileRead(void*, ImU64, ImU64, ImFileHandle)           { return 0; }
static inline ImU64         ImFileWrite(const void*, ImU64, ImU64, ImFileHandle)    { return 0; }
#endif

#ifndef IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef FILE* ImFileHandle;
IMGUI_API ImFileHandle      ImFileOpen(const char* filename, const char* mode);
IMGUI_API bool              ImFileClose(ImFileHandle file);
IMGUI_API ImU64             ImFileGetSize(ImFileHandle file);
IMGUI_API ImU64             ImFileRead(void* data, ImU64 size, ImU64 count, ImFileHandle file);
IMGUI_API ImU64             ImFileWrite(const void* data, ImU64 size, ImU64 count, ImFileHandle file);
#else
#define IMGUI_DISABLE_TTY_FUNCTIONS 
#endif
IMGUI_API void*             ImFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size = NULL, int padding_bytes = 0);

#ifndef IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
static inline float  ImFabs(float x)                                            { return fabsf(x); }
static inline float  ImSqrt(float x)                                            { return sqrtf(x); }
static inline float  ImPow(float x, float y)                                    { return powf(x, y); }
static inline double ImPow(double x, double y)                                  { return pow(x, y); }
static inline float  ImFmod(float x, float y)                                   { return fmodf(x, y); }
static inline double ImFmod(double x, double y)                                 { return fmod(x, y); }
static inline float  ImCos(float x)                                             { return cosf(x); }
static inline float  ImSin(float x)                                             { return sinf(x); }
static inline float  ImAcos(float x)                                            { return acosf(x); }
static inline float  ImAtan2(float y, float x)                                  { return atan2f(y, x); }
static inline double ImAtof(const char* s)                                      { return atof(s); }
static inline float  ImFloorStd(float x)                                        { return floorf(x); }   
static inline float  ImCeil(float x)                                            { return ceilf(x); }
#endif

template<typename T> static inline T ImMin(T lhs, T rhs)                        { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T ImMax(T lhs, T rhs)                        { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T ImClamp(T v, T mn, T mx)                   { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T ImLerp(T a, T b, float t)                  { return (T)(a + (b - a) * t); }
template<typename T> static inline void ImSwap(T& a, T& b)                      { T tmp = a; a = b; b = tmp; }
template<typename T> static inline T ImAddClampOverflow(T a, T b, T mn, T mx)   { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> static inline T ImSubClampOverflow(T a, T b, T mn, T mx)   { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }

static inline ImVec2 ImMin(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImMax(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImClamp(const ImVec2& v, const ImVec2& mn, ImVec2 mx)      { return ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t)          { return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t)  { return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
static inline ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t)          { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
static inline float  ImSaturate(float f)                                        { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
static inline float  ImLengthSqr(const ImVec2& lhs)                             { return lhs.x*lhs.x + lhs.y*lhs.y; }
static inline float  ImLengthSqr(const ImVec4& lhs)                             { return lhs.x*lhs.x + lhs.y*lhs.y + lhs.z*lhs.z + lhs.w*lhs.w; }
static inline float  ImInvLength(const ImVec2& lhs, float fail_value)           { float d = lhs.x*lhs.x + lhs.y*lhs.y; if (d > 0.0f) return 1.0f / ImSqrt(d); return fail_value; }
static inline float  ImFloor(float f)                                           { return (float)(int)(f); }
static inline ImVec2 ImFloor(const ImVec2& v)                                   { return ImVec2((float)(int)(v.x), (float)(int)(v.y)); }
static inline int    ImModPositive(int a, int b)                                { return (a + b) % b; }
static inline float  ImDot(const ImVec2& a, const ImVec2& b)                    { return a.x * b.x + a.y * b.y; }
static inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a)        { return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
static inline float  ImLinearSweep(float current, float target, float speed)    { if (current < target) return ImMin(current + speed, target); if (current > target) return ImMax(current - speed, target); return current; }
static inline ImVec2 ImMul(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }

IMGUI_API ImVec2     ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p);
IMGUI_API bool       ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
IMGUI_API ImVec2     ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
IMGUI_API void       ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w);
inline float         ImTriangleArea(const ImVec2& a, const ImVec2& b, const ImVec2& c) { return ImFabs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y))) * 0.5f; }
IMGUI_API ImGuiDir   ImGetDirQuadrantFromDelta(float dx, float dy);

struct IMGUI_API ImBoolVector
{
    ImVector<int>   Storage;
    ImBoolVector()  { }
    void            Resize(int sz)          { Storage.resize((sz + 31) >> 5); memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear()                 { Storage.clear(); }
    bool            GetBit(int n) const     { int off = (n >> 5); int mask = 1 << (n & 31); return (Storage[off] & mask) != 0; }
    void            SetBit(int n, bool v)   { int off = (n >> 5); int mask = 1 << (n & 31); if (v) Storage[off] |= mask; else Storage[off] &= ~mask; }
};

typedef int ImPoolIdx;
template<typename T>
struct IMGUI_API ImPool
{
    ImVector<T>     Buf;        
    ImGuiStorage    Map;        
    ImPoolIdx       FreeIdx;    

    ImPool()    { FreeIdx = 0; }
    ~ImPool()   { Clear(); }
    T*          GetByKey(ImGuiID key)               { int idx = Map.GetInt(key, -1); return (idx != -1) ? &Buf[idx] : NULL; }
    T*          GetByIndex(ImPoolIdx n)             { return &Buf[n]; }
    ImPoolIdx   GetIndex(const T* p) const          { IM_ASSERT(p >= Buf.Data && p < Buf.Data + Buf.Size); return (ImPoolIdx)(p - Buf.Data); }
    T*          GetOrAddByKey(ImGuiID key)          { int* p_idx = Map.GetIntRef(key, -1); if (*p_idx != -1) return &Buf[*p_idx]; *p_idx = FreeIdx; return Add(); }
    bool        Contains(const T* p) const          { return (p >= Buf.Data && p < Buf.Data + Buf.Size); }
    void        Clear()                             { for (int n = 0; n < Map.Data.Size; n++) { int idx = Map.Data[n].val_i; if (idx != -1) Buf[idx].~T(); } Map.Clear(); Buf.clear(); FreeIdx = 0; }
    T*          Add()                               { int idx = FreeIdx; if (idx == Buf.Size) { Buf.resize(Buf.Size + 1); FreeIdx++; } else { FreeIdx = *(int*)&Buf[idx]; } IM_PLACEMENT_NEW(&Buf[idx]) T(); return &Buf[idx]; }
    void        Remove(ImGuiID key, const T* p)     { Remove(key, GetIndex(p)); }
    void        Remove(ImGuiID key, ImPoolIdx idx)  { Buf[idx].~T(); *(int*)&Buf[idx] = FreeIdx; FreeIdx = idx; Map.SetInt(key, -1); }
    void        Reserve(int capacity)               { Buf.reserve(capacity); Map.Data.reserve(capacity); }
    int         GetSize() const                     { return Buf.Size; }
};

template<typename T>
struct IMGUI_API ImChunkStream
{
    ImVector<char>  Buf;

    void    clear()                     { Buf.clear(); }
    bool    empty() const               { return Buf.Size == 0; }
    int     size() const                { return Buf.Size; }
    T*      alloc_chunk(size_t sz)      { size_t HDR_SZ = 4; sz = ((HDR_SZ + sz) + 3u) & ~3u; int off = Buf.Size; Buf.resize(off + (int)sz); ((int*)(void*)(Buf.Data + off))[0] = (int)sz; return (T*)(void*)(Buf.Data + off + (int)HDR_SZ); }
    T*      begin()                     { size_t HDR_SZ = 4; if (!Buf.Data) return NULL; return (T*)(void*)(Buf.Data + HDR_SZ); }
    T*      next_chunk(T* p)            { size_t HDR_SZ = 4; IM_ASSERT(p >= begin() && p < end()); p = (T*)(void*)((char*)(void*)p + chunk_size(p)); if (p == (T*)(void*)((char*)end() + HDR_SZ)) return (T*)0; IM_ASSERT(p < end()); return p; }
    int     chunk_size(const T* p)      { return ((const int*)p)[-1]; }
    T*      end()                       { return (T*)(void*)(Buf.Data + Buf.Size); }
    int     offset_from_ptr(const T* p) { IM_ASSERT(p >= begin() && p < end()); const ptrdiff_t off = (const char*)p - Buf.Data; return (int)off; }
    T*      ptr_from_offset(int off)    { IM_ASSERT(off >= 4 && off < Buf.Size); return (T*)(void*)(Buf.Data + off); }
};

enum ImGuiButtonFlags_
{
    ImGuiButtonFlags_None                   = 0,
    ImGuiButtonFlags_Repeat                 = 1 << 0,   
    ImGuiButtonFlags_PressedOnClickRelease  = 1 << 1,   
    ImGuiButtonFlags_PressedOnClick         = 1 << 2,   
    ImGuiButtonFlags_PressedOnRelease       = 1 << 3,   
    ImGuiButtonFlags_PressedOnDoubleClick   = 1 << 4,   
    ImGuiButtonFlags_FlattenChildren        = 1 << 5,   
    ImGuiButtonFlags_AllowItemOverlap       = 1 << 6,   
    ImGuiButtonFlags_DontClosePopups        = 1 << 7,   
    ImGuiButtonFlags_Disabled               = 1 << 8,   
    ImGuiButtonFlags_AlignTextBaseLine      = 1 << 9,   
    ImGuiButtonFlags_NoKeyModifiers         = 1 << 10,  
    ImGuiButtonFlags_NoHoldingActiveID      = 1 << 11,  
    ImGuiButtonFlags_PressedOnDragDropHold  = 1 << 12,  
    ImGuiButtonFlags_NoNavFocus             = 1 << 13,  
    ImGuiButtonFlags_NoHoveredOnNav         = 1 << 14   
};

enum ImGuiSliderFlags_
{
    ImGuiSliderFlags_None                   = 0,
    ImGuiSliderFlags_Vertical               = 1 << 0
};

enum ImGuiDragFlags_
{
    ImGuiDragFlags_None                     = 0,
    ImGuiDragFlags_Vertical                 = 1 << 0
};

enum ImGuiColumnsFlags_
{

    ImGuiColumnsFlags_None                  = 0,
    ImGuiColumnsFlags_NoBorder              = 1 << 0,   
    ImGuiColumnsFlags_NoResize              = 1 << 1,   
    ImGuiColumnsFlags_NoPreserveWidths      = 1 << 2,   
    ImGuiColumnsFlags_NoForceWithinWindow   = 1 << 3,   
    ImGuiColumnsFlags_GrowParentContentsSize= 1 << 4    
};

enum ImGuiSelectableFlagsPrivate_
{

    ImGuiSelectableFlags_NoHoldingActiveID  = 1 << 20,
    ImGuiSelectableFlags_PressedOnClick     = 1 << 21,
    ImGuiSelectableFlags_PressedOnRelease   = 1 << 22,
    ImGuiSelectableFlags_DrawFillAvailWidth = 1 << 23,  
    ImGuiSelectableFlags_DrawHoveredWhenHeld= 1 << 24,  
    ImGuiSelectableFlags_SetNavIdOnHover    = 1 << 25
};

enum ImGuiTreeNodeFlagsPrivate_
{
    ImGuiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 20
};

enum ImGuiSeparatorFlags_
{
    ImGuiSeparatorFlags_None                = 0,
    ImGuiSeparatorFlags_Horizontal          = 1 << 0,   
    ImGuiSeparatorFlags_Vertical            = 1 << 1,
    ImGuiSeparatorFlags_SpanAllColumns      = 1 << 2
};

enum ImGuiItemFlags_
{
    ImGuiItemFlags_None                     = 0,
    ImGuiItemFlags_NoTabStop                = 1 << 0,  
    ImGuiItemFlags_ButtonRepeat             = 1 << 1,  
    ImGuiItemFlags_Disabled                 = 1 << 2,  
    ImGuiItemFlags_NoNav                    = 1 << 3,  
    ImGuiItemFlags_NoNavDefaultFocus        = 1 << 4,  
    ImGuiItemFlags_SelectableDontClosePopup = 1 << 5,  
    ImGuiItemFlags_MixedValue               = 1 << 6,  
    ImGuiItemFlags_Default_                 = 0
};

enum ImGuiItemStatusFlags_
{
    ImGuiItemStatusFlags_None               = 0,
    ImGuiItemStatusFlags_HoveredRect        = 1 << 0,
    ImGuiItemStatusFlags_HasDisplayRect     = 1 << 1,
    ImGuiItemStatusFlags_Edited             = 1 << 2,   
    ImGuiItemStatusFlags_ToggledSelection   = 1 << 3,   
    ImGuiItemStatusFlags_ToggledOpen        = 1 << 4,   
    ImGuiItemStatusFlags_HasDeactivated     = 1 << 5,   
    ImGuiItemStatusFlags_Deactivated        = 1 << 6    

#ifdef IMGUI_ENABLE_TEST_ENGINE
    , 
    ImGuiItemStatusFlags_Openable           = 1 << 10,  
    ImGuiItemStatusFlags_Opened             = 1 << 11,  
    ImGuiItemStatusFlags_Checkable          = 1 << 12,  
    ImGuiItemStatusFlags_Checked            = 1 << 13   
#endif
};

enum ImGuiTextFlags_
{
    ImGuiTextFlags_None = 0,
    ImGuiTextFlags_NoWidthForLargeClippedText = 1 << 0
};

enum ImGuiLayoutType_
{
    ImGuiLayoutType_Horizontal = 0,
    ImGuiLayoutType_Vertical = 1
};

enum ImGuiLogType
{
    ImGuiLogType_None = 0,
    ImGuiLogType_TTY,
    ImGuiLogType_File,
    ImGuiLogType_Buffer,
    ImGuiLogType_Clipboard
};

enum ImGuiAxis
{
    ImGuiAxis_None = -1,
    ImGuiAxis_X = 0,
    ImGuiAxis_Y = 1
};

enum ImGuiPlotType
{
    ImGuiPlotType_Lines,
    ImGuiPlotType_Histogram
};

enum ImGuiInputSource
{
    ImGuiInputSource_None = 0,
    ImGuiInputSource_Mouse,
    ImGuiInputSource_Nav,
    ImGuiInputSource_NavKeyboard,   
    ImGuiInputSource_NavGamepad,    
    ImGuiInputSource_COUNT
};

enum ImGuiInputReadMode
{
    ImGuiInputReadMode_Down,
    ImGuiInputReadMode_Pressed,
    ImGuiInputReadMode_Released,
    ImGuiInputReadMode_Repeat,
    ImGuiInputReadMode_RepeatSlow,
    ImGuiInputReadMode_RepeatFast
};

enum ImGuiNavHighlightFlags_
{
    ImGuiNavHighlightFlags_None         = 0,
    ImGuiNavHighlightFlags_TypeDefault  = 1 << 0,
    ImGuiNavHighlightFlags_TypeThin     = 1 << 1,
    ImGuiNavHighlightFlags_AlwaysDraw   = 1 << 2,       
    ImGuiNavHighlightFlags_NoRounding   = 1 << 3
};

enum ImGuiNavDirSourceFlags_
{
    ImGuiNavDirSourceFlags_None         = 0,
    ImGuiNavDirSourceFlags_Keyboard     = 1 << 0,
    ImGuiNavDirSourceFlags_PadDPad      = 1 << 1,
    ImGuiNavDirSourceFlags_PadLStick    = 1 << 2
};

enum ImGuiNavMoveFlags_
{
    ImGuiNavMoveFlags_None                  = 0,
    ImGuiNavMoveFlags_LoopX                 = 1 << 0,   
    ImGuiNavMoveFlags_LoopY                 = 1 << 1,
    ImGuiNavMoveFlags_WrapX                 = 1 << 2,   
    ImGuiNavMoveFlags_WrapY                 = 1 << 3,   
    ImGuiNavMoveFlags_AllowCurrentNavId     = 1 << 4,   
    ImGuiNavMoveFlags_AlsoScoreVisibleSet   = 1 << 5,   
    ImGuiNavMoveFlags_ScrollToEdge          = 1 << 6
};

enum ImGuiNavForward
{
    ImGuiNavForward_None,
    ImGuiNavForward_ForwardQueued,
    ImGuiNavForward_ForwardActive
};

enum ImGuiNavLayer
{
    ImGuiNavLayer_Main  = 0,    
    ImGuiNavLayer_Menu  = 1,    
    ImGuiNavLayer_COUNT
};

enum ImGuiPopupPositionPolicy
{
    ImGuiPopupPositionPolicy_Default,
    ImGuiPopupPositionPolicy_ComboBox
};

struct ImVec1
{
    float   x;
    ImVec1()         { x = 0.0f; }
    ImVec1(float _x) { x = _x; }
};

struct ImVec2ih
{
    short   x, y;
    ImVec2ih()                   { x = y = 0; }
    ImVec2ih(short _x, short _y) { x = _x; y = _y; }
};

struct IMGUI_API ImRect
{
    ImVec2      Min;    
    ImVec2      Max;    

    ImRect()                                        : Min(0.0f, 0.0f), Max(0.0f, 0.0f)              {}
    ImRect(const ImVec2& min, const ImVec2& max)    : Min(min), Max(max)                            {}
    ImRect(const ImVec4& v)                         : Min(v.x, v.y), Max(v.z, v.w)                  {}
    ImRect(float x1, float y1, float x2, float y2)  : Min(x1, y1), Max(x2, y2)                      {}

    ImVec2      GetCenter() const                   { return ImVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
    ImVec2      GetSize() const                     { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
    float       GetWidth() const                    { return Max.x - Min.x; }
    float       GetHeight() const                   { return Max.y - Min.y; }
    ImVec2      GetTL() const                       { return Min; }                   
    ImVec2      GetTR() const                       { return ImVec2(Max.x, Min.y); }  
    ImVec2      GetBL() const                       { return ImVec2(Min.x, Max.y); }  
    ImVec2      GetBR() const                       { return Max; }                   
    bool        Contains(const ImVec2& p) const     { return p.x     >= Min.x && p.y     >= Min.y && p.x     <  Max.x && p.y     <  Max.y; }
    bool        Contains(const ImRect& r) const     { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x <= Max.x && r.Max.y <= Max.y; }
    bool        Overlaps(const ImRect& r) const     { return r.Min.y <  Max.y && r.Max.y >  Min.y && r.Min.x <  Max.x && r.Max.x >  Min.x; }
    void        Add(const ImVec2& p)                { if (Min.x > p.x)     Min.x = p.x;     if (Min.y > p.y)     Min.y = p.y;     if (Max.x < p.x)     Max.x = p.x;     if (Max.y < p.y)     Max.y = p.y; }
    void        Add(const ImRect& r)                { if (Min.x > r.Min.x) Min.x = r.Min.x; if (Min.y > r.Min.y) Min.y = r.Min.y; if (Max.x < r.Max.x) Max.x = r.Max.x; if (Max.y < r.Max.y) Max.y = r.Max.y; }
    void        Expand(const float amount)          { Min.x -= amount;   Min.y -= amount;   Max.x += amount;   Max.y += amount; }
    void        Expand(const ImVec2& amount)        { Min.x -= amount.x; Min.y -= amount.y; Max.x += amount.x; Max.y += amount.y; }
    void        Translate(const ImVec2& d)          { Min.x += d.x; Min.y += d.y; Max.x += d.x; Max.y += d.y; }
    void        TranslateX(float dx)                { Min.x += dx; Max.x += dx; }
    void        TranslateY(float dy)                { Min.y += dy; Max.y += dy; }
    void        ClipWith(const ImRect& r)           { Min = ImMax(Min, r.Min); Max = ImMin(Max, r.Max); }                   
    void        ClipWithFull(const ImRect& r)       { Min = ImClamp(Min, r.Min, r.Max); Max = ImClamp(Max, r.Min, r.Max); } 
    void        Floor()                             { Min.x = IM_FLOOR(Min.x); Min.y = IM_FLOOR(Min.y); Max.x = IM_FLOOR(Max.x); Max.y = IM_FLOOR(Max.y); }
    bool        IsInverted() const                  { return Min.x > Max.x || Min.y > Max.y; }
};

struct ImGuiDataTypeInfo
{
    size_t      Size;           
    const char* PrintFmt;       
    const char* ScanFmt;        
};

struct ImGuiColorMod
{
    ImGuiCol    Col;
    ImVec4      BackupValue;
};

struct ImGuiStyleMod
{
    ImGuiStyleVar   VarIdx;
    union           { int BackupInt[2]; float BackupFloat[2]; };
    ImGuiStyleMod(ImGuiStyleVar idx, int v)     { VarIdx = idx; BackupInt[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, float v)   { VarIdx = idx; BackupFloat[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, ImVec2 v)  { VarIdx = idx; BackupFloat[0] = v.x; BackupFloat[1] = v.y; }
};

struct ImGuiGroupData
{
    ImVec2      BackupCursorPos;
    ImVec2      BackupCursorMaxPos;
    ImVec1      BackupIndent;
    ImVec1      BackupGroupOffset;
    ImVec2      BackupCurrLineSize;
    float       BackupCurrLineTextBaseOffset;
    ImGuiID     BackupActiveIdIsAlive;
    bool        BackupActiveIdPreviousFrameIsAlive;
    bool        EmitItem;
};

struct IMGUI_API ImGuiMenuColumns
{
    float       Spacing;
    float       Width, NextWidth;
    float       Pos[3], NextWidths[3];

    ImGuiMenuColumns();
    void        Update(int count, float spacing, bool clear);
    float       DeclColumns(float w0, float w1, float w2);
    float       CalcExtraSpace(float avail_w) const;
};

struct IMGUI_API ImGuiInputTextState
{
    ImGuiID                 ID;                     
    int                     CurLenW, CurLenA;       
    ImVector<ImWchar>       TextW;                  
    ImVector<char>          TextA;                  
    ImVector<char>          InitialTextA;           
    bool                    TextAIsValid;           
    int                     BufCapacityA;           
    float                   ScrollX;                
    ImStb::STB_TexteditState Stb;                   
    float                   CursorAnim;             
    bool                    CursorFollow;           
    bool                    SelectedAllMouseLock;   
    ImGuiInputTextFlags     UserFlags;              
    ImGuiInputTextCallback  UserCallback;           
    void*                   UserCallbackData;       

    ImGuiInputTextState()                   { memset(this, 0, sizeof(*this)); }
    void        ClearText()                 { CurLenW = CurLenA = 0; TextW[0] = 0; TextA[0] = 0; CursorClamp(); }
    void        ClearFreeMemory()           { TextW.clear(); TextA.clear(); InitialTextA.clear(); }
    int         GetUndoAvailCount() const   { return Stb.undostate.undo_point; }
    int         GetRedoAvailCount() const   { return STB_TEXTEDIT_UNDOSTATECOUNT - Stb.undostate.redo_point; }
    void        OnKeyPressed(int key);      

    void        CursorAnimReset()           { CursorAnim = -0.30f; }                                   
    void        CursorClamp()               { Stb.cursor = ImMin(Stb.cursor, CurLenW); Stb.select_start = ImMin(Stb.select_start, CurLenW); Stb.select_end = ImMin(Stb.select_end, CurLenW); }
    bool        HasSelection() const        { return Stb.select_start != Stb.select_end; }
    void        ClearSelection()            { Stb.select_start = Stb.select_end = Stb.cursor; }
    void        SelectAll()                 { Stb.select_start = 0; Stb.cursor = Stb.select_end = CurLenW; Stb.has_preferred_x = 0; }
};

struct ImGuiWindowSettings
{
    ImGuiID     ID;
    ImVec2ih    Pos;
    ImVec2ih    Size;
    bool        Collapsed;

    ImGuiWindowSettings()       { ID = 0; Pos = Size = ImVec2ih(0, 0); Collapsed = false; }
    char* GetName()             { return (char*)(this + 1); }
};

struct ImGuiSettingsHandler
{
    const char* TypeName;       
    ImGuiID     TypeHash;       
    void*       (*ReadOpenFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name);              
    void        (*ReadLineFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line); 
    void        (*WriteAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf);      
    void*       UserData;

    ImGuiSettingsHandler() { memset(this, 0, sizeof(*this)); }
};

struct ImGuiPopupData
{
    ImGuiID             PopupId;        
    ImGuiWindow*        Window;         
    ImGuiWindow*        SourceWindow;   
    int                 OpenFrameCount; 
    ImGuiID             OpenParentId;   
    ImVec2              OpenPopupPos;   
    ImVec2              OpenMousePos;   

    ImGuiPopupData() { PopupId = 0; Window = SourceWindow = NULL; OpenFrameCount = -1; OpenParentId = 0; }
};

struct ImGuiColumnData
{
    float               OffsetNorm;         
    float               OffsetNormBeforeResize;
    ImGuiColumnsFlags   Flags;              
    ImRect              ClipRect;

    ImGuiColumnData()   { OffsetNorm = OffsetNormBeforeResize = 0.0f; Flags = ImGuiColumnsFlags_None; }
};

struct ImGuiColumns
{
    ImGuiID             ID;
    ImGuiColumnsFlags   Flags;
    bool                IsFirstFrame;
    bool                IsBeingResized;
    int                 Current;
    int                 Count;
    float               OffMinX, OffMaxX;       
    float               LineMinY, LineMaxY;
    float               HostCursorPosY;         
    float               HostCursorMaxPosX;      
    ImRect              HostClipRect;           
    ImRect              HostWorkRect;           
    ImVector<ImGuiColumnData> Columns;

    ImGuiColumns()      { Clear(); }
    void Clear()
    {
        ID = 0;
        Flags = ImGuiColumnsFlags_None;
        IsFirstFrame = false;
        IsBeingResized = false;
        Current = 0;
        Count = 1;
        OffMinX = OffMaxX = 0.0f;
        LineMinY = LineMaxY = 0.0f;
        HostCursorPosY = 0.0f;
        HostCursorMaxPosX = 0.0f;
        Columns.clear();
    }
};

struct IMGUI_API ImDrawListSharedData
{
    ImVec2          TexUvWhitePixel;            
    ImFont*         Font;                       
    float           FontSize;                   
    float           CurveTessellationTol;       
    ImVec4          ClipRectFullscreen;         
    ImDrawListFlags InitialFlags;               

    ImVec2          CircleVtx12[12];

    ImDrawListSharedData();
};

struct ImDrawDataBuilder
{
    ImVector<ImDrawList*>   Layers[2];           

    void Clear()            { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].resize(0); }
    void ClearFreeMemory()  { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].clear(); }
    IMGUI_API void FlattenIntoSingleLayer();
};

struct ImGuiNavMoveResult
{
    ImGuiID       ID;           
    ImGuiID       SelectScopeId;
    ImGuiWindow*  Window;       
    float         DistBox;      
    float         DistCenter;   
    float         DistAxial;
    ImRect        RectRel;      

    ImGuiNavMoveResult() { Clear(); }
    void Clear()         { ID = SelectScopeId = 0; Window = NULL; DistBox = DistCenter = DistAxial = FLT_MAX; RectRel = ImRect(); }
};

enum ImGuiNextWindowDataFlags_
{
    ImGuiNextWindowDataFlags_None               = 0,
    ImGuiNextWindowDataFlags_HasPos             = 1 << 0,
    ImGuiNextWindowDataFlags_HasSize            = 1 << 1,
    ImGuiNextWindowDataFlags_HasContentSize     = 1 << 2,
    ImGuiNextWindowDataFlags_HasCollapsed       = 1 << 3,
    ImGuiNextWindowDataFlags_HasSizeConstraint  = 1 << 4,
    ImGuiNextWindowDataFlags_HasFocus           = 1 << 5,
    ImGuiNextWindowDataFlags_HasBgAlpha         = 1 << 6
};

struct ImGuiNextWindowData
{
    ImGuiNextWindowDataFlags    Flags;
    ImGuiCond                   PosCond;
    ImGuiCond                   SizeCond;
    ImGuiCond                   CollapsedCond;
    ImVec2                      PosVal;
    ImVec2                      PosPivotVal;
    ImVec2                      SizeVal;
    ImVec2                      ContentSizeVal;
    bool                        CollapsedVal;
    ImRect                      SizeConstraintRect;
    ImGuiSizeCallback           SizeCallback;
    void*                       SizeCallbackUserData;
    float                       BgAlphaVal;
    ImVec2                      MenuBarOffsetMinVal;    

    ImGuiNextWindowData()       { memset(this, 0, sizeof(*this)); }
    inline void ClearFlags()    { Flags = ImGuiNextWindowDataFlags_None; }
};

enum ImGuiNextItemDataFlags_
{
    ImGuiNextItemDataFlags_None     = 0,
    ImGuiNextItemDataFlags_HasWidth = 1 << 0,
    ImGuiNextItemDataFlags_HasOpen  = 1 << 1
};

struct ImGuiNextItemData
{
    ImGuiNextItemDataFlags      Flags;
    float                       Width;          
    bool                        OpenVal;        
    ImGuiCond                   OpenCond;

    ImGuiNextItemData()         { memset(this, 0, sizeof(*this)); }
    inline void ClearFlags()    { Flags = ImGuiNextItemDataFlags_None; }
};

struct ImGuiShrinkWidthItem
{
    int             Index;
    float           Width;
};

struct ImGuiPtrOrIndex
{
    void*           Ptr;                
    int             Index;              

    ImGuiPtrOrIndex(void* ptr)          { Ptr = ptr; Index = -1; }
    ImGuiPtrOrIndex(int index)          { Ptr = NULL; Index = index; }
};

struct ImGuiContext
{
    bool                    Initialized;
    bool                    FontAtlasOwnedByContext;            
    ImGuiIO                 IO;
    ImGuiStyle              Style;
    ImFont*                 Font;                               
    float                   FontSize;                           
    float                   FontBaseSize;                       
    ImDrawListSharedData    DrawListSharedData;
    double                  Time;
    int                     FrameCount;
    int                     FrameCountEnded;
    int                     FrameCountRendered;
    bool                    WithinFrameScope;                   
    bool                    WithinFrameScopeWithImplicitWindow; 
    bool                    WithinEndChild;                     

    ImVector<ImGuiWindow*>  Windows;                            
    ImVector<ImGuiWindow*>  WindowsFocusOrder;                  
    ImVector<ImGuiWindow*>  WindowsSortBuffer;
    ImVector<ImGuiWindow*>  CurrentWindowStack;
    ImGuiStorage            WindowsById;                        
    int                     WindowsActiveCount;                 
    ImGuiWindow*            CurrentWindow;                      
    ImGuiWindow*            HoveredWindow;                      
    ImGuiWindow*            HoveredRootWindow;                  
    ImGuiWindow*            MovingWindow;                       
    ImGuiWindow*            WheelingWindow;                     
    ImVec2                  WheelingWindowRefMousePos;
    float                   WheelingWindowTimer;

    ImGuiID                 HoveredId;                          
    bool                    HoveredIdAllowOverlap;
    ImGuiID                 HoveredIdPreviousFrame;
    float                   HoveredIdTimer;                     
    float                   HoveredIdNotActiveTimer;            
    ImGuiID                 ActiveId;                           
    ImGuiID                 ActiveIdIsAlive;                    
    float                   ActiveIdTimer;
    bool                    ActiveIdIsJustActivated;            
    bool                    ActiveIdAllowOverlap;               
    bool                    ActiveIdHasBeenPressedBefore;       
    bool                    ActiveIdHasBeenEditedBefore;        
    bool                    ActiveIdHasBeenEditedThisFrame;
    ImU32                   ActiveIdUsingNavDirMask;            
    ImU32                   ActiveIdUsingNavInputMask;          
    ImU64                   ActiveIdUsingKeyInputMask;          
    ImVec2                  ActiveIdClickOffset;                
    ImGuiWindow*            ActiveIdWindow;
    ImGuiInputSource        ActiveIdSource;                     
    ImGuiID                 ActiveIdPreviousFrame;
    bool                    ActiveIdPreviousFrameIsAlive;
    bool                    ActiveIdPreviousFrameHasBeenEditedBefore;
    ImGuiWindow*            ActiveIdPreviousFrameWindow;
    ImGuiID                 LastActiveId;                       
    float                   LastActiveIdTimer;                  

    ImGuiNextWindowData     NextWindowData;                     
    ImGuiNextItemData       NextItemData;                       

    ImVector<ImGuiColorMod> ColorModifiers;                     
    ImVector<ImGuiStyleMod> StyleModifiers;                     
    ImVector<ImFont*>       FontStack;                          
    ImVector<ImGuiPopupData>OpenPopupStack;                     
    ImVector<ImGuiPopupData>BeginPopupStack;                    

    ImGuiWindow*            NavWindow;                          
    ImGuiID                 NavId;                              
    ImGuiID                 NavActivateId;                      
    ImGuiID                 NavActivateDownId;                  
    ImGuiID                 NavActivatePressedId;               
    ImGuiID                 NavInputId;                         
    ImGuiID                 NavJustTabbedId;                    
    ImGuiID                 NavJustMovedToId;                   
    ImGuiID                 NavJustMovedToMultiSelectScopeId;   
    ImGuiID                 NavNextActivateId;                  
    ImGuiInputSource        NavInputSource;                     
    ImRect                  NavScoringRectScreen;               
    int                     NavScoringCount;                    
    ImGuiWindow*            NavWindowingTarget;                 
    ImGuiWindow*            NavWindowingTargetAnim;             
    ImGuiWindow*            NavWindowingList;
    float                   NavWindowingTimer;
    float                   NavWindowingHighlightAlpha;
    bool                    NavWindowingToggleLayer;
    ImGuiNavLayer           NavLayer;                           
    int                     NavIdTabCounter;                    
    bool                    NavIdIsAlive;                       
    bool                    NavMousePosDirty;                   
    bool                    NavDisableHighlight;                
    bool                    NavDisableMouseHover;               
    bool                    NavAnyRequest;                      
    bool                    NavInitRequest;                     
    bool                    NavInitRequestFromMove;
    ImGuiID                 NavInitResultId;
    ImRect                  NavInitResultRectRel;
    bool                    NavMoveFromClampedRefRect;          
    bool                    NavMoveRequest;                     
    ImGuiNavMoveFlags       NavMoveRequestFlags;
    ImGuiNavForward         NavMoveRequestForward;              
    ImGuiDir                NavMoveDir, NavMoveDirLast;         
    ImGuiDir                NavMoveClipDir;                     
    ImGuiNavMoveResult      NavMoveResultLocal;                 
    ImGuiNavMoveResult      NavMoveResultLocalVisibleSet;       
    ImGuiNavMoveResult      NavMoveResultOther;                 

    ImGuiWindow*            FocusRequestCurrWindow;             
    ImGuiWindow*            FocusRequestNextWindow;             
    int                     FocusRequestCurrCounterAll;         
    int                     FocusRequestCurrCounterTab;         
    int                     FocusRequestNextCounterAll;         
    int                     FocusRequestNextCounterTab;         
    bool                    FocusTabPressed;                    

    ImDrawData              DrawData;                           
    ImDrawDataBuilder       DrawDataBuilder;
    float                   DimBgRatio;                         
    ImDrawList              BackgroundDrawList;                 
    ImDrawList              ForegroundDrawList;                 
    ImGuiMouseCursor        MouseCursor;

    bool                    DragDropActive;
    bool                    DragDropWithinSourceOrTarget;
    ImGuiDragDropFlags      DragDropSourceFlags;
    int                     DragDropSourceFrameCount;
    int                     DragDropMouseButton;
    ImGuiPayload            DragDropPayload;
    ImRect                  DragDropTargetRect;
    ImGuiID                 DragDropTargetId;
    ImGuiDragDropFlags      DragDropAcceptFlags;
    float                   DragDropAcceptIdCurrRectSurface;    
    ImGuiID                 DragDropAcceptIdCurr;               
    ImGuiID                 DragDropAcceptIdPrev;               
    int                     DragDropAcceptFrameCount;           
    ImVector<unsigned char> DragDropPayloadBufHeap;             
    unsigned char           DragDropPayloadBufLocal[16];        

    ImGuiTabBar*                    CurrentTabBar;
    ImPool<ImGuiTabBar>             TabBars;
    ImVector<ImGuiPtrOrIndex>       CurrentTabBarStack;
    ImVector<ImGuiShrinkWidthItem>  ShrinkWidthBuffer;

    ImVec2                  LastValidMousePos;
    ImGuiInputTextState     InputTextState;
    ImFont                  InputTextPasswordFont;
    ImGuiID                 TempInputTextId;                    
    ImGuiColorEditFlags     ColorEditOptions;                   
    float                   ColorEditLastHue;                   
    float                   ColorEditLastColor[3];
    ImVec4                  ColorPickerRef;                     
    bool                    DragCurrentAccumDirty;
    float                   DragCurrentAccum;                   
    float                   DragSpeedDefaultRatio;              
    float                   ScrollbarClickDeltaToGrabCenter;    
    int                     TooltipOverrideCount;
    ImVector<char>          PrivateClipboard;                   

    ImGuiID                 MultiSelectScopeId;

    ImVec2                  PlatformImePos;                     
    ImVec2                  PlatformImeLastPos;

    bool                    SettingsLoaded;
    float                   SettingsDirtyTimer;                 
    ImGuiTextBuffer         SettingsIniData;                    
    ImVector<ImGuiSettingsHandler>      SettingsHandlers;       
    ImChunkStream<ImGuiWindowSettings>  SettingsWindows;        

    bool                    LogEnabled;
    ImGuiLogType            LogType;
    ImFileHandle            LogFile;                            
    ImGuiTextBuffer         LogBuffer;                          
    float                   LogLinePosY;
    bool                    LogLineFirstItem;
    int                     LogDepthRef;
    int                     LogDepthToExpand;
    int                     LogDepthToExpandDefault;            

    bool                    DebugItemPickerActive;
    ImGuiID                 DebugItemPickerBreakID;             

    float                   FramerateSecPerFrame[120];          
    int                     FramerateSecPerFrameIdx;
    float                   FramerateSecPerFrameAccum;
    int                     WantCaptureMouseNextFrame;          
    int                     WantCaptureKeyboardNextFrame;
    int                     WantTextInputNextFrame;
    char                    TempBuffer[1024*3+1];               

    ImGuiContext(ImFontAtlas* shared_font_atlas) : BackgroundDrawList(&DrawListSharedData), ForegroundDrawList(&DrawListSharedData)
    {
        Initialized = false;
        Font = NULL;
        FontSize = FontBaseSize = 0.0f;
        FontAtlasOwnedByContext = shared_font_atlas ? false : true;
        IO.Fonts = shared_font_atlas ? shared_font_atlas : IM_NEW(ImFontAtlas)();
        Time = 0.0f;
        FrameCount = 0;
        FrameCountEnded = FrameCountRendered = -1;
        WithinFrameScope = WithinFrameScopeWithImplicitWindow = WithinEndChild = false;

        WindowsActiveCount = 0;
        CurrentWindow = NULL;
        HoveredWindow = NULL;
        HoveredRootWindow = NULL;
        MovingWindow = NULL;
        WheelingWindow = NULL;
        WheelingWindowTimer = 0.0f;

        HoveredId = 0;
        HoveredIdAllowOverlap = false;
        HoveredIdPreviousFrame = 0;
        HoveredIdTimer = HoveredIdNotActiveTimer = 0.0f;
        ActiveId = 0;
        ActiveIdIsAlive = 0;
        ActiveIdTimer = 0.0f;
        ActiveIdIsJustActivated = false;
        ActiveIdAllowOverlap = false;
        ActiveIdHasBeenPressedBefore = false;
        ActiveIdHasBeenEditedBefore = false;
        ActiveIdHasBeenEditedThisFrame = false;
        ActiveIdUsingNavDirMask = 0x00;
        ActiveIdUsingNavInputMask = 0x00;
        ActiveIdUsingKeyInputMask = 0x00;
        ActiveIdClickOffset = ImVec2(-1,-1);
        ActiveIdWindow = NULL;
        ActiveIdSource = ImGuiInputSource_None;
        ActiveIdPreviousFrame = 0;
        ActiveIdPreviousFrameIsAlive = false;
        ActiveIdPreviousFrameHasBeenEditedBefore = false;
        ActiveIdPreviousFrameWindow = NULL;
        LastActiveId = 0;
        LastActiveIdTimer = 0.0f;

        NavWindow = NULL;
        NavId = NavActivateId = NavActivateDownId = NavActivatePressedId = NavInputId = 0;
        NavJustTabbedId = NavJustMovedToId = NavJustMovedToMultiSelectScopeId = NavNextActivateId = 0;
        NavInputSource = ImGuiInputSource_None;
        NavScoringRectScreen = ImRect();
        NavScoringCount = 0;
        NavWindowingTarget = NavWindowingTargetAnim = NavWindowingList = NULL;
        NavWindowingTimer = NavWindowingHighlightAlpha = 0.0f;
        NavWindowingToggleLayer = false;
        NavLayer = ImGuiNavLayer_Main;
        NavIdTabCounter = INT_MAX;
        NavIdIsAlive = false;
        NavMousePosDirty = false;
        NavDisableHighlight = true;
        NavDisableMouseHover = false;
        NavAnyRequest = false;
        NavInitRequest = false;
        NavInitRequestFromMove = false;
        NavInitResultId = 0;
        NavMoveFromClampedRefRect = false;
        NavMoveRequest = false;
        NavMoveRequestFlags = 0;
        NavMoveRequestForward = ImGuiNavForward_None;
        NavMoveDir = NavMoveDirLast = NavMoveClipDir = ImGuiDir_None;

        FocusRequestCurrWindow = FocusRequestNextWindow = NULL;
        FocusRequestCurrCounterAll = FocusRequestCurrCounterTab = INT_MAX;
        FocusRequestNextCounterAll = FocusRequestNextCounterTab = INT_MAX;
        FocusTabPressed = false;

        DimBgRatio = 0.0f;
        BackgroundDrawList._OwnerName = "##Background"; 
        ForegroundDrawList._OwnerName = "##Foreground"; 
        MouseCursor = ImGuiMouseCursor_Arrow;

        DragDropActive = DragDropWithinSourceOrTarget = false;
        DragDropSourceFlags = 0;
        DragDropSourceFrameCount = -1;
        DragDropMouseButton = -1;
        DragDropTargetId = 0;
        DragDropAcceptFlags = 0;
        DragDropAcceptIdCurrRectSurface = 0.0f;
        DragDropAcceptIdPrev = DragDropAcceptIdCurr = 0;
        DragDropAcceptFrameCount = -1;
        memset(DragDropPayloadBufLocal, 0, sizeof(DragDropPayloadBufLocal));

        CurrentTabBar = NULL;

        LastValidMousePos = ImVec2(0.0f, 0.0f);
        TempInputTextId = 0;
        ColorEditOptions = ImGuiColorEditFlags__OptionsDefault;
        ColorEditLastHue = 0.0f;
        ColorEditLastColor[0] = ColorEditLastColor[1] = ColorEditLastColor[2] = FLT_MAX;
        DragCurrentAccumDirty = false;
        DragCurrentAccum = 0.0f;
        DragSpeedDefaultRatio = 1.0f / 100.0f;
        ScrollbarClickDeltaToGrabCenter = 0.0f;
        TooltipOverrideCount = 0;

        MultiSelectScopeId = 0;

        PlatformImePos = PlatformImeLastPos = ImVec2(FLT_MAX, FLT_MAX);

        SettingsLoaded = false;
        SettingsDirtyTimer = 0.0f;

        LogEnabled = false;
        LogType = ImGuiLogType_None;
        LogFile = NULL;
        LogLinePosY = FLT_MAX;
        LogLineFirstItem = false;
        LogDepthRef = 0;
        LogDepthToExpand = LogDepthToExpandDefault = 2;

        DebugItemPickerActive = false;
        DebugItemPickerBreakID = 0;

        memset(FramerateSecPerFrame, 0, sizeof(FramerateSecPerFrame));
        FramerateSecPerFrameIdx = 0;
        FramerateSecPerFrameAccum = 0.0f;
        WantCaptureMouseNextFrame = WantCaptureKeyboardNextFrame = WantTextInputNextFrame = -1;
        memset(TempBuffer, 0, sizeof(TempBuffer));
    }
};

struct IMGUI_API ImGuiWindowTempData
{
    ImVec2                  CursorPos;              
    ImVec2                  CursorPosPrevLine;
    ImVec2                  CursorStartPos;         
    ImVec2                  CursorMaxPos;           
    ImVec2                  CurrLineSize;
    ImVec2                  PrevLineSize;
    float                   CurrLineTextBaseOffset; 
    float                   PrevLineTextBaseOffset;
    int                     TreeDepth;                      
    ImU32                   TreeMayJumpToParentOnPopMask;   
    ImGuiID                 LastItemId;             
    ImGuiItemStatusFlags    LastItemStatusFlags;    
    ImRect                  LastItemRect;           
    ImRect                  LastItemDisplayRect;    
    ImGuiNavLayer           NavLayerCurrent;        
    int                     NavLayerCurrentMask;    
    int                     NavLayerActiveMask;     
    int                     NavLayerActiveMaskNext; 
    bool                    NavHideHighlightOneFrame;
    bool                    NavHasScroll;           
    bool                    MenuBarAppending;       
    ImVec2                  MenuBarOffset;          
    ImVector<ImGuiWindow*>  ChildWindows;
    ImGuiStorage*           StateStorage;           
    ImGuiLayoutType         LayoutType;
    ImGuiLayoutType         ParentLayoutType;       
    int                     FocusCounterAll;        
    int                     FocusCounterTab;        

    ImGuiItemFlags          ItemFlags;              
    float                   ItemWidth;              
    float                   TextWrapPos;            
    ImVector<ImGuiItemFlags>ItemFlagsStack;
    ImVector<float>         ItemWidthStack;
    ImVector<float>         TextWrapPosStack;
    ImVector<ImGuiGroupData>GroupStack;
    short                   StackSizesBackup[6];    

    ImVec1                  Indent;                 
    ImVec1                  GroupOffset;
    ImVec1                  ColumnsOffset;          
    ImGuiColumns*           CurrentColumns;         

    ImGuiWindowTempData()
    {
        CursorPos = CursorPosPrevLine = CursorStartPos = CursorMaxPos = ImVec2(0.0f, 0.0f);
        CurrLineSize = PrevLineSize = ImVec2(0.0f, 0.0f);
        CurrLineTextBaseOffset = PrevLineTextBaseOffset = 0.0f;
        TreeDepth = 0;
        TreeMayJumpToParentOnPopMask = 0x00;
        LastItemId = 0;
        LastItemStatusFlags = 0;
        LastItemRect = LastItemDisplayRect = ImRect();
        NavLayerActiveMask = NavLayerActiveMaskNext = 0x00;
        NavLayerCurrent = ImGuiNavLayer_Main;
        NavLayerCurrentMask = (1 << ImGuiNavLayer_Main);
        NavHideHighlightOneFrame = false;
        NavHasScroll = false;
        MenuBarAppending = false;
        MenuBarOffset = ImVec2(0.0f, 0.0f);
        StateStorage = NULL;
        LayoutType = ParentLayoutType = ImGuiLayoutType_Vertical;
        FocusCounterAll = FocusCounterTab = -1;

        ItemFlags = ImGuiItemFlags_Default_;
        ItemWidth = 0.0f;
        TextWrapPos = -1.0f;
        memset(StackSizesBackup, 0, sizeof(StackSizesBackup));

        Indent = ImVec1(0.0f);
        GroupOffset = ImVec1(0.0f);
        ColumnsOffset = ImVec1(0.0f);
        CurrentColumns = NULL;
    }
};

struct IMGUI_API ImGuiWindow
{
    char*                   Name;
    ImGuiID                 ID;                                 
    ImGuiWindowFlags        Flags;                              
    ImVec2                  Pos;                                
    ImVec2                  Size;                               
    ImVec2                  SizeFull;                           
    ImVec2                  ContentSize;                        
    ImVec2                  ContentSizeExplicit;                
    ImVec2                  WindowPadding;                      
    float                   WindowRounding;                     
    float                   WindowBorderSize;                   
    int                     NameBufLen;                         
    ImGuiID                 MoveId;                             
    ImGuiID                 ChildId;                            
    ImVec2                  Scroll;
    ImVec2                  ScrollMax;
    ImVec2                  ScrollTarget;                       
    ImVec2                  ScrollTargetCenterRatio;            
    ImVec2                  ScrollbarSizes;                     
    bool                    ScrollbarX, ScrollbarY;             
    bool                    Active;                             
    bool                    WasActive;
    bool                    WriteAccessed;                      
    bool                    Collapsed;                          
    bool                    WantCollapseToggle;
    bool                    SkipItems;                          
    bool                    Appearing;                          
    bool                    Hidden;                             
    bool                    HasCloseButton;                     
    signed char             ResizeBorderHeld;                   
    short                   BeginCount;                         
    short                   BeginOrderWithinParent;             
    short                   BeginOrderWithinContext;            
    ImGuiID                 PopupId;                            
    ImS8                    AutoFitFramesX, AutoFitFramesY;
    ImS8                    AutoFitChildAxises;
    bool                    AutoFitOnlyGrows;
    ImGuiDir                AutoPosLastDirection;
    int                     HiddenFramesCanSkipItems;           
    int                     HiddenFramesCannotSkipItems;        
    ImGuiCond               SetWindowPosAllowFlags;             
    ImGuiCond               SetWindowSizeAllowFlags;            
    ImGuiCond               SetWindowCollapsedAllowFlags;       
    ImVec2                  SetWindowPosVal;                    
    ImVec2                  SetWindowPosPivot;                  

    ImVector<ImGuiID>       IDStack;                            
    ImGuiWindowTempData     DC;                                 

    ImRect                  OuterRectClipped;                   
    ImRect                  InnerRect;                          
    ImRect                  InnerClipRect;                      
    ImRect                  WorkRect;                           
    ImRect                  ClipRect;                           
    ImRect                  ContentRegionRect;                  

    int                     LastFrameActive;                    
    float                   LastTimeActive;                     
    float                   ItemWidthDefault;
    ImGuiMenuColumns        MenuColumns;                        
    ImGuiStorage            StateStorage;
    ImVector<ImGuiColumns>  ColumnsStorage;
    float                   FontWindowScale;                    
    int                     SettingsOffset;                     

    ImDrawList*             DrawList;                           
    ImDrawList              DrawListInst;
    ImGuiWindow*            ParentWindow;                       
    ImGuiWindow*            RootWindow;                         
    ImGuiWindow*            RootWindowForTitleBarHighlight;     
    ImGuiWindow*            RootWindowForNav;                   

    ImGuiWindow*            NavLastChildNavWindow;              
    ImGuiID                 NavLastIds[ImGuiNavLayer_COUNT];    
    ImRect                  NavRectRel[ImGuiNavLayer_COUNT];    

    bool                    MemoryCompacted;
    int                     MemoryDrawListIdxCapacity;
    int                     MemoryDrawListVtxCapacity;

public:
    ImGuiWindow(ImGuiContext* context, const char* name);
    ~ImGuiWindow();

    ImGuiID     GetID(const char* str, const char* str_end = NULL);
    ImGuiID     GetID(const void* ptr);
    ImGuiID     GetID(int n);
    ImGuiID     GetIDNoKeepAlive(const char* str, const char* str_end = NULL);
    ImGuiID     GetIDNoKeepAlive(const void* ptr);
    ImGuiID     GetIDNoKeepAlive(int n);
    ImGuiID     GetIDFromRectangle(const ImRect& r_abs);

    ImRect      Rect() const                { return ImRect(Pos.x, Pos.y, Pos.x+Size.x, Pos.y+Size.y); }
    float       CalcFontSize() const        { ImGuiContext& g = *GImGui; float scale = g.FontBaseSize * FontWindowScale; if (ParentWindow) scale *= ParentWindow->FontWindowScale; return scale; }
    float       TitleBarHeight() const      { ImGuiContext& g = *GImGui; return (Flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : CalcFontSize() + g.Style.FramePadding.y * 2.0f; }
    ImRect      TitleBarRect() const        { return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight())); }
    float       MenuBarHeight() const       { ImGuiContext& g = *GImGui; return (Flags & ImGuiWindowFlags_MenuBar) ? DC.MenuBarOffset.y + CalcFontSize() + g.Style.FramePadding.y * 2.0f : 0.0f; }
    ImRect      MenuBarRect() const         { float y1 = Pos.y + TitleBarHeight(); return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight()); }
};

struct ImGuiItemHoveredDataBackup
{
    ImGuiID                 LastItemId;
    ImGuiItemStatusFlags    LastItemStatusFlags;
    ImRect                  LastItemRect;
    ImRect                  LastItemDisplayRect;

    ImGuiItemHoveredDataBackup() { Backup(); }
    void Backup()           { ImGuiWindow* window = GImGui->CurrentWindow; LastItemId = window->DC.LastItemId; LastItemStatusFlags = window->DC.LastItemStatusFlags; LastItemRect = window->DC.LastItemRect; LastItemDisplayRect = window->DC.LastItemDisplayRect; }
    void Restore() const    { ImGuiWindow* window = GImGui->CurrentWindow; window->DC.LastItemId = LastItemId; window->DC.LastItemStatusFlags = LastItemStatusFlags; window->DC.LastItemRect = LastItemRect; window->DC.LastItemDisplayRect = LastItemDisplayRect; }
};

enum ImGuiTabBarFlagsPrivate_
{
    ImGuiTabBarFlags_DockNode                   = 1 << 20,  
    ImGuiTabBarFlags_IsFocused                  = 1 << 21,
    ImGuiTabBarFlags_SaveSettings               = 1 << 22   
};

enum ImGuiTabItemFlagsPrivate_
{
    ImGuiTabItemFlags_NoCloseButton             = 1 << 20   
};

struct ImGuiTabItem
{
    ImGuiID             ID;
    ImGuiTabItemFlags   Flags;
    int                 LastFrameVisible;
    int                 LastFrameSelected;      
    int                 NameOffset;             
    float               Offset;                 
    float               Width;                  
    float               ContentWidth;           

    ImGuiTabItem()      { ID = 0; Flags = 0; LastFrameVisible = LastFrameSelected = -1; NameOffset = -1; Offset = Width = ContentWidth = 0.0f; }
};

struct ImGuiTabBar
{
    ImVector<ImGuiTabItem> Tabs;
    ImGuiID             ID;                     
    ImGuiID             SelectedTabId;          
    ImGuiID             NextSelectedTabId;
    ImGuiID             VisibleTabId;           
    int                 CurrFrameVisible;
    int                 PrevFrameVisible;
    ImRect              BarRect;
    float               LastTabContentHeight;   
    float               OffsetMax;              
    float               OffsetMaxIdeal;         
    float               OffsetNextTab;          
    float               ScrollingAnim;
    float               ScrollingTarget;
    float               ScrollingTargetDistToVisibility;
    float               ScrollingSpeed;
    ImGuiTabBarFlags    Flags;
    ImGuiID             ReorderRequestTabId;
    ImS8                ReorderRequestDir;
    bool                WantLayout;
    bool                VisibleTabWasSubmitted;
    short               LastTabItemIdx;         
    ImVec2              FramePadding;           
    ImGuiTextBuffer     TabsNames;              

    ImGuiTabBar();
    int                 GetTabOrder(const ImGuiTabItem* tab) const  { return Tabs.index_from_ptr(tab); }
    const char*         GetTabName(const ImGuiTabItem* tab) const
    {
        IM_ASSERT(tab->NameOffset != -1 && tab->NameOffset < TabsNames.Buf.Size);
        return TabsNames.Buf.Data + tab->NameOffset;
    }
};

namespace ImGui
{

    inline    ImGuiWindow*  GetCurrentWindowRead()      { ImGuiContext& g = *GImGui; return g.CurrentWindow; }
    inline    ImGuiWindow*  GetCurrentWindow()          { ImGuiContext& g = *GImGui; g.CurrentWindow->WriteAccessed = true; return g.CurrentWindow; }
    IMGUI_API ImGuiWindow*  FindWindowByID(ImGuiID id);
    IMGUI_API ImGuiWindow*  FindWindowByName(const char* name);
    IMGUI_API void          FocusWindow(ImGuiWindow* window);
    IMGUI_API void          FocusTopMostWindowUnderOne(ImGuiWindow* under_this_window, ImGuiWindow* ignore_window);
    IMGUI_API void          BringWindowToFocusFront(ImGuiWindow* window);
    IMGUI_API void          BringWindowToDisplayFront(ImGuiWindow* window);
    IMGUI_API void          BringWindowToDisplayBack(ImGuiWindow* window);
    IMGUI_API void          UpdateWindowParentAndRootLinks(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window);
    IMGUI_API ImVec2        CalcWindowExpectedSize(ImGuiWindow* window);
    IMGUI_API bool          IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent);
    IMGUI_API bool          IsWindowNavFocusable(ImGuiWindow* window);
    IMGUI_API ImRect        GetWindowAllowedExtentRect(ImGuiWindow* window);
    IMGUI_API void          SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond = 0);
    IMGUI_API void          SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond = 0);
    IMGUI_API void          SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond = 0);
    IMGUI_API void          GcCompactTransientWindowBuffers(ImGuiWindow* window);
    IMGUI_API void          GcAwakeTransientWindowBuffers(ImGuiWindow* window);

    IMGUI_API void          SetCurrentFont(ImFont* font);
    inline ImFont*          GetDefaultFont() { ImGuiContext& g = *GImGui; return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0]; }
    inline ImDrawList*      GetForegroundDrawList(ImGuiWindow* window) { IM_UNUSED(window); ImGuiContext& g = *GImGui; return &g.ForegroundDrawList; } 

    IMGUI_API void          Initialize(ImGuiContext* context);
    IMGUI_API void          Shutdown(ImGuiContext* context);    

    IMGUI_API void          UpdateHoveredWindowAndCaptureFlags();
    IMGUI_API void          StartMouseMovingWindow(ImGuiWindow* window);
    IMGUI_API void          UpdateMouseMovingWindowNewFrame();
    IMGUI_API void          UpdateMouseMovingWindowEndFrame();

    IMGUI_API void                  MarkIniSettingsDirty();
    IMGUI_API void                  MarkIniSettingsDirty(ImGuiWindow* window);
    IMGUI_API ImGuiWindowSettings*  CreateNewWindowSettings(const char* name);
    IMGUI_API ImGuiWindowSettings*  FindWindowSettings(ImGuiID id);
    IMGUI_API ImGuiWindowSettings*  FindOrCreateWindowSettings(const char* name);
    IMGUI_API ImGuiSettingsHandler* FindSettingsHandler(const char* type_name);

    IMGUI_API void          SetScrollX(ImGuiWindow* window, float new_scroll_x);
    IMGUI_API void          SetScrollY(ImGuiWindow* window, float new_scroll_y);
    IMGUI_API void          SetScrollFromPosX(ImGuiWindow* window, float local_x, float center_x_ratio = 0.5f);
    IMGUI_API void          SetScrollFromPosY(ImGuiWindow* window, float local_y, float center_y_ratio = 0.5f);
    IMGUI_API ImVec2        ScrollToBringRectIntoView(ImGuiWindow* window, const ImRect& item_rect);

    inline ImGuiID          GetItemID()     { ImGuiContext& g = *GImGui; return g.CurrentWindow->DC.LastItemId; }
    inline ImGuiID          GetActiveID()   { ImGuiContext& g = *GImGui; return g.ActiveId; }
    inline ImGuiID          GetFocusID()    { ImGuiContext& g = *GImGui; return g.NavId; }
    IMGUI_API void          SetActiveID(ImGuiID id, ImGuiWindow* window);
    IMGUI_API void          SetFocusID(ImGuiID id, ImGuiWindow* window);
    IMGUI_API void          ClearActiveID();
    IMGUI_API ImGuiID       GetHoveredID();
    IMGUI_API void          SetHoveredID(ImGuiID id);
    IMGUI_API void          KeepAliveID(ImGuiID id);
    IMGUI_API void          MarkItemEdited(ImGuiID id);
    IMGUI_API void          PushOverrideID(ImGuiID id);

    IMGUI_API void          ItemSize(const ImVec2& size, float text_baseline_y = -1.0f);
    IMGUI_API void          ItemSize(const ImRect& bb, float text_baseline_y = -1.0f);
    IMGUI_API bool          ItemAdd(const ImRect& bb, ImGuiID id, const ImRect* nav_bb = NULL);
    IMGUI_API bool          ItemHoverable(const ImRect& bb, ImGuiID id);
    IMGUI_API bool          IsClippedEx(const ImRect& bb, ImGuiID id, bool clip_even_when_logged);
    IMGUI_API bool          FocusableItemRegister(ImGuiWindow* window, ImGuiID id);   
    IMGUI_API void          FocusableItemUnregister(ImGuiWindow* window);
    IMGUI_API ImVec2        CalcItemSize(ImVec2 size, float default_w, float default_h);
    IMGUI_API float         CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x);
    IMGUI_API void          PushMultiItemsWidths(int components, float width_full);
    IMGUI_API void          PushItemFlag(ImGuiItemFlags option, bool enabled);
    IMGUI_API void          PopItemFlag();
    IMGUI_API bool          IsItemToggledSelection();                           
    IMGUI_API ImVec2        GetContentRegionMaxAbs();
    IMGUI_API void          ShrinkWidths(ImGuiShrinkWidthItem* items, int count, float width_excess);

    IMGUI_API void          LogBegin(ImGuiLogType type, int auto_open_depth);   
    IMGUI_API void          LogToBuffer(int auto_open_depth = -1);              

    IMGUI_API void          OpenPopupEx(ImGuiID id);
    IMGUI_API void          ClosePopupToLevel(int remaining, bool restore_focus_to_window_under_popup);
    IMGUI_API void          ClosePopupsOverWindow(ImGuiWindow* ref_window, bool restore_focus_to_window_under_popup);
    IMGUI_API bool          IsPopupOpen(ImGuiID id); 
    IMGUI_API bool          BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags);
    IMGUI_API void          BeginTooltipEx(ImGuiWindowFlags extra_flags, bool override_previous_tooltip = true);
    IMGUI_API ImGuiWindow*  GetTopMostPopupModal();
    IMGUI_API ImVec2        FindBestWindowPosForPopup(ImGuiWindow* window);
    IMGUI_API ImVec2        FindBestWindowPosForPopupEx(const ImVec2& ref_pos, const ImVec2& size, ImGuiDir* last_dir, const ImRect& r_outer, const ImRect& r_avoid, ImGuiPopupPositionPolicy policy = ImGuiPopupPositionPolicy_Default);

    IMGUI_API void          NavInitWindow(ImGuiWindow* window, bool force_reinit);
    IMGUI_API bool          NavMoveRequestButNoResultYet();
    IMGUI_API void          NavMoveRequestCancel();
    IMGUI_API void          NavMoveRequestForward(ImGuiDir move_dir, ImGuiDir clip_dir, const ImRect& bb_rel, ImGuiNavMoveFlags move_flags);
    IMGUI_API void          NavMoveRequestTryWrapping(ImGuiWindow* window, ImGuiNavMoveFlags move_flags);
    IMGUI_API float         GetNavInputAmount(ImGuiNavInput n, ImGuiInputReadMode mode);
    IMGUI_API ImVec2        GetNavInputAmount2d(ImGuiNavDirSourceFlags dir_sources, ImGuiInputReadMode mode, float slow_factor = 0.0f, float fast_factor = 0.0f);
    IMGUI_API int           CalcTypematicRepeatAmount(float t0, float t1, float repeat_delay, float repeat_rate);
    IMGUI_API void          ActivateItem(ImGuiID id);   
    IMGUI_API void          SetNavID(ImGuiID id, int nav_layer);
    IMGUI_API void          SetNavIDWithRectRel(ImGuiID id, int nav_layer, const ImRect& rect_rel);

    inline bool             IsActiveIdUsingNavDir(ImGuiDir dir)                         { ImGuiContext& g = *GImGui; return (g.ActiveIdUsingNavDirMask & (1 << dir)) != 0; }
    inline bool             IsActiveIdUsingNavInput(ImGuiNavInput input)                { ImGuiContext& g = *GImGui; return (g.ActiveIdUsingNavInputMask & (1 << input)) != 0; }
    inline bool             IsActiveIdUsingKey(ImGuiKey key)                            { ImGuiContext& g = *GImGui; IM_ASSERT(key < 64); return (g.ActiveIdUsingKeyInputMask & ((ImU64)1 << key)) != 0; }
    IMGUI_API bool          IsMouseDragPastThreshold(ImGuiMouseButton button, float lock_threshold = -1.0f);
    inline bool             IsKeyPressedMap(ImGuiKey key, bool repeat = true)           { ImGuiContext& g = *GImGui; const int key_index = g.IO.KeyMap[key]; return (key_index >= 0) ? IsKeyPressed(key_index, repeat) : false; }
    inline bool             IsNavInputDown(ImGuiNavInput n)                             { ImGuiContext& g = *GImGui; return g.IO.NavInputs[n] > 0.0f; }
    inline bool             IsNavInputTest(ImGuiNavInput n, ImGuiInputReadMode rm)      { return (GetNavInputAmount(n, rm) > 0.0f); }

    IMGUI_API bool          BeginDragDropTargetCustom(const ImRect& bb, ImGuiID id);
    IMGUI_API void          ClearDragDrop();
    IMGUI_API bool          IsDragDropPayloadBeingAccepted();

    IMGUI_API void          BeginColumns(const char* str_id, int count, ImGuiColumnsFlags flags = 0); 
    IMGUI_API void          EndColumns();                                                             
    IMGUI_API void          PushColumnClipRect(int column_index);
    IMGUI_API void          PushColumnsBackground();
    IMGUI_API void          PopColumnsBackground();
    IMGUI_API ImGuiID       GetColumnsID(const char* str_id, int count);
    IMGUI_API ImGuiColumns* FindOrCreateColumns(ImGuiWindow* window, ImGuiID id);
    IMGUI_API float         GetColumnOffsetFromNorm(const ImGuiColumns* columns, float offset_norm);
    IMGUI_API float         GetColumnNormFromOffset(const ImGuiColumns* columns, float offset);

    IMGUI_API bool          BeginTabBarEx(ImGuiTabBar* tab_bar, const ImRect& bb, ImGuiTabBarFlags flags);
    IMGUI_API ImGuiTabItem* TabBarFindTabByID(ImGuiTabBar* tab_bar, ImGuiID tab_id);
    IMGUI_API void          TabBarRemoveTab(ImGuiTabBar* tab_bar, ImGuiID tab_id);
    IMGUI_API void          TabBarCloseTab(ImGuiTabBar* tab_bar, ImGuiTabItem* tab);
    IMGUI_API void          TabBarQueueChangeTabOrder(ImGuiTabBar* tab_bar, const ImGuiTabItem* tab, int dir);
    IMGUI_API bool          TabItemEx(ImGuiTabBar* tab_bar, const char* label, bool* p_open, ImGuiTabItemFlags flags);
    IMGUI_API ImVec2        TabItemCalcSize(const char* label, bool has_close_button);
    IMGUI_API void          TabItemBackground(ImDrawList* draw_list, const ImRect& bb, ImGuiTabItemFlags flags, ImU32 col);
    IMGUI_API bool          TabItemLabelAndCloseButton(ImDrawList* draw_list, const ImRect& bb, ImGuiTabItemFlags flags, ImVec2 frame_padding, const char* label, ImGuiID tab_id, ImGuiID close_button_id);

    IMGUI_API void          RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
    IMGUI_API void          RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
    IMGUI_API void          RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0,0), const ImRect* clip_rect = NULL);
    IMGUI_API void          RenderTextClippedEx(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);
    IMGUI_API void          RenderTextEllipsis(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, float clip_max_x, float ellipsis_max_x, const char* text, const char* text_end, const ImVec2* text_size_if_known);
    IMGUI_API void          RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border = true, float rounding = 0.0f);
    IMGUI_API void          RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f);
    IMGUI_API void          RenderColorRectWithAlphaCheckerboard(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float grid_step, ImVec2 grid_off, float rounding = 0.0f, int rounding_corners_flags = ~0);
    IMGUI_API void          RenderCheckMark(ImVec2 pos, ImU32 col, float sz);
    IMGUI_API void          RenderNavHighlight(const ImRect& bb, ImGuiID id, ImGuiNavHighlightFlags flags = ImGuiNavHighlightFlags_TypeDefault); 
    IMGUI_API const char*   FindRenderedTextEnd(const char* text, const char* text_end = NULL); 
    IMGUI_API void          LogRenderedText(const ImVec2* ref_pos, const char* text, const char* text_end = NULL);

    IMGUI_API void          RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale = 1.0f);
    IMGUI_API void          RenderBullet(ImDrawList* draw_list, ImVec2 pos, ImU32 col);
    IMGUI_API void          RenderMouseCursor(ImDrawList* draw_list, ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor, ImU32 col_fill, ImU32 col_border, ImU32 col_shadow);
    IMGUI_API void          RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col);
    IMGUI_API void          RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect& rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding);

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS

    inline void RenderArrow(ImVec2 pos, ImGuiDir dir, float scale=1.0f) { ImGuiWindow* window = GetCurrentWindow(); RenderArrow(window->DrawList, pos, GetColorU32(ImGuiCol_Text), dir, scale); }
    inline void RenderBullet(ImVec2 pos)                                { ImGuiWindow* window = GetCurrentWindow(); RenderBullet(window->DrawList, pos, GetColorU32(ImGuiCol_Text)); }
#endif

    IMGUI_API void          TextEx(const char* text, const char* text_end = NULL, ImGuiTextFlags flags = 0);
    IMGUI_API bool          ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0,0), ImGuiButtonFlags flags = 0);
    IMGUI_API bool          CloseButton(ImGuiID id, const ImVec2& pos);
    IMGUI_API bool          CollapseButton(ImGuiID id, const ImVec2& pos);
    IMGUI_API bool          ArrowButtonEx(const char* str_id, ImGuiDir dir, ImVec2 size_arg, ImGuiButtonFlags flags);
    IMGUI_API void          Scrollbar(ImGuiAxis axis);
    IMGUI_API bool          ScrollbarEx(const ImRect& bb, ImGuiID id, ImGuiAxis axis, float* p_scroll_v, float avail_v, float contents_v, ImDrawCornerFlags rounding_corners);
    IMGUI_API ImGuiID       GetWindowScrollbarID(ImGuiWindow* window, ImGuiAxis axis);
    IMGUI_API ImGuiID       GetWindowResizeID(ImGuiWindow* window, int n); 
    IMGUI_API void          SeparatorEx(ImGuiSeparatorFlags flags);

    IMGUI_API bool          ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags = 0);
    IMGUI_API bool          DragBehavior(ImGuiID id, ImGuiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, float power, ImGuiDragFlags flags);
    IMGUI_API bool          SliderBehavior(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, float power, ImGuiSliderFlags flags, ImRect* out_grab_bb);
    IMGUI_API bool          SplitterBehavior(const ImRect& bb, ImGuiID id, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f);
    IMGUI_API bool          TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL);
    IMGUI_API bool          TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags = 0);                     
    IMGUI_API void          TreePushOverrideID(ImGuiID id);

    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API bool  DragBehaviorT(ImGuiDataType data_type, T* v, float v_speed, T v_min, T v_max, const char* format, float power, ImGuiDragFlags flags);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API bool  SliderBehaviorT(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, T* v, T v_min, T v_max, const char* format, float power, ImGuiSliderFlags flags, ImRect* out_grab_bb);
    template<typename T, typename FLOAT_T>                      IMGUI_API float SliderCalcRatioFromValueT(ImGuiDataType data_type, T v, T v_min, T v_max, float power, float linear_zero_pos);
    template<typename T, typename SIGNED_T>                     IMGUI_API T     RoundScalarWithFormatT(const char* format, ImGuiDataType data_type, T v);

    IMGUI_API const ImGuiDataTypeInfo*  DataTypeGetInfo(ImGuiDataType data_type);
    IMGUI_API int           DataTypeFormatString(char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format);
    IMGUI_API void          DataTypeApplyOp(ImGuiDataType data_type, int op, void* output, void* arg_1, const void* arg_2);
    IMGUI_API bool          DataTypeApplyOpFromText(const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* p_data, const char* format);

    IMGUI_API bool          InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          TempInputTextScalar(const ImRect& bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* p_data, const char* format);
    inline bool             TempInputTextIsActive(ImGuiID id) { ImGuiContext& g = *GImGui; return (g.ActiveId == id && g.TempInputTextId == id); }

    IMGUI_API void          ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags);
    IMGUI_API void          ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags);
    IMGUI_API void          ColorPickerOptionsPopup(const float* ref_col, ImGuiColorEditFlags flags);

    IMGUI_API void          PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 frame_size);

    IMGUI_API void          ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1);
    IMGUI_API void          ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp);

    inline void             DebugDrawItemRect(ImU32 col = IM_COL32(255,0,0,255))    { ImGuiContext& g = *GImGui; ImGuiWindow* window = g.CurrentWindow; GetForegroundDrawList(window)->AddRect(window->DC.LastItemRect.Min, window->DC.LastItemRect.Max, col); }
    inline void             DebugStartItemPicker()                                  { ImGuiContext& g = *GImGui; g.DebugItemPickerActive = true; }

} 

IMGUI_API bool              ImFontAtlasBuildWithStbTruetype(ImFontAtlas* atlas);
IMGUI_API void              ImFontAtlasBuildRegisterDefaultCustomRects(ImFontAtlas* atlas);
IMGUI_API void              ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent);
IMGUI_API void              ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* stbrp_context_opaque);
IMGUI_API void              ImFontAtlasBuildFinish(ImFontAtlas* atlas);
IMGUI_API void              ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_multiply_factor);
IMGUI_API void              ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride);

#ifndef IM_DEBUG_BREAK
#if defined(__clang__)
#define IM_DEBUG_BREAK()    __builtin_debugtrap()
#elif defined (_MSC_VER)
#define IM_DEBUG_BREAK()    __debugbreak()
#else
#define IM_DEBUG_BREAK()    IM_ASSERT(0)    
#endif
#endif 

#ifdef IMGUI_ENABLE_TEST_ENGINE
extern void                 ImGuiTestEngineHook_PreNewFrame(ImGuiContext* ctx);
extern void                 ImGuiTestEngineHook_PostNewFrame(ImGuiContext* ctx);
extern void                 ImGuiTestEngineHook_ItemAdd(ImGuiContext* ctx, const ImRect& bb, ImGuiID id);
extern void                 ImGuiTestEngineHook_ItemInfo(ImGuiContext* ctx, ImGuiID id, const char* label, ImGuiItemStatusFlags flags);
extern void                 ImGuiTestEngineHook_Log(ImGuiContext* ctx, const char* fmt, ...);
#define IMGUI_TEST_ENGINE_ITEM_ADD(_BB, _ID)                ImGuiTestEngineHook_ItemAdd(&g, _BB, _ID)               
#define IMGUI_TEST_ENGINE_ITEM_INFO(_ID, _LABEL, _FLAGS)    ImGuiTestEngineHook_ItemInfo(&g, _ID, _LABEL, _FLAGS)   
#define IMGUI_TEST_ENGINE_LOG(_FMT, ...)                    ImGuiTestEngineHook_Log(&g, _FMT, __VA_ARGS__)          
#else
#define IMGUI_TEST_ENGINE_ITEM_ADD(_BB, _ID)                do { } while (0)
#define IMGUI_TEST_ENGINE_ITEM_INFO(_ID, _LABEL, _FLAGS)    do { } while (0)
#define IMGUI_TEST_ENGINE_LOG(_FMT, ...)                    do { } while (0)
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif