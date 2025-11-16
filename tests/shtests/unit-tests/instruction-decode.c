#include <stdint.h>
#include <pcut/pcut.h>

#include "../../../src/device/cpu/superh_sh2e/decode.h"
#include "../../../src/device/cpu/superh_sh2e/exec.h"
#include "../../../src/device/cpu/superh_sh2e/insn.h"

#define R0_REG 0b0000
#define R1_REG 0b0001
#define FR0_REG 0b0000
#define FR1_REG 0b0001

PCUT_INIT

PCUT_TEST_SUITE(instruction_decoding);

/****************************************************************************
 * SH-2E `0-format` decoding
 ****************************************************************************/

PCUT_TEST(clrt_decode)
{
    sh2e_insn_z_t insn_clrt = {
        .ic = 0b0000000000001000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_clrt, sh2e_insn_decode((sh2e_insn_t) insn_clrt)->exec);
}

PCUT_TEST(nop_decode)
{
    sh2e_insn_z_t insn_nop = {
        .ic = 0b0000000000001001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_nop, sh2e_insn_decode((sh2e_insn_t) insn_nop)->exec);
}

PCUT_TEST(rts_decode)
{
    sh2e_insn_z_t insn_rts = {
        .ic = 0b0000000000001011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_rts, sh2e_insn_decode((sh2e_insn_t) insn_rts)->exec);
}

PCUT_TEST(sett_decode)
{
    sh2e_insn_z_t insn_sett = {
        .ic = 0b0000000000011000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sett, sh2e_insn_decode((sh2e_insn_t) insn_sett)->exec);
}

PCUT_TEST(div0u_decode)
{
    sh2e_insn_z_t insn_div0u = {
        .ic = 0b0000000000011001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_div0u, sh2e_insn_decode((sh2e_insn_t) insn_div0u)->exec);
}

PCUT_TEST(sleep_decode)
{
    sh2e_insn_z_t insn_sleep = {
        .ic = 0b0000000000011011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sleep, sh2e_insn_decode((sh2e_insn_t) insn_sleep)->exec);
}

PCUT_TEST(clrmac_decode)
{
    sh2e_insn_z_t insn_clrmac = {
        .ic = 0b0000000000101000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_clrmac, sh2e_insn_decode((sh2e_insn_t) insn_clrmac)->exec);
}

PCUT_TEST(rte_decode)
{
    sh2e_insn_z_t insn_rte = {
        .ic = 0b0000000000101011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_rte, sh2e_insn_decode((sh2e_insn_t) insn_rte)->exec);
}

/****************************************************************************
 * SH-2E `n-format` decoding
 ****************************************************************************/

PCUT_TEST(cmppl_decode)
{
    sh2e_insn_n_t insn_cmppl = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00010101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmppl, sh2e_insn_decode((sh2e_insn_t) insn_cmppl)->exec);
}

PCUT_TEST(cmppz_decode)
{
    sh2e_insn_n_t insn_cmppz = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00010001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmppz, sh2e_insn_decode((sh2e_insn_t) insn_cmppz)->exec);
}

PCUT_TEST(dt_decode)
{
    sh2e_insn_n_t insn_dt = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00010000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_dt, sh2e_insn_decode((sh2e_insn_t) insn_dt)->exec);
}

PCUT_TEST(movt_decode)
{
    sh2e_insn_n_t insn_movt = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b00101001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movt, sh2e_insn_decode((sh2e_insn_t) insn_movt)->exec);
}

PCUT_TEST(rotl_decode)
{
    sh2e_insn_n_t insn_rotl = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00000100
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_rotl, sh2e_insn_decode((sh2e_insn_t) insn_rotl)->exec);
}

PCUT_TEST(rotr_decode)
{
    sh2e_insn_n_t insn_rotr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00000101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_rotr, sh2e_insn_decode((sh2e_insn_t) insn_rotr)->exec);
}

PCUT_TEST(rotcl_decode)
{
    sh2e_insn_n_t insn_rotcl = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00100100
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_rotcl, sh2e_insn_decode((sh2e_insn_t) insn_rotcl)->exec);
}

PCUT_TEST(rotcr_decode)
{
    sh2e_insn_n_t insn_rotcr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00100101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_rotcr, sh2e_insn_decode((sh2e_insn_t) insn_rotcr)->exec);
}

PCUT_TEST(shal_decode)
{
    sh2e_insn_n_t insn_shal = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00100000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shal, sh2e_insn_decode((sh2e_insn_t) insn_shal)->exec);
}

PCUT_TEST(shar_decode)
{
    sh2e_insn_n_t insn_shar = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00100001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shar, sh2e_insn_decode((sh2e_insn_t) insn_shar)->exec);
}

PCUT_TEST(shll_decode)
{
    sh2e_insn_n_t insn_shll = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00000000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shll, sh2e_insn_decode((sh2e_insn_t) insn_shll)->exec);
}

PCUT_TEST(shlr_decode)
{
    sh2e_insn_n_t insn_shlr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00000001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shlr, sh2e_insn_decode((sh2e_insn_t) insn_shlr)->exec);
}

