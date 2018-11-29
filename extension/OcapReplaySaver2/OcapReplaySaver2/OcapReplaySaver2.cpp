// by Zealot 
// MIT licence https://opensource.org/licenses/MIT


#pragma region Версия
/*

v 2.0.0.1 2017-09-25 Zealot Начальная версия. Расширение еще ничего не делает.
v 2.0.1.1 2017-09-25 Zealot Начальная версия. Уже должно делать почти все.
v 2.0.2.1 2017-09-28 Zealot Добавлена команда :LOG:
v 2.0.2.2 2017-09-28 Zealot Exception handling
v 2.0.3.2 2017-09-28 Zealot Отдельный файл для лога по команде :LOG:
v 2.0.3.3 2017-09-28 Zealot Проверено в работе
v 2.0.4.1 2017-09-29 Zealot Добавлен Curl
v 2.0.5.0 2017-10-03 Zealot Загрузка файлов через http, файлы создаются во временном каталоге системы, конфиг в json
v 2.0.5.1 2017-10-03 Zealot bugfix
v 2.0.6.1 2017-10-07 Zealot bugfix curl encode && теперь нужно вызывать START перед началом записи, добавлен :FRAMENO:, :FIRED:
v 2.0.6.2 2017-10-08 Zealot fixed unicode
v 2.0.7.0 2017-10-23 Zealot Запись маркеров, фикс обработки строк, старая запись не сохраняется при старте новой, немного фиксов
v 2.0.7.1 2017-10-25 Zealot Новый формат записи маркеров
v 2.0.7.2 2017-10-27 Zealot bugfix
v 2.0.7.3 2017-11-06 Zealot Старый формат записи маркеров
v 3.0.7.3 2017-11-06 Zealot Release для ocap v.2 команды :NEW:UNIT: :NEW:VEH: :UPDATE:UNIT: :UPDATE:VEH:
v 3.0.7.4 2017-11-06 Zealot bug https://bitbucket.org/mrDell/ocap2/issues/24/dll-403-unit-vehicle
v 3.0.7.5 2018-01-21 Zealot MarkerMove меняет предыдущую запись если маркер уже был, а не создает новую
v 3.0.7.6 2018-01-21 Zealot Теперь ошибки обрабатываются с помощью исключений
v 3.0.8.0 2018-01-21 Zealot При команде :START: происходит реконфигурация логгера и он начинает писать в новый файл
v 3.0.8.1 2018-06-16 Zealot При команде SAVE если в названии миссии есть кавычки выкидывается исключение
v 3.0.8.2 2018-06-18 Zealot Финальный фикс проблемы с именем миссии
v 4.0.0.1 2018-11-26 Zealot Test version, worker threads variants
v 4.0.0.2 2018-11-29 Zealot Optimised multithreading

TODO:
- сжатие данных
- чтение запись настроек


*/

#define CURRENT_VERSION "4.0.0.2"

#pragma endregion


#include "easylogging++.h"

#include <cstring>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>
#include <locale>
#include <codecvt>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <regex>
#include <queue>
#include <tuple>

#include <Windows.h>
#include <direct.h>
#include <process.h>
#include <curl\curl.h>

#include "json.hpp"

#define LOG_NAME "ocaplog\\ocap-main.%datetime{%Y%M%d_%H%m%s}.log"
#define LOG_NAME_EXT "ocaplog\\ocap-ext.%datetime{%Y%M%d_%H%m%s}.log"

#define REPLAY_FILEMASK "%Y_%m_%d__%H_%M_"
#define CONFIG_NAME L"OcapReplaySaver2.cfg.json"
#define CONFIG_NAME_SAMPLE L"OcapReplaySaver2.cfg.json.sample"

#define CMD_NEW_UNIT		":NEW:UNIT:"  // новый юнит зарегистрирована ocap
#define CMD_NEW_VEH			":NEW:VEH:"  // новая техника зарегистрирована ocap
#define CMD_EVENT			":EVENT:" // новое событие, кто-то зашел, вышел, стрельнул и попал
#define CMD_CLEAR			":CLEAR:" // очистить json
#define CMD_UPDATE_UNIT		":UPDATE:UNIT:" // обновить данные по позиции юнита
#define CMD_UPDATE_VEH		":UPDATE:VEH:" // обновить данные по позиции техники
#define CMD_SAVE			":SAVE:" // записать json во временный файл и попробовать отправить по сети
#define CMD_LOG				":LOG:" // сделать запись в отдельный файл лога
#define CMD_START			":START:" // начать запись ocap реплея
#define CMD_FIRED			":FIRED:" // кто-то стрельнул

