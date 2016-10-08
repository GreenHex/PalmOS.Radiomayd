/*
 * CryptUtils.h
 */
#ifndef __CRYPTUTILS_H__
#define __CRYPTUTILS_H__

#include <Hs.h>
#include <PalmOS.h>
#include <Encrypt.h>

#define BUFLEN_KEY			8

typedef enum {
	decrypt 				= 0,
	encrypt 				= 1
} cryptAction_e;

// Prototypes
extern void 			EncBuf( UInt8* io_buffer, UInt16 buf_len, cryptAction_e action );

#endif /* __CRYPTUTILS_H__ */

/*
 * CryptUtils.h
 */