PCUT_TEST(shll2_decode)
{
    sh2e_insn_n_t insn_shll2 = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00001000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shll2, sh2e_insn_decode((sh2e_insn_t) insn_shll2)->exec);
}

PCUT_TEST(shll8_decode)
{
    sh2e_insn_n_t insn_shll8 = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00011000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shll8, sh2e_insn_decode((sh2e_insn_t) insn_shll8)->exec);
}

PCUT_TEST(shll16_decode)
{
    sh2e_insn_n_t insn_shll16 = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00101000
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shll16, sh2e_insn_decode((sh2e_insn_t) insn_shll16)->exec);
}

PCUT_TEST(shlr2_decode)
{
    sh2e_insn_n_t insn_shlr2 = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00001001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shlr2, sh2e_insn_decode((sh2e_insn_t) insn_shlr2)->exec);
}

PCUT_TEST(shlr8_decode)
{
    sh2e_insn_n_t insn_shlr8 = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00011001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shlr8, sh2e_insn_decode((sh2e_insn_t) insn_shlr8)->exec);
}

PCUT_TEST(shlr16_decode)
{
    sh2e_insn_n_t insn_shlr16 = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00101001
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_shlr16, sh2e_insn_decode((sh2e_insn_t) insn_shlr16)->exec);
}

PCUT_TEST(stc_sr_decode)
{
    sh2e_insn_n_t insn_stc_sr = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b00000010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stc_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stc_sr)->exec);
}

PCUT_TEST(stc_gbr_decode)
{
    sh2e_insn_n_t insn_stc_gbr = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b00010010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stc_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stc_gbr)->exec);
}

PCUT_TEST(stc_vbr_decode)
{
    sh2e_insn_n_t insn_stc_vbr = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b00110010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stc_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stc_vbr)->exec);
}

PCUT_TEST(sts_fpul_decode)
{
    sh2e_insn_n_t insn_sts_fpul = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b01011010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sts_fpu, sh2e_insn_decode((sh2e_insn_t) insn_sts_fpul)->exec);
}

PCUT_TEST(sts_fpscr_decode)
{
    sh2e_insn_n_t insn_sts_fpscr = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b01101010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sts_fpu, sh2e_insn_decode((sh2e_insn_t) insn_sts_fpscr)->exec);
}

PCUT_TEST(sts_mach_decode)
{
    sh2e_insn_n_t insn_sts_mach = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b00001010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sts_cpu, sh2e_insn_decode((sh2e_insn_t) insn_sts_mach)->exec);
}

PCUT_TEST(sts_macl_decode)
{
    sh2e_insn_n_t insn_sts_macl = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b00011010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sts_cpu, sh2e_insn_decode((sh2e_insn_t) insn_sts_macl)->exec);
}

PCUT_TEST(sts_pr_decode)
{
    sh2e_insn_n_t insn_sts_pr = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .ic_l = 0b00101010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sts_cpu, sh2e_insn_decode((sh2e_insn_t) insn_sts_pr)->exec);
}

PCUT_TEST(tas_decode)
{
    sh2e_insn_n_t insn_tas = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00011011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_tas, sh2e_insn_decode((sh2e_insn_t) insn_tas)->exec);
}

PCUT_TEST(stcm_sr_decode)
{
    sh2e_insn_n_t insn_stcm_sr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00000011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stcm_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stcm_sr)->exec);
}

PCUT_TEST(stcm_gbr_decode)
{
    sh2e_insn_n_t insn_stcm_gbr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00010011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stcm_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stcm_gbr)->exec);
}

PCUT_TEST(stcm_vbr_decode)
{
    sh2e_insn_n_t insn_stcm_vbr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00100011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stcm_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stcm_vbr)->exec);
}

PCUT_TEST(stsm_fpul_decode)
{
    sh2e_insn_n_t insn_stsm_fpul = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b01010010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stsm_fpu, sh2e_insn_decode((sh2e_insn_t) insn_stsm_fpul)->exec);
}

PCUT_TEST(stsm_fpscr_decode)
{
    sh2e_insn_n_t insn_stsm_fpscr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b01100010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stsm_fpu, sh2e_insn_decode((sh2e_insn_t) insn_stsm_fpscr)->exec);
}

PCUT_TEST(stsm_mach_decode)
{
    sh2e_insn_n_t insn_stsm_mach = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00000010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stsm_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stsm_mach)->exec);
}

PCUT_TEST(stsm_macl_decode)
{
    sh2e_insn_n_t insn_stsm_macl = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00010010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stsm_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stsm_macl)->exec);
}

PCUT_TEST(stsm_pr_decode)
{
    sh2e_insn_n_t insn_stsm_pr = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .ic_l = 0b00100010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_stsm_cpu, sh2e_insn_decode((sh2e_insn_t) insn_stsm_pr)->exec);
}

// n-format FPU

PCUT_TEST(fsts_decode)
{
    sh2e_insn_n_t insn_fsts = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .ic_l = 0b00001101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fsts, sh2e_insn_decode((sh2e_insn_t) insn_fsts)->exec);
}

PCUT_TEST(ffloat_decode)
{
    sh2e_insn_n_t insn_ffloat = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .ic_l = 0b00101101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_float, sh2e_insn_decode((sh2e_insn_t) insn_ffloat)->exec);
}

