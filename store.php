\<?php
require 'connect.php';

$distance   = floatval($_GET['distance_detected']);
$led_status = intval($_GET['led_status']);

$stmt = $conn->prepare("INSERT INTO detections (distance_detected, led_status) VALUES (?, ?)");
$stmt->bind_param("di", $distance_detected, $led_status);
$stmt->execute();
$stmt->close();

echo "OK";
?>
