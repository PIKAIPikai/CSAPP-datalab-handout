/* PIKAI
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * it, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff. 注意这里的限制，不能使用大于 255 的常数，可以通过位运算来构造更大的常数。
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
      cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
      than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
      cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider 
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  // 利用德摩根定律：A & B = ~(~A | ~B)
  return ~(~x | ~y);
}

/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  // 先将目标字节右移至最低位，再用0xFF掩码提取该字节
  int shift_bits = n << 3; // 等价于 n * 8
  return (x >> shift_bits) & 0xFF;
}

/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  // C语言默认算术右移，需构造掩码清除高位符号扩展
  int arith_shift = x >> n;
  int clear_mask = ~(((1 << 31) >> n) << 1);
  return arith_shift & clear_mask;
}

/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // 分治法：两两分组统计，逐步合并结果
  int m1 = 0x55 | (0x55 << 8);  // 01010101...
  int m2 = 0x33 | (0x33 << 8);  // 00110011...
  int m3 = 0x0F | (0x0F << 8);  // 00001111...
  int m4 = 0xFF | (0xFF << 16); // 0000000011111111...
  int m5 = 0xFF | (0xFF << 8);  // 00000000000000001111111111111111...

  m1 = m1 | (m1 << 16);
  m2 = m2 | (m2 << 16);
  m3 = m3 | (m3 << 16);

  x = (x & m1) + ((x >> 1) & m1); // 每2位统计
  x = (x & m2) + ((x >> 2) & m2); // 每4位统计
  x = (x & m3) + ((x >> 4) & m3); // 每8位统计
  x = (x & m4) + ((x >> 8) & m4); // 每16位统计
  x = (x & m5) + ((x >> 16) & m5); // 合并为32位统计结果

  return x;
}

/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  // 0的相反数仍是0，非零数与其相反数必有一个符号位为1
  int neg_x = ~x + 1;
  int sign_or = (x | neg_x) >> 31;
  return sign_or + 1;
}

/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // 32位补码最小值：最高位为1，其余为0
  return 1 << 31;
}

/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  // 通过左移再右移检验符号扩展是否一致
  int shift_val = 33 + ~n; // 等价于 32 - n
  int restored_x = (x << shift_val) >> shift_val;
  int n_not_32 = !!(n ^ 32); // n=32时特殊处理
  return !(x ^ restored_x) & n_not_32;
}

/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  // 负数除法需加偏置保证向零取整
  int sign_bit = x >> 31;
  int round_bias = sign_bit & ((1 << n) + ~0);
  return (x + round_bias) >> n;
}

/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  // 补码相反数：按位取反加1
  return ~x + 1;
}

/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  // 符号位为0且非零
  int sign_zero = !(x >> 31);
  int not_zero = !!x;
  return sign_zero & not_zero;
}

/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // 分三种情况：x负y正、同号且x-y负、x等于y
  int x_sign = x >> 31;
  int y_sign = y >> 31;
  int y_neg = ~y + 1;
  int diff = x + y_neg;
  int diff_sign = diff >> 31;

  int case1 = x_sign & !y_sign;          // x负y正
  int case2 = !(x_sign ^ y_sign) & diff_sign; // 同号且差为负
  int case3 = !(x ^ y);                   // 相等

  return case1 | case2 | case3;
}

/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  // 二分查找最高位1的位置
  int pos = 0;
  int step;

  // 检查高16位
  step = (!!(x >> 16)) << 4;
  pos |= step;
  x >>= step;

  // 检查剩余的高8位
  step = (!!(x >> 8)) << 3;
  pos |= step;
  x >>= step;

  // 检查剩余的高4位
  step = (!!(x >> 4)) << 2;
  pos |= step;
  x >>= step;

  // 检查剩余的高2位
  step = (!!(x >> 2)) << 1;
  pos |= step;
  x >>= step;

  // 检查最后1位
  step = !!(x >> 1);
  pos |= step;

  return pos;
}

/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  unsigned exponent = (uf >> 23) & 0xFF;
  unsigned fraction = uf & 0x7FFFFF;

  // NaN直接返回
  if ((exponent == 0xFF) && (fraction != 0)) {
    return uf;
  }
  // 翻转符号位
  return uf ^ (1 << 31);
}

/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  unsigned sign_bit = x & (1 << 31);
  unsigned abs_x;
  unsigned exponent = 158; // 127 + 31
  unsigned fraction;

  // 特殊情况：0和Tmin
  if (!x) return 0;
  if (x == (1 << 31)) return (158 << 23) | sign_bit;

  // 取绝对值
  abs_x = sign_bit ? -x : x;

  // 归一化：找到最高位1
  while (!(abs_x & (1 << 31))) {
    abs_x <<= 1;
    exponent--;
  }

  // 提取尾数并处理舍入
  fraction = (abs_x >> 8) & 0x7FFFFF;
  abs_x &= 0xFF;

  // 舍入判断
  if (abs_x > 0x80 || (abs_x == 0x80 && (fraction & 1))) {
    fraction++;
    // 处理尾数溢出
    if (fraction >> 23) {
      fraction = 0;
      exponent++;
    }
  }

  return sign_bit | (exponent << 23) | fraction;
}

/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  unsigned sign_bit = uf & (1 << 31);
  unsigned exponent = (uf >> 23) & 0xFF;
  unsigned fraction = uf & 0x7FFFFF;

  // NaN或无穷直接返回
  if (exponent == 0xFF) {
    return uf;
  }

  if (exponent == 0) {
    // 非规格化数：左移尾数
    fraction <<= 1;
    // 检查是否转为规格化数
    if (fraction & 0x800000) {
      fraction &= 0x7FFFFF;
      exponent = 1;
    }
  } else {
    // 规格化数：指数加1
    exponent++;
    // 检查是否溢出为无穷
    if (exponent == 0xFF) {
      fraction = 0;
    }
  }

  return sign_bit | (exponent << 23) | fraction;
}