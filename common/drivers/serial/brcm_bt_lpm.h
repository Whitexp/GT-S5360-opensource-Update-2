/*******************************************************************************
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
* 	@file	drivers/serial/brcm_bt_lpm.h
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/

#ifndef __ASM_BRCM_BT_LPM_H
#define __ASM_BRCM_BT_LPM_H

#include <linux/serial_core.h>

/* may need some clock locking mechanism hook to inhibit or allow sleep */
struct bcm_bt_lpm_platform_data {
	unsigned int gpio_bt_wake;	/* HOST -> BCM chip wakeup gpio */
	unsigned int gpio_host_wake;	/* BCM chip -> HOST wakeup gpio */
};

struct bcm_bt_lpm_data {
    unsigned int        gpio_bt_wake;  /* HOST -> BCM chip wakeup gpio */
    unsigned int        gpio_host_wake;    /* BCM chip -> HOST wakeup gpio */
    /*spinlock_t          lock; */
#ifdef CONFIG_HAS_WAKELOCK
    struct wake_lock    host_wake_lock;     /* rx path */
    struct wake_lock    bt_wake_lock;       /* tx path */
#endif
    int                 bt_wake_installed;
    int                 host_wake_installed;
};

extern const struct bcm_bt_lpm_platform_data brcm_bt_lpm_data;
extern int serial8250_ioctl(struct uart_port *port, unsigned int cmd,
			    unsigned long arg);
extern int brcm_init_bt_wake(const struct bcm_bt_lpm_platform_data *gpio_data);
extern int brcm_init_hostwake(const struct bcm_bt_lpm_platform_data *gpio_data);

/* FIXME: the following defines should go into some header file depending on platform used, eg. board config
 * The following defines are used for LPM BT_WAKE handling. Make sure polarity is in sync with HCI command settings
 */
#define TIO_ASSERT_BT_WAKE      0x8003
#define TIO_DEASSERT_BT_WAKE    0x8004
#define TIO_GET_BT_WAKE_STATE   0x8005


#if defined(CONFIG_BOARD_BRAVA_EDN10)
#define GPIO_BT_WAKE 14
#define GPIO_HOST_WAKE 18
#define GPIO_UARTB_SEL 6
#define GPIO_BT_CLK32K_EN 31
#endif

#if defined(CONFIG_BOARD_THUNDERBIRD_EDN31)
#define GPIO_BT_WAKE 29
#define GPIO_HOST_WAKE 28
#define GPIO_UARTB_SEL (-1)
#define GPIO_BT_CLK32K_EN (-1)
#endif

#if defined(CONFIG_BOARD_THUNDERBIRD_EDN5x)
#define GPIO_BT_WAKE 29
#define GPIO_HOST_WAKE 28
#define GPIO_UARTB_SEL (-1)
#define GPIO_BT_CLK32K_EN (-1)
#endif

#if defined(CONFIG_BOARD_TOTORO)
#define GPIO_BT_WAKE 39
#define GPIO_HOST_WAKE 38
#define GPIO_UARTB_SEL (-1)
#define GPIO_BT_CLK32K_EN (-1)
#endif

#if defined(CONFIG_BOARD_LUISA)
#define GPIO_BT_WAKE 39
#define GPIO_HOST_WAKE 38
#define GPIO_UARTB_SEL (-1)
#define GPIO_BT_CLK32K_EN (-1)
#endif

#if defined(CONFIG_BOARD_TASSVE)
#define GPIO_BT_WAKE 39
#define GPIO_HOST_WAKE 38
#define GPIO_UARTB_SEL (-1)
#define GPIO_BT_CLK32K_EN (-1)
#endif

#if defined(CONFIG_BOARD_COOPERVE)
#define GPIO_BT_WAKE 39
#define GPIO_HOST_WAKE 38
#define GPIO_UARTB_SEL (-1)
#define GPIO_BT_CLK32K_EN (-1)
#endif


#ifndef GPIO_BT_WAKE
#define GPIO_BT_WAKE (-1)
#endif
#ifndef GPIO_HOST_WAKE
#define GPIO_HOST_WAKE (-1)
#endif
#ifndef GPIO_UARTB_SEL
#define GPIO_UARTB_SEL (-1)
#endif
#ifndef GPIO_BT_CLK32K_EN
#define GPIO_BT_CLK32K_EN (-1)
#endif


/*#define GPIO_BT_RESET 26                      - not needed for Brava */
//#define GPIO_BT_VREG_CTL 28

/* this define electrical level of GPIO for assert/de-asserted stated. sleep logic has by default negative
   logic */

#ifndef BT_WAKE_ASSERT
#define BT_WAKE_ASSERT 0
#endif
#ifndef BT_WAKE_DEASSERT
#define BT_WAKE_DEASSERT (!(BT_WAKE_ASSERT))
#endif

#ifndef HOST_WAKE_ASSERT
#define HOST_WAKE_ASSERT 0
#endif
#ifndef HOST_WAKE_DEASSERT
#define HOST_WAKE_DEASSERT (!(HOST_WAKE_ASSERT))
#endif

#endif
