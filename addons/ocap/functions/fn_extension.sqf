#define DLL_NAME "OcapReplaySaver2"
params ["_command","_args"];

private _res = DLL_NAME callExtension [_command, _args];

_res params ["_result","_returnCode","_errorCode"];

if (_errorCode != 0 || _returnCode != 0) then {
	diag_log ["fnc_callextension_zlt.sqf: Error: ", _result, _returnCode, _errorCode, _command, _args];
}