PCUT_TEST(fneg_decode)
{
    sh2e_insn_n_t insn_fneg = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .ic_l = 0b01001101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fneg, sh2e_insn_decode((sh2e_insn_t) insn_fneg)->exec);
}

PCUT_TEST(fabs_decode)
{
    sh2e_insn_n_t insn_fabs = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .ic_l = 0b01011101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fabs, sh2e_insn_decode((sh2e_insn_t) insn_fabs)->exec);
}

PCUT_TEST(fldi0_decode)
{
    sh2e_insn_n_t insn_fldi0 = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .ic_l = 0b10001101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fldi0, sh2e_insn_decode((sh2e_insn_t) insn_fldi0)->exec);
}

PCUT_TEST(fldi1_decode)
{
    sh2e_insn_n_t insn_fldi1 = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .ic_l = 0b10011101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fldi1, sh2e_insn_decode((sh2e_insn_t) insn_fldi1)->exec);
}

/****************************************************************************
 * SH-2E `m-format` decoding
 ****************************************************************************/

PCUT_TEST(ldc_sr_decode)
{
    sh2e_insn_m_t insn_ldc_sr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00001110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldc_sr, sh2e_insn_decode((sh2e_insn_t) insn_ldc_sr)->exec);
}

PCUT_TEST(ldc_gbr_decode)
{
    sh2e_insn_m_t insn_ldc_gbr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00011110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldc_gbr, sh2e_insn_decode((sh2e_insn_t) insn_ldc_gbr)->exec);
}

PCUT_TEST(ldc_vbr_decode)
{
    sh2e_insn_m_t insn_ldc_vbr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00101110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldc_vbr, sh2e_insn_decode((sh2e_insn_t) insn_ldc_vbr)->exec);
}

PCUT_TEST(lds_fpul_decode)
{
    sh2e_insn_m_t insn_lds_fpul = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b01011010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_lds_fpul, sh2e_insn_decode((sh2e_insn_t) insn_lds_fpul)->exec);
}

PCUT_TEST(lds_fpscr_decode)
{
    sh2e_insn_m_t insn_lds_fpscr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b01101010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_lds_fpscr, sh2e_insn_decode((sh2e_insn_t) insn_lds_fpscr)->exec);
}

PCUT_TEST(lds_mach_decode)
{
    sh2e_insn_m_t insn_lds_mach = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00001010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_lds_mach, sh2e_insn_decode((sh2e_insn_t) insn_lds_mach)->exec);
}

PCUT_TEST(lds_macl_decode)
{
    sh2e_insn_m_t insn_lds_macl = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00011010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_lds_macl, sh2e_insn_decode((sh2e_insn_t) insn_lds_macl)->exec);
}

PCUT_TEST(lds_pr_decode)
{
    sh2e_insn_m_t insn_lds_pr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00101010
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_lds_pr, sh2e_insn_decode((sh2e_insn_t) insn_lds_pr)->exec);
}

PCUT_TEST(jmp_decode)
{
    sh2e_insn_m_t insn_jmp = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00101011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_jmp, sh2e_insn_decode((sh2e_insn_t) insn_jmp)->exec);
}

PCUT_TEST(jsr_decode)
{
    sh2e_insn_m_t insn_jsr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00001011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_jsr, sh2e_insn_decode((sh2e_insn_t) insn_jsr)->exec);
}

PCUT_TEST(ldcl_sr_decode)
{
    sh2e_insn_m_t insn_ldcl_sr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00000111
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldcl_sr, sh2e_insn_decode((sh2e_insn_t) insn_ldcl_sr)->exec);
}

PCUT_TEST(ldcl_gbr_decode)
{
    sh2e_insn_m_t insn_ldcl_gbr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00010111
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldcl_gbr, sh2e_insn_decode((sh2e_insn_t) insn_ldcl_gbr)->exec);
}

PCUT_TEST(ldcl_vbr_decode)
{
    sh2e_insn_m_t insn_ldcl_vbr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00100111
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldcl_vbr, sh2e_insn_decode((sh2e_insn_t) insn_ldcl_vbr)->exec);
}

PCUT_TEST(ldsl_fpul_decode)
{
    sh2e_insn_m_t insn_ldsl_fpul = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b01010110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldsl_fpul, sh2e_insn_decode((sh2e_insn_t) insn_ldsl_fpul)->exec);
}

PCUT_TEST(ldsl_fpscr_decode)
{
    sh2e_insn_m_t insn_ldsl_fpscr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b01100110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldsl_fpscr, sh2e_insn_decode((sh2e_insn_t) insn_ldsl_fpscr)->exec);
}

PCUT_TEST(ldsl_mach_decode)
{
    sh2e_insn_m_t insn_ldsl_mach = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00000110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldsl_mach, sh2e_insn_decode((sh2e_insn_t) insn_ldsl_mach)->exec);
}

PCUT_TEST(ldsl_macl_decode)
{
    sh2e_insn_m_t insn_ldsl_macl = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00010110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldsl_macl, sh2e_insn_decode((sh2e_insn_t) insn_ldsl_macl)->exec);
}

