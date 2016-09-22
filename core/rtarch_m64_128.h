/******************************************************************************/
/* Copyright (c) 2013-2016 VectorChief (at github, bitbucket, sourceforge)    */
/* Distributed under the MIT software license, see the accompanying           */
/* file COPYING or http://www.opensource.org/licenses/mit-license.php         */
/******************************************************************************/

#ifndef RT_RTARCH_M64_128_H
#define RT_RTARCH_M64_128_H

#include "rtarch_m32_128.h"

#if defined (RT_SIMD_CODE)

/******************************************************************************/
/*********************************   LEGEND   *********************************/
/******************************************************************************/

/*
 * rtarch_m64_128.h: Implementation of MIPS fp64 MSA instructions.
 *
 * This file is a part of the unified SIMD assembler framework (rtarch.h)
 * designed to be compatible with different processor architectures,
 * while maintaining strictly defined common API.
 *
 * Recommended naming scheme for instructions:
 *
 * cmdp*_ri - applies [cmd] to [p]acked: [r]egister from [i]mmediate
 * cmdp*_rr - applies [cmd] to [p]acked: [r]egister from [r]egister
 *
 * cmdp*_rm - applies [cmd] to [p]acked: [r]egister from [m]emory
 * cmdp*_ld - applies [cmd] to [p]acked: as above
 *
 * cmdpx_** - applies [cmd] to [p]acked unsigned integer args, [x] - default
 * cmdpn_** - applies [cmd] to [p]acked   signed integer args, [n] - negatable
 * cmdps_** - applies [cmd] to [p]acked floating point   args, [s] - scalable
 *
 * cmdo*_** - applies [cmd] to 32-bit SIMD register/memory/immediate args
 * cmdp*_** - applies [cmd] to L-size SIMD register/memory/immediate args
 * cmdq*_** - applies [cmd] to 64-bit SIMD register/memory/immediate args
 *
 * The cmdp*_** instructions are intended for SPMD programming model
 * and can be configured to work with 32/64-bit data-elements (int, fp).
 * In this model data-paths are fixed-width, BASE and SIMD data-elements are
 * width-compatible, code-path divergence is handled via CHECK_MASK macro.
 *
 * Interpretation of instruction parameters:
 *
 * upper-case params have triplet structure and require W to pass-forward
 * lower-case params are singular and can be used/passed as such directly
 *
 * XG - SIMD register serving as target and fisrt source
 * XS - SIMD register serving as second source
 * IM - immediate value (smallest size IC is used for shifts)
 *
 * RG - BASE register serving as target and first source
 * RM - BASE register addressing mode (Oeax, M***, I***)
 * DP - displacement value (of given size DP, DF, DG, DH, DV)
 */

/******************************************************************************/
/********************************   INTERNAL   ********************************/
/******************************************************************************/

/******************************************************************************/
/********************************   EXTERNAL   ********************************/
/******************************************************************************/

#undef  movqx_ld

/******************************************************************************/
/**********************************   MSA   ***********************************/
/******************************************************************************/

/**************************   packed generic (SIMD)   *************************/

/* mov */

#define movqx_rr(XG, XS)                                                    \
        EMITW(0x78BE0019 | MXM(REG(XG), REG(XS), 0x00))

#define movqx_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(REG(XG), MOD(RM), VAL(DP), B2(DP), P2(DP)))

#define movqx_st(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000027 | MPM(REG(XG), MOD(RM), VAL(DP), B2(DP), P2(DP)))

/* and */

#define andqx_rr(XG, XS)                                                    \
        EMITW(0x7800001E | MXM(REG(XG), REG(XG), REG(XS)))

#define andqx_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7800001E | MXM(REG(XG), REG(XG), Tmm1))

/* ann (~XG & XS) */

#define annqx_rr(XG, XS)                                                    \
        EMITW(0x78C0001E | MXM(REG(XG), REG(XS), TmmZ))

