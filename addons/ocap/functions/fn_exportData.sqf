#include "\ocap\script_macros.hpp"
params [["_sideWon", sideEmpty, [sideEmpty]], ["_description", "", [""]]];
if (!ocap_capture) exitWith {LOG(["fnc_exportData.sqf called! OCAP don't start."]);};

_realyTime = time - ocap_startTime;
_ocapTime = ocap_frameCaptureDelay * ocap_captureFrameNo;
LOG(ARR6("fnc_exportData.sqf: RealyTime =", _realyTime," OcapTime =", _ocapTime," delta =", _realyTime - _OcapTime));

ocap_capture = false;
ocap_endFrameNo = ocap_captureFrameNo;

[":EVENT:", [ocap_endFrameNo, "endMission", [str _sideWon, SQF2JSON(_description)]]] call ocap_fnc_extension;

[":SAVE:", [worldName, briefingName, getMissionConfigValue ["author", ""], ocap_frameCaptureDelay, ocap_endFrameNo]] call ocap_fnc_extension;