PCUT_TEST(ldsl_pr_decode)
{
    sh2e_insn_m_t insn_ldsl_pr = {
        .ic_h = 0b0100,
        .rm = R0_REG,
        .ic_l = 0b00100110
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ldsl_pr, sh2e_insn_decode((sh2e_insn_t) insn_ldsl_pr)->exec);
}

PCUT_TEST(braf_decode)
{
    sh2e_insn_m_t insn_braf = {
        .ic_h = 0b0000,
        .rm = R0_REG,
        .ic_l = 0b00100011
    };
    sh2e_insn_exec_fn_t ret_value = sh2e_insn_decode((sh2e_insn_t) insn_braf);

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_braf, sh2e_insn_decode((sh2e_insn_t) insn_braf)->exec);
}

PCUT_TEST(bsrf_decode)
{
    sh2e_insn_m_t insn_bsrf = {
        .ic_h = 0b0000,
        .rm = R0_REG,
        .ic_l = 0b00000011
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_bsrf, sh2e_insn_decode((sh2e_insn_t) insn_bsrf)->exec);
}

PCUT_TEST(flds_decode)
{
    sh2e_insn_m_t insn_flds = {
        .ic_h = 0b1111,
        .rm = FR0_REG,
        .ic_l = 0b00011101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_flds, sh2e_insn_decode((sh2e_insn_t) insn_flds)->exec);
}

PCUT_TEST(ftrc_decode)
{
    sh2e_insn_m_t insn_ftrc = {
        .ic_h = 0b1111,
        .rm = FR0_REG,
        .ic_l = 0b00111101
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ftrc, sh2e_insn_decode((sh2e_insn_t) insn_ftrc)->exec);
}

/****************************************************************************
 * SH-2E `nm-format` decoding
 ****************************************************************************/

PCUT_TEST(add_decode)
{
    sh2e_insn_nm_t insn_add = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_add, sh2e_insn_decode((sh2e_insn_t) insn_add)->exec);
}

PCUT_TEST(addc_decode)
{
    sh2e_insn_nm_t insn_addc = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_addc, sh2e_insn_decode((sh2e_insn_t) insn_addc)->exec);
}

PCUT_TEST(addv_decode)
{
    sh2e_insn_nm_t insn_addv = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_addv, sh2e_insn_decode((sh2e_insn_t) insn_addv)->exec);
}

PCUT_TEST(and_decode)
{
    sh2e_insn_nm_t insn_and = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1001,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_and, sh2e_insn_decode((sh2e_insn_t) insn_and)->exec);
}

PCUT_TEST(cmpeq_decode)
{
    sh2e_insn_nm_t insn_cmpeq = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmpeq, sh2e_insn_decode((sh2e_insn_t) insn_cmpeq)->exec);
}

PCUT_TEST(cmphs_decode)
{
    sh2e_insn_nm_t insn_cmphs = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmphs, sh2e_insn_decode((sh2e_insn_t) insn_cmphs)->exec);
}

PCUT_TEST(cmpge_decode)
{
    sh2e_insn_nm_t insn_cmpge = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0011,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmpge, sh2e_insn_decode((sh2e_insn_t) insn_cmpge)->exec);
}

PCUT_TEST(cmphi_decode)
{
    sh2e_insn_nm_t insn_cmphi = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmphi, sh2e_insn_decode((sh2e_insn_t) insn_cmphi)->exec);
}

PCUT_TEST(cmpgt_decode)
{
    sh2e_insn_nm_t insn_cmpgt = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmpgt, sh2e_insn_decode((sh2e_insn_t) insn_cmpgt)->exec);
}

PCUT_TEST(cmpstr_decode)
{
    sh2e_insn_nm_t insn_cmpstr = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmpstr, sh2e_insn_decode((sh2e_insn_t) insn_cmpstr)->exec);
}

PCUT_TEST(div1_decode)
{
    sh2e_insn_nm_t insn_div1 = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_div1, sh2e_insn_decode((sh2e_insn_t) insn_div1)->exec);
}

PCUT_TEST(div0s_decode)
{
    sh2e_insn_nm_t insn_div0s = {
        .ic_h = 0b0010,
        .rn = R1_REG,
        .rm = R0_REG,
        .ic_l = 0b0111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_div0s, sh2e_insn_decode((sh2e_insn_t) insn_div0s)->exec);
}

PCUT_TEST(dmulsl_decode)
{
    sh2e_insn_nm_t insn_dmulsl = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_dmulsl, sh2e_insn_decode((sh2e_insn_t) insn_dmulsl)->exec);
}

PCUT_TEST(dmulul_decode)
{
    sh2e_insn_nm_t insn_dmulul = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_dmulul, sh2e_insn_decode((sh2e_insn_t) insn_dmulul)->exec);
}

PCUT_TEST(extsb_decode)
{
    sh2e_insn_nm_t insn_extsb = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_extsb, sh2e_insn_decode((sh2e_insn_t) insn_extsb)->exec);
}

PCUT_TEST(extsw_decode)
{
    sh2e_insn_nm_t insn_extsw = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_extsw, sh2e_insn_decode((sh2e_insn_t) insn_extsw)->exec);
}

