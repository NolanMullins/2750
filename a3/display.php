<?php
$user = $_POST["user"];
exec('./a3 display.wpml', $index);
foreach($index as $i)
	echo $i;
?>