/***************************************************************************
 *   Xtensa debug module API                                               *
 *   Copyright (C) 2019 Espressif Systems Ltd.                             *
 *   <alexey@espressif.com>                                                *
 *                                                                         *
 *   Derived from original ESP8266 target.                                 *
 *   Copyright (C) 2015 by Angus Gratton                                   *
 *   gus@projectgus.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/
#ifndef __XTENSA_DEBUG_MODULE_H__
#define __XTENSA_DEBUG_MODULE_H__   1

#include <jtag/jtag.h>
#include "target.h"

/* Virtual IDs for using with xtensa_power_ops API */
#define DMREG_PWRCTL       0x00
#define DMREG_PWRSTAT      0x01

/*
 From the manual:
 To properly use Debug registers through JTAG, software must ensure that:
 - Tap is out of reset
 - Xtensa Debug Module is out of reset
 - Other bits of PWRCTL are set to their desired values, and finally
 - JtagDebugUse transitions from 0 to 1
 The bit must continue to be 1 in order for JTAG accesses to the Debug
 Module to happen correctly. When it is set, any write to this bit clears it.
 Either don't access it, or re-write it to 1 so JTAG accesses continue.
*/
#define PWRCTL_JTAGDEBUGUSE (1<<7)
#define PWRCTL_DEBUGRESET   (1<<6)
#define PWRCTL_CORERESET    (1<<4)
#define PWRCTL_DEBUGWAKEUP  (1<<2)
#define PWRCTL_MEMWAKEUP    (1<<1)
#define PWRCTL_COREWAKEUP   (1<<0)

#define PWRSTAT_DEBUGWASRESET   (1<<6)
#define PWRSTAT_COREWASRESET    (1<<4)
#define PWRSTAT_CORESTILLNEEDED (1<<3)
#define PWRSTAT_DEBUGDOMAINON   (1<<2)
#define PWRSTAT_MEMDOMAINON     (1<<1)
#define PWRSTAT_COREDOMAINON    (1<<0)

/* *** NAR addresses (also used as IDs for debug registers in xtensa_debug_ops API) ***
 *TRAX registers */
#define NARADR_TRAXID       0x00
#define NARADR_TRAXCTRL     0x01
#define NARADR_TRAXSTAT     0x02
#define NARADR_TRAXDATA     0x03
#define NARADR_TRAXADDR     0x04
#define NARADR_TRIGGERPC    0x05
#define NARADR_PCMATCHCTRL  0x06
#define NARADR_DELAYCNT     0x07
#define NARADR_MEMADDRSTART 0x08
#define NARADR_MEMADDREND   0x09
/*Performance monitor registers */
#define NARADR_PMG          0x20
#define NARADR_INTPC        0x24
#define NARADR_PM0          0x28
/*... */
#define NARADR_PM7          0x2F
#define NARADR_PMCTRL0      0x30
/*... */
#define NARADR_PMCTRL7      0x37
#define NARADR_PMSTAT0      0x38
/*... */
#define NARADR_PMSTAT7      0x3F
/*OCD registers */
#define NARADR_OCDID        0x40
#define NARADR_DCRCLR       0x42
#define NARADR_DCRSET       0x43
#define NARADR_DSR          0x44
#define NARADR_DDR          0x45
#define NARADR_DDREXEC      0x46
#define NARADR_DIR0EXEC     0x47
#define NARADR_DIR0         0x48
#define NARADR_DIR1         0x49
/*... */
#define NARADR_DIR7         0x4F
/*Misc registers */
#define NARADR_PWRCTL       0x58
#define NARADR_PWRSTAT      0x69
#define NARADR_ERISTAT      0x5A
/*CoreSight registers */
#define NARADR_ITCTRL       0x60
#define NARADR_CLAIMSET     0x68
#define NARADR_CLAIMCLR     0x69
#define NARADR_LOCKACCESS   0x6c
#define NARADR_LOCKSTATUS   0x6d
#define NARADR_AUTHSTATUS   0x6e
#define NARADR_DEVID        0x72
#define NARADR_DEVTYPE      0x73
#define NARADR_PERID4       0x74
/*... */
#define NARADR_PERID7       0x77
#define NARADR_PERID0       0x78
/*... */
#define NARADR_PERID3       0x7b
#define NARADR_COMPID0      0x7c
/*... */
#define NARADR_COMPID3      0x7f
#define NARADR_MAX          NARADR_COMPID3

/*OCD registers, bit definitions */
#define OCDDCR_ENABLEOCD        (1<<0)
#define OCDDCR_DEBUGINTERRUPT   (1<<1)
#define OCDDCR_INTERRUPTALLCONDS    (1<<2)
#define OCDDCR_BREAKINEN        (1<<16)
#define OCDDCR_BREAKOUTEN       (1<<17)
#define OCDDCR_DEBUGSWACTIVE    (1<<20)
#define OCDDCR_RUNSTALLINEN     (1<<21)
#define OCDDCR_DEBUGMODEOUTEN   (1<<22)
#define OCDDCR_BREAKOUTITO      (1<<24)
#define OCDDCR_BREAKACKITO      (1<<25)

