<?php 
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "jlpt";
$content = '';
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
$conn->set_charset("utf8");
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
echo chr(0xEF).chr(0xBB).chr(0xBF);
header('Content-Type: text/html; charset=utf-8');
$id = $_GET["id"];
$query = "select content from kanji where id='$id'";
$results = $conn->query($query);
if($results){
    while($rows = $results->fetch_assoc()){
        echo $rows["content"];
    }
}
$conn->close();
echo "<h1>Sucessfully</h1>"
?>