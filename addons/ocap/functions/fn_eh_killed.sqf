#include "\ocap\script_macros.hpp"
params ["_victim", "_killer", "_instigator"];
if (isNull _instigator) then {_instigator = UAVControl vehicle _killer select 0}; // UAV/UGV player operated road kill
if (isNull _instigator) then {_instigator = _killer};
if ((isNull _instigator) || (_instigator == _victim)) then {_instigator = _victim getVariable ["ace_medical_lastDamageSource", _victim]};

// [ocap_captureFrameNo, "killed", _victimId, ["null"], -1];
private _victimId = _victim getVariable ["ocap_id", -1];
if (_victimId == -1) exitWith {};
private _eventData = [ocap_captureFrameNo, "killed", _victimId, ["null"], -1];

if (!isNull _instigator) then {
	_killerId = _instigator getVariable ["ocap_id", -1];
	if (_killerId != -1) then {
		private _killerInfo = [];
		if (_instigator isKindOf "CAManBase") then {
			_killerInfo = [
				_killerId,
				getText (configFile >> "CfgWeapons" >> currentWeapon _instigator >> "displayName")
			];
		} else {
			_killerInfo = [_killerId];
		};

		_eventData = [
			ocap_captureFrameNo,
			"killed",
			_victimId,
			_killerInfo,
			round(_instigator distance _victim)
		];
	};	
};

[":EVENT:", _eventData] call ocap_fnc_extension;