PCUT_TEST(extub_decode)
{
    sh2e_insn_nm_t insn_extub = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_extub, sh2e_insn_decode((sh2e_insn_t) insn_extub)->exec);
}

PCUT_TEST(extuw_decode)
{
    sh2e_insn_nm_t insn_extuw = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_extuw, sh2e_insn_decode((sh2e_insn_t) insn_extuw)->exec);
}

PCUT_TEST(mov_decode)
{
    sh2e_insn_nm_t insn_mov = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0011,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_mov, sh2e_insn_decode((sh2e_insn_t) insn_mov)->exec);
}

PCUT_TEST(mull_decode)
{
    sh2e_insn_nm_t insn_mull = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_mull, sh2e_insn_decode((sh2e_insn_t) insn_mull)->exec);
}

PCUT_TEST(mulsw_decode)
{
    sh2e_insn_nm_t insn_mulsw = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_mulsw, sh2e_insn_decode((sh2e_insn_t) insn_mulsw)->exec);
}

PCUT_TEST(muluw_decode)
{
    sh2e_insn_nm_t insn_muluw = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_muluw, sh2e_insn_decode((sh2e_insn_t) insn_muluw)->exec);
}

PCUT_TEST(neg_decode)
{
    sh2e_insn_nm_t insn_neg = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1011,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_neg, sh2e_insn_decode((sh2e_insn_t) insn_neg)->exec);
}

PCUT_TEST(negc_decode)
{
    sh2e_insn_nm_t insn_negc = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_negc, sh2e_insn_decode((sh2e_insn_t) insn_negc)->exec);
}

PCUT_TEST(not_decode)
{
    sh2e_insn_nm_t insn_not = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_not, sh2e_insn_decode((sh2e_insn_t) insn_not)->exec);
}

PCUT_TEST(or_decode)
{
    sh2e_insn_nm_t insn_or = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1011,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_or, sh2e_insn_decode((sh2e_insn_t) insn_or)->exec);
}

PCUT_TEST(sub_decode)
{
    sh2e_insn_nm_t insn_sub = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_sub, sh2e_insn_decode((sh2e_insn_t) insn_sub)->exec);
}

PCUT_TEST(subc_decode)
{
    sh2e_insn_nm_t insn_subc = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_subc, sh2e_insn_decode((sh2e_insn_t) insn_subc)->exec);
}

PCUT_TEST(subv_decode)
{
    sh2e_insn_nm_t insn_subv = {
        .ic_h = 0b0011,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1011,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_subv, sh2e_insn_decode((sh2e_insn_t) insn_subv)->exec);
}

PCUT_TEST(swapb_decode)
{
    sh2e_insn_nm_t insn_swapb = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_swapb, sh2e_insn_decode((sh2e_insn_t) insn_swapb)->exec);
}

PCUT_TEST(swapw_decode)
{
    sh2e_insn_nm_t insn_swapw = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1001,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_swapw, sh2e_insn_decode((sh2e_insn_t) insn_swapw)->exec);
}

PCUT_TEST(tst_decode)
{
    sh2e_insn_nm_t insn_tst = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_tst, sh2e_insn_decode((sh2e_insn_t) insn_tst)->exec);
}

PCUT_TEST(xor_decode)
{
    sh2e_insn_nm_t insn_xor = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_xor, sh2e_insn_decode((sh2e_insn_t) insn_xor)->exec);
}

PCUT_TEST(xtrct_decode)
{
    sh2e_insn_nm_t insn_xtrct = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_xtrct, sh2e_insn_decode((sh2e_insn_t) insn_xtrct)->exec);
}

PCUT_TEST(movbs_decode)
{
    sh2e_insn_nm_t insn_movbs = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbs, sh2e_insn_decode((sh2e_insn_t) insn_movbs)->exec);
}

PCUT_TEST(movws_decode)
{
    sh2e_insn_nm_t insn_movws = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0001,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movws, sh2e_insn_decode((sh2e_insn_t) insn_movws)->exec);
}

PCUT_TEST(movls_decode)
{
    sh2e_insn_nm_t insn_movls = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movls, sh2e_insn_decode((sh2e_insn_t) insn_movls)->exec);
}

PCUT_TEST(movbl_decode)
{
    sh2e_insn_nm_t insn_movbl = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbl, sh2e_insn_decode((sh2e_insn_t) insn_movbl)->exec);
}

PCUT_TEST(movwl_decode)
{
    sh2e_insn_nm_t insn_movwl = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0001,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwl, sh2e_insn_decode((sh2e_insn_t) insn_movwl)->exec);
}

PCUT_TEST(movll_decode)
{
    sh2e_insn_nm_t insn_movll = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movll, sh2e_insn_decode((sh2e_insn_t) insn_movll)->exec);
}

PCUT_TEST(macl_decode)
{
    sh2e_insn_nm_t insn_macl = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_macl, sh2e_insn_decode((sh2e_insn_t) insn_macl)->exec);
}

PCUT_TEST(macw_decode)
{
    sh2e_insn_nm_t insn_macw = {
        .ic_h = 0b0100,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_macw, sh2e_insn_decode((sh2e_insn_t) insn_macw)->exec);
}

