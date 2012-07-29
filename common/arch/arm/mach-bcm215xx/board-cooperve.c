/*******************************************************************************
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
*	@file	arch/arm/mach-bcm215xx/board-cooperve.c
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

/*
 * AthenaRay board specific driver definitions
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/cpufreq.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <mach/reg_irq.h>

#include <linux/serial_8250.h>
#include <mach/hardware.h>
#include <mach/reg_syscfg.h>
#include <mach/reg_auxmic.h>
#include <asm/mach/map.h>
#include <mach/setup.h>
#include <mach/gpt.h>
#include <linux/broadcom/gpt.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#ifdef CONFIG_BROADCOM_WIFI_RESERVED_MEM
#include <linux/wlan_plat.h>
#include <linux/skbuff.h>
#endif

#if defined (CONFIG_ANDROID_PMEM)
#include <linux/android_pmem.h>
#include <linux/dma-mapping.h>
#if defined (CONFIG_BMEM)
#define PMEM_ADSP_SIZE (2 * PAGE_SIZE)
#else
#define PMEM_ADSP_SIZE (1024 * 1024 * 8)
#endif
#endif

#ifdef CONFIG_MMC_BCM
#include <mach/sdio.h>
#include <mach/reg_clkpwr.h>
#endif

#if defined(CONFIG_MFD_MAX8986)
#include <linux/power_supply.h>
#include <linux/mfd/max8986/max8986.h>
#include <linux/broadcom/max8986/max8986-audio.h>
#endif

#ifdef CONFIG_USB_DWC_OTG
#include <mach/usbctl.h>
#endif
#ifdef CONFIG_KEYBOARD_BCM
#include <mach/bcm_keymap.h>
#include <plat/bcm_keypad.h>
#endif
#ifdef CONFIG_BACKLIGHT_PWM
#include <linux/pwm_backlight.h>
#endif

#ifdef CONFIG_BACKLIGHT_AAT1401
#include <linux/aat1401_bl.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_QT602240
#include <linux/i2c/qt602240_ts.h>
#endif

#ifdef CONFIG_I2C_GPIO
#include <linux/i2c-gpio.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_CYTTSP_I2C
#include <linux/input/cyttsp.h>
 #endif
 
#if defined (CONFIG_BCM_AUXADC)
#include <plat/bcm_auxadc.h>
#endif
#include "device.h"

#ifdef CONFIG_BCM_GPIO_VIBRATOR
#include <linux/timed_gpio.h>
#endif

#if defined (CONFIG_INPUT_BMA150_SMB380)
#include <linux/bma150.h>
#endif

#if defined(CONFIG_BRCM_HEADSET)  || defined(CONFIG_BRCM_HEADSET_MODULE)
#include <plat/brcm_headset_pd.h>
#endif

#include <linux/spinlock.h>
#include <linux/spi/spi.h>

#ifdef CONFIG_SPI
#include <plat/bcm_spi.h>
#endif

#include <plat/syscfg.h>
#include <plat/timer.h>

#if defined (CONFIG_BCM_PWM)
#include <plat/bcm_pwm_block.h>
#endif

#include <plat/bcm_i2c.h>

#ifdef CONFIG_BCM215XX_DSS
#include <plat/bcm_lcdc.h>
#endif

#define BCM2091_CLK_REQ_PASS_THOUGH 37
#define GPS_CNTIN_CLK_ENABLE 36
#define GPIO_HEADSET_BUTTON 42
#define SYSCFG_IOCR2_ANA_SYSCLKEN_MUX_MASK 0x00040000

#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))
#include <linux/broadcom/bcmblt-rfkill.h>
#include <plat/bcm_rfkill.h>
#endif

#include <linux/broadcom/types.h>
#include <linux/broadcom/bcmtypes.h>

#if defined(CONFIG_BRCM_FUSE_SYSPARM)
#include <linux/broadcom/bcm_fuse_sysparm.h>
#endif

#if defined(CONFIG_USB_ANDROID)
#include <linux/usb/android.h>
#endif

#define FREQ_MHZ(mhz)		((mhz)*1000UL*1000UL)
#define FREQ_KHZ(khz)		((khz)*1000UL)

/*
 * BITMAP to indicate the available GPTs on AP
 * Reset the bitmap to indicate the GPT not to be used on AP
 */
#define GPT_AVAIL_BITMAP         0x3F

#define BATT_FULL_VOLT		4200
#define BATT_LEVEL5_VOLT	3960
#define BATT_LEVEL4_VOLT	3860
#define BATT_LEVEL3_VOLT	3770
#define BATT_LEVEL2_VOLT	3730
#define BATT_LEVEL1_VOLT	3680
#define BATT_LEVEL0_VOLT	3530
#define BATT_LEVEL0_1_VOLT	3480
#define BATT_LOW_VOLT		3400

extern int pmu_is_charger_inserted();
#define KEY_PRESS_THRESHOLD	0x6d00
#define KEY_3POLE_THRESHOLD	56	// ~ 55 and 57 ~, measured on Totoro
#define KEY1_THRESHOLD_L	4	// 9 to 83, mesaured on Luisa
#define KEY1_THRESHOLD_U	100
#define KEY2_THRESHOLD_L	100	// 117 to 183, mesaured on Luisa
#define KEY2_THRESHOLD_U	220
#define KEY3_THRESHOLD_L	220	// 202 to 314, mesaured on Luisa
#define KEY3_THRESHOLD_U	692


extern int bcm_gpio_pull_up(unsigned int gpio, bool up);
extern int bcm_gpio_pull_up_down_enable(unsigned int gpio, bool enable);
void __init gpt_init(struct gpt_base_config *base_config);

//#ifdef CONFIG_BCM_4330_DRIVER

extern void bcmsdhc_sdio_host_force_scan(struct platform_device *pdev, bool on);
void bcm_wlan_Gpioinit_Setting(void);
void bcm_wlan_power_on(int val);
void bcm_wlan_power_off(int val);
int bcm_set_core_power(unsigned int bcm_core, unsigned int pow_on,
		       unsigned int reset);
//#endif /* 4330 */
#ifdef CONFIG_MMC_BCM
extern int bcmsdhc_cfg_card_detect(void __iomem *ioaddr, u8 ctrl_slot);
extern int bcmsdhc_external_reset(void __iomem *ioaddr, u8 ctrl_slot);
extern int bcmsdhc_enable_int(void __iomem *ioaddr, u8 ctrl_slot);
extern void *cam_mempool_base;
int bcmsdhc_set_sys_interface(u8 ctrl_slot);
int bcmsdhc_request_cfg_pullup(void __iomem *ioaddr, u8 ctrl_slot);
int bcmsdhc_finish_cfg_pullup(void __iomem *ioaddr, u8 ctrl_slot);

extern UInt16 SYSPARM_GetDefault4p2VoltReading(void);
extern UInt16 SYSPARM_GetBattEmptyThresh(void);

extern int prev_scaled_level;

static struct bcmsdhc_platform_data bcm21553_sdhc_data1 = {
	.base_clk = FREQ_MHZ(50),
	.flags = SDHC_DEVTYPE_SDIO | SDHC_MANUAL_SUSPEND_RESUME | SDHC_DISABLE_PED_MODE,	
	.cd_pullup_cfg = SDCD_PULLUP | SDCD_UPDOWN_ENABLE,
	.irq_cd = -1,
	.regl_id = "vddo_sdio1",
	.syscfg_interface = board_sysconfig,
	.cfg_card_detect = bcmsdhc_cfg_card_detect,
	.external_reset = bcmsdhc_external_reset,
	.enable_int = bcmsdhc_enable_int,
};

#if !defined(CONFIG_MTD_ONENAND)
/*
 * SDHC2 is used for eMMC and SDHC2 shares pin mux with FLASH(OneNAND)
 * So both OneNAND and SDHC2 cannot co-exist
 */

static struct bcmsdhc_platform_data bcm21553_sdhc_data2 = {
	.base_clk = FREQ_MHZ(50),
	.flags = SDHC_DEVTYPE_EMMC | SDIO_CARD_ALWAYS_PRESENT,
	.cd_pullup_cfg = SDCD_PULLUP | SDCD_UPDOWN_ENABLE,
	.irq_cd = -1,
	.syscfg_interface = board_sysconfig,
	.cfg_card_detect = bcmsdhc_cfg_card_detect,
	.external_reset = bcmsdhc_external_reset,
	.enable_int = bcmsdhc_enable_int,
};
#endif /*CONFIG_MTD_ONENAND */

static struct bcmsdhc_platform_data bcm21553_sdhc_data3 = {
	.base_clk = FREQ_MHZ(50),
	.flags = SDHC_DEVTYPE_SD | SDHC_DISABLE_PED_MODE,
	.cd_pullup_cfg = SDCD_PULLUP | SDCD_UPDOWN_ENABLE,
	.irq_cd = 16,
	.syscfg_interface = board_sysconfig,
	.cfg_card_detect = bcmsdhc_cfg_card_detect,
	.external_reset = bcmsdhc_external_reset,
	.enable_int = bcmsdhc_enable_int,
};
#endif /*CONFIG_MMC_BCM */

//#ifdef CONFIG_BCM_4330_DRIVER

#define BCM4325_BT 1
#define BCM4325_WLAN 2
#define BCM4325_BT_RESET 24		/* BT uses dedicated reset signal, BT_RST_N, GPIO20 */
#define BCM4325_WLAN_RESET 24	/* GPIO24 is dedicated to WL_REG_ON */
#define GPIO_WLAN_BT_REG_ON 24	/* BT reset and WLAN reset are not shared */
// #define SDIO1_WLAN_MUX_TTL 37	/* SDIO MUX is not used in Totoro */
#define BT_VREG_CTRL	21
#define BT_RST_CHECK	20

int bcm_set_core_power(unsigned int bcm_core, unsigned int pow_on,
		       unsigned int reset)
{
	unsigned gpio_rst, gpio_rst_another;

	if ((reset != 2) && (reset != 1)) {
		pr_info("%s: Error!! BAD Argument. ", __FUNCTION__);
		return -EINVAL;
	}

	switch (bcm_core) {
	case BCM4325_BT:
		gpio_rst = BCM4325_BT_RESET;
		gpio_rst_another = BCM4325_WLAN_RESET;
		break;

	case BCM4325_WLAN:
		gpio_rst = BCM4325_WLAN_RESET;
		gpio_rst_another = BCM4325_BT_RESET;
		break;

	default:
		pr_err("bcm_power: Unknown bcm core!\n");
		return -1;
	}

/*    mutex_lock(&bcm4325_pwr_lock); */

	/* Since reg on is coupled, check whether the other core is ON before
	   touching it */
	if ((gpio_get_value(gpio_rst_another) == 0) && ((reset == 1)|| (reset == 2))) {

//		gpio_direction_output(SDIO1_WLAN_MUX_TTL, 0);
//		msleep(500);
		/* enable WLAN_BT_REG_ON */
		gpio_direction_output(GPIO_WLAN_BT_REG_ON, pow_on);
		pr_info
		    ("bcm_power: Set WLAN_BT_REG_ON %s because %s is OFF now.\n",
		     gpio_get_value(GPIO_WLAN_BT_REG_ON) ? "High" : "Low",
		     bcm_core ? "BT" : "WLAN");
		msleep(150);
	}
	/* enable specified core */
	gpio_direction_output(gpio_rst, pow_on);
	pr_info("bcm_power: Set %s %s\n",
		bcm_core ? "WLAN_RESET" : "BT_RESET",
		gpio_get_value(gpio_rst) ? "High [chip out of reset]" :
		"Low [put into reset]");




/*    mutex_unlock(&bcm4325_pwr_lock); */

	return 0;
}

#define BCM_RESET 2
#define BCM_POWER 1
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

void bcm_wlan_Gpioinit_Setting(void)
{

	/*WLAN_RST_N: GPIO24*/
	writel(readl(ADDR_SYSCFG_IOCR6) &
	~(SYSCFG_IOCR6_GPIO25_24_MUX),
	ADDR_SYSCFG_IOCR6);
	writel(readl(ADDR_SYSCFG_IOCR2) &
	~(0x0001),
	ADDR_SYSCFG_IOCR2);

	gpio_request(BCM4325_WLAN_RESET, "gpio_wlan_rst");
	gpio_direction_output(BCM4325_WLAN_RESET, 0);
	gpio_free(BCM4325_WLAN_RESET);

	bcm_gpio_pull_up_down_enable(BCM4325_WLAN_RESET, true);
	bcm_gpio_pull_up(BCM4325_WLAN_RESET, false);

	/*WLAN_HOST_WAKE_UP: GPIO18*/
	/*WLAN_CLK_REQ: GPIO37*/
	/*WLAN_SDIO_SETTING: GPIO52_57*/
	board_sysconfig(SYSCFG_SDHC1, SYSCFG_INIT);

}
/** @addtogroup BoardBravaAPIGroup
	@{
*/
/**
* @brief	Power ON the WLAN
*
* @param val	Start/Power ON
*/
void bcm_wlan_power_on(int val)
{

	int err = 0;
	pr_info("%s: Enter.\n ", __FUNCTION__);



	if( (gpio_get_value(BT_RST_CHECK) == 0) )
	{
		gpio_direction_output(BT_VREG_CTRL, 1);
		msleep(50);		//Changed for bt on time
	}


	switch (val) {
	case BCM_POWER:
		pr_info("SHRI %s: WIFI POWER UP!!  \n ", __FUNCTION__);
		err = bcm_set_core_power(BCM4325_WLAN, TRUE, val);
		if (err < 0)
			pr_info("%s: FAILED!!\n ", __FUNCTION__);
		bcmsdhc_sdio_host_force_scan(&bcm21553_sdhc_slot1, true);
		break;
	case BCM_RESET:
		pr_info("%s: WIFI DRIVER START!!\n ", __FUNCTION__);
		err = bcm_set_core_power(BCM4325_WLAN, TRUE, val);
		if (err < 0)
			pr_info("%s: FAILED!!\n ", __FUNCTION__);
		break;
	default:
		pr_info("%s: INVALID ARG!!\n ", __FUNCTION__);





		}
	/* Note: that the platform device struct has to be exported from where it is defined */
	/* The below function would induce a forced mmc_rescan to detect the newly */
	/* powered up card. */

}


