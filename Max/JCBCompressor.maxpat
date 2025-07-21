{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 9,
			"minor" : 0,
			"revision" : 7,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 34.0, 88.0, 1852.0, 903.0 ],
		"gridsize" : [ 15.0, 15.0 ],
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-185",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 515.0, 539.0, 73.0, 20.0 ],
					"text" : "GenExpr",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"arrow_orientation" : 1,
					"grad1" : [ 0.537254901960784, 0.427450980392157, 0.427450980392157, 1.0 ],
					"grad2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 1.0 ],
					"id" : "obj-183",
					"maxclass" : "panel",
					"mode" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 470.0, 538.5, 43.0, 21.0 ],
					"proportion" : 0.5,
					"shape" : 3
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 13.0,
					"id" : "obj-182",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1523.0, 107.397600000000011, 72.0, 21.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1052.0, 53.0, 72.0, 21.0 ],
					"text" : "TOOLTIPS",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-180",
					"interval" : 20.0,
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 36.0, 738.0, 65.0, 22.0 ],
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-179",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 36.0, 706.0, 46.0, 22.0 ],
					"text" : "atodb~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-178",
					"linecount" : 4,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 36.0, 585.0, 55.0, 62.0 ],
					"text" : "average~ 1000 @mode absolute"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-177",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 251.0, 705.0, 46.0, 22.0 ],
					"text" : "atodb~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-176",
					"linecount" : 4,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 251.0, 585.0, 57.0, 62.0 ],
					"text" : "average~ 480 @mode absolute"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-173",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 480.0, 705.0, 46.0, 22.0 ],
					"text" : "atodb~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-172",
					"linecount" : 4,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 480.0, 585.0, 58.0, 62.0 ],
					"text" : "average~ 1000 @mode absolute"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-162",
					"interval" : 20.0,
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 480.0, 738.0, 65.0, 22.0 ],
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"forceaspect" : 1,
					"id" : "obj-159",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 795.0, 789.0, 256.0, 29.172093023255815 ],
					"pic" : "/Users/jcb/Desktop/Captura de pantalla 2025-07-21 a las 12.06.29.png",
					"presentation" : 1,
					"presentation_rect" : [ 34.0, 37.0, 457.0, 52.076744186046511 ]
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"forceaspect" : 1,
					"id" : "obj-158",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 1519.0, 787.0, 163.0, 30.595469255663428 ],
					"pic" : "/Users/jcb/Desktop/Captura de pantalla 2025-07-21 a las 12.06.21.png",
					"presentation" : 1,
					"presentation_rect" : [ 884.0, 316.0, 344.0, 64.569579288025892 ]
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"forceaspect" : 1,
					"id" : "obj-157",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 1289.0, 789.0, 219.0, 28.441558441558438 ],
					"pic" : "/Users/jcb/Desktop/Captura de pantalla 2025-07-21 a las 12.06.17.png",
					"presentation" : 1,
					"presentation_rect" : [ 42.0, 321.0, 430.0, 55.844155844155836 ]
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 19.0,
					"id" : "obj-156",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1037.0, 820.0, 258.0, 28.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 549.5, 352.569579288025921, 258.0, 28.0 ],
					"text" : "JCBCompresssor 0.9.99 beta",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.149019607843137, 0.149019607843137, 0.149019607843137, 0.0 ],
					"cantchange" : 1,
					"id" : "obj-155",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1394.0, 263.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 806.0, 52.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ -24 ],
							"parameter_initial_enable" : 1,
							"parameter_invisible" : 1,
							"parameter_longname" : "number[14]",
							"parameter_modmode" : 0,
							"parameter_shortname" : "number[14]",
							"parameter_type" : 3
						}

					}
,
					"triangle" : 0,
					"varname" : "number[14]"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.149019607843137, 0.149019607843137, 0.149019607843137, 0.0 ],
					"cantchange" : 1,
					"id" : "obj-154",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1004.0, 263.0, 50.0, 22.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 520.0, 52.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ -12 ],
							"parameter_initial_enable" : 1,
							"parameter_invisible" : 1,
							"parameter_longname" : "number[13]",
							"parameter_modmode" : 0,
							"parameter_shortname" : "number[13]",
							"parameter_type" : 3
						}

					}
,
					"triangle" : 0,
					"varname" : "number[13]"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgcolor2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_angle" : 270.0,
					"bgfillcolor_autogradient" : 0.0,
					"bgfillcolor_color" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color1" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 1.0 ],
					"bgfillcolor_proportion" : 0.5,
					"bgfillcolor_type" : "color",
					"gradient" : 1,
					"id" : "obj-150",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1374.0, 196.0, 44.0, 22.0 ],
					"text" : "-24 dB"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgcolor2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_angle" : 270.0,
					"bgfillcolor_autogradient" : 0.0,
					"bgfillcolor_color" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color1" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 1.0 ],
					"bgfillcolor_proportion" : 0.5,
					"bgfillcolor_type" : "color",
					"gradient" : 1,
					"id" : "obj-151",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1322.0, 196.0, 44.0, 22.0 ],
					"text" : "-12 dB"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-152",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "" ],
					"patching_rect" : [ 1322.0, 159.0, 122.0, 22.0 ],
					"text" : "select 0 1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgcolor2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_angle" : 270.0,
					"bgfillcolor_autogradient" : 0.0,
					"bgfillcolor_color" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color1" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 1.0 ],
					"bgfillcolor_proportion" : 0.5,
					"bgfillcolor_type" : "color",
					"gradient" : 1,
					"id" : "obj-149",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1434.0, 73.0, 44.0, 22.0 ],
					"text" : "-24 dB"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgcolor2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_angle" : 270.0,
					"bgfillcolor_autogradient" : 0.0,
					"bgfillcolor_color" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color1" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"bgfillcolor_color2" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 1.0 ],
					"bgfillcolor_proportion" : 0.5,
					"bgfillcolor_type" : "color",
					"gradient" : 1,
					"id" : "obj-148",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1382.0, 73.0, 44.0, 22.0 ],
					"text" : "-12 dB"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-143",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "" ],
					"patching_rect" : [ 1382.0, 36.0, 122.0, 22.0 ],
					"text" : "select 0 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-140",
					"maxclass" : "live.slider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1747.0, 145.0, 28.0, 70.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1262.0, 97.0, 28.0, 223.0 ],
					"saved_attribute_attributes" : 					{
						"slidercolor" : 						{
							"expression" : ""
						}
,
						"tribordercolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"tricolor" : 						{
							"expression" : "themecolor.live_lcd_title"
						}
,
						"trioncolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "MAKEUP GAIN SLIDER",
							"parameter_mmax" : 12.0,
							"parameter_mmin" : -12.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "MAKEUP GAIN SLIDER",
							"parameter_type" : 0,
							"parameter_unitstyle" : 4
						}

					}
,
					"showname" : 0,
					"shownumber" : 0,
					"slidercolor" : [ 1.0, 1.0, 1.0, 0.3 ],
					"tribordercolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"tricolor" : [ 0.768399306904929, 0.768399117237748, 0.7683991666346, 1.0 ],
					"trioncolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "MAKEUP"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-139",
					"maxclass" : "live.slider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 970.0, 35.0, 28.0, 53.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 122.0, 98.0, 28.0, 222.0 ],
					"saved_attribute_attributes" : 					{
						"slidercolor" : 						{
							"expression" : ""
						}
,
						"tribordercolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"tricolor" : 						{
							"expression" : "themecolor.live_lcd_title"
						}
,
						"trioncolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "SC TRIM SLIDER[",
							"parameter_mmax" : 12.0,
							"parameter_mmin" : -12.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "SC TRIM SLIDER[",
							"parameter_type" : 0,
							"parameter_unitstyle" : 4
						}

					}
,
					"showname" : 0,
					"shownumber" : 0,
					"slidercolor" : [ 1.0, 1.0, 1.0, 0.3 ],
					"tribordercolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"tricolor" : [ 0.768399306904929, 0.768399117237748, 0.7683991666346, 1.0 ],
					"trioncolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "SC TRIM SLIDER["
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-138",
					"maxclass" : "live.slider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 643.0, 281.0, 28.0, 87.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 38.0, 98.0, 28.0, 222.0 ],
					"saved_attribute_attributes" : 					{
						"slidercolor" : 						{
							"expression" : ""
						}
,
						"tribordercolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"tricolor" : 						{
							"expression" : "themecolor.live_lcd_title"
						}
,
						"trioncolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "TRIM SLIDER",
							"parameter_mmax" : 12.0,
							"parameter_mmin" : -12.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "TRIM SLIDER",
							"parameter_type" : 0,
							"parameter_unitstyle" : 4
						}

					}
,
					"showname" : 0,
					"shownumber" : 0,
					"slidercolor" : [ 1.0, 1.0, 1.0, 0.3 ],
					"tribordercolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"tricolor" : [ 0.768399306904929, 0.768399117237748, 0.7683991666346, 1.0 ],
					"trioncolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "TRIM"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 12.0,
					"id" : "obj-136",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1662.0, 566.0, 68.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1103.0, 224.0, 77.0, 20.0 ],
					"text" : "AUTO REL",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-135",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1501.0, 667.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1001.0, 224.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_exponent" : 4.0,
							"parameter_initial" : [ 30 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "REL",
							"parameter_mmax" : 1000.0,
							"parameter_mmin" : 0.1,
							"parameter_modmode" : 3,
							"parameter_shortname" : "REL",
							"parameter_type" : 0,
							"parameter_unitstyle" : 2
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[16]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-134",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1413.0, 667.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 905.0, 224.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_exponent" : 4.0,
							"parameter_initial" : [ 5 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "ATK",
							"parameter_mmax" : 250.0,
							"parameter_mmin" : 0.1,
							"parameter_modmode" : 3,
							"parameter_shortname" : "ATK",
							"parameter_type" : 0,
							"parameter_unitstyle" : 2
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[15]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-133",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1653.0, 145.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1097.0, 123.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "SMO",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "SMO",
							"parameter_type" : 0,
							"parameter_unitstyle" : 1
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[14]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-132",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1568.0, 145.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1001.0, 123.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "REACT",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "REACT",
							"parameter_type" : 0,
							"parameter_unitstyle" : 1
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[13]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-131",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1483.0, 145.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 898.0, 124.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 1 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "ALGO",
							"parameter_mmax" : 2.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "ALGO",
							"parameter_type" : 0,
							"parameter_unitstyle" : 1
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[12]"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 13.0,
					"id" : "obj-130",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1171.0, 37.0, 61.0, 21.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 649.0, 30.0, 61.0, 21.0 ],
					"text" : "FILTERS",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 13.0,
					"id" : "obj-129",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1172.0, 60.0, 70.0, 21.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 648.0, 50.0, 63.0, 21.0 ],
					"text" : "KEY EXT",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 13.0,
					"id" : "obj-128",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1174.0, 87.0, 66.0, 21.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 646.0, 74.0, 66.0, 21.0 ],
					"text" : "SOLO SC",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 13.0,
					"id" : "obj-127",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1191.0, 792.0, 60.0, 21.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 748.0, 323.0, 60.0, 21.0 ],
					"text" : "BYPASS",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 13.0,
					"id" : "obj-126",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1062.0, 792.0, 48.0, 21.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 549.0, 323.0, 48.0, 21.0 ],
					"text" : "DELTA",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"forceaspect" : 1,
					"id" : "obj-124",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 1425.0, 324.0, 20.0, 20.0 ],
					"pic" : "/Users/jcb/JUCEProjects/JCBComp/Assets/lpfOff.png",
					"presentation" : 1,
					"presentation_rect" : [ 810.0, 73.0, 22.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"forceaspect" : 1,
					"id" : "obj-123",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 990.0, 324.0, 20.0, 20.0 ],
					"pic" : "/Users/jcb/JUCEProjects/JCBComp/Assets/hpfOff.png",
					"presentation" : 1,
					"presentation_rect" : [ 520.0, 73.0, 22.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"dialcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"fgdialcolor" : [ 0.768399306904929, 0.768399117237748, 0.7683991666346, 1.0 ],
					"fontsize" : 14.0,
					"id" : "obj-122",
					"maxclass" : "live.dial",
					"needlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1277.0, 30.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 715.0, 26.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"dialcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"fgdialcolor" : 						{
							"expression" : "themecolor.live_lcd_title"
						}
,
						"needlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_exponent" : 4.0,
							"parameter_initial" : [ 20000 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "LPF",
							"parameter_mmax" : 20000.0,
							"parameter_mmin" : 20.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "LPF",
							"parameter_type" : 0,
							"parameter_unitstyle" : 3
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "LPF"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-121",
					"maxclass" : "live.dial",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1080.0, 30.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 554.0, 26.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_exponent" : 4.0,
							"parameter_initial" : [ 20 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "HPF",
							"parameter_mmax" : 20000.0,
							"parameter_mmin" : 20.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "HPF",
							"parameter_type" : 0,
							"parameter_unitstyle" : 3
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[11]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-120",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1050.0, 670.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 416.0, 224.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "PARALLEL",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "PARALLEL",
							"parameter_type" : 0,
							"parameter_unitstyle" : 5
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[10]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-119",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 950.0, 670.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 338.0, 224.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "SOFTCLIP",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "SOFTCLIP",
							"parameter_type" : 0,
							"parameter_unitstyle" : 5
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[9]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-118",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 833.0, 670.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 251.0, 224.0, 82.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "LOOKAHEAD",
							"parameter_mmax" : 10.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "LOOKAHEAD",
							"parameter_type" : 0,
							"parameter_unitstyle" : 2
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[8]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-117",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 695.0, 670.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 180.0, 224.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 1 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "DRY WET",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "DRY WET",
							"parameter_type" : 0,
							"parameter_unitstyle" : 5
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[7]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-116",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 808.0, 287.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 416.0, 122.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "AUTO GAIN",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "AUTO GAIN",
							"parameter_type" : 0,
							"parameter_unitstyle" : 5
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[6]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-115",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 837.0, 184.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 333.0, 122.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "KNEE",
							"parameter_mmax" : 30.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "KNEE",
							"parameter_type" : 0,
							"parameter_unitstyle" : 4
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[5]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-114",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 760.0, 184.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 251.0, 122.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ 4 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "RATIO",
							"parameter_mmax" : 20.0,
							"parameter_mmin" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "RATIO",
							"parameter_type" : 0,
							"parameter_unitstyle" : 0
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[4]"
				}

			}
, 			{
				"box" : 				{
					"activedialcolor" : [ 0.242201021442454, 0.242200956343583, 0.242200973354958, 1.0 ],
					"activeneedlecolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"appearance" : 3,
					"fontsize" : 14.0,
					"id" : "obj-113",
					"maxclass" : "live.dial",
					"needlemode" : 2,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 729.0, 287.0, 75.0, 74.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 177.0, 122.0, 75.0, 74.0 ],
					"saved_attribute_attributes" : 					{
						"activedialcolor" : 						{
							"expression" : "themecolor.live_value_arc_zombie"
						}
,
						"activeneedlecolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"textcolor" : 						{
							"expression" : "themecolor.live_output_curve_outline_color"
						}
,
						"valueof" : 						{
							"parameter_initial" : [ -18 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "THD",
							"parameter_mmax" : 0.0,
							"parameter_mmin" : -60.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "THD",
							"parameter_type" : 0,
							"parameter_unitstyle" : 4
						}

					}
,
					"textcolor" : [ 0.811764705882353, 0.811764705882353, 0.811764705882353, 1.0 ],
					"varname" : "live.dial[3]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-100",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 430.0, 218.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-101",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 430.0, 249.0, 65.0, 22.0 ],
					"text" : "modout $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-97",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 401.0, 169.0, 21.0, 21.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-96",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 430.0, 169.0, 88.0, 22.0 ],
					"text" : "open, loop 1, 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-95",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "signal", "signal", "bang" ],
					"patching_rect" : [ 400.0, 298.0, 113.0, 22.0 ],
					"text" : "sfplay~ 2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-93",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 349.0, 248.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-78",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 349.0, 298.0, 39.0, 22.0 ],
					"text" : "click~"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 22.0,
					"id" : "obj-74",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 645.0, 35.0, 161.0, 31.0 ],
					"text" : "PARÁMETROS",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-72",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 64.0, 77.0, 49.0, 22.0 ],
					"text" : "r toGen"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-58",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1132.0, 794.0, 51.0, 22.0 ],
					"text" : "s toGen"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 18.0,
					"id" : "obj-55",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 9,
							"minor" : 0,
							"revision" : 7,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 643.0, 96.0, 793.0, 884.0 ],
						"gridsize" : [ 15.0, 15.0 ],
						"boxes" : [ 							{
								"box" : 								{
									"fontsize" : 17.0,
									"id" : "obj-8",
									"linecount" : 42,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 6.0, 6.0, 762.0, 804.0 ],
									"text" : "// PARÁMETROS DEL COMPRESOR\n\n// Parámetros principales\nParam a_TRIM(-12, min=-12, default=0, max=12);        // Input trim gain (dB)\nParam b_THD(-60, min=-60, default=-18, max=0);        // Threshold (dB)\nParam c_RATIO(1, min=1, default=4, max=20);           // Compression ratio\nParam d_ATK(0.1, min=0.1, default=5, max=250);       // Attack time (ms)\nParam e_REL(0.1, min=0.1, default=30, max=1000);     // Release time (ms)\nParam i_MAKEUP(-12, min=-12, default=0, max=12);      // Makeup gain (dB)\nParam h_KNEE(0, min=0, default=0, max=30);            // Knee width (dB)\n\n// Algoritmos y detección\nParam f_ALGO(0, min=0, default=1, max=2);             // Detection algorithm (0=Sharp, 1=Classic, 2=Slow)\nParam g_REACT(0, min=0, default=0, max=1);            // Peak/RMS mix (0=Peak, 1=RMS)\nParam z_SMOOTH(0, min=0, default=0, max=1);           // Extra smoothing amount\n\n// Filtros de sidechain\nParam j_HPF(20, min=20, default=20, max=20000);       // Sidechain HPF frequency (Hz)\nParam j_HPFORDER(0, min=0, default=0, max=1);         // HPF order (0=12dB/oct, 1=24dB/oct)\nParam k_LPF(20, min=20, default=20000, max=20000);    // Sidechain LPF frequency (Hz)\nParam k_LPFORDER(0, min=0, default=0, max=1);         // LPF order (0=12dB/oct, 1=24dB/oct)\nParam l_SC(0, min=0, default=0, max=1);               // Sidechain filter enable\nParam y_SCTRIM(-12, min=-12, default=0, max=12);      // Sidechain input trim (dB)\n\n// Routing y mezcla\nParam r_KEY(0, min=0, default=0, max=1);              // External key input mix\nParam m_SOLOSC(0, min=0, default=0, max=1);           // Solo sidechain signal\nParam o_DRYWET(0, min=0, default=1, max=1);           // Dry/Wet mix (0=Dry, 1=Wet)\nParam w_PARALLEL(0, min=0, default=0, max=1);         // Parallel compression amount\nParam v_DELTA(0, min=0, default=0, max=1);            // Delta mode (difference signal)\n\n// Características avanzadas\nParam n_LOOKAHEAD(0, min=0, default=0, max=10);       // Lookahead time (ms)\nParam s_AUTORELEASE(0, min=0, default=0, max=1);      // Auto release enable\n\n\nParam x_AUTOGAIN(0, min=0, default=0, max=1);         // Auto makeup gain enable\nParam u_SOFTCLIP(0, min=0, default=0, max=1);         // Output saturation amount (0=Off, 1=Full)\n\n// Control\nParam p_BYPASS(0, min=0, default=0, max=1);           // Bypass switch"
								}

							}
 ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 645.0, 73.5, 86.0, 29.0 ],
					"text" : "p Params"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-92",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1355.0, 262.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 810.0, 29.0, 22.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_enum" : [ "off", "on" ],
							"parameter_initial" : [ 0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "toggle[5]",
							"parameter_mmax" : 1,
							"parameter_modmode" : 0,
							"parameter_shortname" : "toggle[5]",
							"parameter_type" : 2
						}

					}