PCUT_TEST(movbp_decode)
{
    sh2e_insn_nm_t insn_movbp = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbp, sh2e_insn_decode((sh2e_insn_t) insn_movbp)->exec);
}

PCUT_TEST(movwp_decode)
{
    sh2e_insn_nm_t insn_movwp = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwp, sh2e_insn_decode((sh2e_insn_t) insn_movwp)->exec);
}

PCUT_TEST(movlp_decode)
{
    sh2e_insn_nm_t insn_movlp = {
        .ic_h = 0b0110,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movlp, sh2e_insn_decode((sh2e_insn_t) insn_movlp)->exec);
}

PCUT_TEST(movbm_decode)
{
    sh2e_insn_nm_t insn_movbm = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbm, sh2e_insn_decode((sh2e_insn_t) insn_movbm)->exec);
}

PCUT_TEST(movwm_decode)
{
    sh2e_insn_nm_t insn_movwm = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwm, sh2e_insn_decode((sh2e_insn_t) insn_movwm)->exec);
}

PCUT_TEST(movlm_decode)
{
    sh2e_insn_nm_t insn_movlm = {
        .ic_h = 0b0010,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movlm, sh2e_insn_decode((sh2e_insn_t) insn_movlm)->exec);
}

PCUT_TEST(movbs0_decode)
{
    sh2e_insn_nm_t insn_movbs0 = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbs0, sh2e_insn_decode((sh2e_insn_t) insn_movbs0)->exec);
}

PCUT_TEST(movws0_decode)
{
    sh2e_insn_nm_t insn_movws0 = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movws0, sh2e_insn_decode((sh2e_insn_t) insn_movws0)->exec);
}

PCUT_TEST(movls0_decode)
{
    sh2e_insn_nm_t insn_movls0 = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b0110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movls0, sh2e_insn_decode((sh2e_insn_t) insn_movls0)->exec);
}

PCUT_TEST(movbl0_decode)
{
    sh2e_insn_nm_t insn_movbl0 = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbl0, sh2e_insn_decode((sh2e_insn_t) insn_movbl0)->exec);
}

PCUT_TEST(movwl0_decode)
{
    sh2e_insn_nm_t insn_movwl0 = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwl0, sh2e_insn_decode((sh2e_insn_t) insn_movwl0)->exec);
}

PCUT_TEST(movll0_decode)
{
    sh2e_insn_nm_t insn_movll0 = {
        .ic_h = 0b0000,
        .rn = R0_REG,
        .rm = R1_REG,
        .ic_l = 0b1110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movll0, sh2e_insn_decode((sh2e_insn_t) insn_movll0)->exec);
}

PCUT_TEST(fadd_decode)
{
    sh2e_insn_nm_t insn_fadd = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b0000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fadd, sh2e_insn_decode((sh2e_insn_t) insn_fadd)->exec);
}

PCUT_TEST(fcmpeq_decode)
{
    sh2e_insn_nm_t insn_fcmpeq = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b0100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fcmpeq, sh2e_insn_decode((sh2e_insn_t) insn_fcmpeq)->exec);
}

PCUT_TEST(fcmpgt_decode)
{
    sh2e_insn_nm_t insn_fcmpgt = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b0101,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fcmpgt, sh2e_insn_decode((sh2e_insn_t) insn_fcmpgt)->exec);
}

PCUT_TEST(fdiv_decode)
{
    sh2e_insn_nm_t insn_fdiv = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b0011,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fdiv, sh2e_insn_decode((sh2e_insn_t) insn_fdiv)->exec);
}

PCUT_TEST(fmac_decode)
{
    sh2e_insn_nm_t insn_fmac = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b1110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmac, sh2e_insn_decode((sh2e_insn_t) insn_fmac)->exec);
}

PCUT_TEST(fmov_decode)
{
    sh2e_insn_nm_t insn_fmov = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b1100,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmov, sh2e_insn_decode((sh2e_insn_t) insn_fmov)->exec);
}

PCUT_TEST(fmovli_decode)
{
    sh2e_insn_nm_t insn_fmovli = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = R0_REG,
        .ic_l = 0b0110,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmovli, sh2e_insn_decode((sh2e_insn_t) insn_fmovli)->exec);
}

PCUT_TEST(fmovlr_decode)
{
    sh2e_insn_nm_t insn_fmovlr = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = R0_REG,
        .ic_l = 0b1001,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmovlr, sh2e_insn_decode((sh2e_insn_t) insn_fmovlr)->exec);
}

PCUT_TEST(fmovl_decode)
{
    sh2e_insn_nm_t insn_fmovl = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = R0_REG,
        .ic_l = 0b1000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmovl, sh2e_insn_decode((sh2e_insn_t) insn_fmovl)->exec);
}

PCUT_TEST(fmovsi_decode)
{
    sh2e_insn_nm_t insn_fmovsi = {
        .ic_h = 0b1111,
        .rn = R0_REG,
        .rm = FR0_REG,
        .ic_l = 0b0111,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmovsi, sh2e_insn_decode((sh2e_insn_t) insn_fmovsi)->exec);
}

