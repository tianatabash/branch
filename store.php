\<?php
require 'connect.php';

$distance   = floatval($_GET['distance_detected']);
$Event_status = intval($_GET['Event_status']);

$stmt = $conn->prepare("INSERT INTO detections (distance_detected, Event_status) VALUES (?, ?)");
$stmt->bind_param("id", $distance_detected, $Event_status);
$stmt->execute();
$stmt->close();

echo "OK";
?>