#define CMD_MARKER_CREATE	":MARKER:CREATE:" // был создан маркер на карте
#define CMD_MARKER_DELETE	":MARKER:DELETE:" // маркер удалили
#define CMD_MARKER_MOVE		":MARKER:MOVE:" // маркер передвинули

using namespace std;

int commandEvent(const vector<string> &args);
int commandNewUnit(const vector<string> &args);
int commandNewVeh(const vector<string> &args);
int commandSave(const vector<string> &args);
int commandUpdateUnit(const vector<string> &args);
int commandUpdateVeh(const vector<string> &args);
int commandClear(const vector<string> &args);
int commandLog(const vector<string> &args);
int commandStart(const vector<string> &args);
int commandFired(const vector<string> &args);

int commandMarkerCreate(const vector<string> &args);
int commandMarkerDelete(const vector<string> &args);
int commandMarkerMove(const vector<string> &args);

namespace {
	

	using json = nlohmann::json;

	thread command_thread;
	queue<tuple<string, vector<string> > > commands;
	mutex command_mutex;
	mutex queue_mutex;
	condition_variable command_cond;
	bool command_thread_shutdown = false;



	std::unordered_map<std::string, std::function<int(const vector<string> &)> > dll_commands = {
		{ CMD_NEW_VEH,			commandNewVeh },
		{ CMD_NEW_UNIT,			commandNewUnit },
		{ CMD_CLEAR,			commandClear },
		{ CMD_EVENT,			commandEvent },
		{ CMD_UPDATE_VEH,		commandUpdateVeh },
		{ CMD_UPDATE_UNIT,		commandUpdateUnit },
		{ CMD_SAVE,				commandSave },
		{ CMD_LOG,				commandLog },
		{ CMD_START,			commandStart },
		{ CMD_FIRED,			commandFired },
		{ CMD_MARKER_CREATE,	commandMarkerCreate },
		{ CMD_MARKER_DELETE,	commandMarkerDelete },
		{ CMD_MARKER_MOVE,		commandMarkerMove   }
	};

	class ocapSaverException : public std::exception {
	public:
		ocapSaverException(const char *s, int i) :
			std::exception(s, i) {
			_i = i;
		}
		int getErrorCode() const {
			return _i;
		}
	private:
		int _i = 0;
	};

#define MSG_ERROR_COMMAND "Error while command execution. "
#define MSG_ERROR_NOT_SUPPORTED "Error: Not supported call. "

	enum {
		E_NO_ERRORS = 0,

		E_FUN_NOT_SUPPORTED = 6,
		E_INCORRECT_COMMAND_PARAM = 7,
		E_NOT_WRITING_STATE = 8,
		E_NUMBER_ARG = 9,
		E_NUMBER_ARGS_VAR = 10
	};

#define ERROR_MSG_BUF_LEN 1024
	char errorMsg[ERROR_MSG_BUF_LEN] = "";

	char *errors[] = {
		/* 0 */		"",
		/* 1 */		"",
		/* 2 */		"",
		/* 3 */		"",
		/* 4 */		"",
		/* 5 */		"",
		/* 6 */		"ERROR: Function %s is not supported ! (%s:%s)",
		/* 7 */		"ERROR: Incorrect command param %i ! (%s:%s)",
		/* 8 */		"ERROR: Is not writing state ! (%s:%s)",
		/* 9 */		"ERROR: Incorrect number of given arguments %i instead of expected %i ! (%s:%s)",
		/* 10 */	"ERROR: Incorrect number of given arguments %i instead of expected %s ! (%s:%s)"
	};

#define ERROR_THROW(E, ...) if(true){snprintf(errorMsg, ERROR_MSG_BUF_LEN, errors[E], ##__VA_ARGS__, __FUNCTION__, __LINE__);throw ocapSaverException(errorMsg, E);}
#define COMMAND_CHECK_INPUT_PARAMETERS(N) if(args.size()!=N){ERROR_THROW(E_NUMBER_ARG, args.size(), N)}
#define COMMAND_CHECK_WRITING_STATE	if(!is_writing){ERROR_THROW(E_NOT_WRITING_STATE)}
	
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	json j;
	bool curl_init = false;