PCUT_TEST(fmovss_decode)
{
    sh2e_insn_nm_t insn_fmovss = {
        .ic_h = 0b1111,
        .rn = R0_REG,
        .rm = FR0_REG,
        .ic_l = 0b1011,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmovss, sh2e_insn_decode((sh2e_insn_t) insn_fmovss)->exec);
}

PCUT_TEST(fmovs_decode)
{
    sh2e_insn_nm_t insn_fmovs = {
        .ic_h = 0b1111,
        .rn = R0_REG,
        .rm = FR0_REG,
        .ic_l = 0b1010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmovs, sh2e_insn_decode((sh2e_insn_t) insn_fmovs)->exec);
}

PCUT_TEST(fmul_decode)
{
    sh2e_insn_nm_t insn_fmul = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b0010,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fmul, sh2e_insn_decode((sh2e_insn_t) insn_fmul)->exec);
}

PCUT_TEST(fsub_decode)
{
    sh2e_insn_nm_t insn_fsub = {
        .ic_h = 0b1111,
        .rn = FR0_REG,
        .rm = FR1_REG,
        .ic_l = 0b0001,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_fsub, sh2e_insn_decode((sh2e_insn_t) insn_fsub)->exec);
}

/****************************************************************************
 * SH-2E `md-format` decoding
 ****************************************************************************/

PCUT_TEST(movbl4_decode)
{
    sh2e_insn_md_t insn_movbl4 = {
        .ic = 0b10000100,
        .rm = R0_REG,
        .d4 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbl4, sh2e_insn_decode((sh2e_insn_t) insn_movbl4)->exec);
}

PCUT_TEST(movwl4_decode)
{
    sh2e_insn_md_t insn_movwl4 = {
        .ic = 0b10000101,
        .rm = R0_REG,
        .d4 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwl4, sh2e_insn_decode((sh2e_insn_t) insn_movwl4)->exec);
}

/****************************************************************************
 * SH-2E `nd4-format` decoding
 ****************************************************************************/

PCUT_TEST(movbs4_decode)
{
    sh2e_insn_nd4_t insn_movbs4 = {
        .ic = 0b10000000,
        .rn = R0_REG,
        .d4 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbs4, sh2e_insn_decode((sh2e_insn_t) insn_movbs4)->exec);
}

PCUT_TEST(movws4_decode)
{
    sh2e_insn_nd4_t insn_movws4 = {
        .ic = 0b10000001,
        .rn = R0_REG,
        .d4 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movws4, sh2e_insn_decode((sh2e_insn_t) insn_movws4)->exec);
}

/****************************************************************************
 * SH-2E `nmd-format` decoding
 ****************************************************************************/

PCUT_TEST(movls4_decode)
{
    sh2e_insn_nmd_t insn_movls4 = {
        .ic = 0b0001,
        .rn = R0_REG,
        .d4 = 0x0,
        .rm = R1_REG,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movls4, sh2e_insn_decode((sh2e_insn_t) insn_movls4)->exec);
}

PCUT_TEST(movll4_decode)
{
    sh2e_insn_nmd_t insn_movll4 = {
        .ic = 0b0101,
        .rn = R0_REG,
        .d4 = 0x0,
        .rm = R1_REG,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movll4, sh2e_insn_decode((sh2e_insn_t) insn_movll4)->exec);
}

/****************************************************************************
 * SH-2E `d-format` decoding
 ****************************************************************************/

PCUT_TEST(movblg_decode)
{
    sh2e_insn_d_t insn_movblg = {
        .ic = 0b11000000,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movblg, sh2e_insn_decode((sh2e_insn_t) insn_movblg)->exec);
}

PCUT_TEST(movwlg_decode)
{
    sh2e_insn_d_t insn_movwlg = {
        .ic = 0b11000001,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwlg, sh2e_insn_decode((sh2e_insn_t) insn_movwlg)->exec);
}

PCUT_TEST(movllg_decode)
{
    sh2e_insn_d_t insn_movllg = {
        .ic = 0b11000010,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movllg, sh2e_insn_decode((sh2e_insn_t) insn_movllg)->exec);
}

PCUT_TEST(movbsg_decode)
{
    sh2e_insn_d_t insn_movbsg = {
        .ic = 0b11000100,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movbsg, sh2e_insn_decode((sh2e_insn_t) insn_movbsg)->exec);
}

PCUT_TEST(movwsg_decode)
{
    sh2e_insn_d_t insn_movwsg = {
        .ic = 0b11000101,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwsg, sh2e_insn_decode((sh2e_insn_t) insn_movwsg)->exec);
}

PCUT_TEST(movlsg_decode)
{
    sh2e_insn_d_t insn_movlsg = {
        .ic = 0b11000110,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movlsg, sh2e_insn_decode((sh2e_insn_t) insn_movlsg)->exec);
}

PCUT_TEST(mova_decode)
{
    sh2e_insn_d_t insn_mova = {
        .ic = 0b11000111,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_mova, sh2e_insn_decode((sh2e_insn_t) insn_mova)->exec);
}

PCUT_TEST(bf_decode)
{
    sh2e_insn_d_t insn_bf = {
        .ic = 0b10001011,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_bf, sh2e_insn_decode((sh2e_insn_t) insn_bf)->exec);
}

