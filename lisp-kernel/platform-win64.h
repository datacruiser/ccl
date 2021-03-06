/*
 * Copyright 1994-2010 Clozure Associates
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define WORD_SIZE 64
#define PLATFORM_OS PLATFORM_OS_WINDOWS
#define PLATFORM_CPU PLATFORM_CPU_X86
#define PLATFORM_WORD_SIZE PLATFORM_WORD_SIZE_64

#include <windows.h>

typedef CONTEXT ExceptionInformation;

/* Supposedly, the high-end version of Vista allow 128GB of pageable memory */
#define MAXIMUM_MAPPABLE_MEMORY (512LL<<30LL)
#define IMAGE_BASE_ADDRESS 0x100000000LL

#include "lisptypes.h"
#include "x86-constants64.h"

/* DWORD64 indices in &(CONTEXT->Rax) */
#define REG_RAX     0
#define REG_RCX     1
#define REG_RDX     2
#define REG_RBX     3
#define REG_RSP     4
#define REG_RBP     5
#define REG_RSI     6
#define REG_RDI     7
#define REG_R8      8
#define REG_R9      9
#define REG_R10     10
#define REG_R11     11
#define REG_R12     12
#define REG_R13     13
#define REG_R14     14
#define REG_R15     15
#define REG_RIP     16

#define xpGPRvector(x) ((DWORD64 *)(&(x)->Rax))
#define xpGPR(x,gprno) (xpGPRvector(x)[gprno])
#define xpPC(x) xpGPR(x,Iip)
#define eflags_register(xp) xp->EFlags
#define xpMXCSRptr(x) (DWORD *)(&(x->MxCsr))
#define xpMXCSR(x) ((x)->MxCsr)

#define SIGNUM_FOR_INTN_TRAP SIGSEGV /* Also fake */
#define IS_MAYBE_INT_TRAP(info,xp) \
  ((info->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) &&       \
   (info->ExceptionInformation[0]==0) &&                       \
   (info->ExceptionInformation[1]==(ULONG_PTR)(-1L)))
#define IS_PAGE_FAULT(info,xp) (1)
#define SIGRETURN(context)      /* for now */

#include "os-windows.h"