,
					"varname" : "toggle[5]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-91",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1355.0, 297.0, 103.0, 22.0 ],
					"text" : "k_LPFORDER $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-89",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 970.0, 262.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 519.0, 29.0, 23.0, 23.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_enum" : [ "off", "on" ],
							"parameter_initial" : [ 0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "toggle[4]",
							"parameter_mmax" : 1,
							"parameter_modmode" : 0,
							"parameter_shortname" : "toggle[4]",
							"parameter_type" : 2
						}

					}
,
					"varname" : "toggle[4]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-80",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 970.0, 297.0, 102.0, 22.0 ],
					"text" : "j_HPFORDER $1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.368627450980392, 0.356862745098039, 0.356862745098039, 0.501960784313725 ],
					"id" : "obj-68",
					"maxclass" : "meter~",
					"monotone" : 1,
					"numinlets" : 1,
					"numleds" : 20,
					"numoutlets" : 1,
					"offcolor" : [ 0.270588235294118, 0.270588235294118, 0.270588235294118, 0.0 ],
					"oncolor" : [ 0.345098039215686, 0.066666666666667, 0.443137254901961, 1.0 ],
					"outlettype" : [ "float" ],
					"patching_rect" : [ 451.0, 656.0, 12.0, 58.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 150.0, 102.0, 32.0, 214.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.368627450980392, 0.356862745098039, 0.356862745098039, 0.501960784313725 ],
					"id" : "obj-79",
					"maxclass" : "meter~",
					"monotone" : 1,
					"numinlets" : 1,
					"numleds" : 20,
					"numoutlets" : 1,
					"offcolor" : [ 0.270588235294118, 0.270588235294118, 0.270588235294118, 0.0 ],
					"oncolor" : [ 0.345098039215686, 0.066666666666667, 0.443137254901961, 1.0 ],
					"outlettype" : [ "float" ],
					"patching_rect" : [ 402.0, 656.0, 12.0, 58.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 103.0, 103.0, 32.0, 214.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.368627450980392, 0.356862745098039, 0.356862745098039, 0.501960784313725 ],
					"id" : "obj-56",
					"maxclass" : "meter~",
					"monotone" : 1,
					"numinlets" : 1,
					"numleds" : 20,
					"numoutlets" : 1,
					"offcolor" : [ 0.270588235294118, 0.270588235294118, 0.270588235294118, 0.0 ],
					"oncolor" : [ 0.345098039215686, 0.066666666666667, 0.443137254901961, 1.0 ],
					"outlettype" : [ "float" ],
					"patching_rect" : [ 353.0, 656.0, 12.0, 58.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 65.0, 102.0, 32.0, 214.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.368627450980392, 0.356862745098039, 0.356862745098039, 0.501960784313725 ],
					"id" : "obj-36",
					"maxclass" : "meter~",
					"monotone" : 1,
					"numinlets" : 1,
					"numleds" : 20,
					"numoutlets" : 1,
					"offcolor" : [ 0.270588235294118, 0.270588235294118, 0.270588235294118, 0.0 ],
					"oncolor" : [ 0.345098039215686, 0.066666666666667, 0.443137254901961, 1.0 ],
					"outlettype" : [ "float" ],
					"patching_rect" : [ 304.0, 656.0, 12.0, 58.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 19.0, 103.0, 32.0, 214.0 ]
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-61",
					"maxclass" : "flonum",
					"maximum" : 1.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1591.0, 446.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-50",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1591.0, 480.0, 91.0, 22.0 ],
					"text" : "z_SMOOTH $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-51",
					"maxclass" : "flonum",
					"maximum" : 12.0,
					"minimum" : -12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 970.0, 154.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-46",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 970.0, 184.0, 84.0, 22.0 ],
					"text" : "y_SCTRIM $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-44",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 388.0, 721.0, 86.0, 20.0 ],
					"text" : "SC TRIM OUT",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-32",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 303.0, 721.0, 74.0, 20.0 ],
					"text" : "TRIM OUT",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-83",
					"maxclass" : "flonum",
					"maximum" : 1.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 950.0, 446.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-81",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 950.0, 480.0, 100.0, 22.0 ],
					"text" : "x_AUTOGAIN $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-75",
					"maxclass" : "flonum",
					"maximum" : 1.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1025.0, 579.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-73",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1025.0, 612.0, 100.0, 22.0 ],
					"text" : "w_PARALLEL $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-67",
					"maxclass" : "flonum",
					"maximum" : 1.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 923.0, 584.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-66",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 924.0, 612.0, 97.0, 22.0 ],
					"text" : "u_SOFTCLIP $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-65",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1189.0, 553.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 515.0, 317.0, 32.0, 32.0 ]
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-63",
					"maxclass" : "flonum",
					"maximum" : 1.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1189.0, 589.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-59",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1189.0, 620.0, 74.0, 22.0 ],
					"text" : "v_DELTA $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-52",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1633.0, 564.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1119.0, 252.0, 31.0, 31.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-45",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1633.0, 599.0, 126.0, 22.0 ],
					"text" : "s_AUTORELEASE $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-60",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 368.0, 508.0, 73.0, 20.0 ],
					"text" : "SC INPUTS",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-53",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1212.0, 158.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 626.0, 48.0, 24.0, 24.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_enum" : [ "off", "on" ],
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "toggle[3]",
							"parameter_mmax" : 1,
							"parameter_modmode" : 0,
							"parameter_shortname" : "toggle",
							"parameter_type" : 2
						}

					}
,
					"varname" : "toggle[3]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-54",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1212.0, 193.0, 60.0, 22.0 ],
					"text" : "r_KEY $1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.149019607843137, 0.149019607843137, 0.149019607843137, 0.5 ],
					"id" : "obj-48",
					"maxclass" : "meter~",
					"monotone" : 1,
					"numinlets" : 1,
					"numleds" : 20,
					"numoutlets" : 1,
					"offcolor" : [ 0.270588235294118, 0.270588235294118, 0.270588235294118, 0.0 ],
					"oncolor" : [ 0.211764705882353, 0.384313725490196, 0.666666666666667, 1.0 ],
					"outlettype" : [ "float" ],
					"patching_rect" : [ 218.0, 641.0, 13.0, 58.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1289.0, 102.0, 31.0, 214.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.149019607843137, 0.149019607843137, 0.149019607843137, 0.5 ],
					"id" : "obj-39",
					"maxclass" : "meter~",
					"monotone" : 1,
					"numinlets" : 1,
					"numleds" : 20,
					"numoutlets" : 1,
					"offcolor" : [ 0.270588235294118, 0.270588235294118, 0.270588235294118, 0.0 ],
					"oncolor" : [ 0.211764705882353, 0.384313725490196, 0.666666666666667, 1.0 ],
					"outlettype" : [ "float" ],
					"patching_rect" : [ 139.0, 641.0, 13.0, 58.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 1240.0, 102.0, 34.0, 214.0 ]
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-49",
					"maxclass" : "flonum",
					"maximum" : 2.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1389.0, 446.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 1 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[8]",
							"parameter_mmax" : 2.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[8]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[8]"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-47",
					"maxclass" : "flonum",
					"maximum" : 1.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1483.0, 446.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_longname" : "number[9]",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[9]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[9]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-43",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1389.0, 480.0, 69.0, 22.0 ],
					"text" : "f_ALGO $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-42",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1483.0, 480.0, 79.0, 22.0 ],
					"text" : "g_REACT $1"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-28",
					"interval" : 20.0,
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 251.0, 738.0, 57.0, 22.0 ],
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 643.0, 447.0, 68.0, 22.0 ],
					"text" : "a_TRIM $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-30",
					"maxclass" : "flonum",
					"maximum" : 12.0,
					"minimum" : -12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 643.0, 413.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[3]",
							"parameter_mmax" : 12.0,
							"parameter_mmin" : -12.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[3]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[3]"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-29",
					"maxclass" : "flonum",
					"maximum" : 1.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 680.0, 585.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 1.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number",
							"parameter_mmax" : 1.0,
							"parameter_modmode" : 0,
							"parameter_shortname" : "number",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 210.0, 211.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 210.0, 242.0, 65.0, 22.0 ],
					"text" : "modout $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1205.0, 436.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 626.0, 73.0, 24.0, 24.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_enum" : [ "off", "on" ],
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "toggle[1]",
							"parameter_mmax" : 1,
							"parameter_modmode" : 0,
							"parameter_shortname" : "toggle[1]",
							"parameter_type" : 2
						}

					}
,
					"varname" : "toggle[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-20",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1264.0, 531.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 810.0, 317.0, 32.0, 32.0 ],
					"varname" : "toggle[2]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1205.0, 471.0, 92.0, 22.0 ],
					"text" : "m_SOLOSC $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-18",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1264.0, 565.0, 86.0, 22.0 ],
					"text" : "p_BYPASS $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-22",
					"maxclass" : "flonum",
					"maximum" : 20000.0,
					"minimum" : 20.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1080.0, 263.0, 69.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 20.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[2]",
							"parameter_mmax" : 20000.0,
							"parameter_mmin" : 20.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[2]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[2]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-19",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1154.0, 159.0, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 626.0, 29.0, 24.0, 24.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_enum" : [ "off", "on" ],
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "toggle",
							"parameter_mmax" : 1,
							"parameter_modmode" : 0,
							"parameter_shortname" : "toggle",
							"parameter_type" : 2
						}

					}
