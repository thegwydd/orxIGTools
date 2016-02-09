#ifndef __TYPES_INCLUDE_H_
#define __TYPES_INCLUDE_H_

/*----------------------------------------------------------------------------
 *  Basic Data Types
 *--------------------------------------------------------------------------*/

#if defined(__APPLE__) || defined(HAVE_STDINT_H)
#include <inttypes.h>
	/*
	 * TODO: This is a C99 standard header.  We should be able to test for
	 * #if __STDC_VERSION__ >= 199901L, but that breaks the Netware build
	 * (which doesn't have stdint.h).
	 */
	typedef 	uint64_t 	uint64;
	typedef 	int64_t 	int64;	
	typedef 	uint32_t 	uint32;		
	typedef 	int32_t 	int32;	
	typedef 	uint16_t 	uint16;		
	typedef 	int16_t 	int16;		
	typedef 	uint8_t		uint8;			

	/*
	 * XXX: int8_t is defined to be 'signed char' on Mac hosts.
	 * Unfortunately, GCC 4.0.1 warns when doing pointer assignment or
	 * comparison between signed char * and char * (even if char is
	 * signed).
	 * If we want to use int8_t to define int8, we need to go through and
	 * replace uses of char * with signed char * to prevent warnings.
	 */
	#define int8	char 				
#else /* !HAVE_STDINT_H */

	#ifdef _MSC_VER
		typedef  	unsigned __int64	uint64;
		typedef 	signed __int64		int64;
	#elif __GNUC__
	/* The Xserver source compiles with -ansi -pendantic */
		#ifndef __STRICT_ANSI__
			#if defined(VM_X86_64)
				typedef  	unsigned long		uint64;
				typedef 	long				int64;
			#else
				typedef 	unsigned long long	uint64;
				typedef 	long long			int64;
			#endif
		#elif defined __FreeBSD__
			typedef 	unsigned long long	 uint64;
			typedef 	long long			 int64;
		#endif
	#else
		#error - Need compiler define for int64/uint64
	#endif /* _MSC_VER */

	typedef  	unsigned int	uint32;
	typedef  	unsigned short  uint16; 	
	typedef  	unsigned char   uint8; 	
	
	typedef 	int       		int32;	
	typedef 	short     		int16;	
	typedef 	char      		int8;		

#endif /* HAVE_STDINT_H */

typedef uint64	QWORD;
#ifndef _MSC_VER
typedef 	uint32	DWORD;
typedef 	uint16	WORD;
typedef 	uint8	BYTE;
#endif

#ifndef INLINE
#   ifdef _MSC_VER
#      define INLINE        __inline
#   else
#      define INLINE        inline
#   endif
#endif

#if defined(_WIN32) && !defined(VMX86_NO_THREADS)
#define THREADSPECIFIC __declspec(thread)
#else
#define THREADSPECIFIC
#endif

/*
 * Branch prediction hints:
 *     LIKELY(exp)   - Expression exp is likely TRUE.
 *     UNLIKELY(exp) - Expression exp is likely FALSE.
 *   Usage example:
 *        if (LIKELY(excCode == EXC_NONE)) {
 *               or
 *        if (UNLIKELY(REAL_MODE(vc))) {
 *
 * We know how to predict branches on gcc3 and later (hopefully),
 * all others we don't so we do nothing.
 */

#if (__GNUC__ >= 3)
/*
 * gcc3 uses __builtin_expect() to inform the compiler of an expected value.
 * We use this to inform the static branch predictor. The '!!' in LIKELY
 * will convert any !=0 to a 1.
 */
#define LIKELY(_exp)     __builtin_expect(!!(_exp), 1)
#define UNLIKELY(_exp)   __builtin_expect((_exp), 0)
#else
#define LIKELY(_exp)      (_exp)
#define UNLIKELY(_exp)    (_exp)
#endif

/*
 * UNUSED_PARAM should surround the parameter name and type declaration,
 * e.g. "int MyFunction(int var1, UNUSED_PARAM(int var2))"
 *
 */

#ifndef UNUSED_PARAM
# if defined(__GNUC__)
#  define UNUSED_PARAM(_parm) _parm  __attribute__((__unused__))
# else
#  define UNUSED_PARAM(_parm) _parm
# endif
#endif

/*
 * ALIGNED specifies minimum alignment in "n" bytes.
 */

#ifdef __GNUC__
#define ALIGNED(n) __attribute__((__aligned__(n)))
#else
#define ALIGNED(n)
#endif

/*
 * __func__ is a stringified function name that is part of the C99 standard. The block
 * below defines __func__ on older systems where the compiler does not support that
 * macro.
 */
#if defined(__GNUC__) \
   && ((__GNUC__ == 2 && __GNUC_MINOR < 96) \
       || (__GNUC__ < 2))
#   define __func__ __FUNCTION__
#endif






#endif // _TYPES_INCLUDE_H_