#define OCDDSR_EXECDONE         (1<<0)
#define OCDDSR_EXECEXCEPTION    (1<<1)
#define OCDDSR_EXECBUSY         (1<<2)
#define OCDDSR_EXECOVERRUN      (1<<3)
#define OCDDSR_STOPPED          (1<<4)
#define OCDDSR_COREWROTEDDR     (1<<10)
#define OCDDSR_COREREADDDR      (1<<11)
#define OCDDSR_HOSTWROTEDDR     (1<<14)
#define OCDDSR_HOSTREADDDR      (1<<15)
#define OCDDSR_DEBUGPENDBREAK   (1<<16)
#define OCDDSR_DEBUGPENDHOST    (1<<17)
#define OCDDSR_DEBUGPENDTRAX    (1<<18)
#define OCDDSR_DEBUGINTBREAK    (1<<20)
#define OCDDSR_DEBUGINTHOST     (1<<21)
#define OCDDSR_DEBUGINTTRAX     (1<<22)
#define OCDDSR_RUNSTALLTOGGLE   (1<<23)
#define OCDDSR_RUNSTALLSAMPLE   (1<<24)
#define OCDDSR_BREACKOUTACKITI  (1<<25)
#define OCDDSR_BREAKINITI       (1<<26)
#define OCDDSR_DBGMODPOWERON    (1U<<31)

#define DEBUGCAUSE_IC           (1<<0)	/*ICOUNT exception */
#define DEBUGCAUSE_IB           (1<<1)	/*IBREAK exception */
#define DEBUGCAUSE_DB           (1<<2)	/*DBREAK exception */
#define DEBUGCAUSE_BI           (1<<3)	/*BREAK instruction encountered */
#define DEBUGCAUSE_BN           (1<<4)	/*BREAK.N instruction encountered */
#define DEBUGCAUSE_DI           (1<<5)	/*Debug Interrupt */

#define TRAXCTRL_TREN           (1<<0)	/*Trace enable. Tracing starts on 0->1 */
#define TRAXCTRL_TRSTP          (1<<1)	/*Trace Stop. Make 1 to stop trace. */
#define TRAXCTRL_PCMEN          (1<<2)	/*PC match enable */
#define TRAXCTRL_PTIEN          (1<<4)	/*Processor-trigger enable */
#define TRAXCTRL_CTIEN          (1<<5)	/*Cross-trigger enable */
#define TRAXCTRL_TMEN           (1<<7)	/*Tracemem Enable. Always set. */
#define TRAXCTRL_CNTU           (1<<9)	/*Post-stop-trigger countdown units; selects when
					 * DelayCount-- happens. */
					/*0 - every 32-bit word written to tracemem, 1 - every cpu
					 * instruction */
#define TRAXCTRL_TSEN           (1<<11)	/*Undocumented/deprecated? */
#define TRAXCTRL_SMPER_SHIFT    12	/*Send sync every 2^(9-smper) messages. 7=reserved, 0=no
					 * sync msg */
#define TRAXCTRL_SMPER_MASK     0x7	/*Synchronization message period */
#define TRAXCTRL_PTOWT          (1<<16)	/*Processor Trigger Out (OCD halt) enabled when stop
					 * triggered */
#define TRAXCTRL_PTOWS          (1<<17)	/*Processor Trigger Out (OCD halt) enabled when trace stop
					 * completes */
#define TRAXCTRL_CTOWT          (1<<20)	/*Cross-trigger Out enabled when stop triggered */
#define TRAXCTRL_CTOWS          (1<<21)	/*Cross-trigger Out enabled when trace stop completes */
#define TRAXCTRL_ITCTO          (1<<22)	/*Integration mode: cross-trigger output */
#define TRAXCTRL_ITCTIA         (1<<23)	/*Integration mode: cross-trigger ack */
#define TRAXCTRL_ITATV          (1<<24)	/*replaces ATID when in integration mode: ATVALID output */
#define TRAXCTRL_ATID_MASK      0x7F	/*ARB source ID */
#define TRAXCTRL_ATID_SHIFT     24
#define TRAXCTRL_ATEN           (1U<<31)	/*ATB interface enable */

#define TRAXSTAT_TRACT          (1<<0)	/*Trace active flag. */
#define TRAXSTAT_TRIG           (1<<1)	/*Trace stop trigger. Clears on TREN 1->0 */
#define TRAXSTAT_PCMTG          (1<<2)	/*Stop trigger caused by PC match. Clears on TREN 1->0 */
#define TRAXSTAT_PJTR           (1<<3)	/*JTAG transaction result. 1=err in preceding jtag
					 * transaction. */
