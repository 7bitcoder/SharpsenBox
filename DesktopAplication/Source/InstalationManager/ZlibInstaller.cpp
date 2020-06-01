#include <QtZlib/zlib.h>


namespace bb {
	typedef struct z_stream_s {
		Bytef* next_in;    /* next input byte */
		uInt avail_in;     /* number of bytes available at next_in */
		uLong total_in;    /* count of input bytes read so far */
		Bytef* next_out;   /* next output byte should be put there */
		uInt avail_out;    /* remaining free space at next_out */
		uLong total_out;   /* count of bytes output so far */
		char* msg;         /* last error message, NULL if no error */
		struct internal_state* state; /* not visible by applications*/
		alloc_func zalloc; /* used to allocate the internal state*/
		free_func zfree;   /* used to free the internal state */
		voidpf opaque;     /* private data passed to zalloc and zfree*/
		int data_type;     /* best guess about the data: ascii or binary*/
		uLong adler;       /* adler32 value of the uncompressed data */
		uLong reserved;    /* reserved for future use */
	} z_stream;

}