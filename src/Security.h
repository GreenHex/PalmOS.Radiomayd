/*
 * Security.h
 */

#ifndef __SECURITY_H__
#define __SECURITY_H__

#include <Hs.h>
#include <Password.h >
#include <Preferences.h>

#include "PhnSecDB.h"

// Prototypes
extern void 					LockDBs( Boolean bLock );
extern void 					ProtectDBs( Boolean bProtect );
extern void 					LockDevice( void );
extern void 					SetLockPassword( void );
extern Boolean 					IsPrivate( void );
extern void 					SetPrivacy( Boolean bHide );


#endif /* __SECURITY_H__ */

 
/*
 * Security.h
 */