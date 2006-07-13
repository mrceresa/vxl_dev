#ifndef v3p_netlib_unmangle_h
#define v3p_netlib_unmangle_h

/* This header undefines mangled names defined by v3p_netlib_mangle.h. */

/* Functions */

#undef balanc_
#undef balbak_
#undef calcsc_
#undef caxpy_
#undef ccopy_
#undef cdiv_
#undef cdotc_
#undef cg_
#undef chscdf_
#undef cqrdc_
#undef cqrsl_
#undef cscal_
#undef csrot_
#undef csvdc_
#undef cswap_
#undef cub_
#undef dasum_
#undef daxpy_
#undef dcabs1_
#undef dchscdf_
#undef dcopy_
#undef ddot_
#undef dgecon_
#undef dgemm_
#undef dgemv_
#undef dgeqr2_
#undef dgeqrf_
#undef dger_
#undef dgerq2_
#undef dgesc2_
#undef dgetc2_
#undef dggbak_
#undef dggbal_
#undef dgges_
#undef dgghrd_
#undef dgpfa2f_
#undef dgpfa3f_
#undef dgpfa5f_
#undef dgpfa_
#undef dhgeqz_
#undef dlabad_
#undef dlabax_
#undef dlabcm_
#undef dlabfc_
#undef dlacon_
#undef dlacpy_
#undef dladiv_
#undef dlaeig_
#undef dlag2_
#undef dlager_
#undef dlagv2_
#undef dlamc1_
#undef dlamc2_
#undef dlamc3_
#undef dlamc4_
#undef dlamc5_
#undef dlamch_
#undef dlange_
#undef dlanhs_
#undef dlapy2_
#undef dlapy3_
#undef dlaran_
#undef dlarf_
#undef dlarfb_
#undef dlarfg_
#undef dlarft_
#undef dlartg_
#undef dlascl_
#undef dlaset_
#undef dlassq_
#undef dlasv2_
#undef dlaswp_
#undef dlatdf_
#undef dlatrs_
#undef dmvpc_
#undef dnlaso_
#undef dnorcdf_
#undef dnppla_
#undef dnrm2_
#undef dnwla_
#undef dorg2r_
#undef dorgqr_
#undef dorgr2_
#undef dorm2r_
#undef dormqr_
#undef dormr2_
#undef dortqr_
#undef dpmpar_
#undef dpoco_
#undef dpodi_
#undef dpofa_
#undef dposl_
#undef dqrdc_
#undef dqrsl_
#undef drot_
#undef drotg_
#undef drscl_
#undef dscal_
#undef dsvdc_
#undef dswap_
#undef dtgex2_
#undef dtgexc_
#undef dtgsen_
#undef dtgsy2_
#undef dtgsyl_
#undef dtrmm_
#undef dtrmv_
#undef dtrsv_
#undef dvsort_
#undef dzasum_
#undef dznrm2_
#undef elmhes_
#undef eltran_
#undef enorm_
#undef epslon_
#undef fd_
#undef fdjac2_
#undef fv_
#undef fvd_
#undef fxshfr_
#undef gpfa2f_
#undef gpfa3f_
#undef gpfa5f_
#undef gpfa_
#undef hqr2_
#undef hqr_
#undef idamax_
#undef ieeeck_
#undef ilaenv_
#undef ins_
#undef izamax_
#undef lb1_
#undef lbfgs_
#undef lce_
#undef lde_
#undef lmder1_
#undef lmder_
#undef lmdif_
#undef lmpar_
#undef lsame_
#undef lse_
#undef lsqr_
#undef mcsrch_
#undef mcstep_
#undef newest_
#undef nextk_
#undef pythag_
#undef qrfac_
#undef qrsolv_
#undef quad_
#undef quadit_
#undef quadsd_
#undef realit_
#undef rebak_
#undef reduc_
#undef rg_
#undef rpoly_
#undef rs_
#undef rsg_
#undef saxpy_
#undef sbeg_
#undef sblas2_
#undef schck1_
#undef schck2_
#undef schck3_
#undef schck4_
#undef schck5_
#undef scnrm2_
#undef scopy_
#undef sdiff_
#undef sdot_
#undef setdgpfa_
#undef setgpfa_
#undef sgbmv_
#undef sgemv_
#undef sger_
#undef simpru_
#undef smake_
#undef smessg_
#undef snrm2_
#undef sopen_
#undef sqrdc_
#undef sqrsl_
#undef srot_
#undef srotg_
#undef ssbmv_
#undef sscal_
#undef sspmv_
#undef sspr2_
#undef sspr_
#undef ssvdc_
#undef sswap_
#undef ssymv_
#undef ssyr2_
#undef ssyr_
#undef stbmv_
#undef stbsv_
#undef stpmv_
#undef stpsv_
#undef strmv_
#undef strsv_
#undef tql1_
#undef tql2_
#undef tqlrat_
#undef tred1_
#undef tred2_
#undef urand_
#undef xerbla_
#undef xsimpru_
#undef zaxpy_
#undef zcopy_
#undef zdotc_
#undef zdotu_
#undef zdrot_
#undef zdscal_
#undef zgebak_
#undef zgebal_
#undef zgeev_
#undef zgehd2_
#undef zgehrd_
#undef zgemm_
#undef zgemv_
#undef zgerc_
#undef zhseqr_
#undef zlacgv_
#undef zlacpy_
#undef zladiv_
#undef zlahqr_
#undef zlahrd_
#undef zlange_
#undef zlanhs_
#undef zlarf_
#undef zlarfb_
#undef zlarfg_
#undef zlarft_
#undef zlarfx_
#undef zlascl_
#undef zlaset_
#undef zlassq_
#undef zlatrs_
#undef zqrdc_
#undef zqrsl_
#undef zscal_
#undef zsvdc_
#undef zswap_
#undef ztrevc_
#undef ztrmm_
#undef ztrmv_
#undef ztrsv_
#undef zung2r_
#undef zunghr_
#undef zungqr_

#endif
