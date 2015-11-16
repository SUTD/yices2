/*
 * The Yices SMT Solver. Copyright 2014 SRI International.
 *
 * This program may only be used subject to the noncommercial end user
 * license agreement which is downloadable along with this program.
 */

/*
 * YICES API
 */

#ifndef __YICES_H
#define __YICES_H

/*
 * On windows/cygwin/mingw:
 *
 *   __YICES_DLLSPEC__ is '__declspec(dllimport) by default
 *
 * This can be overridden as follows:
 *
 * 1) give -DNOYICES_DLL as a compilation flag (if you want to
 *    link with libyices.a rather than the DLL)
 *
 * 2) define __YICES_DLLSPEC__ to '__declspec(dllexport)' before
 *      #include "yices.h"
 *    when building yices.
 *
 * On any system other than Windows: __YICES_DLLSPEC__ is empty.
 */
#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(NOYICES_DLL)
#undef __YICES_DLLSPEC__
#define __YICES_DLLSPEC__
#else
#if ! defined(__YICES_DLLSPEC__)
#define __YICES_DLLSPEC__ __declspec(dllimport)
#endif
#endif
#else
#define __YICES_DLLSPEC__
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*
 * WARNING: yices requires the header file <stdint.h>
 *
 * It's not available in Microsoft Visual Studio (prior to Visual Studio 2010),
 * and it's possibly missing from other compilers too.
 *
 * If necessary, there are open-source 'stdint.h' that can
 * be downloaded at
 *      http://code.google.com/p/msinttypes/   (for MS Visual Studio only)
 *   or http://www.azillionmonkeys.com/qed/pstdint.h
 */
#include <stdint.h>
#include <stdio.h>

#include "yices_types.h"
#include "yices_limits.h"



/*********************
 *  VERSION NUMBERS  *
 ********************/

#define __YICES_VERSION            2
#define __YICES_VERSION_MAJOR      4
#define __YICES_VERSION_PATCHLEVEL 1


/*
 * The version as a string "x.y.z"
 */
__YICES_DLLSPEC__ extern const char *yices_version;


/*
 * More details about the release:
 * - build_arch is a string like "x86_64-unknown-linux-gnu"
 * - build_mode is "release" or "debug"
 * - build_date is the compilation date as in "2014-01-27"
 */
__YICES_DLLSPEC__ extern const char *yices_build_arch;
__YICES_DLLSPEC__ extern const char *yices_build_mode;
__YICES_DLLSPEC__ extern const char *yices_build_date;



/***************************************
 *  GLOBAL INITIALIZATION AND CLEANUP  *
 **************************************/

/*
 * This function must be called before anything else to initialize
 * internal data structures.
 */
__YICES_DLLSPEC__ extern void yices_init(void);


/*
 * Delete all internal data structures and objects
 * - this must be called to avoid memory leaks
 */
__YICES_DLLSPEC__ extern void yices_exit(void);


/*
 * Full reset
 * - delete all terms and types and reset the symbol tables
 * - delete all contexts, models, configuration descriptors and
 *   parameter records.
 */
__YICES_DLLSPEC__ extern void yices_reset(void);


/*
 * Delete a string returned by yices
 *
 * Several functions construct and return a string:
 * - yices_error_string
 * - yices_type_to_string
 * - yices_term_to_string
 * - yices_model_to_string
 *
 * The returned string must be freed when it's no longer used by
 * calling this function.
 */
__YICES_DLLSPEC__ extern void yices_free_string(char *s);



/*********************
 *  ERROR REPORTING  *
 ********************/

/*
 * Error codes and the error_report data structure are defined in
 * yices_types.h. When an API function is called with invalid
 * arguments or when some error occurs for whatever reason, then the
 * function returns a specific value (typically a negative value) and
 * stores information about the error in a global error_report
 * structure.  This structure can be examined by calling
 * yices_error_report().  The most important component of the
 * error_report is an error code that is returned by a call to
 * yices_error_code().
 */

/*
 * Get the last error code
 */
__YICES_DLLSPEC__ extern error_code_t yices_error_code(void);


/*
 * Get the last error report
 */
__YICES_DLLSPEC__ extern error_report_t *yices_error_report(void);


/*
 * Clear the error report
 */
__YICES_DLLSPEC__ extern void yices_clear_error(void);


/*
 * Print an error message on stream f.  This converts the current error
 * code + error report structure into an error message.
 * - f must be a non-NULL open stream (writable)
 *
 * Return -1 if there's an error while writing to f (as reported by fprintf).
 * Return 0 otherwise.
 *
 * If there's an error, errno, perror, and friends can be used for diagnostic.
 */
__YICES_DLLSPEC__ extern int32_t yices_print_error(FILE *f);


/*
 * Build a string from the current error code + error report structure.
 *
 * The returned string must be freed when no-longer used by calling
 * yices_free_string.
 */
__YICES_DLLSPEC__ extern char *yices_error_string(void);



/*
 * Register a callback function to invoke if Yices runs out of memory.
 * The callback function takes no arguments and returns nothing.
 *
 * By default, there's no callback. When Yices runs out of memory, it
 * first prints an error message on stderr; then it calls
 * exit(YICES_EXIT_OUT_OF_MEMORY).  This kills the whole process.
 *
 * Installing an out-of-memory callback allows you to do
 * something a bit less brutal. If there's a callback,
 * yices will call it first when it runs out of memory.
 * The callback function should not return. If it does,
 * yices will call exit as previously.
 *
 * In other words, the code that handles out-of-memory is as follows:
 *
 *   if (callback != NULL) {
 *     callback();
 *   } else {
 *     fprintf(stderr, ...);
 *   }
 *   exit(YICES_EXIT_OUT_OF_MEMORY);
 *
 *
 * IMPORTANT
 * ---------
 * After Yices runs out of memory, its internal data structures may be
 * left in an inconsistent/corrupted state. The API is effectively
 * unusable at this point and nothing can be done to recover cleanly.
 * Evan a call to yices_exit() may cause a seg fault. The callback
 * should not try to cleanup anything, or call any function from the API.
 * 
 * A plausible use of this callback feature is to implement an
 * exception mechanism using setjmp/longjmp.
 */
__YICES_DLLSPEC__ extern void yices_set_out_of_mem_callback(void (*callback)(void));



/********************************
 *  VECTORS OF TERMS AND TYPES  *
 *******************************/

/*
 * Some functions in the API return arrays of terms in a vector object
 * (i.e., a resizable array). The vector structures are defined in
 * yices_types.h:
 * - v.size = number of elements in the array 
 * - v.data = the array proper: 
 *   the elements are stored in v.data[0]  .... v.data[n-1] 
 *   where n = v.size.
 *
 * Before calling any function that fills in a term vector, the vector
 * object must be initialized via yices_init_term_vector. To prevent
 * memory leaks, it must be deleted when no longer needed.
 */

/*
 * Initialize a term vector v
 */
__YICES_DLLSPEC__ extern void yices_init_term_vector(term_vector_t *v);

/*
 * Delete vector v
 */
__YICES_DLLSPEC__ extern void yices_delete_term_vector(term_vector_t *v);


/*
 * Reset: empty the vector (reset size to 0)
 */
__YICES_DLLSPEC__ extern void yices_reset_term_vector(term_vector_t *v);



/***********************
 *  TYPE CONSTRUCTORS  *
 **********************/

/*
 * All constructors return NULL_TYPE (-1) if the type definition is wrong.
 */

/*
 * Built-in type: bool
 */
__YICES_DLLSPEC__ extern type_t yices_bool_type(void);


/*
 * Bitvectors of given size (number of bits)
 * Requires size > 0
 *
 * If size = 0, the error report is set
 *   code = POS_INT_REQUIRED
 *   badval = size
 * If size > YICES_MAX_BVSIZE
 *   code = MAX_BVSIZE_EXCEEDED
 *   badval = size
 */
__YICES_DLLSPEC__ extern type_t yices_bv_type(uint32_t size);




/*************************
 *   TYPE EXPLORATION    *
 ************************/

/*
 * Checks on a type tau:
 * - all functions return 0 for false, 1 for true
 *
 * yices_type_is_arithmetic(tau) returns true if tau is either int or real.
 *
 * if tau not a valid type, the functions return false
 * and set the error report:
 *   code = INVALID_TYPE
 *   type1 = tau
 */
__YICES_DLLSPEC__ extern int32_t yices_type_is_bool(type_t tau);
__YICES_DLLSPEC__ extern int32_t yices_type_is_bitvector(type_t tau);


/*
 * Number of bits for type tau
 * - returns 0 if there's an error
 *
 * Error report:
 * if tau is not a valid type
 *    code = INVALID_TYPE
 *    type1 = tau
 * if tau is not a bitvector type
 *    code = BVTYPE_REQUIRED
 *    type1 = tau
 */
__YICES_DLLSPEC__ extern uint32_t yices_bvtype_size(type_t tau);




/*******************************
 *  BOOELAN TERM CONSTRUCTORS  *
 ******************************/

/*
 * Constructors do type checking and simplification.
 * They return NULL_TERM (< 0) if there's a type error.
 */

/*
 * Boolean constants: no error report
 */
__YICES_DLLSPEC__ extern term_t yices_true(void);
__YICES_DLLSPEC__ extern term_t yices_false(void);


/*
 * Uninterpreted term of type tau
 *
 * Error report:
 * if tau is undefined
 *   code = INVALID_TYPE
 *   type1 = tau
 */
__YICES_DLLSPEC__ extern term_t yices_new_uninterpreted_term(type_t tau);