	bool is_writing = false;

	struct {
		std::string dbInsertUrl = "http://ocap.red-bear.ru/data/receive.php?option=dbInsert";
		std::string addFileUrl = "http://ocap.red-bear.ru/data/receive.php?option=addFile";
		int httpRequestTimeout = 120;
		int traceLog = 0;
	} config;
}


void perform_command(tuple<string, vector<string> > &command) {
	string function(std::move(std::get<0>(command)));
	vector<string> args(std::move(std::get<1>(command)));

	int res = 1;

	if (config.traceLog) {
		stringstream ss;
		ss << function << " " << args.size() << ":[";
		for (int i = 0; i < args.size(); i++) {
			if (i > 0)
				ss << "::";
			ss << args[i];
		}
		ss << "]";
		LOG(TRACE) << ss.str();
	}


	try {

		auto fn = dll_commands.find(function);
		if (fn == dll_commands.end()) {
			ERROR_THROW(E_FUN_NOT_SUPPORTED, function)
		}
		else {
			res = fn->second(args);

		}
	}
	catch (const ocapSaverException &e) {
		res = e.getErrorCode();
		LOG(ERROR) << "E:" << res << e.what();
	}
	catch (const exception &e) {
		res = 1;
		LOG(ERROR) << "Exception: " << e.what();
	}
	catch (...) {
		res = 1;
		LOG(ERROR) << "Exception: Unknown";
	}

	if (res != 0) {
		stringstream ss;
		ss << "Return: " << res << " parameters were: " << function << " ";
		ss << args.size() << ":[";
		for (int i = 0; i < args.size(); i++) {
			if (i > 0)
				ss << "::";
			ss << args[i];
		}
		ss << "]";
		LOG(ERROR) << ss.str();
	}

}


void command_loop() {
	try {
		while (true) {
			unique_lock<mutex> lock(command_mutex);
			command_cond.wait(lock, [] {return !commands.empty() || command_thread_shutdown; });
			lock.release();

			while (true) {
				unique_lock<mutex> lock2(command_mutex);
				if (commands.empty())
				{
					break;
				}
				tuple<string, vector<string> > cur_command = std::move(commands.front());
				commands.pop();
				lock2.release();
				perform_command(cur_command);
			}
			if (command_thread_shutdown)
			{
				LOG(INFO) << "Exit flag is set. Quiting command loop.";
				return;
			}
		}
	}
	catch (const exception &e) {
		LOG(ERROR) << "Exception: " << e.what();
	}
	catch (...) {
		LOG(ERROR) << "Exception: Unknown";
	}
}

string removeAdd(const char * c) {
	std::string r(c);
	r.erase(remove(r.begin(), r.end(), '#'), r.end());
	return r;
}


std::string removeQuotes(const char * c) {
	std::string r(c);
	r.erase(remove(r.begin(), r.end(), '\"'), r.end());
	return r;
}

// убирает начальные и конечные кавычки в текстк, сдвоенные кавычки превращает в одинарные
std::string prepStr(const char * c) {
	std::string out(c);
	std::regex e("\"\"");
	std::regex eb("^\"");
	std::regex ee("\"$");
	out = regex_replace(out, eb, "");
	out = regex_replace(out, ee, "");
	out = regex_replace(out, e, "\"");
	return out;
}

void prepareMarkerFrames(int frames) {
	// причесывает "Markers"

	try {
		if (j["Markers"].is_null() || !j["Markers"].is_array() || j["Markers"].size() < 1) {
			LOG(ERROR) << j["Markers"] << "has incorrect state!";
			return;
		}

		/*
		маркер сохраняется как:
		[
		0	:	“тип маркера”, // “” имя иконки маркера
		1	:	“Подпись маркера”,
		2	:	12, //кто поставил id
		3	:	“000000”, //цвет в HEX
		4	:	52, // стартовый фрейм
		5	:	104, // конечный фрейм
		6	:	0, // 0 -east, 1 - west, 2 - resistance, 3- civilian, -1 - global
		7	:	[[52,[1000,5000],180],[70,[1500,5600],270]] // позиция
		]
		*/
		/*
		[0:_mname , 1: 0, 2 : swt_cfgMarkers_names select _type, 3: _mtext, 4: ocap_captureFrameNo, 5:-1, 6: _pl getVariable ["ocap_id", 0],
		7: call bis_fnc_colorRGBtoHTML, 8:[1,1], 9:side _pl call BIS_fnc_sideID, 10:_mpos]]
		оставить:  2, 3, 4, 5, 6, 7, 9, 10
		*/

		// чистит "Markers"
		for (int i = 0; i < j["Markers"].size(); ++i) {
			json ja = j["Markers"][i];
			if (ja[5].get<int>() == -1) {
				ja[5] = frames;
			}
			j["Markers"][i] = json::array({ja[2], ja[3], ja[4], ja[5], ja[6], ja[7],ja[9], ja[10]});
		}
	}
	catch (exception &e) {
		LOG(ERROR) << "Error happens" << e.what();
		return;

	}
	catch (...)
	{
		LOG(ERROR) << "Error happens" << frames;
		return;
	}

}


