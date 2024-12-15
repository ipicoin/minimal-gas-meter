
#define ENUMERATE_CONTROL_OPCODES(visitOp) \
       /*(  code,        name,    gas, bytes, uvecs,  u32s, u64s ) */ \
  visitOp(0x01,                 nop,      10,     0,     0,     1,    0 ) \
	visitOp(0x02,               block,      10,     0,     0,     1,    0 ) \
	visitOp(0x03,                loop,      10,     0,     0,     1,    0 ) \
	visitOp(0x04,                 if_,      10,     0,     0,     1,    0 ) \
	visitOp(0x05,               else_,      10,     0,     0,     0,    0 ) \
  visitOp(0x0B,                 end,       0,     0,     0,     0,    0 ) \
  visitOp(0x0C,                  br,      10,     0,     0,     1,    0 ) \
  visitOp(0x0D,               br_if,      10,     0,     0,     1,    0 ) \
  visitOp(0x0E,            br_table,      10,     0,     1,     1,    0 ) \
  visitOp(0x0F,             return_,      10,     0,     0,     0,    0 ) \
  visitOp(0x10,                call,      10,     0,     0,     1,    0 ) \
  visitOp(0x11,       call_indirect,      10,     0,     0,     2,    0 )

#define ENUMERATE_REFERENCE_OPCODES(visitOp) \
       /*(  code,        name,    gas, bytes, uvecs,  u32s, u64s ) */ \
  visitOp(0xD0,            ref_null,      10,     1,     0,     0,    0 ) \
	visitOp(0xD1,         ref_is_null,      10,     0,     0,     0,    0 ) \
	visitOp(0xD2,            ref_func,      10,     0,     0,     1,    0 )

#define ENUMERATE_PARAMETRIC_OPCODES(visitOp) \
       /*(  code,        name,    gas, bytes, uvecs,  u32s, u64s ) */ \
  visitOp(0x1A,                drop,      10,     0,     0,     1,    0 ) \
	visitOp(0x1B,              select,      10,     0,     0,     1,    0 ) \
	visitOp(0x1C,            select_v,      10,     0,     1,     1,    0 )

#define ENUMERATE_VARIABLE_ACCESS_OPCODES(visitOp) \
       /*(  code,        name,    gas, bytes, uvecs,  u32s, u64s ) */ \
  visitOp(0x20,           local_get,      10,     0,     0,     1,    0 ) \
  visitOp(0x21,           local_set,      10,     0,     0,     1,    0 ) \
  visitOp(0x22,           local_tee,      10,     0,     0,     1,    0 ) \
  visitOp(0x23,          global_get,      10,     0,     0,     1,    0 ) \
  visitOp(0x24,          global_set,      10,     0,     0,     1,    0 )

#define ENUMERATE_TABLE_ACCESS_OPCODES(visitOp) \
       /*(  code,        name,    gas, bytes, uvecs,  u32s, u64s ) */ \
  visitOp(0x25,           table_get,      10,     0,     0,     1,    0 ) \
  visitOp(0x26,           table_set,      10,     0,     0,     1,    0 ) \
  visitOp(0xFC0C,        table_init,     -10,     0,     0,     2,    0 ) \
  visitOp(0xFC0D,         elem_drop,      10,     0,     0,     1,    0 ) \
  visitOp(0xFC0E,        table_copy,     -10,     0,     0,     2,    0 ) \
  visitOp(0xFC0F,        table_grow,     -30,     0,     0,     1,    0 ) \
  visitOp(0xFC10,        table_size,      10,     0,     0,     1,    0 ) \
  visitOp(0xFC11,        table_fill,     -10,     0,     0,     1,    0 ) \

#define ENUMERATE_FLOATING_POINT_MEMORY_ACCESS_OPCODES(visitOp) \
  visitOp(0x2A,            f32_load,      10,     0,     0,     2,    0 )    \
  visitOp(0x2B,            f64_load,      10,     0,     0,     2,    0 )

