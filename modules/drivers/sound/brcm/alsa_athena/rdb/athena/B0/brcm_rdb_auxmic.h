/*******************************************************************************************
Copyright 2010 Broadcom Corporation.  All rights reserved.

Unless you and Broadcom execute a separate written software license agreement
governing use of this software, this software is licensed to you under the
terms of the GNU General Public License version 2, available at
http://www.gnu.org/copyleft/gpl.html (the "GPL").

Notwithstanding the above, under no circumstances may you combine this software
in any way with any other Broadcom software provided under a license other than
the GPL, without Broadcom's express prior written consent.
*******************************************************************************************/

#ifndef __BRCM_RDB_AUXMIC_H__
#define __BRCM_RDB_AUXMIC_H__

#define AUXMIC_PRB_CYC_OFFSET                                             0x00000000
#define AUXMIC_PRB_CYC_TYPE                                               UInt32
#define AUXMIC_PRB_CYC_RESERVED_MASK                                      0xFFFFFFF8
#define    AUXMIC_PRB_CYC_PROBE_SHIFT                                     0
#define    AUXMIC_PRB_CYC_PROBE_MASK                                      0x00000007
#define       AUXMIC_PRB_CYC_PROBE_CMD_4MS                                0x00000000
#define       AUXMIC_PRB_CYC_PROBE_CMD_8MS                                0x00000001
#define       AUXMIC_PRB_CYC_PROBE_CMD_16MS                               0x00000002
#define       AUXMIC_PRB_CYC_PROBE_CMD_32MS                               0x00000003
#define       AUXMIC_PRB_CYC_PROBE_CMD_64MS                               0x00000004
#define       AUXMIC_PRB_CYC_PROBE_CMD_128MS                              0x00000005
#define       AUXMIC_PRB_CYC_PROBE_CMD_256MS                              0x00000006
#define       AUXMIC_PRB_CYC_PROBE_CMD_512MS                              0x00000007

#define AUXMIC_MSR_DLY_OFFSET                                             0x00000004
#define AUXMIC_MSR_DLY_TYPE                                               UInt32
#define AUXMIC_MSR_DLY_RESERVED_MASK                                      0xFFFFFFF8
#define    AUXMIC_MSR_DLY_MSR_DLY_SHIFT                                   0
#define    AUXMIC_MSR_DLY_MSR_DLY_MASK                                    0x00000007
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_1MS                              0x00000000
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_2MS                              0x00000001
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_4MS                              0x00000002
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_8MS                              0x00000003
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_16MS                             0x00000004
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_32MS                             0x00000005
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_64MS                             0x00000006
#define       AUXMIC_MSR_DLY_MSR_DLY_CMD_128MS                            0x00000007

#define AUXMIC_MSR_INTVL_OFFSET                                           0x00000008
#define AUXMIC_MSR_INTVL_TYPE                                             UInt32
#define AUXMIC_MSR_INTVL_RESERVED_MASK                                    0xFFFFFFF8
#define    AUXMIC_MSR_INTVL_MSR_INTVL_SHIFT                               0
#define    AUXMIC_MSR_INTVL_MSR_INTVL_MASK                                0x00000007
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_2MS                          0x00000000
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_4MS                          0x00000001
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_8MS                          0x00000002
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_16MS                         0x00000003
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_32MS                         0x00000004
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_64MS                         0x00000005
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_128MS                        0x00000006
#define       AUXMIC_MSR_INTVL_MSR_INTVL_CMD_256MS                        0x00000007

#define AUXMIC_CMC_OFFSET                                                 0x0000000C
#define AUXMIC_CMC_TYPE                                                   UInt32
#define AUXMIC_CMC_RESERVED_MASK                                          0xFFFFFFFE
#define    AUXMIC_CMC_CONT_MSR_CTRL_SHIFT                                 0
#define    AUXMIC_CMC_CONT_MSR_CTRL_MASK                                  0x00000001
#define       AUXMIC_CMC_CONT_MSR_CTRL_CMD_PROB_CYC_INF                   0x00000001

#define AUXMIC_MIC_OFFSET                                                 0x00000010
#define AUXMIC_MIC_TYPE                                                   UInt32
#define AUXMIC_MIC_RESERVED_MASK                                          0xFFFFFFFE
#define    AUXMIC_MIC_MSR_INTVL_CTRL_SHIFT                                0
#define    AUXMIC_MIC_MSR_INTVL_CTRL_MASK                                 0x00000001
#define       AUXMIC_MIC_MSR_INTVL_CTRL_CMD_ONE_MEASUREMENT_INTVL         0x00000000

#define AUXMIC_AUXEN_OFFSET                                               0x00000014
#define AUXMIC_AUXEN_TYPE                                                 UInt32
#define AUXMIC_AUXEN_RESERVED_MASK                                        0xFFFFFFFE
#define    AUXMIC_AUXEN_MICAUX_EN_SHIFT                                   0
#define    AUXMIC_AUXEN_MICAUX_EN_MASK                                    0x00000001

