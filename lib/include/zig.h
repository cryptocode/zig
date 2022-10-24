#undef linux

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__has_builtin)
#define zig_has_builtin(builtin) __has_builtin(__builtin_##builtin)
#else
#define zig_has_builtin(builtin) 0
#endif

#if defined(__has_attribute)
#define zig_has_attribute(attribute) __has_attribute(attribute)
#else
#define zig_has_attribute(attribute) 0
#endif

#if __STDC_VERSION__ >= 201112L
#define zig_threadlocal thread_local
#elif __GNUC__
#define zig_threadlocal __thread
#elif _MSC_VER
#define zig_threadlocal __declspec(thread)
#else
#define zig_threadlocal zig_threadlocal_unavailable
#endif

#if zig_has_attribute(naked)
#define zig_naked __attribute__((naked))
#elif defined(_MSC_VER)
#define zig_naked __declspec(naked)
#else
#define zig_naked zig_naked_unavailable
#endif

#if zig_has_attribute(cold)
#define zig_cold __attribute__((cold))
#else
#define zig_cold
#endif

#if __STDC_VERSION__ >= 199901L
#define zig_restrict restrict
#elif defined(__GNUC__)
#define zig_restrict __restrict
#else
#define zig_restrict
#endif

#if __STDC_VERSION__ >= 201112L
#define zig_align(alignment) _Alignas(alignment)
#elif zig_has_attribute(aligned)
#define zig_align(alignment) __attribute__((aligned(alignment)))
#elif _MSC_VER
#else
#error the C compiler being used does not support aligning variables
#endif

#if zig_has_builtin(unreachable)
#define zig_unreachable() __builtin_unreachable()
#else
#define zig_unreachable()
#endif

#if defined(__cplusplus)
#define zig_extern_c extern "C"
#else
#define zig_extern_c
#endif

#if zig_has_builtin(debugtrap)
#define zig_breakpoint() __builtin_debugtrap()
#elif zig_has_builtin(trap)
#define zig_breakpoint() __builtin_trap()
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#define zig_breakpoint() __debugbreak()
#elif defined(__i386__) || defined(__x86_64__)
#define zig_breakpoint() __asm__ volatile("int $0x03");
#else
#define zig_breakpoint() raise(SIGTRAP)
#endif

#if zig_has_builtin(return_address)
#define zig_return_address() __builtin_extract_return_addr(__builtin_return_address(0))
#elif defined(_MSC_VER)
#define zig_return_address() _ReturnAddress()
#else
#define zig_return_address() 0
#endif

#if zig_has_builtin(frame_address)
#define zig_frame_address() __builtin_frame_address(0)
#else
#define zig_frame_address() 0
#endif

#if zig_has_builtin(prefetch)
#define zig_prefetch(addr, rw, locality) __builtin_prefetch(addr, rw, locality)
#else
#define zig_prefetch(addr, rw, locality)
#endif

#if zig_has_builtin(memory_size) && zig_has_builtin(memory_grow)
#define zig_wasm_memory_size(index) __builtin_wasm_memory_size(index)
#define zig_wasm_memory_grow(index, delta) __builtin_wasm_memory_grow(index, delta)
#else
#define zig_wasm_memory_size(index) zig_unimplemented()
#define zig_wasm_memory_grow(index, delta) zig_unimplemented()
#endif

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#define zig_atomic(type) _Atomic(type)
#define zig_cmpxchg_strong(obj, expected, desired, succ, fail) atomic_compare_exchange_strong_explicit(obj, &(expected), desired, succ, fail)
#define   zig_cmpxchg_weak(obj, expected, desired, succ, fail) atomic_compare_exchange_weak_explicit  (obj, &(expected), desired, succ, fail)
#define zig_atomicrmw_xchg(obj, arg, order) atomic_exchange_explicit  (obj, arg, order)
#define  zig_atomicrmw_add(obj, arg, order) atomic_fetch_add_explicit (obj, arg, order)
#define  zig_atomicrmw_sub(obj, arg, order) atomic_fetch_sub_explicit (obj, arg, order)
#define   zig_atomicrmw_or(obj, arg, order) atomic_fetch_or_explicit  (obj, arg, order)
#define  zig_atomicrmw_xor(obj, arg, order) atomic_fetch_xor_explicit (obj, arg, order)
#define  zig_atomicrmw_and(obj, arg, order) atomic_fetch_and_explicit (obj, arg, order)
#define zig_atomicrmw_nand(obj, arg, order) __atomic_fetch_nand       (obj, arg, order)
#define  zig_atomicrmw_min(obj, arg, order) __atomic_fetch_min        (obj, arg, order)
#define  zig_atomicrmw_max(obj, arg, order) __atomic_fetch_max        (obj, arg, order)
#define   zig_atomic_store(obj, arg, order) atomic_store_explicit     (obj, arg, order)
#define    zig_atomic_load(obj,      order) atomic_load_explicit      (obj,      order)
#define zig_fence(order) atomic_thread_fence(order)
#elif __GNUC__
#define memory_order_relaxed __ATOMIC_RELAXED
#define memory_order_consume __ATOMIC_CONSUME
#define memory_order_acquire __ATOMIC_ACQUIRE
#define memory_order_release __ATOMIC_RELEASE
#define memory_order_acq_rel __ATOMIC_ACQ_REL
#define memory_order_seq_cst __ATOMIC_SEQ_CST
#define zig_atomic(type) type
#define zig_cmpxchg_strong(obj, expected, desired, succ, fail) __atomic_compare_exchange_n(obj, &(expected), desired, zig_false, succ, fail)
#define   zig_cmpxchg_weak(obj, expected, desired, succ, fail) __atomic_compare_exchange_n(obj, &(expected), desired, zig_true , succ, fail)
#define zig_atomicrmw_xchg(obj, arg, order) __atomic_exchange_n(obj, arg, order)
#define  zig_atomicrmw_add(obj, arg, order) __atomic_fetch_add (obj, arg, order)
#define  zig_atomicrmw_sub(obj, arg, order) __atomic_fetch_sub (obj, arg, order)
#define   zig_atomicrmw_or(obj, arg, order) __atomic_fetch_or  (obj, arg, order)
#define  zig_atomicrmw_xor(obj, arg, order) __atomic_fetch_xor (obj, arg, order)
#define  zig_atomicrmw_and(obj, arg, order) __atomic_fetch_and (obj, arg, order)
#define zig_atomicrmw_nand(obj, arg, order) __atomic_fetch_nand(obj, arg, order)
#define  zig_atomicrmw_min(obj, arg, order) __atomic_fetch_min (obj, arg, order)
#define  zig_atomicrmw_max(obj, arg, order) __atomic_fetch_max (obj, arg, order)
#define   zig_atomic_store(obj, arg, order) __atomic_store_n   (obj, arg, order)
#define    zig_atomic_load(obj,      order) __atomic_load_n    (obj,      order)
#define zig_fence(order) __atomic_thread_fence(order)
#else
#define memory_order_relaxed 0
#define memory_order_consume 1
#define memory_order_acquire 2
#define memory_order_release 3
#define memory_order_acq_rel 4
#define memory_order_seq_cst 5
#define zig_atomic(type) type
#define zig_cmpxchg_strong(obj, expected, desired, succ, fail) zig_unimplemented()
#define   zig_cmpxchg_weak(obj, expected, desired, succ, fail) zig_unimplemented()
#define zig_atomicrmw_xchg(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_add(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_sub(obj, arg, order) zig_unimplemented()
#define   zig_atomicrmw_or(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_xor(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_and(obj, arg, order) zig_unimplemented()
#define zig_atomicrmw_nand(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_min(obj, arg, order) zig_unimplemented()
#define  zig_atomicrmw_max(obj, arg, order) zig_unimplemented()
#define   zig_atomic_store(obj, arg, order) zig_unimplemented()
#define    zig_atomic_load(obj,      order) zig_unimplemented()
#define zig_fence(order) zig_unimplemented()
#endif