std::string saveCurrentReplayToTempFile() {
	char tPath[MAX_PATH] = { 0 };
	char tName[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, tPath);
	GetTempFileNameA(tPath, "ocap", 0, tName);

	fstream currentReplay(tName, fstream::out | fstream::binary);
	if (!currentReplay.is_open()) {
		LOG(ERROR) << "Cannot open result file: " << tName;
		throw std::exception("Cannot open temp file!");
	}
	if (config.traceLog) 
		currentReplay << j.dump(4);
	else 
		currentReplay << j.dump();
	currentReplay.flush();
	currentReplay.close();
	LOG(INFO) << "Replay saved:" << tName;
	return tName;

}

std::string generateResultFileName(const std::string &name) {
	std::time_t t = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &t);
	std::stringstream ss;
	ss << std::put_time(&tm, REPLAY_FILEMASK) << name << ".json";
	return ss.str();
}






#pragma region Вычитка конфига
void readWriteConfig(HMODULE hModule) {
	wchar_t szPath[MAX_PATH], szDirPath[_MAX_DIR];
	GetModuleFileNameW(hModule, szPath, MAX_PATH);
	_wsplitpath_s(szPath, 0, 0, szDirPath, _MAX_DIR, 0, 0, 0, 0);
	wstring path(szDirPath), path_sample(szDirPath);
	path += CONFIG_NAME;
	path_sample += CONFIG_NAME_SAMPLE;
	if (!std::ifstream(path_sample)) {
		LOG(INFO) << "Creating sample config file: " << converter.to_bytes(path_sample);
		json j = { { "addFileUrl", config.addFileUrl },{ "dbInsertUrl", config.dbInsertUrl },{ "httpRequestTimeout", config.httpRequestTimeout }, {"traceLog", config.traceLog} };
		std::ofstream out(path_sample, ofstream::out | ofstream::binary);
		out << j.dump(4) << endl;
	}
	LOG(INFO) << "Trying to read config file:" << converter.to_bytes(path);
	bool cfgOpened = false;
	ifstream cfg(path, ifstream::in | ifstream::binary);

	json jcfg;
	if (!cfg.is_open()) {
		LOG(WARNING) << "Cannot open cfg file! Using default params.";
		return;
	}
	cfg >> jcfg;
	if (!jcfg["addFileUrl"].is_null() && jcfg["addFileUrl"].is_string()) {
		config.addFileUrl = jcfg["addFileUrl"].get<string>();
	}
	else {
		LOG(WARNING) << "addFileUrl should be string!";
	}

	if (!jcfg["dbInsertUrl"].is_null() && jcfg["dbInsertUrl"].is_string()) {
		config.dbInsertUrl = jcfg["dbInsertUrl"].get<string>();
	}
	else {
		LOG(WARNING) << "dbInsertUrl should be string!";
	}

	if (!jcfg["httpRequestTimeout"].is_null() && jcfg["httpRequestTimeout"].is_number_integer()) {
		config.httpRequestTimeout = jcfg["httpRequestTimeout"].get<int>();
	}
	else {
		LOG(WARNING) << "httpRequestTimeout should be integer!";
	}

	if (!jcfg["traceLog"].is_null() && jcfg["traceLog"].is_number_integer()) {
		config.traceLog = jcfg["traceLog"].get<int>();
	}
	else {
		LOG(WARNING) << "traceLog should be integer!";
	}

	if (config.traceLog) {
		el::Configurations defaultConf(*el::Loggers::getLogger("default")->configurations());
		defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
		el::Loggers::reconfigureLogger(el::Loggers::getLogger("default", true), defaultConf);
	}
}
#pragma endregion


