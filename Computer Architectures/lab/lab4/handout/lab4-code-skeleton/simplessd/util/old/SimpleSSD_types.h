/**
 * This file is part of SimpleSSD.
 *
 * SimpleSSD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Jie Zhang <jie@camelab.org>
 *          Donghyun Gouk <kukdh1@camelab.org>
 */

#ifndef __ssdsim_types_h__
#define __ssdsim_types_h__

#include <cinttypes>

/*==============================
Switches
==============================*/

// Option Switch
#define DMA_PREEMPTION 1  // For distingushing before-implementation. On/off is available at config file.
#define ENABLE_FTL 1   //FTL integration


// DBG Print Switch
#define DBG_PRINT 1
#if DBG_PRINT
#define DBG_PRINT_PPN 0
#define DBG_PRINT_TICK 0
#define DBG_PRINT_CHANNEL 0
#define DBG_PRINT_BUSY 0
#define DBG_PRINT_REQSTART 1
#define DBG_PRINT_REQDONE 1
#define DBG_PRINT_CONFLICT 0
#define DBG_PRINT_CONFIGPARSER 0
#define DBG_PRINT_REQUEST 1
#endif

#define LOG_PRINT_ABSOLUTE_TIME 0
#define LOG_PRINT_CONSUMED_TIME 0

#define LOG_PRINT_OCCUPY_EACH 0

#define GATHER_TIME_SERIES 0
#define GATHER_RESOURCE_CONFLICT 1
#define FULL_VERIFY_TIMELINE 0 // don't flush timeline for verification at the end
#define HARD_VERIFY_TIMELINE 0 // Verify PAL Timeline on every update! ... CAUCTION: takes VERY LONG time


/*==============================
Strings
==============================*/
extern char ADDR_STRINFO[][10];
extern char ADDR_STRINFO2[][15];
extern char OPER_STRINFO[][10];
extern char OPER_STRINFO2[][10];
extern char BUSY_STRINFO[][10];
extern char PAGE_STRINFO[][10];
extern char NAND_STRINFO[][10];
#if GATHER_RESOURCE_CONFLICT
extern char CONFLICT_STRINFO[][10];
#endif

typedef uint64_t Tick;
typedef uint64_t Addr;

/*==============================
Macros
==============================*/
//#define GET_ACCESSOR(propName, propType) propType get##propName() const { return m_##propName; }
//#define SET_ACCESSOR(propName, propType) void set##propName(const propType &newVal) { m_##propName = newVal; }

#define CREATE_ACCESSOR(propType, propName) \
private: \
propType propName; \
public: \
void set##propName(const propType newVal) { propName = newVal; }; \
propType get##propName() const { return propName; };

#ifdef _MSC_VER
#define printa(fmt, ...)    do { printf(fmt, __VA_ARGS__); if (outfp) fprintf(outfp, fmt, __VA_ARGS__); } while(0);
#define printo(fmt, ...)    do { if (!outfp) printf(fmt, __VA_ARGS__); if(outfp) fprintf(outfp, fmt, __VA_ARGS__); } while(0);
#define printft(fmt, ...)   do { printf("    "); printf(fmt, __VA_ARGS__); } while(0);
#else
#define printa(fmt...)    do { printf(fmt); if (outfp) fprintf(outfp, fmt); } while(0);
#define printo(fmt...)    do { if (!outfp) printf(fmt); if(outfp) fprintf(outfp, fmt); } while(0);
#define printft(fmt...)   do { printf("    "); printf(fmt); } while(0);
#endif

//#define ERR_EXIT(fmt...) {printf(fmt); std::terminate();}
#define SAFEDIV(left,right) ((right)==0?0:(left)/(right))


/*==============================
Type & Struct
==============================*/
#define MAX64 0xFFFFFFFFFFFFFFFF
#define MAX32 0xFFFFFFFF

#define  BYTE (1)
#define KBYTE (1024)
#define MBYTE (1024*KBYTE)
#define GBYTE (1024*MBYTE)
#define TBYTE (1024*GBYTE)

#define SEC  (1)
#define MSEC (1000)      // 1 000
#define USEC (1000*MSEC) // 1 000 000
#define NSEC ((uint64_t)1000*USEC) // 1 000 000 000
#define PSEC ((uint64_t)1000*NSEC) // 1 000 000 000 000