#if __STDC_VERSION__ >= 201112L
#define zig_noreturn _Noreturn void
#define zig_threadlocal thread_local
#elif __GNUC__
#define zig_noreturn __attribute__ ((noreturn)) void
#define zig_threadlocal __thread
#elif _MSC_VER
#define zig_noreturn __declspec(noreturn) void
#define zig_threadlocal __declspec(thread)
#else
#define zig_noreturn void
#define zig_threadlocal zig_threadlocal_unavailable
#endif

#define zig_bitSizeOf(T) (CHAR_BIT * sizeof(T))

typedef void zig_void;

#if defined(__cplusplus)
typedef bool zig_bool;
#define zig_false false
#define zig_true true
#else
#if __STDC_VERSION__ >= 199901L
typedef _Bool zig_bool;
#else
typedef char zig_bool;
#endif
#define zig_false ((zig_bool)0)
#define zig_true  ((zig_bool)1)
#endif

typedef              uintptr_t zig_usize;
typedef               intptr_t zig_isize;
typedef   signed     short int zig_c_short;
typedef unsigned     short int zig_c_ushort;
typedef   signed           int zig_c_int;
typedef unsigned           int zig_c_uint;
typedef   signed      long int zig_c_long;
typedef unsigned      long int zig_c_ulong;
typedef   signed long long int zig_c_longlong;
typedef unsigned long long int zig_c_ulonglong;
typedef            long double zig_c_longdouble;

typedef uint8_t  zig_u8;
typedef  int8_t  zig_i8;
typedef uint16_t zig_u16;
typedef  int16_t zig_i16;
typedef uint16_t zig_u16;
typedef  int16_t zig_i16;
typedef uint32_t zig_u32;
typedef  int32_t zig_i32;
typedef uint64_t zig_u64;
typedef  int64_t zig_i64;

#define zig_as_u8(val)  UINT8_C(val)
#define zig_as_i8(val)   INT8_C(val)
#define zig_as_u16(val) UINT16_C(val)
#define zig_as_i16(val)  INT16_C(val)
#define zig_as_u32(val) UINT32_C(val)
#define zig_as_i32(val)  INT32_C(val)
#define zig_as_u64(val) UINT64_C(val)
#define zig_as_i64(val)  INT64_C(val)

#define zig_minInt_u8  zig_as_u8(0)
#define zig_maxInt_u8   UINT8_MAX
#define zig_minInt_i8    INT8_MIN
#define zig_maxInt_i8    INT8_MAX
#define zig_minInt_u16 zig_as_u16(0)
#define zig_maxInt_u16 UINT16_MAX
#define zig_minInt_i16  INT16_MIN
#define zig_maxInt_i16  INT16_MAX
#define zig_minInt_u32 zig_as_u32(0)
#define zig_maxInt_u32 UINT32_MAX
#define zig_minInt_i32  INT32_MIN
#define zig_maxInt_i32  INT32_MAX
#define zig_minInt_u64 zig_as_u64(0)
#define zig_maxInt_u64 UINT64_MAX
#define zig_minInt_i64  INT64_MIN
#define zig_maxInt_i64  INT64_MAX

#if FLT_MANT_DIG == 11
typedef float zig_f16;
#elif DBL_MANT_DIG == 11
typedef double zig_f16;
#elif LDBL_MANT_DIG == 11
typedef long double zig_f16;
#elif FLT16_MANT_DIG == 11
typedef _Float16 zig_f16;
#endif

#if FLT_MANT_DIG == 24
typedef float zig_f32;
#elif DBL_MANT_DIG == 24
typedef double zig_f32;
#elif LDBL_MANT_DIG == 24
typedef long double zig_f32;
#elif FLT32_MANT_DIG == 24
typedef _Float32 zig_f32;
#endif

#if FLT_MANT_DIG == 53
typedef float zig_f64;
#elif DBL_MANT_DIG == 53
typedef double zig_f64;
#elif LDBL_MANT_DIG == 53
typedef long double zig_f64;
#elif FLT64_MANT_DIG == 53
typedef _Float64 zig_f64;
#endif

#if FLT_MANT_DIG == 64
typedef float zig_f80;
#elif DBL_MANT_DIG == 64
typedef double zig_f80;
#elif LDBL_MANT_DIG == 64
typedef long double zig_f80;
#elif FLT80_MANT_DIG == 64
typedef _Float80 zig_f80;
#elif defined(__SIZEOF_FLOAT80__)
typedef __float80 zig_f80;
#endif

#if FLT_MANT_DIG == 113
typedef float zig_f128;
#elif DBL_MANT_DIG == 113
typedef double zig_f128;
#elif LDBL_MANT_DIG == 113
typedef long double zig_f128;
#elif FLT128_MANT_DIG == 113
typedef _Float128 zig_f128;
#elif defined(__SIZEOF_FLOAT128__)
typedef __float128 zig_f128;
#endif

zig_extern_c void *memcpy (void *zig_restrict, void const *zig_restrict, zig_usize);
zig_extern_c void *memset (void *, int, zig_usize);

/* ==================== 8/16/32/64-bit Integer Routines ===================== */

#define zig_maxInt(Type, bits) zig_shr_##Type(zig_maxInt_##Type, (zig_bitSizeOf(zig_##Type) - bits))
#define zig_minInt(Type, bits) zig_not_##Type(zig_maxInt(Type, bits), bits)

#define zig_int_operator(Type, RhsType, operation, operator) \
    static inline zig_##Type zig_##operation##_##Type(zig_##Type lhs, zig_##RhsType rhs) { \
        return lhs operator rhs; \
    }