#define TRAXSTAT_PTITG          (1<<4)	/*Stop trigger caused by Processor Trigger Input. Clears on
					 * TREN 1->0 */
#define TRAXSTAT_CTITG          (1<<5)	/*Stop trigger caused by Cross-Trigger Input. Clears on TREN
					 * 1->0 */
#define TRAXSTAT_MEMSZ_SHIFT    8	/*Traceram size inducator. Usable trace ram is 2^MEMSZ
					 * bytes. */
#define TRAXSTAT_MEMSZ_MASK     0x1F
#define TRAXSTAT_PTO            (1<<16)	/*Processor Trigger Output: current value */
#define TRAXSTAT_CTO            (1<<17)	/*Cross-Trigger Output: current value */
#define TRAXSTAT_ITCTOA         (1<<22)	/*Cross-Trigger Out Ack: current value */
#define TRAXSTAT_ITCTI          (1<<23)	/*Cross-Trigger Input: current value */
#define TRAXSTAT_ITATR          (1<<24)	/*ATREADY Input: current value */

#define TRAXADDR_TADDR_SHIFT    0	/*Trax memory address, in 32-bit words. */
#define TRAXADDR_TADDR_MASK     0x1FFFFF/*Actually is only as big as the trace buffer size max addr.
					 * */
#define TRAXADDR_TWRAP_SHIFT    21	/*Amount of times TADDR has overflown */
#define TRAXADDR_TWRAP_MASK     0x3FF
#define TRAXADDR_TWSAT          (1U<<31)	/*1 if TWRAP has overflown, clear by disabling tren.
						 **/

#define PCMATCHCTRL_PCML_SHIFT  0	/*Amount of lower bits to ignore in pc trigger register */
#define PCMATCHCTRL_PCML_MASK   0x1F
#define PCMATCHCTRL_PCMS        (1U<<31)	/*PC Match Sense, 0 - match when procs PC is in-range, 1 -
						* match when */
/*out-of-range */

#define XTENSA_MAX_PERF_COUNTERS    2
#define XTENSA_MAX_PERF_SELECT      32
#define XTENSA_MAX_PERF_MASK        0xffff

struct xtensa_debug_module;

struct xtensa_debug_ops {
	/** enable operation */
	int (*queue_enable)(struct xtensa_debug_module *dm);
	/** register read. */
	int (*queue_reg_read)(struct xtensa_debug_module *dm, unsigned reg, uint8_t *data);
	/** register write. */
	int (*queue_reg_write)(struct xtensa_debug_module *dm, unsigned reg, uint32_t data);
};

struct xtensa_power_ops {
	/** register read. */
	int (*queue_reg_read)(struct xtensa_debug_module *dm, unsigned reg, uint8_t *data,
		uint8_t clear);
	/** register write. */
	int (*queue_reg_write)(struct xtensa_debug_module *dm, unsigned reg, uint8_t data);
};

typedef uint8_t xtensa_pwrstat_t;
typedef uint32_t xtensa_ocdid_t;
typedef uint32_t xtensa_dsr_t;
typedef uint32_t xtensa_traxstat_t;


struct xtensa_power_status {
	xtensa_pwrstat_t stat;
	xtensa_pwrstat_t stath;
	/* TODO: do not need to keep previous status to detect that core or debug module has been
	 * reset, */
	/*       we can clear PWRSTAT_DEBUGWASRESET and PWRSTAT_COREWASRESET after reading will do
	 * the job; */
	/*       upon next reet those bits will be set again. So we can get rid of
	 *       xtensa_dm_power_status_cache_reset() and xtensa_dm_power_status_cache(). */
	xtensa_pwrstat_t prev_stat;
};

struct xtensa_core_status {
	xtensa_dsr_t dsr;
};

struct xtensa_trace_config {
	uint32_t ctrl;
	uint32_t memaddr_start;
	uint32_t memaddr_end;
	uint32_t addr;
};

struct xtensa_trace_status {
	xtensa_traxstat_t stat;
};

struct xtensa_trace_start_config {
	uint32_t stoppc;
	bool after_is_words;
	uint32_t after;
	uint32_t stopmask;	/* -1: disable PC match option */
};

struct xtensa_perfmon_config {
	int select;
	uint32_t mask;
	int kernelcnt;
	int tracelevel;
};

struct xtensa_perfmon_result {
	uint64_t value;
	bool overflow;
};

struct xtensa_debug_module_config {
	const struct xtensa_power_ops *pwr_ops;
	const struct xtensa_debug_ops *dbg_ops;
	struct jtag_tap *tap;
	void (*queue_tdi_idle)(struct target *target);
	void *queue_tdi_idle_arg;
};