#define annqx_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x78C0001E | MXM(REG(XG), Tmm1,    TmmZ))

/* orr */

#define orrqx_rr(XG, XS)                                                    \
        EMITW(0x7820001E | MXM(REG(XG), REG(XG), REG(XS)))

#define orrqx_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7820001E | MXM(REG(XG), REG(XG), Tmm1))

/* orn (~XG | XS) */

#define ornqx_rr(XG, XS)                                                    \
        notqx_rx(W(XG))                                                     \
        orrqx_rr(W(XG), W(XS))

#define ornqx_ld(XG, RM, DP)                                                \
        notqx_rx(W(XG))                                                     \
        orrqx_ld(W(XG), W(RM), W(DP))

/* xor */

#define xorqx_rr(XG, XS)                                                    \
        EMITW(0x7860001E | MXM(REG(XG), REG(XG), REG(XS)))

#define xorqx_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7860001E | MXM(REG(XG), REG(XG), Tmm1))

/* not */

#define notqx_rx(XG)                                                        \
        EMITW(0x7840001E | MXM(REG(XG), TmmZ,    REG(XG)))

/**************   packed double precision floating point (SIMD)   *************/

/* neg */

#define negqs_rx(XG)                                                        \
        EMITW(0x7860001E | MXM(REG(XG), REG(XG), TmmT))

/* add */

#define addqs_rr(XG, XS)                                                    \
        EMITW(0x7820001B | MXM(REG(XG), REG(XG), REG(XS)))

#define addqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7820001B | MXM(REG(XG), REG(XG), Tmm1))

/* sub */

#define subqs_rr(XG, XS)                                                    \
        EMITW(0x7860001B | MXM(REG(XG), REG(XG), REG(XS)))

#define subqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7860001B | MXM(REG(XG), REG(XG), Tmm1))

/* mul */

#define mulqs_rr(XG, XS)                                                    \
        EMITW(0x78A0001B | MXM(REG(XG), REG(XG), REG(XS)))

#define mulqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x78A0001B | MXM(REG(XG), REG(XG), Tmm1))

/* div */

#define divqs_rr(XG, XS)                                                    \
        EMITW(0x78E0001B | MXM(REG(XG), REG(XG), REG(XS)))

#define divqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x78E0001B | MXM(REG(XG), REG(XG), Tmm1))

/* sqr */

#define sqrqs_rr(XG, XS)                                                    \
        EMITW(0x7B27001E | MXM(REG(XG), REG(XS), 0x00))

#define sqrqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7B27001E | MXM(REG(XG), Tmm1,    0x00))

/* cbr */

        /* cbe, cbs, cbr defined in rtbase.h
         * under "COMMON SIMD INSTRUCTIONS" section */

/* rcp
 * accuracy/behavior may vary across supported targets, use accordingly */

#if RT_SIMD_COMPAT_RCP == 0

#define rceqs_rr(XG, XS)                                                    \
        EMITW(0x7B2B001E | MXM(REG(XG), REG(XS), 0x00))

#define rcsqs_rr(XG, XS) /* destroys RM */

#endif /* RT_SIMD_COMPAT_RCP */

        /* rcp defined in rtbase.h
         * under "COMMON SIMD INSTRUCTIONS" section */

/* rsq
 * accuracy/behavior may vary across supported targets, use accordingly */

#if RT_SIMD_COMPAT_RSQ == 0

#define rseqs_rr(XG, XS)                                                    \
        EMITW(0x7B29001E | MXM(REG(XG), REG(XS), 0x00))

#define rssqs_rr(XG, XS) /* destroys RM */

#endif /* RT_SIMD_COMPAT_RSQ */

        /* rsq defined in rtbase.h
         * under "COMMON SIMD INSTRUCTIONS" section */

/* min */

#define minqs_rr(XG, XS)                                                    \
        EMITW(0x7B20001B | MXM(REG(XG), REG(XG), REG(XS)))

