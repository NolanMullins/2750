<?php
$user = $_POST["user"];
$index = $_POST["index"];
$size = $_POST["size"];
$order = $_POST["order"];
exec('./a2/view.py listStream ' . $user . ' ' . $order . ' 2>&1', $index);
foreach($index as $i)
	echo $i;
?>