//===== Address Sequence =====
enum {
  ADDR_CHANNEL = 0,
  ADDR_PACKAGE = 1,
  ADDR_DIE     = 2,
  ADDR_PLANE   = 3,
  ADDR_BLOCK   = 4,
  ADDR_PAGE    = 5,
  ADDR_NUM
};

enum {
  FTL_PAGE_MAPPING,
  FTL_BLOCK_MAPPING,
  FTL_HYBRID_MAPPING
};

enum {
  CACHE_EVICT_SUPER_PAGE,
  CACHE_EVICT_SUPER_BLOCK
};

//===== Operation Types =====
typedef enum _PAL_OPERATION {
  OPER_READ  = 0,
  OPER_WRITE = 1,
  OPER_ERASE = 2,
  OPER_NUM
} PAL_OPERATION;

//===== Busy Types =====
enum {
  BUSY_DMA0WAIT,
  BUSY_DMA0,
  BUSY_MEM,
  BUSY_DMA1WAIT,
  BUSY_DMA1,
  BUSY_END,
  BUSY_NUM
};


//===== Log-purpose: Tick types =====
enum {
  TICK_DMA0WAIT = 0, // == TICK_IOREQUESTED
  TICK_DMA0,
  TICK_MEM,
  TICK_DMA1WAIT,
  TICK_DMA1,
  TICK_IOEND,
  TICK_NUM
};

//===== NAND Flash - Page address kind =====
enum {
  PAGE_LSB = 0,
  PAGE_CSB = 1,
  PAGE_MSB = 2,
  PAGE_NUM
};

//===== NAND Flash type =====
enum {
  NAND_SLC,
  NAND_MLC,
  NAND_TLC,
  NAND_NUM
};

//===== Request2 Status type =====
enum
{
  REQSTAT_NEW,
  REQSTAT_PROC,
  REQSTAT_END
};


#if GATHER_RESOURCE_CONFLICT
enum
{
  CONFLICT_NONE = 0,
  CONFLICT_DMA0 = 1<<0, //DMA0 couldn't start due to CH  BUSY --- exclusive to CONFLICT_MEM
  CONFLICT_MEM  = 1<<1, //DMA0 couldn't start due to MEM BUSY --- exclusive to CONFLICT_DMA0
  CONFLICT_DMA1 = 1<<2, //DMA1 couldn't start due to CH  BUSY
  CONFLICT_NUM  = 4 //0~3 = 4
};
#endif

/*
Todo: Make those structs to class?, to check address limitation & warn.
*/

//===== Divided Address =====
typedef struct _CPDPBP
{
  uint32_t Channel;
  uint32_t Package;
  uint32_t Die;
  uint32_t Plane;
  uint32_t Block;
  uint32_t Page;
}CPDPBP;

//===== PPN Request Info (would be in the queue) =====
class RequestFTL
{
public:
  uint64_t PPN;
  uint8_t  Oper;
  uint64_t TickRequested;
};

//===== Task which can be assign to Channel or Memory =====
class Task
{
public:
  uint64_t PPN;
  CPDPBP CPD;
  uint32_t PlaneIdx;
  uint8_t  Oper;
  uint8_t  Busy;
  #if DMA_PREEMPTION
  uint64_t DMASuspend;   // 0 = no suspend, true = suspended & left time
  #endif
  uint64_t TickStart[TICK_NUM]; // End Time of each BUSY status, [BUSY_NUM] = END_TIME, actually not needed for simulation itself.
  uint64_t TickNext;
};

//===== PPN Request Info (would be in the queue) =====
class RequestLL
{
public:
  uint64_t PPN;
  uint8_t  Oper;
  uint64_t TickRequested;
  uint64_t TickFinished;

  CPDPBP CPD;
  uint8_t status; // 0-New, 1-Fetched, 2-Finished
  RequestLL* LLprev; //Dual-Linked-List
  RequestLL* LLnext; //Dual-Linked-List
};

//===== Task which can be assign to Channel or Memory =====
class TaskLL : Task
{
public:
  RequestLL* SrcRequest; //Dual-Linked-List
};

#endif //__ssdsim_types_h__
