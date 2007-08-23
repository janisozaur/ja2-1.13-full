#ifndef __ENCRYPTED_H_
#define __ENCRYPTED_H_

#include "types.h"

BOOLEAN LoadEncryptedDataFromFile16(STR pFileName, void *pDestString, UINT32 uiSeekFrom, UINT32 uiSeekAmount);
BOOLEAN LoadEncryptedDataFromFile(STR pFileName, STR16 pDestString, UINT32 uiSeekFrom, UINT32 uiSeekAmount);

#endif