/**
* @brief	Power OFF the WLAN
*
* @param val	Stop/Power OFF
*/
void bcm_wlan_power_off(int val)
{

	int err = 0;
	pr_info("%s: Enter.\n ", __FUNCTION__);



	switch (val) {
	case BCM_POWER:
		pr_info("%s: WIFI POWER DOWN!\n ", __FUNCTION__);
		err = bcm_set_core_power(BCM4325_WLAN, FALSE, val);
		if (err < 0)
			pr_info("%s: FAILED!!\n ", __FUNCTION__);
		bcmsdhc_sdio_host_force_scan(&bcm21553_sdhc_slot1, false);
		break;
	case BCM_RESET:
		pr_info("%s: WIFI DRIVER STOP!!\n ", __FUNCTION__);
		err = bcm_set_core_power(BCM4325_WLAN, FALSE, val);
		if (err < 0)
			pr_info("%s: FAILED!!\n ", __FUNCTION__);
		break;
	default:
		pr_info("%s: INVALID ARG!!\n ", __FUNCTION__);

	}



	if( (gpio_get_value(BT_RST_CHECK) == 0) )
	{
		gpio_direction_output(BT_VREG_CTRL, 0);
		msleep(50);			//Changed for bt on time
	}


	/* Note: that the platform device struct has to be exported from where it is defined */
	/* The below function would induce a forced mmc_rescan to detect the newly */
	/* powered up card. */

}
/** @} */
EXPORT_SYMBOL(bcm_wlan_power_on);
EXPORT_SYMBOL(bcm_wlan_power_off);

#ifdef CONFIG_BROADCOM_WIFI_RESERVED_MEM

#define PREALLOC_WLAN_SEC_NUM          4
#define PREALLOC_WLAN_BUF_NUM          160
#define PREALLOC_WLAN_SECTION_HEADER   24

#define WLAN_SKB_BUF_NUM       17

#define WLAN_SECTION_SIZE_0    (PREALLOC_WLAN_BUF_NUM * 128)
#define WLAN_SECTION_SIZE_1    (PREALLOC_WLAN_BUF_NUM * 128)
#define WLAN_SECTION_SIZE_2    (PREALLOC_WLAN_BUF_NUM * 512)
#define WLAN_SECTION_SIZE_3    (PREALLOC_WLAN_BUF_NUM * 1024)

static struct sk_buff *wlan_static_skb[WLAN_SKB_BUF_NUM];

struct wifi_mem_prealloc {
       void *mem_ptr;
       unsigned long size;
};

static struct wifi_mem_prealloc wifi_mem_array[PREALLOC_WLAN_SEC_NUM] = {
       {NULL, (WLAN_SECTION_SIZE_0 + PREALLOC_WLAN_SECTION_HEADER)},
       {NULL, (WLAN_SECTION_SIZE_1 + PREALLOC_WLAN_SECTION_HEADER)},
       {NULL, (WLAN_SECTION_SIZE_2 + PREALLOC_WLAN_SECTION_HEADER)},
       {NULL, (WLAN_SECTION_SIZE_3 + PREALLOC_WLAN_SECTION_HEADER)}
};

void *wlan_mem_prealloc(int section, unsigned long size)
{
       if (section == PREALLOC_WLAN_SEC_NUM)
               return wlan_static_skb;

       if ((section < 0) || (section > PREALLOC_WLAN_SEC_NUM))
               return NULL;

       if (wifi_mem_array[section].size < size)
               return NULL;

       return wifi_mem_array[section].mem_ptr;
}
EXPORT_SYMBOL (wlan_mem_prealloc);
#define DHD_SKB_HDRSIZE                336
#define DHD_SKB_1PAGE_BUFSIZE  ((PAGE_SIZE*1)-DHD_SKB_HDRSIZE)
#define DHD_SKB_2PAGE_BUFSIZE  ((PAGE_SIZE*2)-DHD_SKB_HDRSIZE)
#define DHD_SKB_4PAGE_BUFSIZE  ((PAGE_SIZE*4)-DHD_SKB_HDRSIZE)

int __init brcm_init_wifi_mem(void)
{
       int i;
       int j;

       printk("brcm_init_wifi_mem...\n");

       for (i = 0; i < 8; i++) {
               wlan_static_skb[i] = dev_alloc_skb(DHD_SKB_1PAGE_BUFSIZE);
               if (!wlan_static_skb[i])
                       goto err_skb_alloc;
       }

       for (; i < 16; i++) {
               wlan_static_skb[i] = dev_alloc_skb(DHD_SKB_2PAGE_BUFSIZE);
               if (!wlan_static_skb[i])
                       goto err_skb_alloc;
       }

       wlan_static_skb[i] = dev_alloc_skb(DHD_SKB_4PAGE_BUFSIZE);
       if (!wlan_static_skb[i])
               goto err_skb_alloc;

       for (i = 0 ; i < PREALLOC_WLAN_SEC_NUM ; i++) {
               wifi_mem_array[i].mem_ptr =
                               kmalloc(wifi_mem_array[i].size, GFP_KERNEL);

               if (!wifi_mem_array[i].mem_ptr)
                       goto err_mem_alloc;
       }
       return 0;

 err_mem_alloc:
       pr_err("Failed to mem_alloc for WLAN\n");
       for (j = 0 ; j < i ; j++)
               kfree(wifi_mem_array[j].mem_ptr);

       i = WLAN_SKB_BUF_NUM;

 err_skb_alloc:
       pr_err("Failed to skb_alloc for WLAN\n");
       for (j = 0 ; j < i ; j++)
               dev_kfree_skb(wlan_static_skb[j]);

       return -ENOMEM;
}

static struct wifi_platform_data wifi_pdata = {
       .set_power              = NULL,
       .set_reset              = NULL,
       .set_carddetect         = NULL,
       .mem_prealloc           = wlan_mem_prealloc,
};

#endif /*CONFIG_BROADCOM_WIFI_RESERVED_MEM*/


//#endif /* 4330 */
#ifdef CONFIG_KEYBOARD_BCM
static void bcm_keypad_config_iocr(int row, int col)
{
	row = (1 << row) - 1;
	col = (1 << col) - 1;
	/* Set lower "row" & "col" number of bits to 1 to indicate configuration of keypad */
	writel((SYSCFG_IOCR1_KEY_ROW(row) | SYSCFG_IOCR1_KEY_COL(col)),
				ADDR_SYSCFG_IOCR1);
}
#ifdef CONFIG_INPUT_TOUCHSCREEN
static struct bcm_keymap newKeymap[] = {
	{BCM_KEY_ROW_0, BCM_KEY_COL_0, "Volume Up", KEY_VOLUMEUP},
	{BCM_KEY_ROW_0, BCM_KEY_COL_1, "Volume Down", KEY_VOLUMEDOWN},
	{BCM_KEY_ROW_0, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_0, "key home", KEY_HOME},
	{BCM_KEY_ROW_1, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_7, "unused", 0},
};
#endif

#if 0//def CONFIG_TOUCHSCREEN_TMA340
static struct bcm_keymap newKeymap[] = {
	{BCM_KEY_ROW_0, BCM_KEY_COL_0, "Volume Up", KEY_VOLUMEUP},
	{BCM_KEY_ROW_0, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_0, "Volume Down", KEY_VOLUMEDOWN},
	{BCM_KEY_ROW_1, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_0, "key home", KEY_HOME},
	{BCM_KEY_ROW_2, BCM_KEY_COL_1, "key back", KEY_BACK},
	{BCM_KEY_ROW_2, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_1, "key menu", KEY_MENU},
	{BCM_KEY_ROW_3, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_7, "unused", 0},
};
#endif

static struct bcm_keypad_platform_info bcm215xx_keypad_data = {
	.row_num = 4,
	.col_num = 2,
	.keymap = newKeymap,
	.iocr_cfg = bcm_keypad_config_iocr,
	.bcm_keypad_base = (void *)__iomem IO_ADDRESS(BCM21553_KEYPAD_BASE),
};

#endif


struct platform_device bcm_test_vibrator_devices = {
	.name = "vibrator", 
	.id = 0,
	.voltage = 3300000,
};
#ifdef CONFIG_BACKLIGHT_AAT1401
struct platform_device bcm_aat1401_backlight_devices = {
	.name = "aat1401-backlight", 
	.id = 0,
};
static struct platform_aat1401_backlight_data bcm_aat1401_backlight_data = {
	.max_brightness = 255,
	.dft_brightness = 143,
	.ctrl_pin = 17,
};
#endif

#ifdef CONFIG_BCM_PWM
static struct pwm_platform_data pwm_dev = {
	.max_pwm_id = 6,
	.syscfg_inf = board_sysconfig,
};
#endif

#if defined (CONFIG_MTD_ONENAND_BCM_XBOARD)
static struct resource onenand_resources[] = {
	{
	 .start = IO_ADDRESS(BCM21553_ONENAND_BASE),
	 .end = IO_ADDRESS(BCM21553_ONENAND_BASE) + SZ_128K - 1,
	 .flags = IORESOURCE_MEM,
	 },
};

static struct flash_platform_data onenand_data = {
	.parts = NULL,
	.nr_parts = 0,
};

static struct platform_device bcm21553_device_onenand = {
	.name = "onenand",
	.resource = onenand_resources,
	.num_resources = ARRAY_SIZE(onenand_resources),
	.dev = {
		.platform_data = &onenand_data,
		},
};
#endif

#if defined (CONFIG_ANDROID_PMEM)
static struct android_pmem_platform_data android_pmem_pdata = {
        .name = "pmem_adsp",
        .no_allocator = 0,
        .cached = 0,
};

static struct platform_device android_pmem_device = {
        .name = "android_pmem",
        .id = 0,
        .dev = {.platform_data = &android_pmem_pdata},
};
#endif

#if defined (CONFIG_BRCM_HAL_CAM)
struct i2c_slave_platform_data cooperve_cam_pdata = {
        .i2c_spd = I2C_SPD_400K,
};

static struct i2c_board_info __initdata bcm21553_cam_i2c_board_info[] = {
        {I2C_BOARD_INFO("cami2c", (0x5A >> 1)),
         .platform_data = (void *)&cooperve_cam_pdata,
         .irq = IRQ_CAM_CCP2, 
         },
};
#endif

#if defined (CONFIG_INPUT_BMA150_SMB380)
int bma_gpio_init(struct device *dev)
{
	gpio_request(27, "bma150");
	gpio_direction_input(27);
	set_irq_type(GPIO_TO_IRQ(27), IRQF_TRIGGER_RISING);
	return 0;
}

static struct bma150_accl_platform_data bma_pdata = {
	.orientation = BMA_ROT_90,
	.invert = true,
	.init = bma_gpio_init,
	.i2c_pdata = {.i2c_spd = I2C_SPD_100K,},
};
#endif

#ifdef CONFIG_BCM_GPIO_VIBRATOR
#define HAPTIC_ENABLE 56
#define HAPTIC_PWM 16
static struct timed_gpio timed_gpios[] = {
	{
	   .name = "vibrator",
	   .gpio = HAPTIC_ENABLE,
	   .max_timeout = 15000,
	   .active_low = 0,
	    .regl_id = "haptic_pwm",
	},
};

struct timed_gpio_platform_data timed_gpio_data = {
	.num_gpios      = ARRAY_SIZE(timed_gpios),
	.gpios          = timed_gpios,
};

struct platform_device bcm_timed_gpio = {
	.name           = "timed-gpio",
	.id             = -1,
	.dev			= {
		.platform_data = &timed_gpio_data,
	},
};

static void vibrator_init(void)
{
	/* Initialise the vibrator settings */
	board_sysconfig(SYSCFG_VIBRATOR, SYSCFG_INIT);

	gpio_request(HAPTIC_ENABLE, "haptic_enable");
	gpio_direction_output(HAPTIC_ENABLE, 0);
	bcm_gpio_pull_up(HAPTIC_ENABLE, false);
	bcm_gpio_pull_up_down_enable(HAPTIC_ENABLE, false);

	gpio_request(HAPTIC_PWM, "haptic_pwm");
	gpio_direction_output(HAPTIC_PWM, 0);
	bcm_gpio_pull_up(HAPTIC_PWM, true);
	bcm_gpio_pull_up_down_enable(HAPTIC_PWM, true);
}
#endif

#if defined(CONFIG_MFD_MAX8986)

