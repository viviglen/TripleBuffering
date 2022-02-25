#include <assert.h>
#include <stdio.h>
#define _GLIBCXX_ATOMIC_BUILTINS 1

#define uint_fast8_t	unsigned char

/* bit flags are (unused) (new write) (2x dirty) (2x clean) (2x snap) */
#define TRIPLE_BUFFER_TYPE(TYPENAME, TYPE) \
  struct TYPENAME { \
    TYPE buffer[3]; \
    volatile uint_fast8_t flags; \
  }


/* initially dirty = 0, clean = 1 and snap = 2 */
#define TRIPLE_BUFFER_NEW(NAME,TYPENAME) \
  struct TYPENAME NAME; \
  NAME.flags = 0x6;

#define TRIPLE_BUFFER_SNAP_PTR(BUFFER) &BUFFER.buffer[BUFFER.flags & 0x3]
#define TRIPLE_BUFFER_WRITE_PTR(BUFFER) &BUFFER.buffer[(BUFFER.flags & 0x30) >> 4]


#ifdef _GLIBCXX_ATOMIC_BUILTINS
    #define TRIPLE_BUFFER_FLIP_WRITER(BUFFER) \
	    do { \
		    uint_fast8_t flagsNow; \
		    uint_fast8_t newFlags; \
		    do { \
			    flagsNow = BUFFER.flags; \
			    newFlags = 0x40 | ((flagsNow & 0xC) << 2) | ((flagsNow & 0x30) >> 2) | (flagsNow & 0x3); \
		    } while(!__sync_bool_compare_and_swap(&BUFFER.flags, flagsNow, newFlags)); \
	    } while(0)
#else
  #error no atomic operations available!
#endif


#ifdef _GLIBCXX_ATOMIC_BUILTINS
    #define TRIPLE_BUFFER_NEW_SNAP(BUFFER) \
	   do { \
		   uint_fast8_t flagsNow; \
		   uint_fast8_t newFlags; \
		   do { \
			   flagsNow = BUFFER.flags; \
			   if((flagsNow & 0x40)==0) break; \
			   newFlags = (flagsNow & 0x30) | ((flagsNow & 0x3) << 2) | ((flagsNow & 0xC) >> 2); \
		   } while(!__sync_bool_compare_and_swap(&BUFFER.flags, flagsNow, newFlags)); \
	   } while(0)
#else
  #error no atomic operations available!
#endif

void main(){
	TRIPLE_BUFFER_TYPE(TestStruct, int);
	TRIPLE_BUFFER_NEW(it, TestStruct);

	/* Test 1 */

	*TRIPLE_BUFFER_WRITE_PTR(it) = 3;
	TRIPLE_BUFFER_FLIP_WRITER(it);

	TRIPLE_BUFFER_NEW_SNAP(it);
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));

	/* Test 2 */

	*TRIPLE_BUFFER_WRITE_PTR(it) = 4;
	TRIPLE_BUFFER_FLIP_WRITER(it);
	*TRIPLE_BUFFER_WRITE_PTR(it) = 5;
	TRIPLE_BUFFER_FLIP_WRITER(it);
	*TRIPLE_BUFFER_WRITE_PTR(it) = 6;
	TRIPLE_BUFFER_FLIP_WRITER(it);
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	TRIPLE_BUFFER_NEW_SNAP(it);
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	TRIPLE_BUFFER_NEW_SNAP(it);

	*TRIPLE_BUFFER_WRITE_PTR(it) = 7;
	TRIPLE_BUFFER_FLIP_WRITER(it);
	*TRIPLE_BUFFER_WRITE_PTR(it) = 8;
	TRIPLE_BUFFER_FLIP_WRITER(it);
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	TRIPLE_BUFFER_NEW_SNAP(it);
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
	TRIPLE_BUFFER_NEW_SNAP(it);
	printf("%d,%d\n",__LINE__,*TRIPLE_BUFFER_SNAP_PTR(it));
}
