#include "JCBCompressor.h"

namespace JCBCompressor {

/****************************************************************************************
Copyright (c) 2023 Cycling '74

The code that Max generates automatically and that end users are capable of
exporting and using, and any associated documentation files (the “Software”)
is a work of authorship for which Cycling '74 is the author and owner for
copyright purposes.

This Software is dual-licensed either under the terms of the Cycling '74
License for Max-Generated Code for Export, or alternatively under the terms
of the General Public License (GPL) Version 3. You may use the Software
according to either of these licenses as it is most appropriate for your
project on a case-by-case basis (proprietary or not).

A) Cycling '74 License for Max-Generated Code for Export

A license is hereby granted, free of charge, to any person obtaining a copy
of the Software (“Licensee”) to use, copy, modify, merge, publish, and
distribute copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The Software is licensed to Licensee for all uses that do not include the sale,
sublicensing, or commercial distribution of software that incorporates this
source code. This means that the Licensee is free to use this software for
educational, research, and prototyping purposes, to create musical or other
creative works with software that incorporates this source code, or any other
use that does not constitute selling software that makes use of this source
code. Commercial distribution also includes the packaging of free software with
other paid software, hardware, or software-provided commercial services.

For entities with UNDER 200k USD in annual revenue or funding, a license is hereby
granted, free of charge, for the sale, sublicensing, or commercial distribution
of software that incorporates this source code, for as long as the entity's
annual revenue remains below 200k USD annual revenue or funding.

For entities with OVER 200k USD in annual revenue or funding interested in the
sale, sublicensing, or commercial distribution of software that incorporates
this source code, please send inquiries to licensing (at) cycling74.com.

The above copyright notice and this license shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Please see
https://support.cycling74.com/hc/en-us/articles/360050779193-Gen-Code-Export-Licensing-FAQ
for additional information

B) General Public License Version 3 (GPLv3)
Details of the GPLv3 license can be found at: https://www.gnu.org/licenses/gpl-3.0.html
****************************************************************************************/

// global noise generator
Noise noise;
[[maybe_unused]] static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State {
	CommonState __commonstate;
	Delay m_sidechainTapDelay_23;
	Delay m_rightInputDelay_24;
	Delay m_sidechainWriteDelay_22;
	Delay m_leftInputDelay_25;
	Delay m_leftOutputDelay_27;
	Delay m_rightOutputDelay_26;
	Delay m_rmsDelay_21;
	int __exception;
	int vectorsize;
	t_sample m_thresholdHistory_65;
	t_sample m_makeupGainHistory_64;
	t_sample m_soloSidechainHistory_63;
	t_sample m_bypassAmountHistory_62;
	t_sample m_autoGainHistory_66;
	t_sample m_parallelMixHistory_68;
	t_sample m_deltaModeHistory_67;
	t_sample m_u_SOFTCLIP_70;
	t_sample m_p_BYPASS_69;
	t_sample m_x_AUTOGAIN_71;
	t_sample m_keyMixHistory_61;
	t_sample m_trimHistory_59;
	t_sample m_smoothAmountHistory_54;
	t_sample m_lookaheadHistory_53;
	t_sample m_hpfFreqHistory_52;
	t_sample m_dryWetMixHistory_60;
	t_sample m_reactHistory_55;
	t_sample m_ratioHistory_57;
	t_sample m_kneeHistory_56;
	t_sample m_sidechainTrimHistory_58;
	t_sample m_f_ALGO_87;
	t_sample m_g_REACT_86;
	t_sample m_d_ATK_91;
	t_sample m_k_LPFORDER_81;
	t_sample m_l_SC_80;
	t_sample m_y_SCTRIM_79;
	t_sample m_r_KEY_78;
	t_sample m_k_LPF_82;
	t_sample m_j_HPF_84;
	t_sample m_j_HPFORDER_83;
	t_sample m_z_SMOOTH_85;
	t_sample m_c_RATIO_92;
	t_sample m_m_SOLOSC_77;
	t_sample m_w_PARALLEL_75;
	t_sample m_i_MAKEUP_89;
	t_sample m_lpfFreqHistory_51;
	t_sample m_e_REL_90;
	t_sample m_o_DRYWET_76;
	t_sample m_h_KNEE_88;
	t_sample m_n_LOOKAHEAD_73;
	t_sample m_s_AUTORELEASE_72;
	t_sample m_v_DELTA_74;
	t_sample m_scEnableHistory_50;
	t_sample m_lpfHistory_46;
	t_sample m_atkHistory_48;
	t_sample m_rmsSqClassic_13;
	t_sample m_histRmsClassic_12;
	t_sample m_releaseHistClassic_11;
	t_sample m_attackHistClassic_10;
	t_sample m_peakEnvClassic_14;
	t_sample m_attackHistSharp_16;
	t_sample m_extraSmoothHistSharp_15;
	t_sample m_peakEnvSharp_18;
	t_sample m_releaseHistSharp_17;
	t_sample m_prevEnvelope_19;
	t_sample m_extraSmoothHistClassic_9;
	t_sample m_rmsSqSlow_7;
	t_sample m_reductionHistLeft_2;
	t_sample m_reductionHistRight_1;
	t_sample samplerate;
	t_sample m_peakEnvSlow_8;
	t_sample m_extraSmoothHistSlow_3;
	t_sample m_releaseHistSlow_5;
	t_sample m_attackHistSlow_4;
	t_sample m_histRmsSlow_6;
	t_sample m_softclipHistory_49;
	t_sample m_rmsSum_20;
	t_sample m_lpfOrderHistory_29;
	t_sample m_lpfHistory_43;
	t_sample m_lpfHistory_42;
	t_sample m_lpfHistory_41;
	t_sample m_lpfHistory_40;
	t_sample m_lpfHistory_44;
	t_sample m_b_THD_93;
	t_sample m_lpfHistory_45;
	t_sample m_relHistory_47;
	t_sample m_transientDetector_28;
	t_sample m_lpfHistory_39;
	t_sample m_hpfHistory_37;
	t_sample m_hpfHistory_32;
	t_sample m_hpfHistory_31;
	t_sample m_hpfOrderHistory_30;
	t_sample m_hpfHistory_38;
	t_sample m_hpfHistory_33;
	t_sample m_hpfHistory_35;
	t_sample m_hpfHistory_34;
	t_sample m_hpfHistory_36;
	t_sample m_a_TRIM_94;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_reductionHistRight_1 = ((int)0);
		m_reductionHistLeft_2 = ((int)0);
		m_extraSmoothHistSlow_3 = ((int)0);
		m_attackHistSlow_4 = ((int)0);
		m_releaseHistSlow_5 = ((int)0);
		m_histRmsSlow_6 = ((int)0);
		m_rmsSqSlow_7 = ((int)0);
		m_peakEnvSlow_8 = ((int)0);
		m_extraSmoothHistClassic_9 = ((int)0);
		m_attackHistClassic_10 = ((int)0);
		m_releaseHistClassic_11 = ((int)0);
		m_histRmsClassic_12 = ((int)0);
		m_rmsSqClassic_13 = ((int)0);
		m_peakEnvClassic_14 = ((int)0);
		m_extraSmoothHistSharp_15 = ((int)0);
		m_attackHistSharp_16 = ((int)0);
		m_releaseHistSharp_17 = ((int)0);
		m_peakEnvSharp_18 = ((int)0);
		m_prevEnvelope_19 = ((int)0);
		m_rmsSum_20 = ((int)0);
		m_rmsDelay_21.reset("m_rmsDelay_21", ((int)500));
		m_sidechainWriteDelay_22.reset("m_sidechainWriteDelay_22", (((t_sample)0.025) * samplerate));
		m_sidechainTapDelay_23.reset("m_sidechainTapDelay_23", (((t_sample)0.025) * samplerate));
		m_rightInputDelay_24.reset("m_rightInputDelay_24", (((t_sample)0.025) * samplerate));
		m_leftInputDelay_25.reset("m_leftInputDelay_25", (((t_sample)0.025) * samplerate));
		m_rightOutputDelay_26.reset("m_rightOutputDelay_26", (((t_sample)0.025) * samplerate));
		m_leftOutputDelay_27.reset("m_leftOutputDelay_27", (((t_sample)0.025) * samplerate));
		m_transientDetector_28 = ((int)0);
		m_lpfOrderHistory_29 = ((int)0);
		m_hpfOrderHistory_30 = ((int)0);
		m_hpfHistory_31 = ((int)0);
		m_hpfHistory_32 = ((int)0);
		m_hpfHistory_33 = ((int)0);
		m_hpfHistory_34 = ((int)0);
		m_hpfHistory_35 = ((int)0);
		m_hpfHistory_36 = ((int)0);
		m_hpfHistory_37 = ((int)0);
		m_hpfHistory_38 = ((int)0);
		m_lpfHistory_39 = ((int)0);
		m_lpfHistory_40 = ((int)0);
		m_lpfHistory_41 = ((int)0);
		m_lpfHistory_42 = ((int)0);
		m_lpfHistory_43 = ((int)0);
		m_lpfHistory_44 = ((int)0);
		m_lpfHistory_45 = ((int)0);
		m_lpfHistory_46 = ((int)0);
		m_relHistory_47 = ((int)0);
		m_atkHistory_48 = ((int)0);
		m_softclipHistory_49 = ((int)0);
		m_scEnableHistory_50 = ((int)0);
		m_lpfFreqHistory_51 = ((int)0);
		m_hpfFreqHistory_52 = ((int)0);
		m_lookaheadHistory_53 = ((int)0);
		m_smoothAmountHistory_54 = ((int)0);
		m_reactHistory_55 = ((int)0);
		m_kneeHistory_56 = ((int)0);
		m_ratioHistory_57 = ((int)0);
		m_sidechainTrimHistory_58 = ((int)0);
		m_trimHistory_59 = ((int)0);
		m_dryWetMixHistory_60 = ((int)0);
		m_keyMixHistory_61 = ((int)0);
		m_bypassAmountHistory_62 = ((int)0);
		m_soloSidechainHistory_63 = ((int)0);
		m_makeupGainHistory_64 = ((int)0);
		m_thresholdHistory_65 = ((int)0);
		m_autoGainHistory_66 = ((int)0);
		m_deltaModeHistory_67 = ((int)0);
		m_parallelMixHistory_68 = ((int)0);
		m_p_BYPASS_69 = 0;
		m_u_SOFTCLIP_70 = 0;
		m_x_AUTOGAIN_71 = 0;
		m_s_AUTORELEASE_72 = 0;
		m_n_LOOKAHEAD_73 = 0;
		m_v_DELTA_74 = 0;
		m_w_PARALLEL_75 = 0;
		m_o_DRYWET_76 = 1;
		m_m_SOLOSC_77 = 0;
		m_r_KEY_78 = 0;
		m_y_SCTRIM_79 = 0;
		m_l_SC_80 = 0;
		m_k_LPFORDER_81 = 0;
		m_k_LPF_82 = 20000;
		m_j_HPFORDER_83 = 0;
		m_j_HPF_84 = 20;
		m_z_SMOOTH_85 = 0;
		m_g_REACT_86 = 0;
		m_f_ALGO_87 = 1;
		m_h_KNEE_88 = 0;
		m_i_MAKEUP_89 = 0;
		m_e_REL_90 = 30;
		m_d_ATK_91 = 5;
		m_c_RATIO_92 = 4;
		m_b_THD_93 = -18;
		m_a_TRIM_94 = 0;
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) {
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		const t_sample * __in2 = __ins[1];
		const t_sample * __in3 = __ins[2];
		const t_sample * __in4 = __ins[3];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		t_sample * __out3 = __outs[2];
		t_sample * __out4 = __outs[3];
		t_sample * __out5 = __outs[4];
		t_sample * __out6 = __outs[5];
		t_sample * __out7 = __outs[6];
		if (__exception) {
			return __exception;
			
		} else if (( (__in1 == 0) || (__in2 == 0) || (__in3 == 0) || (__in4 == 0) || (__out1 == 0) || (__out2 == 0) || (__out3 == 0) || (__out4 == 0) || (__out5 == 0) || (__out6 == 0) || (__out7 == 0) )) {
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample smoothLookahead = m_n_LOOKAHEAD_73;
		t_sample lookaheadHistoryNext = fixdenorm(smoothLookahead);
		t_sample lookaheadSamples_211 = (smoothLookahead * (samplerate * 0.001));
		t_sample bypassInverted = (((int)1) - m_p_BYPASS_69);
		t_sample ALGO = m_f_ALGO_87;
		t_sample AUTO_RELEASE = m_s_AUTORELEASE_72;
		t_sample maxb_99 = floor((((int)25) * (samplerate * ((t_sample)2.0833333333333e-05))));
		t_sample rms_window = ((((int)1) < maxb_99) ? maxb_99 : ((int)1));
		t_sample rms_window_inv = safediv(((int)1), rms_window);
		t_sample rms_coeff = exp(safediv(((int)-1), ((((int)50) * samplerate) * ((t_sample)0.001))));
		t_sample rms_coeff_slow = exp(safediv(((int)-1), ((((int)200) * samplerate) * ((t_sample)0.001))));
		t_sample detector_control = ((ALGO <= ((int)0)) ? ((int)0) : ((ALGO >= ((int)2)) ? ((int)2) : ALGO));
		t_sample fade_amount = (detector_control - ((int)1));
		int cond_100 = (detector_control <= ((int)1));
		// the main sample loop;
		while ((__n--)) {
			const t_sample in1 = (*(__in1++));
			const t_sample in2 = (*(__in2++));
			const t_sample in3 = (*(__in3++));
			const t_sample in4 = (*(__in4++));
			t_sample smoothParallelMix = ((m_parallelMixHistory_68 * ((t_sample)0.999)) + (m_w_PARALLEL_75 * ((t_sample)0.001)));
			t_sample parallelMixHistoryNext = fixdenorm(smoothParallelMix);
			t_sample smoothDeltaMode = ((m_deltaModeHistory_67 * ((t_sample)0.999)) + (m_v_DELTA_74 * ((t_sample)0.001)));
			t_sample deltaModeHistoryNext = fixdenorm(smoothDeltaMode);
			t_sample smoothAutoGain = ((m_autoGainHistory_66 * ((t_sample)0.999)) + (m_x_AUTOGAIN_71 * ((t_sample)0.001)));
			t_sample autoGainHistoryNext = fixdenorm(smoothAutoGain);
			t_sample mix_283 = (smoothAutoGain + (smoothDeltaMode * (((int)0) - smoothAutoGain)));
			t_sample mix_284 = (smoothParallelMix + (smoothDeltaMode * (((int)0) - smoothParallelMix)));
			t_sample smoothThresholdDb = ((m_thresholdHistory_65 * ((t_sample)0.999)) + (m_b_THD_93 * ((t_sample)0.001)));
			t_sample thresholdHistoryNext = fixdenorm(smoothThresholdDb);
			t_sample smoothHpfOrder = ((m_hpfOrderHistory_30 * ((t_sample)0.999)) + (m_j_HPFORDER_83 * ((t_sample)0.001)));
			t_sample hpfOrderHistoryNext = fixdenorm(smoothHpfOrder);
			t_sample smoothLpfOrder = ((m_lpfOrderHistory_29 * ((t_sample)0.999)) + (m_k_LPFORDER_81 * ((t_sample)0.001)));
			t_sample lpfOrderHistoryNext = fixdenorm(smoothLpfOrder);
			t_sample smoothRatio = ((m_ratioHistory_57 * ((t_sample)0.999)) + (m_c_RATIO_92 * ((t_sample)0.001)));
			t_sample ratioHistoryNext = fixdenorm(smoothRatio);
			t_sample smoothKnee = ((m_kneeHistory_56 * ((t_sample)0.999)) + (m_h_KNEE_88 * ((t_sample)0.001)));
			t_sample kneeHistoryNext = fixdenorm(smoothKnee);
			t_sample smoothReact = ((m_reactHistory_55 * ((t_sample)0.999)) + (m_g_REACT_86 * ((t_sample)0.001)));
			t_sample reactHistoryNext = fixdenorm(smoothReact);
			t_sample smoothSmoothAmount = ((m_smoothAmountHistory_54 * ((t_sample)0.999)) + (m_z_SMOOTH_85 * ((t_sample)0.001)));
			t_sample smoothAmountHistoryNext = fixdenorm(smoothSmoothAmount);
			t_sample smoothAttack = ((m_atkHistory_48 * ((t_sample)0.999)) + (m_d_ATK_91 * ((t_sample)0.001)));
			t_sample atkHistoryNext = fixdenorm(smoothAttack);
			t_sample smoothRelease = ((m_relHistory_47 * ((t_sample)0.999)) + (m_e_REL_90 * ((t_sample)0.001)));
			t_sample relHistoryNext = fixdenorm(smoothRelease);
			t_sample smoothHpfFreq = ((m_hpfFreqHistory_52 * ((t_sample)0.999)) + (m_j_HPF_84 * ((t_sample)0.001)));
			t_sample hpfFreqHistoryNext = fixdenorm(smoothHpfFreq);
			t_sample smoothLpfFreq = ((m_lpfFreqHistory_51 * ((t_sample)0.999)) + (m_k_LPF_82 * ((t_sample)0.001)));
			t_sample lpfFreqHistoryNext = fixdenorm(smoothLpfFreq);
			t_sample smoothScEnable = ((m_scEnableHistory_50 * ((t_sample)0.999)) + (m_l_SC_80 * ((t_sample)0.001)));
			t_sample scEnableHistoryNext = fixdenorm(smoothScEnable);
			t_sample smoothSoftclip = ((m_softclipHistory_49 * ((t_sample)0.999)) + (m_u_SOFTCLIP_70 * ((t_sample)0.001)));
			t_sample softclipHistoryNext = fixdenorm(smoothSoftclip);
			t_sample smoothTrimGain = ((m_trimHistory_59 * ((t_sample)0.999)) + (m_a_TRIM_94 * ((t_sample)0.001)));
			t_sample trimGainLinear = dbtoa(smoothTrimGain);
			t_sample leftTrimmed = (in1 * trimGainLinear);
			t_sample rightTrimmed = (in2 * trimGainLinear);
			m_leftInputDelay_25.write(in1);
			m_rightInputDelay_24.write(in2);
			m_leftOutputDelay_27.write(leftTrimmed);
			m_rightOutputDelay_26.write(rightTrimmed);
			t_sample leftDelayedForCompression = m_leftOutputDelay_27.read_step(lookaheadSamples_211);
			t_sample rightDelayedForCompression = m_rightOutputDelay_26.read_step(lookaheadSamples_211);
			t_sample leftDelayedForMixing = m_leftInputDelay_25.read_step(lookaheadSamples_211);
			t_sample rightDelayedForMixing = m_rightInputDelay_24.read_step(lookaheadSamples_211);
			t_sample smoothMakeupGain = ((m_makeupGainHistory_64 * ((t_sample)0.999)) + (m_i_MAKEUP_89 * ((t_sample)0.001)));
			t_sample makeupGainHistoryNext = fixdenorm(smoothMakeupGain);
			t_sample sidechainDelayedTap = m_sidechainTapDelay_23.read_step(lookaheadSamples_211);
			t_sample smoothSoloSidechain = ((m_soloSidechainHistory_63 * ((t_sample)0.999)) + (m_m_SOLOSC_77 * ((t_sample)0.001)));
			t_sample soloSidechainHistoryNext = fixdenorm(smoothSoloSidechain);
			t_sample sidechainDelayedWrite = m_sidechainWriteDelay_22.read_step(lookaheadSamples_211);
			t_sample smoothBypassAmount = ((m_bypassAmountHistory_62 * ((t_sample)0.999)) + (bypassInverted * ((t_sample)0.001)));
			t_sample bypassAmountHistoryNext = fixdenorm(smoothBypassAmount);
			t_sample smoothKeyMix = ((m_keyMixHistory_61 * ((t_sample)0.999)) + (m_r_KEY_78 * ((t_sample)0.001)));
			t_sample keyMixHistoryNext = fixdenorm(smoothKeyMix);
			t_sample smoothDryWetMix = ((m_dryWetMixHistory_60 * ((t_sample)0.999)) + (m_o_DRYWET_76 * ((t_sample)0.001)));
			t_sample dryWetMixHistoryNext = fixdenorm(smoothDryWetMix);
			t_sample mix_285 = (smoothDryWetMix + (smoothDeltaMode * (((int)1) - smoothDryWetMix)));
			t_sample smoothTrimGain_212 = ((m_trimHistory_59 * ((t_sample)0.999)) + (m_a_TRIM_94 * ((t_sample)0.001)));
			t_sample trimHistoryNext_213 = fixdenorm(smoothTrimGain_212);
			t_sample trimGainLinear_214 = dbtoa(smoothTrimGain_212);
			t_sample leftTrimmed_215 = (in1 * trimGainLinear_214);
			t_sample rightTrimmed_216 = (in2 * trimGainLinear_214);
			t_sample mainSignalMono = ((rightTrimmed_216 + leftTrimmed_215) * ((t_sample)0.707));
			t_sample smoothSidechainTrimGain = ((m_sidechainTrimHistory_58 * ((t_sample)0.999)) + (m_y_SCTRIM_79 * ((t_sample)0.001)));
			t_sample sidechainTrimHistoryNext = fixdenorm(smoothSidechainTrimGain);
			t_sample sidechainTrimGainLinear = dbtoa(smoothSidechainTrimGain);
			t_sample sidechainTrimReference = sidechainTrimGainLinear;
			t_sample leftSidechainTrimmed = (in3 * sidechainTrimReference);
			t_sample rightSidechainTrimmed = (in4 * sidechainTrimReference);
			t_sample expr_233 = leftSidechainTrimmed;
			t_sample expr_234 = rightSidechainTrimmed;
			t_sample sidechainSignalMono = ((rightSidechainTrimmed + leftSidechainTrimmed) * ((t_sample)0.707));
			t_sample mix_286 = (mainSignalMono + (smoothKeyMix * (sidechainSignalMono - mainSignalMono)));
			t_sample keyMixedSignal = mix_286;
			t_sample ONSIDECHAIN = smoothScEnable;
			t_sample REACT = smoothReact;
			t_sample EXTRA_SMOOTH = smoothSmoothAmount;
			t_sample omega = safediv((smoothLpfFreq * ((t_sample)6.2831853071796)), samplerate);
			t_sample sn = sin(omega);
			t_sample cs = cos(omega);
			t_sample alpha = ((sn * ((t_sample)0.5)) * ((int)1));
			t_sample b0 = safediv(((int)1), (((int)1) + alpha));
			t_sample a2 = (((((int)1) - cs) * ((t_sample)0.5)) * b0);
			t_sample a1 = ((((int)1) - cs) * b0);
			t_sample b1 = ((((int)-2) * cs) * b0);
			t_sample b2 = ((((int)1) - alpha) * b0);
			t_sample lpfCoeffA2 = a2;
			t_sample lpfCoeffA1 = a1;
			t_sample lpfCoeffA0 = a2;
			t_sample lpfCoeffB1 = b1;
			t_sample lpfCoeffB2 = b2;
			t_sample omega_hpf = safediv((smoothHpfFreq * ((t_sample)6.2831853071796)), samplerate);
			t_sample sn_hpf = sin(omega_hpf);
			t_sample cs_hpf = cos(omega_hpf);
			t_sample alpha_hpf = ((sn_hpf * ((t_sample)0.5)) * ((t_sample)1));
			t_sample b0_hpf = safediv(((int)1), (((int)1) + alpha_hpf));
			t_sample a_hpf = (((((int)1) + cs_hpf) * ((t_sample)0.5)) * b0_hpf);
			t_sample a_neg_hpf = ((-(((int)1) + cs_hpf)) * b0_hpf);
			t_sample b1_hpf = ((((int)-2) * cs_hpf) * b0_hpf);
			t_sample b2_hpf = ((((int)1) - alpha_hpf) * b0_hpf);
			t_sample hpfCoeffA2 = a_hpf;
			t_sample hpfCoeffA1 = a_neg_hpf;
			t_sample hpfCoeffA0 = a_hpf;
			t_sample hpfCoeffB1 = b1_hpf;
			t_sample hpfCoeffB2 = b2_hpf;
			t_sample sidechainMixedSignal = (keyMixedSignal * ((t_sample)0.707));
			t_sample lpfInput = (sidechainMixedSignal * lpfCoeffA2);
			t_sample lpfFromHistory1 = (m_lpfHistory_44 * lpfCoeffA1);
			t_sample lpfFromHistory2 = (m_lpfHistory_46 * lpfCoeffA0);
			t_sample lpfFeedback1 = (m_lpfHistory_45 * lpfCoeffB2);
			t_sample lpfFeedback2 = (m_lpfHistory_43 * lpfCoeffB1);
			t_sample lpfFilteredSignal = (((lpfFromHistory2 + lpfFromHistory1) + lpfInput) - (lpfFeedback2 + lpfFeedback1));
			t_sample lpfHistory1Next = fixdenorm(m_lpfHistory_44);
			t_sample lpfHistory2Next = fixdenorm(m_lpfHistory_43);
			t_sample lpfHistory3Next = fixdenorm(sidechainMixedSignal);
			t_sample lpfHistory4Next = fixdenorm(lpfFilteredSignal);
			t_sample lpfStage2Input = (lpfFilteredSignal * lpfCoeffA2);
			t_sample lpfStage2FromHistory1 = (m_lpfHistory_40 * lpfCoeffA1);
			t_sample lpfStage2FromHistory2 = (m_lpfHistory_42 * lpfCoeffA0);
			t_sample lpfStage2Feedback1 = (m_lpfHistory_41 * lpfCoeffB2);
			t_sample lpfStage2Feedback2 = (m_lpfHistory_39 * lpfCoeffB1);
			t_sample lpfStage2FilteredSignal = (((lpfStage2FromHistory2 + lpfStage2FromHistory1) + lpfStage2Input) - (lpfStage2Feedback2 + lpfStage2Feedback1));
			t_sample lpfHistory5Next = fixdenorm(m_lpfHistory_40);
			t_sample lpfHistory6Next = fixdenorm(m_lpfHistory_39);
			t_sample lpfHistory7Next = fixdenorm(lpfFilteredSignal);
			t_sample lpfHistory8Next = fixdenorm(lpfStage2FilteredSignal);
			t_sample mix_287 = (lpfFilteredSignal + (smoothLpfOrder * (lpfStage2FilteredSignal - lpfFilteredSignal)));
			t_sample lpfFinalSignal = mix_287;
			t_sample mix_288 = (sidechainMixedSignal + (ONSIDECHAIN * (lpfFinalSignal - sidechainMixedSignal)));
			t_sample lpfMixedSignal = mix_288;
			t_sample hpfInput = (lpfMixedSignal * hpfCoeffA2);
			t_sample hpfFromHistory1 = (m_hpfHistory_36 * hpfCoeffA1);
			t_sample hpfFromHistory2 = (m_hpfHistory_37 * hpfCoeffA0);
			t_sample hpfFeedback1 = (m_hpfHistory_38 * hpfCoeffB2);
			t_sample hpfFeedback2 = (m_hpfHistory_35 * hpfCoeffB1);
			t_sample hpfFilteredSignal = (((hpfFromHistory2 + hpfFromHistory1) + hpfInput) - (hpfFeedback2 + hpfFeedback1));
			t_sample hpfHistory1Next = fixdenorm(m_hpfHistory_35);
			t_sample hpfHistory2Next = fixdenorm(m_hpfHistory_36);
			t_sample hpfHistory3Next = fixdenorm(lpfMixedSignal);
			t_sample hpfHistory4Next = fixdenorm(hpfFilteredSignal);
			t_sample hpfStage2Input = (hpfFilteredSignal * hpfCoeffA2);
			t_sample hpfStage2FromHistory1 = (m_hpfHistory_32 * hpfCoeffA1);
			t_sample hpfStage2FromHistory2 = (m_hpfHistory_33 * hpfCoeffA0);
			t_sample hpfStage2Feedback1 = (m_hpfHistory_34 * hpfCoeffB2);
			t_sample hpfStage2Feedback2 = (m_hpfHistory_31 * hpfCoeffB1);
			t_sample hpfStage2FilteredSignal = (((hpfStage2FromHistory2 + hpfStage2FromHistory1) + hpfStage2Input) - (hpfStage2Feedback2 + hpfStage2Feedback1));
			t_sample hpfHistory5Next = fixdenorm(m_hpfHistory_31);
			t_sample hpfHistory6Next = fixdenorm(m_hpfHistory_32);
			t_sample hpfHistory7Next = fixdenorm(hpfFilteredSignal);
			t_sample hpfHistory8Next = fixdenorm(hpfStage2FilteredSignal);
			t_sample mix_289 = (hpfFilteredSignal + (smoothHpfOrder * (hpfStage2FilteredSignal - hpfFilteredSignal)));
			t_sample hpfFinalSignal = mix_289;
			t_sample mix_290 = (lpfMixedSignal + (ONSIDECHAIN * (hpfFinalSignal - lpfMixedSignal)));
			t_sample sidechainProcessedSignal = mix_290;
			t_sample sidechainDetectionSignal = sidechainProcessedSignal;
			t_sample attack_ms = ((((t_sample)0.05) < smoothAttack) ? smoothAttack : ((t_sample)0.05));
			t_sample release_ms = ((((t_sample)0.1) < smoothRelease) ? smoothRelease : ((t_sample)0.1));
			t_sample attack_coeff = exp(safediv(((int)-1), ((attack_ms * samplerate) * ((t_sample)0.001))));
			t_sample current_signal_abs = fabs(sidechainDetectionSignal);
			t_sample signal_change = fabs((current_signal_abs - m_prevEnvelope_19));
			t_sample final_release_time = release_ms;
			if ((AUTO_RELEASE > ((t_sample)0.5))) {
				t_sample maxb_95 = (current_signal_abs * ((t_sample)0.3));
				t_sample relative_threshold = ((((t_sample)0.01) < maxb_95) ? maxb_95 : ((t_sample)0.01));
				int cond_96 = (signal_change > relative_threshold);
				int is_transient = (cond_96 ? ((int)1) : ((int)0));
				t_sample program_smooth = ((t_sample)0.995);
				if ((is_transient > ((t_sample)0.5))) {
					program_smooth = ((t_sample)0.95);
					
				};
				m_transientDetector_28 = ((m_transientDetector_28 * program_smooth) + (is_transient * (((int)1) - program_smooth)));
				t_sample transient_shaped = safepow(m_transientDetector_28, ((t_sample)1.5));
				t_sample mix_291 = (((int)500) + (transient_shaped * ((int)-485)));
				t_sample base_release = mix_291;
				t_sample sub_295 = (current_signal_abs - ((int)0));
				t_sample scale_292 = ((safepow((sub_295 * ((t_sample)2)), ((int)1)) * ((t_sample)-0.4)) + ((int)1));
				t_sample level_factor = scale_292;
				t_sample sub_299 = (signal_change - ((int)0));
				t_sample scale_296 = ((safepow((sub_299 * ((t_sample)5)), ((int)1)) * ((t_sample)-0.6)) + ((int)1));
				t_sample change_factor = scale_296;
				t_sample instant_mod = ((change_factor < level_factor) ? change_factor : level_factor);
				final_release_time = (base_release * instant_mod);
				if ((m_prevEnvelope_19 > ((int)0))) {
					t_sample max_change = (final_release_time * ((t_sample)0.7));
					t_sample min_97 = (m_prevEnvelope_19 - max_change);
					t_sample max_98 = (m_prevEnvelope_19 + max_change);
					final_release_time = ((final_release_time <= min_97) ? min_97 : ((final_release_time >= max_98) ? max_98 : final_release_time));
					
				};
				final_release_time = ((final_release_time <= ((int)10)) ? ((int)10) : ((final_release_time >= ((int)600)) ? ((int)600) : final_release_time));
				
			};
			t_sample release_coeff = exp(safediv(((int)-1), ((final_release_time * samplerate) * ((t_sample)0.001))));
			m_prevEnvelope_19 = current_signal_abs;
			t_sample input_squared = (sidechainDetectionSignal * sidechainDetectionSignal);
			t_sample oldest_sq = m_rmsDelay_21.read_step(rms_window);
			t_sample rms_sum_new = ((input_squared + m_rmsSum_20) - oldest_sq);
			t_sample rms_sum_clipped = ((((int)0) < rms_sum_new) ? rms_sum_new : ((int)0));
			t_sample rms_value_sharp = sqrt((rms_sum_clipped * rms_window_inv));
			m_rmsSum_20 = rms_sum_clipped;
			m_rmsDelay_21.write(input_squared);
			m_peakEnvSharp_18 = fabs(sidechainDetectionSignal);
			t_sample mix_300 = (m_peakEnvSharp_18 + (REACT * (rms_value_sharp - m_peakEnvSharp_18)));
			t_sample env_mix_sharp = mix_300;
			t_sample release_env_175 = (m_releaseHistSharp_17 * release_coeff);
			t_sample env_post_release_176 = ((env_mix_sharp < release_env_175) ? release_env_175 : env_mix_sharp);
			t_sample attack_env_177 = (m_attackHistSharp_16 * attack_coeff);
			t_sample attack_mix_178 = (((((int)1) - attack_coeff) * env_post_release_176) + (attack_coeff * attack_env_177));
			t_sample sharpEnvelopeFinal = ((attack_mix_178 < ((t_sample)1e-06)) ? ((t_sample)1e-06) : attack_mix_178);
			m_releaseHistSharp_17 = fixdenorm(env_post_release_176);
			m_attackHistSharp_16 = fixdenorm(attack_mix_178);
			t_sample input_abs_classic = fabs(sidechainDetectionSignal);
			if ((input_abs_classic > m_peakEnvClassic_14)) {
				m_peakEnvClassic_14 = (((((int)1) - attack_coeff) * input_abs_classic) + (attack_coeff * m_peakEnvClassic_14));
				
			} else {
				m_peakEnvClassic_14 = (((((int)1) - release_coeff) * input_abs_classic) + (release_coeff * m_peakEnvClassic_14));
				
			};
			t_sample input_sq_classic = (sidechainDetectionSignal * sidechainDetectionSignal);
			m_rmsSqClassic_13 = ((m_rmsSqClassic_13 * rms_coeff) + (input_sq_classic * (((int)1) - rms_coeff)));
			t_sample rms_env_classic_raw = sqrt(m_rmsSqClassic_13);
			if ((rms_env_classic_raw > m_histRmsClassic_12)) {
				m_histRmsClassic_12 = (((((int)1) - attack_coeff) * rms_env_classic_raw) + (attack_coeff * m_histRmsClassic_12));
				
			} else {
				m_histRmsClassic_12 = (((((int)1) - release_coeff) * rms_env_classic_raw) + (release_coeff * m_histRmsClassic_12));
				
			};
			t_sample mix_301 = (m_peakEnvClassic_14 + (REACT * (m_histRmsClassic_12 - m_peakEnvClassic_14)));
			t_sample env_mix_classic = mix_301;
			t_sample input_abs_slow = fabs(sidechainDetectionSignal);
			if ((input_abs_slow > m_peakEnvSlow_8)) {
				m_peakEnvSlow_8 = (((((int)1) - attack_coeff) * input_abs_slow) + (attack_coeff * m_peakEnvSlow_8));
				
			} else {
				m_peakEnvSlow_8 = (((((int)1) - release_coeff) * input_abs_slow) + (release_coeff * m_peakEnvSlow_8));
				
			};
			t_sample input_sq_slow = (sidechainDetectionSignal * sidechainDetectionSignal);
			m_rmsSqSlow_7 = ((m_rmsSqSlow_7 * rms_coeff_slow) + (input_sq_slow * (((int)1) - rms_coeff_slow)));
			t_sample rms_env_slow_raw = sqrt(m_rmsSqSlow_7);
			if ((rms_env_slow_raw > m_histRmsSlow_6)) {
				m_histRmsSlow_6 = (((((int)1) - attack_coeff) * rms_env_slow_raw) + (attack_coeff * m_histRmsSlow_6));
				
			} else {
				m_histRmsSlow_6 = (((((int)1) - release_coeff) * rms_env_slow_raw) + (release_coeff * m_histRmsSlow_6));
				
			};
			t_sample mix_302 = (m_peakEnvSlow_8 + (REACT * (m_histRmsSlow_6 - m_peakEnvSlow_8)));
			t_sample env_mix_slow = mix_302;
			t_sample smooth_amount_shaped = safepow(EXTRA_SMOOTH, ((t_sample)2.5));
			t_sample mix_303 = (((t_sample)0.001) + (smooth_amount_shaped * ((t_sample)99.999)));
			t_sample smooth_time_ms = mix_303;
			t_sample extra_smooth_coeff = exp(safediv(((int)-1), ((smooth_time_ms * samplerate) * ((t_sample)0.001))));
			t_sample expr_algo0 = atodb(sharpEnvelopeFinal);
			if ((EXTRA_SMOOTH >= ((t_sample)0.001))) {
				t_sample extraSmoothedSharpEnv = ((m_extraSmoothHistSharp_15 * extra_smooth_coeff) + (sharpEnvelopeFinal * (((int)1) - extra_smooth_coeff)));
				m_extraSmoothHistSharp_15 = fixdenorm(extraSmoothedSharpEnv);
				expr_algo0 = atodb(((extraSmoothedSharpEnv < ((t_sample)1e-06)) ? ((t_sample)1e-06) : extraSmoothedSharpEnv));
				
			};
			t_sample expr_algo1 = atodb(((env_mix_classic < ((t_sample)1e-06)) ? ((t_sample)1e-06) : env_mix_classic));
			if ((EXTRA_SMOOTH >= ((t_sample)0.001))) {
				t_sample extraSmoothedClassicEnv = ((m_extraSmoothHistClassic_9 * extra_smooth_coeff) + (env_mix_classic * (((int)1) - extra_smooth_coeff)));
				m_extraSmoothHistClassic_9 = fixdenorm(extraSmoothedClassicEnv);
				expr_algo1 = atodb(((extraSmoothedClassicEnv < ((t_sample)1e-06)) ? ((t_sample)1e-06) : extraSmoothedClassicEnv));
				
			};
			t_sample expr_algo2 = atodb(((env_mix_slow < ((t_sample)1e-06)) ? ((t_sample)1e-06) : env_mix_slow));
			if ((EXTRA_SMOOTH >= ((t_sample)0.001))) {
				t_sample extraSmoothedSlowEnv = ((m_extraSmoothHistSlow_3 * extra_smooth_coeff) + (env_mix_slow * (((int)1) - extra_smooth_coeff)));
				m_extraSmoothHistSlow_3 = fixdenorm(extraSmoothedSlowEnv);
				expr_algo2 = atodb(((extraSmoothedSlowEnv < ((t_sample)1e-06)) ? ((t_sample)1e-06) : extraSmoothedSlowEnv));
				
			};
			t_sample mix_304 = (expr_algo0 + (detector_control * (expr_algo1 - expr_algo0)));
			t_sample mix_305 = (expr_algo1 + (fade_amount * (expr_algo2 - expr_algo1)));
			t_sample envelope_out = (cond_100 ? mix_304 : mix_305);
			t_sample envelopeDetectorOutput = envelope_out;
			t_sample finalEnvelopeDb = envelopeDetectorOutput;
			t_sample trimCompensationFactor = safediv(((int)1), trimGainLinear_214);
			t_sample sidechainTrimCompensated = (sidechainDetectionSignal * trimCompensationFactor);
			t_sample compressionGainReduction = softkneeLinear_d_d_d_d(finalEnvelopeDb, smoothThresholdDb, smoothRatio, smoothKnee);
			t_sample gainReductionDb = (compressionGainReduction - finalEnvelopeDb);
			t_sample gainReductionLinear = dbtoa(gainReductionDb);
			t_sample compressedLeft = (leftDelayedForCompression * gainReductionLinear);
			t_sample delta_control = ((smoothDeltaMode <= ((int)0)) ? ((int)0) : ((smoothDeltaMode >= ((int)1)) ? ((int)1) : smoothDeltaMode));
			t_sample delta_signal = (leftDelayedForCompression - compressedLeft);
			t_sample mix_306 = (compressedLeft + (delta_control * (delta_signal - compressedLeft)));
			t_sample autoMakeupGainReduction = gainReductionDb;
			t_sample reduction_smooth_mul = (m_reductionHistLeft_2 * ((t_sample)0.999));
			t_sample reduction_current_mul = (autoMakeupGainReduction * ((t_sample)0.001));
			t_sample reduction_smoothed = (reduction_current_mul + reduction_smooth_mul);
			t_sample auto_makeup = (reduction_smoothed * ((t_sample)-0.7));
			t_sample combined_makeup = (auto_makeup + smoothMakeupGain);
			t_sample mix_307 = (smoothMakeupGain + (mix_283 * (combined_makeup - smoothMakeupGain)));
			t_sample mix_308 = (mix_307 + (smoothDeltaMode * (((int)0) - mix_307)));
			t_sample makeup_linear = dbtoa(mix_308);
			t_sample leftWithMakeup = (mix_306 * makeup_linear);
			m_reductionHistLeft_2 = fixdenorm(reduction_smoothed);
			t_sample parallel_control = ((mix_284 <= ((int)0)) ? ((int)0) : ((mix_284 >= ((int)1)) ? ((int)1) : mix_284));
			t_sample parallel_compressed = (leftWithMakeup * parallel_control);
			t_sample parallel_mix = (leftDelayedForCompression + parallel_compressed);
			t_sample mix_309 = (leftWithMakeup + (parallel_control * (parallel_mix - leftWithMakeup)));
			t_sample mix_310 = (leftDelayedForMixing + (mix_285 * (mix_309 - leftDelayedForMixing)));
			t_sample mix_311 = (mix_310 + (smoothSoloSidechain * (sidechainDelayedWrite - mix_310)));
			t_sample leftWithSidechain = mix_311;
			t_sample saturationAmount = ((smoothSoftclip <= ((int)0)) ? ((int)0) : ((smoothSoftclip >= ((int)1)) ? ((int)1) : smoothSoftclip));
			t_sample mix_312 = (saturationAmount + (smoothDeltaMode * (((int)0) - saturationAmount)));
			t_sample effectiveSaturationAmount_218 = (mix_312 * mix_285);
			t_sample effectiveSaturationAmount_219 = ((effectiveSaturationAmount_218 <= ((int)0)) ? ((int)0) : ((effectiveSaturationAmount_218 >= ((int)1)) ? ((int)1) : effectiveSaturationAmount_218));
			t_sample leftSaturated = ((int)0);
			if ((leftWithSidechain > ((int)0))) {
				leftSaturated = (tanh((leftWithSidechain * ((t_sample)1.2))) * ((t_sample)0.833));
				
			} else {
				leftSaturated = (tanh((leftWithSidechain * ((t_sample)0.8))) * ((t_sample)1.25));
				
			};
			t_sample leftSaturated_220 = ((leftSaturated <= ((t_sample)-0.989)) ? ((t_sample)-0.989) : ((leftSaturated >= ((t_sample)0.989)) ? ((t_sample)0.989) : leftSaturated));
			t_sample mix_313 = (leftWithSidechain + (effectiveSaturationAmount_219 * (leftSaturated_220 - leftWithSidechain)));
			t_sample mix_314 = (leftDelayedForMixing + (smoothBypassAmount * (mix_313 - leftDelayedForMixing)));
			t_sample rightGainReductionDb = (compressionGainReduction - finalEnvelopeDb);
			t_sample rightGainReductionLinear = dbtoa(rightGainReductionDb);
			t_sample compressedRight = (rightDelayedForCompression * rightGainReductionLinear);
			t_sample rightDeltaControl = ((smoothDeltaMode <= ((int)0)) ? ((int)0) : ((smoothDeltaMode >= ((int)1)) ? ((int)1) : smoothDeltaMode));
			t_sample rightDeltaSignal = (rightDelayedForCompression - compressedRight);
			t_sample mix_315 = (compressedRight + (rightDeltaControl * (rightDeltaSignal - compressedRight)));
			t_sample rightAutoMakeupGainReduction = rightGainReductionDb;
			t_sample rightReductionSmoothMul = (m_reductionHistRight_1 * ((t_sample)0.999));
			t_sample rightReductionCurrentMul = (rightAutoMakeupGainReduction * ((t_sample)0.001));
			t_sample rightReductionSmoothed = (rightReductionCurrentMul + rightReductionSmoothMul);
			t_sample rightAutoMakeup = (rightReductionSmoothed * ((t_sample)-0.7));
			t_sample rightCombinedMakeup = (rightAutoMakeup + smoothMakeupGain);
			t_sample mix_316 = (smoothMakeupGain + (mix_283 * (rightCombinedMakeup - smoothMakeupGain)));
			t_sample mix_317 = (mix_316 + (smoothDeltaMode * (((int)0) - mix_316)));
			t_sample rightMakeupLinear = dbtoa(mix_317);
			t_sample rightWithMakeup = (mix_315 * rightMakeupLinear);
			m_reductionHistRight_1 = fixdenorm(rightReductionSmoothed);
			t_sample rightParallelControl = ((mix_284 <= ((int)0)) ? ((int)0) : ((mix_284 >= ((int)1)) ? ((int)1) : mix_284));
			t_sample rightParallelCompressed = (rightWithMakeup * rightParallelControl);
			t_sample rightParallelMix = (rightDelayedForCompression + rightParallelCompressed);
			t_sample mix_318 = (rightWithMakeup + (rightParallelControl * (rightParallelMix - rightWithMakeup)));
			t_sample mix_319 = (rightDelayedForMixing + (mix_285 * (mix_318 - rightDelayedForMixing)));
			t_sample mix_320 = (mix_319 + (smoothSoloSidechain * (sidechainDelayedTap - mix_319)));
			t_sample rightWithSidechain = mix_320;
			t_sample rightSaturationAmount = ((smoothSoftclip <= ((int)0)) ? ((int)0) : ((smoothSoftclip >= ((int)1)) ? ((int)1) : smoothSoftclip));
			t_sample mix_321 = (rightSaturationAmount + (smoothDeltaMode * (((int)0) - rightSaturationAmount)));
			t_sample rightEffectiveSaturationAmount_222 = ((mix_321 * mix_285) * (((int)1) - smoothSoloSidechain));
			t_sample rightEffectiveSaturationAmount_223 = ((rightEffectiveSaturationAmount_222 <= ((int)0)) ? ((int)0) : ((rightEffectiveSaturationAmount_222 >= ((int)1)) ? ((int)1) : rightEffectiveSaturationAmount_222));
			t_sample rightSaturated = ((int)0);
			if ((rightWithSidechain > ((int)0))) {
				rightSaturated = (tanh((rightWithSidechain * ((t_sample)1.2))) * ((t_sample)0.833));
				
			} else {
				rightSaturated = (tanh((rightWithSidechain * ((t_sample)0.8))) * ((t_sample)1.25));
				
			};
			t_sample rightSaturated_224 = ((rightSaturated <= ((t_sample)-0.989)) ? ((t_sample)-0.989) : ((rightSaturated >= ((t_sample)0.989)) ? ((t_sample)0.989) : rightSaturated));
			t_sample mix_322 = (rightWithSidechain + (rightEffectiveSaturationAmount_223 * (rightSaturated_224 - rightWithSidechain)));
			t_sample mix_323 = (rightDelayedForMixing + (smoothBypassAmount * (mix_322 - rightDelayedForMixing)));
			t_sample mix_324 = (((int)1) + (bypassInverted * (rightGainReductionLinear - ((int)1))));
			t_sample gainReductionOutput = ((((int)1) < mix_324) ? ((int)1) : mix_324);
			m_parallelMixHistory_68 = parallelMixHistoryNext;
			m_deltaModeHistory_67 = deltaModeHistoryNext;
			m_autoGainHistory_66 = autoGainHistoryNext;
			m_thresholdHistory_65 = thresholdHistoryNext;
			m_makeupGainHistory_64 = makeupGainHistoryNext;
			m_soloSidechainHistory_63 = soloSidechainHistoryNext;
			m_bypassAmountHistory_62 = bypassAmountHistoryNext;
			m_keyMixHistory_61 = keyMixHistoryNext;
			m_dryWetMixHistory_60 = dryWetMixHistoryNext;
			m_trimHistory_59 = trimHistoryNext_213;
			m_sidechainTrimHistory_58 = sidechainTrimHistoryNext;
			m_ratioHistory_57 = ratioHistoryNext;
			m_kneeHistory_56 = kneeHistoryNext;
			m_reactHistory_55 = reactHistoryNext;
			m_smoothAmountHistory_54 = smoothAmountHistoryNext;
			m_lookaheadHistory_53 = lookaheadHistoryNext;
			m_hpfFreqHistory_52 = hpfFreqHistoryNext;
			m_lpfFreqHistory_51 = lpfFreqHistoryNext;
			m_scEnableHistory_50 = scEnableHistoryNext;
			m_softclipHistory_49 = softclipHistoryNext;
			m_atkHistory_48 = atkHistoryNext;
			m_relHistory_47 = relHistoryNext;
			m_lpfHistory_46 = lpfHistory1Next;
			m_lpfHistory_45 = lpfHistory2Next;
			m_lpfHistory_44 = lpfHistory3Next;
			m_lpfHistory_43 = lpfHistory4Next;
			m_lpfHistory_42 = lpfHistory5Next;
			m_lpfHistory_41 = lpfHistory6Next;
			m_lpfHistory_40 = lpfHistory7Next;
			m_lpfHistory_39 = lpfHistory8Next;
			m_hpfHistory_38 = hpfHistory1Next;
			m_hpfHistory_37 = hpfHistory2Next;
			m_hpfHistory_36 = hpfHistory3Next;
			m_hpfHistory_35 = hpfHistory4Next;
			m_hpfHistory_34 = hpfHistory5Next;
			m_hpfHistory_33 = hpfHistory6Next;
			m_hpfHistory_32 = hpfHistory7Next;
			m_hpfHistory_31 = hpfHistory8Next;
			m_hpfOrderHistory_30 = hpfOrderHistoryNext;
			m_lpfOrderHistory_29 = lpfOrderHistoryNext;
			m_sidechainWriteDelay_22.write(sidechainTrimCompensated);
			m_sidechainTapDelay_23.write(sidechainTrimCompensated);
			t_sample out4 = leftDelayedForCompression;
			t_sample out1 = mix_314;
			t_sample out5 = rightDelayedForCompression;
			t_sample out2 = mix_323;
			t_sample out6 = expr_233;
			t_sample out3 = gainReductionOutput;
			t_sample out7 = expr_234;
			m_rmsDelay_21.step();
			m_sidechainWriteDelay_22.step();
			m_sidechainTapDelay_23.step();
			m_rightInputDelay_24.step();
			m_leftInputDelay_25.step();
			m_rightOutputDelay_26.step();
			m_leftOutputDelay_27.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			(*(__out3++)) = out3;
			(*(__out4++)) = out4;
			(*(__out5++)) = out5;
			(*(__out6++)) = out6;
			(*(__out7++)) = out7;
			
		};
		return __exception;
		
	};
	inline void set_p_BYPASS(t_param _value) {
		m_p_BYPASS_69 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_u_SOFTCLIP(t_param _value) {
		m_u_SOFTCLIP_70 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_x_AUTOGAIN(t_param _value) {
		m_x_AUTOGAIN_71 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_s_AUTORELEASE(t_param _value) {
		m_s_AUTORELEASE_72 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_n_LOOKAHEAD(t_param _value) {
		m_n_LOOKAHEAD_73 = (_value < 0 ? 0 : (_value > 10 ? 10 : _value));
	};
	inline void set_v_DELTA(t_param _value) {
		m_v_DELTA_74 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_w_PARALLEL(t_param _value) {
		m_w_PARALLEL_75 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_o_DRYWET(t_param _value) {
		m_o_DRYWET_76 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_m_SOLOSC(t_param _value) {
		m_m_SOLOSC_77 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_r_KEY(t_param _value) {
		m_r_KEY_78 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_y_SCTRIM(t_param _value) {
		m_y_SCTRIM_79 = (_value < -12 ? -12 : (_value > 12 ? 12 : _value));
	};
	inline void set_l_SC(t_param _value) {
		m_l_SC_80 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_k_LPFORDER(t_param _value) {
		m_k_LPFORDER_81 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_k_LPF(t_param _value) {
		m_k_LPF_82 = (_value < 20 ? 20 : (_value > 20000 ? 20000 : _value));
	};
	inline void set_j_HPFORDER(t_param _value) {
		m_j_HPFORDER_83 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_j_HPF(t_param _value) {
		m_j_HPF_84 = (_value < 20 ? 20 : (_value > 20000 ? 20000 : _value));
	};
	inline void set_z_SMOOTH(t_param _value) {
		m_z_SMOOTH_85 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_g_REACT(t_param _value) {
		m_g_REACT_86 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_f_ALGO(t_param _value) {
		m_f_ALGO_87 = (_value < 0 ? 0 : (_value > 2 ? 2 : _value));
	};
	inline void set_h_KNEE(t_param _value) {
		m_h_KNEE_88 = (_value < 0 ? 0 : (_value > 30 ? 30 : _value));
	};
	inline void set_i_MAKEUP(t_param _value) {
		m_i_MAKEUP_89 = (_value < -12 ? -12 : (_value > 12 ? 12 : _value));
	};
	inline void set_e_REL(t_param _value) {
		m_e_REL_90 = (_value < 0.1 ? 0.1 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_d_ATK(t_param _value) {
		m_d_ATK_91 = (_value < 0.1 ? 0.1 : (_value > 250 ? 250 : _value));
	};
	inline void set_c_RATIO(t_param _value) {
		m_c_RATIO_92 = (_value < 1 ? 1 : (_value > 20 ? 20 : _value));
	};
	inline void set_b_THD(t_param _value) {
		m_b_THD_93 = (_value < -60 ? -60 : (_value > 0 ? 0 : _value));
	};
	inline void set_a_TRIM(t_param _value) {
		m_a_TRIM_94 = (_value < -12 ? -12 : (_value > 12 ? 12 : _value));
	};
	inline t_sample softkneeLinear_d_d_d_d(t_sample xg, t_sample T, t_sample R, t_sample W) {
		t_sample _softkneeLinear_ret_1 = ((int)0);
		if (((((int)2) * (xg - T)) < (W * (-((int)1))))) {
			_softkneeLinear_ret_1 = xg;
			
		} else {
			if (((((int)2) * fabs((xg - T))) <= W)) {
				_softkneeLinear_ret_1 = ((T - (W * ((t_sample)0.5))) + ((((xg - T) + (W * ((t_sample)0.5))) * (((int)1) + safediv(((int)1), R))) * ((t_sample)0.5)));
				
			} else {
				if (((((int)2) * (xg - T)) > W)) {
					_softkneeLinear_ret_1 = (T + safediv((xg - T), R));
					
				};
				
			};
			
		};
		return _softkneeLinear_ret_1;
		
	};
	
} State;


///
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs

int gen_kernel_numins = 4;
int gen_kernel_numouts = 7;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 26; }

/// Assistive lables for the signal inputs and outputs

const char *gen_kernel_innames[] = { "in1", "in2", "in3", "in4" };
const char *gen_kernel_outnames[] = { "out1", "out2", "out3", "out4", "out5", "out6", "out7" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) {
	State* self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) {
	State* self = (State *)cself;
	self->reset(cself->sr, cself->vs);
}

/// Set a parameter of a State object

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State *self = (State *)cself;
	switch (index) {
		case 0: self->set_a_TRIM(value); break;
		case 1: self->set_b_THD(value); break;
		case 2: self->set_c_RATIO(value); break;
		case 3: self->set_d_ATK(value); break;
		case 4: self->set_e_REL(value); break;
		case 5: self->set_f_ALGO(value); break;
		case 6: self->set_g_REACT(value); break;
		case 7: self->set_h_KNEE(value); break;
		case 8: self->set_i_MAKEUP(value); break;
		case 9: self->set_j_HPF(value); break;
		case 10: self->set_j_HPFORDER(value); break;
		case 11: self->set_k_LPF(value); break;
		case 12: self->set_k_LPFORDER(value); break;
		case 13: self->set_l_SC(value); break;
		case 14: self->set_m_SOLOSC(value); break;
		case 15: self->set_n_LOOKAHEAD(value); break;
		case 16: self->set_o_DRYWET(value); break;
		case 17: self->set_p_BYPASS(value); break;
		case 18: self->set_r_KEY(value); break;
		case 19: self->set_s_AUTORELEASE(value); break;
		case 20: self->set_u_SOFTCLIP(value); break;
		case 21: self->set_v_DELTA(value); break;
		case 22: self->set_w_PARALLEL(value); break;
		case 23: self->set_x_AUTOGAIN(value); break;
		case 24: self->set_y_SCTRIM(value); break;
		case 25: self->set_z_SMOOTH(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_a_TRIM_94; break;
		case 1: *value = self->m_b_THD_93; break;
		case 2: *value = self->m_c_RATIO_92; break;
		case 3: *value = self->m_d_ATK_91; break;
		case 4: *value = self->m_e_REL_90; break;
		case 5: *value = self->m_f_ALGO_87; break;
		case 6: *value = self->m_g_REACT_86; break;
		case 7: *value = self->m_h_KNEE_88; break;
		case 8: *value = self->m_i_MAKEUP_89; break;
		case 9: *value = self->m_j_HPF_84; break;
		case 10: *value = self->m_j_HPFORDER_83; break;
		case 11: *value = self->m_k_LPF_82; break;
		case 12: *value = self->m_k_LPFORDER_81; break;
		case 13: *value = self->m_l_SC_80; break;
		case 14: *value = self->m_m_SOLOSC_77; break;
		case 15: *value = self->m_n_LOOKAHEAD_73; break;
		case 16: *value = self->m_o_DRYWET_76; break;
		case 17: *value = self->m_p_BYPASS_69; break;
		case 18: *value = self->m_r_KEY_78; break;
		case 19: *value = self->m_s_AUTORELEASE_72; break;
		case 20: *value = self->m_u_SOFTCLIP_70; break;
		case 21: *value = self->m_v_DELTA_74; break;
		case 22: *value = self->m_w_PARALLEL_75; break;
		case 23: *value = self->m_x_AUTOGAIN_71; break;
		case 24: *value = self->m_y_SCTRIM_79; break;
		case 25: *value = self->m_z_SMOOTH_85; break;
		
		default: break;
	}
}

/// Get the name of a parameter of a State object

const char *getparametername(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].name;
	}
	return 0;
}

/// Get the minimum value of a parameter of a State object

t_param getparametermin(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmin;
	}
	return 0;
}

/// Get the maximum value of a parameter of a State object

t_param getparametermax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmax;
	}
	return 0;
}

/// Get parameter of a State object has a minimum and maximum value

char getparameterhasminmax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].hasminmax;
	}
	return 0;
}

/// Get the units of a parameter of a State object

const char *getparameterunits(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].units;
	}
	return 0;
}

/// Get the size of the state of all parameters of a State object

size_t getstatesize(CommonState *cself) {
	return genlib_getstatesize(cself, &getparameter);
}

/// Get the state of all parameters of a State object

short getstate(CommonState *cself, char *state) {
	return genlib_getstate(cself, state, &getparameter);
}

/// set the state of all parameters of a State object

short setstate(CommonState *cself, const char *state) {
	return genlib_setstate(cself, state, &setparameter);
}

/// Allocate and configure a new State object and it's internal CommonState:

void *create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(26 * sizeof(ParamInfo));
	self->__commonstate.numparams = 26;
	// initialize parameter 0 ("m_a_TRIM_94")
	pi = self->__commonstate.params + 0;
	pi->name = "a_TRIM";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_a_TRIM_94;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -12;
	pi->outputmax = 12;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_b_THD_93")
	pi = self->__commonstate.params + 1;
	pi->name = "b_THD";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_b_THD_93;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -60;
	pi->outputmax = 0;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_c_RATIO_92")
	pi = self->__commonstate.params + 2;
	pi->name = "c_RATIO";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_c_RATIO_92;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 20;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_d_ATK_91")
	pi = self->__commonstate.params + 3;
	pi->name = "d_ATK";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_d_ATK_91;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 250;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_e_REL_90")
	pi = self->__commonstate.params + 4;
	pi->name = "e_REL";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_e_REL_90;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_f_ALGO_87")
	pi = self->__commonstate.params + 5;
	pi->name = "f_ALGO";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_f_ALGO_87;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_g_REACT_86")
	pi = self->__commonstate.params + 6;
	pi->name = "g_REACT";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_g_REACT_86;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_h_KNEE_88")
	pi = self->__commonstate.params + 7;
	pi->name = "h_KNEE";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_h_KNEE_88;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 30;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 8 ("m_i_MAKEUP_89")
	pi = self->__commonstate.params + 8;
	pi->name = "i_MAKEUP";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_i_MAKEUP_89;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -12;
	pi->outputmax = 12;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 9 ("m_j_HPF_84")
	pi = self->__commonstate.params + 9;
	pi->name = "j_HPF";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_j_HPF_84;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 20;
	pi->outputmax = 20000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 10 ("m_j_HPFORDER_83")
	pi = self->__commonstate.params + 10;
	pi->name = "j_HPFORDER";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_j_HPFORDER_83;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 11 ("m_k_LPF_82")
	pi = self->__commonstate.params + 11;
	pi->name = "k_LPF";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_k_LPF_82;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 20;
	pi->outputmax = 20000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 12 ("m_k_LPFORDER_81")
	pi = self->__commonstate.params + 12;
	pi->name = "k_LPFORDER";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_k_LPFORDER_81;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 13 ("m_l_SC_80")
	pi = self->__commonstate.params + 13;
	pi->name = "l_SC";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_l_SC_80;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 14 ("m_m_SOLOSC_77")
	pi = self->__commonstate.params + 14;
	pi->name = "m_SOLOSC";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_m_SOLOSC_77;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 15 ("m_n_LOOKAHEAD_73")
	pi = self->__commonstate.params + 15;
	pi->name = "n_LOOKAHEAD";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_n_LOOKAHEAD_73;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 10;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 16 ("m_o_DRYWET_76")
	pi = self->__commonstate.params + 16;
	pi->name = "o_DRYWET";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_o_DRYWET_76;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 17 ("m_p_BYPASS_69")
	pi = self->__commonstate.params + 17;
	pi->name = "p_BYPASS";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_p_BYPASS_69;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 18 ("m_r_KEY_78")
	pi = self->__commonstate.params + 18;
	pi->name = "r_KEY";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_r_KEY_78;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 19 ("m_s_AUTORELEASE_72")
	pi = self->__commonstate.params + 19;
	pi->name = "s_AUTORELEASE";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_s_AUTORELEASE_72;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 20 ("m_u_SOFTCLIP_70")
	pi = self->__commonstate.params + 20;
	pi->name = "u_SOFTCLIP";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_u_SOFTCLIP_70;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 21 ("m_v_DELTA_74")
	pi = self->__commonstate.params + 21;
	pi->name = "v_DELTA";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_v_DELTA_74;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 22 ("m_w_PARALLEL_75")
	pi = self->__commonstate.params + 22;
	pi->name = "w_PARALLEL";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_w_PARALLEL_75;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 23 ("m_x_AUTOGAIN_71")
	pi = self->__commonstate.params + 23;
	pi->name = "x_AUTOGAIN";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_x_AUTOGAIN_71;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 24 ("m_y_SCTRIM_79")
	pi = self->__commonstate.params + 24;
	pi->name = "y_SCTRIM";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_y_SCTRIM_79;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -12;
	pi->outputmax = 12;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 25 ("m_z_SMOOTH_85")
	pi = self->__commonstate.params + 25;
	pi->name = "z_SMOOTH";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_z_SMOOTH_85;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) {
	State *self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self;
}


} // JCBCompressor::
