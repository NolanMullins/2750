<?php
$user = $_POST["user"];
$stream = $_POST["stream"];
$index = $_POST["index"];
$size = $_POST["size"];
$order = $_POST["order"];
exec('./a3 submitPost.wpml ' . $user . ' '. $stream .' ' . $index .' ' . $size . ' ' . $order . ' 2>&1', $index);
foreach($index as $i)
	echo $i;
?>