#define ENUMERATE_MEMORY_ACCESS_OPCODES(visitOp) \
       /*(  code,        name,    gas, bytes, uvecs,  u32s, u64s ) */ \
  visitOp(0x28,            i32_load,      10,     0,     0,     2,    0 )    \
  visitOp(0x29,            i64_load,      10,     0,     0,     2,    0 )    \
  ENUMERATE_FLOATING_POINT_MEMORY_ACCESS_OPCODES(visitOp)  \
  visitOp(0x2C,         i32_load8_s,      10,     0,     0,     2,    0 )    \
  visitOp(0x2D,         i32_load8_u,      10,     0,     0,     2,    0 )    \
  visitOp(0x2E,        i32_load16_s,      10,     0,     0,     2,    0 )    \
  visitOp(0x2F,        i32_load16_u,      10,     0,     0,     2,    0 )    \
  visitOp(0x30,         i64_load8_s,      10,     0,     0,     2,    0 )    \
  visitOp(0x31,         i64_load8_u,      10,     0,     0,     2,    0 )    \
  visitOp(0x32,        i64_load16_s,      10,     0,     0,     2,    0 )    \
  visitOp(0x33,        i64_load16_u,      10,     0,     0,     2,    0 )    \
  visitOp(0x34,        i64_load32_s,      10,     0,     0,     2,    0 )    \
  visitOp(0x35,        i64_load32_u,      10,     0,     0,     2,    0 )    \
  visitOp(0x36,           i32_store,      10,     0,     0,     2,    0 )    \
  visitOp(0x37,           i64_store,      10,     0,     0,     2,    0 )    \
  visitOp(0x38,           f32_store,      10,     0,     0,     2,    0 )    \
  visitOp(0x39,           f64_store,      10,     0,     0,     2,    0 )    \
  visitOp(0x3A,          i32_store8,      10,     0,     0,     2,    0 )    \
  visitOp(0x3B,         i32_store16,      10,     0,     0,     2,    0 )    \
  visitOp(0x3C,          i64_store8,      10,     0,     0,     2,    0 )    \
  visitOp(0x3D,         i64_store16,      10,     0,     0,     2,    0 )    \
  visitOp(0x3E,         i64_store32,      10,     0,     0,     2,    0 )    \
  visitOp(0x3F,         memory_size,      10,     1,     0,     0,    0 )    \
  visitOp(0x40,         memory_grow,     -30,     1,     0,     0,    0 )    \
  visitOp(0xFC08,       memory_init,     -10,     1,     0,     1,    0 )    \
  visitOp(0xFC09,         data_drop,      10,     1,     0,     0,    0 )    \
  visitOp(0xFC0A,       memory_copy,     -10,     2,     0,     0,    0 )    \
  visitOp(0xFC0B,       memory_fill,     -10,     1,     0,     0,    0 )

#define ENUMERATE_FLOATING_POINT_CONST_OPCODES(visitOp) \
     /*(  code,        name,    gas, bytes, uvecs,  u32s,  u64 ) */ \
  visitOp(0x43,           f32_const,      10,     4,     0,     0,    0 ) \
  visitOp(0x44,           f64_const,      10,     8,     0,     0,    0 )

#define ENUMERATE_NUMERIC_CONST_OPCODES(visitOp) \
     /*(  code,        name,    gas, bytes, uvecs,  u32s,  u64 ) */ \
  visitOp(0x41,           i32_const,      10,     0,     0,     1,    0 ) \
  visitOp(0x42,           i64_const,      10,     0,     0,     1,    1 ) \
  ENUMERATE_FLOATING_POINT_CONST_OPCODES(visitOp)

