<?php
$conn = new mysqli('localhost', 'root', '', 'Security_db');

if ($conn->connect_error) {
    die("Connection error: " . $conn->connect_error);
}
?>