#pragma region CURL




void curlDbInsert(string b_url, string worldname, string missionName, string missionDuration, string filename, int timeout) {
	LOG(INFO) << worldname << missionName << missionDuration << filename;
	try {
		CURL *curl;
		CURLcode res;
		curl = curl_easy_init();
		if (curl) {
			stringstream ss;
			ss << b_url << "&worldName="; char * url = curl_easy_escape(curl, worldname.c_str(), 0); ss << url;  curl_free(url);
			ss << "&missionName="; url = curl_easy_escape(curl, missionName.c_str(), 0); ss << url;  curl_free(url);
			ss << "&missionDuration="; url = curl_easy_escape(curl, missionDuration.c_str(), 0); ss << url;  curl_free(url);
			ss << "&filename="; url = curl_easy_escape(curl, filename.c_str(), 0); ss << url;  curl_free(url);
			curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)timeout);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				LOG(ERROR) << "Curl error:" << curl_easy_strerror(res) << ss.str();
			else
				LOG(INFO) << "Curl OK:" << ss.str();

			curl_easy_cleanup(curl);
			
		}
	}
	catch (...) {
		LOG(ERROR) << "Curl unknown exception!";
	}
}


void curlUploadFile(string url, string file, string fileName, int timeout) {
	LOG(INFO) << fileName << file << timeout << url;
	try {
		CURL *curl;
		CURLcode res;
		curl_mime *form = NULL;
		curl_mimepart *field = NULL;
		struct curl_slist *headerlist = NULL;
		static const char buf[] = "Expect:";

		curl = curl_easy_init();
		if (curl) {
			form = curl_mime_init(curl);

			field = curl_mime_addpart(form);
			curl_mime_name(field, "fileContents");
			curl_mime_filedata(field, file.c_str()); 

			field = curl_mime_addpart(form);
			curl_mime_name(field, "fileName");
			curl_mime_data(field, fileName.c_str(), CURL_ZERO_TERMINATED); 
			field = curl_mime_addpart(form);
			curl_mime_name(field, "submit");
			curl_mime_data(field, "send", CURL_ZERO_TERMINATED);
			headerlist = curl_slist_append(headerlist, buf);

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)timeout);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
			curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

			res = curl_easy_perform(curl);

			stringstream total;
			if (!res) {
				curl_off_t ul; 
				double ttotal;
				res = curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &ul);
				if (!res)
					total << "Uploaded " << ul << " bytes";
				res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &ttotal);
				if (!res)
					total << " in " <<  ttotal << " sec.";
				
			}
			total << " URL:" << url;

			if (res != CURLE_OK)
				LOG(ERROR) << "Curl error:" << curl_easy_strerror(res) << total.str();
			else
				LOG(INFO) << "Curl OK:" << total.str();

			curl_easy_cleanup(curl);
		}
	}
	catch (...) {
		LOG(ERROR) << "Curl unknown exception!";
	}


}

void curlActions(string worldName, string missionName, string duration, string filename, string tfile) {
	LOG(INFO) << worldName, missionName, duration, filename, tfile;
	if (!curl_init) {
		curl_global_init(CURL_GLOBAL_ALL);
		curl_init = true;
	}
	curlDbInsert(config.dbInsertUrl, worldName, missionName, duration, filename, config.httpRequestTimeout);
	curlUploadFile(config.addFileUrl, tfile, filename, config.httpRequestTimeout);
	LOG(INFO) << "Finished!";
}

#pragma endregion

#pragma region Commands Handlers

int commandMarkerCreate(const vector<string> &args) {
	COMMAND_CHECK_INPUT_PARAMETERS(11)
		COMMAND_CHECK_WRITING_STATE

		//создать новый маркер
		if (j["Markers"].is_null()) {
			j["Markers"] = json::array();
		}
	/* входные параметры
	[0:_mname , 1: 0, 2 : swt_cfgMarkers_names select _type, 3: _mtext, 4: ocap_captureFrameNo, 5:-1, 6: _pl getVariable ["ocap_id", 0],
	7: call bis_fnc_colorRGBtoHTML, 8:[1,1], 9:side _pl call BIS_fnc_sideID, 10:_mpos]]
	*/


	json::string_t clr = json::string_t(removeAdd(prepStr(args[7].c_str()).c_str()));
	if (clr == "any") {
		clr = "000000";
	}
	json frameNo = json::parse(args[4].c_str());
	json a = json::array({ json::parse(args[0].c_str()), json::parse(args[1].c_str()), json::parse(args[2].c_str()), json::string_t(prepStr(args[3].c_str())), frameNo, json::parse(args[5].c_str()), json::parse(args[6].c_str()),
		clr, json::parse(args[8].c_str()), json::parse(args[9].c_str()), json::array() });
	json coordRecord = json::array({ frameNo, json::parse(args[10].c_str()), json::parse(args[1].c_str()) });
	a[10].push_back(coordRecord);
	j["Markers"].push_back(a);
	return 0;
}