,
					"varname" : "toggle"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1154.0, 193.0, 51.0, 22.0 ],
					"text" : "l_SC $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-14",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1080.0, 297.0, 59.0, 22.0 ],
					"text" : "j_HPF $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-10",
					"maxclass" : "flonum",
					"maximum" : 20000.0,
					"minimum" : 20.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1277.0, 263.0, 66.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 20000.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[1]",
							"parameter_mmax" : 20000.0,
							"parameter_mmin" : 20.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[1]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-13",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1277.0, 297.0, 60.0, 22.0 ],
					"text" : "k_LPF $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-35",
					"maxclass" : "flonum",
					"maximum" : 20.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 875.0, 446.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[10]",
							"parameter_mmax" : 20.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[10]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[10]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-33",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 875.0, 480.0, 71.0, 22.0 ],
					"text" : "h_KNEE $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-2",
					"maxclass" : "flonum",
					"maximum" : 10.0,
					"minimum" : 0.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 795.0, 585.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[12]",
							"parameter_mmax" : 10.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[12]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[12]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 795.0, 612.0, 113.0, 22.0 ],
					"text" : "n_LOOKAHEAD $1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.164705882352941, 0.831372549019608, 0.094117647058824, 1.0 ],
					"bgcolor2" : [ 0.2, 0.2, 0.2, 1.0 ],
					"bgfillcolor_angle" : 270.0,
					"bgfillcolor_autogradient" : 0.0,
					"bgfillcolor_color" : [ 0.164705882352941, 0.831372549019608, 0.094117647058824, 1.0 ],
					"bgfillcolor_color1" : [ 0.164705882352941, 0.831372549019608, 0.094117647058824, 1.0 ],
					"bgfillcolor_color2" : [ 0.2, 0.2, 0.2, 1.0 ],
					"bgfillcolor_proportion" : 0.5,
					"bgfillcolor_type" : "gradient",
					"gradient" : 1,
					"id" : "obj-23",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 150.0, 77.0, 68.0, 22.0 ],
					"text" : "exportcode"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 275.0, 182.0, 21.0, 21.0 ]
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-98",
					"maxclass" : "flonum",
					"maximum" : 12.0,
					"minimum" : -12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1690.0, 406.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 0.0 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[11]",
							"parameter_mmax" : 12.0,
							"parameter_mmin" : -12.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[11]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[11]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-99",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1690.0, 440.0, 85.0, 22.0 ],
					"text" : "i_MAKEUP $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-16",
					"maxclass" : "flonum",
					"maximum" : 1000.0,
					"minimum" : 0.1,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1405.0, 565.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 0.01 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[6]",
							"parameter_mmax" : 1000.0,
							"parameter_mmin" : 0.1,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[6]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[6]"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-24",
					"maxclass" : "flonum",
					"maximum" : 1000.0,
					"minimum" : 0.1,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 1480.0, 565.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 150 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[7]",
							"parameter_mmax" : 1000.0,
							"parameter_mmin" : 0.1,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[7]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[7]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-40",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1480.0, 599.0, 61.0, 22.0 ],
					"text" : "e_REL $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-41",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1405.0, 599.0, 61.0, 22.0 ],
					"text" : "d_ATK $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-57",
					"maxclass" : "flonum",
					"maximum" : 20.0,
					"minimum" : 1.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 799.0, 446.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 4 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[5]",
							"parameter_mmax" : 20.0,
							"parameter_mmin" : 1.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[5]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[5]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-62",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 799.0, 480.0, 74.0, 22.0 ],
					"text" : "c_RATIO $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-70",
					"maxclass" : "flonum",
					"maximum" : 0.0,
					"minimum" : -60.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 729.0, 446.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ -18 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "number[4]",
							"parameter_mmax" : 0.0,
							"parameter_mmin" : -60.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "number[4]",
							"parameter_type" : 0
						}

					}
,
					"varname" : "number[4]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-77",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 729.0, 480.0, 63.0, 22.0 ],
					"text" : "b_THD $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-86",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 680.0, 612.0, 90.0, 22.0 ],
					"text" : "o_DRYWET $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 156.0, 641.0, 57.0, 57.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 181.0, 181.0, 88.0, 22.0 ],
					"text" : "open, loop 1, 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "signal", "signal", "bang" ],
					"patching_rect" : [ 182.0, 297.0, 121.0, 22.0 ],
					"text" : "sfplay~ 2"
				}

			}
, 			{
				"box" : 				{
					"attr" : "exportfolder",
					"id" : "obj-38",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 124.0, 39.0, 437.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 4,
					"numoutlets" : 7,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 9,
							"minor" : 0,
							"revision" : 7,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "dsp.gen",
						"rect" : [ 435.0, 87.0, 1052.0, 903.0 ],
						"gridsize" : [ 15.0, 15.0 ],
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-28",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 865.0, 872.0, 159.0, 20.0 ],
									"text" : "POST TRIM SC R (METER)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-27",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 562.0, 872.0, 139.0, 20.0 ],
									"text" : "POST TRIM R (METER)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-26",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 865.0, 6.0, 53.0, 20.0 ],
									"text" : "SC IN R"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-25",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 598.0, 6.0, 51.0, 20.0 ],
									"text" : "SC IN L"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 333.0, 6.0, 32.0, 20.0 ],
									"text" : "IN R"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-23",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 63.0, 6.0, 31.0, 20.0 ],
									"text" : "IN L"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 703.0, 872.0, 156.0, 20.0 ],
									"text" : "POST TRIM SC L (METER)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-17",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 415.0, 872.0, 136.0, 20.0 ],
									"text" : "POST TRIM L (METER)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 309.0, 872.0, 80.0, 20.0 ],
									"text" : "GR (METER)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-15",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 194.0, 872.0, 46.0, 20.0 ],
									"text" : "OUT R"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 59.0, 872.0, 44.0, 20.0 ],
									"text" : "OUT L"
								}

							}
