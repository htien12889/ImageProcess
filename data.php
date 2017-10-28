<?php 
include_once("simplehtmldom_1_5/simple_html_dom.php");
$link = "http://kanjicards.org/kanji-list-by-jlpt-level.html";
$link_arrays = array();
$url = "http://kanjicards.org";
$html = file_get_html($link);
$i = 0;
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "jptest";
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
$conn->set_charset("utf8");
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
echo chr(0xEF).chr(0xBB).chr(0xBF);
header('Content-Type: text/html; charset=utf-8');



foreach($html->find("#main-copy a") as $element){
	$link_arrays[$i] = $url.$element->href;
	$i++;
}
$check = false;
foreach ($link_arrays as $link_array) {
	$html_content = file_get_html($link_array);
	$name = "";
	$content = "";
	foreach($html_content->find("#main-copy") as $elements){
		foreach ($elements->find("h2") as $kanji) {
			$name = $kanji;
			$name = str_replace('<h2>', '', $name);
			$name = str_replace('</h2>', '', $name);
		}
		foreach ($elements->find("ul") as $kanji_content) {
			$content = $kanji_content;
		}
		$query = "INSERT INTO kanji_eng (`name`, `content`) values ('$name', '$content')";
		if ($conn->query($query) === TRUE) {
			//echo "New record created successfully";
		} else {
			$check = true;
			echo "Error: " . $query . "<br>" . $conn->error;
		}
	}
	if($check){
		break;
	}
}
$conn->close();
echo "<h1>Successfully</h1>";

?>