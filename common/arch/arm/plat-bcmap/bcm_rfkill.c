/******************************************************************************
* Copyright 2010 Broadcom Corporation.  All rights reserved.
*
*      @file   arch/arm/plat-bcmap/bcm_rfkill.c
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
******************************************************************************/

/*
 * Broadcom Bluetooth rfkill power control via GPIO. This file is
 * board dependent!
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/rfkill.h>
#include <linux/gpio.h>
#include <linux/broadcom/bcmblt-rfkill.h>
#include <plat/bcm_rfkill.h>

#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))

static int bcmblt_rfkill_set_power(void *data, bool blocked)
{
	int vreg_gpio =
		((struct bcmblt_rfkill_platform_data *)data)->vreg_gpio;
	int n_reset_gpio =
	    ((struct bcmblt_rfkill_platform_data *)data)->n_reset_gpio;
	int aux0_gpio =
		((struct bcmblt_rfkill_platform_data *)data)->aux0_gpio;
	int aux1_gpio =
		((struct bcmblt_rfkill_platform_data *)data)->aux1_gpio;

	pr_info("bcm_blt_rfkill_setpower(): vreg_gpio: %d, n_reset_gpio: %d, aux0_gpio: %d, aux1_gpio: %d\n",
	        vreg_gpio, n_reset_gpio, aux0_gpio, aux1_gpio);

	if (blocked == false) {	/* Transmitter ON (Unblocked) */
		if (BCMBLT_UNUSED_GPIO != aux1_gpio)
			gpio_set_value(aux1_gpio, BCMBLT_AUX1_ON);
		if (BCMBLT_UNUSED_GPIO != aux0_gpio)
			gpio_set_value(aux0_gpio, BCMBLT_AUX0_ON);
		if (BCMBLT_UNUSED_GPIO != n_reset_gpio)
			gpio_set_value(n_reset_gpio, BCMBLT_N_RESET_ON);
		gpio_set_value(vreg_gpio, BCMBLT_VREG_ON);
		if (BCMBLT_UNUSED_GPIO != n_reset_gpio)
			gpio_set_value(n_reset_gpio, BCMBLT_N_RESET_OFF);
		pr_info("bcm_blt_rfkill_setpower: unblocked %s\n",
			gpio_get_value(vreg_gpio) ? "High [POWER ON]" :
			"Low [POWER_OFF]");
	} else {		/* Transmitter OFF (Blocked) */
		if (BCMBLT_UNUSED_GPIO != n_reset_gpio)
			gpio_set_value(n_reset_gpio, BCMBLT_N_RESET_ON);
		gpio_set_value(vreg_gpio, BCMBLT_VREG_OFF);
		if (BCMBLT_UNUSED_GPIO != aux1_gpio)
			gpio_set_value(aux1_gpio, BCMBLT_AUX1_OFF);
		if (BCMBLT_UNUSED_GPIO != aux0_gpio)
			gpio_set_value(aux0_gpio, BCMBLT_AUX0_OFF);
		pr_info("bcm_blt_rfkill_setpower: blocked %s\n",
			gpio_get_value(vreg_gpio) ? "High [POWER ON]" :
			"Low [POWER_OFF]");
	}
	return 0;
}

static const struct rfkill_ops bcmblt_rfkill_ops = {
	.set_block = bcmblt_rfkill_set_power,
};

static int bcmblt_rfkill_probe(struct platform_device *pdev)
{
	int rc = 0;
	struct bcmblt_rfkill_platform_data *pdata = pdev->dev.platform_data;

	gpio_request(pdata->vreg_gpio, "rfkill_vreg_gpio");
	pr_err("bcmblt_rfkill_probe:  Set vreg_gpio: %d, level: %s\n",
	       pdata->vreg_gpio,
	       gpio_get_value(pdata->vreg_gpio) ? "High" : "Low");
    gpio_export(pdata->vreg_gpio, false);
	gpio_direction_output(pdata->vreg_gpio, BCMBLT_VREG_OFF);

	if (BCMBLT_UNUSED_GPIO != pdata->n_reset_gpio) {
		gpio_request(pdata->n_reset_gpio, "rfkill_reset_gpio");
		gpio_direction_output(pdata->n_reset_gpio, BCMBLT_N_RESET_ON);
		pr_err("bcmblt_probe: n_reset: %s\n",
		       gpio_get_value(pdata->
			      n_reset_gpio) ? "High [chip out of reset]"
		       : "Low [put into reset]");
	}
	if (BCMBLT_UNUSED_GPIO != pdata->aux0_gpio) {	/* CLK32 */
		gpio_request(pdata->aux0_gpio, "rfkill_aux0_gpio");
		gpio_direction_output(pdata->aux0_gpio, BCMBLT_AUX0_OFF);
		pr_err("bcmblt_probe:  aux0: %s\n",
		       gpio_get_value(pdata->aux0_gpio) ? "High" : "Low");
	}

	if (BCMBLT_UNUSED_GPIO != pdata->aux1_gpio) {	/* UARTB_SEL */
		gpio_request(pdata->aux1_gpio, "rfkill_aux1_gpio");
		gpio_direction_output(pdata->aux1_gpio, BCMBLT_AUX1_OFF);
		pr_err("bcmblt_probe:  aux1: %s\n",
		       gpio_get_value(pdata->aux1_gpio) ? "High" : "Low");
	}

	pdata->rfkill =
	    rfkill_alloc("bcmblt", &pdev->dev, RFKILL_TYPE_BLUETOOTH,
			 &bcmblt_rfkill_ops, pdata);

	if (unlikely(!pdata->rfkill))
		return -ENOMEM;

	/* Keep BT Blocked by default as per above init */
	rfkill_init_sw_state(pdata->rfkill, true);

	rc = rfkill_register(pdata->rfkill);

	if (unlikely(rc))
		rfkill_destroy(pdata->rfkill);

	return 0;
}


static int bcmblt_rfkill_remove(struct platform_device *pdev)
{
	struct bcmblt_rfkill_platform_data *pdata = pdev->dev.platform_data;

	rfkill_unregister(pdata->rfkill);
	rfkill_destroy(pdata->rfkill);
	/* Free the GPIO resources */
	gpio_free(pdata->vreg_gpio);
	gpio_free(pdata->n_reset_gpio);
	gpio_free(pdata->aux0_gpio);
	gpio_free(pdata->aux1_gpio);

	return 0;
}

static struct platform_driver bcmblt_rfkill_platform_driver = {
	.probe = bcmblt_rfkill_probe,
	.remove = bcmblt_rfkill_remove,
	.driver = {
		   .name = "bcmblt-rfkill",
		   .owner = THIS_MODULE,
		   },
};

static int __init bcmblt_rfkill_init(void)
{
	return platform_driver_register(&bcmblt_rfkill_platform_driver);
}

static void __exit bcmblt_rfkill_exit(void)
{
	platform_driver_unregister(&bcmblt_rfkill_platform_driver);
}

module_init(bcmblt_rfkill_init);
module_exit(bcmblt_rfkill_exit);

MODULE_DESCRIPTION("bcmblt-rfkill");
MODULE_AUTHOR("Broadcom");
MODULE_LICENSE("GPL");
#endif
