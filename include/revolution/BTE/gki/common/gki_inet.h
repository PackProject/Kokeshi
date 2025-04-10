/******************************************************************************
 *
 *  Copyright (C) 2002-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#ifndef GKI_INET_H
#define GKI_INET_H

#include "data_types.h"

#define htons	ntohs
#define htonl	ntohl

#define htonets	nettohs
#define htonetl	nettohl

#if BIG_ENDIAN == TRUE
#define ntohs(n) (n)
#define ntohl(n) (n)
#define ntoh6(n) (n)

#define nettohs(n) (n)
#define nettohl(n) (n)
#else
extern UINT16 ntohs(UINT16 n);
extern UINT32 ntohl(UINT32 n);
extern UINT8 *ntoh6(UINT8 *p);

#define nettohs(n) ((UINT16)((((n) << 8) & 0xff00) | (((n) >> 8) & 0x00ff)))
#define nettohl(n) ((((n) & 0x000000ff) << 24) | (((n) << 8) & 0x00ff0000) | \
                   (((n) >> 8) & 0x0000ff00) | (((n) >> 24) & 0x000000ff))
#endif

#endif /* GKI_INET_H */