#define ENUMERATE_FLOATING_POINT_OPCODES(visitOp) \
  visitOp(0x5B,              f32_eq,      10,     0,     0,     0,    0 ) \
  visitOp(0x5C,              f32_ne,      10,     0,     0,     0,    0 ) \
  visitOp(0x5D,              f32_lt,      10,     0,     0,     0,    0 ) \
  visitOp(0x5E,              f32_gt,      10,     0,     0,     0,    0 ) \
  visitOp(0x5F,              f32_le,      10,     0,     0,     0,    0 ) \
  visitOp(0x60,              f32_ge,      10,     0,     0,     0,    0 ) \
  visitOp(0x61,              f64_eq,      10,     0,     0,     0,    0 ) \
  visitOp(0x62,              f64_ne,      10,     0,     0,     0,    0 ) \
  visitOp(0x63,              f64_lt,      10,     0,     0,     0,    0 ) \
  visitOp(0x64,              f64_gt,      10,     0,     0,     0,    0 ) \
  visitOp(0x65,              f64_le,      10,     0,     0,     0,    0 ) \
  visitOp(0x66,              f64_ge,      10,     0,     0,     0,    0 ) \
  visitOp(0x8B,             f32_abs,      10,     0,     0,     0,    0 )    \
  visitOp(0x8C,             f32_neg,      10,     0,     0,     0,    0 )    \
  visitOp(0x8D,            f32_ceil,      10,     0,     0,     0,    0 )    \
  visitOp(0x8E,           f32_floor,      10,     0,     0,     0,    0 )    \
  visitOp(0x8F,           f32_trunc,      10,     0,     0,     0,    0 )    \
  visitOp(0x90,         f32_nearest,      10,     0,     0,     0,    0 )    \
  visitOp(0x91,            f32_sqrt,      10,     0,     0,     0,    0 )    \
  visitOp(0x92,             f32_add,      10,     0,     0,     0,    0 )    \
  visitOp(0x93,             f32_sub,      10,     0,     0,     0,    0 )    \
  visitOp(0x94,             f32_mul,      10,     0,     0,     0,    0 )    \
  visitOp(0x95,             f32_div,      10,     0,     0,     0,    0 )    \
  visitOp(0x96,             f32_min,      10,     0,     0,     0,    0 )    \
  visitOp(0x97,             f32_max,      10,     0,     0,     0,    0 )    \
  visitOp(0x98,        f32_copysign,      10,     0,     0,     0,    0 )    \
  visitOp(0x99,             f64_abs,      10,     0,     0,     0,    0 )    \
  visitOp(0x9A,             f64_neg,      10,     0,     0,     0,    0 )    \
  visitOp(0x9B,            f64_ceil,      10,     0,     0,     0,    0 )    \
  visitOp(0x9C,           f64_floor,      10,     0,     0,     0,    0 )    \
  visitOp(0x9D,           f64_trunc,      10,     0,     0,     0,    0 )    \
  visitOp(0x9E,         f64_nearest,      10,     0,     0,     0,    0 )    \
  visitOp(0x9F,            f64_sqrt,      10,     0,     0,     0,    0 )    \
  visitOp(0xA0,             f64_add,      10,     0,     0,     0,    0 )    \
  visitOp(0xA1,             f64_sub,      10,     0,     0,     0,    0 )    \
  visitOp(0xA2,             f64_mul,      10,     0,     0,     0,    0 )    \
  visitOp(0xA3,             f64_div,      10,     0,     0,     0,    0 )    \
  visitOp(0xA4,             f64_min,      10,     0,     0,     0,    0 )    \
  visitOp(0xA5,             f64_max,      10,     0,     0,     0,    0 )    \
  visitOp(0xA6,        f64_copysign,      10,     0,     0,     0,    0 )    \
  visitOp(0xA7,        i32_wrap_u64,      10,     0,     0,     0,    0 )    \
  visitOp(0xA8,     i32_trunc_f32_s,      10,     0,     0,     0,    0 )    \
  visitOp(0xA9,     i32_trunc_f32_u,      10,     0,     0,     0,    0 )    \
  visitOp(0xAA,     i32_trunc_f64_s,      10,     0,     0,     0,    0 )    \
  visitOp(0xAB,     i32_trunc_f64_u,      10,     0,     0,     0,    0 )    \
  visitOp(0xAC,    i64_extend_i32_s,      10,     0,     0,     0,    0 )   \
  visitOp(0xAD,    i64_extend_i32_u,      10,     0,     0,     0,    0 )   \
  visitOp(0xAE,     i64_trunc_f32_s,      10,     0,     0,     0,    0 )   \
  visitOp(0xAF,     i64_trunc_f32_u,      10,     0,     0,     0,    0 )   \
  visitOp(0xB0,     i64_trunc_f64_s,      10,     0,     0,     0,    0 )   \
  visitOp(0xB1,     i64_trunc_f64_u,      10,     0,     0,     0,    0 )   \
  visitOp(0xB2,   f32_convert_i32_s,      10,     0,     0,     0,    0 )   \
  visitOp(0xB3,   f32_convert_i32_u,      10,     0,     0,     0,    0 )   \
  visitOp(0xB4,   f32_convert_i64_s,      10,     0,     0,     0,    0 )   \
  visitOp(0xB5,   f32_convert_i64_u,      10,     0,     0,     0,    0 )   \
  visitOp(0xB6,      f32_demote_f64,      10,     0,     0,     0,    0 )   \
  visitOp(0xB7,   f64_convert_i32_s,      10,     0,     0,     0,    0 )   \
  visitOp(0xB8,   f64_convert_i32_u,      10,     0,     0,     0,    0 )   \
  visitOp(0xB9,   f64_convert_i64_s,      10,     0,     0,     0,    0 )   \
  visitOp(0xBA,   f64_convert_i64_u,      10,     0,     0,     0,    0 )   \
  visitOp(0xBB,     f64_promote_f32,      10,     0,     0,     0,    0 )   \
  visitOp(0xBC, i32_reinterpret_f32,      10,     0,     0,     0,    0 )   \
  visitOp(0xBD, i64_reinterpret_f64,      10,     0,     0,     0,    0 )   \
  visitOp(0xBE, f32_reinterpret_i32,      10,     0,     0,     0,    0 )   \
  visitOp(0xBF, f64_reinterpret_i64,      10,     0,     0,     0,    0 )   \
  visitOp(0xFC00, i32_trunc_sat_f32_s,    10,     0,     0,     0,    0 )   \
  visitOp(0xFC01, i32_trunc_sat_f32_u,    10,     0,     0,     0,    0 )   \
  visitOp(0xFC02, i32_trunc_sat_f64_s,    10,     0,     0,     0,    0 )   \
  visitOp(0xFC03, i32_trunc_sat_f64_u,    10,     0,     0,     0,    0 )   \
  visitOp(0xFC04, i64_trunc_sat_f32_s,    10,     0,     0,     0,    0 )   \
  visitOp(0xFC05, i64_trunc_sat_f32_u,    10,     0,     0,     0,    0 )   \
  visitOp(0xFC06, i64_trunc_sat_f64_s,    10,     0,     0,     0,    0 )   \
  visitOp(0xFC07, i64_trunc_sat_f64_u,    10,     0,     0,     0,    0 )