#define minqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7B20001B | MXM(REG(XG), REG(XG), Tmm1))

/* max */

#define maxqs_rr(XG, XS)                                                    \
        EMITW(0x7BA0001B | MXM(REG(XG), REG(XG), REG(XS)))

#define maxqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7BA0001B | MXM(REG(XG), REG(XG), Tmm1))

/* cmp */

#define ceqqs_rr(XG, XS)                                                    \
        EMITW(0x78A0001A | MXM(REG(XG), REG(XG), REG(XS)))

#define ceqqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x78A0001A | MXM(REG(XG), REG(XG), Tmm1))

#define cneqs_rr(XG, XS)                                                    \
        EMITW(0x78E0001C | MXM(REG(XG), REG(XG), REG(XS)))

#define cneqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x78E0001C | MXM(REG(XG), REG(XG), Tmm1))

#define cltqs_rr(XG, XS)                                                    \
        EMITW(0x7920001A | MXM(REG(XG), REG(XG), REG(XS)))

#define cltqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7920001A | MXM(REG(XG), REG(XG), Tmm1))

#define cleqs_rr(XG, XS)                                                    \
        EMITW(0x79A0001A | MXM(REG(XG), REG(XG), REG(XS)))

#define cleqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x79A0001A | MXM(REG(XG), REG(XG), Tmm1))

#define cgtqs_rr(XG, XS)                                                    \
        EMITW(0x7920001A | MXM(REG(XG), REG(XS), REG(XG)))

#define cgtqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7920001A | MXM(REG(XG), Tmm1,    REG(XG)))

#define cgeqs_rr(XG, XS)                                                    \
        EMITW(0x79A0001A | MXM(REG(XG), REG(XS), REG(XG)))

#define cgeqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x79A0001A | MXM(REG(XG), Tmm1,    REG(XG)))

/**************************   packed integer (SIMD)   *************************/

/* cvz (fp-to-signed-int)
 * rounding mode is encoded directly (can be used in FCTRL blocks)
 * NOTE: due to compatibility with legacy targets, SIMD fp-to-int
 * round instructions are only accurate within 64-bit signed int range */

#define rnzqs_rr(XG, XS)     /* round towards zero */                       \
        cvzqs_rr(W(XG), W(XS))                                              \
        cvnqn_rr(W(XG), W(XG))

#define rnzqs_ld(XG, RM, DP) /* round towards zero */                       \
        cvzqs_ld(W(XG), W(RM), W(DP))                                       \
        cvnqn_rr(W(XG), W(XG))

#define cvzqs_rr(XG, XS)     /* round towards zero */                       \
        EMITW(0x7B23001E | MXM(REG(XG), REG(XS), 0x00))

#define cvzqs_ld(XG, RM, DP) /* round towards zero */                       \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7B23001E | MXM(REG(XG), Tmm1,    0x00))

/* cvp (fp-to-signed-int)
 * rounding mode encoded directly (cannot be used in FCTRL blocks)
 * NOTE: due to compatibility with legacy targets, SIMD fp-to-int
 * round instructions are only accurate within 64-bit signed int range */

#define rnpqs_rr(XG, XS)     /* round towards +inf */                       \
        FCTRL_ENTER(ROUNDP)                                                 \
        rndqs_rr(W(XG), W(XS))                                              \
        FCTRL_LEAVE(ROUNDP)

#define rnpqs_ld(XG, RM, DP) /* round towards +inf */                       \
        FCTRL_ENTER(ROUNDP)                                                 \
        rndqs_ld(W(XG), W(RM), W(DP))                                       \
        FCTRL_LEAVE(ROUNDP)

#define cvpqs_rr(XG, XS)     /* round towards +inf */                       \
        FCTRL_ENTER(ROUNDP)                                                 \
        cvtqs_rr(W(XG), W(XS))                                              \
        FCTRL_LEAVE(ROUNDP)