PCUT_TEST(bfs_decode)
{
    sh2e_insn_d_t insn_bfs = {
        .ic = 0b10001111,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_bfs, sh2e_insn_decode((sh2e_insn_t) insn_bfs)->exec);
}

PCUT_TEST(bt_decode)
{
    sh2e_insn_d_t insn_bt = {
        .ic = 0b10001001,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_bt, sh2e_insn_decode((sh2e_insn_t) insn_bt)->exec);
}

PCUT_TEST(bts_decode)
{
    sh2e_insn_d_t insn_bts = {
        .ic = 0b10001101,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_bts, sh2e_insn_decode((sh2e_insn_t) insn_bts)->exec);
}

/****************************************************************************
 * SH-2E `d12-format` decoding
 ****************************************************************************/

PCUT_TEST(bra_decode)
{
    sh2e_insn_d12_t insn_bra = {
        .ic = 0b1010,
        .d12 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_bra, sh2e_insn_decode((sh2e_insn_t) insn_bra)->exec);
}

PCUT_TEST(bsr_decode)
{
    sh2e_insn_d12_t insn_bsr = {
        .ic = 0b1011,
        .d12 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_bsr, sh2e_insn_decode((sh2e_insn_t) insn_bsr)->exec);
}

/****************************************************************************
 * SH-2E `nd8-format` decoding
 ****************************************************************************/

PCUT_TEST(movwi_decode)
{
    sh2e_insn_nd8_t insn_movwi = {
        .ic = 0b1001,
        .rn = R0_REG,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movwi, sh2e_insn_decode((sh2e_insn_t) insn_movwi)->exec);
}

PCUT_TEST(movli_decode)
{
    sh2e_insn_nd8_t insn_movli = {
        .ic = 0b1101,
        .rn = R0_REG,
        .d8 = 0x0,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movli, sh2e_insn_decode((sh2e_insn_t) insn_movli)->exec);
}

/****************************************************************************
 * SH-2E `i-format` decoding
 ****************************************************************************/

PCUT_TEST(tstm_decode)
{
    sh2e_insn_i_t insn_tstm = {
        .ic = 0b11001100,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_tstm, sh2e_insn_decode((sh2e_insn_t) insn_tstm)->exec);
}

PCUT_TEST(andm_decode)
{
    sh2e_insn_i_t insn_andm = {
        .ic = 0b11001101,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_andm, sh2e_insn_decode((sh2e_insn_t) insn_andm)->exec);
}

PCUT_TEST(xorm_decode)
{
    sh2e_insn_i_t insn_xorm = {
        .ic = 0b11001110,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_xorm, sh2e_insn_decode((sh2e_insn_t) insn_xorm)->exec);
}

PCUT_TEST(orm_decode)
{
    sh2e_insn_i_t insn_orm = {
        .ic = 0b11001111,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_orm, sh2e_insn_decode((sh2e_insn_t) insn_orm)->exec);
}

PCUT_TEST(cmpim_decode)
{
    sh2e_insn_i_t insn_cmpim = {
        .ic = 0b10001000,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_cmpim, sh2e_insn_decode((sh2e_insn_t) insn_cmpim)->exec);
}

PCUT_TEST(tsti_decode)
{
    sh2e_insn_i_t insn_tsti = {
        .ic = 0b11001000,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_tsti, sh2e_insn_decode((sh2e_insn_t) insn_tsti)->exec);
}

PCUT_TEST(andi_decode)
{
    sh2e_insn_i_t insn_andi = {
        .ic = 0b11001001,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_andi, sh2e_insn_decode((sh2e_insn_t) insn_andi)->exec);
}

PCUT_TEST(xori_decode)
{
    sh2e_insn_i_t insn_xori = {
        .ic = 0b11001010,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_xori, sh2e_insn_decode((sh2e_insn_t) insn_xori)->exec);
}

PCUT_TEST(ori_decode)
{
    sh2e_insn_i_t insn_ori = {
        .ic = 0b11001011,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_ori, sh2e_insn_decode((sh2e_insn_t) insn_ori)->exec);
}

PCUT_TEST(trapa_decode)
{
    sh2e_insn_i_t insn_trapa = {
        .ic = 0b11000011,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_trapa, sh2e_insn_decode((sh2e_insn_t) insn_trapa)->exec);
}

/****************************************************************************
 * SH-2E `ni-format` decoding
 ****************************************************************************/

PCUT_TEST(addi_decode)
{
    sh2e_insn_ni_t insn_addi = {
        .ic = 0b0111,
        .rn = R0_REG,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_addi, sh2e_insn_decode((sh2e_insn_t) insn_addi)->exec);
}

PCUT_TEST(movi_decode)
{
    sh2e_insn_ni_t insn_movi = {
        .ic = 0b1110,
        .rn = R0_REG,
        .i8 = 0b00000000,
    };

    PCUT_ASSERT_EQUALS(sh2e_insn_exec_movi, sh2e_insn_decode((sh2e_insn_t) insn_movi)->exec);
}

PCUT_EXPORT(instruction_decoding);