/*
 * if-then-else
 *
 * Error report:
 * if cond, then_term, or else_term is not a valid term
 *   code = INVALID_TERM
 *   term1 = whichever of cond, then_term, or else_term is invalid
 * if cond is not boolean
 *   code = TYPE_MISMATCH
 *   term1 = cond
 *   type1 = bool (expected type)
 * if then_term and else_term have incompatible types
 *   code = INCOMPATIBLE_TYPES
 *   term1 = then_term
 *   type1 = term1's type
 *   term2 = else_term
 *   type2 = term2's type
 */
__YICES_DLLSPEC__ extern term_t yices_ite(term_t cond, term_t then_term, term_t else_term);


/*
 * Equality (= left right)
 * Disequality (/= left right)
 *
 * Error report:
 * if left or right is not a valid term
 *   code = INVALID_TERM
 *   term1 = left or right
 * if left and right do not have compatible types
 *   code = INCOMPATIBLE_TYPES
 *   term1 = left
 *   type1 = term1's type
 *   term2 = right
 *   type2 = term2's type
 */
__YICES_DLLSPEC__ extern term_t yices_eq(term_t left, term_t right);
__YICES_DLLSPEC__ extern term_t yices_neq(term_t left, term_t right);


/*
 * (not arg)
 *
 * Error report:
 * if arg is invalid
 *    code = INVALID_TERM
 *    term1 = arg
 * if arg is not boolean
 *    code = TYPE_MISMATCH
 *    term1 = arg
 *    type1 = bool (expected type)
 */
__YICES_DLLSPEC__ extern term_t yices_not(term_t arg);


/*
 * (or  arg[0] ... arg[n-1])
 * (and arg[0] ... arg[n-1])
 * (xor arg[0] ... arg[n-1])
 *
 * NOTE: ARRAY ARG MAY BE MODIFIED.
 *
 * Error report:
 * if n > YICES_MAX_ARITY
 *   code = TOO_MANY_ARGUMENTS
 *   badval = n
 * if arg[i] is not a valid term
 *   code = INVALID_TERM
 *   term1 = arg[i]
 * if arg[i] is not boolean
 *   code = TYPE_MISMATCH
 *   term1 = arg[i]
 *   type1 = bool (expected type)
 */
__YICES_DLLSPEC__ extern term_t yices_or(uint32_t n, term_t arg[]);
__YICES_DLLSPEC__ extern term_t yices_and(uint32_t n, term_t arg[]);
__YICES_DLLSPEC__ extern term_t yices_xor(uint32_t n, term_t arg[]);


/*
 * Variants of or/and/xor with 2 or 3 arguments
 */
__YICES_DLLSPEC__ extern term_t yices_or2(term_t t1, term_t t2);
__YICES_DLLSPEC__ extern term_t yices_and2(term_t t1, term_t t2);
__YICES_DLLSPEC__ extern term_t yices_xor2(term_t t1, term_t t2);

__YICES_DLLSPEC__ extern term_t yices_or3(term_t t1, term_t t2, term_t t3);
__YICES_DLLSPEC__ extern term_t yices_and3(term_t t1, term_t t2, term_t t3);
__YICES_DLLSPEC__ extern term_t yices_xor3(term_t t1, term_t t2, term_t t3);


/*
 * (iff left right)
 * (implies left right)
 *
 * Error report:
 * if left or right is invalid
 *    code = INVALID_TERM
 *    term1 = left/right
 * if left or right is not boolean
 *    code = TYPE_MISMATCH
 *    term1 = left/right
 *    type1 = bool (expected type)
 */
__YICES_DLLSPEC__ extern term_t yices_iff(term_t left, term_t right);
__YICES_DLLSPEC__ extern term_t yices_implies(term_t left, term_t right);


/*
 * Distinct
 *
 * NOTE: ARG MANY BE MODIFIED
 *
 * Error report:
 * if n == 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if n > YICES_MAX_ARITY
 *    code = TOO_MANY_ARGUMENTS
 *    badval = n
 * if arg[i] is not a valid term
 *    code = INVALID_TERM
 *    term1 = arg[i]
 * if two terms arg[i] and arg[j] don't have compatible types
 *    code = INCOMPATIBLE_TYPES
 *    term1 = arg[i]
 *    type1 = term1's type
 *    term2 = arg[j]
 *    type2 = term2's type
 */
__YICES_DLLSPEC__ extern term_t yices_distinct(uint32_t n, term_t arg[]);



/*********************************
 *  BITVECTOR TERM CONSTRUCTORS  *
 ********************************/

/*
 * BITVECTOR CONSTANTS
 *
 * Constants can be constructed from C integers (32 or 64 bits),
 * from GMP integers, from arrays, or by parsing strings.
 *
 * The constant constructors return NULL_TERM (-1) if there's
 * an error and set the error report.
 */

/*
 * Conversion of an integer to a bitvector constant.
 * - n = number of bits
 * - x = value
 * The low-order bit of x is bit 0 of the constant.
 *
 * For yices_bvconst_uint32:
 * - if n is less than 32, then the value of x is truncated to 
 *   n bits (i.e., only the n least significant bits of x are considered) 
 * - if n is more than 32, then the value of x is zero-extended to
 *   n bits.
 *
 * For yices_bvconst_uint64:
 * - if n is less than 64, then the value of x is truncated to 
 *   n bits (i.e., only the n least significant bits of x are considered) 
 * - if n is more than 64, then the value of x is zero-extended to
 *   n bits.
 *
 * For yices_bvconst_int32:
 * - if n is less than 32, then the value of x is truncated to 
 *   n bits (i.e., only the n least significant bits of x are considered) 
 * - if n is more than 32, then the value of x is sign-extended to
 *   n bits.
 *
 * For yices_bvconst_int64:
 * - if n is less than 64, then the value of x is truncated to 
 *   n bits (i.e., only the n least significant bits of x are considered) 
 * - if n is more than 64, then the value of x is sign-extended to
 *   n bits.
 *
 * For yices_bvconst_mpz:
 * - x is interpreted as a signed number in 2-s complement
 * - if x has fewer than n bits (in 2's complement), then the value is sign-extended
 * - if x has more than n bits (in 2's complement) then the value is truncated
 *   (to n least significant bits).
 *
 * Error report:
 * if n = 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if n > YICES_MAX_BVSIZE
 *    code = MAX_BVSIZE_EXCEEDED
 *    badval = n
 */
__YICES_DLLSPEC__ extern term_t yices_bvconst_uint32(uint32_t n, uint32_t x);
__YICES_DLLSPEC__ extern term_t yices_bvconst_uint64(uint32_t n, uint64_t x);

__YICES_DLLSPEC__ extern term_t yices_bvconst_int32(uint32_t n, int32_t x);
__YICES_DLLSPEC__ extern term_t yices_bvconst_int64(uint32_t n, int64_t x);


#ifdef __GMP_H__
__YICES_DLLSPEC__ extern term_t yices_bvconst_mpz(uint32_t n, const mpz_t x);
#endif


/*
 * bvconst_zero: set all bits to 0
 * bvconst_one: set low-order bit to 1, all the other bits to 0
 * bvconst_minus_one: set all bits to 1
 *
 * Error report:
 * if n = 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if n > YICES_MAX_BVSIZE
 *    code = MAX_BVSIZE_EXCEEDED
 *    badval = n
 */