#define cvpqs_ld(XG, RM, DP) /* round towards +inf */                       \
        FCTRL_ENTER(ROUNDP)                                                 \
        cvtqs_ld(W(XG), W(RM), W(DP))                                       \
        FCTRL_LEAVE(ROUNDP)

/* cvm (fp-to-signed-int)
 * rounding mode encoded directly (cannot be used in FCTRL blocks)
 * NOTE: due to compatibility with legacy targets, SIMD fp-to-int
 * round instructions are only accurate within 64-bit signed int range */

#define rnmqs_rr(XG, XS)     /* round towards -inf */                       \
        FCTRL_ENTER(ROUNDM)                                                 \
        rndqs_rr(W(XG), W(XS))                                              \
        FCTRL_LEAVE(ROUNDM)

#define rnmqs_ld(XG, RM, DP) /* round towards -inf */                       \
        FCTRL_ENTER(ROUNDM)                                                 \
        rndqs_ld(W(XG), W(RM), W(DP))                                       \
        FCTRL_LEAVE(ROUNDM)

#define cvmqs_rr(XG, XS)     /* round towards -inf */                       \
        FCTRL_ENTER(ROUNDM)                                                 \
        cvtqs_rr(W(XG), W(XS))                                              \
        FCTRL_LEAVE(ROUNDM)

#define cvmqs_ld(XG, RM, DP) /* round towards -inf */                       \
        FCTRL_ENTER(ROUNDM)                                                 \
        cvtqs_ld(W(XG), W(RM), W(DP))                                       \
        FCTRL_LEAVE(ROUNDM)

/* cvn (fp-to-signed-int)
 * rounding mode encoded directly (cannot be used in FCTRL blocks)
 * NOTE: due to compatibility with legacy targets, SIMD fp-to-int
 * round instructions are only accurate within 64-bit signed int range */

#define rnnqs_rr(XG, XS)     /* round towards near */                       \
        rndqs_rr(W(XG), W(XS))

#define rnnqs_ld(XG, RM, DP) /* round towards near */                       \
        rndqs_ld(W(XG), W(RM), W(DP))

#define cvnqs_rr(XG, XS)     /* round towards near */                       \
        cvtqs_rr(W(XG), W(XS))

#define cvnqs_ld(XG, RM, DP) /* round towards near */                       \
        cvtqs_ld(W(XG), W(RM), W(DP))

/* cvn (signed-int-to-fp)
 * rounding mode encoded directly (cannot be used in FCTRL blocks) */

#define cvnqn_rr(XG, XS)     /* round towards near */                       \
        cvtqn_rr(W(XG), W(XS))

#define cvnqn_ld(XG, RM, DP) /* round towards near */                       \
        cvtqn_ld(W(XG), W(RM), W(DP))

/* add */

#define addqx_rr(XG, XS)                                                    \
        EMITW(0x7860000E | MXM(REG(XG), REG(XG), REG(XS)))

#define addqx_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7860000E | MXM(REG(XG), REG(XG), Tmm1))

/* sub */

#define subqx_rr(XG, XS)                                                    \
        EMITW(0x78E0000E | MXM(REG(XG), REG(XG), REG(XS)))

#define subqx_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x78E0000E | MXM(REG(XG), REG(XG), Tmm1))

/* shl */

#define shlqx_ri(XG, IM)                                                    \
        EMITW(0x78000009 | MXM(REG(XG), REG(XG), 0x00) |                    \
                                                 (0x3F & VAL(IM)) << 16)

#define shlqx_ld(XG, RM, DP) /* loads SIMD, uses 1 elem at given address */ \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C1(DP), EMPTY2)   \
        EMITW(0xDC000000 | MDM(TMxx,    MOD(RM), VAL(DP), B1(DP), P1(DP)))  \
        EMITW(0x7B03001E | MXM(Tmm1,    TMxx,    0x00))                     \
        EMITW(0x7860000D | MXM(REG(XG), REG(XG), Tmm1))

/* shr */

