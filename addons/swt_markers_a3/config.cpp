////////////////////////////////////////////////////////////////////
//DeRap: Produced from mikero's Dos Tools Dll version 5.58
//'now' is Tue Oct 17 22:55:42 2017 : 'file' last modified on Sun Sep 03 21:48:18 2017
//http://dev-heaven.net/projects/list_files/mikero-pbodll
////////////////////////////////////////////////////////////////////

#define _ARMA_

//Class swt_markers_a3 : config.bin{
class CfgPatches
{
	class swt_markers
	{
		units[] = {"swt_markers_disable","swt_markers_params"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"a3_ui_f"};
		author[] = {"swatSTEAM"};
	};
};
class CfgFunctions
{
	class swt_markers
	{
		class Init
		{
			file = "\swt_markers_a3";
			class init
			{
				preInit = 1;
			};
		};
	};
};
class RscListBox;
class RscIGUIListBox;
class RscXListBox;
class RscStructuredText;
class RscButtonMenu;
class RscButton;
class RscPicture;
class RscText;
class RscEdit;
class RscActivePicture;
class RscToolbox;
class RscIGUIShortcutButton;
class RscShortcutButton;
class RscActiveText;
class ScrollBar;
class RscCombo;
class RscControlsGroup;
class RscButtonMenuOK;
class RscButtonMenuCancel;
class RscCheckBox;
class swt_RscButtonMenu: RscButtonMenu
{
	style = 2;
	w = "10 * (((safezoneW/safezoneH) min 1.2)/40)";
	h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
};
class swt_RscStructuredText: RscStructuredText
{
	x = "(0.9+0.3) * (((safezoneW/safezoneH) min 1.2)/40)";
	y = 0;
	w = "(10-0.9) *  (((safezoneW/safezoneH) min 1.2)/40)";
	h = "0.9 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
	size = "(((((safezoneW/safezoneH) min 1.2)/1.2)/25) * 0.85)";
	class Attributes
	{
		color = "#ffffff";
	};
};
class swt_RscCheckBox: RscCheckBox
{
	idc = -1;
	type = 77;
	style = 0;
	checked = 0;
	x = "0.3 * (((safezoneW/safezoneH) min 1.2)/40)";
	y = 0;
	w = "1 * (((safezoneW/safezoneH) min 1.2)/40)";
	h = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
};
class swt_ScrollBar: ScrollBar
{
	color[] = {0,0,0,0.7};
	colorActive[] = {0,0,0,0.7};
	colorDisabled[] = {0,0,0,0.7};
	arrowEmpty = "#(argb,8,8,3)color(0,0,0,0)";
	arrowFull = "#(argb,8,8,3)color(0,0,0,0)";
};
class swt_RscControlsGroup: RscControlsGroup
{
	class VScrollbar: swt_ScrollBar
	{
		width = 0.021;
		autoScrollEnabled = 1;
	};
	class HScrollbar: swt_ScrollBar
	{
		height = 0;
	};
};
class swt_RscCombo: RscCombo
{
	colorSelect[] = {0,0,0,1};
	colorText[] = {1,1,1,1};
	colorBackground[] = {0,0,0,0.7};
	colorSelectBackground[] = {1,1,1,0.7};
	colorScrollbar[] = {1,0,0,1};
	wholeHeight = 0.45;
	colorActive[] = {1,0,0,1};
	colorDisabled[] = {1,1,1,0.25};
	class ComboScrollBar: ScrollBar
	{
		color[] = {1,1,1,0.5};
	};
};
class swt_RscActivePicture: RscActivePicture
{
	style = 48;
	color[] = {1,1,1,0.6};
	colorActive[] = {1,1,1,1};
	w = "(10/6) * (((safezoneW/safezoneH) min 1.2)/40)";
	h = "0.7 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
};
class swt_RscButton
{
	type = 1;
	style = 2;
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	shadow = 2;
	font = "PuristaMedium";
	sizeEx = "(((((safezoneW/safezoneH) min 1.2)/1.2)/25) * 1)";
	text = "";
	colorText[] = {0,0,0,0};
	colorBackground[] = {0,0,0,0};
	colorFocused[] = {0,0,0,0};
	colorBackgroundActive[] = {0,0,0,0};
	colorDisabled[] = {0,0,0,0};
	colorBackgroundDisabled[] = {0,0,0,0};
	offsetX = 0;
	offsetY = 0;
	offsetPressedX = 0;
	offsetPressedY = 0;
	colorShadow[] = {0,0,0,0};
	colorBorder[] = {0,0,0,0};
	borderSize = 0;
	soundEnter[] = {};
	soundPush[] = {};
	soundClick[] = {};
	soundEscape[] = {};
};
class RscDisplayChannel
{
	onLoad = "[_this select 0] spawn {swt_markers_channel = ctrlText ((_this select 0) displayCtrl 101)}";
};
class swt_RscDisplayInsertMarker
{
	enableSimulation = 0;
	onLoad = "_this call swt_markers_onLoad";
	onUnload = "call swt_markers_unLoad";
	idd = 54;
	movingEnable = "true";
	onKeyDown = "_this call swt_markers_DOWN";
	onKeyUp = "_this call swt_markers_UP";
	onMouseButtonDown = "_this call swt_markers_d_cl_pic";
	onMouseZChanged = "_this call swt_markers_MouseZ";
	class controlsBackground
	{
		class Description: RscStructuredText
		{
			colorBackground[] = {0,0,0,0.7};
			idc = 1100;
			x = 0;
			y = 0;
			w = "10 * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "2 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
		};
	};
	class controls
	{
		class swt_Text: RscEdit
		{
			idc = 203;
			x = 0;
			y = 0;
			w = "10 * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onKeyDown = "_this call swt_markers_editDOWN";
		};
		class swt_Picture: RscPicture
		{
			idc = 204;
			moving = "true";
			x = 0.259984;
			y = 0.4;
			w = 0.05;
			h = 0.0666667;
		};
		class ButtonMenuOK: RscButtonMenuOK
		{
			default = 1;
			idc = 1;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "['mark',(ctrlParent (_this select 0))] call swt_markers_sys_sendMark; true";
		};
		class ButtonChannel: swt_RscButton
		{
			idc = 205;
			x = 0;
			y = 0;
			w = "7 * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "_this call swt_markers_click_chann";
		};
		class ButtonMenuCancel: RscButtonMenuCancel
		{
			idc = 2;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
		};
		class ButtonMenuInfo: swt_RscButtonMenu
		{
			idc = 2400;
			text = "$STR_A3_RscDisplayInsertMarker_ButtonMenuInfo";
			x = 0;
			y = 0;
			w = "10 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "(_this select 0) call swt_markers_infoAnim";
		};
		class Settings_butt: swt_RscActivePicture
		{
			idc = 900;
			text = "\a3\Ui_f\data\GUI\Rsc\RscDisplayArcadeMap\icon_config_ca.paa";
			x = 0;
			y = 0;
			w = "0.75 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "_this spawn swt_markers_set_butt";
		};
		class add_group: RscActiveText
		{
			idc = 903;
			text = "G";
			color[] = {1,1,1,0.25};
			colorText[] = {1,1,1,0.25};
			colorActive[] = {0.8,0.8,0.8,0.5};
			x = 0;
			y = 0;
			w = "0.5 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			h = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			sizeEx = "0.8 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,'G'] call swt_markers_fast_text";
		};
		class add_name: RscActiveText
		{
			idc = 902;
			text = "N";
			color[] = {1,1,1,0.25};
			colorText[] = {1,1,1,0.25};
			colorActive[] = {0.8,0.8,0.8,0.5};
			x = 0;
			y = 0;
			w = "0.5 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			h = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			sizeEx = "0.8 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,'N'] call swt_markers_fast_text";
		};
		class add_text: RscActiveText
		{
			idc = 901;
			text = "T";
			color[] = {1,1,1,0.25};
			colorText[] = {1,1,1,0.25};
			colorActive[] = {0.8,0.8,0.8,0.5};
			x = 0;
			y = 0;
			w = "0.5 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			sizeEx = "0.8 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,'T'] call swt_markers_fast_text";
		};
		class RscListbox_15000_color: RscListBox
		{
			idc = 15000;
			x = 0;
			y = 0;
			w = "2 * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "10 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel_adv";
		};
		class RscListbox_15001_pic: RscListBox
		{
			idc = 15001;
			x = 0;
			y = 0;
			w = "2 * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "10 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel_adv";
		};
		class swt_RscControlsGroup: swt_RscControlsGroup
		{
			idc = 1103;
			x = 0;
			y = 0;
			w = "10 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "15 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			class controls
			{
				class Info_butt_1: swt_RscButtonMenu
				{
					idc = 2000;
					text = "Info";
					x = 0;
					y = 0;
					w = "3.33 *  (((safezoneW/safezoneH) min 1.2)/40)";
					h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					OnButtonClick = "[_this select 0,'info'] call swt_markers_info_buttons";
				};
				class Info_butt_2: swt_RscButtonMenu
				{
					idc = 2001;
					text = "Sett";
					x = "3.33*  (((safezoneW/safezoneH) min 1.2)/40)";
					y = 0;
					w = "3.33 *  (((safezoneW/safezoneH) min 1.2)/40)";
					h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					OnButtonClick = "[_this select 0,'sett'] call swt_markers_info_buttons";
				};
				class Info_butt_3: swt_RscButtonMenu
				{
					idc = 2002;
					text = "Author";
					x = "6.66*  (((safezoneW/safezoneH) min 1.2)/40)";
					y = 0;
					w = "3.33 *  (((safezoneW/safezoneH) min 1.2)/40)";
					h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					OnButtonClick = "[_this select 0,'author'] call swt_markers_info_buttons";
				};
				class Info_12: RscStructuredText
				{
					colorBackground[] = {0,0,0,0.7};
					idc = 1101;
					x = 0;
					y = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					w = "10 *  (((safezoneW/safezoneH) min 1.2)/40)";
					h = "23.5 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
			};
		};
		class swt_color_1200: swt_RscActivePicture
		{
			idc = 1200;
			text = "#(argb,8,8,3)color(1,1,1,1)";
			w = "(10/6) * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "0.7 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,0] call swt_markers_setColor";
		};
		class swt_color_1201: swt_RscActivePicture
		{
			idc = 1201;
			text = "#(argb,8,8,3)color(1,1,1,1)";
			w = "(10/6) * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "0.7 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,1] call swt_markers_setColor";
		};
		class swt_color_1202: swt_RscActivePicture
		{
			idc = 1202;
			text = "#(argb,8,8,3)color(1,1,1,1)";
			w = "(10/6) * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "0.7 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,2] call swt_markers_setColor";
		};
		class swt_color_1203: swt_RscActivePicture
		{
			idc = 1203;
			text = "#(argb,8,8,3)color(1,1,1,1)";
			w = "(10/6) * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "0.7 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,3] call swt_markers_setColor";
		};
		class swt_color_1204: swt_RscActivePicture
		{
			idc = 1204;
			text = "#(argb,8,8,3)color(1,1,1,1)";
			w = "(10/6) * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "0.7 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,4] call swt_markers_setColor";
		};
		class swt_color_1205: swt_RscActivePicture
		{
			idc = 1205;
			text = "#(argb,8,8,3)color(1,1,1,1)";
			w = "(10/6) *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "0.7 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "[_this select 0,5] call swt_markers_setColor";
		};
		class swt_icon_1300: swt_RscButton
		{
			idc = 1300;
			text = "";
			w = 0.05;
			h = 0.0666667;
			OnButtonClick = "[_this select 0,0] call swt_markers_setIcon";
		};
		class swt_icon_1301: swt_RscButton
		{
			idc = 1301;
			text = "";
			w = 0.05;
			h = 0.0666667;
			OnButtonClick = "[_this select 0,1] call swt_markers_setIcon";
		};
		class swt_icon_1302: swt_RscButton
		{
			idc = 1302;
			text = "";
			w = 0.05;
			h = 0.0666667;
			OnButtonClick = "[_this select 0,2] call swt_markers_setIcon";
		};
		class swt_icon_1303: swt_RscButton
		{
			idc = 1303;
			text = "";
			w = 0.05;
			h = 0.0666667;
			OnButtonClick = "[_this select 0,3] call swt_markers_setIcon";
		};
		class swt_icon_1304: swt_RscButton
		{
			idc = 1304;
			text = "";
			w = 0.05;
			h = 0.0666667;
			OnButtonClick = "[_this select 0,4] call swt_markers_setIcon";
		};
		class swt_icon_1305: swt_RscButton
		{
			idc = 1305;
			text = "";
			w = 0.05;
			h = 0.0666667;
			OnButtonClick = "[_this select 0,5] call swt_markers_setIcon";
		};
		class swt_icon_1400: RscPicture
		{
			idc = 1400;
			text = "\A3\ui_f\data\map\markers\military\dot_CA.paa";
			w = 0.05;
			h = 0.0666667;
		};
		class swt_icon_1401: RscPicture
		{
			idc = 1401;
			text = "\A3\ui_f\data\map\markers\nato\o_inf.paa";
			w = 0.05;
			h = 0.0666667;
		};
		class swt_icon_1402: RscPicture
		{
			idc = 1402;
			text = "\A3\ui_f\data\map\markers\nato\o_armor.paa";
			w = 0.05;
			h = 0.0666667;
		};
		class swt_icon_1403: RscPicture
		{
			idc = 1403;
			text = "\A3\ui_f\data\map\markers\handdrawn\pickup_CA.paa";
			w = 0.05;
			h = 0.0666667;
		};
		class swt_icon_1404: RscPicture
		{
			idc = 1404;
			text = "\A3\ui_f\data\map\markers\handdrawn\warning_CA.paa";
			w = 0.05;
			h = 0.0666667;
		};
		class swt_icon_1405: RscPicture
		{
			idc = 1405;
			text = "\A3\ui_f\data\map\markers\handdrawn\unknown_CA.paa";
			w = 0.05;
			h = 0.0666667;
		};
		class swt_RscCombo_2100: swt_RscCombo
		{
			idc = 2100;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2101: swt_RscCombo
		{
			idc = 2101;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2102: swt_RscCombo
		{
			idc = 2102;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2103: swt_RscCombo
		{
			idc = 2103;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2104: swt_RscCombo
		{
			idc = 2104;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2105: swt_RscCombo
		{
			idc = 2105;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2106: swt_RscCombo
		{
			idc = 2106;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2107: swt_RscCombo
		{
			idc = 2107;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2108: swt_RscCombo
		{
			idc = 2108;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2109: swt_RscCombo
		{
			idc = 2109;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2110: swt_RscCombo
		{
			idc = 2110;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_RscCombo_2111: swt_RscCombo
		{
			idc = 2111;
			x = 0;
			y = 0;
			w = "5 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			onLBSelChanged = "_this call swt_markers_lb_sel;";
		};
		class swt_ButtonAdv: swt_RscButtonMenu
		{
			idc = 228;
			text = "$STR_SWT_M_ADV";
			x = 0;
			y = 0;
			w = "10 *  (((safezoneW/safezoneH) min 1.2)/40)";
			h = "1 *  ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			OnButtonClick = "_this spawn swt_markers_adv_set_butt";
		};
		class swt_RscControlsGroup_adv: swt_RscControlsGroup
		{
			idc = 1104;
			x = 0;
			y = 0;
			w = "25 * (((safezoneW/safezoneH) min 1.2)/40)";
			h = "20 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
			class controls
			{
				class adv_background: RscText
				{
					idc = 822;
					x = 0;
					y = 0;
					w = "(21.1) * (((safezoneW/safezoneH) min 1.2)/40)";
					h = "((3.5*0.9)+(4.5*0.9)+(7.5*0.9) + 2.5 * 0.9 + 0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					colorBackground[] = {0,0,0,0.7};
					text = "";
				};
				class first_block: RscText
				{
					style = "64 + 2";
					x = "0.15 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "0 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					text = "$STR_SWT_M_SHOW_BUTTONS";
					w = "(9.7) * (((safezoneW/safezoneH) min 1.2)/40)";
					h = "(3.5*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					SizeEx = "0.8 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					colorText[] = {"248/255","131/255","121/255",1};
				};
				class cb_show_butt: swt_RscCheckBox
				{
					idc = 229;
					y = "(0.9)* ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SHOW OK'] spawn swt_markers_cb_butt";
				};
				class text_show_butt: swt_RscStructuredText
				{
					idc = 230;
					text = "$STR_SWT_M_OKCAN";
					y = "(0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_show_info: swt_RscCheckBox
				{
					idc = 241;
					x = "(0.3) * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "(2*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SHOW INFO'] spawn swt_markers_cb_butt";
				};
				class text_show_info: swt_RscStructuredText
				{
					idc = 242;
					text = "INFO";
					y = "(2*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class Second_block: first_block
				{
					x = "0.15 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "(3.5*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					text = "$STR_SWT_M_CHOOSE";
					h = "(4.5*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_show_icon: swt_RscCheckBox
				{
					idc = 231;
					y = "(3.5*0.9+0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SHOW ICON'] spawn swt_markers_cb_butt";
				};
				class text_show_icon: swt_RscStructuredText
				{
					idc = 232;
					text = "$STR_SWT_M_ICONS";
					y = "(3.5*0.9+0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_show_color: swt_RscCheckBox
				{
					idc = 233;
					y = "(3.5*0.9+2*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SHOW COLOR'] spawn swt_markers_cb_butt";
				};
				class text_show_color: swt_RscStructuredText
				{
					idc = 234;
					text = "$STR_SWT_M_COLORS";
					y = "(3.5*0.9+2*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_show_lb: swt_RscCheckBox
				{
					idc = 235;
					y = "(3.5*0.9+3*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SHOW LB'] spawn swt_markers_cb_butt";
				};
				class text_show_lb: swt_RscStructuredText
				{
					idc = 236;
					text = "$STR_SWT_M_ADVL";
					y = "(3.5*0.9+3*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class third_block: first_block
				{
					style = "64 + 2";
					x = "0.15 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "((3.5*0.9)+(4.5*0.9)) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					text = "$STR_SWT_M_F";
					h = "(7.5*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					SizeEx = "0.8 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_save_text: swt_RscCheckBox
				{
					idc = 237;
					y = "((3.5*0.9)+(4.5*0.9) + 0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SAVE TEXT'] spawn swt_markers_cb_butt";
				};
				class text_save_text: swt_RscStructuredText
				{
					idc = 238;
					text = "$STR_SWT_M_SAVET";
					y = "((3.5*0.9)+(4.5*0.9) + 0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_save_mark: swt_RscCheckBox
				{
					idc = 245;
					y = "((3.5*0.9)+(4.5*0.9) + 2*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SAVE MARK'] spawn swt_markers_cb_butt";
				};
				class text_save_mark: swt_RscStructuredText
				{
					idc = 246;
					text = "$STR_SWT_M_SAVEM";
					y = "((3.5*0.9)+(4.5*0.9) + 2*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_fast_load: swt_RscCheckBox
				{
					idc = 239;
					y = "((3.5*0.9)+(4.5*0.9)+3*0.9)* ((((safezoneW/safezoneH) min 1.2) / 1.2) / 25)";
					onButtonClick = "[_this,'FAST LOAD'] spawn swt_markers_cb_butt";
				};
				class text_fast_load: swt_RscStructuredText
				{
					idc = 240;
					text = "$STR_SWT_M_FASTL";
					y = "((3.5*0.9)+(4.5*0.9)+3*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_show_back: swt_RscCheckBox
				{
					idc = 243;
					y = "((3.5*0.9)+(4.5*0.9) + 4*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'SHOW BACK'] spawn swt_markers_cb_butt";
				};
				class text_show_back: swt_RscStructuredText
				{
					idc = 244;
					text = "$STR_SWT_M_BACKGROUND";
					y = "((3.5*0.9)+(4.5*0.9) + 4*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_log: swt_RscCheckBox
				{
					idc = 247;
					y = "((3.5*0.9)+(4.5*0.9) + 5*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'LOG'] spawn swt_markers_cb_butt";
				};
				class text_log: swt_RscStructuredText
				{
					idc = 248;
					text = "$STR_SWT_M_LOG";
					y = "((3.5*0.9)+(4.5*0.9) + 5*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class cb_markinfo: swt_RscCheckBox
				{
					idc = 249;
					y = "((3.5*0.9)+(4.5*0.9) + 6*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onButtonClick = "[_this,'MARK INFO'] spawn swt_markers_cb_butt";
				};
				class text_markimfo: swt_RscStructuredText
				{
					idc = 250;
					text = "$STR_SWT_M_MARKINFO";
					y = "((3.5*0.9)+(4.5*0.9) + 6*0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class swt_text_saved: first_block
				{
					idc = -1;
					text = "$STR_SWT_M_SAVEDTEXT";
					x = "0.3 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "((3.5*0.9)+(4.5*0.9)+(7.5*0.9)) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					h = "2.5 * 0.9 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class swt_edit_saved: RscEdit
				{
					idc = 551;
					text = "";
					x = "0.45 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "((3.5*0.9)+(4.5*0.9)+(7.5*0.9) + 0.9) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					w = "9.2 * (((safezoneW/safezoneH) min 1.2)/40)";
					h = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					onKeyUp = "swt_markers_fast_text_T_saved = ctrlText (_this select 0);swt_marker_settings_params set [9, swt_markers_fast_text_T_saved]; profileNamespace setVariable ['swt_marker_settings_params', swt_marker_settings_params];saveProfileNamespace;";
					color[] = {1,1,1,1};
				};
				class swt_text_markersmap: first_block
				{
					idc = 650;
					text = "$STR_SWT_M_SAVEDM";
					x = "10.2 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "0 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					w = "10.8 * (((safezoneW/safezoneH) min 1.2)/40)";
					h = "(9 + 9*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class swt_text_markerspr: first_block
				{
					idc = 651;
					text = "$STR_SWT_M_INPROF";
					x = "10.4 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					w = "10.4 * (((safezoneW/safezoneH) min 1.2)/40)";
					h = "(4+4*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class swt_ButtonSAVE: swt_RscButtonMenu
				{
					idc = 347;
					text = "$STR_SWT_M_SAVE";
					x = "10.6 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "2 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					size = "0.85 * (((((safezoneW/safezoneH) min 1.2)/1.2)/25) * 1)";
					OnButtonClick = "_this call swt_markers_fnc_save_markers";
				};
				class swt_ButtonLOAD: swt_ButtonSAVE
				{
					idc = 348;
					text = "$STR_SWT_M_LOAD";
					y = "(3 + 1*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					OnButtonClick = "[_this] call swt_markers_fnc_load_markers";
				};
				class swt_ButtonUnload: swt_ButtonSAVE
				{
					idc = 346;
					text = "$STR_SWT_M_UNLOAD";
					y = "(4 + 2*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					OnButtonClick = "profileNamespace setVariable ['swt_markers_save_arr',[]]; saveProfileNamespace;";
				};
				class swt_text_clip: first_block
				{
					idc = 652;
					text = "$STR_SWT_M_CLIP";
					x = "10.4 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "(5 + 3*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					w = "10.4 * (((safezoneW/safezoneH) min 1.2)/40)";
					h = "(4+4*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
				};
				class swt_ButtonSaveClp: swt_ButtonSAVE
				{
					idc = 351;
					text = "$STR_SWT_M_SAVE";
					y = "(6 + 3*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					OnButtonClick = "'CLIP' call swt_markers_fnc_save_markers";
				};
				class swt_Edit: RscEdit
				{
					idc = 653;
					text = "$STR_SWT_M_ENTERARRAY";
					x = "10.6 * (((safezoneW/safezoneH) min 1.2)/40)";
					y = "(7 + 4*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					w = "10 * (((safezoneW/safezoneH) min 1.2)/40)";
					h = "1 * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					size = "0.85*(((((safezoneW/safezoneH) min 1.2)/1.2)/25) * 1)";
				};
				class swt_ButtonClpLoad: swt_ButtonSAVE
				{
					idc = 654;
					text = "$STR_SWT_M_LOAD";
					y = "(8 + 5*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					OnButtonClick = "[_this,ctrlText ((ctrlParent (_this select 0)) displayCtrl 653)] call swt_markers_fnc_load_markers";
				};
				class swt_Buttondef: swt_ButtonSAVE
				{
					idc = 345;
					text = "$STR_SWT_M_DEF";
					y = "(9 + 10*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					colorBackground[] = {0.957,0,0,0.8};
					color[] = {0,0,0,1};
					OnButtonClick = "call swt_def; (ctrlParent (_this select 0)) closeDisplay 0;";
				};
				class swt_ButtonClear: swt_ButtonSAVE
				{
					idc = 349;
					text = "$STR_SWT_M_CLEARMAP";
					y = "(10 + 11*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					colorBackground[] = {0.957,0,0,0.8};
					color[] = {0,0,0,1};
					OnButtonClick = "call swt_markers_clear_map";
				};
				class swt_ButtonDisable: swt_ButtonSAVE
				{
					idc = 350;
					text = "$STR_SWT_M_DISABLE";
					y = "(11 + 12*0.15) * ((((safezoneW/safezoneH) min 1.2)/1.2)/25)";
					colorBackground[] = {0.957,0,0,0.8};
					color[] = {0,0,0,1};
					OnButtonClick = "(_this select 0) call swt_markers_DisableLoc_fnc";
				};
			};
		};
	};
};
class CfgFactionClasses
{
	class swt_markers_a3
	{
		displayName = "Sweet Markers System";
		priority = 0;
		side = 7;
	};
};
class CfgVehicles
{
	class Logic;
	class Module_F;
	class swt_markers_disable: Module_F
	{
		scope = 2;
		author = "swatSTEAM";
		displayName = "$STR_SWT_M_MODUL_BRIEF";
		category = "swt_markers_a3";
		function = "";
		functionPriority = 1;
		class init
		{
			postInit = 1;
		};
		isGlobal = 2;
		isTriggerActivated = 0;
		class Eventhandlers
		{
			init = "_this spawn compile preprocessFileLineNumbers '\swt_markers_a3\Stuff\swt_disable.sqf';";
		};
	};
	class swt_markers_params: Module_F
	{
		scope = 2;
		author = "swatSTEAM";
		displayName = "$STR_SWT_M_MODUL_CONF";
		category = "swt_markers_a3";
		function = "";
		functionPriority = 1;
		class init
		{
			postInit = 1;
		};
		isGlobal = 2;
		isTriggerActivated = 0;
		class Eventhandlers
		{
			init = "_this spawn compile preprocessFileLineNumbers '\swt_markers_a3\Stuff\swt_markers_params.sqf';";
		};
		class Arguments
		{
			class Loads
			{
				displayName = "$STR_SWT_M_MODUL_LOAD";
				description = "$STR_SWT_M_MODUL_LOAD_T";
				typeName = "BOOL";
				class values
				{
					class On
					{
						name = "On";
						value = "true";
						default = 1;
					};
					class Off
					{
						name = "Off";
						value = "false";
					};
				};
			};
			class Loads_for
			{
				displayName = "$STR_SWT_M_MODUL_LOAD_FOR";
				description = "$STR_SWT_M_MODUL_LOAD_FOR_T";
				typeName = "BOOL";
				class values
				{
					class On
					{
						name = "$STR_SWT_M_MODUL_LOAD_FOR_LEAD";
						value = "true";
						default = 1;
					};
					class Off
					{
						name = "$STR_SWT_M_MODUL_LOAD_FOR_ALL";
						value = "false";
					};
				};
			};
			class Loads_brif
			{
				displayName = "$STR_SWT_M_MODUL_LOAD_WHEN";
				description = "$STR_SWT_M_MODUL_LOAD_WHEN";
				typeName = "BOOL";
				class values
				{
					class On
					{
						name = "$STR_SWT_M_MODUL_LOAD_WHEN_A";
						value = "true";
						default = 1;
					};
					class Off
					{
						name = "$STR_SWT_M_MODUL_LOAD_WHEN_BRIEF";
						value = "false";
					};
				};
			};
		};
	};
};
class cfgMods
{
	author = "swatSTEAM";
	timepacked = "1442689873";
};
//};
