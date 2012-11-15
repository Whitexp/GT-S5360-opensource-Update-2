/*
 * BaCeM-tweak is small kernel module to enable UV on Samsung Galaxy Y.
 *
 * Copyright (c) 2012 Irfan Bagus. All rights reserved.
 * Written by Irfan Bagus (irfanbagus@gmail.com)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kallsyms.h>
#include <linux/cpufreq.h>
#include <linux/err.h>
#include <linux/regulator/consumer.h>
#include <plat/bcm_cpufreq_drv.h>
#include <linux/sysctl.h>
#include <asm/uaccess.h>

#define VOLT_CPU_LEN	7
#define VOLT_CPU_LOW	1240000
#define VOLT_CPU_HIGH	1300000
#define FIX_CPUFREQ	1

static char volt_cpu_str[VOLT_CPU_LEN+4];

#ifdef FIX_CPUFREQ
static struct cpufreq_frequency_table *cpufreq_fix_table = NULL;
#endif

static int read_voltage(char *in,size_t in_size,char *out_str,int *out_int) {

	int i = 0,j = 0,number = 0;
	int negative = 0;
	
	*out_int = 0;
	// read front padding
	while(i<in_size) {
		if ((in[i] != ' ')&&(in[i] != '\t'))
			break;
		i++;
	}
	// read sign padding
	if (i<in_size) {
		if (in[i] == '-') {
			out_str[j] = '-';
			negative = 1;
			i++;
			j++;
		}
		else if (in[i] == '+')
			i++;
	}
	// read number
	while(i<in_size) {
		if ((in[i] < '0')||(in[i] > '9'))
			break;
		out_str[j] = in[i];
		*out_int = (*out_int)*10+(in[i]-'0');
		i++;
		j++;
		number++;
	}
	// read mV
	if ((in_size-i)>1)
		if ((in[i] == 'm')&&((in[i+1] == 'v')||(in[i+1] == 'V')))
			i += 2;
	// read back padding
	while(i<in_size) {
		if ((in[i] != ' ')&&(in[i] != '\t')&&(in[i] != '\n'))
			break;
		i++;
	}
	if ((i != in_size)||(number<1))
		return 0;
	*out_int = negative ? -(*out_int) : *out_int;
	out_str[j] = 0;
	return j;
}

static int set_volt_cpu(int voltage) {
	struct bcm_cpu_info *cpu_info;
	struct bcm_freq_tbl *freq_tbl;
	struct regulator *cpu_regulator;
	int freq_tbl_size;
	int a,c,i,new_volt,old_volt,reg_volt;
	
	cpu_info = (struct bcm_cpu_info*)kallsyms_lookup_name(
			"bcm215xx_cpu_info");
	if (cpu_info)
	{
		freq_tbl = cpu_info[0].freq_tbl;
		freq_tbl_size = cpu_info[0].num_freqs;
		cpu_regulator = regulator_get(NULL,
			cpu_info[0].cpu_regulator);
		reg_volt = IS_ERR(cpu_regulator) ? 0 :
			regulator_get_voltage(cpu_regulator);
		a = (VOLT_CPU_HIGH - VOLT_CPU_LOW) /
			(freq_tbl[freq_tbl_size-1].cpu_freq - 
			freq_tbl[0].cpu_freq);
		c = VOLT_CPU_LOW - a*freq_tbl[0].cpu_freq;

		local_irq_disable();
		for (i=0;i<freq_tbl_size;i++) {
			old_volt = freq_tbl[i].cpu_voltage;
			new_volt = a*freq_tbl[i].cpu_freq+c+voltage;
			/* rounding */
			freq_tbl[i].cpu_voltage = (new_volt/20000)*20000;
			if ((new_volt-freq_tbl[i].cpu_voltage)>=10000)
				freq_tbl[i].cpu_voltage += 20000;
			/* enable voltage */
			if ((reg_volt == old_volt)&&
					(!IS_ERR(cpu_regulator)))
				regulator_set_voltage(cpu_regulator,
					freq_tbl[i].cpu_voltage,
					freq_tbl[i].cpu_voltage);
		}
		local_irq_enable();

		if (!IS_ERR(cpu_regulator))
			regulator_put(cpu_regulator);

		return -1;
	}
	return 0;
}

