<?php
$user = $_POST["user"];
exec('./a2/view.py listStream ' . $user . ' 2>&1', $index);
foreach($index as $i)
	echo $i;
?>