#define SYSPARM_PMU_REG(index, dst)					\
	do {								\
		unsigned int parm;					\
		int ret = SYSPARM_GetPMURegSettings(index, &parm);	\
		if (ret == 0) {						\
			pr_info("sysparm: %s: %x\n", #index, parm);	\
			dst = parm;					\
		}							\
	} while (0)

/* Indexes of PMU registers in syparm */
enum {
	PMU_REG_0X07_PM_A1OPMODCTRL = 0x07,
	PMU_REG_0X08_PM_D1OPMODCTRL = 0x08,
	PMU_REG_0X09_PM_A8OPMODCTRL = 0x09,
	PMU_REG_0X0A_PM_A2OPMODCTRL = 0x0A,
	PMU_REG_0X0B_PM_H1OPMODCTRL = 0x0B,
	PMU_REG_0X0C_PM_H2OPMODCTRL = 0x0C,
	PMU_REG_0X0D_PM_D2OPMODCTRL = 0x0D,
	PMU_REG_0X0E_PM_A5OPMODCTRL = 0x0E,
	PMU_REG_0X0F_PM_A4OPMODCTRL = 0x0F,
	PMU_REG_0X10_PM_LVOPMODCTRL = 0x10,
	PMU_REG_0X11_PM_SIMOPMODCTRL = 0x11,
	PMU_REG_0X15_PM_CSROPMODCTRL = 0x15,
	PMU_REG_0X17_PM_IOSROPMODCTRL = 0x17,
	PMU_REG_0x1B_PM_MBCCTRL4 = 0x1B,
	PMU_REG_0X2B_PM_A6OPMODCTRL = 0x2B,
	PMU_REG_0X2C_PM_A3OPMODCTRL = 0x2C,
	PMU_REG_0X2D_PM_AX1OPMODCTRL = 0x2D,
	PMU_REG_0X2E_PM_A7OPMODCTRL = 0x2E,
	PMU_REG_0x3F_PM_ADISCHARG2 = 0x3F,
	PMU_REG_0X41_PM_D3OPMODCTRL = 0x41,
	PMU_REG_0x43_PM_D4OPMODCTRL = 0x43,
	PMU_REG_0X45_PM_A9OPMODCTRL = 0x45,
	PMU_REG_0X47_PM_TSROPMODCTRL = 0x47,
};

static struct regulator_consumer_supply dldo3_consumers[] = {
	{
		.dev = NULL,
		.supply = "cam_vddi",
	},
	{
		.dev = NULL,
		.supply = "haptic_pwm",
	},
};

static struct regulator_init_data dldo3_init_data = {
	.constraints = {
		.min_uV = 1800000,
		.max_uV = 3000000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(dldo3_consumers),
	.consumer_supplies = dldo3_consumers,
};

		static struct regulator_consumer_supply dldo4_consumers[] = {
	{
		.dev = NULL,
		.supply = "cam_vddc",
	},
};

static struct regulator_init_data dldo4_init_data = {
	.constraints = {
		.min_uV = 1800000,
		.max_uV = 1800000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.boot_on = 0,
		.apply_uV=1,
	},
	.num_consumer_supplies = ARRAY_SIZE(dldo4_consumers),
	.consumer_supplies = dldo4_consumers,
};

static struct regulator_consumer_supply hcldo1_consumers[] = {
	{
		.dev = NULL,
		.supply = "touch_keyled",
	},
};

static struct regulator_init_data hcldo1_init_data = {
	.constraints = {
		.min_uV = 2500000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(hcldo1_consumers),
	.consumer_supplies = hcldo1_consumers,
};

static struct regulator_consumer_supply hcldo2_consumers[] = {
	{
		.dev = NULL,
		.supply = "cam_vdda",
	},
};

static struct regulator_init_data hcldo2_init_data = {
	.constraints = {
		.min_uV = 2500000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(hcldo2_consumers),
	.consumer_supplies = hcldo2_consumers,
};

static struct regulator_consumer_supply sim_consumers[] = {
	{
		.dev	= NULL,
		.supply	= "sim_vcc",
	},
};

static struct regulator_init_data sim_init_data = {
	.constraints = {
		.min_uV = 1800000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(sim_consumers),
	.consumer_supplies = sim_consumers,
};

static struct regulator_consumer_supply vib_consumers[] = {
	{
		.dev	= NULL,
		.supply	= "vib_vcc",
	},
};

static struct regulator_init_data vib_init_data = {
	.constraints = {
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(vib_consumers),
	.consumer_supplies = vib_consumers,
};
static struct regulator_consumer_supply touch_consumers[] = {
	{
		.dev	= NULL,
		.supply	= "touch_vcc",
	},
};

static struct regulator_init_data touch_init_data = {
	.constraints = {
		.min_uV = 1200000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		.always_on = 0,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(touch_consumers),
	.consumer_supplies = touch_consumers,
};

static struct regulator_consumer_supply csr_nm1_consumers[] = {
	{
		.dev	= NULL,
		.supply	= "csr_nm1",
	},
};

static struct regulator_init_data csr_nm1_init_data = {
	.constraints = {
		.min_uV = 900000,
		.max_uV = 1380000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.boot_on = 1,
	},
	.num_consumer_supplies = ARRAY_SIZE(csr_nm1_consumers),
	.consumer_supplies = csr_nm1_consumers,
};

static struct regulator_consumer_supply csr_nm2_consumers[] = {
	{
		.dev	= NULL,
		.supply	= "csr_nm2",
	},
};

static struct regulator_init_data csr_nm2_init_data = {
	.constraints = {
		.min_uV = 900000,
		.max_uV = 1380000,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.boot_on = 1,
	},
	.num_consumer_supplies = ARRAY_SIZE(csr_nm2_consumers),
	.consumer_supplies = csr_nm2_consumers,
};

/* GPS */
static struct regulator_consumer_supply aldo1_consumers[] = {
	{
		.dev	= NULL,
		.supply	= "aldo1_3v0",
	},
};

static struct regulator_init_data aldo1_init_data = {
	.constraints = {
		.valid_ops_mask = REGULATOR_CHANGE_MODE,
		.always_on = 0,
		.boot_on = 1,
	},
	.num_consumer_supplies = ARRAY_SIZE(aldo1_consumers),
	.consumer_supplies = aldo1_consumers,
};

static struct max8986_regl_init_data bcm21553_regulators[] = {
	{
		.regl_id = MAX8986_REGL_SIMLDO,
		.dsm_opmode = MAX8986_REGL_LPM_IN_DSM,
		.init_data = &sim_init_data,
	}, {
		.regl_id = MAX8986_REGL_AUXLDO1,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &vib_init_data,
	}, {
		.regl_id = MAX8986_REGL_ALDO9,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &touch_init_data,
	}, {
		.regl_id = MAX8986_REGL_DLDO3,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &dldo3_init_data,
	},
/*	{
		.regl_id = MAX8986_REGL_ALDO8,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &aldo8_init_data,
	},
*/	{
		.regl_id = MAX8986_REGL_CSR_NM1,
		.dsm_opmode = MAX8986_REGL_LPM_IN_DSM,
		.init_data = &csr_nm1_init_data,
	},
	{
		.regl_id = MAX8986_REGL_CSR_NM2,
		.dsm_opmode = MAX8986_REGL_LPM_IN_DSM,
		.init_data = &csr_nm2_init_data,
	},
	
	{
		.regl_id = MAX8986_REGL_DLDO4,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &dldo4_init_data,
	},
	{
		.regl_id = MAX8986_REGL_HCLDO1,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &hcldo1_init_data,
	},
	{
		.regl_id = MAX8986_REGL_HCLDO2,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &hcldo2_init_data,
	},
	{
		.regl_id = MAX8986_REGL_ALDO1,
		.dsm_opmode = MAX8986_REGL_OFF_IN_DSM,
		.init_data = &aldo1_init_data,
	},

};

static struct max8986_regl_pdata regl_pdata = {
	.num_regulators = ARRAY_SIZE(bcm21553_regulators),
	.regl_init =  bcm21553_regulators,
	.regl_default_pmmode = {
		[MAX8986_REGL_ALDO1]	= 0x22,
		[MAX8986_REGL_ALDO2]	= 0x11,
		[MAX8986_REGL_ALDO3]	= 0x11,
		[MAX8986_REGL_ALDO4]	= 0x11,
		[MAX8986_REGL_ALDO5]	= 0x01,
		[MAX8986_REGL_ALDO6]	= 0x11,
		[MAX8986_REGL_ALDO7]	= 0x11,
		//[MAX8986_REGL_ALDO8]	= 0x22,
		[MAX8986_REGL_ALDO8]	= 0x00,
		[MAX8986_REGL_ALDO9]	= 0x22,
		[MAX8986_REGL_DLDO1]	= 0x11,
		[MAX8986_REGL_DLDO2]	= 0x55,
		[MAX8986_REGL_DLDO3]	= 0xAA,
		[MAX8986_REGL_DLDO4]	= 0xAA,
		[MAX8986_REGL_HCLDO1]	= 0x22,
		[MAX8986_REGL_HCLDO2]	= 0xAA,
		[MAX8986_REGL_LVLDO]	= 0x11,
		[MAX8986_REGL_SIMLDO]	= 0x11,
		[MAX8986_REGL_AUXLDO1]	= 0xAA,
		[MAX8986_REGL_TSRLDO]	= 0x01,
		[MAX8986_REGL_CSR_LPM]	= 0xCD,
		[MAX8986_REGL_IOSR]	= 0x01,
	},
};

/*Default settings for audio amp */

static struct max8986_audio_pdata audio_pdata = {
                .ina_def_mode = MAX8986_INPUT_MODE_DIFFERENTIAL,    /*Set INA in differential mode */
                .inb_def_mode = MAX8986_INPUT_MODE_SINGLE_ENDED, /* Set INB in single ended mode */
                .ina_def_preampgain = PREAMP_GAIN_NEG_6DB,   /*INA pre-amp gain - 6dB*/
                .inb_def_preampgain = PREAMP_GAIN_NEG_6DB, /*INB pre-amp gain - 6dB*/

                .lhs_def_mixer_in = MAX8986_MIXER_INB1,   /*Left Headset -  INB1*/
                .rhs_def_mixer_in = MAX8986_MIXER_INB2, /*Right Headset -  INB2*/
                .ihf_def_mixer_in = MAX8986_MIXER_INA2, /*handsfree- INA*/

				.hs_input_path = MAX8986_INPUTB,
				.ihf_input_path = MAX8986_INPUTA,
};

static struct max8986_power_pdata power_pdata = {
	.usb_charging_cc = MAX8986_CHARGING_CURR_550MA,
	.wac_charging_cc = MAX8986_CHARGING_CURR_550MA,
	.eoc_current = MAX8986_EOC_100MA,

	.temp_adc_channel =  0,
	.temp_low_limit = 1,
	.temp_high_limit = 1000,

	.batt_technology = POWER_SUPPLY_TECHNOLOGY_LION,
};

struct max8986_muic_pdata muic_data = {
	.mic_insert_adc_val = PMU_MUIC_ADC_OUTPUT_1M,
	.mic_btn_press_adc_val = PMU_MUIC_ADC_OUTPUT_2K,
};


static void __init update_power_sysparm(void)
{
#if defined(CONFIG_BRCM_FUSE_SYSPARM)
//	SYSPARM_PMU_REG(PMU_REG_0x1B_PM_MBCCTRL4, power_pdata.usb_charging_cc);
	SYSPARM_PMU_REG(PMU_REG_0x1B_PM_MBCCTRL4, power_pdata.wac_charging_cc);
#endif /* CONFIG_BRCM_FUSE_SYSPARM */
}

/*****************************************************************************
 * platform specific usb interface routines for the power driver
 *****************************************************************************/
/* indicates whether the usb stack is ready or not */
static bool usb_driver_initialized;
/* indicates whether usb charger is detected or not */
static bool usb_charger_detected;
extern void Android_usb_cable_connection(bool is_connected);
extern void dwc_otg_pcd_StartUsb(int is_on);
extern void Android_PMU_USB_Start(void);
extern void Android_PMU_USB_Stop(void);


/* if both the power driver and the usb driver are ready, then start the
 * enumeration seqence.
 */
static void pmu_usb_start(void)
{
	pr_info("%s\n", __func__);

#ifdef CONFIG_USB_ANDROID
	if (usb_driver_initialized == true && usb_charger_detected == true)
	{
		Android_usb_cable_connection(1);
		Android_PMU_USB_Start();
		pr_info("%s: usb stack is UP\n", __func__);
	} else
	{
		Android_usb_cable_connection(0);
		pr_info("%s: usb stack is not up\n", __func__);
	}
#endif
}

static void pmu_usb_stop(void)
{
	pr_info("%s\n", __func__);
#ifdef CONFIG_USB_DWC_OTG
	Android_usb_cable_connection(0);
	Android_PMU_USB_Stop();
#endif
}

/*****************************************************************************
 * USB driver call backs
 *****************************************************************************/

/* this function is called from the usb driver once the USB reset event is
 * detected.
 */
void pmu_usb_enum_started(void)
{
	pr_info("%s\n", __func__);
}
EXPORT_SYMBOL(pmu_usb_enum_started);

/* this function is called by the usb driver once the usb stack init is
 * completed.
 */
void pmu_usb_driver_initialized(void)
{
	pr_info("%s\n", __func__);

	/* start usb enumeration if this function is called for the first
	 * time
	 */
	if (usb_driver_initialized == false) {
		usb_driver_initialized = true;
		pmu_usb_start();
	}
}
EXPORT_SYMBOL(pmu_usb_driver_initialized);

#define PMU_IRQ_GPIO    (34)
#define NB_TEMP_TABLE 18
#define NB_BATT_TABLE 8

static void Bcm_PMU_GpioInit_Setting()
{
	pr_info("%s: PMU: init hardware\n", __func__);

	if (gpio_request(PMU_IRQ_GPIO, "pmu_irq") == 0) {
		gpio_direction_input(PMU_IRQ_GPIO);
		bcm_gpio_pull_up_down_enable(PMU_IRQ_GPIO, true);
		bcm_gpio_pull_up(PMU_IRQ_GPIO, true);

		set_irq_type(GPIO_TO_IRQ(PMU_IRQ_GPIO), IRQF_TRIGGER_FALLING);
	} else {
		pr_err("%s: failed to allocate GPIO for PMU IRQ\n",__func__);
	}
}

static void Bcm_HEADSET_GpioInit_Setting()
{
	pr_info("%s: HEADSET: init hardware\n", __func__);
	
	//++Change ANA_SYSCLKEN to GPIO42 
	writel(readl(ADDR_SYSCFG_IOCR2) |(SYSCFG_IOCR2_ANA_SYSCLKEN_MUX_MASK),ADDR_SYSCFG_IOCR2);
	gpio_direction_input(GPIO_HEADSET_BUTTON);
	bcm_gpio_pull_up(GPIO_HEADSET_BUTTON, true);
	bcm_gpio_pull_up_down_enable(GPIO_HEADSET_BUTTON, false);
	//--Change ANA_SYSCLKEN to GPIO42 
}



static int calculate_batt_level(int batt_volt)
{
	int scaled_level = 0;

	if(batt_volt >= BATT_FULL_VOLT) {		//100%
		scaled_level = 99;
	}
	else if(batt_volt >=  BATT_LEVEL5_VOLT) {	//99% ~ 80%
		scaled_level = ((batt_volt -BATT_LEVEL5_VOLT+1)*20)/(BATT_FULL_VOLT-BATT_LEVEL5_VOLT);
 		scaled_level = scaled_level+80;
	}
	else if(batt_volt >= BATT_LEVEL4_VOLT) { 	//79% ~ 65%
		scaled_level = ((batt_volt -BATT_LEVEL4_VOLT)*15)/(BATT_LEVEL5_VOLT-BATT_LEVEL4_VOLT);
 		scaled_level = scaled_level+65;
	}
	else if(batt_volt >= BATT_LEVEL3_VOLT) { 	//64% ~ 50%
		scaled_level = ((batt_volt -BATT_LEVEL3_VOLT)*15)/(BATT_LEVEL4_VOLT-BATT_LEVEL3_VOLT);
 		scaled_level = scaled_level+50;
	}
	else if(batt_volt >= BATT_LEVEL2_VOLT) {	//49% ~ 35%
		scaled_level = ((batt_volt -BATT_LEVEL2_VOLT)*15)/(BATT_LEVEL3_VOLT-BATT_LEVEL2_VOLT);
		scaled_level = scaled_level+35;
	}
	else if(batt_volt >= BATT_LEVEL1_VOLT) {	//34% ~ 20%
		scaled_level = ((batt_volt -BATT_LEVEL1_VOLT)*15)/(BATT_LEVEL2_VOLT-BATT_LEVEL1_VOLT);
 		scaled_level = scaled_level+20;
	}
	else if(batt_volt >= BATT_LEVEL0_VOLT) {	//19% ~ 5%
		scaled_level = ((batt_volt -BATT_LEVEL0_VOLT)*15)/(BATT_LEVEL1_VOLT-BATT_LEVEL0_VOLT);
 		scaled_level = scaled_level+5;
	}
	else if(batt_volt >= BATT_LEVEL0_1_VOLT) {	// 4% ~ 3%
		scaled_level = ((batt_volt -BATT_LEVEL0_1_VOLT)*2)/(BATT_LEVEL0_VOLT-BATT_LEVEL0_1_VOLT);
 		scaled_level = scaled_level+3;
	}
	else if(batt_volt > BATT_LOW_VOLT) {		// 2% ~ 1%
				scaled_level = ((batt_volt -BATT_LOW_VOLT)*2)/(BATT_LEVEL0_1_VOLT-BATT_LOW_VOLT);
 		scaled_level = scaled_level+1;
	}
	else {
		scaled_level = 1;
	}


	pr_info("%s: scaled_level=%d prev_scaled_level = %d\n", __func__,scaled_level,prev_scaled_level);

	if(  prev_scaled_level!=0 && prev_scaled_level!=1)
	{
		if( scaled_level > prev_scaled_level)
		{
			if(pmu_is_charger_inserted()==true)
			{
				if( scaled_level <= prev_scaled_level+2)
					prev_scaled_level = scaled_level;
				else
				{
					scaled_level = prev_scaled_level+1;
					prev_scaled_level = scaled_level;
				}
	}
			else
				scaled_level = prev_scaled_level;
		}	
		else
		{
			if( scaled_level >= prev_scaled_level-2 )
				prev_scaled_level = scaled_level;
			else
			{
				scaled_level = prev_scaled_level-1;
				prev_scaled_level = scaled_level;
			}
		}
	}
	else
	{
		prev_scaled_level = scaled_level;
	}
	//pr_info("%s: batt level = %d\n", __func__,scaled_level);
	return scaled_level;
}
static u32 pmu_event_callback(int event, int param)
{
	int i,inx;
	u32 ratio=0,ret = 0;
	u32 gain = 0, offset = 0;
	
	/*
	bat_adc_volt_mul calculated as follows:
	V(mV) = ADC *((R1+R2)/ R2)*( ADC input range /10-bit ADC)
	V(mV) = ADC * ((750K+243K)/ 243K)*(1200/1023)
	V(mV) = ADC * (4.08642)*(1.173021)
	V(mV) = ADC * (4.793454)
	*/
	const u32 bat_adc_volt_mul = 4793;
//	static u16 bat_vol[] = {3400, 3500, 3600, 3720, 3800, 3950, 4200};
	/* These values are observed on ThunderbirdEDN31 */
//	static u16 bat_adc[] = {0x2BE, 0x2D3, 0x2E8, 0x2FC, 0x311, 0x32C, 0x364};

	static u32 TempAdcTable[] = {     786,732,674,612,550,488,428,373,322,276,236,201,171,145,123,104,97,91,}; 
	static u32 TempDegreeTable[] ={-10,-5,     0,    5,  10,  15,  20, 25,  30,  35,  40, 45,  50,  55,  60, 63,65,67,};

	static u16 Default4p2Volt;
	static u16 BattEmptyThresh;

	static u16 bat_vol[] = {3400, 3500, 3600, 3720, 3800, 3950, 4200, 4400};
	static u16 bat_adc[NB_BATT_TABLE] = {};
 
	switch (event) {

	case PMU_EVENT_INIT_PLATFORM:
#if 0
	pr_info("%s: PMU: init hardware\n", __func__);

	if (gpio_request(PMU_IRQ_GPIO, "pmu_irq") == 0) {
		gpio_direction_input(PMU_IRQ_GPIO);
		bcm_gpio_pull_up_down_enable(PMU_IRQ_GPIO, true);
		bcm_gpio_pull_up(PMU_IRQ_GPIO, true);

		set_irq_type(GPIO_TO_IRQ(PMU_IRQ_GPIO), IRQF_TRIGGER_FALLING);
	} else {
		pr_err("%s: failed to allocate GPIO for PMU IRQ\n",
			__func__);
	}
#endif	

	if(SYSPARM_GetDefault4p2VoltReading())	
//		Default4p2Volt = SYSPARM_GetDefault4p2VoltReading();
		Default4p2Volt = 0x364;
	else
		Default4p2Volt = 0x364;

	if(SYSPARM_GetBattEmptyThresh())
//		BattEmptyThresh = SYSPARM_GetBattEmptyThresh();
		BattEmptyThresh = 0x2BF;
	else
		BattEmptyThresh = 0x2BF;
	
	printk(KERN_INFO"Default4p2Volt 0x%x\n", Default4p2Volt);
	printk(KERN_INFO"BattEmptyThresh3P4V 0x%x\n", BattEmptyThresh);

	bat_adc[0] = BattEmptyThresh;
	bat_adc[6] = Default4p2Volt;
	gain = ((bat_adc[6]-bat_adc[0])*1000)/(bat_vol[6]-bat_vol[0]);
	offset = (bat_adc[0]*1000) -(gain * bat_vol[0]);

	for (inx = 1; inx < (NB_BATT_TABLE); inx++)
		bat_adc[inx] = (bat_vol[inx] * gain + offset)/1000; 

	/* init batt params */
	power_pdata.batt_adc_tbl.num_entries = ARRAY_SIZE(bat_adc);
	power_pdata.batt_adc_tbl.bat_adc = bat_adc;
	power_pdata.batt_adc_tbl.bat_vol = bat_vol;
	break;

	case PMU_EVENT_BATT_ADC_TO_VOLTAGE:
#if 0
		ret = param*bat_adc_volt_mul;
#else
		ret = calculate_batt_level(param);
#endif
		break;

	case PMU_EVENT_CHARGER_INSERT:
	pr_info("%s: PMU_EVENT_CHARGER_INSERT\n", __func__);

	/* try to start the usb enumeration process. this may not succeed if
	 * the usb stack is still not up.
	 */
	if (param == PMU_MUIC_CHGTYP_USB) {
		usb_charger_detected = true;
		pmu_usb_start();
	}
	break;

	case PMU_EVENT_CHARGER_REMOVE:
	pr_info("%s: PMU_EVENT_CHARGER_REMOVE\n", __func__);
	if (param == PMU_MUIC_CHGTYP_USB) {
		usb_charger_detected = false;
		pmu_usb_stop();
	}
	break;


	case PMU_EVENT_BATT_TEMP_BEYOND_LIMIT:
		pr_info("--------------%s: PMU_EVENT_BATT_TEMP_BEYOND_LIMIT--------------------\n", __func__);
		break;

	case PMU_EVENT_BATT_TEMP_TO_DEGREE_C:

		ret = TempDegreeTable[NB_TEMP_TABLE - 1];

		if (param > TempAdcTable[0]) {
			ret = TempDegreeTable[0];
		} else if (param >= TempAdcTable[NB_TEMP_TABLE - 1]) {
			for (inx = 1; inx < NB_TEMP_TABLE; inx++) {
				if (param >= TempAdcTable[inx]) 
				{
					/* using liner interpolation (lerp) */
					ratio = ((TempAdcTable[inx-1]-param) * 1000)/(TempAdcTable[inx-1]-TempAdcTable[inx]);
					ret = TempDegreeTable[inx-1] + ((TempDegreeTable[inx] - TempDegreeTable[inx-1]) * ratio)/1000;
					break;
				}
			}
		}
		else
			ret = TempDegreeTable[NB_TEMP_TABLE-1]+3;
		ret = ret *10;	
		break;
	default:
	pr_err("%s: unrecognized event: %d\n", __func__, event);
	break;

	}
	return ret;
}

#if defined(CONFIG_BRCM_FUSE_SYSPARM)
static void max8986_load_sysparm(int sysparm_index, int regl_addr,
				 struct max8986 *max8986)
{
	unsigned int parm;
	int ret;

	ret = SYSPARM_GetPMURegSettings(sysparm_index, &parm);
	if (ret == 0)
		max8986->write_dev(max8986, MAX8986_PM_REG_ADISCHARGE2, parm);
}

static void max8986_sysparms(struct max8986 *max8986)
{
	max8986_load_sysparm(PMU_REG_0x3F_PM_ADISCHARG2,
			     MAX8986_PM_REG_ADISCHARGE2, max8986);
}
#endif /* CONFIG_BRCM_FUSE_SYSPARM */

struct max8986_platform_data max8986_pdata = {
	.i2c_pdata = {.i2c_spd = I2C_SPD_400K,},
	.flags = MAX8986_USE_REGULATORS | MAX8986_REGISTER_POWER_OFF |
		MAX8986_ENABLE_AUDIO | MAX8986_USE_PONKEY | MAX8986_ENABLE_MUIC |
		MAX8986_USE_RTC | MAX8986_USE_POWER | MAX8986_ENABLE_DVS,
	.pmu_event_cb = pmu_event_callback,
	.regulators = &regl_pdata,
	.audio_pdata = &audio_pdata,
	.power = &power_pdata,
	.muic_pdata = &muic_data,
	/* CSR Voltage data */
	.csr_nm_volt = CSR_VOUT_1_36V,
	.csr_nm2_volt = -1, /*AVS driver will set this*/
	.csr_lpm_volt = CSR_VOUT_0_90V,
#if defined(CONFIG_BRCM_FUSE_SYSPARM)
	.max8986_sysparms = max8986_sysparms,
#endif
};
#endif /*CONFIG_MFD_MAX8986*/

#ifdef CONFIG_BCM_AUXADC
static struct bcm_plat_auxadc adcplat = {
	.readmask = 0x3FF,
	.croff = 0,
	.rdoff = 8,
	.ready = (0x1 << 15),
	.start = (0x1 << 3),
	.auxpm = 0,
	.regoff = (0x1 << 23),
	.bgoff = (0x1 << 22),
};
#endif

#ifdef CONFIG_TOUCHSCREEN_QT602240
#define PEN_IRQ_GPIO   30
static int qt602240_init_platform_hw(void)
{
	gpio_request(PEN_IRQ_GPIO, "qt602240");
	gpio_direction_input(PEN_IRQ_GPIO);
	bcm_gpio_pull_up(PEN_IRQ_GPIO, true);
	bcm_gpio_pull_up_down_enable(PEN_IRQ_GPIO, true);
	set_irq_type(GPIO_TO_IRQ(PEN_IRQ_GPIO), IRQF_TRIGGER_FALLING);
	return 0;
}

static struct qt602240_platform_data qt602240_platform_data = {
	.i2c_pdata = {.i2c_spd = I2C_SPD_400K,},
	.platform_init  = qt602240_init_platform_hw,
	.x_line         = 15,
	.y_line         = 11,
	.x_size         = 1023,
	.y_size         = 1023,
	.x_min		= 90,
	.y_min		= 90,
	.x_max		= 0x3ff,
	.y_max		= 0x3ff,
	.max_area	= 0xff,
	.blen           = 33,
	.threshold      = 70,
	.voltage        = 2700000,              /* 2.8V */
	.orient         = QT602240_DIAGONAL_COUNTER,
};

#endif

#if defined(CONFIG_INPUT_YAS_ACCELEROMETER)
#define ACC_SDA 15
#define ACC_SCL 7

#if defined(CONFIG_I2C_GPIO)
static struct i2c_gpio_platform_data bma222_i2c_gpio_data = {
        .sda_pin    = ACC_SDA,
        .scl_pin    = ACC_SCL,
        .udelay  = 3,  //// brian :3
        .timeout = 100,
};

static struct platform_device bma222_i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = 0x4,
        .dev        = {
                .platform_data  = &bma222_i2c_gpio_data,
        },
};
#endif
#endif

#if defined(CONFIG_SENSORS_GP2A)
#define PROXI_SDA 28
#define PROXI_SCL 31

#if defined(CONFIG_I2C_GPIO)
static struct i2c_gpio_platform_data gp2a_i2c_gpio_data = {
        .sda_pin    = PROXI_SDA,
        .scl_pin    = PROXI_SCL,
        .udelay  = 3,  //// brian :3
        .timeout = 100,
};

static struct platform_device gp2a_i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = 0x5,
        .dev        = {
                .platform_data  = &gp2a_i2c_gpio_data,
        },
};
#endif
#endif

#if defined(CONFIG_SENSORS_TAOS)
#define PROXI_SDA 28
#define PROXI_SCL 31

#if defined(CONFIG_I2C_GPIO)
static struct i2c_gpio_platform_data taos_i2c_gpio_data = {
        .sda_pin    = PROXI_SDA,
        .scl_pin    = PROXI_SCL,
        .udelay  = 3,  //// brian :3
        .timeout = 100,
};

static struct platform_device taos_i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = 0x5,
        .dev        = {
                .platform_data  = &taos_i2c_gpio_data,
        },
};
#endif
#endif

#if defined(CONFIG_INPUT_YAS_MAGNETOMETER)
#define GEO_SDA 14
#define GEO_SCL 5

#if defined(CONFIG_I2C_GPIO)
static struct i2c_gpio_platform_data i2c_gpio_data = {
        .sda_pin    = GEO_SDA,
        .scl_pin    = GEO_SCL,
        .udelay  = 3,  //// brian :3
        .timeout = 100,
};

static struct platform_device i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = 0x6,
        .dev        = {
                .platform_data  = &i2c_gpio_data,
        },
};
#endif

#endif
static void sensors_init(void)
{
	board_sysconfig(SYSCFG_SENSORS, SYSCFG_ENABLE);
}

module_init(sensors_init);

#define TS_GPIO_IRQ	30
#define TSP_INT 30
#define TSP_SDA 27
#define TSP_SCL 26

#if 0
static int cyttsp_init(void)
{
	/* add any special code to initialize any required system hw
	 * such as regulators or gpio pins
	 */
	gpio_request(TSP_INT, "ts_irq");
	gpio_direction_input(TSP_INT);
	bcm_gpio_pull_up(TSP_INT, true);
	bcm_gpio_pull_up_down_enable(TSP_INT, true);
	set_irq_type(GPIO_TO_IRQ(TSP_INT), IRQF_TRIGGER_FALLING);
	return 0;
}

static void cyttsp_exit(void)
{
	gpio_free(TSP_INT);
}

static int cyttsp_wakeup(void)
 {
 	return 0;
 }
 
static struct cyttsp_platform_data cypress_ttsp_platform_data = {
	.maxx = 239,
	.maxy = 319,
	.use_hndshk = 1,
	.use_sleep = 1,
	.act_dist = CY_ACT_DIST_DFLT,	/* Active distance */
	/* change act_intrvl to customize the Active power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.act_intrvl = CY_ACT_INTRVL_DFLT,
	/* change tch_tmout to customize the touch timeout for the
	 * Active power state for Operating mode
	 */
	.tch_tmout = CY_TCH_TMOUT_DFLT,
	/* change lp_intrvl to customize the Low Power power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.lp_intrvl = CY_LP_INTRVL_DFLT,
	.wakeup = cyttsp_wakeup,
	.init = cyttsp_init,
	.exit = cyttsp_exit,
	.name = CY_I2C_NAME,
	.irq_gpio = TSP_INT,
	.bl_keys = 0,
 };
 


#endif

#if defined(CONFIG_I2C_GPIO)
static struct i2c_gpio_platform_data touch_i2c_gpio_data = {
        .sda_pin    = TSP_SDA,
        .scl_pin    = TSP_SCL,
        .udelay  = 3,  //// brian :3
        .timeout = 100,
};

static struct platform_device touch_i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = 0x3,
        .dev        = {
                .platform_data  = &touch_i2c_gpio_data,
        },
};

#endif

#if defined(CONFIG_BRCM_HEADSET)  || defined(CONFIG_BRCM_HEADSET_MODULE)
#define HEADSET_DET_GPIO        33    /* HEADSET DET */
void check_hs_state (int *headset_state)
{
    *headset_state = (gpio_get_value(HEADSET_DET_GPIO)) ? 0 : 1;
}
static struct brcm_headset_pd headset_pd = {
	.hsirq		= IRQ_MICIN,
	.hsbirq		= IRQ_MICON,
	.check_hs_state = check_hs_state,
	.hsgpio= HEADSET_DET_GPIO,	
	.debounce_ms    = 60,
	.key_press_threshold = KEY_PRESS_THRESHOLD,
	.key_3pole_threshold = KEY_3POLE_THRESHOLD,
	.key1_threshold_l = KEY1_THRESHOLD_L,
	.key1_threshold_u = KEY1_THRESHOLD_U,
	.key2_threshold_l = KEY2_THRESHOLD_L,
	.key2_threshold_u = KEY2_THRESHOLD_U,
	.key3_threshold_l = KEY3_THRESHOLD_L,
	.key3_threshold_u = KEY3_THRESHOLD_U
};
#endif

#if defined(CONFIG_I2C_BOARDINFO)
struct i2c_host_platform_data i2c1_host_pdata = {
	.retries = 3,
	.timeout = 20 * HZ,
};

static struct i2c_board_info __initdata athenaray_i2c1_board_info[] = {
#if defined(CONFIG_MFD_MAX8986)
{
				I2C_BOARD_INFO("max8986", 0x08),
				.platform_data = (void *)&max8986_pdata,
				.irq = GPIO_TO_IRQ(34),
},
#endif /*CONFIG_MFD_MAX8986*/
};

static struct i2c_board_info __initdata athenaray_i2c2_board_info[] = {
#if defined(CONFIG_INPUT_BMA150_SMB380)
	{
		I2C_BOARD_INFO("bma150_accl", 0x38),
	 .platform_data = (void *)&bma_pdata,
	 .irq = GPIO_TO_IRQ(27),
	 },
#endif
};

static struct i2c_board_info __initdata athenaray_i2c3_board_info[] = {
#ifdef CONFIG_TOUCHSCREEN_QT602240
	{
				I2C_BOARD_INFO("qt602240_ts", 0x4b),
				.platform_data = &qt602240_platform_data,
				.irq = GPIO_TO_IRQ(PEN_IRQ_GPIO),
	 },
#endif
};

static struct i2c_board_info __initdata athenaray_i2cgpio0_board_info[] = {
#if 0
	{
				I2C_BOARD_INFO("synaptics-rmi-ts", 0x48),
				.irq = GPIO_TO_IRQ(TSP_INT),
	},
#endif
#if defined(CONFIG_TOUCHSCREEN_MMS128_TASSCOOPER)
	{
				I2C_BOARD_INFO("melfas-mms128", 0x48),
				.irq = GPIO_TO_IRQ(TSP_INT),
	},
#endif
#ifdef CONFIG_TOUCHSCREEN_TMA340_COOPERVE
	{
				I2C_BOARD_INFO("synaptics-rmi-ts", 0x20),
				.irq = GPIO_TO_IRQ(TSP_INT),
	},
#endif
};

static struct i2c_board_info __initdata athenaray_i2cgpio1_board_info[] = {
#if defined  (CONFIG_INPUT_YAS_ACCELEROMETER)
	{
		I2C_BOARD_INFO("accelerometer", 0x08),
	},
#endif
 };

static struct i2c_board_info __initdata athenaray_i2cgpio2_board_info[] = {
#if defined  (CONFIG_SENSORS_TAOS)
	{
		I2C_BOARD_INFO("taos", 0x39),
	},
#endif
 };

static struct i2c_board_info __initdata athenaray_i2cgpio3_board_info[] = {
#if defined  (CONFIG_INPUT_YAS_MAGNETOMETER)
	{
		I2C_BOARD_INFO("geomagnetic", 0x2e),
	},
 #endif
};

#endif

static void athenaray_add_i2c_slaves(void)
{
	i2c_register_board_info(I2C_BSC_ID0, athenaray_i2c1_board_info,
				ARRAY_SIZE(athenaray_i2c1_board_info));
/*	i2c_register_board_info(I2C_BSC_ID1, athenaray_i2c2_board_info,
				ARRAY_SIZE(athenaray_i2c2_board_info));
	i2c_register_board_info(I2C_BSC_ID2, athenaray_i2c3_board_info,
				ARRAY_SIZE(athenaray_i2c3_board_info));
*/
#if defined (CONFIG_BRCM_HAL_CAM)
        i2c_register_board_info(I2C_BSC_ID1, bcm21553_cam_i2c_board_info,
                                ARRAY_SIZE(bcm21553_cam_i2c_board_info));
#endif

	i2c_register_board_info(0x3, athenaray_i2cgpio0_board_info,
				ARRAY_SIZE(athenaray_i2cgpio0_board_info));

#if defined (CONFIG_INPUT_YAS_ACCELEROMETER)
	i2c_register_board_info(0x4, athenaray_i2cgpio1_board_info,
				ARRAY_SIZE(athenaray_i2cgpio1_board_info));
#endif
#if defined (CONFIG_SENSORS_TAOS)
	i2c_register_board_info(0x5, athenaray_i2cgpio2_board_info,
				ARRAY_SIZE(athenaray_i2cgpio2_board_info));
#endif
#if defined (CONFIG_INPUT_YAS_MAGNETOMETER)
	i2c_register_board_info(0x6, athenaray_i2cgpio3_board_info,
				ARRAY_SIZE(athenaray_i2cgpio3_board_info));
#endif
}

#ifdef CONFIG_SPI
static DEFINE_SPINLOCK(bcm_spi_lock);
static int bcm21xx_cs_control(struct driver_data *drv_data, u8 cs)
{
	unsigned long flags;

	if (!drv_data)
		return -EINVAL;

	spin_lock_irqsave(&bcm_spi_lock, flags);

	writeb(SPI_SPIFSSCR_FSSNEW(cs) |
	       SPI_SPIFSSCR_FSSSELNEW, drv_data->ioaddr + SPI_SPIFSSCR);

	spin_unlock_irqrestore(&bcm_spi_lock, flags);

	return 0;
}

static struct bcm21xx_spi_platform_data bcm21xx_spi_info = {
	.slot_id = 0,
	.enable_dma = 1,
	.cs_line = 1,
	.mode = 0,		/* Configure for master mode */
	.syscfg_inf = board_sysconfig,
	.cs_control = bcm21xx_cs_control,
};

/*
 * SPI board info for the slaves
 */
static struct spi_board_info spi_slave_board_info[] __initdata = {
	{
	 .modalias = "spidev",	/* use spidev generic driver */
	 .max_speed_hz = 60000000,	/* use max speed */
	 .bus_num = 0,		/* framework bus number */
	 .chip_select = 0,	/* for each slave */
	 .platform_data = NULL,	/* no spi_driver specific */
	 .controller_data = &bcm21xx_spi_info,
	 .irq = 0,		/* IRQ for this device */
	 .mode = SPI_LOOP,	/* SPI mode 0 */
	 },
	/* TODO: adding more slaves here */
};

#endif

#if defined(CONFIG_FB_BCM)
/*!
 *  * The LCD definition structure.
 *   */

struct platform_device bcm21553_device_fb = {
	.name   = "LCDfb",
	.id     = -1,
	.dev    = {
		.dma_mask               = (u64 *)~(u32)0,
		.coherent_dma_mask      = ~(u32)0,
	},
	.num_resources = 0,
};
#endif

#ifdef CONFIG_BCM215XX_DSS
static struct lcdc_platform_data_t lcdc_pdata = {
	.gpio = 19,
	.te_supported = true,
};
#endif

#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))

#define BCMBLT_VREG_GPIO        21
#define BCMBLT_N_RESET_GPIO     20
#define BCMBLT_AUX0_GPIO        (-1)	/* clk32 */
#define BCMBLT_AUX1_GPIO        (-1) /* UARTB_SEL */

static struct bcmblt_rfkill_platform_data board_bcmblt_rfkill_cfg = {
	.vreg_gpio = BCMBLT_VREG_GPIO,
	.n_reset_gpio = BCMBLT_N_RESET_GPIO,
	.aux0_gpio = BCMBLT_AUX0_GPIO,	/* CLK32 */
	.aux1_gpio = BCMBLT_AUX1_GPIO,	/* UARTB_SEL, probably not required */
};

static struct platform_device board_bcmblt_rfkill_device = {
	.name = "bcmblt-rfkill",
	.id = -1,
};
#endif

#if defined(CONFIG_CRYPTO_DEV_BRCM_BLKCRYPTO)
static struct resource blkcrypto_resources[] = {
        {
         .start = IO_ADDRESS(BCM21553_CRYPTO_BASE),
         .end = IO_ADDRESS(BCM21553_CRYPTO_BASE) + SZ_64K - 1,
         .flags = IORESOURCE_MEM,
         },
	{
	 .start = BCM21553_CRYPTO_BASE,
	 .end	= BCM21553_CRYPTO_BASE + SZ_64K - 1,
	 .flags	= IORESOURCE_MEM,
	}
};

static struct platform_device board_blkcrypto_device = {
       .name           =       "brcm_blkcrypto",
       .id             =       0,
	.resource	=	blkcrypto_resources,
	.num_resources	=	ARRAY_SIZE(blkcrypto_resources),
};
#endif

#ifdef CONFIG_USB_ANDROID
static struct android_usb_platform_data android_usb_pdata = {
	.vendor_id = 0x18D1,
	.product_id = 0x0005,
	.adb_product_id = 0x0002,
	.version = 0x0100,
	.product_name = "BCM21553-Thunderbird",
	.manufacturer_name = "Broadcom",
	.serial_number="0123456789ABCDEF",
	.nluns = 1,
};

static struct platform_device android_usb_device = {
      .name = "android_usb",
      .id         = -1,
      .dev        = {
            .platform_data = &android_usb_pdata,
      },
};
#endif

/* These are active devices on this board */
static struct platform_device *devices[] __initdata = {
#if defined(CONFIG_SERIAL_8250) || defined(CONFIG_SERIAL_8250_MODULE)
	&bcm21553_serial_device0,
	&bcm21553_serial_device1,
	&bcm21553_serial_device2,
#endif
#if defined(CONFIG_DMADEVICES) && defined(CONFIG_OSDAL_SUPPORT)
	&bcm21xx_dma_device,
#endif
#ifdef CONFIG_MMC_BCM
	&bcm21553_sdhc_slot1,
#if !defined(CONFIG_MTD_ONENAND)
	&bcm21553_sdhc_slot2,
#endif
	&bcm21553_sdhc_slot3,
#endif

#if defined(CONFIG_BCM_WATCHDOG)
	&bcm_watchdog_device,
#endif

#ifdef CONFIG_KEYBOARD_BCM
	&bcm215xx_kp_device,
#endif
#ifdef CONFIG_BCM_AUXADC
	&auxadc_device,
#endif
#if defined(CONFIG_BRCM_HEADSET) || defined(CONFIG_BRCM_HEADSET_MODULE)
	&bcm_headset_device,
#endif
        &bcm_test_vibrator_devices,
#ifdef CONFIG_BACKLIGHT_AAT1401
	&bcm_aat1401_backlight_devices,
#endif
#ifdef CONFIG_BCM_PWM
	&bcm_pwm_device,
#ifdef CONFIG_BACKLIGHT_PWM
	&bcm_backlight_devices,
#endif
#endif
#ifdef CONFIG_MTD_ONENAND_BCM_XBOARD
	&bcm21553_device_onenand,
#endif
#ifdef CONFIG_I2C
	&bcm21553_device_i2c1,
	&bcm21553_device_i2c2,
	&bcm21553_device_i2c3,   
#endif
#if defined(CONFIG_I2C_GPIO)
	&touch_i2c_gpio_device,
	&bma222_i2c_gpio_device,
	&taos_i2c_gpio_device,
	&i2c_gpio_device,
#endif
#ifdef CONFIG_SPI
	&bcm21xx_device_spi,
#endif
#ifdef CONFIG_FB_BCM
	&bcm21553_device_fb,
#endif
#ifdef CONFIG_BCM215XX_DSS
	&bcm215xx_lcdc_device,
#endif
#if defined (CONFIG_BCM_OTP)
	&bcm_otp_device,
#endif
#ifdef CONFIG_BCM_GPIO_VIBRATOR
	&bcm_timed_gpio,
#endif
#ifdef CONFIG_BCM_I2SDAI
    &i2sdai_device,
#endif
#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))
	&board_bcmblt_rfkill_device,
#endif
#ifdef CONFIG_BCM_CPU_FREQ
	&bcm21553_cpufreq_drv,
#endif
#ifdef CONFIG_CPU_FREQ_GOV_BCM21553
	&bcm21553_cpufreq_gov,
#endif
#ifdef CONFIG_BCM_AVS
	&bcm215xx_avs_device,
#endif
#ifdef CONFIG_CRYPTO_DEV_BRCM_BLKCRYPTO
       &board_blkcrypto_device,
#endif
#ifdef CONFIG_USB_ANDROID
        &android_usb_device,
#endif
#if defined (CONFIG_BRCM_V3D)
    &v3d_device,
#endif
};

#ifdef CONFIG_MMC_BCM
int bcmsdhc_ctrl_slot_is_invalid(u8 ctrl_slot)
{
	if (ctrl_slot > 3)
		return -EINVAL;

	return 0;
}
#endif /*CONFIG_MMC_BCM */

static void __init bcm21553_timer_init(void)
{
	struct timer_config bcm21553_timer_config = {
		.ce_module = TIMER_MODULE_GPT,
		.cs_module = TIMER_MODULE_GPT,
		.cp_cs_module = TIMER_MODULE_GPT,
		.ce_base = (void __iomem *)io_p2v(BCM21553_GPTIMER_BASE),
		.cp_cs_base = (void __iomem *)io_p2v(BCM21553_SLPTMR1_BASE),
		.cs_index = 0,
		.ce_index = 1,
		.cp_cs_index = 0,
		.irq = IRQ_GPT1,
	};

	struct gpt_base_config base_config = {
		.avail_bitmap = GPT_AVAIL_BITMAP,
		.base_addr = (void __iomem *)io_p2v(BCM21553_GPTIMER_BASE),
		.irq = IRQ_GPT1,
	};

	gpt_init(&base_config);
	bcm_timer_init(&bcm21553_timer_config);
}

static struct sys_timer bcm21553_timer = {
	.init = bcm21553_timer_init,
};

int board_sysconfig(uint32_t module, uint32_t op)
{
	static DEFINE_SPINLOCK(bcm_syscfg_lock);
	unsigned long flags, auxmic_base;
	int ret = 0;

	spin_lock_irqsave(&bcm_syscfg_lock, flags);
	auxmic_base = (unsigned long) io_p2v(BCM21553_AUXMIC_BASE);
	switch (module) {
	case SYSCFG_LCD:
		if(op == SYSCFG_ENABLE) {
			writel(readl(ADDR_SYSCFG_IOCR6) &
				~(SYSCFG_IOCR6_GPIO25_24_MUX),
				ADDR_SYSCFG_IOCR6);
			writel(readl(ADDR_SYSCFG_IOCR2) |
				(SYSCFG_IOCR2_OSC2_ENABLE),
				ADDR_SYSCFG_IOCR2);
		} else {
			/* Configure IOCR0[29].IOCR0[25] = 10 (GPIO[49:44])*/
			/* Configure IOCR0[29].IOCR0[25] = 00 (LCDCTRL,LCDD[0])*/
			writel((readl(ADDR_SYSCFG_IOCR0) &
				~(SYSCFG_IOCR0_LCD_CTRL_MUX | SYSCFG_IOCR0_MPHI_MUX)),
				ADDR_SYSCFG_IOCR0);
			if (op == SYSCFG_INIT) {
				writel((readl(ADDR_SYSCFG_IOCR0) | SYSCFG_IOCR0_LCD_CTRL_MUX),
				ADDR_SYSCFG_IOCR0);
			}
		}
		break;
	case (SYSCFG_PWM0 + 1): /* PWM1 => LCD Backlight */
		if ((op == SYSCFG_INIT) || (op == SYSCFG_DISABLE))
			writel(readl(ADDR_SYSCFG_IOCR0) &
				~SYSCFG_IOCR0_GPIO17_PWM0_MUX, ADDR_SYSCFG_IOCR0);
		else if (op == SYSCFG_ENABLE)
			writel(readl(ADDR_SYSCFG_IOCR0) |
				SYSCFG_IOCR0_GPIO17_PWM0_MUX, ADDR_SYSCFG_IOCR0);

		break;
	case SYSCFG_RESETREASON_SOFT_RESET:
		if (op == SYSCFG_INIT) {
		 	ret =  readl(ADDR_SYSCFG_PUMR);

		} else if (op == SYSCFG_DISABLE) {

			writel(PUMR_VAL_SOFT_RESET, ADDR_SYSCFG_PUMR);
		} else if (op == SYSCFG_ENABLE) {
		}
		break;
	case SYSCFG_RESETREASON_AP_ONLY_BOOT:
		if (op == SYSCFG_INIT) {
			ret =  readl(ADDR_SYSCFG_PUMR);

		} else if (op == SYSCFG_DISABLE) {

			writel(PUMR_VAL_AP_ONLY_BOOT, ADDR_SYSCFG_PUMR);
		} else if (op == SYSCFG_ENABLE) {
		}
		break;

	 case SYSCFG_CAMERA:
                 if(op == SYSCFG_INIT){
                        u32 val;
                        /* IOCR 0 */
                        val = readl(ADDR_SYSCFG_IOCR0) & ~(SYSCFG_IOCR0_CAMCK_GPIO_MUX);
                        writel(val, ADDR_SYSCFG_IOCR0);
                        /* IOCR 3 */
                        val = readl(ADDR_SYSCFG_IOCR3);
                         /* Clear bits 6,5 and 4 */
                        val &= ~(SYSCFG_IOCR3_CAMCK_DIS |
                                 SYSCFG_IOCR3_CAMD_PD   |
                                 SYSCFG_IOCR3_CAMD_PU   | 
                                 SYSCFG_IOCR3_CAMHVS_PD |
                                 SYSCFG_IOCR3_CAMHVS_PU |
                                 SYSCFG_IOCR3_CAMDCK_PD |
                                 SYSCFG_IOCR3_CAMDCK_PU);
                        writel(val, ADDR_SYSCFG_IOCR3);
                        /* IOCR 4*/
                        val = readl(ADDR_SYSCFG_IOCR4);
                        /* Bits 14:12 */
                        val |= SYSCFG_IOCR4_CAM_DRV_STGTH(0x7);
                        writel(val, ADDR_SYSCFG_IOCR4);
                  } else if(op == SYSCFG_ENABLE){
                        u32 val;
                         /* IOCR 0 */
                        val = readl(ADDR_SYSCFG_IOCR0) & ~(SYSCFG_IOCR0_CAMCK_GPIO_MUX);
                        writel(val, ADDR_SYSCFG_IOCR0);
                        /* IOCR 3 */
                        val = readl(ADDR_SYSCFG_IOCR3);
                        /* Enable CAMCK ouptut */
                        val &= ~(SYSCFG_IOCR3_CAMCK_DIS);
                        writel(val, ADDR_SYSCFG_IOCR3);
                        /* IOCR 5 for trace muxing */
                        val = readl(ADDR_SYSCFG_IOCR5);
                        val &= ~(SYSCFG_IOCR5_CAM_TRACE_EN);
#if defined (CONFIG_CAM_CPI)
			val |= (1 << 26);
#endif
                        writel(val, ADDR_SYSCFG_IOCR5);
#if 1                        
						// GPIO 4 :Output
						val = readl(ADDR_GPIO_IOTR0); // 0x088ce000);
						val &= ~(3 << 8);
						val |= (2 << 8);
						writel(val, ADDR_GPIO_IOTR0);

						// GPIO 35 : Ouput
                        writel(readl(ADDR_SYSCFG_IOCR5)& (~(3 << 10)) , ADDR_SYSCFG_IOCR5);
						val = readl(ADDR_GPIO_IOTR2);
						val &= ~(3 << 6);
						val |= (2 << 6);
						writel(val, ADDR_GPIO_IOTR2);
#endif
                        /* IOCR 4*/
                        val = readl(ADDR_SYSCFG_IOCR4);
                        /* Bits 14:12 Cam drive strength */
                        val |= SYSCFG_IOCR4_CAM_DRV_STGTH(0x7);
                        writel(val, ADDR_SYSCFG_IOCR4);
                        /* IOCR 6 for cam afe and mode */
                        val = readl(ADDR_SYSCFG_IOCR6);
                        val &= ~(SYSCFG_IOCR6_CAM2_CAM1_B);
                        val |= (SYSCFG_IOCR6_CAM2_CAM1_B); /* CAM 1 interface for now */
                        val &= ~(SYSCFG_IOCR6_CAM_MODE(0x3)); /* Clear */
                        val |= (SYSCFG_IOCR6_CAM_MODE(0x3));
                        writel(val, ADDR_SYSCFG_IOCR6);
                        printk(KERN_INFO"Board sys Done enable 0x%x\n", readl(ADDR_SYSCFG_IOCR6));
                  } else if(op == SYSCFG_DISABLE) {
			u32 val;
                          /* IOCR 3 */
                        val = readl(ADDR_SYSCFG_IOCR3);
                        val &= ~(SYSCFG_IOCR3_CAMD_PU   | 
                                 SYSCFG_IOCR3_CAMHVS_PU |
                                 SYSCFG_IOCR3_CAMDCK_PU);
                        val |= (SYSCFG_IOCR3_CAMCK_DIS | 
                                SYSCFG_IOCR3_CAMD_PD   |
                                SYSCFG_IOCR3_CAMHVS_PD | 
                                SYSCFG_IOCR3_CAMDCK_PD);
                        writel(val, ADDR_SYSCFG_IOCR3);
                        /* IOCR 4*/
                        val = readl(ADDR_SYSCFG_IOCR4);
                        /* Bits 14:12 Cam drive strength */
                        val &= ~(SYSCFG_IOCR4_CAM_DRV_STGTH(0x7));
                        /* Drive strength minimum */
                        val |= (SYSCFG_IOCR4_CAM_DRV_STGTH(1));
                        writel(val, ADDR_SYSCFG_IOCR4);
                        /* IOCR 6 for cam afe and mode */
                        val = readl(ADDR_SYSCFG_IOCR6);
                        val &= ~(SYSCFG_IOCR6_CAM_MODE(0x3)); /* Clear */
                        writel(val, ADDR_SYSCFG_IOCR6);
			/* for floor current */
			writel(readl(ADDR_SYSCFG_IOCR5) |
			    SYSCFG_IOCR5_CAM_TRACE_EN
			    , ADDR_SYSCFG_IOCR5);
                   }
                break;
	case SYSCFG_SDHC1:
		if (op == SYSCFG_INIT) {
			u32 val;

#if 0	// Totoro doesn't use SDIO MUX for WLAN. Only on Thunderbird
			/* Offset for IOCR5 = 0x24 */
			/* GPIO35_MUX set to GPIO35 */
			writel((readl(ADDR_SYSCFG_IOCR5) & ~SYSCFG_IOCR5_GPIO35_UARTA_OUT2N_MUX(0x03)),
                		ADDR_SYSCFG_IOCR5);

			/* Make GPIO35 to out direction, and set value 0 */
			gpio_request(SDIO1_WLAN_MUX_TTL, "sdio1_wlan_mux_ttl");
			gpio_direction_output(SDIO1_WLAN_MUX_TTL, 1);
			gpio_set_value(SDIO1_WLAN_MUX_TTL, 0);
			gpio_free(SDIO1_WLAN_MUX_TTL);
#endif
			/* Offset for IOCR0 = 0x00 */
			/* SPI_GPIO_MASK = 0x18 */
			writel((readl(ADDR_SYSCFG_IOCR0) & ~SYSCFG_IOCR0_SD1_MUX(0x3)),
			       ADDR_SYSCFG_IOCR0);
#if 0	       
			writel((readl(ADDR_SYSCFG_IOCR0) | SYSCFG_IOCR0_GPIO52_GPEN7_MUX),
			       ADDR_SYSCFG_IOCR0);
#endif			       

            val = readl(ADDR_SYSCFG_IOCR2);
            val &= ~(SYSCFG_IOCR2_SD1CMD_PULL_CTRL(SD_PULL_DOWN)|SYSCFG_IOCR2_SD1DAT_PULL_CTRL(SD_PULL_DOWN));
            val |= (SYSCFG_IOCR2_SD1CMD_PULL_CTRL(SD_PULL_UP) | SYSCFG_IOCR2_SD1DAT_PULL_CTRL(SD_PULL_UP));
			writel(val, ADDR_SYSCFG_IOCR2);

		        unsigned int regv;
		
		        regv = readl(HW_GPIO_BASE +  8 );									
	           	writel((regv & ~(0x00000c00)) ,HW_GPIO_BASE +  8 ); 	   

		        bcm_gpio_pull_up_down_enable(BCM2091_CLK_REQ_PASS_THOUGH, true);
		        bcm_gpio_pull_up(BCM2091_CLK_REQ_PASS_THOUGH, false);



				// This is to Enable the SYS_CLK_REQ From 2091 for P102 WCG WLAN CARD
			writel((readl(ADDR_CLKPWR_CLK_SYSCLK_REQ_MASK) | 0x00000003),
                		ADDR_CLKPWR_CLK_SYSCLK_REQ_MASK);

		} else if (op == SYSCFG_ENABLE) {
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       & ~(SYSCFG_IOCR2_SD1CMD_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       | SYSCFG_IOCR2_SD1CMD_PULL_CTRL(SD_PULL_UP),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       & ~(SYSCFG_IOCR2_SD1DAT_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       | SYSCFG_IOCR2_SD1DAT_PULL_CTRL(SD_PULL_UP),
			       ADDR_SYSCFG_IOCR2);

				//This is to Enable the SYS_CLK_REQ From 2091 for P102 WCG WLAN CARD

			writel((readl(ADDR_CLKPWR_CLK_SYSCLK_REQ_MASK) | 0x00000003),
                		ADDR_CLKPWR_CLK_SYSCLK_REQ_MASK);

			//Set SDIO1 Driving Strength as 4mA
			writel((readl(ADDR_SYSCFG_IOCR4) | SYSCFG_IOCR4_SD1_DAT_DRV_STGTH(0x2)),
					ADDR_SYSCFG_IOCR4);

			writel((readl(ADDR_SYSCFG_IOCR6) | SYSCFG_IOCR6_SD1_CLK_DRV_STGTH(0x2)),
					ADDR_SYSCFG_IOCR6);

		} else if (op == SYSCFG_DISABLE) {
					u32 val;

            val = readl(ADDR_SYSCFG_IOCR2);
            val &= ~(SYSCFG_IOCR2_SD1CMD_PULL_CTRL(SD_PULL_DOWN)|SYSCFG_IOCR2_SD1DAT_PULL_CTRL(SD_PULL_DOWN));
            val |= (SYSCFG_IOCR2_SD1CMD_PULL_CTRL(SD_PULL_UP) | SYSCFG_IOCR2_SD1DAT_PULL_CTRL(SD_PULL_UP));
			writel(val, ADDR_SYSCFG_IOCR2);
		}
		break;
	case SYSCFG_SDHC2:
		if (op == SYSCFG_INIT) {
			/* Offset for IOCR0 = 0x00 */
			writel((readl(ADDR_SYSCFG_IOCR0) |
				SYSCFG_IOCR0_FLASH_SD2_MUX),
			       ADDR_SYSCFG_IOCR0);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2) &
			       ~(SYSCFG_IOCR2_SD2CMD_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN) |
				 SYSCFG_IOCR2_SD2DATL_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
		} else if (op == SYSCFG_ENABLE) {
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       & ~(SYSCFG_IOCR2_SD2CMD_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       | SYSCFG_IOCR2_SD2CMD_PULL_CTRL(SD_PULL_UP),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       & ~(SYSCFG_IOCR2_SD2DATL_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       | SYSCFG_IOCR2_SD2DATL_PULL_CTRL(SD_PULL_UP),
			       ADDR_SYSCFG_IOCR2);
		} else if (op == SYSCFG_DISABLE) {
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2) &
				~(SYSCFG_IOCR2_SD2CMD_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN) |
			         SYSCFG_IOCR2_SD2DATL_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
		}
		break;
	case SYSCFG_SDHC3:
		if (op == SYSCFG_INIT) {
			/* Offset for IOCR0 = 0x00 */
			/* MSPRO_SD3 = 0x3 */
			writel((readl(ADDR_SYSCFG_IOCR0) &
				~SYSCFG_IOCR0_SD3_MUX(3)),
			       ADDR_SYSCFG_IOCR0);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2) &
			       ~(SYSCFG_IOCR2_SD3CMD_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN) |
				 SYSCFG_IOCR2_SD3DAT_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
		} else if (op == SYSCFG_ENABLE) {
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       & ~(SYSCFG_IOCR2_SD3CMD_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       | SYSCFG_IOCR2_SD3CMD_PULL_CTRL(SD_PULL_UP),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       & ~(SYSCFG_IOCR2_SD3DAT_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2)
			       | SYSCFG_IOCR2_SD3DAT_PULL_CTRL(SD_PULL_UP),
			       ADDR_SYSCFG_IOCR2);
			
			//Set SDIO1 Driving Strength
			printk("SDIO3 Driving Strenght is 4mA\n");
			writel((readl(ADDR_SYSCFG_IOCR4) | SYSCFG_IOCR4_SD3_DAT_DRV_STGTH(0x2)),
							ADDR_SYSCFG_IOCR4);
			
			writel((readl(ADDR_SYSCFG_IOCR4) | SYSCFG_IOCR4_SD3_CLK_DRV_STGTH(0x2)),
							ADDR_SYSCFG_IOCR4);
		} else if (op == SYSCFG_DISABLE) {
			/* Offset for IOCR2 = 0x0c */
			writel(readl(ADDR_SYSCFG_IOCR2) &
				~(SYSCFG_IOCR2_SD3CMD_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN) |
			         SYSCFG_IOCR2_SD3DAT_PULL_CTRL(SD_PULL_UP | SD_PULL_DOWN)),
			       ADDR_SYSCFG_IOCR2);
		}
		break;
#ifdef CONFIG_USB_DWC_OTG
	case SYSCFG_USB:
		if (op == SYSCFG_DISABLE)
			{
				REG_SYS_ANACR9 = (ANACR9_USB_IDLE_ENABLE |
				ANACR9_USB_UTMI_STOP_DIGITAL_CLOCKS |
				ANACR9_USB_UTMI_SOFT_RESET_DISABLE);
				REG_SYS_ANACR9 &= ~(ANACR9_PLL_SUSPEND_ENABLE |
					ANACR9_USB_PLL_POWER_ON);

				REG_CLKPWR_CLK_USB_48_ENABLE &= ~1;
				REG_CLKPWR_CLK_ANALOG_PHASE_ENABLE |= 2; //bit 1
				REG_CLKPWR_CLK_USB_48_ENABLE &= ~1;
				REG_CLKPWR_USBPLL_ENABLE &= ~1;
				REG_SYS_ANACR9 |= ANACR9_USB_AFE_NON_DRVING;

			}
			else if (op == SYSCFG_ENABLE)
			{
				REG_CLKPWR_CLK_USB_48_ENABLE |= 1;
				REG_CLKPWR_CLK_ANALOG_PHASE_ENABLE &= ~2; //bit 1
				REG_CLKPWR_CLK_USB_48_ENABLE |= 1;
				REG_CLKPWR_USBPLL_ENABLE |= 1;

				REG_SYS_ANACR9 = (
				ANACR9_USB_UTMI_STOP_DIGITAL_CLOCKS |
				ANACR9_USB_UTMI_SOFT_RESET_DISABLE |
				ANACR9_USB_PLL_POWER_ON |
				ANACR9_USB_PLL_CAL_ENABLE |
				ANACR9_USB_SELECT_OTG_MODE |
				ANACR9_USB_SELECT_DEVICE_MODE |
				ANACR9_PLL_SUSPEND_ENABLE |
				ANACR9_USB_PPC_PWR_OFF_ANALOG_DRIVERS);

				/* Reset the PHY since this could
				 * be a mode change too */
				REG_SYS_ANACR9 = (
				ANACR9_USB_UTMI_STOP_DIGITAL_CLOCKS |
				ANACR9_USB_PLL_POWER_ON |
				ANACR9_USB_PLL_CAL_ENABLE |
				ANACR9_USB_SELECT_OTG_MODE |
				ANACR9_USB_SELECT_DEVICE_MODE |
				ANACR9_PLL_SUSPEND_ENABLE |
				ANACR9_USB_PPC_PWR_OFF_ANALOG_DRIVERS);

				/* De-activate PHY reset */
				REG_SYS_ANACR9 = (
				ANACR9_USB_UTMI_STOP_DIGITAL_CLOCKS |
				ANACR9_USB_UTMI_SOFT_RESET_DISABLE |
				ANACR9_USB_PLL_POWER_ON |
				ANACR9_USB_PLL_CAL_ENABLE |
				ANACR9_USB_SELECT_OTG_MODE |
				ANACR9_USB_SELECT_DEVICE_MODE |
				ANACR9_USB_PPC_PWR_OFF_ANALOG_DRIVERS |
				ANACR9_PLL_SUSPEND_ENABLE);
			}
		break;
#endif
	case SYSCFG_HEADSET:
		/* power up analog block for headset */
		#define SYSCFG_ANACR2_AUDIO_RXLDO_PWRDOWN 1
		#define SYSCFG_ANACR2_MIC_PWR_DOWN (1<<1)
		
		if (op == SYSCFG_INIT){
			writel(readl(ADDR_SYSCFG_ANACR2) | SYSCFG_ANACR2_AUDIO_RXLDO_PWRDOWN, ADDR_SYSCFG_ANACR2);
			writel(readl(ADDR_SYSCFG_ANACR2) | SYSCFG_ANACR2_MIC_PWR_DOWN, ADDR_SYSCFG_ANACR2);
		}
		else if(op == SYSCFG_ENABLE){
				writel(readl(ADDR_SYSCFG_ANACR2) & ~SYSCFG_ANACR2_AUDIO_RXLDO_PWRDOWN, ADDR_SYSCFG_ANACR2);
		}
		else if(op == SYSCFG_DISABLE){
				if(!(readl(ADDR_SYSCFG_ANACR2) & 0x00000004))
				{
						writel(readl(ADDR_SYSCFG_ANACR2) | SYSCFG_ANACR2_AUDIO_RXLDO_PWRDOWN, ADDR_SYSCFG_ANACR2);
						writel(readl(ADDR_SYSCFG_ANACR2) | SYSCFG_ANACR2_MIC_PWR_DOWN, ADDR_SYSCFG_ANACR2);
				}
		}
 
		break;
	case SYSCFG_AUXMIC:
		if (op == SYSCFG_INIT) {
			writel(AUXMIC_PRB_CYC_MS(64), auxmic_base + AUXMIC_PRB_CYC);
			writel(AUXMIC_MSR_DLY_MS(4), auxmic_base + AUXMIC_MSR_DLY);
			writel(AUXMIC_MSR_INTVL_MS(32), auxmic_base + AUXMIC_MSR_INTVL);
			writel(readl(auxmic_base + AUXMIC_CMC) & ~AUXMIC_CMC_PROB_CYC_INF,
				auxmic_base + AUXMIC_CMC);
			writel(readl(auxmic_base + AUXMIC_MIC) | AUXMIC_MIC_SPLT_MSR_INTR,
					auxmic_base + AUXMIC_MIC);
			writel(readl(auxmic_base + AUXMIC_AUXEN) & ~AUXMIC_ENABLE_BIAS,
					auxmic_base + AUXMIC_AUXEN);
			writel(AUXMIC_MICINTH_ADJ_VAL(13),
					auxmic_base + AUXMIC_MICINTH_ADJ);
			writel(AUXMIC_MICINENTH_ADJ_VAL(9),
					auxmic_base + AUXMIC_MICINENTH_ADJ);
			writel(AUXMIC_MICONTH_ADJ_VAL(0x00),
					auxmic_base + AUXMIC_MICONTH_ADJ);
			writel(AUXMIC_MICONENTH_ADJ_VAL(0x00),
					auxmic_base + AUXMIC_MICONENTH_ADJ);
			writel(readl(auxmic_base + AUXMIC_F_PWRDWN) & ~AUXMIC_F_PWRDWN_ENABLE,
					auxmic_base + AUXMIC_F_PWRDWN);
			/* Enable the debounce counter for headset insertion */
			writel((readl(ADDR_IRQ_ICCR)|(INTC_ICCR_MICINDBEN | INTC_ICCR_MICINDBC(3))),ADDR_IRQ_ICCR);
		}

	if (op == SYSCFG_ENABLE) {
			
			writel(AUXMIC_PRB_CYC_MS(64), auxmic_base + AUXMIC_PRB_CYC);
			writel(AUXMIC_MSR_DLY_MS(4), auxmic_base + AUXMIC_MSR_DLY);
			writel(AUXMIC_MSR_INTVL_MS(32), auxmic_base + AUXMIC_MSR_INTVL);
			writel(readl(auxmic_base + AUXMIC_MIC) | AUXMIC_MIC_SPLT_MSR_INTR,
					auxmic_base + AUXMIC_MIC);
			writel(readl(auxmic_base + AUXMIC_AUXEN) | AUXMIC_ENABLE_BIAS,
					auxmic_base + AUXMIC_AUXEN);
			writel(readl(auxmic_base + AUXMIC_CMC) | AUXMIC_CMC_PROB_CYC_INF,
					auxmic_base + AUXMIC_CMC);

			writel(AUXMIC_MICINTH_ADJ_VAL(13),
					auxmic_base + AUXMIC_MICINTH_ADJ);
			writel(AUXMIC_MICINENTH_ADJ_VAL(9),
					auxmic_base + AUXMIC_MICINENTH_ADJ);
			writel(AUXMIC_MICONTH_ADJ_VAL(0x00),
					auxmic_base + AUXMIC_MICONTH_ADJ);
			writel(AUXMIC_MICONENTH_ADJ_VAL(0x00),
					auxmic_base + AUXMIC_MICONENTH_ADJ);
			
			writel(readl(auxmic_base + AUXMIC_F_PWRDWN) & ~AUXMIC_F_PWRDWN_ENABLE,
					auxmic_base + AUXMIC_F_PWRDWN);
		}
		if (op == (SYSCFG_ENABLE | SYSCFG_DISABLE)) { // LP mode
			writel(readl(auxmic_base + AUXMIC_AUXEN) & ~AUXMIC_ENABLE_BIAS,
					auxmic_base + AUXMIC_AUXEN);
                        writel(readl(auxmic_base + AUXMIC_CMC) | AUXMIC_CMC_PROB_CYC_INF,
					auxmic_base + AUXMIC_CMC);
			writel(readl(auxmic_base + AUXMIC_F_PWRDWN) & ~AUXMIC_F_PWRDWN_ENABLE,
					auxmic_base + AUXMIC_F_PWRDWN);
		}
		if (op == SYSCFG_DISABLE) { // OFF mode
			writel(readl(auxmic_base + AUXMIC_AUXEN) & ~AUXMIC_ENABLE_BIAS,
				auxmic_base + AUXMIC_AUXEN);
			writel(readl(auxmic_base + AUXMIC_CMC) & ~AUXMIC_CMC_PROB_CYC_INF,
					auxmic_base + AUXMIC_CMC);
						writel(readl(auxmic_base + AUXMIC_F_PWRDWN) |AUXMIC_F_PWRDWN_ENABLE,
							auxmic_base + AUXMIC_F_PWRDWN);
		}
		break;
	case SYSCFG_SPI1:
		/* During init 21:11 is set to 01 to select UARTC
		 * To enable SPI1 set [21:11] =  00
		 * To disable SPI1 set [21:11] = 01 to revert UARTC setting
		 * We toggle Bit 11 to enable/disable SPI, and bit 21 need
		 * not be touched
		 */
		if (op == SYSCFG_ENABLE)	{
			 writel((readl(ADDR_SYSCFG_IOCR0) &
				~(SYSCFG_IOCR0_SPIL_GPIO_MUX
				| SYSCFG_IOCR0_SPIH_GPIO_MUX)),
		                ADDR_SYSCFG_IOCR0);
		}
		/* Set Bits [21:11] = 01 to disable SPI1 and enable UARTC */
		if (op == SYSCFG_DISABLE)	{
			 writel((readl(ADDR_SYSCFG_IOCR0) |
				(SYSCFG_IOCR0_SPIL_GPIO_MUX
				 | SYSCFG_IOCR0_SPIH_GPIO_MUX)),
                		ADDR_SYSCFG_IOCR0);
		}
		break;
	case SYSCFG_SYSTEMINIT:
		if (op == SYSCFG_INIT) {
			/* BIT8 | BIT12 | BIT13 | BIT14 | BIT15:16 | BIT23 | BIT24 | BIT27 */
			writel(readl(ADDR_SYSCFG_IOCR0) |
				SYSCFG_IOCR0_SPI_UARTC_MUX |
				/*SYSCFG_IOCR0_GPIO52_GPEN7_MUX |*/
				/*SYSCFG_IOCR0_GPIO53_GPEN8_MUX |*/
				SYSCFG_IOCR0_GPEN9_SPI_GPIO54_L_MUX |
				SYSCFG_IOCR0_GPIO55_GPEN10_MUX(0x3) |
				SYSCFG_IOCR0_PCM_SPI2_GPIO4043_MUX(0x02) |
				SYSCFG_IOCR0_DIGMIC_GPIO6362_MUX |
				SYSCFG_IOCR0_GPEN9_SPI_GPIO54_H_MUX
				, ADDR_SYSCFG_IOCR0);
			/* SD1/SD2/SD3 DAT[0:3]/CMD lines pulled down
			 * UARTA enable internal pullup */
			/* BIT10 | BIT12 | BIT20 | BIT23 | BIT25 | BIT27 | BIT29 | BIT31 */
			writel(readl(ADDR_SYSCFG_IOCR2) |
				SYSCFG_IOCR2_LCDDATA_PULL_CTRL(0x1) |
				SYSCFG_IOCR2_UARTA_PULL_CTRL(0x1) |
				SYSCFG_IOCR2_SD3CMD_PULL_CTRL(SD_PULL_DOWN) |
				SYSCFG_IOCR2_SD3DAT_PULL_CTRL(SD_PULL_DOWN) |
				SYSCFG_IOCR2_SD1CMD_PULL_CTRL(SD_PULL_DOWN) |
				SYSCFG_IOCR2_SD1DAT_PULL_CTRL(SD_PULL_DOWN) |
				SYSCFG_IOCR2_SD2CMD_PULL_CTRL(SD_PULL_DOWN) |
				SYSCFG_IOCR2_SD2DATL_PULL_CTRL(SD_PULL_DOWN)
				, ADDR_SYSCFG_IOCR2);
			/* BIT21 | BIT27 | BIT31 */
			writel(readl(ADDR_SYSCFG_IOCR3) |
				SYSCFG_IOCR3_TWIF_ENB |
				SYSCFG_IOCR3_SIMDAT_PU | SYSCFG_IOCR3_I2C3_EN
				, ADDR_SYSCFG_IOCR3);
			/* UARTB enable internal pullup */
			/* BIT9 | BIT11 | BIT14 */
			writel(readl(ADDR_SYSCFG_IOCR5) |
//			    SYSCFG_IOCR5_GPIO34_UARTA_OUT1N_MUX(0x2) |
			    SYSCFG_IOCR5_GPIO35_UARTA_OUT2N_MUX(0x2) |
			    SYSCFG_IOCR5_GPIO15L_DRV_STGTH(0x1), ADDR_SYSCFG_IOCR5);
			/* SD2 DAT[7:4] pulled down */
			/* BIT21 */
			writel(readl(ADDR_SYSCFG_IOCR6) |
			    SYSCFG_IOCR6_SD2DATH_PULL_CTRL(SD_PULL_DOWN)
			    , ADDR_SYSCFG_IOCR6);
			/* BIT31 */
			writel(readl(ADDR_SYSCFG_IOCR7) |
				SYSCFG_IOCR7_RFGPIO5_GPIO7_MUX,
					ADDR_SYSCFG_IOCR7);
#ifdef CONFIG_ARCH_BCM21553_B1
			/*Disable Hantro performance ECO*/
			writel(readl(ADDR_SYSCFG_IOCR10) |
				SYSCFG_IOCR10_DEC_SYNC_BRG_INCR_TO_INCR4 |
				SYSCFG_IOCR10_ENC_SYNC_BRG_INCR_TO_INCR4,
				ADDR_SYSCFG_IOCR10);
#endif

                        /* Enable BSC3 on GPIO7 and GPIO15 */
                        /* Configure IOCR3[31].IOCR10[29] = 1 0 */
                        writel(readl(ADDR_SYSCFG_IOCR3) |
                                SYSCFG_IOCR3_I2C3_EN,
                                ADDR_SYSCFG_IOCR3);
                        writel(readl(ADDR_SYSCFG_IOCR10) &
                                ~SYSCFG_IOCR10_BSC3_GPIOH_ENABLE,
                                ADDR_SYSCFG_IOCR10);

			bcm21553_cam_lmatrix_prio();
#define EXT_CTRL_REQ13 26
#define EXT_CTRL_REQ2	4
			writel(EXT_BY_32AHB_L_CYC << EXT_CTRL_REQ13,
					ADDR_SYSCFG_SYSCONF_AHB_CLK_EXTEND0);
			writel(EXT_BY_32AHB_L_CYC << EXT_CTRL_REQ2,
					ADDR_SYSCFG_SYSCONF_AHB_CLK_EXTEND1);

			/* for floor current */
 			writel(readl(ADDR_SYSCFG_IOCR5) |
			    SYSCFG_IOCR5_CAM_TRACE_EN
			    , ADDR_SYSCFG_IOCR5);

			// GPEN11 to GPIO56, set IOCR0[20]/[17] bit to 11.
   			writel(readl(ADDR_SYSCFG_IOCR0) |
    				SYSCFG_IOCR0_GPEN11_SPI_GPIO56_L_MUX |
    				SYSCFG_IOCR0_GPEN11_SPI_GPIO56_H_MUX,
    				ADDR_SYSCFG_IOCR0);
			
			/*WLAN_GPIO_INIT*/
			bcm_wlan_Gpioinit_Setting();

			/*PMU_GPIO_INIT*/			
			Bcm_PMU_GpioInit_Setting();
	
			/*HEADSET_GPIO_INIT*/			
			Bcm_HEADSET_GpioInit_Setting();
			
	
			
		}
		break;
#ifdef CONFIG_BRCM_V3D
	case SYSCFG_V3D:
		if (op == SYSCFG_INIT) {
			writel(SYSCFG_V3DRSTR_RST, ADDR_SYSCFG_V3DRSTR);
			writel(~SYSCFG_V3DRSTR_RST, ADDR_SYSCFG_V3DRSTR);
		}
		break;
#endif
	case SYSCFG_HAPTIC_VIB:
		if (op == SYSCFG_INIT) {
			/* Selecting GPIO56 */
			/* BIT17 | BIT20 */
			writel(readl(ADDR_SYSCFG_IOCR0) |
				SYSCFG_IOCR0_GPEN11_SPI_GPIO56_L_MUX |
				SYSCFG_IOCR0_GPEN11_SPI_GPIO56_H_MUX, ADDR_SYSCFG_IOCR0);
		}
		break;
	case SYSCFG_CSL_DMA:
		if(op == SYSCFG_ENABLE)
			writel(SYSCFG_AHB_PER_CLK_EN, ADDR_SYSCFG_DMAC_AHB_CLK_MODE);
		else if(op == SYSCFG_DISABLE)
			writel(~SYSCFG_AHB_PER_CLK_EN, ADDR_SYSCFG_DMAC_AHB_CLK_MODE);
		break;
	case SYSCFG_SENSORS:
		if(op == SYSCFG_ENABLE){
			writel(readl(IO_ADDRESS(ADDR_SYSCFG_IOCR3_PHYS)) &0x7fffffff  , IO_ADDRESS(ADDR_SYSCFG_IOCR3_PHYS));	//Disable the 3rd BSC on GPIO7, GPIO15
			writel(readl(HW_GPIO_BASE) & ~(0x3 << (ACC_SDA * 2))  | (0x2 << (ACC_SDA * 2)) , HW_GPIO_BASE);
			writel(readl(HW_GPIO_BASE) & ~(0x3 << (ACC_SCL * 2))  | (0x2 << (ACC_SCL * 2)) , HW_GPIO_BASE);
			writel(readl(HW_GPIO_BASE) & ~(0x3 << (GEO_SDA * 2))  | (0x2 << (GEO_SDA * 2)) , HW_GPIO_BASE);
			writel(readl(HW_GPIO_BASE) & ~(0x3 << (GEO_SCL * 2))  | (0x2 << (GEO_SCL * 2)) , HW_GPIO_BASE);
			writel(readl(HW_GPIO_BASE+4) & ~(0x3 <<( PROXI_SCL-16)*2 )  | (0x2 << ( PROXI_SCL-16)*2) , HW_GPIO_BASE+4);
			writel(readl(HW_GPIO_BASE+4) & ~(0x3 <<( PROXI_SDA-16)*2)  | (0x2 << ( PROXI_SDA-16)*2) , HW_GPIO_BASE+4);
		}
		else if (op == SYSCFG_INIT){
		/*[todo]Interrupt mode config*/
		}
		break;

	case SYSCFG_CP_START:
		if (op == SYSCFG_INIT) {
			writel(readl(ADDR_SYSCFG_IOCR3) |
				SYSCFG_IOCR3_TWIF_ENB,
				ADDR_SYSCFG_IOCR3);
		}
		break;
	default:
		pr_info("%s: inval arguments\n", __func__);
		spin_unlock_irqrestore(&bcm_syscfg_lock, flags);
		return -EINVAL;
	}
	spin_unlock_irqrestore(&bcm_syscfg_lock, flags);
	return ret;
}

EXPORT_SYMBOL(board_sysconfig);

static void __init bcm21553_anthenaray_init(void)
{
	/* Configure the SYSCFG Registers */
	board_sysconfig(SYSCFG_SYSTEMINIT, SYSCFG_INIT);
}

static void athenaray_add_platform_data(void)
{
#if defined(CONFIG_SERIAL_8250) || defined(CONFIG_SERIAL_8250_MODULE)
	bcm21553_serial_device0.dev.platform_data = &serial_platform_data0;
	bcm21553_serial_device1.dev.platform_data = &serial_platform_data1;
	bcm21553_serial_device2.dev.platform_data = &serial_platform_data2;
#endif
#ifdef CONFIG_MMC
	bcm21553_sdhc_slot1.dev.platform_data = &bcm21553_sdhc_data1;
#if !defined(CONFIG_MTD_ONENAND)
	bcm21553_sdhc_slot2.dev.platform_data = &bcm21553_sdhc_data2;
#endif
	bcm21553_sdhc_slot3.dev.platform_data = &bcm21553_sdhc_data3;
#endif
#ifdef CONFIG_BCM_PWM
	bcm_pwm_device.dev.platform_data = &pwm_dev;
#ifdef CONFIG_BACKLIGHT_PWM
	bcm_backlight_devices.dev.platform_data = &bcm_backlight_data;
#endif
#endif
#if defined(CONFIG_I2C_BOARDINFO)
	bcm21553_device_i2c1.dev.platform_data = &i2c1_host_pdata;
#endif
#ifdef CONFIG_BACKLIGHT_AAT1401
	bcm_aat1401_backlight_devices.dev.platform_data = &bcm_aat1401_backlight_data;
#endif
#ifdef CONFIG_KEYBOARD_BCM
	bcm215xx_kp_device.dev.platform_data = &bcm215xx_keypad_data;
#endif
#ifdef CONFIG_BCM_AUXADC
	auxadc_device.dev.platform_data = &adcplat;
#endif
#ifdef CONFIG_SPI
	bcm21xx_device_spi.dev.platform_data = &bcm21xx_spi_info;
#endif
#if defined(CONFIG_BRCM_HEADSET)  || defined(CONFIG_BRCM_HEADSET_MODULE)
	bcm_headset_device.dev.platform_data = &headset_pd;
#endif
#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))
	board_bcmblt_rfkill_device.dev.platform_data = &board_bcmblt_rfkill_cfg;
#endif
#ifdef CONFIG_BCM215XX_DSS
	bcm215xx_lcdc_device.dev.platform_data = &lcdc_pdata;
#endif
}

static int __init arch_late_init(void)
{
	static dma_addr_t dma_address;
        static void *alloc_mem;
	pr_info("arch_late_init\n");
#ifdef CONFIG_BCM_GPIO_VIBRATOR
	vibrator_init();
#endif
#ifdef CONFIG_BACKLIGHT_PWM
#define GPIO_LCD_BACKLIGHT	(17)
	/*
	 * This is required as bootloader uses BackLight in GPIO
	 * as against Linux kernel using in PWM mode.
	 * Note: Kernel puts this pin in GPIO mode during suspend
	 */
	gpio_request(GPIO_LCD_BACKLIGHT, "gpio_bl");
	gpio_direction_output(GPIO_LCD_BACKLIGHT, 0);
	gpio_free(GPIO_LCD_BACKLIGHT);
#endif
	gpio_request(GPS_CNTIN_CLK_ENABLE, "cntin clock");
	gpio_direction_input(GPS_CNTIN_CLK_ENABLE);
	bcm_gpio_pull_up(GPS_CNTIN_CLK_ENABLE, false);
	bcm_gpio_pull_up_down_enable(GPS_CNTIN_CLK_ENABLE, true);
	gpio_free(GPS_CNTIN_CLK_ENABLE);
	#if defined (CONFIG_ANDROID_PMEM)
		alloc_mem = cam_mempool_base;
		dma_address = (dma_addr_t) virt_to_phys(alloc_mem);
		if (alloc_mem != NULL) {
                         android_pmem_pdata.start = dma_address;
                         android_pmem_pdata.size = PMEM_ADSP_SIZE;
                         platform_device_register(&android_pmem_device);
                        printk(" ****** %s:Success PMEM alloc 0x%x ****** \n", __func__,
                                 dma_address);
                 } else {
                        printk("******* %s:Fail to alloc memory ****** \n", __func__);
                 }
	#endif

#ifdef CONFIG_BROADCOM_WIFI_RESERVED_MEM
	brcm_init_wifi_mem();
#endif

	return 0;
}
late_initcall(arch_late_init);

static void __init update_pm_sysparm(void)
{
#if defined(CONFIG_BCM_AVS)
	update_avs_sysparm();
#endif
}

static void __init bcm21553_init_machine(void)
{
	bcm21553_platform_init();
	bcm21553_anthenaray_init();
	athenaray_add_i2c_slaves();
	athenaray_add_platform_data();
	platform_add_devices(devices, ARRAY_SIZE(devices));
#ifdef CONFIG_SPI
	/*Function to register SPI board info : required when spi device is
	   present */
	spi_register_board_info(spi_slave_board_info,
				ARRAY_SIZE(spi_slave_board_info));
#endif
#if 0
        {
                static dma_addr_t dma_address;
                static void *alloc_mem;

                alloc_mem =
                    dma_alloc_coherent(NULL, PMEM_ADSP_SIZE, &dma_address,
                                       GFP_ATOMIC | GFP_DMA);
                if (alloc_mem != NULL) {
                        android_pmem_pdata.start = dma_address;
                        android_pmem_pdata.size = PMEM_ADSP_SIZE;
                        platform_device_register(&android_pmem_device);
                        pr_info("%s:Success PMEM alloc 0x%x\n", __func__,
                                dma_address);
                } else {
                        pr_info("%s:Fail to alloc memory\n", __func__);
                }
        }
#endif
	update_pm_sysparm();
}

#define LOKE_MAGIC_CODE	0x66262564
#define LOKE_MAGIC_AREA	0xE0004000
static int ramdump_panic(struct notifier_block *this, unsigned long event, void *ptr)
{
	writel(LOKE_MAGIC_CODE, LOKE_MAGIC_AREA);

	panic_timeout = 2;

	return NOTIFY_DONE;
}

static struct notifier_block panic_block = {
	.notifier_call = ramdump_panic,
};

static int __init ramdump_init(void)
{
	atomic_notifier_chain_register(&panic_notifier_list, &panic_block);
	return 0;
}

module_init(ramdump_init);

/* TODO: Replace BCM1160 with BCM21553/AthenaRay once registered */
MACHINE_START(BCM1160, "BCM21553 ThunderbirdEDN31 platform")
	/* Maintainer: Broadcom Corporation */
	.phys_io = BCM21553_UART_A_BASE,
	.io_pg_offst = (IO_ADDRESS(BCM21553_UART_A_BASE) >> 18) & 0xfffc,
	.boot_params = (PHYS_OFFSET + 0x100),
	.map_io = bcm21553_map_io,
	.init_irq = bcm21553_init_irq,
	.timer = &bcm21553_timer,
	.init_machine = bcm21553_init_machine,
MACHINE_END