#define AUXMIC_MICINTH_ADJ_OFFSET                                         0x00000018
#define AUXMIC_MICINTH_ADJ_TYPE                                           UInt32
#define AUXMIC_MICINTH_ADJ_RESERVED_MASK                                  0xFFFFFF80
#define    AUXMIC_MICINTH_ADJ_MICINTH_ADJ_SHIFT                           0
#define    AUXMIC_MICINTH_ADJ_MICINTH_ADJ_MASK                            0x0000007F

#define AUXMIC_MICINENTH_ADJ_OFFSET                                       0x0000001C
#define AUXMIC_MICINENTH_ADJ_TYPE                                         UInt32
#define AUXMIC_MICINENTH_ADJ_RESERVED_MASK                                0xFFFFFF80
#define    AUXMIC_MICINENTH_ADJ_MICINENTH_ADJ_SHIFT                       0
#define    AUXMIC_MICINENTH_ADJ_MICINENTH_ADJ_MASK                        0x0000007F

#define AUXMIC_MICONTH_ADJ_OFFSET                                         0x00000020
#define AUXMIC_MICONTH_ADJ_TYPE                                           UInt32
#define AUXMIC_MICONTH_ADJ_RESERVED_MASK                                  0xFFFFFF80
#define    AUXMIC_MICONTH_ADJ_MICONTH_ADJ_SHIFT                           0
#define    AUXMIC_MICONTH_ADJ_MICONTH_ADJ_MASK                            0x0000007F

#define AUXMIC_MICONENTH_ADJ_OFFSET                                       0x00000024
#define AUXMIC_MICONENTH_ADJ_TYPE                                         UInt32
#define AUXMIC_MICONENTH_ADJ_RESERVED_MASK                                0xFFFFFF80
#define    AUXMIC_MICONENTH_ADJ_MICONENTH_ADJ_SHIFT                       0
#define    AUXMIC_MICONENTH_ADJ_MICONENTH_ADJ_MASK                        0x0000007F

#define AUXMIC_F_PWRDWN_OFFSET                                            0x00000028
#define AUXMIC_F_PWRDWN_TYPE                                              UInt32
#define AUXMIC_F_PWRDWN_RESERVED_MASK                                     0xFFFFFFFE
#define    AUXMIC_F_PWRDWN_FORCE_PWR_DWN_SHIFT                            0
#define    AUXMIC_F_PWRDWN_FORCE_PWR_DWN_MASK                             0x00000001

#define AUXMIC_RESERVED_OFFSET                                            0x0000002C
#define AUXMIC_RESERVED_TYPE                                              UInt32
#define AUXMIC_RESERVED_RESERVED_MASK                                     0xFFFFFFFF

#define AUXMIC_MICINTH_DEF_OFFSET                                         0x00000030
#define AUXMIC_MICINTH_DEF_TYPE                                           UInt32
#define AUXMIC_MICINTH_DEF_RESERVED_MASK                                  0xFFFFFF80
#define    AUXMIC_MICINTH_DEF_MICINTH_DEF_SHIFT                           0
#define    AUXMIC_MICINTH_DEF_MICINTH_DEF_MASK                            0x0000007F

#define AUXMIC_MICINENTH_DEF_OFFSET                                       0x00000034
#define AUXMIC_MICINENTH_DEF_TYPE                                         UInt32
#define AUXMIC_MICINENTH_DEF_RESERVED_MASK                                0xFFFFFF80
#define    AUXMIC_MICINENTH_DEF_MICINENTH_DEF_SHIFT                       0
#define    AUXMIC_MICINENTH_DEF_MICINENTH_DEF_MASK                        0x0000007F

#define AUXMIC_MICONTH_DEF_OFFSET                                         0x00000038
#define AUXMIC_MICONTH_DEF_TYPE                                           UInt32
#define AUXMIC_MICONTH_DEF_RESERVED_MASK                                  0xFFFFFF80
#define    AUXMIC_MICONTH_DEF_MICONTH_DEF_SHIFT                           0
#define    AUXMIC_MICONTH_DEF_MICONTH_DEF_MASK                            0x0000007F

#define AUXMIC_MICONENTH_DEF_OFFSET                                       0x0000003C
#define AUXMIC_MICONENTH_DEF_TYPE                                         UInt32
#define AUXMIC_MICONENTH_DEF_RESERVED_MASK                                0xFFFFFF80
#define    AUXMIC_MICONENTH_DEF_MICONENTH_DEF_SHIFT                       0
#define    AUXMIC_MICONENTH_DEF_MICONENTH_DEF_MASK                        0x0000007F

#define AUXMIC_MICINTH_OFFSET                                             0x00000040
#define AUXMIC_MICINTH_TYPE                                               UInt32
#define AUXMIC_MICINTH_RESERVED_MASK                                      0xFFFFFF80
#define    AUXMIC_MICINTH_MICINTH_SHIFT                                   0
#define    AUXMIC_MICINTH_MICINTH_MASK                                    0x0000007F

#define AUXMIC_MICONTH_OFFSET                                             0x00000044
#define AUXMIC_MICONTH_TYPE                                               UInt32
#define AUXMIC_MICONTH_RESERVED_MASK                                      0xFFFFFF80
#define    AUXMIC_MICONTH_MICONTH_SHIFT                                   0
#define    AUXMIC_MICONTH_MICONTH_MASK                                    0x0000007F

#endif /* __BRCM_RDB_AUXMIC_H__ */