struct xtensa_debug_module {
	const struct xtensa_power_ops *pwr_ops;
	const struct xtensa_debug_ops *dbg_ops;
	struct jtag_tap *tap;
	void (*queue_tdi_idle)(struct target *target);
	void *queue_tdi_idle_arg;

	struct xtensa_power_status power_status;
	struct xtensa_core_status core_status;
	xtensa_ocdid_t device_id;
};


int xtensa_dm_init(struct xtensa_debug_module *dm, const struct xtensa_debug_module_config *cfg);
int xtensa_dm_queue_enable(struct xtensa_debug_module *dm);
int xtensa_dm_queue_reg_read(struct xtensa_debug_module *dm, unsigned reg, uint8_t *value);
int xtensa_dm_queue_reg_write(struct xtensa_debug_module *dm, unsigned reg, uint32_t value);
int xtensa_dm_queue_pwr_reg_read(struct xtensa_debug_module *dm,
	unsigned reg,
	uint8_t *data,
	uint8_t clear);
int xtensa_dm_queue_pwr_reg_write(struct xtensa_debug_module *dm, unsigned reg, uint8_t data);

static inline void xtensa_dm_queue_tdi_idle(struct xtensa_debug_module *dm)
{
	if (dm->queue_tdi_idle)
		dm->queue_tdi_idle(dm->queue_tdi_idle_arg);
}

int xtensa_dm_power_status_read(struct xtensa_debug_module *dm, uint32_t clear);
static inline void xtensa_dm_power_status_cache_reset(struct xtensa_debug_module *dm)
{
	dm->power_status.prev_stat = 0;
}
static inline void xtensa_dm_power_status_cache(struct xtensa_debug_module *dm)
{
	dm->power_status.prev_stat = dm->power_status.stath;
}
static inline xtensa_pwrstat_t xtensa_dm_power_status_get(struct xtensa_debug_module *dm)
{
	return dm->power_status.stat;
}

int xtensa_dm_core_status_read(struct xtensa_debug_module *dm);
int xtensa_dm_core_status_clear(struct xtensa_debug_module *dm, xtensa_dsr_t bits);
int xtensa_dm_core_status_check(struct xtensa_debug_module *dm);
static inline xtensa_dsr_t xtensa_dm_core_status_get(struct xtensa_debug_module *dm)
{
	return dm->core_status.dsr;
}

int xtensa_dm_device_id_read(struct xtensa_debug_module *dm);
static inline xtensa_ocdid_t xtensa_dm_device_id_get(struct xtensa_debug_module *dm)
{
	return dm->device_id;
}

int xtensa_dm_trace_start(struct xtensa_debug_module *dm, struct xtensa_trace_start_config *cfg);
int xtensa_dm_trace_stop(struct xtensa_debug_module *dm, bool pto_enable);
int xtensa_dm_trace_config_read(struct xtensa_debug_module *dm, struct xtensa_trace_config *config);
int xtensa_dm_trace_status_read(struct xtensa_debug_module *dm, struct xtensa_trace_status *status);
int xtensa_dm_trace_data_read(struct xtensa_debug_module *dm, uint8_t *dest, uint32_t size);

static inline bool xtensa_dm_is_online(struct xtensa_debug_module *dm)
{
	int res = xtensa_dm_device_id_read(dm);
	if (res != ERROR_OK)
		return false;
	return (dm->device_id != 0xffffffff && dm->device_id != 0);
}

static inline bool xtensa_dm_tap_was_reset(struct xtensa_debug_module *dm)
{
	return (!(dm->power_status.prev_stat & PWRSTAT_DEBUGWASRESET) &&
		dm->power_status.stat & PWRSTAT_DEBUGWASRESET);
}

static inline bool xtensa_dm_core_was_reset(struct xtensa_debug_module *dm)
{
	return (!(dm->power_status.prev_stat & PWRSTAT_COREWASRESET) &&
		dm->power_status.stat & PWRSTAT_COREWASRESET);
}

static inline bool xtensa_dm_core_is_stalled(struct xtensa_debug_module *dm)
{
	return dm->core_status.dsr & OCDDSR_RUNSTALLSAMPLE;
}

static inline bool xtensa_dm_is_powered(struct xtensa_debug_module *dm)
{
	return dm->core_status.dsr & OCDDSR_DBGMODPOWERON;
}

int xtensa_dm_perfmon_enable(struct xtensa_debug_module *dm, int counter_id,
	const struct xtensa_perfmon_config *config);
int xtensa_dm_perfmon_dump(struct xtensa_debug_module *dm, int counter_id,
	struct xtensa_perfmon_result *out_result);

#endif	/*__XTENSA_DEBUG_MODULE_H__*/