int commandMarkerDelete(const vector<string> &args) {
	//найти старый маркер и поставить ему текущий номер фрейма
	COMMAND_CHECK_INPUT_PARAMETERS(2)
		COMMAND_CHECK_WRITING_STATE

		json mname = json::parse(args[0].c_str());
	auto it = find_if(j["Markers"].rbegin(), j["Markers"].rend(), [&](const auto & i) { return i[0] == mname; });
	if (it == j["Markers"].rend()) {
		LOG(ERROR) << "No such marker" << args[0];
		return 7;
	}

	(*it)[5] = json::parse(args[1].c_str());
	return 0;
}

int commandMarkerMove(const vector<string> &args) {
	COMMAND_CHECK_INPUT_PARAMETERS(3)
		COMMAND_CHECK_WRITING_STATE

		json mname = json::parse(args[0].c_str()); // имя маркера
	auto it = find_if(j["Markers"].rbegin(), j["Markers"].rend(), [&](const auto & i) { return i[0] == mname; });
	if (it == j["Markers"].rend()) {
		LOG(ERROR) << "No such marker" << args[0];
		return 7;
	}
	json coordRecord = json::array({ json::parse(args[1].c_str()), json::parse(args[2].c_str()), 0 });
	// ищем последнюю запись с таким же фреймом
	int frame = coordRecord[0].get<int>();
	auto coord = find_if((*it)[10].rbegin(), (*it)[10].rend(), [&](const auto & i) { return i[0] == frame; });
	if (coord == (*it)[10].rend()) {
		// такой записи нет
		LOG(TRACE) << "No marker coord on this frame. Adding new." << coordRecord;
		(*it)[10].push_back(coordRecord);
	}
	else
	{
		LOG(TRACE) << "Record on this frame already exists." << *coord << "replacing it with new params" << coordRecord;
		*coord = coordRecord;
	}
	return 0;
}

int commandLog(const vector<string> &args) {
	stringstream ss;
	for (int i = 0; i < args.size(); i++)
		ss << args[i] << " ";
	CLOG(WARNING, "ext") << ss.str();
	return 0;
}

int commandFired(const vector<string> &args)
{
	COMMAND_CHECK_INPUT_PARAMETERS(3)
		COMMAND_CHECK_WRITING_STATE


		int id = stoi(args[0]);
	if (!j["entities"][id].is_null()) {
		j["entities"][id]["framesFired"].push_back(json::array({ json::parse(args[1].c_str()), json::parse(args[2].c_str()) }));
	}
	else {
		ERROR_THROW(E_INCORRECT_COMMAND_PARAM, 0)
			return 1;
	}

	return 0;
}

int commandStart(const vector<string> &args) {
	COMMAND_CHECK_INPUT_PARAMETERS(4)

		if (is_writing) {
			LOG(WARNING) << ":START: while writing mission. Clearing old data.";
			commandClear(args);
		}
	LOG(INFO) << "Closing old log. Starting record." << args[0] << args[1] << args[2] << args[3];

	auto loggerDefault = el::Loggers::getLogger("default");
	auto loggerExt = el::Loggers::getLogger("ext");

	if (loggerDefault && loggerExt) {

		loggerDefault->reconfigure();
		loggerExt->reconfigure();
	}
	else {
		LOG(ERROR) << "Problem with reconfiguring loggers!";
	}

	is_writing = true;
	j["worldName"] = json::parse(args[0].c_str());
	j["missionName"] = json::string_t(prepStr(args[1].c_str()));
	j["missionAuthor"] = json::parse(args[2].c_str());
	j["captureDelay"] = json::parse(args[3].c_str());

	LOG(INFO) << "Starting record." << args[0] << args[1] << args[2] << args[3];
	CLOG(INFO, "ext") << "Starting record." << args[0] << args[1] << args[2] << args[3];
	return 0;
}

