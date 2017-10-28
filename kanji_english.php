<?php 
include_once("simplehtmldom_1_5/simple_html_dom.php");
$i = 0;
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "jlpt";
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
$conn->set_charset("utf8");
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
echo chr(0xEF).chr(0xBB).chr(0xBF);
header('Content-Type: text/html; charset=utf-8');
$query = "select * from kanji";
$result = $conn->query($query);
if($result){
	
	if ($result->num_rows > 0) {
		while($row = $result->fetch_assoc()){
			$name_kanji  = $row["kanji_name"];

			$query_ = "select * from kanji_eng where `name`='$name_kanji'";
			$result_ = $conn->query($query_);				
			if($result_){
				if ($result_->num_rows > 0) {
					while($row_ = $result_->fetch_assoc()) {
						$html = str_get_html($row_["content"]);
						$i = 0;
						$kun = "";
						$on = "";
						$meaning = "";
						$example = array();
						foreach ($html->find("ul li") as $item){
							$itemTemp = $item;
							$itemTemp = str_replace('<li>kun:', '', $itemTemp);
							$itemTemp = str_replace('</li>', '', $itemTemp);
							$itemTemp = str_replace('<li>on:', '', $itemTemp);
							$itemTemp = str_replace('<li>meaning:', '', $itemTemp);
							if($i == 0){
								$kun = $itemTemp;
							} else if ($i == 1) {
								$on = $itemTemp;
							} else if ($i == 2) {
								$meaning = $itemTemp;
							} else if ($i == 3) {
								$j = 0;
								$k = 0;
								$l = 0;

								foreach($item->find("table tr") as $trItem){
									if($j == 0){
										$j++;
										continue;
									} else {
										foreach($trItem->find("td") as $tdItem){
											$example[$k][$l] = str_replace('<td>', '', $tdItem);
											$example[$k][$l] = str_replace('</td>', '', $example[$k][$l]);
											$l++;
										}
									}
									$k++;
								}
							} else {
								break;
							}
							//$content[] = 
							$i++;
						}
						echo "<p>".$kun."</p>";
						echo "<p>".$on."</p>";
						echo "<p>".$meaning."</p>";
						echo "<pre>";
						print_r($example);
						echo "</pre>";
						die();
					}
				} else {
					
				}
			} else {
				
			}
		}
		
	}
}

$conn->close();
echo "<h1>Successfully</h1>";

?>