#define zig_int_operators(w) \
    zig_int_operator(u##w, u##w,       and,  &) \
    zig_int_operator(i##w, i##w,       and,  &) \
    zig_int_operator(u##w, u##w,        or,  |) \
    zig_int_operator(i##w, i##w,        or,  |) \
    zig_int_operator(u##w, u##w,       xor,  ^) \
    zig_int_operator(i##w, i##w,       xor,  ^) \
    zig_int_operator(u##w, u8,         shl, <<) \
    zig_int_operator(i##w, u8,         shl, <<) \
    zig_int_operator(u##w, u8,         shr, >>) \
    zig_int_operator(u##w, u##w, div_floor,  /) \
    zig_int_operator(u##w, u##w,       mod,  %)
zig_int_operators(8)
zig_int_operators(16)
zig_int_operators(32)
zig_int_operators(64)

#define zig_int_helpers(w) \
    static inline zig_i##w zig_shr_i##w(zig_i##w lhs, zig_u8 rhs) { \
        zig_i##w sign_mask = lhs < zig_as_i##w(0) ? zig_as_i##w(-1) : zig_as_i##w(0); \
        return ((lhs ^ sign_mask) >> rhs) ^ sign_mask; \
    } \
\
    static inline zig_u##w zig_not_u##w(zig_u##w val, zig_u8 bits) { \
        return val ^ zig_maxInt(u##w, bits); \
    } \
\
    static inline zig_i##w zig_not_i##w(zig_i##w val, zig_u8 bits) { \
        (void)bits; \
        return ~val; \
    } \
\
    static inline zig_u##w zig_wrap_u##w(zig_u##w val, zig_u8 bits) { \
        return val & zig_maxInt(u##w, bits); \
    } \
\
    static inline zig_i##w zig_wrap_i##w(zig_i##w val, zig_u8 bits) { \
        return (val & zig_as_u##w(1) << (bits - zig_as_u8(1))) != 0 \
            ? val | zig_minInt(i##w, bits) : val & zig_maxInt(i##w, bits); \
    } \
\
    static inline zig_i##w zig_div_floor_i##w(zig_i##w lhs, zig_i##w rhs) { \
        return lhs / rhs - (((lhs ^ rhs) & (lhs % rhs)) < zig_as_i##w(0)); \
    } \
\
    static inline zig_i##w zig_mod_i##w(zig_i##w lhs, zig_i##w rhs) { \
        zig_i##w rem = lhs % rhs; \
        return rem + (((lhs ^ rhs) & rem) < zig_as_i##w(0) ? rhs : zig_as_i##w(0)); \
    }
zig_int_helpers(8)
zig_int_helpers(16)
zig_int_helpers(32)
zig_int_helpers(64)

static inline zig_bool zig_addo_u32(zig_u32 *res, zig_u32 lhs, zig_u32 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u32 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u32(full_res, bits);
    return overflow || full_res < zig_minInt(u32, bits) || full_res > zig_maxInt(u32, bits);
#else
    *res = zig_addw_u32(lhs, rhs, bits);
    return *res < lhs;
#endif
}

zig_extern_c zig_i32  __addosi4(zig_i32 lhs, zig_i32 rhs, zig_c_int *overflow);
static inline zig_bool zig_addo_i32(zig_i32 *res, zig_i32 lhs, zig_i32 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i32 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u32 full_res = __addosi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i32(full_res, bits);
    return overflow || full_res < zig_minInt(i32, bits) || full_res > zig_maxInt(i32, bits);
}

static inline zig_bool zig_addo_u64(zig_u64 *res, zig_u64 lhs, zig_u64 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u64 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u64(full_res, bits);
    return overflow || full_res < zig_minInt(u64, bits) || full_res > zig_maxInt(u64, bits);
#else
    *res = zig_addw_u64(lhs, rhs, bits);
    return *res < lhs;
#endif
}

zig_extern_c zig_i64  __addodi4(zig_i64 lhs, zig_i64 rhs, zig_c_int *overflow);
static inline zig_bool zig_addo_i64(zig_i64 *res, zig_i64 lhs, zig_i64 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i64 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u64 full_res = __addodi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i64(full_res, bits);
    return overflow || full_res < zig_minInt(i64, bits) || full_res > zig_maxInt(i64, bits);
}

static inline zig_bool zig_addo_u8(zig_u8 *res, zig_u8 lhs, zig_u8 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u8 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u8(full_res, bits);
    return overflow || full_res < zig_minInt(u8, bits) || full_res > zig_maxInt(u8, bits);
#else
    return zig_addo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_addo_i8(zig_i8 *res, zig_i8 lhs, zig_i8 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i8 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i8(full_res, bits);
    return overflow || full_res < zig_minInt(i8, bits) || full_res > zig_maxInt(i8, bits);
#else
    return zig_addo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_addo_u16(zig_u16 *res, zig_u16 lhs, zig_u16 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u16 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u16(full_res, bits);
    return overflow || full_res < zig_minInt(u16, bits) || full_res > zig_maxInt(u16, bits);
#else
    return zig_addo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_addo_i16(zig_i16 *res, zig_i16 lhs, zig_i16 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i16 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i16(full_res, bits);
    return overflow || full_res < zig_minInt(i16, bits) || full_res > zig_maxInt(i16, bits);
#else
    return zig_addo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_u32(zig_u32 *res, zig_u32 lhs, zig_u32 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u32 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u32(full_res, bits);
    return overflow || full_res < zig_minInt(u32, bits) || full_res > zig_maxInt(u32, bits);
#else
    *res = zig_subw_u32(lhs, rhs, bits);
    return *res > lhs;
#endif
}

zig_extern_c zig_i32  __subosi4(zig_i32 lhs, zig_i32 rhs, zig_c_int *overflow);
static inline zig_bool zig_subo_i32(zig_i32 *res, zig_i32 lhs, zig_i32 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i32 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u32 full_res = __subosi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i32(full_res, bits);
    return overflow || full_res < zig_minInt(i32, bits) || full_res > zig_maxInt(i32, bits);
}

static inline zig_bool zig_subo_u64(zig_u64 *res, zig_u64 lhs, zig_u64 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u64 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u64(full_res, bits);
    return overflow || full_res < zig_minInt(u64, bits) || full_res > zig_maxInt(u64, bits);
#else
    *res = zig_subw_u64(lhs, rhs, bits);
    return *res > lhs;
#endif
}

zig_extern_c zig_i64  __subodi4(zig_i64 lhs, zig_i64 rhs, zig_c_int *overflow);
static inline zig_bool zig_subo_i64(zig_i64 *res, zig_i64 lhs, zig_i64 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i64 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u64 full_res = __subodi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i64(full_res, bits);
    return overflow || full_res < zig_minInt(i64, bits) || full_res > zig_maxInt(i64, bits);
}

static inline zig_bool zig_subo_u8(zig_u8 *res, zig_u8 lhs, zig_u8 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u8 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u8(full_res, bits);
    return overflow || full_res < zig_minInt(u8, bits) || full_res > zig_maxInt(u8, bits);
#else
    return zig_subo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_i8(zig_i8 *res, zig_i8 lhs, zig_i8 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i8 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i8(full_res, bits);
    return overflow || full_res < zig_minInt(i8, bits) || full_res > zig_maxInt(i8, bits);
#else
    return zig_subo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_u16(zig_u16 *res, zig_u16 lhs, zig_u16 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u16 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u16(full_res, bits);
    return overflow || full_res < zig_minInt(u16, bits) || full_res > zig_maxInt(u16, bits);
#else
    return zig_subo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_subo_i16(zig_i16 *res, zig_i16 lhs, zig_i16 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i16 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i16(full_res, bits);
    return overflow || full_res < zig_minInt(i16, bits) || full_res > zig_maxInt(i16, bits);
#else
    return zig_subo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_u32(zig_u32 *res, zig_u32 lhs, zig_u32 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u32 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u32(full_res, bits);
    return overflow || full_res < zig_minInt(u32, bits) || full_res > zig_maxInt(u32, bits);
#else
    *res = zig_mulw_u32(lhs, rhs, bits);
    return rhs != zig_as_u32(0) && lhs > zig_maxInt(u32, bits) / rhs;
#endif
}

zig_extern_c zig_i32  __mulosi4(zig_i32 lhs, zig_i32 rhs, zig_c_int *overflow);
static inline zig_bool zig_mulo_i32(zig_i32 *res, zig_i32 lhs, zig_i32 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i32 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u32 full_res = __mulosi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i32(full_res, bits);
    return overflow || full_res < zig_minInt(i32, bits) || full_res > zig_maxInt(i32, bits);
}

static inline zig_bool zig_mulo_u64(zig_u64 *res, zig_u64 lhs, zig_u64 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u64 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u64(full_res, bits);
    return overflow || full_res < zig_minInt(u64, bits) || full_res > zig_maxInt(u64, bits);
#else
    *res = zig_mulw_u64(lhs, rhs, bits);
    return rhs != zig_as_u64(0) && lhs > zig_maxInt(u64, bits) / rhs;
#endif
}

zig_extern_c zig_i64  __mulodi4(zig_i64 lhs, zig_i64 rhs, zig_c_int *overflow);
static inline zig_bool zig_mulo_i64(zig_i64 *res, zig_i64 lhs, zig_i64 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i64 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_u64 full_res = __mulodi4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i64(full_res, bits);
    return overflow || full_res < zig_minInt(i64, bits) || full_res > zig_maxInt(i64, bits);
}

static inline zig_bool zig_mulo_u8(zig_u8 *res, zig_u8 lhs, zig_u8 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u8 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u8(full_res, bits);
    return overflow || full_res < zig_minInt(u8, bits) || full_res > zig_maxInt(u8, bits);
#else
    return zig_mulo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_i8(zig_i8 *res, zig_i8 lhs, zig_i8 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i8 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i8(full_res, bits);
    return overflow || full_res < zig_minInt(i8, bits) || full_res > zig_maxInt(i8, bits);
#else
    return zig_mulo_i32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_u16(zig_u16 *res, zig_u16 lhs, zig_u16 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u16 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u16(full_res, bits);
    return overflow || full_res < zig_minInt(u16, bits) || full_res > zig_maxInt(u16, bits);
#else
    return zig_mulo_u32(res, lhs, rhs, bits);
#endif
}

static inline zig_bool zig_mulo_i16(zig_i16 *res, zig_i16 lhs, zig_i16 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i16 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_i16(full_res, bits);
    return overflow || full_res < zig_minInt(i16, bits) || full_res > zig_maxInt(i16, bits);
#else
    return zig_mulo_i32(res, lhs, rhs, bits);
#endif
}

#define zig_int_builtins(w) \
    static inline zig_u##w zig_shlw_u##w(zig_u##w lhs, zig_u8 rhs, zig_u8 bits) { \
        return zig_wrap_u##w(zig_shl_u##w(lhs, rhs), bits); \
    } \
\
    static inline zig_i##w zig_shlw_i##w(zig_i##w lhs, zig_u8 rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)zig_shl_u##w((zig_u##w)lhs, (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_u##w zig_addw_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        return zig_wrap_u##w(lhs + rhs, bits); \
    } \
\
    static inline zig_i##w zig_addw_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)((zig_u##w)lhs + (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_u##w zig_subw_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        return zig_wrap_u##w(lhs - rhs, bits); \
    } \
\
    static inline zig_i##w zig_subw_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)((zig_u##w)lhs - (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_u##w zig_mulw_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        return zig_wrap_u##w(lhs * rhs, bits); \
    } \
\
    static inline zig_i##w zig_mulw_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        return zig_wrap_i##w((zig_i##w)((zig_u##w)lhs * (zig_u##w)rhs), bits); \
    } \
\
    static inline zig_bool zig_shlo_u##w(zig_u##w *res, zig_u##w lhs, zig_u8 rhs, zig_u8 bits) { \
        *res = zig_shlw_u##w(lhs, rhs, bits); \
        return (lhs & zig_maxInt_u##w << (bits - rhs)) != zig_as_u##w(0); \
    } \
\
    static inline zig_bool zig_shlo_i##w(zig_i##w *res, zig_i##w lhs, zig_u8 rhs, zig_u8 bits) { \
        *res = zig_shlw_i##w(lhs, rhs, bits); \
        zig_i##w mask = (zig_i##w)(zig_maxInt_u##w << (bits - rhs - 1)); \
        return (lhs & mask) != zig_as_i##w(0) && (lhs & mask) != mask; \
    } \
\
    static inline zig_u##w zig_shls_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        if (rhs >= bits) return lhs != zig_as_u##w(0) ? zig_maxInt(u##w, bits) : lhs; \
        return zig_shlo_u##w(&res, lhs, (zig_u8)rhs, bits) ? zig_maxInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_shls_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if ((zig_u##w)rhs < (zig_u##w)bits && !zig_shlo_i##w(&res, lhs, rhs, bits)) return res; \
        return lhs < zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    } \
\
    static inline zig_u##w zig_adds_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        return zig_addo_u##w(&res, lhs, rhs, bits) ? zig_maxInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_adds_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if (!zig_addo_i##w(&res, lhs, rhs, bits)) return res; \
        return res >= zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    } \
\
    static inline zig_u##w zig_subs_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        return zig_subo_u##w(&res, lhs, rhs, bits) ? zig_minInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_subs_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if (!zig_subo_i##w(&res, lhs, rhs, bits)) return res; \
        return res >= zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    } \
\
    static inline zig_u##w zig_muls_u##w(zig_u##w lhs, zig_u##w rhs, zig_u8 bits) { \
        zig_u##w res; \
        return zig_mulo_u##w(&res, lhs, rhs, bits) ? zig_maxInt(u##w, bits) : res; \
    } \
\
    static inline zig_i##w zig_muls_i##w(zig_i##w lhs, zig_i##w rhs, zig_u8 bits) { \
        zig_i##w res; \
        if (!zig_mulo_i##w(&res, lhs, rhs, bits)) return res; \
        return (lhs ^ rhs) < zig_as_i##w(0) ? zig_minInt(i##w, bits) : zig_maxInt(i##w, bits); \
    }
zig_int_builtins(8)
zig_int_builtins(16)
zig_int_builtins(32)
zig_int_builtins(64)

#define zig_builtin8(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin8;

#define zig_builtin16(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin16;

#if INT_MIN <= INT32_MIN
#define zig_builtin32(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin32;
#elif LONG_MIN <= INT32_MIN
#define zig_builtin32(name, val) __builtin_##name##l(val)
typedef zig_c_ulong zig_Builtin32;
#endif

#if INT_MIN <= INT64_MIN
#define zig_builtin64(name, val) __builtin_##name(val)
typedef zig_c_uint zig_Builtin64;
#elif LONG_MIN <= INT64_MIN
#define zig_builtin64(name, val) __builtin_##name##l(val)
typedef zig_c_ulong zig_Builtin64;
#elif LLONG_MIN <= INT64_MIN
#define zig_builtin64(name, val) __builtin_##name##ll(val)
typedef zig_c_ulonglong zig_Builtin64;
#endif

#if zig_has_builtin(clz)
#define zig_builtin_clz(w) \
    static inline zig_u8 zig_clz_u##w(zig_u##w val, zig_u8 bits) { \
        if (val == 0) return bits; \
        return zig_builtin##w(clz, val) - (zig_bitSizeOf(zig_Builtin##w) - bits); \
    } \
\
    static inline zig_u8 zig_clz_i##w(zig_i##w val, zig_u8 bits) { \
        return zig_clz_u##w((zig_u##w)val, bits); \
    }
zig_builtin_clz(8)
zig_builtin_clz(16)
zig_builtin_clz(32)
zig_builtin_clz(64)
#endif

#if zig_has_builtin(ctz)
#define zig_builtin_ctz(w) \
    static inline zig_u8 zig_ctz_u##w(zig_u##w val, zig_u8 bits) { \
        if (val == 0) return bits; \
        return zig_builtin##w(ctz, val); \
    } \
\
    static inline zig_u8 zig_ctz_i##w(zig_i##w val, zig_u8 bits) { \
        return zig_ctz_u##w((zig_u##w)val, bits); \
    }
zig_builtin_ctz(8)
zig_builtin_ctz(16)
zig_builtin_ctz(32)
zig_builtin_ctz(64)
#endif

#if zig_has_builtin(popcount)
#define zig_builtin_popcount(w) \
    static inline zig_u8 zig_popcount_u##w(zig_u##w val, zig_u8 bits) { \
        (void)bits; \
        return zig_builtin##w(popcount, val); \
    } \
\
    static inline zig_u8 zig_popcount_i##w(zig_i##w val, zig_u8 bits) { \
        \
        return zig_popcount_u##w((zig_u##w)val, bits); \
    }
zig_builtin_popcount(8)
zig_builtin_popcount(16)
zig_builtin_popcount(32)
zig_builtin_popcount(64)
#endif

static inline zig_u8 zig_byte_swap_u8(zig_u8 val, zig_u8 bits) {
    return zig_wrap_u8(val >> (8 - bits), bits);
}

static inline zig_i8 zig_byte_swap_i8(zig_i8 val, zig_u8 bits) {
    return zig_wrap_i8((zig_i8)zig_byte_swap_u8((zig_u8)val, bits), bits);
}

static inline zig_u16 zig_byte_swap_u16(zig_u16 val, zig_u8 bits) {
    zig_u16 full_res;
#if zig_has_builtin(bswap16)
    full_res = __builtin_bswap16(val);
#else
    full_res = (zig_u16)zig_byte_swap_u8((zig_u8)(val >>  0)) <<  8 |
               (zig_u16)zig_byte_swap_u8((zig_u8)(val >>  8)) >>  0;
#endif
    return zig_wrap_u16(full_res >> (16 - bits), bits);
}

static inline zig_i16 zig_byte_swap_i16(zig_i16 val, zig_u8 bits) {
    return zig_wrap_i16((zig_i16)zig_byte_swap_u16((zig_u16)val, bits), bits);
}

static inline zig_u32 zig_byte_swap_u32(zig_u32 val, zig_u8 bits) {
    zig_u32 full_res;
#if zig_has_builtin(bswap32)
    full_res = __builtin_bswap32(val);
#else
    full_res = (zig_u32)zig_byte_swap_u16((zig_u16)(val >>  0)) << 16 |
               (zig_u32)zig_byte_swap_u16((zig_u16)(val >> 16)) >>  0;
#endif
    return zig_wrap_u32(full_res >> (32 - bits), bits);
}

static inline zig_i32 zig_byte_swap_i32(zig_i32 val, zig_u8 bits) {
    return zig_wrap_i32((zig_i32)zig_byte_swap_u32((zig_u32)val, bits), bits);
}

static inline zig_u64 zig_byte_swap_u64(zig_u64 val, zig_u8 bits) {
    zig_u64 full_res;
#if zig_has_builtin(bswap64)
    full_res = __builtin_bswap64(val);
#else
    full_res = (zig_u64)zig_byte_swap_u32((zig_u32)(val >>  0)) << 32 |
               (zig_u64)zig_byte_swap_u32((zig_u32)(val >> 32)) >>  0;
#endif
    return zig_wrap_u64(full_res >> (64 - bits), bits);
}

static inline zig_i64 zig_byte_swap_i64(zig_i64 val, zig_u8 bits) {
    return zig_wrap_i64((zig_i64)zig_byte_swap_u64((zig_u64)val, bits), bits);
}

static inline zig_u8 zig_bit_reverse_u8(zig_u8 val, zig_u8 bits) {
    zig_u8 full_res;
#if zig_has_builtin(bitreverse8)
    full_res = __builtin_bitreverse8(val);
#else
    static zig_u8 const lut[0x10] = {
        0b0000, 0b1000, 0b0100, 0b1100,
        0b0010, 0b1010, 0b0110, 0b1110,
        0b0001, 0b1001, 0b0101, 0b1101,
        0b0011, 0b1011, 0b0111, 0b1111,
    };
    full_res = lut[val >> 0 & 0xF] << 4 | lut[val >> 4 & 0xF] << 0;
#endif
    return zig_wrap_u8(full_res >> (8 - bits), bits);
}

static inline zig_i8 zig_bit_reverse_i8(zig_i8 val, zig_u8 bits) {
    return zig_wrap_i8((zig_i8)zig_bit_reverse_u8((zig_u8)val, bits), bits);
}

static inline zig_u16 zig_bit_reverse_u16(zig_u16 val, zig_u8 bits) {
    zig_u16 full_res;
#if zig_has_builtin(bitreverse16)
    full_res = __builtin_bitreverse16(val);
#else
    full_res = (zig_u16)zig_bit_reverse_u8((zig_u8)(val >>  0)) <<  8 |
               (zig_u16)zig_bit_reverse_u8((zig_u8)(val >>  8)) >>  0;
#endif
    return zig_wrap_u16(full_res >> (16 - bits), bits);
}

static inline zig_i16 zig_bit_reverse_i16(zig_i16 val, zig_u8 bits) {
    return zig_wrap_i16((zig_i16)zig_bit_reverse_u16((zig_u16)val, bits), bits);
}

static inline zig_u32 zig_bit_reverse_u32(zig_u32 val, zig_u8 bits) {
    zig_u32 full_res;
#if zig_has_builtin(bitreverse32)
    full_res = __builtin_bitreverse32(val);
#else
    full_res = (zig_u32)zig_bit_reverse_u16((zig_u16)(val >>  0)) << 16 |
               (zig_u32)zig_bit_reverse_u16((zig_u16)(val >> 16)) >>  0;
#endif
    return zig_wrap_u32(full_res >> (32 - bits), bits);
}

static inline zig_i32 zig_bit_reverse_i32(zig_i32 val, zig_u8 bits) {
    return zig_wrap_i32((zig_i32)zig_bit_reverse_u32((zig_u32)val, bits), bits);
}

static inline zig_u64 zig_bit_reverse_u64(zig_u64 val, zig_u8 bits) {
    zig_u64 full_res;
#if zig_has_builtin(bitreverse64)
    full_res = __builtin_bitreverse64(val);
#else
    full_res = (zig_u64)zig_bit_reverse_u32((zig_u32)(val >>  0)) << 32 |
               (zig_u64)zig_bit_reverse_u32((zig_u32)(val >> 32)) >>  0;
#endif
    return zig_wrap_u64(full_res >> (64 - bits), bits);
}

static inline zig_i64 zig_bit_reverse_i64(zig_i64 val, zig_u8 bits) {
    return zig_wrap_i64((zig_i64)zig_bit_reverse_u64((zig_u64)val, bits), bits);
}

/* ======================== 128-bit Integer Routines ======================== */

#if !defined(zig_has_int128)
# if defined(__SIZEOF_INT128__)
#  define zig_has_int128 1
# else
#  define zig_has_int128 0
# endif
#endif

#if zig_has_int128

typedef unsigned __int128 zig_u128;
typedef   signed __int128 zig_i128;

#define zig_as_u128(hi, lo) ((zig_u128)(hi)<<64|(lo))
#define zig_as_i128(hi, lo) ((zig_i128)zig_as_u128(hi, lo))
#define zig_hi_u128(val) ((zig_u64)((val) >> 64))
#define zig_lo_u128(val) ((zig_u64)((val) >>  0))
#define zig_hi_i128(val) ((zig_i64)((val) >> 64))
#define zig_lo_i128(val) ((zig_u64)((val) >>  0))
#define zig_bitcast_u128(val) ((zig_u128)(val))
#define zig_bitcast_i128(val) ((zig_i128)(val))
#define zig_cmp_int128(Type) \
    static inline zig_i8 zig_cmp_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return (lhs > rhs) - (lhs < rhs); \
    }
#define zig_bit_int128(Type, operation, operator) \
    static inline zig_##Type zig_##operation##_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return lhs operator rhs; \
    }

#else /* zig_has_int128 */

#if __LITTLE_ENDIAN__ || _MSC_VER
typedef struct { zig_align(16) zig_u64 lo; zig_u64 hi; } zig_u128;
typedef struct { zig_align(16) zig_u64 lo; zig_i64 hi; } zig_i128;
#else
typedef struct { zig_align(16) zig_u64 hi; zig_u64 lo; } zig_u128;
typedef struct { zig_align(16) zig_i64 hi; zig_u64 lo; } zig_i128;
#endif

#define zig_as_u128(hi, lo) ((zig_u128){ .h##i = (hi), .l##o = (lo) })
#define zig_as_i128(hi, lo) ((zig_i128){ .h##i = (hi), .l##o = (lo) })
#define zig_hi_u128(val) ((val).hi)
#define zig_lo_u128(val) ((val).lo)
#define zig_hi_i128(val) ((val).hi)
#define zig_lo_i128(val) ((val).lo)
#define zig_bitcast_u128(val) zig_as_u128((zig_u64)(val).hi, (val).lo)
#define zig_bitcast_i128(val) zig_as_i128((zig_i64)(val).hi, (val).lo)
#define zig_cmp_int128(Type) \
    static inline zig_i8 zig_cmp_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return (lhs.hi == rhs.hi) \
            ? (lhs.lo > rhs.lo) - (lhs.lo < rhs.lo) \
            : (lhs.hi > rhs.hi) - (lhs.hi < rhs.hi); \
    }
#define zig_bit_int128(Type, operation, operator) \
    static inline zig_##Type zig_##operation##_##Type(zig_##Type lhs, zig_##Type rhs) { \
        return (zig_##Type){ .hi = lhs.hi operator rhs.hi, .lo = lhs.lo operator rhs.lo }; \
    }

#endif /* zig_has_int128 */

#define zig_minInt_u128 zig_as_u128(zig_minInt_u64, zig_minInt_u64)
#define zig_maxInt_u128 zig_as_u128(zig_maxInt_u64, zig_maxInt_u64)
#define zig_minInt_i128 zig_as_i128(zig_minInt_i64, zig_minInt_u64)
#define zig_maxInt_i128 zig_as_i128(zig_maxInt_i64, zig_maxInt_u64)

zig_cmp_int128(u128)
zig_cmp_int128(i128)

zig_bit_int128(u128, and, &)
zig_bit_int128(i128, and, &)

zig_bit_int128(u128,  or, |)
zig_bit_int128(i128,  or, |)

zig_bit_int128(u128, xor, ^)
zig_bit_int128(i128, xor, ^)

static inline zig_u128 zig_shr_u128(zig_u128 lhs, zig_u8 rhs);

#if zig_has_int128

static inline zig_u128 zig_not_u128(zig_u128 val, zig_u8 bits) {
    return val ^ zig_maxInt(u128, bits);
}

static inline zig_i128 zig_not_i128(zig_i128 val, zig_u8 bits) {
    (void)bits;
    return ~val;
}

static inline zig_u128 zig_shr_u128(zig_u128 lhs, zig_u8 rhs) {
    return lhs >> rhs;
}

static inline zig_u128 zig_shl_u128(zig_u128 lhs, zig_u8 rhs) {
    return lhs << rhs;
}

static inline zig_i128 zig_shl_i128(zig_i128 lhs, zig_u8 rhs) {
    return lhs << rhs;
}

static inline zig_u128 zig_add_u128(zig_u128 lhs, zig_u128 rhs) {
    return lhs + rhs;
}

static inline zig_i128 zig_add_i128(zig_i128 lhs, zig_i128 rhs) {
    return lhs + rhs;
}

static inline zig_u128 zig_sub_u128(zig_u128 lhs, zig_u128 rhs) {
    return lhs - rhs;
}

static inline zig_i128 zig_sub_i128(zig_i128 lhs, zig_i128 rhs) {
    return lhs - rhs;
}

static inline zig_u128 zig_mul_u128(zig_u128 lhs, zig_u128 rhs) {
    return lhs * rhs;
}

static inline zig_i128 zig_mul_i128(zig_i128 lhs, zig_i128 rhs) {
    return lhs * rhs;
}

static inline zig_u128 zig_div_trunc_u128(zig_u128 lhs, zig_u128 rhs) {
    return lhs / rhs;
}

static inline zig_i128 zig_div_trunc_i128(zig_i128 lhs, zig_i128 rhs) {
    return lhs / rhs;
}

static inline zig_u128 zig_rem_u128(zig_u128 lhs, zig_u128 rhs) {
    return lhs % rhs;
}

static inline zig_i128 zig_rem_i128(zig_i128 lhs, zig_i128 rhs) {
    return lhs % rhs;
}

static inline zig_i128 zig_div_floor_i128(zig_i128 lhs, zig_i128 rhs) {
    return zig_div_trunc_i128(lhs, rhs) - (((lhs ^ rhs) & zig_rem_i128(lhs, rhs)) < zig_as_i128(0, 0));
}

static inline zig_i128 zig_mod_i128(zig_i128 lhs, zig_i128 rhs) {
    zig_i128 rem = zig_rem_i128(lhs, rhs);
    return rem + (((lhs ^ rhs) & rem) < zig_as_i128(0, 0) ? rhs : zig_as_i128(0, 0));
}

#else /* zig_has_int128 */

static inline zig_u128 zig_not_u128(zig_u128 val, zig_u8 bits) {
    return (zig_u128){ .hi = zig_not_u64(val.hi, bits - zig_as_u8(64)), .lo = zig_not_u64(val.lo, zig_as_u8(64)) };
}

static inline zig_i128 zig_not_i128(zig_i128 val, zig_u8 bits) {
    return (zig_i128){ .hi = zig_not_i64(val.hi, bits - zig_as_u8(64)), .lo = zig_not_u64(val.lo, zig_as_u8(64)) };
}

static inline zig_u128 zig_shr_u128(zig_u128 lhs, zig_u8 rhs) {
    if (rhs >= zig_as_u8(64)) return (zig_u128){ .hi = lhs.hi << (rhs - zig_as_u8(64)), .lo = zig_minInt_u64 };
    return (zig_u128){ .hi = lhs.hi << rhs | lhs.lo >> (zig_as_u8(64) - rhs), .lo = lhs.lo << rhs };
}

static inline zig_u128 zig_shl_u128(zig_u128 lhs, zig_u8 rhs) {
    if (rhs >= zig_as_u8(64)) return (zig_u128){ .hi = lhs.hi << (rhs - zig_as_u8(64)), .lo = zig_minInt_u64 };
    return (zig_u128){ .hi = lhs.hi << rhs | lhs.lo >> (zig_as_u8(64) - rhs), .lo = lhs.lo << rhs };
}

static inline zig_i128 zig_shl_i128(zig_i128 lhs, zig_u8 rhs) {
    if (rhs >= zig_as_u8(64)) return (zig_i128){ .hi = lhs.hi << (rhs - zig_as_u8(64)), .lo = zig_minInt_u64 };
    return (zig_i128){ .hi = lhs.hi << rhs | lhs.lo >> (zig_as_u8(64) - rhs), .lo = lhs.lo << rhs };
}

static inline zig_u128 zig_add_u128(zig_u128 lhs, zig_u128 rhs) {
    zig_u128 res;
    res.hi = lhs.hi + rhs.hi + zig_addo_u64(&res.lo, lhs.lo, rhs.lo, zig_maxInt_u64);
    return res;
}

static inline zig_i128 zig_add_i128(zig_i128 lhs, zig_i128 rhs) {
    zig_i128 res;
    res.hi = lhs.hi + rhs.hi + zig_addo_u64(&res.lo, lhs.lo, rhs.lo, zig_maxInt_u64);
    return res;
}

static inline zig_u128 zig_sub_u128(zig_u128 lhs, zig_u128 rhs) {
    zig_u128 res;
    res.hi = lhs.hi - rhs.hi - zig_subo_u64(&res.lo, lhs.lo, rhs.lo, zig_maxInt_u64);
    return res;
}

static inline zig_i128 zig_sub_i128(zig_i128 lhs, zig_i128 rhs) {
    zig_i128 res;
    res.hi = lhs.hi - rhs.hi - zig_subo_u64(&res.lo, lhs.lo, rhs.lo, zig_maxInt_u64);
    return res;
}

static inline zig_i128 zig_div_floor_i128(zig_i128 lhs, zig_i128 rhs) {
    return zig_sub_i128(zig_div_trunc_i128(lhs, rhs), (((lhs.hi ^ rhs.hi) & zig_rem_i128(lhs, rhs).hi) < zig_as_i64(0)) ? zig_as_i128(0, 1) : zig_as_i128(0, 0));
}

static inline zig_i128 zig_mod_i128(zig_i128 lhs, zig_i128 rhs) {
    zig_i128 rem = zig_rem_i128(lhs, rhs);
    return rem + (((lhs.hi ^ rhs.hi) & rem.hi) < zig_as_i64(0) ? rhs : zig_as_i128(0, 0));
}

#endif /* zig_has_int128 */

#define zig_div_floor_u128 zig_div_trunc_u128
#define zig_mod_u128 zig_rem_u128

static inline zig_i128 zig_shr_i128(zig_i128 lhs, zig_u8 rhs) {
    zig_i128 sign_mask = zig_cmp_i128(lhs, zig_as_i128(0, 0)) < zig_as_i8(0) ? zig_as_i128(-1, UINT64_MAX) : zig_as_i128(0, 0);
    return zig_xor_i128(zig_bitcast_i128(zig_shr_u128(zig_bitcast_u128(zig_xor_i128(lhs, sign_mask)), rhs)), sign_mask);
}

static inline zig_u128 zig_wrap_u128(zig_u128 val, zig_u8 bits) {
    return zig_and_u128(val, zig_maxInt(u128, bits));
}

static inline zig_i128 zig_wrap_i128(zig_i128 val, zig_u8 bits) {
    return zig_as_i128(zig_wrap_i64(zig_hi_i128(val), bits - zig_as_u8(64)), zig_lo_i128(val));
}

static inline zig_u128 zig_shlw_u128(zig_u128 lhs, zig_u8 rhs, zig_u8 bits) {
    return zig_wrap_u128(zig_shl_u128(lhs, rhs), bits);
}

static inline zig_i128 zig_shlw_i128(zig_i128 lhs, zig_u8 rhs, zig_u8 bits) {
    return zig_wrap_i128(zig_bitcast_i128(zig_shl_u128(zig_bitcast_u128(lhs), zig_bitcast_u128(rhs))), bits);
}

static inline zig_u128 zig_addw_u128(zig_u128 lhs, zig_u8 rhs, zig_u8 bits) {
    return zig_wrap_u128(zig_add_u128(lhs, rhs), bits);
}

static inline zig_i128 zig_addw_i128(zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
    return zig_wrap_i128(zig_bitcast_i128(zig_add_u128(zig_bitcast_u128(lhs), zig_bitcast_u128(rhs))), bits);
}

static inline zig_u128 zig_subw_u128(zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
    return zig_wrap_u128(zig_sub_u128(lhs, rhs), bits);
}

static inline zig_i128 zig_subw_i128(zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
    return zig_wrap_i128(zig_bitcast_i128(zig_sub_u128(zig_bitcast_u128(lhs), zig_bitcast_u128(rhs))), bits);
}

static inline zig_u128 zig_mulw_u128(zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
    return zig_wrap_u128(zig_mul_u128(lhs, rhs), bits);
}

static inline zig_i128 zig_mulw_i128(zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
    return zig_wrap_i128(zig_bitcast_i128(zig_mul_u128(zig_bitcast_u128(lhs), zig_bitcast_u128(rhs))), bits);
}

#if zig_has_int128

static inline zig_bool zig_shlo_u128(zig_u128 *res, zig_u128 lhs, zig_u8 rhs, zig_u8 bits) {
    *res = zig_shlw_u128(lhs, rhs, bits);
    return zig_and_u128(lhs, zig_shl_u128(zig_maxInt_u128, bits - rhs)) != zig_as_u128(0, 0);
}

static inline zig_bool zig_shlo_i128(zig_i128 *res, zig_i128 lhs, zig_u8 rhs, zig_u8 bits) {
    *res = zig_shlw_i128(lhs, rhs, bits);
    zig_i128 mask = zig_bitcast_i128(zig_shl_u128(zig_maxInt_u128, bits - rhs - zig_as_u8(1)));
    return zig_cmp_i128(zig_and_i128(lhs, mask), zig_as_i128(0, 0)) != zig_as_i8(0) &&
           zig_cmp_i128(zig_and_i128(lhs, mask), mask) != zig_as_i8(0);
}

static inline zig_bool zig_addo_u128(zig_u128 *res, zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_u128 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u128(full_res, bits);
    return overflow || full_res < zig_minInt(u128, bits) || full_res > zig_maxInt(u128, bits);
#else
    *res = zig_addw_u128(lhs, rhs, bits);
    return *res < lhs;
#endif
}

zig_extern_c zig_i128  __addoti4(zig_i128 lhs, zig_i128 rhs, zig_c_int *overflow);
static inline zig_bool zig_addo_i128(zig_i128 *res, zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
#if zig_has_builtin(add_overflow)
    zig_i128 full_res;
    zig_bool overflow = __builtin_add_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_i128 full_res =  __addoti4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i128(full_res, bits);
    return overflow || full_res < zig_minInt(i128, bits) || full_res > zig_maxInt(i128, bits);
}

static inline zig_bool zig_subo_u128(zig_u128 *res, zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_u128 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u128(full_res, bits);
    return overflow || full_res < zig_minInt(u128, bits) || full_res > zig_maxInt(u128, bits);
#else
    *res = zig_subw_u128(lhs, rhs, bits);
    return *res > lhs;
#endif
}

zig_extern_c zig_i128  __suboti4(zig_i128 lhs, zig_i128 rhs, zig_c_int *overflow);
static inline zig_bool zig_subo_i128(zig_i128 *res, zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
#if zig_has_builtin(sub_overflow)
    zig_i128 full_res;
    zig_bool overflow = __builtin_sub_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_i128 full_res = __suboti4(lhs, rhs, &overflow_int);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i128(full_res, bits);
    return overflow || full_res < zig_minInt(i128, bits) || full_res > zig_maxInt(i128, bits);
}

static inline zig_bool zig_mulo_u128(zig_u128 *res, zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_u128 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
    *res = zig_wrap_u128(full_res, bits);
    return overflow || full_res < zig_minInt(u128, bits) || full_res > zig_maxInt(u128, bits);
#else
    *res = zig_mulw_u128(lhs, rhs, bits);
    return rhs != zig_as_u128(0, 0) && lhs > zig_maxInt(u128, bits) / rhs;
#endif
}

zig_extern_c zig_i128  __muloti4(zig_i128 lhs, zig_i128 rhs, zig_c_int *overflow);
static inline zig_bool zig_mulo_i128(zig_i128 *res, zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
#if zig_has_builtin(mul_overflow)
    zig_i128 full_res;
    zig_bool overflow = __builtin_mul_overflow(lhs, rhs, &full_res);
#else
    zig_c_int overflow_int;
    zig_i128 full_res =  __muloti4(lhs, rhs, &overflow);
    zig_bool overflow = overflow_int != 0;
#endif
    *res = zig_wrap_i128(full_res, bits);
    return overflow || full_res < zig_minInt(i128, bits) || full_res > zig_maxInt(i128, bits);
}

#else /* zig_has_int128 */

static inline zig_bool zig_addo_u128(zig_u128 *res, zig_u128 lhs, zig_u128 rhs) {
    return zig_addo_u64(&res->hi, lhs.hi, rhs.hi, UINT64_MAX) |
           zig_addo_u64(&res->hi, res->hi, zig_addo_u64(&res->lo, lhs.lo, rhs.lo, UINT64_MAX));
}

static inline zig_bool zig_subo_u128(zig_u128 *res, zig_u128 lhs, zig_u128 rhs) {
    return zig_subo_u64(&res->hi, lhs.hi, rhs.hi, UINT64_MAX) |
           zig_subo_u64(&res->hi, res->hi, zig_subo_u64(&res->lo, lhs.lo, rhs.lo, UINT64_MAX));
}

#endif /* zig_has_int128 */

static inline zig_u128 zig_shls_u128(zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
    zig_u128 res;
    if (zig_cmp_u128(rhs, zig_as_u128(0, bits)) >= zig_as_i8(0))
        return zig_cmp_u128(lhs, zig_as_u128(0, 0)) != zig_as_i8(0) ? zig_maxInt(u128, bits) : lhs;
    return zig_shlo_u128(&res, lhs, (zig_u8)rhs, bits) ? zig_maxInt(u128, bits) : res;
}

static inline zig_i128 zig_shls_i128(zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
    zig_i128 res;
    if (zig_cmp_u128(zig_bitcast_u128(rhs), zig_as_u128(0, bits)) < zig_as_i8(0) && !zig_shlo_i128(&res, lhs, rhs, bits)) return res;
    return zig_cmp_i128(lhs, zig_as_i128(0, 0)) < zig_as_i8(0) ? zig_minInt(i128, bits) : zig_maxInt(i128, bits);
}

static inline zig_u128 zig_adds_u128(zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
    zig_u128 res;
    return zig_addo_u128(&res, lhs, rhs, bits) ? zig_maxInt(u128, bits) : res;
}

static inline zig_i128 zig_adds_i128(zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
    zig_i128 res;
    if (!zig_addo_i128(&res, lhs, rhs, bits)) return res;
    return zig_cmp_i128(res, zig_as_i128(0, 0)) >= zig_as_i8(0) ? zig_minInt(i128, bits) : zig_maxInt(i128, bits);
}

static inline zig_u128 zig_subs_u128(zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
    zig_u128 res;
    return zig_subo_u128(&res, lhs, rhs, bits) ? zig_minInt(u128, bits) : res;
}

static inline zig_i128 zig_subs_i128(zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
    zig_i128 res;
    if (!zig_subo_i128(&res, lhs, rhs, bits)) return res;
    return zig_cmp_i128(res, zig_as_i128(0, 0)) >= zig_as_i8(0) ? zig_minInt(i128, bits) : zig_maxInt(i128, bits);
}

static inline zig_u128 zig_muls_u128(zig_u128 lhs, zig_u128 rhs, zig_u8 bits) {
    zig_u128 res;
    return zig_mulo_u128(&res, lhs, rhs, bits) ? zig_maxInt(u128, bits) : res;
}

static inline zig_i128 zig_muls_i128(zig_i128 lhs, zig_i128 rhs, zig_u8 bits) {
    zig_i128 res;
    if (!zig_mulo_i128(&res, lhs, rhs, bits)) return res;
    return zig_cmp_i128(zig_xor_i128(lhs, rhs), zig_as_i128(0, 0)) < zig_as_i8(0) ? zig_minInt(i128, bits) : zig_maxInt(i128, bits);
}

static inline zig_u8 zig_clz_u128(zig_u128 val, zig_u8 bits) {
    if (zig_hi_u128(val) != 0) return zig_clz_u64(zig_hi_u128(val), bits - zig_as_u8(64));
    return zig_clz_u64(zig_lo_u128(val), zig_as_u8(64)) + zig_as_u8(64);
}

static inline zig_u8 zig_clz_i128(zig_i128 val, zig_u8 bits) {
    return zig_clz_u128(zig_bitcast_u128(val), bits);
}

static inline zig_u8 zig_ctz_u128(zig_u128 val, zig_u8 bits) {
    if (zig_lo_u128(val) != 0) return zig_ctz_u64(zig_lo_u128(val), zig_as_u8(64));
    return zig_ctz_u64(zig_hi_u128(val), bits - zig_as_u8(64)) + zig_as_u8(64);
}

static inline zig_u8 zig_ctz_i128(zig_i128 val, zig_u8 bits) {
    return zig_ctz_u128(zig_bitcast_u128(val), bits);
}

static inline zig_u8 zig_popcount_u128(zig_u128 val, zig_u8 bits) {
    return zig_popcount_u64(zig_hi_u128(val), bits - zig_as_u8(64)) +
           zig_popcount_u64(zig_lo_u128(val), zig_as_u8(64));
}

static inline zig_u8 zig_popcount_i128(zig_i128 val, zig_u8 bits) {
    return zig_popcount_u128(zig_bitcast_u128(val), bits);
}

static inline zig_u128 zig_byte_swap_u128(zig_u128 val, zig_u8 bits) {
    zig_u128 full_res;
#if zig_has_builtin(bswap128)
    full_res = __builtin_bswap128(val);
#else
    full_res = zig_as_u128(zig_byte_swap_u64(zig_lo_u128(val), zig_as_u8(64)),
                           zig_byte_swap_u64(zig_hi_u128(val), zig_as_u8(64)));
#endif
    return zig_shr_u128(full_res, zig_as_u8(128) - bits);
}

static inline zig_i128 zig_byte_swap_i128(zig_i128 val, zig_u8 bits) {
    return zig_byte_swap_u128(zig_bitcast_u128(val), bits);
}

static inline zig_u128 zig_bit_reverse_u128(zig_u128 val, zig_u8 bits) {
    return zig_shr_u128(zig_as_u128(zig_bit_reverse_u64(zig_lo_u128(val), zig_as_u8(64)),
                                    zig_bit_reverse_u64(zig_hi_u128(val), zig_as_u8(64))),
                        zig_as_u8(128) - bits);
}

static inline zig_i128 zig_bit_reverse_i128(zig_i128 val, zig_u8 bits) {
    return zig_bit_reverse_u128(zig_bitcast_u128(val), bits);
}

/* ========================== Float Point Routines ========================== */

static inline zig_f32 zig_bitcast_f32_u32(zig_u32 arg) {
    zig_f32 dest;
    memcpy(&dest, &arg, sizeof dest);
    return dest;
}

static inline zig_f64 zig_bitcast_f64_u64(zig_u64 arg) {
    zig_f64 dest;
    memcpy(&dest, &arg, sizeof dest);
    return dest;
}

static inline float zig_div_truncf(float numerator, float denominator) {
    return __builtin_truncf(numerator / denominator);
}

static inline double zig_div_trunc(double numerator, double denominator) {
    return __builtin_trunc(numerator / denominator);
}

static inline long double zig_div_truncl(long double numerator, long double denominator) {
    return __builtin_truncf(numerator / denominator);
}

#define zig_div_trunc_f16  zig_div_truncf
#define zig_div_trunc_f32  zig_div_truncf
#define zig_div_trunc_f64  zig_div_trunc
#define zig_div_trunc_f80  zig_div_truncl
#define zig_div_trunc_f128 zig_div_truncl

#define zig_div_floorf(numerator, denominator) \
    __builtin_floorf((float)(numerator) / (float)(denominator))

#define zig_div_floor(numerator, denominator) \
    __builtin_floor((double)(numerator) / (double)(denominator))

#define zig_div_floorl(numerator, denominator) \
    __builtin_floorl((long double)(numerator) / (long double)(denominator))

#define zig_div_floor_f16  zig_div_floorf
#define zig_div_floor_f32  zig_div_floorf
#define zig_div_floor_f64  zig_div_floor
#define zig_div_floor_f80  zig_div_floorl
#define zig_div_floor_f128 zig_div_floorl

static inline float zig_modf(float numerator, float denominator) {
    return (numerator - (zig_div_floorf(numerator, denominator) * denominator));
}

static inline double zig_mod(double numerator, double denominator) {
    return (numerator - (zig_div_floor(numerator, denominator) * denominator));
}

static inline long double zig_modl(long double numerator, long double denominator) {
    return (numerator - (zig_div_floorl(numerator, denominator) * denominator));
}

#define zig_mod_f16  zig_modf
#define zig_mod_f32  zig_modf
#define zig_mod_f64  zig_mod
#define zig_mod_f80  zig_modl
#define zig_mod_f128 zig_modl
