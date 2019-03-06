<?php
    header('Content-Type: application/json');
    $dbPath = "data/data.db";
    $db = new PDO('sqlite:' . $dbPath);
    $error = 0;
    $ops = array();
    if (!file_exists($dbPath)) {
        $error = 1;
    } else {
		/*
        // Get list of operations from DB
        $req = "SELECT * FROM operations WHERE
            type LIKE '%".addslashes($_POST['type'])."%' AND
            mission_name LIKE '%".addslashes($_POST["name"])."%' AND 
            date <= '".addslashes($_POST['older'])."' AND 
            date >= '".addslashes($_POST['newer'])."'";
        $result = $db->query($req);
        //$result = $db->query("SELECT * FROM operations");
		
		*/
		//var_dump($_POST);
		$q = "SELECT * FROM operations WHERE date <= :older AND date >= :newer";
		$p = array(
			'older' => $_POST['older'],
			'newer' => $_POST['newer']
        );
		//var_dump($q);
		if ($_POST['type'] != '') {
			$q .= " AND type LIKE :type ";
			$p['type'] = $_POST['type'];
		}
			
		if ($_POST['name']) {
			$q .= " AND mission_name LIKE :name ";
			$p['name'] = "%{$_POST['name']}%";
		}
			
		$stmt = $db -> prepare($q);
		$stmt -> execute($p);
		$result = $stmt->fetchAll();
		//$stmt->debugDumpParams();
		//print_r($result);
        $ops = array();
        foreach($result as $row) {
            $ops[] = $row;
        }

        $db = NULL;
    };
?>
{
    "error" : <?php echo json_encode($error) ?>,
    "list"  : <?php echo json_encode($ops) ?>
}