int commandNewUnit(const vector<string> &args) {
	COMMAND_CHECK_INPUT_PARAMETERS(6)
		COMMAND_CHECK_WRITING_STATE

		json unit { { "startFrameNum", json::parse(args[0].c_str()) }, { "type" , "unit" },
			  { "id", json::parse(args[1].c_str()) }, { "name", json::string_t(prepStr(args[2].c_str())) },
			  { "group", json::parse(args[3].c_str()) }, { "side", json::parse(args[4].c_str()) },
			  { "isPlayer", json::parse(args[5].c_str()) }
	};
	unit["positions"] = json::array();
	unit["framesFired"] = json::array();
	j["entities"].push_back(unit);
	return 0;
}

int commandNewVeh(const vector<string> &args) {
	COMMAND_CHECK_INPUT_PARAMETERS(4)
		COMMAND_CHECK_WRITING_STATE

		json unit { { "startFrameNum", json::parse(args[0].c_str()) },
				  { "type" , "vehicle" }, { "id", json::parse(args[1].c_str()) },
				  { "name", json::string_t(prepStr(args[3].c_str())) },
				  { "class", json::parse(args[2].c_str()) }
	};
	unit["positions"] = json::array();
	unit["framesFired"] = json::array();
	j["entities"].push_back(unit);
	return 0;
}


int commandSave(const vector<string> &args) {
	COMMAND_CHECK_INPUT_PARAMETERS(5)
		COMMAND_CHECK_WRITING_STATE
		LOG(INFO) << args[0] << args[1] << args[2] << args[3] << args[4];

	j["worldName"] = json::parse(args[0].c_str());
	j["missionName"] = json::string_t(prepStr(args[1].c_str()));
	j["missionAuthor"] = json::parse(args[2].c_str());
	j["captureDelay"] = json::parse(args[3].c_str());
	j["endFrame"] = json::parse(args[4].c_str());

	prepareMarkerFrames(j["endFrame"]);

	string tName = saveCurrentReplayToTempFile();
	string fname = generateResultFileName(j["missionName"].get<std::string>());
	curlActions(json::parse(args[0].c_str()), j["missionName"].get<std::string>(), to_string(stod(args[3]) * stod(args[4])), fname, tName);
	
	return commandClear(args);
}


int commandClear(const vector<string> &args)
{
	COMMAND_CHECK_WRITING_STATE
		LOG(INFO) << "CLEAR";
	j.clear();
	is_writing = false;
	return 0;
}

int commandUpdateUnit(const vector<string> &args)
{
	COMMAND_CHECK_INPUT_PARAMETERS(7)
		COMMAND_CHECK_WRITING_STATE

		int id = stoi(args[0]);
	if (!j["entities"][id].is_null()) {
		j["entities"][id]["positions"].push_back(json::array({ json::parse(args[1].c_str()), json::parse(args[2].c_str()),
			json::parse(args[3].c_str()), json::parse(args[4].c_str()), json::string_t(prepStr(args[5].c_str())), json::parse(args[6].c_str()) }));
	}
	else {
		ERROR_THROW(E_INCORRECT_COMMAND_PARAM, 0)
	}

	return 0;
}

int commandUpdateVeh(const vector<string> &args)
{
	COMMAND_CHECK_INPUT_PARAMETERS(5)
		COMMAND_CHECK_WRITING_STATE

		int id = stoi(args[0]);
	if (!j["entities"][id].is_null()) {
		j["entities"][id]["positions"].push_back(json::array({ json::parse(args[1].c_str()), json::parse(args[2].c_str()),
			json::parse(args[3].c_str()), json::parse(args[4].c_str()) }));
	}
	else {
		ERROR_THROW(E_INCORRECT_COMMAND_PARAM, 0)
	}

	return 0;
}

int commandEvent(const vector<string> &args)
{
	COMMAND_CHECK_WRITING_STATE
		if (args.size() < 3) {
			ERROR_THROW(E_NUMBER_ARGS_VAR, args.size(), "<3")
		}
	json arr = json::array();
	for (int i = 0; i < args.size(); i++) {
		arr.push_back(json::parse(args[i].c_str()));
	}
	j["events"].push_back(arr);
	return 0;
}



#pragma endregion