#define ENUMERATE_NUMERIC_OPCODES(visitOp) \
     /*(  code,        name,    gas, bytes, uvecs,  u32s,  u64 ) */ \
  ENUMERATE_NUMERIC_CONST_OPCODES(visitOp) \
  visitOp(0x45,             i32_eqz,      10,     0,     0,     0,    0 ) \
  visitOp(0x46,              i32_eq,      10,     0,     0,     0,    0 ) \
  visitOp(0x47,              i32_ne,      10,     0,     0,     0,    0 ) \
  visitOp(0x48,            i32_lt_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x49,            i32_lt_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x4A,            i32_gt_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x4B,            i32_gt_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x4C,            i32_le_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x4D,            i32_le_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x4E,            i32_ge_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x4F,            i32_ge_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x50,             i64_eqz,      10,     0,     0,     0,    0 ) \
  visitOp(0x51,              i64_eq,      10,     0,     0,     0,    0 ) \
  visitOp(0x52,              i64_ne,      10,     0,     0,     0,    0 ) \
  visitOp(0x53,            i64_lt_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x54,            i64_lt_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x55,            i64_gt_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x56,            i64_gt_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x57,            i64_le_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x58,            i64_le_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x59,            i64_ge_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x5A,            i64_ge_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x67,             i32_clz,      10,     0,     0,     0,    0 ) \
  visitOp(0x68,             i32_ctz,      10,     0,     0,     0,    0 ) \
  visitOp(0x69,          i32_popcnt,      10,     0,     0,     0,    0 ) \
  visitOp(0x6A,             i32_add,      10,     0,     0,     0,    0 ) \
  visitOp(0x6B,             i32_sub,      10,     0,     0,     0,    0 ) \
  visitOp(0x6C,             i32_mul,      10,     0,     0,     0,    0 ) \
  visitOp(0x6D,           i32_div_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x6E,           i32_div_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x6F,           i32_rem_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x70,           i32_rem_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x71,             i32_and,      10,     0,     0,     0,    0 ) \
  visitOp(0x72,              i32_or,      10,     0,     0,     0,    0 ) \
  visitOp(0x73,             i32_xor,      10,     0,     0,     0,    0 ) \
  visitOp(0x74,             i32_shl,      10,     0,     0,     0,    0 ) \
  visitOp(0x75,           i32_shr_s,      10,     0,     0,     0,    0 ) \
  visitOp(0x76,           i32_shr_u,      10,     0,     0,     0,    0 ) \
  visitOp(0x77,            i32_rotl,      10,     0,     0,     0,    0 ) \
  visitOp(0x78,            i32_rotr,      10,     0,     0,     0,    0 )    \
  visitOp(0x79,             i64_clz,      10,     0,     0,     0,    0 )    \
  visitOp(0x7A,             i64_ctz,      10,     0,     0,     0,    0 )    \
  visitOp(0x7B,          i64_popcnt,      10,     0,     0,     0,    0 )    \
  visitOp(0x7C,             i64_add,      10,     0,     0,     0,    0 )    \
  visitOp(0x7D,             i64_sub,      10,     0,     0,     0,    0 )    \
  visitOp(0x7E,             i64_mul,      10,     0,     0,     0,    0 )    \
  visitOp(0x7F,           i64_div_s,      10,     0,     0,     0,    0 )    \
  visitOp(0x80,           i64_div_u,      10,     0,     0,     0,    0 )    \
  visitOp(0x81,           i64_rem_s,      10,     0,     0,     0,    0 )    \
  visitOp(0x82,           i64_rem_u,      10,     0,     0,     0,    0 )    \
  visitOp(0x83,             i64_and,      10,     0,     0,     0,    0 )    \
  visitOp(0x84,              i64_or,      10,     0,     0,     0,    0 )    \
  visitOp(0x85,             i64_xor,      10,     0,     0,     0,    0 )    \
  visitOp(0x86,             i64_shl,      10,     0,     0,     0,    0 )    \
  visitOp(0x87,           i64_shr_s,      10,     0,     0,     0,    0 )    \
  visitOp(0x88,           i64_shr_u,      10,     0,     0,     0,    0 )    \
  visitOp(0x89,            i64_rotl,      10,     0,     0,     0,    0 )    \
  visitOp(0x8A,            i64_rotr,      10,     0,     0,     0,    0 )    \
  visitOp(0xC0,       i32_extend8_s,      10,     0,     0,     0,    0 )    \
  visitOp(0xC1,      i32_extend16_s,      10,     0,     0,     0,    0 )    \
  visitOp(0xC2,       i64_extend8_s,      10,     0,     0,     0,    0 )    \
  visitOp(0xC3,      i64_extend16_s,      10,     0,     0,     0,    0 )    \
  visitOp(0xC4,      i64_extend32_s,      10,     0,     0,     0,    0 )    \
  ENUMERATE_FLOATING_POINT_OPCODES(visitOp)

