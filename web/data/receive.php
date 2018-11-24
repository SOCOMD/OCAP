<?php

include "../common.php";

if (count($_GET) == 0) {
	echo "No data received.";
	exit;
}

if (!file_exists("data.db")) {
	echo "Database not found. Please ensure you have ran the installer first.";
	exit;
};
if (($_SERVER['REMOTE_ADDR'] != "195.88.209.214") && ($_SERVER['REMOTE_ADDR'] != "193.19.118.241")) die("Don't hack me! ".$_SERVER["REMOTE_ADDR"]);

$option = $_GET["option"];

//echo "Raw data received: " .implode(" ", $_GET) . "\n";
$string = "";
foreach($_GET as $key => $value) {
	$string .= $key . ": " . $value . "\n";
}

// Truncate string if too large
if (strlen($string) > 500) {
	$string = substr($string, 0, 500) . "...";
}
echo "Processed data: \n". $string . "\n";

$forbiddenChar = array("\\", "/", ":", "*", "?", "\*", "<", ">", "|", "\"");

if ($option == "addFile") { // Add receieved file to this directory
	// Проверяем лог на имя файла
	$fileopen=fopen("POST.log", "a+");
	$write =  date ("Y-m-d H:i:s") . "POST[fileName]:" . $_POST['fileName'] . " urldecode - " . urldecode($_POST["fileName"]) . "\r\n";
	fwrite($fileopen,$write);
	fclose($fileopen);
	
	$fileName = str_replace($forbiddenChar, "", $_POST["fileName"]);
	$fileContents = $_FILES["fileContents"];

	try {
		if (move_uploaded_file($fileContents["tmp_name"],$fileName )) {
			echo "Successfully created file.";
		} else {
			echo "No successfully created file.";
		};
	} catch (Exception $e) {
		echo $e->getMessage();
	};
	$data = implode("", file($fileName));
	$gzdata = gzencode($data, 9);
	$fp = fopen("$fileName.gz", "w");
	fwrite($fp, $gzdata);
	fclose($fp);
} elseif ($option == "dbInsert") { // Insert values into SQLite database
	$date = date("Y-m-d");
	$serverId = -1;
	try {
		$db = new PDO('sqlite:data.db');
		if (!isSet($_GET["type"])) {$type = "other";} else {$type = $_GET["type"];};
		// Add operation to database
		/*
		$db->exec(sprintf("
			INSERT INTO operations (world_name, mission_name, mission_duration, filename, date, type) VALUES (
				'%s',
				'%s',
				%d,
				'%s',
				'%s',
				'%s'
			)
		", addslashes($_GET["worldName"]), addslashes($_GET["missionName"]), $_GET["missionDuration"], addslashes($_GET["filename"]), $date, $type));*/
		// TODO: Increment local capture count
		$stmt = $db -> prepare("INSERT INTO operations (world_name, mission_name, mission_duration, filename, date, type) VALUES (
				:world_name,
				:mission_name,
				:mission_duration,
				:filename,
				:date,
				:type
			)");
		$stmt -> execute(array(
			'world_name' => $_GET["worldName"],
			'mission_name' => $_GET["missionName"],
			'mission_duration' => $_GET["missionDuration"],
			'filename' => str_replace($forbiddenChar, "", $_GET["filename"]),
			'date' => $date,
			'type' => $type
			));
		// Get server ID
		$results = $db->query("SELECT remote_id FROM servers");
		$serverId = $results->fetch()["remote_id"];
		$db = null;
		print_debug($serverId);

	} catch (PDOExecption $e) {
		echo "Exception: ".$e->getMessage();
	}

	// TODO: Increment capture count on remote database
	
	$result = curlRemote("stats-manager.php", array(
		"option" => "increment_capture_count",
		"server_id" => $serverId
	));

	print_debug($result);
} else {
	echo "Invalid option";
}

?>