, 							{
								"box" : 								{
									"code" : "// Funciones\nsoftkneeLinear(xg, T, R, W) {\n    _softkneeLinear_ret_1 = 0;\n    if((2 * ((xg - T))) < (W * (-1))) {\n        _softkneeLinear_ret_1 = xg;\n    }\n    else if((2 * (abs(xg - T))) <= W) {\n        _softkneeLinear_ret_1 = (T - (W / 2)) + ((((((xg - T) + (W / 2))) * ((1 + (1 / R))))) / 2);\n    }\n    else if((2 * ((xg - T))) > W) {\n        _softkneeLinear_ret_1 = T + (((xg - T)) / R);\n    }\n    return _softkneeLinear_ret_1;\n}\n\n// =============================================================================\n// PARÁMETROS DEL COMPRESOR\n// =============================================================================\n\n// Parámetros principales\nParam a_TRIM(-12, min=-12, default=0, max=12);        // Input trim gain (dB)\nParam b_THD(-60, min=-60, default=-18, max=0);        // Threshold (dB)\nParam c_RATIO(1, min=1, default=4, max=20);           // Compression ratio\nParam d_ATK(0.1, min=0.1, default=5, max=250);       // Attack time (ms)\nParam e_REL(0.1, min=0.1, default=30, max=1000);     // Release time (ms)\nParam i_MAKEUP(-12, min=-12, default=0, max=12);      // Makeup gain (dB)\nParam h_KNEE(0, min=0, default=0, max=30);            // Knee width (dB)\n\n// Algoritmos y detección\nParam f_ALGO(0, min=0, default=1, max=2);             // Detection algorithm (0=Sharp, 1=Classic, 2=Slow)\nParam g_REACT(0, min=0, default=0, max=1);            // Peak/RMS mix (0=Peak, 1=RMS)\nParam z_SMOOTH(0, min=0, default=0, max=1);           // Extra smoothing amount\n\n// Filtros de sidechain\nParam j_HPF(20, min=20, default=20, max=20000);       // Sidechain HPF frequency (Hz)\nParam j_HPFORDER(0, min=0, default=0, max=1);         // HPF order (0=12dB/oct, 1=24dB/oct)\nParam k_LPF(20, min=20, default=20000, max=20000);    // Sidechain LPF frequency (Hz)\nParam k_LPFORDER(0, min=0, default=0, max=1);         // LPF order (0=12dB/oct, 1=24dB/oct)\nParam l_SC(0, min=0, default=0, max=1);               // Sidechain filter enable\nParam y_SCTRIM(-12, min=-12, default=0, max=12);      // Sidechain input trim (dB)\n\n// Routing y mezcla\nParam r_KEY(0, min=0, default=0, max=1);              // External key input mix\nParam m_SOLOSC(0, min=0, default=0, max=1);           // Solo sidechain signal\nParam o_DRYWET(0, min=0, default=1, max=1);           // Dry/Wet mix (0=Dry, 1=Wet)\nParam w_PARALLEL(0, min=0, default=0, max=1);         // Parallel compression amount\nParam v_DELTA(0, min=0, default=0, max=1);            // Delta mode (difference signal)\n\n// Características avanzadas\nParam n_LOOKAHEAD(0, min=0, default=0, max=10);       // Lookahead time (ms)\nParam s_AUTORELEASE(0, min=0, default=0, max=1);      // Auto release enable (simplificado)\nParam x_AUTOGAIN(0, min=0, default=0, max=1);         // Auto makeup gain enable\nParam u_SOFTCLIP(0, min=0, default=0, max=1);         // Output saturation amount (0=Off, 1=Full)\n\n// Control\nParam p_BYPASS(0, min=0, default=0, max=1);           // Bypass switch\n\n// =============================================================================\n// HISTORIAS Y DELAYS\n// =============================================================================\n\n// Historias para smoothing de parámetros\nHistory parallelMixHistory(0);         // Para parallel compression amount\nHistory deltaModeHistory(0);           // Para delta mode amount\nHistory autoGainHistory(0);            // Para auto makeup gain enable\nHistory thresholdHistory(0);           // Para threshold value\nHistory makeupGainHistory(0);          // Para makeup gain amount\nHistory soloSidechainHistory(0);       // Para solo sidechain amount\nHistory bypassAmountHistory(0);        // Para bypass amount\nHistory keyMixHistory(0);              // Para key input mix\nHistory dryWetMixHistory(0);           // Para dry/wet mix\nHistory trimHistory(0);                // Para input trim gain\nHistory sidechainTrimHistory(0);       // Para sidechain trim gain\n\n// Historias adicionales para smoothing de parámetros\nHistory ratioHistory(0);              // Para compression ratio\nHistory kneeHistory(0);               // Para knee width\nHistory reactHistory(0);              // Para peak/RMS mix\nHistory smoothAmountHistory(0);       // Para extra smoothing\nHistory lookaheadHistory(0);          // Para lookahead (con factor especial)\nHistory hpfFreqHistory(0);            // Para HPF frequency\nHistory lpfFreqHistory(0);            // Para LPF frequency\nHistory scEnableHistory(0);           // Para sidechain filter enable\nHistory softclipHistory(0);           // Para softclip amount\nHistory atkHistory(0);                // Para attack time\nHistory relHistory(0);                // Para release time\n\n// Historias para filtros de sidechain\nHistory lpfHistory1(0);                // LPF stage 1 input delay\nHistory lpfHistory2(0);                // LPF stage 1 output delay z^-1\nHistory lpfHistory3(0);                // LPF stage 1 input z^-1\nHistory lpfHistory4(0);                // LPF stage 1 output z^-2\nHistory lpfHistory5(0);                // LPF stage 2 input delay\nHistory lpfHistory6(0);                // LPF stage 2 output delay z^-1\nHistory lpfHistory7(0);                // LPF stage 2 input z^-1\nHistory lpfHistory8(0);                // LPF stage 2 output z^-2\nHistory hpfHistory1(0);                // HPF stage 1 output delay z^-1\nHistory hpfHistory2(0);                // HPF stage 1 input delay\nHistory hpfHistory3(0);                // HPF stage 1 input z^-1\nHistory hpfHistory4(0);                // HPF stage 1 output z^-2\nHistory hpfHistory5(0);                // HPF stage 2 output delay z^-1\nHistory hpfHistory6(0);                // HPF stage 2 input delay\nHistory hpfHistory7(0);                // HPF stage 2 input z^-1\nHistory hpfHistory8(0);                // HPF stage 2 output z^-2\n\n// Historias para smoothing de orden de filtros\nHistory hpfOrderHistory(0);            // Para suavizar cambio de orden HPF\nHistory lpfOrderHistory(0);            // Para suavizar cambio de orden LPF\n\n// Nueva historia para detección de transientes (auto-release simplificado)\nHistory transientDetector(0);          // Detector de transientes acumulado\n\n// Delays de lookahead (25ms a 48kHz = 1200 muestras)\nDelay leftOutputDelay(0.025*samplerate);   // Delay señal izquierda para compresión\nDelay rightOutputDelay(0.025*samplerate);  // Delay señal derecha para compresión\nDelay leftInputDelay(0.025*samplerate);    // Delay entrada izquierda para mezcla\nDelay rightInputDelay(0.025*samplerate);   // Delay entrada derecha para mezcla\nDelay sidechainTapDelay(0.025*samplerate); // Delay para tap de sidechain\nDelay sidechainWriteDelay(0.025*samplerate); // Delay para escritura de sidechain\n\n// =============================================================================\n// VARIABLES DE DETECCIÓN DE ENVOLVENTE\n// =============================================================================\n\n// RMS Detection Buffer\nDelay rmsDelay(500);                   // Circular buffer para cálculo RMS (500 muestras)\nHistory rmsSum(0);                     // Suma acumulada para RMS\nHistory prevEnvelope(0);               // Envolvente anterior para auto-release\n\n// Sharp Detector (Algoritmo 0) - Respuesta rápida\nHistory peakEnvSharp(0);               // Detector de picos rápido\nHistory releaseHistSharp(0);           // Historia de release para sharp\nHistory attackHistSharp(0);            // Historia de attack para sharp\nHistory extraSmoothHistSharp(0);       // Extra smoothing para sharp\n\n// Classic Detector (Algoritmo 1) - Respuesta equilibrada\nHistory peakEnvClassic(0);             // Detector de picos clásico\nHistory rmsSqClassic(0);               // Suma cuadrática RMS clásico\nHistory histRmsClassic(0);             // Historia RMS clásico\nHistory releaseHistClassic(0);         // Historia de release para classic\nHistory attackHistClassic(0);          // Historia de attack para classic\nHistory extraSmoothHistClassic(0);     // Extra smoothing para classic\n\n// Slow Detector (Algoritmo 2) - Respuesta suave\nHistory peakEnvSlow(0);                // Detector de picos lento\nHistory rmsSqSlow(0);                  // Suma cuadrática RMS lento\nHistory histRmsSlow(0);                // Historia RMS lento\nHistory releaseHistSlow(0);            // Historia de release para slow\nHistory attackHistSlow(0);             // Historia de attack para slow\nHistory extraSmoothHistSlow(0);        // Extra smoothing para slow\n\n// Gain Reduction Smoothing\nHistory reductionHistLeft(0);          // Suavizado de gain reduction canal L\nHistory reductionHistRight(0);         // Suavizado de gain reduction canal R\n\n// =============================================================================\n// VARIABLES DE SALIDA Y CONSTANTES\n// =============================================================================\n\n// Variables para almacenar las salidas procesadas\nleftProcessedOutput = 0;               // Salida procesada canal izquierdo\nrightProcessedOutput = 0;              // Salida procesada canal derecho\n\n// Constantes para optimización y legibilidad\nSMOOTH_HISTORY_FACTOR = 0.999;         // Factor de suavizado para valores históricos (99.9%)\nSMOOTH_PARAM_FACTOR = 0.001;           // Factor de suavizado para nuevos parámetros (0.1%)\nAUTO_MAKEUP_FACTOR = -0.7;             // Factor de compensación automática de ganancia\nLOOKAHEAD_DELAY_SAMPLES = 0.025*samplerate; // Muestras de delay para lookahead\nLOOKAHEAD_SMOOTH_FACTOR = 0.9999;      // Factor ultra lento para lookahead\n\n// PROCESAMIENTO PRINCIPAL\n\n// =============================================================================\n// SMOOTHING DE PARÁMETROS - Filtro de primer orden para evitar clicks\n// =============================================================================\n// Implementación: y[n] = a * x[n] + (1-a) * y[n-1]\n// Donde: a = SMOOTH_PARAM_FACTOR (0.001), (1-a) = SMOOTH_HISTORY_FACTOR (0.999)\n// Resultado: Suavizado exponencial con tiempo de respuesta ~1000 muestras\n\n// Parallel Mix Amount - Cantidad de compresión paralela\nsmoothParallelMix = (parallelMixHistory * SMOOTH_HISTORY_FACTOR) + (w_PARALLEL * SMOOTH_PARAM_FACTOR);\nparallelMixHistoryNext = fixdenorm(smoothParallelMix);\n\n// Delta Mode Amount - Cantidad de señal diferencia (experimental)\nsmoothDeltaMode = (deltaModeHistory * SMOOTH_HISTORY_FACTOR) + (v_DELTA * SMOOTH_PARAM_FACTOR);\ndeltaModeHistoryNext = fixdenorm(smoothDeltaMode);\n\n// Auto Gain Enable - Activación de makeup gain automático\nsmoothAutoGain = (autoGainHistory * SMOOTH_HISTORY_FACTOR) + (x_AUTOGAIN * SMOOTH_PARAM_FACTOR);\nautoGainHistoryNext = fixdenorm(smoothAutoGain);\n\n// =============================================================================\n// IMPLEMENTACIÓN DELTA MODE - Bypass automático de procesamiento posterior\n// =============================================================================\n// Cuando DELTA está activo, se bypasean automáticamente:\n// - Auto Gain: se fuerza a 0 (solo makeup manual)\n// - Parallel Compression: se fuerza a 0 (sin compresión paralela)\n// - Dry/Wet: se fuerza a 1 (100% wet)\n// Esto permite escuchar SOLO la señal diferencia sin amplificaciones posteriores\n\n// Crear valores de bypass cuando DELTA está activo\ndeltaBypassValue = int(0);  // Valor para forzar auto gain y parallel a 0\ndeltaWetValue = int(1);     // Valor para forzar dry/wet a 100% wet\n\n// Aplicar bypass condicional basado en DELTA\neffectiveAutoGain = mix(smoothAutoGain, deltaBypassValue, smoothDeltaMode);\neffectiveParallelMix = mix(smoothParallelMix, deltaBypassValue, smoothDeltaMode);\n\n// Threshold Level - Umbral de compresión en dB\nsmoothThresholdDb = (thresholdHistory * SMOOTH_HISTORY_FACTOR) + (b_THD * SMOOTH_PARAM_FACTOR);\nthresholdHistoryNext = fixdenorm(smoothThresholdDb);\n\n// Filter Order Smoothing - Suavizado de orden de filtros\nsmoothHpfOrder = (hpfOrderHistory * SMOOTH_HISTORY_FACTOR) + (j_HPFORDER * SMOOTH_PARAM_FACTOR);\nhpfOrderHistoryNext = fixdenorm(smoothHpfOrder);\nsmoothLpfOrder = (lpfOrderHistory * SMOOTH_HISTORY_FACTOR) + (k_LPFORDER * SMOOTH_PARAM_FACTOR);\nlpfOrderHistoryNext = fixdenorm(smoothLpfOrder);\n\n// Ratio Smoothing\nsmoothRatio = (ratioHistory * SMOOTH_HISTORY_FACTOR) + (c_RATIO * SMOOTH_PARAM_FACTOR);\nratioHistoryNext = fixdenorm(smoothRatio);\n\n// Knee Smoothing\nsmoothKnee = (kneeHistory * SMOOTH_HISTORY_FACTOR) + (h_KNEE * SMOOTH_PARAM_FACTOR);\nkneeHistoryNext = fixdenorm(smoothKnee);\n\n// React (Peak/RMS) Smoothing\nsmoothReact = (reactHistory * SMOOTH_HISTORY_FACTOR) + (g_REACT * SMOOTH_PARAM_FACTOR);\nreactHistoryNext = fixdenorm(smoothReact);\n\n// Extra Smooth Amount Smoothing\nsmoothSmoothAmount = (smoothAmountHistory * SMOOTH_HISTORY_FACTOR) + (z_SMOOTH * SMOOTH_PARAM_FACTOR);\nsmoothAmountHistoryNext = fixdenorm(smoothSmoothAmount);\n\n// Attack Time Smoothing\nsmoothAttack = (atkHistory * SMOOTH_HISTORY_FACTOR) + (d_ATK * SMOOTH_PARAM_FACTOR);\natkHistoryNext = fixdenorm(smoothAttack);\n\n// Release Time Smoothing\nsmoothRelease = (relHistory * SMOOTH_HISTORY_FACTOR) + (e_REL * SMOOTH_PARAM_FACTOR);\nrelHistoryNext = fixdenorm(smoothRelease);\n\n// HPF Frequency Smoothing\nsmoothHpfFreq = (hpfFreqHistory * SMOOTH_HISTORY_FACTOR) + (j_HPF * SMOOTH_PARAM_FACTOR);\nhpfFreqHistoryNext = fixdenorm(smoothHpfFreq);\n\n// LPF Frequency Smoothing\nsmoothLpfFreq = (lpfFreqHistory * SMOOTH_HISTORY_FACTOR) + (k_LPF * SMOOTH_PARAM_FACTOR);\nlpfFreqHistoryNext = fixdenorm(smoothLpfFreq);\n\n// Sidechain Enable Smoothing\nsmoothScEnable = (scEnableHistory * SMOOTH_HISTORY_FACTOR) + (l_SC * SMOOTH_PARAM_FACTOR);\nscEnableHistoryNext = fixdenorm(smoothScEnable);\n\n// Softclip Amount Smoothing\nsmoothSoftclip = (softclipHistory * SMOOTH_HISTORY_FACTOR) + (u_SOFTCLIP * SMOOTH_PARAM_FACTOR);\nsoftclipHistoryNext = fixdenorm(smoothSoftclip);\n\n// Lookahead Smoothing (ULTRA LENTO)\nsmoothLookahead = (lookaheadHistory * LOOKAHEAD_SMOOTH_FACTOR) + (n_LOOKAHEAD * (1 - LOOKAHEAD_SMOOTH_FACTOR));\nlookaheadHistoryNext = fixdenorm(smoothLookahead);\n\n// =============================================================================\n// LOOKAHEAD DELAY SYSTEM - Sistema de delay predictivo\n// =============================================================================\n// El lookahead permite al compresor \"ver\" la señal antes de que llegue,\n// evitando transientes que causen overshooting. Las señales de audio se retrasan\n// mientras que la detección trabaja con la señal sin retrasar.\n\n// Convertir tiempo de lookahead (ms) a muestras\nlookaheadSamples = mstosamps(smoothLookahead);\n\n// Leer señales retrasadas para compresión (delayed audio)\nleftDelayedForCompression = leftOutputDelay.read(lookaheadSamples, interp=\"step\");\nrightDelayedForCompression = rightOutputDelay.read(lookaheadSamples, interp=\"step\");\n\n// Asignar salidas de medidores principales (señales que entran a la compresión)\nout4 = leftDelayedForCompression;\nout5 = rightDelayedForCompression;\n\n// Leer señales retrasadas para mezcla final (las escrituras se hacen más abajo después del trim)\nleftDelayedForMixing = leftInputDelay.read(lookaheadSamples, interp=\"step\");\nrightDelayedForMixing = rightInputDelay.read(lookaheadSamples, interp=\"step\");\n\n// =============================================================================\n// MAKEUP GAIN Y SIDECHAIN TAPS\n// =============================================================================\n\n// Makeup Gain - Ganancia de compensación\nsmoothMakeupGain = (makeupGainHistory * SMOOTH_HISTORY_FACTOR) + (i_MAKEUP * SMOOTH_PARAM_FACTOR);\nmakeupGainHistoryNext = fixdenorm(smoothMakeupGain);\n// Duplicar valor para uso en canales L y R\nleftMakeupGain = smoothMakeupGain;\nrightMakeupGain = leftMakeupGain;\n\n// Sidechain Tap - Señal retrasada para procesamiento de sidechain\nsidechainDelayedTap = sidechainTapDelay.read(lookaheadSamples, interp=\"step\");\n\n// Solo Sidechain - Monitoreo de señal de sidechain\nsmoothSoloSidechain = (soloSidechainHistory * SMOOTH_HISTORY_FACTOR) + (m_SOLOSC * SMOOTH_PARAM_FACTOR);\nsoloSidechainHistoryNext = fixdenorm(smoothSoloSidechain);\n\n// Sidechain Write Tap - Señal retrasada para escritura\nsidechainDelayedWrite = sidechainWriteDelay.read(lookaheadSamples, interp=\"step\");\n\n// =============================================================================\n// CONTROL PARAMS - Parámetros de control y bypass\n// =============================================================================\n\n// Bypass - Activación/desactivación del efecto\nbypassInverted = 1 - p_BYPASS;               // Invertir lógica (1=activo, 0=bypass)\nsmoothBypassAmount = (bypassAmountHistory * SMOOTH_HISTORY_FACTOR) + (bypassInverted * SMOOTH_PARAM_FACTOR);\nbypassAmountHistoryNext = fixdenorm(smoothBypassAmount);\n\n// Key Mix - Mezcla entre señal principal y external key\nsmoothKeyMix = (keyMixHistory * SMOOTH_HISTORY_FACTOR) + (r_KEY * SMOOTH_PARAM_FACTOR);\nkeyMixHistoryNext = fixdenorm(smoothKeyMix);\n\n// Dry/Wet Mix - Balance entre señal original y procesada\nsmoothDryWetMix = (dryWetMixHistory * SMOOTH_HISTORY_FACTOR) + (o_DRYWET * SMOOTH_PARAM_FACTOR);\ndryWetMixHistoryNext = fixdenorm(smoothDryWetMix);\n\n// Aplicar bypass de dry/wet cuando DELTA está activo\neffectiveDryWetMix = mix(smoothDryWetMix, deltaWetValue, smoothDeltaMode);\n\n// =============================================================================\n// INPUT PROCESSING - Procesamiento de entradas con trim\n// =============================================================================\n// Trim Principal - Ganancia de entrada\nsmoothTrimGain = (trimHistory * SMOOTH_HISTORY_FACTOR) + (a_TRIM * SMOOTH_PARAM_FACTOR);\ntrimHistoryNext = fixdenorm(smoothTrimGain);\ntrimGainLinear = dbtoa(smoothTrimGain);\n\n// Aplicar trim a entradas\nleftTrimmed = in1 * trimGainLinear;\nrightTrimmed = in2 * trimGainLinear;\n\n// Escribir señales en delays\nleftInputDelay.write(in1);          // Sin trim para bypass/dry\nrightInputDelay.write(in2);         // Sin trim para bypass/dry\nleftOutputDelay.write(leftTrimmed); // Con trim para compresión\nrightOutputDelay.write(rightTrimmed); // Con trim para compresión\n\n// Crear señal mono de entrada principal\nmainSignalMono = ((rightTrimmed + leftTrimmed)) * 0.707;  // Suma mono de entrada principal\ntrimGainReference = trimGainLinear;           // Referencia para normalización posterior\n\n// Sidechain Trim - Ganancia de entrada de sidechain\nsmoothSidechainTrimGain = (sidechainTrimHistory * SMOOTH_HISTORY_FACTOR) + (y_SCTRIM * SMOOTH_PARAM_FACTOR);\nsidechainTrimHistoryNext = fixdenorm(smoothSidechainTrimGain);\nsidechainTrimGainLinear = dbtoa(smoothSidechainTrimGain);\nsidechainTrimReference = sidechainTrimGainLinear;  // Referencia para uso posterior\n\n// Aplicar trim a entradas de sidechain\nleftSidechainTrimmed = in3 * sidechainTrimReference;\nrightSidechainTrimmed = in4 * sidechainTrimReference;\n\n// Asignar salidas de sidechain (sin delay por ahora)\nout6 = leftSidechainTrimmed;\nout7 = rightSidechainTrimmed;\n\n// Crear señal mono de sidechain\nsidechainSignalMono = ((rightSidechainTrimmed + leftSidechainTrimmed)) * 0.707;  // Suma mono de sidechain\n\n// Key mixing - Mezcla entre señal principal y external key input\nkeyMixedSignal = mix(mainSignalMono, sidechainSignalMono, smoothKeyMix);\n\n// Control params asignados directamente\nONSIDECHAIN = smoothScEnable;\nALGO = f_ALGO;\nREACT = smoothReact;\nAUTO_RELEASE = s_AUTORELEASE;  // Directo, sin SPEED\nEXTRA_SMOOTH = smoothSmoothAmount;\n\n// =============================================================================\n// FILTROS DE SIDECHAIN - BIQUAD BUTTERWORTH (Robert Bristow-Johnson)\n// =============================================================================\n// Implementación de filtros biquad de 2º orden (12 dB/oct) con opción de \n// cascadear para 4º orden (24 dB/oct). Transición suave entre órdenes.\n// Basado en las fórmulas de Robert Bristow-Johnson's Audio EQ Cookbook\n\n// Q factor para respuesta Butterworth (maximally flat)\nBUTTERWORTH_Q = 1.0;                         // Q = 1 para respuesta plana sin resonancia\n\n// Coeficientes LPF Butterworth de 2º orden\nomega = ((smoothLpfFreq * twopi)) / samplerate;   // Frecuencia normalizada\nsn = sin(omega);\ncs = cos(omega);\none_over_Q = 1 / BUTTERWORTH_Q;              // Inverso del Q factor\nalpha = ((sn * 0.5)) * one_over_Q;\nb0 = 1 / ((1 + alpha));                     // Normalización\na2 = ((((1 - cs)) * 0.5)) * b0;             // Coeficiente feedforward z^-2\na1 = ((1 - cs)) * b0;                       // Coeficiente feedforward z^-1\nb1 = ((-2 * cs)) * b0;                      // Coeficiente feedback z^-1\nb2 = ((1 - alpha)) * b0;                    // Coeficiente feedback z^-2\n\n// Asignar coeficientes LPF\nlpfCoeffA2 = a2;\nlpfCoeffA1 = a1;\nlpfCoeffA0 = a2;  // a0 = a2 para LPF simétrico\nlpfCoeffB1 = b1;\nlpfCoeffB2 = b2;\n\n// Coeficientes HPF Butterworth de 2º orden\nomega_hpf = ((smoothHpfFreq * twopi)) / samplerate;\nsn_hpf = sin(omega_hpf);\ncs_hpf = cos(omega_hpf);\nalpha_hpf = ((sn_hpf * 0.5)) / BUTTERWORTH_Q;  // Usar mismo Q factor\nb0_hpf = 1 / ((1 + alpha_hpf));\na_hpf = ((((1 + cs_hpf)) * 0.5)) * b0_hpf;   // Coeficiente feedforward\na_neg_hpf = (-(1 + cs_hpf)) * b0_hpf;       // Coeficiente feedforward negativo\nb1_hpf = ((-2 * cs_hpf)) * b0_hpf;\nb2_hpf = ((1 - alpha_hpf)) * b0_hpf;\n\n// Asignar coeficientes HPF\nhpfCoeffA2 = a_hpf;\nhpfCoeffA1 = a_neg_hpf;\nhpfCoeffA0 = a_hpf;  // a0 = a2 para HPF simétrico\nhpfCoeffB1 = b1_hpf;\nhpfCoeffB2 = b2_hpf;\n\n// Pre-procesar señal de sidechain\nsidechainMixedSignal = keyMixedSignal * 0.707;\n\n// =============================================================================\n// LPF - FILTRO PASO BAJO CON OPCIÓN 12/24 dB/oct\n// =============================================================================\n\n// Primera etapa LPF (12 dB/oct)\nlpfInput = sidechainMixedSignal * lpfCoeffA2;\nlpfFromHistory1 = lpfHistory3 * lpfCoeffA1;\nlpfFromHistory2 = lpfHistory1 * lpfCoeffA0;\nlpfFeedback1 = lpfHistory2 * lpfCoeffB2;\nlpfFeedback2 = lpfHistory4 * lpfCoeffB1;\nlpfFilteredSignal = ((((lpfFromHistory2 + lpfFromHistory1)) + lpfInput)) - ((lpfFeedback2 + lpfFeedback1));\n\n// Actualizar historias LPF primera etapa\nlpfHistory1Next = fixdenorm(lpfHistory3);\nlpfHistory2Next = fixdenorm(lpfHistory4);\nlpfHistory3Next = fixdenorm(sidechainMixedSignal);\nlpfHistory4Next = fixdenorm(lpfFilteredSignal);\n\n// Segunda etapa LPF (para 24 dB/oct)\nlpfStage2Input = lpfFilteredSignal * lpfCoeffA2;\nlpfStage2FromHistory1 = lpfHistory7 * lpfCoeffA1;\nlpfStage2FromHistory2 = lpfHistory5 * lpfCoeffA0;\nlpfStage2Feedback1 = lpfHistory6 * lpfCoeffB2;\nlpfStage2Feedback2 = lpfHistory8 * lpfCoeffB1;\nlpfStage2FilteredSignal = ((((lpfStage2FromHistory2 + lpfStage2FromHistory1)) + lpfStage2Input)) - ((lpfStage2Feedback2 + lpfStage2Feedback1));\n\n// Actualizar historias LPF segunda etapa\nlpfHistory5Next = fixdenorm(lpfHistory7);\nlpfHistory6Next = fixdenorm(lpfHistory8);\nlpfHistory7Next = fixdenorm(lpfFilteredSignal);\nlpfHistory8Next = fixdenorm(lpfStage2FilteredSignal);\n\n// Mezclar entre 12dB/oct y 24dB/oct según smoothLpfOrder\nlpfFinalSignal = mix(lpfFilteredSignal, lpfStage2FilteredSignal, smoothLpfOrder);\n\n// Mezclar señal directa con LPF según ONSIDECHAIN\nlpfMixedSignal = mix(sidechainMixedSignal, lpfFinalSignal, ONSIDECHAIN);\n\n// =============================================================================\n// HPF - FILTRO PASO ALTO CON OPCIÓN 12/24 dB/oct\n// =============================================================================\n\n// Primera etapa HPF (12 dB/oct)\nhpfInput = lpfMixedSignal * hpfCoeffA2;\nhpfFromHistory1 = hpfHistory3 * hpfCoeffA1;\nhpfFromHistory2 = hpfHistory2 * hpfCoeffA0;\nhpfFeedback1 = hpfHistory1 * hpfCoeffB2;\nhpfFeedback2 = hpfHistory4 * hpfCoeffB1;\nhpfFilteredSignal = ((((hpfFromHistory2 + hpfFromHistory1)) + hpfInput)) - ((hpfFeedback2 + hpfFeedback1));\n\n// Actualizar historias HPF primera etapa\nhpfHistory1Next = fixdenorm(hpfHistory4);\nhpfHistory2Next = fixdenorm(hpfHistory3);\nhpfHistory3Next = fixdenorm(lpfMixedSignal);\nhpfHistory4Next = fixdenorm(hpfFilteredSignal);\n\n// Segunda etapa HPF (para 24 dB/oct)\nhpfStage2Input = hpfFilteredSignal * hpfCoeffA2;\nhpfStage2FromHistory1 = hpfHistory7 * hpfCoeffA1;\nhpfStage2FromHistory2 = hpfHistory6 * hpfCoeffA0;\nhpfStage2Feedback1 = hpfHistory5 * hpfCoeffB2;\nhpfStage2Feedback2 = hpfHistory8 * hpfCoeffB1;\nhpfStage2FilteredSignal = ((((hpfStage2FromHistory2 + hpfStage2FromHistory1)) + hpfStage2Input)) - ((hpfStage2Feedback2 + hpfStage2Feedback1));\n\n// Actualizar historias HPF segunda etapa\nhpfHistory5Next = fixdenorm(hpfHistory8);\nhpfHistory6Next = fixdenorm(hpfHistory7);\nhpfHistory7Next = fixdenorm(hpfFilteredSignal);\nhpfHistory8Next = fixdenorm(hpfStage2FilteredSignal);\n\n// Mezclar entre 12dB/oct y 24dB/oct según smoothHpfOrder\nhpfFinalSignal = mix(hpfFilteredSignal, hpfStage2FilteredSignal, smoothHpfOrder);\n\n// Mezclar señal con HPF según ONSIDECHAIN y obtener señal final de sidechain\nsidechainProcessedSignal = mix(lpfMixedSignal, hpfFinalSignal, ONSIDECHAIN);\nsidechainDetectionSignal = sidechainProcessedSignal;\n\n// =============================================================================\n// DETECCIÓN DE ENVOLVENTE - Sistema híbrido de 3 algoritmos\n// =============================================================================\n// Los algoritmos y su hibridación han sido elegidos empíricamente por oído\n\n// Detección de envolvente - Preparación de coeficientes\nattack_ms = max(0.05, smoothAttack);\nrelease_ms = max(0.1, smoothRelease);\nattack_coeff = exp(-1 / ((((attack_ms * samplerate)) * 0.001)));\n\n// =============================================================================\n// NUEVO SISTEMA AUTO-RELEASE SIMPLIFICADO\n// =============================================================================\ncurrent_signal_abs = abs(sidechainDetectionSignal);\nsignal_change = abs(current_signal_abs - prevEnvelope);\n\n// Declarar la variable antes del condicional\nfinal_release_time = release_ms;  // Por defecto usar release manual\n\nif(AUTO_RELEASE > 0.5) {\n    // Sistema automático activado\n    \n    // 1. Detección mejorada de transientes\n    // Usar un umbral relativo al nivel actual para mejor detección\n    relative_threshold = max(0.01, current_signal_abs * 0.3);  // 30% del nivel actual\n    is_transient = signal_change > relative_threshold ? 1 : 0;\n    \n    // 2. Acumulador más rápido para mejor respuesta\n    // Usar diferentes velocidades para subida y bajada\n    program_smooth = 0.995;  // Valor por defecto\n    if(is_transient > 0.5) {\n        // Transiente detectado - respuesta rápida\n        program_smooth = 0.95;  // ~20 muestras para detectar percusivo\n    }\n    \n    transientDetector = (transientDetector * program_smooth) + (is_transient * (1 - program_smooth));\n    \n    // 3. Rangos de release ajustados con curva no lineal\n    // Usar pow() para hacer la transición más musical\n    transient_shaped = pow(transientDetector, 1.5);  // Curva exponencial para más separación\n    percussive_release = 15;     // 15ms para material muy percusivo\n    sustained_release = 500;     // 500ms para material sostenido\n    \n    // 4. Interpolación basada en el contenido detectado\n    base_release = mix(sustained_release, percussive_release, transient_shaped);\n    \n    // 5. Modulación dinámica mejorada\n    // Considerar tanto el cambio como el nivel absoluto\n    level_factor = scale(current_signal_abs, 0, 0.5, 1, 0.6, 1);  // Señales fuertes = release más rápido\n    change_factor = scale(signal_change, 0, 0.2, 1, 0.4, 1);      // Cambios grandes = release más rápido\n    instant_mod = min(level_factor, change_factor);               // Usar el más agresivo\n    \n    // 6. Release final\n    final_release_time = base_release * instant_mod;\n    \n    // 7. Límites de seguridad con histéresis suave\n    // Evitar cambios bruscos entre samples consecutivos\n    release_change_limit = 0.7;  // Máximo 70% de cambio entre samples\n    if(prevEnvelope > 0) {\n        max_change = final_release_time * release_change_limit;\n        final_release_time = clamp(final_release_time, \n                                  prevEnvelope - max_change, \n                                  prevEnvelope + max_change);\n    }\n    final_release_time = clamp(final_release_time, 10, 600);\n}\n\n// Calcular coeficiente de release\nrelease_coeff = exp(-1 / ((((final_release_time * samplerate)) * 0.001)));\n\n// Actualizar historia para siguiente sample\nprevEnvelope = current_signal_abs;\n\n// =============================================================================\n// RMS SHARP (para ALGO 0) - Sliding RMS con ventana corta\n// =============================================================================\nrms_window = max(1, floor(25 * (samplerate / 48000)));  // Escalar ventana por SR\nrms_window_inv = 1 / rms_window;                        // Precalcular inverso\ninput_squared = sidechainDetectionSignal * sidechainDetectionSignal;\noldest_sq = rmsDelay.read(rms_window, interp=\"none\");   // Leer muestra más antigua\nrms_sum_new = ((input_squared + rmsSum)) - oldest_sq;   // Cálculo incremental\nrms_sum_clipped = max(0, rms_sum_new);                  // Evitar valores negativos\nrms_value_sharp = sqrt(rms_sum_clipped * rms_window_inv); // RMS final\nrmsSum = rms_sum_clipped;                               // Actualizar suma\nrmsDelay.write(input_squared);                          // Escribir nueva muestra\n\n// =============================================================================\n// ALGORITMO 0: SHARP DETECTOR - Respuesta rápida y precisa\n// =============================================================================\npeakEnvSharp = abs(sidechainDetectionSignal);           // Detección de pico instantánea\nenv_mix_sharp = mix(peakEnvSharp, rms_value_sharp, REACT); // Peak/RMS según REACT\nrelease_env_175 = releaseHistSharp * release_coeff;     // Aplicar release\nenv_post_release_176 = max(env_mix_sharp, release_env_175); // Solo decay, no attack en release\nattack_env_177 = attackHistSharp * attack_coeff;       // Historia de attack\nattack_mix_178 = (((1 - attack_coeff)) * env_post_release_176) + (attack_coeff * attack_env_177); // Mezcla attack\nsharpEnvelopeFinal = max(attack_mix_178, 1e-06);       // Evitar valores nulos\nreleaseHistSharp = fixdenorm(env_post_release_176);     // Actualizar historia release\nattackHistSharp = fixdenorm(attack_mix_178);           // Actualizar historia attack\n\n// =============================================================================\n// ALGORITMO 1: CLASSIC DETECTOR - Comportamiento tradicional\n// =============================================================================\ninput_abs_classic = abs(sidechainDetectionSignal);     // Señal absoluta para Peak\nrms_coeff = exp(-1 / ((((50 * samplerate)) * 0.001))); // RMS fijo: 50ms\n\n// Detector de picos con attack/release separados\nif(input_abs_classic > peakEnvClassic) {\n    peakEnvClassic = (((1 - attack_coeff)) * input_abs_classic) + (attack_coeff * peakEnvClassic);\n}\nelse {\n    peakEnvClassic = (((1 - release_coeff)) * input_abs_classic) + (release_coeff * peakEnvClassic);\n}\n\n// RMS exponencial independiente\ninput_sq_classic = sidechainDetectionSignal * sidechainDetectionSignal;\nrmsSqClassic = (rmsSqClassic * rms_coeff) + (input_sq_classic * ((1 - rms_coeff))); // RMS cuadrático\nrms_env_classic_raw = sqrt(rmsSqClassic);              // Raíz cuadrada = RMS real\n\n// Envolvente para RMS con attack/release propios\nif(rms_env_classic_raw > histRmsClassic) {\n    histRmsClassic = (((1 - attack_coeff)) * rms_env_classic_raw) + (attack_coeff * histRmsClassic);\n}\nelse {\n    histRmsClassic = (((1 - release_coeff)) * rms_env_classic_raw) + (release_coeff * histRmsClassic);\n}\n\nenv_mix_classic = mix(peakEnvClassic, histRmsClassic, REACT); // Peak/RMS según REACT\n\n// =============================================================================\n// ALGORITMO 2: SLOW DETECTOR - Respuesta suave y musical\n// =============================================================================\ninput_abs_slow = abs(sidechainDetectionSignal);        // Señal absoluta para Peak\nrms_coeff_slow = exp(-1 / ((((200 * samplerate)) * 0.001))); // RMS lento: 200ms\n\n// Detector de picos con attack/release separados\nif(input_abs_slow > peakEnvSlow) {\n    peakEnvSlow = (((1 - attack_coeff)) * input_abs_slow) + (attack_coeff * peakEnvSlow);\n}\nelse {\n    peakEnvSlow = (((1 - release_coeff)) * input_abs_slow) + (release_coeff * peakEnvSlow);\n}\n\n// RMS exponencial lento\ninput_sq_slow = sidechainDetectionSignal * sidechainDetectionSignal;\nrmsSqSlow = (rmsSqSlow * rms_coeff_slow) + (input_sq_slow * ((1 - rms_coeff_slow))); // RMS cuadrático\nrms_env_slow_raw = sqrt(rmsSqSlow);                    // Raíz cuadrada = RMS real\n\n// Envolvente para RMS con attack/release propios\nif(rms_env_slow_raw > histRmsSlow) {\n    histRmsSlow = (((1 - attack_coeff)) * rms_env_slow_raw) + (attack_coeff * histRmsSlow);\n}\nelse {\n    histRmsSlow = (((1 - release_coeff)) * rms_env_slow_raw) + (release_coeff * histRmsSlow);\n}\n\nenv_mix_slow = mix(peakEnvSlow, histRmsSlow, REACT);   // Peak/RMS según REACT\n\n// =============================================================================\n// EXTRA SMOOTHING POST-DETECCIÓN - Suavizado adicional configurable\n// =============================================================================\nsmooth_amount_shaped = pow(EXTRA_SMOOTH, 2.5);         // Curva exponencial para control musical\nsmooth_time_ms = mix(0.001, 100, smooth_amount_shaped); // Mapeo a rango temporal\nextra_smooth_coeff = exp(-1 / ((((smooth_time_ms * samplerate)) * 0.001))); // Coeficiente de filtro\n\n// Algoritmo 0 - Salida en dB con smoothing opcional\nexpr_algo0 = atodb(sharpEnvelopeFinal);               // Conversión a dB base\nif(EXTRA_SMOOTH >= 0.001) {                           // Solo si hay smoothing significativo\n    extraSmoothedSharpEnv = (extraSmoothHistSharp * extra_smooth_coeff) + (sharpEnvelopeFinal * ((1 - extra_smooth_coeff)));\n    extraSmoothHistSharp = fixdenorm(extraSmoothedSharpEnv);\n    expr_algo0 = atodb(max(extraSmoothedSharpEnv, 1e-06)); // Evitar log(0)\n}\n\n// Algoritmo 1 - Salida en dB con smoothing opcional\nexpr_algo1 = atodb(max(env_mix_classic, 1e-06));      // Conversión a dB base\nif(EXTRA_SMOOTH >= 0.001) {\n    extraSmoothedClassicEnv = (extraSmoothHistClassic * extra_smooth_coeff) + (env_mix_classic * ((1 - extra_smooth_coeff)));\n    extraSmoothHistClassic = fixdenorm(extraSmoothedClassicEnv);\n    expr_algo1 = atodb(max(extraSmoothedClassicEnv, 1e-06));\n}\n\n// Algoritmo 2 - Salida en dB con smoothing opcional\nexpr_algo2 = atodb(max(env_mix_slow, 1e-06));         // Conversión a dB base\nif(EXTRA_SMOOTH >= 0.001) {\n    extraSmoothedSlowEnv = (extraSmoothHistSlow * extra_smooth_coeff) + (env_mix_slow * ((1 - extra_smooth_coeff)));\n    extraSmoothHistSlow = fixdenorm(extraSmoothedSlowEnv);\n    expr_algo2 = atodb(max(extraSmoothedSlowEnv, 1e-06));\n}\n\n// =============================================================================\n// SELECTOR Y MORPHING DE ALGORITMOS - Transición continua sin saltos\n// =============================================================================\ndetector_control = clamp(ALGO, 0, 2);                 // Asegurar rango válido\nfade_amount = detector_control - 1;                    // Calcular fade para segunda transición\n// Morphing en dos etapas: 0→1 (Sharp↔Classic), 1→2 (Classic↔Slow)\nenvelope_out = detector_control <= 1 ? mix(expr_algo0, expr_algo1, detector_control) : mix(expr_algo1, expr_algo2, fade_amount);\nenvelopeDetectorOutput = envelope_out;                 // Salida para debug/monitoring\nfinalEnvelopeDb = envelopeDetectorOutput;              // Envolvente final en dB\n\n// =============================================================================\n// COMPENSACIÓN DE GANANCIA PARA SOLO SIDECHAIN - Evitar saltos abruptos\n// =============================================================================\nunitaryReference = int(1);                           // Valor de referencia unitario\ntrimCompensationFactor = unitaryReference / trimGainReference; // Factor de compensación inverso al trim\nsidechainTrimCompensated = sidechainDetectionSignal * trimCompensationFactor; // Aplicar compensación de trim\nnormalizedSidechainSignal = sidechainTrimCompensated; // Señal compensada para monitoreo\n\n// =============================================================================\n// PROCESAMIENTO DE SALIDA - Canal Izquierdo\n// =============================================================================\n// Flujo completo de procesamiento: Compresión → Delta → Auto Makeup →\n// Parallel → Dry/Wet → Solo Sidechain → Output Processing → Bypass\n\n// COMPRESIÓN BÁSICA - Aplicar gain reduction calculado\ncompressionGainReduction = softkneeLinear(finalEnvelopeDb, smoothThresholdDb, smoothRatio, smoothKnee);\ngainReductionDb = compressionGainReduction - finalEnvelopeDb;  // Diferencia = reducción real\ngainReductionLinear = dbtoa(gainReductionDb);                  // Convertir a factor lineal\ncompressedLeft = leftDelayedForCompression * gainReductionLinear; // Aplicar compresión\n\n// MODO DELTA - Señal diferencia (experimental)\ndelta_control = clamp(smoothDeltaMode, 0, 1);                 // Asegurar rango válido\ndelta_signal = leftDelayedForCompression - compressedLeft;     // Diferencia = lo que se quitó\nleftWithDelta = mix(compressedLeft, delta_signal, delta_control); // Mezclar según control\n\n// AUTO MAKEUP - Compensación automática de ganancia (70% del gain reduction)\nautoMakeupGainReduction = gainReductionDb;                    // Usar gain reduction como referencia\nreduction_smooth_mul = reductionHistLeft * SMOOTH_HISTORY_FACTOR; // Suavizar historia\nreduction_current_mul = autoMakeupGainReduction * SMOOTH_PARAM_FACTOR; // Suavizar actual\nreduction_smoothed = reduction_current_mul + reduction_smooth_mul; // Combinar suavizado\nauto_makeup = reduction_smoothed * AUTO_MAKEUP_FACTOR;        // Aplicar factor -0.7 (70%)\ncombined_makeup = auto_makeup + leftMakeupGain;               // Combinar con makeup manual\nfinal_makeup_db = mix(leftMakeupGain, combined_makeup, effectiveAutoGain); // Usar effectiveAutoGain (afectado por DELTA)\n// Cuando DELTA está activo, forzar makeup a 0dB\nfinalMakeupDbWithDelta = mix(final_makeup_db, 0, smoothDeltaMode); // 0dB cuando DELTA=1\nmakeup_linear = dbtoa(finalMakeupDbWithDelta);            // Convertir a factor lineal\nleftWithMakeup = leftWithDelta * makeup_linear;               // Aplicar makeup final\nreductionHistLeft = fixdenorm(reduction_smoothed);            // Actualizar historia\n\n// COMPRESIÓN PARALELA - Mezcla de señal comprimida con señal original\nparallel_control = clamp(effectiveParallelMix, 0, 1);        // Usar effectiveParallelMix (afectado por DELTA)\nparallel_compressed = leftWithMakeup * parallel_control;      // Señal comprimida escalada\nparallel_mix = leftDelayedForCompression + parallel_compressed; // Sumar original + comprimida\nleftParallelMixed = mix(leftWithMakeup, parallel_mix, parallel_control); // Mezclar según control\n\n// MIX DRY/WET - Balance entre señal original y procesada\nleftDryWetMixed = mix(leftDelayedForMixing, leftParallelMixed, effectiveDryWetMix); // Usar effectiveDryWetMix (afectado por DELTA)\nleftWithSidechain = mix(leftDryWetMixed, sidechainDelayedWrite, smoothSoloSidechain); // Solo sidechain\n\n// OUTPUT PROCESSING - Saturación asimétrica (Off/Soft)\nmaxOutput = 0.989; // Limitar a -0.1 dBFS (0.989 lineal) para dejar headroom\nsaturationAmount = clamp(smoothSoftclip, 0, 1);                  // Cantidad de saturación (0=Off, 1=Full)\n// Cuando DELTA está activo, bypasear también el softclipping\neffectiveSaturationAmount = mix(saturationAmount, 0, smoothDeltaMode); // 0 cuando DELTA=1\n\n// Saturación asimétrica para añadir color/calidez\nleftSaturated = 0;  // Declarar variable\nif(leftWithSidechain > 0) {\n    // Semiciclo positivo - más saturación (genera armónicos pares)\n    leftSaturated = tanh(leftWithSidechain * 1.2) * 0.833;   // Compensar ganancia\n}\nelse {\n    // Semiciclo negativo - menos saturación\n    leftSaturated = tanh(leftWithSidechain * 0.8) * 1.25;    // Compensar ganancia\n}\nleftSaturated = clamp(leftSaturated, -maxOutput, maxOutput);\n\n// Mezclar señal original con saturada según el parámetro\nleftProcessedOutput = mix(leftWithSidechain, leftSaturated, effectiveSaturationAmount);\nleftFinalOutput = mix(leftDelayedForMixing, leftProcessedOutput, smoothBypassAmount); // Bypass final\nout1 = leftFinalOutput;                                       // Salida canal izquierdo\n\n// =============================================================================\n// PROCESAMIENTO DE SALIDA - Canal Derecho\n// =============================================================================\n// Idéntico al canal izquierdo pero con variables propias para claridad.\n\n// COMPRESIÓN BÁSICA - Aplicar gain reduction calculado (compartido entre canales)\nrightGainReductionDb = compressionGainReduction - finalEnvelopeDb; // Misma reducción que L\nrightGainReductionLinear = dbtoa(rightGainReductionDb);             // Convertir a factor lineal\ncompressedRight = rightDelayedForCompression * rightGainReductionLinear; // Aplicar compresión\n\n// MODO DELTA - Señal diferencia (experimental)\nrightDeltaControl = clamp(smoothDeltaMode, 0, 1);               // Asegurar rango válido\nrightDeltaSignal = rightDelayedForCompression - compressedRight; // Diferencia = lo que se quitó\nrightWithDelta = mix(compressedRight, rightDeltaSignal, rightDeltaControl); // Mezclar según control\n\n// AUTO MAKEUP - Compensación automática de ganancia (70% del gain reduction)\nrightAutoMakeupGainReduction = rightGainReductionDb;              // Usar gain reduction como referencia\nrightReductionSmoothMul = reductionHistRight * SMOOTH_HISTORY_FACTOR; // Suavizar historia\nrightReductionCurrentMul = rightAutoMakeupGainReduction * SMOOTH_PARAM_FACTOR; // Suavizar actual\nrightReductionSmoothed = rightReductionCurrentMul + rightReductionSmoothMul; // Combinar suavizado\nrightAutoMakeup = rightReductionSmoothed * AUTO_MAKEUP_FACTOR;    // Aplicar factor -0.7 (70%)\nrightCombinedMakeup = rightAutoMakeup + rightMakeupGain;          // Combinar con makeup manual\nrightFinalMakeupDb = mix(rightMakeupGain, rightCombinedMakeup, effectiveAutoGain); // Usar effectiveAutoGain\n// Cuando DELTA está activo, forzar makeup a 0dB\nrightFinalMakeupDbWithDelta = mix(rightFinalMakeupDb, 0, smoothDeltaMode); // 0dB cuando DELTA=1\nrightMakeupLinear = dbtoa(rightFinalMakeupDbWithDelta);         // Convertir a factor lineal\nrightWithMakeup = rightWithDelta * rightMakeupLinear;             // Aplicar makeup final\nreductionHistRight = fixdenorm(rightReductionSmoothed);           // Actualizar historia\n\n// COMPRESIÓN PARALELA - Mezcla de señal comprimida con señal original\nrightParallelControl = clamp(effectiveParallelMix, 0, 1);        // Usar effectiveParallelMix\nrightParallelCompressed = rightWithMakeup * rightParallelControl; // Señal comprimida escalada\nrightParallelMix = rightDelayedForCompression + rightParallelCompressed; // Sumar original + comprimida\nrightParallelMixed = mix(rightWithMakeup, rightParallelMix, rightParallelControl); // Mezclar según control\n\n// MIX DRY/WET - Balance entre señal original y procesada\nrightDryWetMixed = mix(rightDelayedForMixing, rightParallelMixed, effectiveDryWetMix); // Usar effectiveDryWetMix\nrightWithSidechain = mix(rightDryWetMixed, sidechainDelayedTap, smoothSoloSidechain); // Solo sidechain\n\n// OUTPUT PROCESSING - Saturación asimétrica (Off/Soft)\nrightSaturationAmount = clamp(smoothSoftclip, 0, 1);             // Cantidad de saturación (0=Off, 1=Full)\n// Cuando DELTA está activo, bypasear también el softclipping\nrightEffectiveSaturationAmount = mix(rightSaturationAmount, 0, smoothDeltaMode); // 0 cuando DELTA=1\n\n// Saturación asimétrica para añadir color/calidez\nrightSaturated = 0;  // Declarar variable\nif(rightWithSidechain > 0) {\n    // Semiciclo positivo - más saturación (genera armónicos pares)\n    rightSaturated = tanh(rightWithSidechain * 1.2) * 0.833;  // Compensar ganancia\n}\nelse {\n    // Semiciclo negativo - menos saturación\n    rightSaturated = tanh(rightWithSidechain * 0.8) * 1.25;   // Compensar ganancia\n}\nrightSaturated = clamp(rightSaturated, -maxOutput, maxOutput);\n\n// Mezclar señal original con saturada según el parámetro\nrightProcessedOutput = mix(rightWithSidechain, rightSaturated, rightEffectiveSaturationAmount);\nrightFinalOutput = mix(rightDelayedForMixing, rightProcessedOutput, smoothBypassAmount); // Bypass final\nout2 = rightFinalOutput;                                      // Salida canal derecho\n\n// =============================================================================\n// GAIN REDUCTION OUTPUT - Medidor de compresión para UI\n// =============================================================================\n// Genera señal de monitoreo para el medidor de gain reduction en la interfaz.\n// Mezcla entre valor unitario (sin compresión) y gain reduction real según bypass.\n\ngainReductionMeter = mix(unitaryReference, rightGainReductionLinear, bypassInverted); // 1=activo, 0=bypass\ngainReductionOutput = min(gainReductionMeter, 1);              // Limitar a máximo 1.0\nout3 = gainReductionOutput;                                    // Salida para medidor GR\n\n// =============================================================================\n// ACTUALIZACIÓN DE HISTORIAS PARA EL SIGUIENTE CICLO\n// =============================================================================\n// Todas las historias se actualizan al final del ciclo de procesamiento\n\n// Historias de smoothing de parámetros\nparallelMixHistory = parallelMixHistoryNext;\ndeltaModeHistory = deltaModeHistoryNext;\nautoGainHistory = autoGainHistoryNext;\nthresholdHistory = thresholdHistoryNext;\nmakeupGainHistory = makeupGainHistoryNext;\nsoloSidechainHistory = soloSidechainHistoryNext;\nbypassAmountHistory = bypassAmountHistoryNext;\nkeyMixHistory = keyMixHistoryNext;\ndryWetMixHistory = dryWetMixHistoryNext;\ntrimHistory = trimHistoryNext;\nsidechainTrimHistory = sidechainTrimHistoryNext;\n\n// Historias adicionales de smoothing\nratioHistory = ratioHistoryNext;\nkneeHistory = kneeHistoryNext;\nreactHistory = reactHistoryNext;\nsmoothAmountHistory = smoothAmountHistoryNext;\nlookaheadHistory = lookaheadHistoryNext;\nhpfFreqHistory = hpfFreqHistoryNext;\nlpfFreqHistory = lpfFreqHistoryNext;\nscEnableHistory = scEnableHistoryNext;\nsoftclipHistory = softclipHistoryNext;\natkHistory = atkHistoryNext;\nrelHistory = relHistoryNext;\n\n// Historias de filtros\nlpfHistory1 = lpfHistory1Next;\nlpfHistory2 = lpfHistory2Next;\nlpfHistory3 = lpfHistory3Next;\nlpfHistory4 = lpfHistory4Next;\nlpfHistory5 = lpfHistory5Next;\nlpfHistory6 = lpfHistory6Next;\nlpfHistory7 = lpfHistory7Next;\nlpfHistory8 = lpfHistory8Next;\nhpfHistory1 = hpfHistory1Next;\nhpfHistory2 = hpfHistory2Next;\nhpfHistory3 = hpfHistory3Next;\nhpfHistory4 = hpfHistory4Next;\nhpfHistory5 = hpfHistory5Next;\nhpfHistory6 = hpfHistory6Next;\nhpfHistory7 = hpfHistory7Next;\nhpfHistory8 = hpfHistory8Next;\nhpfOrderHistory = hpfOrderHistoryNext;\nlpfOrderHistory = lpfOrderHistoryNext;\n\n// Delays de lookahead (escritura para el siguiente ciclo)\nsidechainWriteDelay.write(normalizedSidechainSignal);\nsidechainTapDelay.write(normalizedSidechainSignal);\n// Las escrituras de leftOutputDelay y rightOutputDelay se hacen ahora arriba con el trim",
									"fontface" : 0,
									"fontname" : "<Monospaced>",
									"fontsize" : 12.0,
									"id" : "obj-13",
									"maxclass" : "codebox",
									"numinlets" : 4,
									"numoutlets" : 7,
									"outlettype" : [ "", "", "", "", "", "", "" ],
									"patching_rect" : [ 64.0, 60.0, 831.0, 773.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 606.0, 28.0, 28.0, 22.0 ],
									"text" : "in 3"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 876.0, 28.0, 28.0, 22.0 ],
									"text" : "in 4"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-10",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 876.0, 848.0, 35.0, 22.0 ],
									"text" : "out 7"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 741.0, 848.0, 35.0, 22.0 ],
									"text" : "out 6"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 605.333333333333371, 848.0, 35.0, 22.0 ],
									"text" : "out 5"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 470.0, 848.0, 35.0, 22.0 ],
									"text" : "out 4"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 335.0, 848.0, 35.0, 22.0 ],
									"text" : "out 3"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 200.0, 848.0, 35.0, 22.0 ],
									"text" : "out 2"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 64.0, 28.0, 28.0, 22.0 ],
									"text" : "in 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 335.0, 28.0, 28.0, 22.0 ],
									"text" : "in 2"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 64.0, 848.0, 35.0, 22.0 ],
									"text" : "out 1"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 2 ],
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 3 ],
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"source" : [ "obj-13", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-13", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"source" : [ "obj-13", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"source" : [ "obj-13", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"source" : [ "obj-13", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"source" : [ "obj-13", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 1 ],
									"source" : [ "obj-2", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 153.0, 538.0, 314.0, 22.0 ],
					"saved_object_attributes" : 					{
						"autoexport" : 0,
						"exportfolder" : "Sequoia:/Users/jcb/JUCEProjects/JCBComp/exported-code/"
					}
,
					"text" : "gen~ @title JCBCompressor @autoexport 0"
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.325490196078431, 0.054901960784314, 0.482352941176471, 0.3 ],
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-103",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 636.0, 402.0, 494.0, 262.0 ],
					"proportion" : 0.5
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.054901960784314, 0.482352941176471, 0.137254901960784, 0.3 ],
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-104",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1382.0, 400.0, 399.0, 265.0 ],
					"proportion" : 0.5
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.101960784313725, 0.258823529411765, 0.427450980392157, 0.3 ],
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-105",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1155.0, 402.0, 204.0, 262.0 ],
					"proportion" : 0.5
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"bgcolor" : [ 0.556862745098039, 0.392156862745098, 0.203921568627451, 0.3 ],
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-102",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 956.0, 145.0, 518.0, 209.0 ],
					"proportion" : 0.5
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.92156862745098, 0.909803921568627, 1.0, 0.6 ],
					"candicane7" : [ 0.733333333333333, 0.035294117647059, 0.788235294117647, 1.0 ],
					"contdata" : 2,
					"id" : "obj-5",
					"maxclass" : "multislider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 288.0, 771.0, 24.0, 62.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 844.0, 102.0, 8.0, 214.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 1 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "multislider",
							"parameter_mmax" : 0.0,
							"parameter_mmin" : -72.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "multislider",
							"parameter_type" : 0
						}

					}