#ifdef FIX_CPUFREQ
static void fix_freq_table(void) {
	struct bcm_cpu_info *cpu_info;
	struct cpufreq_policy *policy;
	int i;

	/* table */
	if (!cpufreq_frequency_get_table(0)) {
		cpu_info = (struct bcm_cpu_info*)kallsyms_lookup_name(
				"bcm215xx_cpu_info");
		if (cpu_info) {
			cpufreq_fix_table = kmalloc(sizeof(struct cpufreq_frequency_table)*
				(cpu_info[0].num_freqs+1),GFP_KERNEL);
			for (i=0;i<cpu_info[0].num_freqs;i++) {
				cpufreq_fix_table[i].index = i;
				cpufreq_fix_table[i].frequency =
					cpu_info[0].freq_tbl[i].cpu_freq*1000;
			}
			i = cpu_info[0].num_freqs;
			cpufreq_fix_table[i].index = i;
			cpufreq_fix_table[i].frequency = CPUFREQ_TABLE_END;
			cpufreq_frequency_table_get_attr(cpufreq_fix_table,0);
		}
	}

	/* latency */
	policy = cpufreq_cpu_get(0);
	if (policy) {
		if (policy->cpuinfo.transition_latency > 10000000)
			policy->cpuinfo.transition_latency = 1000000;
		cpufreq_cpu_put(policy);
	}
}
#endif

static int proc_volt_cpu(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp, loff_t *ppos) {
	size_t len;
	int ret;
	char temp[VOLT_CPU_LEN+4];
	int voltage;

	if (write) {
		if (*lenp>VOLT_CPU_LEN)
			return 0;
		len = read_voltage((char*)buffer,*lenp,temp,&voltage);
		if (len) {
			if (set_volt_cpu(voltage*1000)) {
				memcpy(volt_cpu_str,temp,len);
				strcpy(&(volt_cpu_str[len]),"mV\n");
			}
		}
	}
	else {
		len = strlen(volt_cpu_str) - *ppos;
		len = *lenp < len ? *lenp : len;
		if (len) {
			ret = copy_to_user(buffer,volt_cpu_str,len);
			len = ret ? 0 : len;
		}
		*lenp = len;
		*ppos +=len;
	}
	return 0;
}

static struct ctl_table bacem_voltage_cpu[] = {
        {
        .procname	= "cpu",
        .mode		= 0644,
        .proc_handler	= &proc_volt_cpu,
        },
        {NULL}
};

static struct ctl_table bacem_voltage[] = {
        {
        .procname	= "volt",
        .mode		= 0555,
        .child		= bacem_voltage_cpu,
        },
        {NULL}
};

static struct ctl_table bacem[] = {
        {
        .procname	= "bacem",
        .mode		= 0555,
        .child		= bacem_voltage,
        },
        {NULL}
};

static struct ctl_table_header *bacem_header = NULL;

static int __init bacem_tweak_init(void)
{
	strcpy(volt_cpu_str,"0mV\n");
	bacem_header = register_sysctl_table(bacem);
#ifdef FIX_CPUFREQ
	fix_freq_table();
#endif
	return 0;
}

static void __exit bacem_tweak_exit(void)
{
	unregister_sysctl_table(bacem_header);
#ifdef FIX_CPUFREQ
	if (cpufreq_fix_table) {
		cpufreq_frequency_table_put_attr(0);
		kfree(cpufreq_fix_table);
	}
#endif
}

module_init(bacem_tweak_init);
module_exit(bacem_tweak_exit);

MODULE_LICENSE("GPL");