#define ENUMERATE_ALL_SHORT_OPCODES(visitOp) \
  ENUMERATE_CONTROL_OPCODES(visitOp)         \
  ENUMERATE_REFERENCE_OPCODES(visitOp)       \
  ENUMERATE_PARAMETRIC_OPCODES(visitOp)      \
  ENUMERATE_VARIABLE_ACCESS_OPCODES(visitOp) \
  ENUMERATE_MEMORY_ACCESS_OPCODES(visitOp)   \
  ENUMERATE_TABLE_ACCESS_OPCODES(visitOp)    \
  ENUMERATE_NUMERIC_OPCODES(visitOp)

#define ENUMERATE_NON_BRANCHING_OPCODES(visitOp) \
  ENUMERATE_REFERENCE_OPCODES(visitOp)           \
  ENUMERATE_PARAMETRIC_OPCODES(visitOp)          \
  ENUMERATE_VARIABLE_ACCESS_OPCODES(visitOp)     \
  ENUMERATE_MEMORY_ACCESS_OPCODES(visitOp)       \
  ENUMERATE_TABLE_ACCESS_OPCODES(visitOp)        \
  ENUMERATE_NUMERIC_OPCODES(visitOp)

#define ENUMERATE_EXTENSION_OPCODES(visitOp) \
   /*(  code,        name,    gas, bytes, uvecs,  u32s, u64s ) */ \
  visitOp(0xFC,                 ext,      -1,     0,     0,     0,    0 ) \
  visitOp(0xFD,             vec_ext,      -1,     0,     0,     0,    0 )

#define MEMORY_GROW_GAS_FACTOR 100
#define MEMORY_GROW_GAS_DIVIDER 1

#define MEMORY_FILL_GAS_FACTOR 1
#define MEMORY_FILL_GAS_DIVIDER 1000

#define MEMORY_COPY_GAS_FACTOR 2
#define MEMORY_COPY_GAS_DIVIDER 1000

#define MEMORY_INIT_GAS_FACTOR 2
#define MEMORY_INIT_GAS_DIVIDER 1000

#define TABLE_FILL_GAS_FACTOR 1
#define TABLE_FILL_GAS_DIVIDER 1

#define TABLE_COPY_GAS_FACTOR 2
#define TABLE_COPY_GAS_DIVIDER 1

#define TABLE_INIT_GAS_FACTOR 2
#define TABLE_INIT_GAS_DIVIDER 1

#define TABLE_GROW_GAS_FACTOR 5
#define TABLE_GROW_GAS_DIVIDER 1

