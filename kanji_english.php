<?php 
include_once("simplehtmldom_1_5/simple_html_dom.php");
die();
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
			$id = $row["id"];
			$parent_id = $row["parent_id"];
			$type = $row["type"];
			$mean = $row["mean"];
			$level = $row["level"];
			$on = $row["on"];
			$kun = $row["kun"];
			$writing = $row["writing"];
			$detail = $row["detail"];
			$freq = $row["freq"];
			$comp = $row["comp"];
			$stroke_count = $row["stroke_count"];
			$page = $row["page"];
			$content = $row["content"];
			$content = str_replace("'","\'",$content);
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
								$mean = $itemTemp;
							} else if ($i == 3) {
								$j = 0;
								$k = 0;
								

								foreach($item->find("table tr") as $trItem){
									if($j == 0){
										$j++;
										continue;
									} else {
										$l = 0;
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
						echo "<p>".$mean."</p>";
						echo "<pre>";
						print_r($example);
						echo "</pre>";
						//ghi vào cơ sở dữ liệu cho phần kanji
						$query_kanji = "INSERT INTO kanji_eng_data (`id`, `parent_id`, `type`, `kanji_name`, `mean`, `level`, `on`, `kun`, `writing`, `detail`, `freq`, `comp`, `stroke_count`, `page`, `content`) VALUES ('$id', '$parent_id', '$type', '$name_kanji', '$mean', '$level', '$on', '$kun', '$writing', '$detail', '$freq', '$comp', '$stroke_count', '$page', '$content')";
						if ($conn->query($query_kanji) === TRUE) {
							//echo "New record created successfully";
						} else {
							$check = true;
							echo "Error: " . $query_kanji . "<br>" . $conn->error;
						}
						//ghi vào cơ sở dữ liệu cho phần kanji ví dụ
						$index_vd = 0;
						foreach ($example as $item_ex) {
							$parent_id = $id;
							$type = "kanji";
							$lesson_name = $name_kanji;
							$w = $item_ex[0];
							$p = $item_ex[1];
							$m = $item_ex[2];
							$h = "";
							$order = $index_vd;
							$query_ex = "INSERT INTO kanji_example_eng (`parent_id`, `type`, `level`, `lesson_name`, `w`, `p`, `m`, `h`, `order`) VALUES ('$parent_id', '$type', '$level', '$lesson_name', '$w', '$p', '$m', '$h', '$order')";
							if ($conn->query($query_ex) === TRUE) {
								//echo "New record created successfully";
							} else {
								$check = true;
								echo "Error: " . $query_ex . "<br>" . $conn->error;
							}
							$index_vd += 1;
						}
						//die();
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