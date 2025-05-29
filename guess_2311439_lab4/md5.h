#include <iostream>
#include <string>
#include <cstring>
#include <arm_neon.h>

using namespace std;

// 定义了Byte，便于使用
typedef unsigned char Byte;
// 定义了32比特
typedef unsigned int bit32;

// MD5的一系列参数。参数是固定的，其实你不需要看懂这些
#define s11 7
#define s12 12
#define s13 17
#define s14 22
#define s21 5
#define s22 9
#define s23 14
#define s24 20
#define s31 4
#define s32 11
#define s33 16
#define s34 23
#define s41 6
#define s42 10
#define s43 15
#define s44 21

/**
 * @Basic MD5 functions.
 *
 * @param there bit32.
 *
 * @return one bit32.
 */
// 定义了一系列MD5中的具体函数
// 这四个计算函数是需要你进行SIMD并行化的
// 可以看到，FGHI四个函数都涉及一系列位运算，在数据上是对齐的，非常容易实现SIMD的并行化

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))


static inline uint32x4_t F_NEON(uint32x4_t x, uint32x4_t y, uint32x4_t z)   // F(x,y,z) = (x & y) | (~x & z)
{
  uint32x4_t xy = vandq_u32(x, y);      // x & y
  uint32x4_t notx = vmvnq_u32(x);       // ~x
  uint32x4_t notx_z = vandq_u32(notx, z); // ~x & z
  return vorrq_u32(xy, notx_z);         // (x&y) | (~x&z)
}

static inline uint32x4_t G_NEON(uint32x4_t x, uint32x4_t y, uint32x4_t z)   // G(x,y,z) = (x & z) | (y & ~z)
 {
  uint32x4_t xz = vandq_u32(x, z);      // x & z
  uint32x4_t notz = vmvnq_u32(z);       // ~z
  uint32x4_t y_notz = vandq_u32(y, notz); // y & ~z
  return vorrq_u32(xz, y_notz);          // (x&z) | (y&~z)
}

static inline uint32x4_t H_NEON(uint32x4_t x, uint32x4_t y, uint32x4_t z)     // H(x,y,z) = x ^ y ^ z
{
  uint32x4_t xy_xor = veorq_u32(x, y);  // x ^ y
  return veorq_u32(xy_xor, z);          // (x^y)^z
}

static inline uint32x4_t I_NEON(uint32x4_t x, uint32x4_t y, uint32x4_t z)      // I(x,y,z) = y ^ (x | ~z)
 {
  uint32x4_t notz = vmvnq_u32(z);       // ~z
  uint32x4_t x_or_notz = vorrq_u32(x, notz); // x | ~z
  return veorq_u32(y, x_or_notz);       // y ^ (x|~z)
 }


/**
 * @Rotate Left. 
 *
 * @param {num} the raw number.
 *
 * @param {n} rotate left n.
 *
 * @return the number after rotated left.
 */
// 定义了一系列MD5中的具体函数
// 这五个计算函数（ROTATELEFT/FF/GG/HH/II）和之前的FGHI一样，都是需要你进行SIMD并行化的
// 但是你需要注意的是#define的功能及其效果，可以发现这里的FGHI是没有返回值的，为什么呢？你可以查询#define的含义和用法
#define ROTATELEFT(num, n) (((num) << (n)) | ((num) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
  (a) += F ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}

#define GG(a, b, c, d, x, s, ac) { \
  (a) += G ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
  (a) += H ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
  (a) += I ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}

static inline uint32x4_t rotate_left_neon(uint32x4_t vec, const int n) {
  return vorrq_u32(vshlq_n_u32(vec, n), vshrq_n_u32(vec, 32 - n));
}

static inline void FF_NEON(uint32x4_t* a, uint32x4_t* b, uint32x4_t* c, uint32x4_t* d,uint32x4_t x, const int s, const uint32_t ac) {
uint32x4_t ac_vec = vdupq_n_u32(ac);
*a = vaddq_u32(*a, F_NEON(*b, *c, *d));
*a = vaddq_u32(*a, x);
*a = vaddq_u32(*a, ac_vec);
*a = rotate_left_neon(*a, s);
*a = vaddq_u32(*a, *b);
}

static inline void GG_NEON(uint32x4_t* a, uint32x4_t* b, uint32x4_t* c, uint32x4_t* d,uint32x4_t x, const int s, const uint32_t ac) {
uint32x4_t ac_vec = vdupq_n_u32(ac);
*a = vaddq_u32(*a, G_NEON(*b, *c, *d));
*a = vaddq_u32(*a, x);
*a = vaddq_u32(*a, ac_vec);
*a = rotate_left_neon(*a, s);
*a = vaddq_u32(*a, *b);
}

static inline void HH_NEON(uint32x4_t* a, uint32x4_t* b, uint32x4_t* c, uint32x4_t* d,uint32x4_t x, const int s, const uint32_t ac) {
uint32x4_t ac_vec = vdupq_n_u32(ac);
*a = vaddq_u32(*a, H_NEON(*b, *c, *d));
*a = vaddq_u32(*a, x);
*a = vaddq_u32(*a, ac_vec);
*a = rotate_left_neon(*a, s);
*a = vaddq_u32(*a, *b);
}

static inline void II_NEON(uint32x4_t* a, uint32x4_t* b, uint32x4_t* c, uint32x4_t* d,uint32x4_t x, const int s, const uint32_t ac) {
uint32x4_t ac_vec = vdupq_n_u32(ac);
*a = vaddq_u32(*a, I_NEON(*b, *c, *d));
*a = vaddq_u32(*a, x);
*a = vaddq_u32(*a, ac_vec);
*a = rotate_left_neon(*a, s);
*a = vaddq_u32(*a, *b);
}

void MD5Hash(string input, bit32 *state);

void My_MD5Hash(string input[4], bit32 state[4][4]);