void initialize_logger(bool forcelog = false, int verb_level = 0) {
	bool file_exists = forcelog;
	if (std::ifstream(LOG_NAME))
		file_exists = true;

	el::Configurations defaultConf;
	defaultConf.setToDefault();
	if (!file_exists) {
		defaultConf.setGlobally(el::ConfigurationType::Enabled, "false");
		defaultConf.setGlobally(el::ConfigurationType::ToFile, "false");
	}
	else {
		defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
		defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
		defaultConf.setGlobally(el::ConfigurationType::Filename, LOG_NAME);
	}

	defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
	defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %thread [%fbase:%line:%func] %level %msg");
	defaultConf.setGlobally(el::ConfigurationType::MillisecondsWidth, "4");
	defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600"); // 100 Mb
	defaultConf.setGlobally(el::ConfigurationType::LogFlushThreshold, "100");

	defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");

	el::Configurations externalConf;
	externalConf.setToDefault();
	externalConf.setFromBase(&defaultConf);
	externalConf.setGlobally(el::ConfigurationType::Filename, LOG_NAME_EXT);
	externalConf.setGlobally(el::ConfigurationType::Format, "%datetime %msg");

	//el::Loggers::reconfigureAllLoggers(defaultConf);
	//el::Loggers::setDefaultConfigurations(defaultConf);
	el::Loggers::reconfigureLogger(el::Loggers::getLogger("default", true), defaultConf);
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
	el::Loggers::addFlag(el::LoggingFlag::AutoSpacing);
	//el::Loggers::addFlag(el::LoggingFlag::ImmediateFlush);
	//el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::setVerboseLevel(verb_level);
	//	el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
	el::Helpers::validateFileRolling(el::Loggers::getLogger("default"), el::Level::Info);

	el::Loggers::reconfigureLogger(el::Loggers::getLogger("ext", true), externalConf);
	el::Helpers::validateFileRolling(el::Loggers::getLogger("ext"), el::Level::Info);
		
	LOG(INFO) << "Logging initialized " << CURRENT_VERSION << " build: " << __TIMESTAMP__;
	CLOG(INFO, "ext") << "External logging initialized " << CURRENT_VERSION << " build: " << __TIMESTAMP__;
}


extern "C"
{
	__declspec (dllexport) void __stdcall RVExtensionVersion(char *output, int outputSize);
	__declspec (dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
	__declspec (dllexport) int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
}

void __stdcall RVExtensionVersion(char *output, int outputSize)
{
	strncpy_s(output, outputSize, CURRENT_VERSION, _TRUNCATE);
}

void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
	LOG(ERROR) << "IN:" << function << " OUT:" << MSG_ERROR_NOT_SUPPORTED;
	strncpy_s(output, outputSize, MSG_ERROR_NOT_SUPPORTED, _TRUNCATE);
}

int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt)
{
	int res = 0;
	if (config.traceLog) {
		stringstream ss;
		ss << function << " " << argsCnt << ":[";
		for (int i = 0; i < argsCnt; i++) {
			if (i > 0)
				ss << "::";
			ss << args[i];
		}
		ss << "]";
		LOG(TRACE) << ss.str();
	}

	try {
		auto fn = dll_commands.find(function);
		if (fn == dll_commands.end()) {
			ERROR_THROW(E_FUN_NOT_SUPPORTED, function)
		}
		else {
			// res = fn->second(args, argsCnt);
			// append to commands
			string str_function(function);
			vector<string> str_args;
		
			for (int i = 0; i < argsCnt; ++i) 
			{
				str_args.push_back(string(args[i]));
			}
			{
				unique_lock<mutex> lock(command_mutex);
				commands.push(std::make_tuple(std::move(str_function), std::move(str_args)));
			}
			if (!command_thread.joinable())
			{
				LOG(TRACE) << "No worker thread. Creating one!";
				command_thread = thread(command_loop);
			}
			command_cond.notify_one();
		}
	}
	catch (const exception &e) 
	{
		LOG(ERROR) << "Exception: " << e.what();
	}
	catch (...) 
	{
		LOG(ERROR) << "Exception: Unknown";
	}

	return res;
}


// Normal Windows DLL junk...
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		initialize_logger(true);
		readWriteConfig(hModule);
		break;
	}

	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH: {
		if (curl_init)
			curl_global_cleanup();
		command_thread_shutdown = true;
		command_cond.notify_one();
		if (command_thread.joinable())
			command_thread.join();
		break;

	}
	}
	return TRUE;
}