__YICES_DLLSPEC__ extern term_t yices_bvconst_zero(uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_bvconst_one(uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_bvconst_minus_one(uint32_t n);


/*
 * Construction from an integer array
 * bit i of the constant is 0 if a[i] == 0
 * bit i of the constant is 1 if a[i] != 0
 *
 * Error report:
 * if n = 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if n > YICES_MAX_BVSIZE
 *    code = MAX_BVSIZE_EXCEEDED
 *    badval = n
 */
__YICES_DLLSPEC__ extern term_t yices_bvconst_from_array(uint32_t n, const int32_t a[]);


/*
 * Parsing from a string of characters '0' and '1'
 * First character = high-order bit
 * Last character = low-order bit
 * The constant has n bits if the strings has n characters.
 *
 * Error report:
 * if the format is incorrect:
 *   code = INVALID_BVBIN_FORMAT
 * if the string has more than YICES_MAX_BVSIZE digits
 *   code = MAX_BVSIZE_EXCEEDED
 *   badval = n
 */
__YICES_DLLSPEC__ extern term_t yices_parse_bvbin(const char *s);

// same function under a different name for backward compatibility
__YICES_DLLSPEC__ extern term_t yices_bvconst_from_string(const char *s);


/*
 * Parsing from a hexadecimal string
 * All characters must be '0' to '9' or 'a' to 'f' or 'A' to 'F'
 * - First character = 4 high-order bits
 * - Last character = 4 low-order bits
 * The constant has 4n bits if s has n characters.
 *
 * Error report:
 * if the format is incorrect:
 *   code = INVALID_BVHEX_FORMAT
 * if the result would have more than YICES_MAX_BVSIZE digits
 *   code = MAX_BVSIZE_EXCEEDED
 *   badval = 4n
 */
__YICES_DLLSPEC__ extern term_t yices_parse_bvhex(const char *s);

// same function under a different name for backward compatibility
__YICES_DLLSPEC__ extern term_t yices_bvconst_from_hexa_string(const char *s);




/*
 * BIT-VECTOR ARITHMETIC
 */

/*
 * Binary operations: both arguments must be bitvector terms of the same size.
 * The functions return NULL_TERM (-1) if there's an error.
 *
 * Error reports
 * if t1 or t2 is not valid
 *   code = INVALID_TERM
 *   term1 = t1 or t2
 * if t1 or t2 is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t1 or t2
 * if t1 and t2 do not have the same bitvector type
 *   code = INCOMPATIBLE_TYPES
 *   term1 = t1
 *   type1 = type of t1
 *   term2 = t2
 *   type2 = type of t2
 *
 * For bvmul, bvsquare, or bvpower, if the degree is too large
 *   code = DEGREE_OVERFLOW
 *
 *
 * In case of division by 0, Yices uses the following conventions:
 *
 *   (bvdiv  x 0b00...0) is the  largest unsigned integer that can be represented using n bits
 *                       (i.e., 0b111....1)
 *
 *   (bvrem  x 0b00...0) is x
 *
 *   (bvsdiv x 0b00...0) is   0b00..01 (i.e., +1) if x's sign bit is 1
 *                       and  0b111111 (i.e., -1) if x's sign bit is 0
 *
 *   (bvsrem x 0b00...0) is x
 *
 *   (bvsmod x 0b00...0) is x
 *
 */
__YICES_DLLSPEC__ extern term_t yices_bvadd(term_t t1, term_t t2);     // addition (t1 + t2)
__YICES_DLLSPEC__ extern term_t yices_bvsub(term_t t1, term_t t2);     // subtraction (t1 - t2)
__YICES_DLLSPEC__ extern term_t yices_bvneg(term_t t1);                // negation (- t1)
__YICES_DLLSPEC__ extern term_t yices_bvmul(term_t t1, term_t t2);     // multiplication (t1 * t2)
__YICES_DLLSPEC__ extern term_t yices_bvsquare(term_t t1);             // square (t1 * t1)
__YICES_DLLSPEC__ extern term_t yices_bvpower(term_t t1, uint32_t d);  // exponentiation (t1 ^ d)

__YICES_DLLSPEC__ extern term_t yices_bvdiv(term_t t1, term_t t2);   // unsigned div
__YICES_DLLSPEC__ extern term_t yices_bvrem(term_t t1, term_t t2);   // unsigned rem
__YICES_DLLSPEC__ extern term_t yices_bvsdiv(term_t t1, term_t t2);  // signed div
__YICES_DLLSPEC__ extern term_t yices_bvsrem(term_t t1, term_t t2);  // signed rem
__YICES_DLLSPEC__ extern term_t yices_bvsmod(term_t t1, term_t t2);  // signed mod

__YICES_DLLSPEC__ extern term_t yices_bvnot(term_t t1);              // bitwise not
__YICES_DLLSPEC__ extern term_t yices_bvnand(term_t t1, term_t t2);  // bitwise not and
__YICES_DLLSPEC__ extern term_t yices_bvnor(term_t t1, term_t t2);   // bitwise not or
__YICES_DLLSPEC__ extern term_t yices_bvxnor(term_t t1, term_t t2);  // bitwise not xor

__YICES_DLLSPEC__ extern term_t yices_bvshl(term_t t1, term_t t2);   // shift t1 left by k bits where k = value of t2
__YICES_DLLSPEC__ extern term_t yices_bvlshr(term_t t1, term_t t2);  // logical shift t1 right by k bits, where k = value of t2
__YICES_DLLSPEC__ extern term_t yices_bvashr(term_t t1, term_t t2);  // arithmetic shift t1 right by k bits, k = value of t2



/*
 * Bitvector and/or/xor 
 *
 * The general form takes an array t[0 ...n-1] as argument (n must be positive).
 * - all t[i]s must be bitvector term of the same type (i.e., the same number of bits).
 * - special forms are provided for convenience for n=2 and 3.
 *
 * These function return NULL_TERM if there's an error.
 *
 * Error reports:
 * if n == 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if t[i] is not valid
 *    code = INVALID_TERM
 *    term1 = t[i]
 * if t[i] is not a bitvector term
 *    code = BITVECTOR_REQUIRED
 *    badval = n
 * if t[0] and t[i] don't have the same bitvector type
 *    code = INCOMPATIBLE_TYPES
 *    term1 = t[0]
 *    type1 = type of t[0]
 *    term2 = t[i]
 *    type2 = type of t[i]
 *
 */
__YICES_DLLSPEC__ extern term_t yices_bvand(uint32_t n, const term_t t[]);
__YICES_DLLSPEC__ extern term_t yices_bvor(uint32_t n, const term_t t[]);
__YICES_DLLSPEC__ extern term_t yices_bvxor(uint32_t n, const term_t t[]);

__YICES_DLLSPEC__ extern term_t yices_bvand2(term_t t1, term_t t2);
__YICES_DLLSPEC__ extern term_t yices_bvor2(term_t t1, term_t t2);
__YICES_DLLSPEC__ extern term_t yices_bvxor2(term_t t1, term_t t2);

__YICES_DLLSPEC__ extern term_t yices_bvand3(term_t t1, term_t t2, term_t t3);
__YICES_DLLSPEC__ extern term_t yices_bvor3(term_t t1, term_t t2, term_t t3);
__YICES_DLLSPEC__ extern term_t yices_bvxor3(term_t t1, term_t t2, term_t t3);


/*
 * Sum of n bitvector terms t[0] ... t[n-1]
 * - n must be positive
 * - all t[i]s must be bitvector terms of the same type (same number of bits)
 *
 * Return NULL_TERM if there's an error.
 *
 * Error reports:
 * if n == 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if t[i] is not valid
 *    code = INVALID_TERM
 *    term1 = t[i]
 * if t[i] is not a bitvector term
 *    code = BITVECTOR_REQUIRED
 *    badval = n
 * if t[0] and t[i] don't have the same bitvector type
 *    code = INCOMPATIBLE_TYPES
 *    term1 = t[0]
 *    type1 = type of t[0]
 *    term2 = t[i]
 *    type2 = type of t[i]
 */
__YICES_DLLSPEC__ extern term_t yices_bvsum(uint32_t n, const term_t t[]);


/*
 * Product of n bitvector terms t[0] ... t[n-1]
 * 
 * - n must be positive
 * - all t[i]s must be bitvector terms of the same type (same number of bits)
 *
 * Return NULL_TERM if there's an error.
 *
 * Error reports:
 * if n == 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if t[i] is not valid
 *    code = INVALID_TERM
 *    term1 = t[i]
 * if t[i] is not a bitvector term
 *    code = BITVECTOR_REQUIRED
 *    term1 = t[i]
 * if t[0] and t[i] don't have the same bitvector type
 *    code = INCOMPATIBLE_TYPES
 *    term1 = t[0]
 *    type1 = type of t[0]
 *    term2 = t[i]
 *    type2 = type of t[i]
 * if the result has degree > YICES_MAX_DEGREE
 *    code = DEGREE_OVERFLOW
 *    badval = degree
 */
__YICES_DLLSPEC__ extern term_t yices_bvproduct(uint32_t n, const term_t t[]);



/*
 * Shift or rotation by an integer constant n
 * - shift_left0 sets the low-order bits to zero
 * - shift_left1 sets the low-order bits to one
 * - shift_right0 sets the high-order bits to zero
 * - shift_right1 sets the high-order bits to one
 * - ashift_right is arithmetic shift, it copies the sign bit
 * - rotate_left: circular rotation
 * - rotate_right: circular rotation
 *
 * If t is a vector of m bits, then n must satisfy 0 <= n <= m.
 *
 * The functions return NULL_TERM (-1) if there's an error.
 *
 * Error reports:
 * if t is not valid
 *   code = INVALID_TERM
 *   term1 = t
 * if t is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t
 * if n > size of t
 *   code = INVALID_BITSHIFT
 *   badval = n
 */
__YICES_DLLSPEC__ extern term_t yices_shift_left0(term_t t, uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_shift_left1(term_t t, uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_shift_right0(term_t t, uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_shift_right1(term_t t, uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_ashift_right(term_t t, uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_rotate_left(term_t t, uint32_t n);
__YICES_DLLSPEC__ extern term_t yices_rotate_right(term_t t, uint32_t n);


/*
 * Extract a subvector of t
 * - t must be a bitvector term of size m
 * - i and j must satisfy i <= j <= m-1
 * The result is the bits i to j of t.
 *
 * Return NULL_TERM (-1) if there's an error.
 *
 * Error reports:
 * if t is not valid
 *   code = INVALID_TERM
 *   term1 = t
 * if t is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t
 * if i <= j <= m-1 does not hold
 *   code = INVALID_BVEXTRACT
 */
__YICES_DLLSPEC__ extern term_t yices_bvextract(term_t t, uint32_t i, uint32_t j);


/*
 * Concatenation
 * - t1 and t2 must be bitvector terms
 *
 * NOTE: t1 is the high-order part of the result, t2 is the low-order part.
 * For example, if t1 is 0b0000 and t2 is 0b11111, then the function will
 * construct 0b000011111.
 *
 * Return NULL_TERM (-1) if there's an error.
 *
 * Error reports
 * if t1 or t2 is not a valid term
 *   code = INVALID_TERM
 *   term1 = t1 or t2
 * if t1 or t2 is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t1 or t2
 * if the size of the result would be larger than MAX_BVSIZE
 *   code = MAX_BVSIZE_EXCEEDED
 *   badval = n1 + n2 (n1 = size of t1, n2 = size of t2)
 */
__YICES_DLLSPEC__ extern term_t yices_bvconcat2(term_t t1, term_t t2);


 /*
  * General form of concatenation: the input is an array of n bitvector terms
  * - n must be positive.
  *
  * NOTE: t[0] is the high-order part of the result, and t[n-1] is the low-order
  * part. For example, if n=3, t[0] is 0b000, t[1] is 0b111, and t[2] is 0b01, then
  * the function constructs 0b00011101.
  * 
  * Error reports:
  * if n == 0
  *    code = POS_INT_REQUIRED
  *    badval = n
  * if t[i] is not valid
  *    code = INVALID_TERM
  *    term1 = t[i]
  * if t[i] is not a bitvector term
  *    code = BITVECTOR_REQUIRED
  *    term1 = t[i]  
  * if the size of the result would be more than YICES_MAX_BVSIZE
  *    code = MAX_BVSIZE_EXCEEDED
  *    badval = sum of the size of t[i]s
  */
__YICES_DLLSPEC__ extern term_t yices_bvconcat(uint32_t n, const term_t t[]);


/*
 * Repeated concatenation:
 * - make n copies of t and concatenate them
 * - n must be positive
 *
 * Return NULL_TERM (-1) if there's an error
 *
 * Error report:
 * if t is not valid
 *   code = INVALID_TERM
 *   term1 = t
 * if t is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t
 * if n == 0
 *   code = POS_INT_REQUIRED
 *   badval = n
 * if n * size of t > MAX_BVSIZE
 *   code = MAX_BVSIZE_EXCEEDED
 *   badval = n * size of t
 */
__YICES_DLLSPEC__ extern term_t yices_bvrepeat(term_t t, uint32_t n);


/*
 * Sign extension
 * - add n copies of t's sign bit
 *
 * Return NULL_TERM if there's an error.
 *
 * Error reports:
 * if t is invalid
 *   code = INVALID_TERM
 *   term1 = t
 * if t is not a bitvector
 *   code = BITVECTOR_REQUIRED
 *   term1 = t
 * if n + size of t > MAX_BVSIZE
 *   code = MAX_BVSIZE_EXCEEDED
 *   badval = n * size of t
 */
__YICES_DLLSPEC__ extern term_t yices_sign_extend(term_t t, uint32_t n);


/*
 * Zero extension
 * - add n zeros to t
 *
 * Return NULL_TERM if there's an error.
 *
 * Error reports:
 * if t is invalid
 *   code = INVALID_TERM
 *   term1 = t
 * if t is not a bitvector
 *   code = BITVECTOR_REQUIRED
 *   term1 = t
 * if n + size of t > MAX_BVSIZE
 *   code = MAX_BVSIZE_EXCEEDED
 *   badval = n * size of t
 */
__YICES_DLLSPEC__ extern term_t yices_zero_extend(term_t t, uint32_t n);


/*
 * AND-reduction:
 * if t is b[m-1] ... b[0], then the result is a bit-vector of 1 bit
 * equal to the conjunction of all bits of t (i.e., (and b[0] ... b[m-1])
 *
 * OR-reduction: compute (or b[0] ... b[m-1])
 *
 * Return NULL_TERM if there's an error
 *
 * Error reports:
 * if t is invalid
 *   code = INVALID_TERM
 *   term1 = t
 * if t is not a bitvector
 *   code = BITVECTOR_REQUIRED
 *   term1 = t
 */
__YICES_DLLSPEC__ extern term_t yices_redand(term_t t);
__YICES_DLLSPEC__ extern term_t yices_redor(term_t t);


/*
 * Bitwise equality comparison: if t1 and t2 are bitvectors of size n,
 * construct (bvredand (bvxnor t1 t2))
 *
 * Return NULL_TERM if there's an error
 *
 * Error reports:
 * if t1 or t2 is not valid
 *   code = INVALID_TERM
 *   term1 = t1 or t2
 * if t1 or t2 is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t1 or t2
 * if t1 and t2 do not have the same bitvector type
 *   code = INCOMPATIBLE_TYPES
 *   term1 = t1
 *   type1 = type of t1
 *   term2 = t2
 *   type2 = type of t2
 */
__YICES_DLLSPEC__ extern term_t yices_redcomp(term_t t1, term_t t2);


/*
 * Convert an array of boolean terms arg[0 ... n-1] into
 * a bitvector term of n bits
 * - arg[0] = low-order bit of the result
 * - arg[n-1] = high-order bit
 * 
 * Error report:
 * if n == 0
 *    code = POS_INT_REQUIRED
 *    badval = n
 * if n > YICES_MAX_BVSIZE
 *    code = MAX_BVSIZE_EXCEEDED
 *    badval = size
 * if arg[i] is invalid
 *    code = INVALID_TERM
 *    term1 = arg[i]
 * if arg[i] is not a boolean
 *    code = TYPE_MISMATCH
 *    term1 = arg[i]
 *    type1 = bool
 */
__YICES_DLLSPEC__ extern term_t yices_bvarray(uint32_t n, const term_t arg[]);


/*
 * Extract bit i of vector t (as a boolean)
 * - if t is a bitvector of n bits, then i must be between 0 and n-1
 * - the low-order bit of t has index 0
 * - the high-order bit of t has index (n-1)
 *
 * Error report:
 * if t is invalid
 *    code = INVALID_TERM
 *    term1 = t
 * if t is not a bitvector term
 *    code = BITVECTOR_REQUIRED
 *    term1 = t
 * if i >= t's bitsize
 *    code = INVALID_BITEXTRACT
 */
__YICES_DLLSPEC__ extern term_t yices_bitextract(term_t t, uint32_t i);


/*
 * BITVECTOR ATOMS
 */

/*
 * All operations return NULL_TERM (i.e., a negative index) on error
 *
 * Error reports
 * if t1 or t2 is not valid
 *   code = INVALID_TERM
 *   term1 = t1 or t2
 * if t1 or t2 is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t1 or t2
 * if t1 and t2 do not have the same bitvector type
 *   code = INCOMPATIBLE_TYPES
 *   term1 = t1
 *   type1 = type of t1
 *   term2 = t2
 *   type2 = type of t2
 */

/*
 * Equality and disequality
 */
__YICES_DLLSPEC__ extern term_t yices_bveq_atom(term_t t1, term_t t2);   // t1 == t2
__YICES_DLLSPEC__ extern term_t yices_bvneq_atom(term_t t1, term_t t2);  // t1 != t2


/*
 * Unsigned inequalities
 */
__YICES_DLLSPEC__ extern term_t yices_bvge_atom(term_t t1, term_t t2);  // t1 >= t2
__YICES_DLLSPEC__ extern term_t yices_bvgt_atom(term_t t1, term_t t2);  // t1 > t2
__YICES_DLLSPEC__ extern term_t yices_bvle_atom(term_t t1, term_t t2);  // t1 <= t2
__YICES_DLLSPEC__ extern term_t yices_bvlt_atom(term_t t1, term_t t2);  // t1 < t2


/*
 * Signed inequalities
 */
__YICES_DLLSPEC__ extern term_t yices_bvsge_atom(term_t t1, term_t t2);  // t1 >= t2
__YICES_DLLSPEC__ extern term_t yices_bvsgt_atom(term_t t1, term_t t2);  // t1 > t2
__YICES_DLLSPEC__ extern term_t yices_bvsle_atom(term_t t1, term_t t2);  // t1 <= t2
__YICES_DLLSPEC__ extern term_t yices_bvslt_atom(term_t t1, term_t t2);  // t1 < t2




/**************
 *  PARSING   *
 *************/

/*
 * Parsing uses the Yices language (cf. doc/YICES-LANGUAGE)
 * - convert an input string s to a type or term.
 * - s must be terminated by '\0'
 *
 * The parsing function return NULL_TYPE or NULL_TERM if there's an
 * error and set the error report. The line and column fields of the
 * error report give information about the error location.
 */
__YICES_DLLSPEC__ extern type_t yices_parse_type(const char *s);
__YICES_DLLSPEC__ extern term_t yices_parse_term(const char *s);




/*******************
 *  SUBSTITUTIONS  *
 ******************/

/*
 * Apply the substitution defined by arrays var and map to a term t
 * - var must be an array of n terms. Each element of var must
 *   be either an uninterpreted term or a variable.
 *   (cf. yices_new_uninterpreted_term and yices_new_variable).
 * - map must be an array of n terms
 * - the type of map[i] must be a subtype of var[i]'s type
 * - every occurrence of var[i] in t is replaced by map[i]
 * - if a variable occurs several times in v, the last occurrence
 *   counts. (e.g., if v[i] = x and v[j] = x with i < j, and
 *   there are no other occurrences of x in v, then x is
 *   replaced by map[j]).
 *
 * Return the resulting term or NULL_TERM if there's an error.
 *
 * Error codes:
 * - INVALID_TERM if var[i] or map[i] is not valid
 * - VARIABLE_REQUIRED if var[i] is not a variable or an uninterpreted term
 * - TYPE_MISMATCH if map[i]'s type is not a subtype of var[i]'s type
 * - DEGREE_OVERFLOW if the substitution causes an overflow
 */
__YICES_DLLSPEC__ extern term_t yices_subst_term(uint32_t n, const term_t var[], const term_t map[], term_t t);


/*
 * Apply a substitution to m terms in parallel
 * - the substitution is defined by arrays var and map:
 *   var must be an array of n variables or uninterpreted terms
 *   map must be an array of n terms
 *   map[i]'s type must be a subtype of var[i]'s type
 * - the substitution is applied to terms t[0] ... t[m-1]
 * - on entry to the function: t[i] must be a valid term
 *   the function applies the substitution to t[i]
 *   then store the result in place (i.e., t[i] := subst(n, var, map, t[i])).
 *
 * Note: it's more efficient to call this function than to call
 * yices_subst_term m times.
 *
 * Return code:
 *  0 if all goes well
 * -1 if there's an error
 *
 * Error codes: as above
 */
__YICES_DLLSPEC__ extern int32_t yices_subst_term_array(uint32_t n, const term_t var[], const term_t map[], uint32_t m, term_t t[]);




/************
 *  NAMES   *
 ***********/

/*
 * It's possible to assign names to terms and types, and later
 * retrieve the term or type from these names.
 *
 * For each term and type, Yices stores a base name that's
 * used for pretty printing. By default, the base name is NULL.
 * The base name is set on the first call to yices_set_term_name or
 * yices_set_type_name.
 *
 * In addition, Yices stores two symbol tables that maps names to
 * terms and types, respectively. The name spaces for types and terms
 * are disjoint. The term or type that a name refers to can be changed,
 * and Yices provides a scoping mechanism:
 * - when function  yices_set_term_name(t, name) is called,
 *   then the previous mapping for 'name' (if any) is hidden and now
 *   'name' refers to term 't'.
 * - if function yices_remove_term_name(name) is called, then the current
 *   mapping for 'name' is removed and the previous mapping (if any)
 *   is restored.
 */

/*
 * The following functions attach a name to a type or a term
 * - name  must be a '\0'-terminated string
 * - if tau or t does not have a base name yet, then name is stored
 *   as base name for tau or t.
 * - if name referred to another term or another type, then this
 *   previous mapping is hidden
 *
 * The functions return -1 and set the error report if the term or
 * type is invalid. Otherwise they return 0.
 *
 * A copy of string name is made internally.
 */
__YICES_DLLSPEC__ extern int32_t yices_set_type_name(type_t tau, const char *name);
__YICES_DLLSPEC__ extern int32_t yices_set_term_name(term_t t, const char *name);


/*
 * Remove the current mapping of name
 * - no effect if name is not assigned to a term or type
 * - if name is assigned to some term t or type tau, then this current
 *   mapping is removed. If name was previously mapped to another term
 *   or type, then the previous mapping is restored.
 */
__YICES_DLLSPEC__ extern void yices_remove_type_name(const char *name);
__YICES_DLLSPEC__ extern void yices_remove_term_name(const char *name);


/*
 * Get type or term of the given name
 * - return NULL_TYPE or NULL_TERM if there's no type or term with that name
 */
__YICES_DLLSPEC__ extern type_t yices_get_type_by_name(const char *name);
__YICES_DLLSPEC__ extern term_t yices_get_term_by_name(const char *name);


/*
 * Remove the base name of a type tau or of a term t.
 *
 * The functions return -1 and set the error report if the
 * type or term is invalid. Otherwise, they return 0.
 *
 * If tau or t doesn't have a base name, the functions do
 * nothing and return 0.
 *
 * Otherwise, the mapping from the base_name to tau or t is removed
 * from the symbol table for terms or types, and the base_name of
 * tau or t is set to NULL (i.e., tau or t don't have a base name anymore).
 */
__YICES_DLLSPEC__ extern int32_t yices_clear_type_name(type_t tau);
__YICES_DLLSPEC__ extern int32_t yices_clear_term_name(term_t t);


/*
 * Get the base name of a term or type
 *
 * The functions return NULL if the  term or type has no name,
 * of if the term or type is not valid. The error report is set
 * to INVALID_TERM or INVALID_TYPE in such cases.
 */
__YICES_DLLSPEC__ extern const char *yices_get_type_name(type_t tau);
__YICES_DLLSPEC__ extern const char *yices_get_term_name(term_t t);





/***********************
 *  TERM EXPLORATION   *
 **********************/

/*
 * Get the type of term t
 * returns NULL_TYPE if t is not a valid term
 * and sets the error report:
 *   code = INVALID_TERM
 *   term1 = t
 */
__YICES_DLLSPEC__ extern type_t yices_type_of_term(term_t t);


/*
 * Check the type of a term t:
 * - returns 0 for false, 1 for true
 *
 * If t is not a valid term, the check functions return false
 * and set the error report as above.
 */
__YICES_DLLSPEC__ extern int32_t yices_term_is_bool(term_t t);
__YICES_DLLSPEC__ extern int32_t yices_term_is_bitvector(term_t t);


/*
 * Size of a bitvector term (i.e., number of bits)
 * - returns 0 if there's an error
 *
 * Error report:
 * if t is not a valid term
 *    code = INVALID_TERM
 *    term1 = t
 * if t is not a bitvector term
 *    code = BITVECTOR_REQUIRED
 *    term1 = t
 */
__YICES_DLLSPEC__ extern uint32_t yices_term_bitsize(term_t t);



/*
 * Internal term structure:
 *
 * - atomic terms are Boolean and bitvectoe constants,
 *   and uninterpreted terms (i.e., terms that don't 
 *   have subterms)
 *
 * - composite terms are of the form
 *    (constructor, number-of-children, list-of-children)
 *
 * - projection terms are of the form
 *    (constructor, index, child)
 *
 * - a bitvector sum is a sum
 *      a_0 t_0 + ... + a_n t_n
 *   where the coefficients a_0 ... a_n are bitvector constants
 *   and t_0 ... t_n are bitvector terms
 *
 * - a product is a term of the form t_0^d_0 x ... x t_n ^d_n
 *   where d_0 ... d_n are positive exponents,
 *   and t_0 ... t_n are either all arithmetic terms or all
 *   bitvector terms
 *
 * - the number of terms in a bitvector sum or product
 *   is always positive, but it may be equal to 1. For
 *   example, the expression (- x) is internally represented
 *   as a sum with one monomial (-1 * x).
 *
 *
 * Composite terms:
 *
 *   if-then-else: (if c t1 t2)
 *   - three children
 *   - the first child is the condition c
 *   - the second child is the 'then' part t1
 *   - the third child is the 'else' part  t2
 *
 *   equality: (eq t1 t2)
 *
 *   distinct: (distinct t1 ... t_n)
 *
 *   negation: (not t)
 *
 *   disjunction: (or t1 ... t_n)
 *
 *   exclusive or: (xor t1 ... t_n)
 *
 *   bit array: (bv-array t1 ... t_n)
 *   - each t_i is a Boolean term
 *   - this uses little-endian form: the first child is the
 *     low-order bit, the last child is the high-order bit.
 *
 *   bitvector operators:
 *     (bvdiv  t1 t2)    unsigned division
 *     (bvrem  t1 t2)    unsigned remainder
 *     (bvsdiv t1 t2)    signed division
 *     (bvsrem t1 t2)    signed remainder (rounding to 0)
 *     (bvsmod t1 t2)    signed remainder (rounding to -infinity)
 *     (bvshl  t1 t2)    shift left
 *     (bvlshr t1 t2)    logical shift right
 *     (bvashr t1 t2)    arithmetic shift right
 *     (bvge   t1 t2)    unsigned comparison: (t1 >= t2)
 *     (bvsge  t1 t2)    signed comparison: (t1 >= t2)
 *
 * Projection terms:
 *
 *   bit selection:  (bit i t)
 *   - the child t is a bitvector term of n bits
 *   - i is an index between 0 and n-1
 */

/*
 * The following functions check the structure of a term_t.
 * They return 0 for false, 1 for true.
 *
 * If t is not a valid term, then the functions return 0
 * and set the error report:
 *    code = INVALID_TERM
 *    term1 = t
 */
__YICES_DLLSPEC__ extern int32_t yices_term_is_atomic(term_t t);
__YICES_DLLSPEC__ extern int32_t yices_term_is_composite(term_t t);
__YICES_DLLSPEC__ extern int32_t yices_term_is_projection(term_t t);
__YICES_DLLSPEC__ extern int32_t yices_term_is_bvsum(term_t t);
__YICES_DLLSPEC__ extern int32_t yices_term_is_product(term_t t);


/*
 * Constructor of term t:
 * - if t is a valid term, the function returns one of the following codes
 *   defined in yices_types.h:
 *
 *   if t is atomic:
 *
 *    YICES_BOOL_CONSTANT        boolean constant
 *    YICES_BV_CONSTANT          bitvector constant
 *    YICES_UNINTERPRETED_TERM   global variables
 *
 *   if t is a composite terms:
 *
 *    YICES_ITE_TERM             if-then-else
 *    YICES_EQ_TERM              equality
 *    YICES_DISTINCT_TERM        (distinct t_1 ... t_n)
 *    YICES_NOT_TERM             (not t)
 *    YICES_OR_TERM              n-ary OR
 *    YICES_XOR_TERM             n-ary XOR
 *
 *    YICES_BV_ARRAY             array of boolean terms
 *    YICES_BV_DIV               unsigned division
 *    YICES_BV_REM               unsigned remainder
 *    YICES_BV_SDIV              signed division
 *    YICES_BV_SREM              remainder in signed division (rounding to 0)
 *    YICES_BV_SMOD              remainder in signed division (rounding to -infinity)
 *    YICES_BV_SHL               shift left (padding with 0)
 *    YICES_BV_LSHR              logical shift right (padding with 0)
 *    YICES_BV_ASHR              arithmetic shift right (padding with sign bit)
 *    YICES_BV_GE_ATOM           unsigned bitvector comparison: (t1 >= t2)
 *    YICES_BV_SGE_ATOM          signed bitvector comparison (t1 >= t2)
 *
 *   if t is a projection
 *
 *    YICES_BIT_TERM             bit-select: extract the i-th bit of a bitvector
 *
 *   if t is a sum
 *
 *    YICES_BV_SUM               sum of pairs a * t where a is a bitvector constant (and t is a bitvector term)
 *
 *  if t is a product
 *
 *    YICES_POWER_PRODUCT         power products: (t1^d1 * ... * t_n^d_n)
 *
 * If t is not a valid term, the function returns a negative number,
 * (i.e., YICES_CONSTRUCTOR_ERROR) and sets the error report.
 *    code = INVALID_TERM
 *    term1 = t
 */
__YICES_DLLSPEC__ extern term_constructor_t yices_term_constructor(term_t t);


/*
 * Number of children of term t
 * - for atomic terms, returns 0
 * - for composite terms, returns the number of children
 * - for projections, returns 1
 * - for sums, returns the number of summands
 * - for products, returns the number of factors
 *
 * - returns -1 if t is not a valid term and sets the error report
 */
__YICES_DLLSPEC__ extern int32_t yices_term_num_children(term_t t);


/*
 * Get i-th child of a composite term
 * - if t has n children (as returned by yices_term_num_children)
 *   then i must be between 0 and n-1.
 *
 * - the function returns NULL_TERM if there's an error.
 *
 * Error codes:
 * if t is not valid
 *    code = INVALID_TERM
 *    term1 = t
 * if t is not a composite, or i is not in the range [0 .. n-1]
 *    code = INVALID_TERM_OP
 */
__YICES_DLLSPEC__ extern term_t yices_term_child(term_t t, int32_t i);


/*
 * Get the argument and index of a projection
 * - if t is invalid or not a projection term then
 *     yices_proj_index returns -1
 *     yices_proj_arg returns NULL_TERM
 *
 * Error codes:
 * if t is not valid
 *    code = INVALID_TERM
 *    term1 = t
 * if t is not a projection
 *    code = INVALID_TERM_OP
 */
__YICES_DLLSPEC__ extern int32_t yices_proj_index(term_t t);
__YICES_DLLSPEC__ extern term_t yices_proj_arg(term_t t);


/*
 * Value of a constant term:
 * - these functions return 0 if t is a valid term and store t's value
 *   in *val
 * - if t is invalid or it's not the right kind of term, then the
 *   functions return -1 and leave *val unchanged.
 *
 * Error codes:
 * if t is not valid
 *    code = INVALID_TERM
 *    term1 = t
 * if t is not of the right kind
 *    code = INVALID_TERM_OP
 */
__YICES_DLLSPEC__ extern int32_t yices_bool_const_value(term_t t, int32_t *val);
__YICES_DLLSPEC__ extern int32_t yices_bv_const_value(term_t t, int32_t val[]);


/*
 * Components of a sum t
 * - i = index (must be between 0 and t's number of children - 1)
 * - each component is a pair (bvconstant, term)
 * - if the term in the pair is NULL_TERM then the component consists of 
 *   only the constant
 * - the number of bits in the bvconstant is the same as in t
 *
 * The function returns 0 on success and -1 on error.
 *
 * Error codes:
 * if t is not valid
 *    code = INVALID_TERM
 *    term1 = t
 * if t is not of the right kind of the index is invalid
 *    code = INVALID_TERM_OP
 */
__YICES_DLLSPEC__ extern int32_t yices_bvsum_component(term_t t, int32_t i, int32_t val[], term_t *term);


/*
 * Component of power product t
 * - i = index (must be between 0 and t's arity - 1)
 * - the component is of the form (term, exponent)
 *   (where exponent is a positive integer)
 *
 * The function returns 0 on success and -1 on error.
 *
 * Error codes:
 * if t is not valid
 *    code = INVALID_TERM
 *    term1 = t
 * if t is not of the right kind or i is invalid
 *    code = INVALID_TERM_OP
 */
__YICES_DLLSPEC__ extern int32_t yices_product_component(term_t t, int32_t i, term_t *term, uint32_t *exp);



/*************************
 *  GARBAGE COLLECTION   *
 ************************/

/*
 * By default, Yices never deletes any terms or types. All terms and
 * types returned by the functions above can always be used by the
 * application. There's no explicit term or type deletion function.
 *
 * If you want to delete terms or types that are no longer useful, you
 * must make an explicit call to the garbage collector by calling
 * function yices_garbage_collect.
 *
 * Yices uses a mark-and-sweep garbage collector. Given a set of root
 * terms and types that must be preserved, Yices marks the roots and
 * all the terms and types on which the roots depend.  After this
 * marking phase, all unmarked terms and types are deleted. Yices
 * includes several methods to specify the root terms and types that
 * you want to keep.
 *
 * First, there's a set of default roots, namely, every term or type
 * that is used by a live context or model. For example, if you call
 * yices_new_context to obtain a new context and assert formulas in
 * this context, then all these formulas are considered root terms
 * until you delete the context using yices_free_context.
 *
 * In addition, you can specify more roots using any of the following
 * mechanisms (they can be combined).
 *
 * 1) give a list of root terms and types as arguments to
 *    yices_garbage_collect.
 *
 * 2) set parameter 'keep_named' to true when calling
 *    yices_garbage_collect.
 *
 *    If this flag is true, any term or type that is stored in the
 *    symbol tables is added to the set of roots.
 *
 * 3) maintain reference counts for terms and types, using the functions
 *      yices_incref_term
 *      yices_decref_term
 *      yices_incref_type
 *      yices_decref_type
 *
 *    When yices_garbage_collect is called, all terms and all types with
 *    a positive reference count are considered roots. If you never
 *    call yices_incref_term or yices_incref_type, then the reference
 *    counting is disabled and it's not taken into account when calling
 *    yices_garbage_collect.
 *
 * Remember that nothing is deleted until the call to yices_garbage_collect.
 */


/*
 * The following functions can be used to get the number of terms
 * and types currently stored in the internal data structures.
 */
__YICES_DLLSPEC__ extern uint32_t yices_num_terms(void);
__YICES_DLLSPEC__ extern uint32_t yices_num_types(void);


/*
 * Reference counting support:
 * - the functions return -1 if there's an error, 0 otherwise
 *
 * Error reports:
 * - INVALID_TERM or INVALID_TYPE if the argument is not valid
 *
 * The decref functions also report an error if the argument has a
 * current reference count of zero. The error report's code is set to
 * BAD_TERM_DECREF or BAD_TYPE_DECREF in such a case.
 */
__YICES_DLLSPEC__ extern int32_t yices_incref_term(term_t t);
__YICES_DLLSPEC__ extern int32_t yices_decref_term(term_t t);
__YICES_DLLSPEC__ extern int32_t yices_incref_type(type_t tau);
__YICES_DLLSPEC__ extern int32_t yices_decref_type(type_t tau);


/*
 * The following functions give the number of terms and types
 * that have a positive reference count. They return 0 if
 * no call to yices_incref_term or yices_incref_type has been
 * made.
 */
__YICES_DLLSPEC__ extern uint32_t yices_num_posref_terms(void);
__YICES_DLLSPEC__ extern uint32_t yices_num_posref_types(void);


/*
 * Call the garbage collector.
 * - t = optional array of terms
 * - nt = size of t
 * - tau = optional array of types
 * - ntau = size of tau
 * - keep_named specifies whether the named terms and types should
 *   all be preserved
 *
 * The set of roots is determined as follows:
 * 1) all terms/types used by contexts and models are roots.
 * 2) if t is non NULL, then all elements in t[0 ... nt-1] are added to
 *    the set of root terms.
 * 3) if tau is non NULL, then all elements in tau[0 ... ntau - 1] are added
 *    to the set of root types.
 * 4) if keep_named is non zero (i.e., true), then all terms and types
 *    that are referenced in the symbol tables are added to the set of
 *    roots.
 * 5) all terms and types with a positive reference count are added to
 *    the set of roots.
 *
 * The function silently ignores any term t[i] or any type tau[j] that's not valid.
 */
__YICES_DLLSPEC__ extern void yices_garbage_collect(const term_t t[], uint32_t nt,
                                                    const type_t tau[], uint32_t ntau,
                                                    int32_t keep_named);






/***************
 *  CONTEXTS   *
 **************/

/*
 * A context is a stack of assertions.
 *
 * The intended use is:
 * 1) create a context (empty)
 * 2) assert one or more formulas in the context.
 *    (it's allowed to call assert several times before check).
 * 3) check satisfiability
 * 4) if the context is satisfiable, optionally build a model
 * 5) reset the context or call push or pop, then go back to 2
 * 6) delete the context
 *
 *
 * A context can be in one of the following states:
 * 1) STATUS_IDLE: this is the initial state.
 *    In this state, it's possible to assert formulas.
 *    After assertions, the status may change to STATUS_UNSAT (if
 *    the assertions are trivially unsatisfiable). Otherwise
 *    the state remains STATUS_IDLE.
 *
 * 2) STATUS_SEARCHING: this is the context status during search.
 *    The context moves into that state after a call to 'check'
 *    and remains in that state until the solver completes
 *    or the search is interrupted.
 *
 * 3) STATUS_SAT/STATUS_UNSAT/STATUS_UNKNOWN: status returned after a search
 *    - STATUS_UNSAT means the assertions are not satisfiable.
 *    - STATUS_SAT means they are satisfiable.
 *    - STATUS_UNKNOWN means that the solver could not determine whether
 *      the assertions are satisfiable or not. This may happen if
 *      Yices is not complete for the specific logic used (e.g.,
 *      if the formula includes quantifiers).
 *
 * 4) STATUS_INTERRUPTED: if the context is in the STATUS_SEARCHING state,
 *    then it can be interrupted via a call to stop_search.
 *    The status STATUS_INTERRUPTED indicates that.
 *
 * For fine tuning: there are options that determine which internal
 * simplifications are applied when formulas are asserted, and
 * other options to control heuristics used by the solver.
 */

/*
 * Create a new context (for QF_BV)
 */
__YICES_DLLSPEC__ extern context_t *yices_new_context(void);


/*
 * Deletion
 */
__YICES_DLLSPEC__ extern void yices_free_context(context_t *ctx);


/*
 * Get status: return the context's status flag
 * - return one of the codes defined in yices_types.h,
 *   namely one of the constants
 *
 *    STATUS_IDLE
 *    STATUS_SEARCHING
 *    STATUS_UNKNOWN
 *    STATUS_SAT
 *    STATUS_UNSAT
 *    STATUS_INTERRUPTED
 *
 */
__YICES_DLLSPEC__ extern smt_status_t yices_context_status(context_t *ctx);


/*
 * Reset: remove all assertions and restore ctx's
 * status to STATUS_IDLE.
 */
__YICES_DLLSPEC__ extern void yices_reset_context(context_t *ctx);


/*
 * Push: mark a backtrack point
 * - return 0 if this operation is supported by the context
 *         -1 otherwise
 *
 * Error report:
 * - if the context is not configured to support push/pop
 *   code = CTX_OPERATION_NOT_SUPPORTED
 * - if the context status is STATUS_UNSAT or STATUS_SEARCHING or STATUS_INTERRUPTED
 *   code = CTX_INVALID_OPERATION
 */
__YICES_DLLSPEC__ extern int32_t yices_push(context_t *ctx);


/*
 * Pop: backtrack to the previous backtrack point (i.e., the matching
 * call to yices_push).
 * - return 0 if the operation succeeds, -1 otherwise.
 *
 * Error report:
 * - if the context is not configured to support push/pop
 *   code = CTX_OPERATION_NOT_SUPPORTED
 * - if there's no matching push (i.e., the context stack is empty)
 *   or if the context's status is STATUS_SEARCHING
 *   code = CTX_INVALID_OPERATION
 */
__YICES_DLLSPEC__ extern int32_t yices_pop(context_t *ctx);


/*
 * Several options determine how much simplification is performed
 * when formulas are asserted. It's best to leave them untouched
 * unless you really know what you're doing.
 *
 * The following functions selectively enable/disable a preprocessing
 * option. The current options include:
 *
 * 1) Variable elimination (on by default).
 *    This eliminates variables by turning equalities into 
 *    substitutions whenever possible. For example, if (= x 0x111)) is 
 *    asserted, then x is replaced by 0x111 everywhere.
 * 
 * 2) Flattening (on by default).
 *    This attempts to flatten the boolean expressions to 
 *    eliminate nested operators.  
 *    For example, (OR a b (OR a c d e) f g) is rewritten 
 *    to (OR a b c d e f g) by flattening.
 *
 * 3) Bit-vector arithmetic elimination (on by default).
 *    This is the bit-vector analogue of Gaussian elimination.
 *    For example, if (= (bv-add x y z) 0) is asserted, then
 *    x may be replaced by (bv-neg (bv-add y z)) everywhere.
 *
 * The following functions selectively enable/disable each of these
 * options.
 */
__YICES_DLLSPEC__ extern void yices_enable_var_elim(context_t *ctx);
__YICES_DLLSPEC__ extern void yices_disable_var_elim(context_t *ctx);

__YICES_DLLSPEC__ extern void yices_enable_flattening(context_t *ctx);
__YICES_DLLSPEC__ extern void yices_disable_flattening(context_t *ctx);

__YICES_DLLSPEC__ extern void yices_enable_bvarith_elim(context_t *ctx);
__YICES_DLLSPEC__ extern void yices_disable_bvarith_elim(context_t *ctx);


/*
 * General form: enable or disable the given option:
 * - the option must be given as a '\0'-terminated string
 * - recognized options include
 *    "var-elim"
 *    "bvarith-elim"
 *    "flatten"
 *
 * The two function return -1 if there's an error, 0 otherwise.
 *
 * Error codes:
 *  CTX_UNKNOWN_PARAMETER if the option name is not one of the above.
 */
__YICES_DLLSPEC__ extern int32_t yices_context_enable_option(context_t *ctx, const char *option);
__YICES_DLLSPEC__ extern int32_t yices_context_disable_option(context_t *ctx, const char *option);



/*
 * Assert formula t in ctx
 * - ctx status must be STATUS_IDLE or STATUS_UNSAT or STATUS_SAT or STATUS_UNKNOWN
 * - t must be a boolean term
 *
 * If ctx's status is STATUS_UNSAT, nothing is done.
 *
 * If ctx's status is STATUS_IDLE, STATUS_SAT, or STATUS_UNKNOWN, then
 * the formula is simplified and  asserted in the context. The context
 * status is changed  to STATUS_UNSAT if the formula  is simplified to
 * 'false' or to STATUS_IDLE otherwise.
 *
 * This returns 0 if there's no error or -1 if there's an error.
 *
 * Error report:
 * if t is invalid
 *   code = INVALID_TERM
 *   term1 = t
 * if t is not boolean
 *   code = TYPE_MISMATCH
 *   term1 = t
 *   type1 = bool (expected type)
 * if ctx's status is not STATUS_IDLE or STATUS_UNSAT or STATUS_SAT or STATUS_UNKNOWN
 *   code = CTX_INVALID_OPERATION
 * if ctx's status is neither STATUS_IDLE nor STATUS_UNSAT, and the context is
 * not configured for multiple checks
 *   code = CTX_OPERATION_NOT_SUPPORTED
 *
 * Other error codes are defined in yices_types.h to report that t is
 * outside the logic supported by ctx.
 */
__YICES_DLLSPEC__ extern int32_t yices_assert_formula(context_t *ctx, term_t t);


/*
 * Assert an array of n formulas t[0 ... n-1]
 * - ctx's status must be STATUS_IDLE or STATUS_UNSAT or STATUS_SAT or STATUS_UNKNOWN
 * - all t[i]'s must be valid boolean terms.
 *
 * The function returns -1 on error, 0 otherwise.
 *
 * The error report is set as in the previous function.
 */
__YICES_DLLSPEC__ extern int32_t yices_assert_formulas(context_t *ctx, uint32_t n, const term_t t[]);


/*
 * Check satisfiability: check whether the assertions stored in ctx
 * are satisfiable.
 * - params is an optional structure that stores heuristic parameters.
 * - if params is NULL, default parameter settings are used.
 *
 * It's better to keep params=NULL unless you encounter performance
 * problems.  Then you may want to play with the heuristics to see if
 * performance improves.
 *
 * The behavior and returned value depend on ctx's current status:
 *
 * 1) If ctx's status is STATUS_SAT, STATUS_UNSAT, or STATUS_UNKNOWN, the function
 *    does nothing and just returns the status.
 *
 * 2) If ctx's status is STATUS_IDLE, then the solver searches for a
 *    satisfying assignment. If param != NULL, the search parameters
 *    defined by params are used.
 *
 *    The function returns one of the following codes:
 *    - STATUS_SAT: the context is satisfiable
 *    - STATUS_UNSAT: the context is not satisfiable
 *    - STATUS_UNKNOWN: satisfiability can't be proved or disproved
 *    - STATUS_INTERRUPTED: the search was interrupted
 *
 *    The returned status is also stored as the new ctx's status flag,
 *    with the following exception. If the context was built with
 *    mode = INTERACTIVE and the search was interrupted, then the
 *    function returns STATUS_INTERRUPTED but the ctx's state is restored to
 *    what it was before the call to 'yices_check_context' and the
 *    status flag is reset to STATUS_IDLE.
 *
 * 3) Otherwise, the function does nothing and returns 'STATUS_ERROR',
 *    it also sets the yices error report (code = CTX_INVALID_OPERATION).
 */
__YICES_DLLSPEC__ extern smt_status_t yices_check_context(context_t *ctx, const param_t *params);


/*
 * Add a blocking clause: this is intended to help enumerate different models
 * for a set of assertions.
 * - if ctx's status is STATUS_SAT or STATUS_UNKNOWN, then a new clause is added to ctx
 *   to remove the current truth assignment from the search space. After this
 *   clause is added, the next call to yices_check_context will either produce
 *   a different truth assignment (hence a different model) or return STATUS_UNSAT.
 *
 * - ctx's status flag is updated to STATUS_IDLE (if the new clause is not empty) or
 *   to STATUS_UNSAT (if the new clause is the empty clause).
 *
 * Return code: 0 if there's no error, -1 if there's an error.
 *
 * Error report:
 * if ctx's status is different from STATUS_SAT or STATUS_UNKNOWN
 *    code = CTX_INVALID_OPERATION
 * if ctx is not configured to support multiple checks
 *    code = CTX_OPERATION_NOT_SUPPORTED
 */
__YICES_DLLSPEC__ extern int32_t yices_assert_blocking_clause(context_t *ctx);


/*
 * Interrupt the search:
 * - this can be called from a signal handler to stop the search,
 *   after a call to yices_check_context to interrupt the solver.
 *
 * If ctx's status is STATUS_SEARCHING, then the current search is
 * interrupted. Otherwise, the function does nothing.
 */
__YICES_DLLSPEC__ extern void yices_stop_search(context_t *ctx);




/*
 * SEARCH PARAMETERS
 */

/*
 * A parameter record is an opaque object that stores various
 * search parameters and options that control the heuristics used by
 * the solver.
 *
 * A parameter structure is created by calling
 * - yices_new_param_record(void)
 * This returns a parameter structure initialized with default
 * settings.
 *
 * Then individual parameters can be set using function
 * - yices_set_param(s, name, value) where both name and value are
 *   character strings.
 * - an unknown/unsupported parameter name is ignored
 *
 * Then the param object can be passed on as argument to yices_check_context.
 *
 * When it's no longer needed, the object must be deleted by
 * calling yices_free_param_structure(param).
 */

/*
 * Return a parameter record initialized with default settings.
 */
__YICES_DLLSPEC__ extern param_t *yices_new_param_record(void);


/*
 * Set a parameter in record p
 * - pname = parameter name
 * - value = setting
 *
 * The parameters are explained in doc/YICES-LANGUAGE.
 *
 * Return -1 if there's an error, 0 otherwise.
 *
 * Error codes:
 * - CTX_UNKNOWN_PARAMETER if pname is not a known parameter name
 * - CTX_INVALID_PARAMETER_VALUE if value is not valid for the parameter
 */
__YICES_DLLSPEC__ extern int32_t yices_set_param(param_t *p, const char *pname, const char *value);


/*
 * Delete the record param
 */
__YICES_DLLSPEC__ extern void yices_free_param_record(param_t *param);





/**************
 *   MODELS   *
 *************/

/*
 * Build a model from ctx
 * - keep_subst indicates whether the model should include
 *   the eliminated variables:
 *   keep_subst = 0 means don't keep substitutions,
 *   keep_subst != 0 means keep them
 * - ctx status must be STATUS_SAT or STATUS_UNKNOWN
 *
 * The function returns NULL if the status isn't SAT or STATUS_UNKNOWN
 * and sets an error report (code = CTX_INVALID_OPERATION).
 *
 * When assertions are added to the context, the simplifications may
 * eliminate variables (cf. simplification options above).  The flag
 * 'keep_subst' indicates whether the model should keep track of these
 * eliminated variables and include their value.
 *
 * Example: after the following assertions
 *
 *    (= x (bv-add y z))
 *    (bv-gt y 0b000)
 *    (bg-gt z 0b000)
 *
 * variable 'x' gets eliminated. Then a call to 'check_context' will
 * return STATUS_SAT and we can ask for a model 'M'
 * - if 'keep_subst' is false then the value of 'x' in 'M' is unavailable.
 * - if 'keep_subst' is true then the value of 'x' in 'M' is computed,
 *   based on the value of 'y' and 'z' in 'M'.
 *
 * It's always better to set 'keep_subst' true. The only exceptions
 * are some of the large SMT_LIB benchmarks where millions of variables
 * are eliminated.  In such cases, it saves memory to set 'keep_subst'
 * false, and model construction is faster too.
 */
__YICES_DLLSPEC__ extern model_t *yices_get_model(context_t *ctx, int32_t keep_subst);


/*
 * Delete model mdl
 */
__YICES_DLLSPEC__ extern void yices_free_model(model_t *mdl);



/***********************
 *  VALUES IN A MODEL  *
 **********************/

/*
 * Evaluation functions. Once a model is constructed, it's possible
 * to query for the value of a term t in that model. The following
 * functions do that for different term types.
 *
 * The evaluation functions return -1 if the value of t is unknown
 * or can't be computed in the model. Otherwise, they return 0 (except
 * function yices_get_value_as_term).
 *
 * Possible error codes:
 * If t is not valid:
 *   code = INVALID_TERM
 *   term1 = t
 * If t contains a subterm whose value is not known
 *   code = EVAL_UNKNOWN_TERM
 * If t contains free variables
 *   code = EVAL_FREEVAR_IN_TERM
 * If t contains quantifier(s)
 *   code = EVAL_QUANTIFIER
 * If t contains lambda terms
 *   code = EVAL_LAMBDA
 * If the evaluation fails for other reasons:
 *   code = EVAL_FAILED
 *
 * Other codes are possible depending on the specific evaluation function.
 */

/*
 * Value of boolean term t: returned as an integer val
 * - val = 0 means t is false in mdl
 * - val = 1 means t is true in mdl
 *
 * Error codes:
 * If t is not boolean
 *   code = TYPE_MISMATCH
 *   term1 = t
 *   type1 = bool (expected type)
 * + the other evaluation error codes above.
 */
__YICES_DLLSPEC__ extern int32_t yices_get_bool_value(model_t *mdl, term_t t, int32_t *val);

// same function under a different name for backward compatibility
__YICES_DLLSPEC__ extern int32_t yices_eval_bool_term_in_model(model_t *mdl, term_t t, int32_t *val);


/*
 * Value of bitvector term t in mdl
 * - the value is returned in array val
 * - val must be an integer array of sufficient size to store all bits of t
 *   (the number of bits of t can be found yices_term_bitsize).
 * - bit i of t is stored in val[i] (val[i] is either 0 or 1)
 * - the value is returned using small-endian convention:
 *    val[0] is the low-order bit
 *    ...
 *    val[n-1] is the high-order bit
 *
 * If t is not a bitvector term
 *   code = BITVECTOR_REQUIRED
 *   term1 = t
 */
__YICES_DLLSPEC__ extern int32_t yices_get_bv_value(model_t *mdl, term_t t, int32_t val[]);

// same function under a different name for backward compatibility
__YICES_DLLSPEC__ extern int32_t yices_eval_bv_term_in_model(model_t *mdl, term_t t, int32_t val[]);


/*
 * CHECK THE VALUE OF BOOLEAN FORMULAS
 */

/*
 * Check whether f is true in mdl
 * - the returned value is
 *     1 if f is true in mdl,
 *     0 if f is false in mdl,
 *    -1 if f's value can't be evaluated (then an error code is set)
 *
 * Error codes:
 * - same as yices_get_bool_value
 */
__YICES_DLLSPEC__ int32_t yices_formula_true_in_model(model_t *mdl, term_t f);


/*
 * Check whether f[0 ... n-1] are all true in mdl
 * - the returned value is as in the previous function:
 *     1 if all f[i] are true
 *     0 if one f[i] is false (and f[0 ... i-1] are all true)
 *    -1 if one f[i] can't be evaluated (and f[0 ... i-1] are all true)
 *
 * Error codes:
 * - same as yices_get_bool_value
 *
 * NOTE: if n>1, it's more efficient to call this function once than to
 * call the previous function n times.
 */
__YICES_DLLSPEC__ int32_t yices_formulas_true_in_model(model_t *mdl, uint32_t n, const term_t f[]);



/**********************
 *  PRETTY PRINTING   *
 *********************/

/*
 * Pretty printing uses a rectangular display area, characterized
 * by its width, height, and offset as follows.
 *
 *                  <----------- width ------------->
 *                   _______________________________
 * <---- offset --->|                               |   ^
 *                  |                               |   |
 *                  |                               | Height
 *                  |                               |   |
 *                  |                               |   v
 *                   -------------------------------
 *
 */

/*
 * Pretty print type tau or term t on file f
 * - width, height, offset define the print area
 * - f = output file to use.
 *   f must be open and writable.
 *
 * - return -1 on error
 * - return 0 otherwise.
 *
 * - possible error report for yices_pp_type
 *    code = INVALID_TYPE
 *    type1 = tau
 *
 * - possible error report for yices_pp_term
 *    code = INVALID_TERM
 *    term1 = t
 *
 * - other errors (for both)
 *    code = OUTPUT_ERROR if writing to file f failed.
 *    in this case, errno, perror, etc. can be used for diagnostic.
 */
__YICES_DLLSPEC__ extern int32_t yices_pp_type(FILE *f, type_t tau, uint32_t width, uint32_t height, uint32_t offset);
__YICES_DLLSPEC__ extern int32_t yices_pp_term(FILE *f, term_t t, uint32_t width, uint32_t height, uint32_t offset);


/*
 * Pretty print an array of terms:
 * - f = output file to use
 * - n = number of terms in the array a
 * - a = array of terms
 * - width, height, offset define the print area
 * - horiz = Boolean flag that determines the layout
 *
 * If horiz is true (non-zero), the terms are printed as follows
 *     a[0]  a[1] .... a[k]
 *     a[k+1] ... a[n-1]
 *
 * If horiz is false (zero), the terms are printed as follows
 *     a[0]
 *     a[1]
 *      ...
 *     a[n-1]
 *
 * The function first checks whether all terms in a[0... n-1] are
 * valid.  If not, it sets the error report:
 *    code = INVALID_TERM
 *    term = a[i] (first invalid term in the array)
 * and returns -1. Nothing is printed in this case.
 *
 * Otherwise, the terms a[0... n-1] are printed in the specified
 * print area (some terms may be omitted if the area is too small).
 * The function returns 0 unless there's an error while writing to
 * file f. In such as case, the function returns -1 and
 * set the error report to:
 *    code = OUTPUT_ERROR
 *
 */
__YICES_DLLSPEC__ extern int32_t yices_pp_term_array(FILE *f, uint32_t n, const term_t a[],
                                                     uint32_t witdh, uint32_t height, uint32_t offset, int32_t horiz);



/*
 * Print model mdl on FILE f
 * - f must be open/writable
 *
 * The model stores a mapping from uninterpreted terms to values.
 * This function will print the value of these uninterpreted terms,
 * but it will skip the ones that don't have a name.
 *
 * To see the value of uninterpreted term x in the model, you have to
 * give a name to 'x'. For example, this can be done by creating 'x'
 * as follows:
 *
 *   x = yices_new_uninterpreted_term(<some type>)
 *   yices_set_term_name(x, "x")
 *
 */
__YICES_DLLSPEC__ extern void yices_print_model(FILE *f, model_t *mdl);


/*
 * Pretty printing:
 * - f = output file to use
 * - width, height, offset define the print area
 *
 * return -1 on error, 0 otherwise
 *
 * Like yices_print_model, this function ignores the uninterpreted terms
 * that don't have a name.
 *
 * On error:
 *   code = OUTPUT_ERROR (means that writing to f failed)
 *   in this case, errno, perror, etc. can be used for diagnostic.
 */
__YICES_DLLSPEC__ extern int32_t yices_pp_model(FILE *f, model_t *mdl, uint32_t width, uint32_t height, uint32_t offset);


/*
 * Convert type tau or term t to a string using the pretty printer.
 * - width, height, offset define the print area as above.
 *
 * - return NULL on error
 * - return a '\0' terminated string otherwise
 *   this string must be deleted by calling yices_free_string when it's no longer used
 *
 * - possible error report for yices_type_to_string
 *    code = INVALID_TYPE
 *    type1 = tau
 *
 * - possible error report for yices_term_to_string
 *    code = INVALID_TERM
 *    term1 = t
 *
 */
__YICES_DLLSPEC__ extern char *yices_type_to_string(type_t tau, uint32_t width, uint32_t height, uint32_t offset);
__YICES_DLLSPEC__ extern char *yices_term_to_string(term_t t, uint32_t width, uint32_t height, uint32_t offset);


/*
 * Convert model to a string using the pretty printer.
 * - width, height, offset define the print area
 *
 * Returns a '\0'-terminated string otherwise. This string must be deleted
 * when no longer needed by calling yices_free_string.
 */
__YICES_DLLSPEC__ extern char *yices_model_to_string(model_t *mdl, uint32_t width, uint32_t height, uint32_t offset);


#ifdef __cplusplus
} /* close extern "C" { */
#endif


#endif /* __YICES_H */