,
					"setminmax" : [ -72.0, 0.0 ],
					"setstyle" : 1,
					"slidercolor" : [ 0.092923320829868, 0.189730256795883, 0.218167155981064, 1.0 ],
					"varname" : "multislider"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.164705882352941, 0.298039215686275, 0.603921568627451, 0.66 ],
					"id" : "obj-181",
					"maxclass" : "multislider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 36.0, 776.0, 112.0, 52.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 515.0, 216.0, 327.0, 102.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 1 ],
							"parameter_invisible" : 1,
							"parameter_longname" : "multislider[2]",
							"parameter_mmax" : 0.0,
							"parameter_mmin" : -50.0,
							"parameter_modmode" : 0,
							"parameter_shortname" : "multislider",
							"parameter_type" : 3
						}

					}
,
					"setminmax" : [ -50.0, 0.0 ],
					"setstyle" : 5,
					"shadowperbar" : 0,
					"shadowproportion" : 1.0,
					"slidercolor" : [ 0.075975470244884, 0.141983732581139, 0.165274441242218, 1.0 ],
					"varname" : "multislider[2]"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.501960784313725, 0.192156862745098, 0.592156862745098, 0.54 ],
					"id" : "obj-170",
					"maxclass" : "multislider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 480.0, 777.0, 112.0, 52.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 515.0, 104.0, 327.0, 110.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 1 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "multislider[1]",
							"parameter_mmax" : 0.0,
							"parameter_mmin" : -50.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "multislider",
							"parameter_type" : 0
						}

					}