#define shrqx_ri(XG, IM)                                                    \
        EMITW(0x79000009 | MXM(REG(XG), REG(XG), 0x00) |                    \
                                                 (0x3F & VAL(IM)) << 16)

#define shrqx_ld(XG, RM, DP) /* loads SIMD, uses 1 elem at given address */ \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C1(DP), EMPTY2)   \
        EMITW(0xDC000000 | MDM(TMxx,    MOD(RM), VAL(DP), B1(DP), P1(DP)))  \
        EMITW(0x7B03001E | MXM(Tmm1,    TMxx,    0x00))                     \
        EMITW(0x7960000D | MXM(REG(XG), REG(XG), Tmm1))

#define shrqn_ri(XG, IM)                                                    \
        EMITW(0x78800009 | MXM(REG(XG), REG(XG), 0x00) |                    \
                                                 (0x3F & VAL(IM)) << 16)

#define shrqn_ld(XG, RM, DP) /* loads SIMD, uses 1 elem at given address */ \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C1(DP), EMPTY2)   \
        EMITW(0xDC000000 | MDM(TMxx,    MOD(RM), VAL(DP), B1(DP), P1(DP)))  \
        EMITW(0x7B03001E | MXM(Tmm1,    TMxx,    0x00))                     \
        EMITW(0x78E0000D | MXM(REG(XG), REG(XG), Tmm1))

/**************************   helper macros (SIMD)   **************************/

/* cvt (fp-to-signed-int)
 * rounding mode comes from fp control register (set in FCTRL blocks)
 * NOTE: ROUNDZ is not supported on pre-VSX Power systems, use cvz
 * NOTE: due to compatibility with legacy targets, SIMD fp-to-int
 * round instructions are only accurate within 64-bit signed int range */

#define rndqs_rr(XG, XS)                                                    \
        EMITW(0x7B2D001E | MXM(REG(XG), REG(XS), 0x00))

#define rndqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7B2D001E | MXM(REG(XG), Tmm1,    0x00))

#define cvtqs_rr(XG, XS)                                                    \
        EMITW(0x7B39001E | MXM(REG(XG), REG(XS), 0x00))

#define cvtqs_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7B39001E | MXM(REG(XG), Tmm1,    0x00))

/* cvt (signed-int-to-fp)
 * rounding mode comes from fp control register (set in FCTRL blocks)
 * NOTE: only default ROUNDN is supported on pre-VSX Power systems */

#define cvtqn_rr(XG, XS)                                                    \
        EMITW(0x7B3D001E | MXM(REG(XG), REG(XS), 0x00))

#define cvtqn_ld(XG, RM, DP)                                                \
        AUW(SIB(RM),  EMPTY,  EMPTY,    MOD(RM), VAL(DP), C2(DP), EMPTY2)   \
        EMITW(0x78000023 | MPM(Tmm1,    MOD(RM), VAL(DP), B2(DP), P2(DP)))  \
        EMITW(0x7B3D001E | MXM(REG(XG), Tmm1,    0x00))

/* cvr (fp-to-signed-int)
 * rounding mode is encoded directly (cannot be used in FCTRL blocks)
 * NOTE: on targets with full-IEEE SIMD fp-arithmetic the ROUND*_F mode
 * isn't always taken into account when used within full-IEEE ASM block
 * NOTE: due to compatibility with legacy targets, SIMD fp-to-int
 * round instructions are only accurate within 64-bit signed int range */

#define rnrqs_rr(XG, XS, mode)                                              \
        FCTRL_ENTER(mode)                                                   \
        rndqs_rr(W(XG), W(XS))                                              \
        FCTRL_LEAVE(mode)

#define cvrqs_rr(XG, XS, mode)                                              \
        FCTRL_ENTER(mode)                                                   \
        cvtqs_rr(W(XG), W(XS))                                              \
        FCTRL_LEAVE(mode)

#endif /* RT_SIMD_CODE */

#endif /* RT_RTARCH_M64_128_H */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