,
					"setminmax" : [ -50.0, 0.0 ],
					"setstyle" : 5,
					"slidercolor" : [ 0.094117647058824, 0.188235294117647, 0.219607843137255, 1.0 ],
					"varname" : "multislider[1]"
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"forceaspect" : 1,
					"id" : "obj-106",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 1523.0, 28.0, 271.0, 77.397600000000011 ],
					"pic" : "/Users/jcb/JUCEProjects/JCBComp/Assets/fondo.png",
					"presentation" : 1,
					"presentation_rect" : [ 16.0, 22.0, 1310.0, 374.136000000000024 ]
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"background" : 1,
					"bgcolor" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"border" : 3,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-71",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 616.0, 9.0, 1199.0, 847.0 ],
					"proportion" : 0.5
				}

			}
, 			{
				"box" : 				{
					"angle" : 270.0,
					"background" : 1,
					"bgcolor" : [ 0.208680531953877, 0.20868047419733, 0.208680489290039, 0.0 ],
					"border" : 3,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-160",
					"maxclass" : "panel",
					"mode" : 0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 17.0, 9.0, 587.0, 847.0 ],
					"proportion" : 0.5
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 1 ],
					"midpoints" : [ 211.666666666666657, 588.086046511627956, 203.5, 588.086046511627956 ],
					"order" : 1,
					"source" : [ "obj-1", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"midpoints" : [ 162.5, 588.086046511627956, 165.5, 588.086046511627956 ],
					"order" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-172", 0 ],
					"midpoints" : [ 359.166666666666629, 568.17578125, 489.5, 568.17578125 ],
					"order" : 0,
					"source" : [ "obj-1", 4 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-172", 0 ],
					"midpoints" : [ 310.0, 570.05859375, 489.5, 570.05859375 ],
					"order" : 0,
					"source" : [ "obj-1", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-176", 0 ],
					"source" : [ "obj-1", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-178", 0 ],
					"midpoints" : [ 211.666666666666657, 576.0, 45.5, 576.0 ],
					"order" : 2,
					"source" : [ "obj-1", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-178", 0 ],
					"midpoints" : [ 162.5, 576.0, 45.5, 576.0 ],
					"order" : 2,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 0 ],
					"order" : 1,
					"source" : [ "obj-1", 3 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 0 ],
					"midpoints" : [ 162.5, 587.086046511627956, 146.0, 587.086046511627956 ],
					"order" : 1,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-48", 0 ],
					"midpoints" : [ 211.666666666666657, 587.086046511627956, 225.0, 587.086046511627956 ],
					"order" : 0,
					"source" : [ "obj-1", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-56", 0 ],
					"order" : 1,
					"source" : [ "obj-1", 4 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-68", 0 ],
					"source" : [ "obj-1", 6 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-79", 0 ],
					"source" : [ "obj-1", 5 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-101", 0 ],
					"source" : [ "obj-100", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-95", 0 ],
					"midpoints" : [ 439.5, 291.113390261627956, 409.5, 291.113390261627956 ],
					"source" : [ "obj-101", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"midpoints" : [ 190.5, 292.953234011627956, 191.5, 292.953234011627956 ],
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-70", 0 ],
					"source" : [ "obj-113", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-57", 0 ],
					"midpoints" : [ 769.5, 351.086046511627956, 808.5, 351.086046511627956 ],
					"source" : [ "obj-114", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-35", 0 ],
					"midpoints" : [ 846.5, 275.121202761627956, 884.5, 275.121202761627956 ],
					"source" : [ "obj-115", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-83", 0 ],
					"midpoints" : [ 817.5, 382.933702761627956, 959.5, 382.933702761627956 ],
					"source" : [ "obj-116", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-29", 0 ],
					"midpoints" : [ 704.5, 759.175890261627956, 666.88671875, 759.175890261627956, 666.88671875, 572.484484011627956, 689.5, 572.484484011627956 ],
					"source" : [ "obj-117", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"midpoints" : [ 842.5, 753.914171511627956, 786.0625, 753.914171511627956, 786.0625, 644.086046511627956, 786.1015625, 644.086046511627956, 786.1015625, 575.078234011627956, 804.5, 575.078234011627956 ],
					"source" : [ "obj-118", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-67", 0 ],
					"midpoints" : [ 959.5, 740.086046511627956, 918.0, 740.086046511627956, 918.0, 572.691515261627956, 932.5, 572.691515261627956 ],
					"source" : [ "obj-119", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-75", 0 ],
					"midpoints" : [ 1059.5, 775.289171511627956, 651.5390625, 775.289171511627956, 651.5390625, 563.086046511627956, 1034.5, 563.086046511627956 ],
					"source" : [ "obj-120", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"source" : [ "obj-121", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"source" : [ "obj-122", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1286.5, 386.992296511627956, 1142.89453125, 386.992296511627956, 1142.89453125, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-49", 0 ],
					"midpoints" : [ 1492.5, 369.961046511627956, 1398.5, 369.961046511627956 ],
					"source" : [ "obj-131", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-47", 0 ],
					"midpoints" : [ 1577.5, 249.699327761627956, 1492.5, 249.699327761627956 ],
					"source" : [ "obj-132", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-61", 0 ],
					"midpoints" : [ 1662.5, 258.121202761627956, 1600.5, 258.121202761627956 ],
					"source" : [ "obj-133", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"midpoints" : [ 1422.5, 756.824327761627956, 1392.0, 756.824327761627956, 1392.0, 547.644640261627956, 1414.5, 547.644640261627956 ],
					"source" : [ "obj-134", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 0 ],
					"midpoints" : [ 1510.5, 752.086046511627956, 1585.19921875, 752.086046511627956, 1585.19921875, 632.086046511627956, 1585.359375, 632.086046511627956, 1585.359375, 551.086046511627956, 1489.5, 551.086046511627956 ],
					"source" : [ "obj-135", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-30", 0 ],
					"source" : [ "obj-138", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-51", 0 ],
					"midpoints" : [ 979.5, 122.086046511627956, 979.5, 122.086046511627956 ],
					"source" : [ "obj-139", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1089.5, 386.359484011627956, 1142.0, 386.359484011627956, 1142.0, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-14", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-98", 0 ],
					"midpoints" : [ 1756.5, 278.586046511627956, 1757.34375, 278.586046511627956, 1757.34375, 382.836046511627956, 1699.5, 382.836046511627956 ],
					"source" : [ "obj-140", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-148", 0 ],
					"source" : [ "obj-143", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-149", 0 ],
					"source" : [ "obj-143", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-154", 0 ],
					"midpoints" : [ 1391.5, 143.086046511627956, 1065.0, 143.086046511627956, 1065.0, 227.086046511627956, 1013.5, 227.086046511627956 ],
					"source" : [ "obj-148", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-154", 0 ],
					"midpoints" : [ 1443.5, 143.086046511627956, 1065.0, 143.086046511627956, 1065.0, 227.086046511627956, 1013.5, 227.086046511627956 ],
					"source" : [ "obj-149", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1163.5, 389.058702761627956, 1141.5, 389.058702761627956 ],
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-155", 0 ],
					"midpoints" : [ 1383.5, 239.586046511627956, 1403.5, 239.586046511627956 ],
					"source" : [ "obj-150", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-155", 0 ],
					"midpoints" : [ 1331.5, 239.586046511627956, 1403.5, 239.586046511627956 ],
					"source" : [ "obj-151", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-150", 0 ],
					"source" : [ "obj-152", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-151", 0 ],
					"source" : [ "obj-152", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-41", 0 ],
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-170", 0 ],
					"midpoints" : [ 535.5, 766.90625, 489.5, 766.90625 ],
					"source" : [ "obj-162", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-173", 0 ],
					"source" : [ "obj-172", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-162", 0 ],
					"source" : [ "obj-173", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-177", 0 ],
					"source" : [ "obj-176", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"source" : [ "obj-177", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-179", 0 ],
					"source" : [ "obj-178", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-180", 0 ],
					"source" : [ "obj-179", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1273.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-181", 0 ],
					"midpoints" : [ 91.5, 769.0, 45.5, 769.0 ],
					"source" : [ "obj-180", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-15", 0 ],
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"midpoints" : [ 284.5, 291.523546511627956, 191.5, 291.523546511627956 ],
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-14", 0 ],
					"source" : [ "obj-22", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"color" : [ 0.0, 0.0, 0.0, 0.548708545918367 ],
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 159.5, 526.988390261627956, 162.5, 526.988390261627956 ],
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-40", 0 ],
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"midpoints" : [ 219.5, 292.714952761627956, 191.5, 292.714952761627956 ],
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"midpoints" : [ 298.5, 763.293023255813978, 297.5, 763.293023255813978 ],
					"source" : [ "obj-28", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-86", 0 ],
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 804.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 652.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 884.5, 518.839952761627956, 1141.6796875, 518.839952761627956, 1141.6796875, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"color" : [ 0.0, 0.0, 0.0, 0.548708545918367 ],
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 133.5, 176.586046511627956, 133.5546875, 176.586046511627956, 133.5546875, 526.105577761627956, 162.5, 526.105577761627956 ],
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1489.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1414.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-41", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1492.5, 521.761827761627956, 1140.06640625, 521.761827761627956, 1140.06640625, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-42", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1398.5, 521.171984011627956, 1226.0, 521.171984011627956, 1226.0, 521.359484011627956, 1142.0, 521.359484011627956, 1142.0, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1642.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 979.5, 234.714952761627956, 1164.1953125, 234.714952761627956, 1164.1953125, 388.531359011627956, 1142.81640625, 388.531359011627956, 1142.81640625, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-42", 0 ],
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1600.5, 519.257921511627956, 1600.0390625, 519.257921511627956, 1600.0390625, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-45", 0 ],
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-54", 0 ],
					"source" : [ "obj-53", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1221.5, 386.531359011627956, 1141.5, 386.531359011627956 ],
					"source" : [ "obj-54", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-62", 0 ],
					"source" : [ "obj-57", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1198.5, 720.230577761627956, 1141.5, 720.230577761627956 ],
					"source" : [ "obj-59", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1214.5, 521.207140261627956, 1141.84765625, 521.207140261627956, 1141.84765625, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-50", 0 ],
					"source" : [ "obj-61", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 808.5, 518.086046511627956, 1016.0, 518.086046511627956, 1016.0, 518.675890261627956, 1141.921875, 518.675890261627956, 1141.921875, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-62", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-59", 0 ],
					"source" : [ "obj-63", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-63", 0 ],
					"source" : [ "obj-65", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 933.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-66", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-66", 0 ],
					"source" : [ "obj-67", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-77", 0 ],
					"source" : [ "obj-70", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 73.5, 525.586046511627956, 162.5, 525.586046511627956 ],
					"source" : [ "obj-72", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1034.5, 643.914171511627956, 1141.5, 643.914171511627956 ],
					"source" : [ "obj-73", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-73", 0 ],
					"source" : [ "obj-75", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 738.5, 518.086046511627956, 1016.0, 518.086046511627956, 1016.0, 518.734484011627956, 1142.9921875, 518.734484011627956, 1142.9921875, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-77", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 3 ],
					"order" : 0,
					"source" : [ "obj-78", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 2 ],
					"order" : 1,
					"source" : [ "obj-78", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 979.5, 388.054796511627956, 1142.0, 388.054796511627956, 1142.0, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-80", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 959.5, 518.086046511627956, 1140.29296875, 518.086046511627956, 1140.29296875, 587.086046511627956, 1141.0546875, 587.086046511627956, 1141.0546875, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-81", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-81", 0 ],
					"source" : [ "obj-83", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 689.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-86", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-143", 0 ],
					"midpoints" : [ 979.5, 293.238390261627956, 1065.0, 293.238390261627956, 1065.0, 119.086046511627956, 1368.0, 119.086046511627956, 1368.0, 32.086046511627956, 1391.5, 32.086046511627956 ],
					"order" : 0,
					"source" : [ "obj-89", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-80", 0 ],
					"order" : 1,
					"source" : [ "obj-89", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 1 ],
					"midpoints" : [ 242.5, 420.492296511627956, 260.833333333333314, 420.492296511627956 ],
					"source" : [ "obj-9", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 191.5, 428.0, 162.5, 428.0 ],
					"source" : [ "obj-9", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1364.5, 388.660265261627956, 1142.0, 388.660265261627956, 1142.0, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-91", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-152", 0 ],
					"midpoints" : [ 1364.5, 287.086046511627956, 1350.0, 287.086046511627956, 1350.0, 253.164171511627956, 1308.0, 253.164171511627956, 1308.0, 155.086046511627956, 1331.5, 155.086046511627956 ],
					"order" : 1,
					"source" : [ "obj-92", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-91", 0 ],
					"order" : 0,
					"source" : [ "obj-92", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-78", 0 ],
					"source" : [ "obj-93", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 3 ],
					"source" : [ "obj-95", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 2 ],
					"source" : [ "obj-95", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-95", 0 ],
					"midpoints" : [ 439.5, 203.777452761627956, 409.5, 203.777452761627956 ],
					"source" : [ "obj-96", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-95", 0 ],
					"midpoints" : [ 410.5, 254.586046511627956, 409.5, 254.586046511627956 ],
					"source" : [ "obj-97", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-99", 0 ],
					"source" : [ "obj-98", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"midpoints" : [ 1699.5, 776.086046511627956, 1141.5, 776.086046511627956 ],
					"source" : [ "obj-99", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-10" : [ "number[1]", "number[1]", 0 ],
			"obj-113" : [ "THD", "THD", 0 ],
			"obj-114" : [ "RATIO", "RATIO", 0 ],
			"obj-115" : [ "KNEE", "KNEE", 0 ],
			"obj-116" : [ "AUTO GAIN", "AUTO GAIN", 0 ],
			"obj-117" : [ "DRY WET", "DRY WET", 0 ],
			"obj-118" : [ "LOOKAHEAD", "LOOKAHEAD", 0 ],
			"obj-119" : [ "SOFTCLIP", "SOFTCLIP", 0 ],
			"obj-120" : [ "PARALLEL", "PARALLEL", 0 ],
			"obj-121" : [ "HPF", "HPF", 0 ],
			"obj-122" : [ "LPF", "LPF", 0 ],
			"obj-131" : [ "ALGO", "ALGO", 0 ],
			"obj-132" : [ "REACT", "REACT", 0 ],
			"obj-133" : [ "SMO", "SMO", 0 ],
			"obj-134" : [ "ATK", "ATK", 0 ],
			"obj-135" : [ "REL", "REL", 0 ],
			"obj-138" : [ "TRIM SLIDER", "TRIM SLIDER", 0 ],
			"obj-139" : [ "SC TRIM SLIDER[", "SC TRIM SLIDER[", 0 ],
			"obj-140" : [ "MAKEUP GAIN SLIDER", "MAKEUP GAIN SLIDER", 0 ],
			"obj-154" : [ "number[13]", "number[13]", 0 ],
			"obj-155" : [ "number[14]", "number[14]", 0 ],
			"obj-16" : [ "number[6]", "number[6]", 0 ],
			"obj-170" : [ "multislider[1]", "multislider", 0 ],
			"obj-181" : [ "multislider[2]", "multislider", 0 ],
			"obj-19" : [ "toggle", "toggle", 0 ],
			"obj-2" : [ "number[12]", "number[12]", 0 ],
			"obj-22" : [ "number[2]", "number[2]", 0 ],
			"obj-24" : [ "number[7]", "number[7]", 0 ],
			"obj-25" : [ "toggle[1]", "toggle[1]", 0 ],
			"obj-29" : [ "number", "number", 0 ],
			"obj-30" : [ "number[3]", "number[3]", 0 ],
			"obj-35" : [ "number[10]", "number[10]", 0 ],
			"obj-47" : [ "number[9]", "number[9]", 0 ],
			"obj-49" : [ "number[8]", "number[8]", 0 ],
			"obj-5" : [ "multislider", "multislider", 0 ],
			"obj-53" : [ "toggle[3]", "toggle", 0 ],
			"obj-57" : [ "number[5]", "number[5]", 0 ],
			"obj-70" : [ "number[4]", "number[4]", 0 ],
			"obj-89" : [ "toggle[4]", "toggle[4]", 0 ],
			"obj-92" : [ "toggle[5]", "toggle[5]", 0 ],
			"obj-98" : [ "number[11]", "number[11]", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "fondo.png",
				"bootpath" : "~/JUCEProjects/JCBComp/Assets",
				"patcherrelativepath" : "../Assets",
				"type" : "PNG",
				"implicit" : 1
			}
, 			{
				"name" : "hpfOff.png",
				"bootpath" : "~/JUCEProjects/JCBComp/Assets",
				"patcherrelativepath" : "../Assets",
				"type" : "PNG",
				"implicit" : 1
			}
, 			{
				"name" : "lpfOff.png",
				"bootpath" : "~/JUCEProjects/JCBComp/Assets",
				"patcherrelativepath" : "../Assets",
				"type" : "PNG",
				"implicit" : 1
			}
 ],
		"autosave" : 0,
		"styles" : [ 			{
				"name" : "rnbodefault",
				"default" : 				{
					"accentcolor" : [ 0.343034118413925, 0.506230533123016, 0.86220508813858, 1.0 ],
					"bgcolor" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
					"bgfillcolor" : 					{
						"angle" : 270.0,
						"autogradient" : 0.0,
						"color" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
						"color1" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
						"color2" : [ 0.263682, 0.004541, 0.038797, 1.0 ],
						"proportion" : 0.39,
						"type" : "color"
					}
,
					"color" : [ 0.929412, 0.929412, 0.352941, 1.0 ],
					"elementcolor" : [ 0.357540726661682, 0.515565991401672, 0.861786782741547, 1.0 ],
					"fontname" : [ "Lato" ],
					"fontsize" : [ 12.0 ],
					"stripecolor" : [ 0.258338063955307, 0.352425158023834, 0.511919498443604, 1.0 ],
					"textcolor_inverse" : [ 0.968627, 0.968627, 0.968627, 1 ]
				}
,
				"parentstyle" : "",
				"multi" : 0
			}
, 			{
				"name" : "rnbohighcontrast",
				"default" : 				{
					"accentcolor" : [ 0.666666666666667, 0.666666666666667, 0.666666666666667, 1.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"bgfillcolor" : 					{
						"angle" : 270.0,
						"autogradient" : 0.0,
						"color" : [ 0.0, 0.0, 0.0, 1.0 ],
						"color1" : [ 0.090196078431373, 0.090196078431373, 0.090196078431373, 1.0 ],
						"color2" : [ 0.156862745098039, 0.168627450980392, 0.164705882352941, 1.0 ],
						"proportion" : 0.5,
						"type" : "color"
					}
,
					"clearcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"color" : [ 1.0, 0.874509803921569, 0.141176470588235, 1.0 ],
					"editing_bgcolor" : [ 0.258823529411765, 0.258823529411765, 0.258823529411765, 1.0 ],
					"elementcolor" : [ 0.223386004567146, 0.254748553037643, 0.998085916042328, 1.0 ],
					"fontsize" : [ 13.0 ],
					"locked_bgcolor" : [ 0.258823529411765, 0.258823529411765, 0.258823529411765, 1.0 ],
					"selectioncolor" : [ 0.301960784313725, 0.694117647058824, 0.949019607843137, 1.0 ],
					"stripecolor" : [ 0.258823529411765, 0.258823529411765, 0.258823529411765, 1.0 ],
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"textcolor_inverse" : [ 1.0, 1.0, 1.0, 1.0 ]
				}
,
				"parentstyle" : "",
				"multi" : 0
			}
, 			{
				"name" : "rnbolight",
				"default" : 				{
					"accentcolor" : [ 0.443137254901961, 0.505882352941176, 0.556862745098039, 1.0 ],
					"bgcolor" : [ 0.796078431372549, 0.862745098039216, 0.925490196078431, 1.0 ],
					"bgfillcolor" : 					{
						"angle" : 270.0,
						"autogradient" : 0.0,
						"color" : [ 0.835294117647059, 0.901960784313726, 0.964705882352941, 1.0 ],
						"color1" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
						"color2" : [ 0.263682, 0.004541, 0.038797, 1.0 ],
						"proportion" : 0.39,
						"type" : "color"
					}
,
					"clearcolor" : [ 0.898039, 0.898039, 0.898039, 1.0 ],
					"color" : [ 0.815686274509804, 0.509803921568627, 0.262745098039216, 1.0 ],
					"editing_bgcolor" : [ 0.898039, 0.898039, 0.898039, 1.0 ],
					"elementcolor" : [ 0.337254901960784, 0.384313725490196, 0.462745098039216, 1.0 ],
					"fontname" : [ "Lato" ],
					"locked_bgcolor" : [ 0.898039, 0.898039, 0.898039, 1.0 ],
					"stripecolor" : [ 0.309803921568627, 0.698039215686274, 0.764705882352941, 1.0 ],
					"textcolor_inverse" : [ 0.0, 0.0, 0.0, 1.0 ]
				}
,
				"parentstyle" : "",
				"multi" : 0
			}
, 			{
				"name" : "rnbomonokai",
				"default" : 				{
					"accentcolor" : [ 0.501960784313725, 0.501960784313725, 0.501960784313725, 1.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"bgfillcolor" : 					{
						"angle" : 270.0,
						"autogradient" : 0.0,
						"color" : [ 0.0, 0.0, 0.0, 1.0 ],
						"color1" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
						"color2" : [ 0.263682, 0.004541, 0.038797, 1.0 ],
						"proportion" : 0.39,
						"type" : "color"
					}
,
					"clearcolor" : [ 0.976470588235294, 0.96078431372549, 0.917647058823529, 1.0 ],
					"color" : [ 0.611764705882353, 0.125490196078431, 0.776470588235294, 1.0 ],
					"editing_bgcolor" : [ 0.976470588235294, 0.96078431372549, 0.917647058823529, 1.0 ],
					"elementcolor" : [ 0.749019607843137, 0.83921568627451, 1.0, 1.0 ],
					"fontname" : [ "Lato" ],
					"locked_bgcolor" : [ 0.976470588235294, 0.96078431372549, 0.917647058823529, 1.0 ],
					"stripecolor" : [ 0.796078431372549, 0.207843137254902, 1.0, 1.0 ],
					"textcolor" : [ 0.129412, 0.129412, 0.129412, 1.0 ]
				}
,
				"parentstyle" : "",
				"multi" : 0
			}
 ]
	}

}
