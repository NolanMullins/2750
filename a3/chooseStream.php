<?php
exec('./a2/view.py listStream nolan 2>&1', $index);
foreach($index as $i)
	echo $i;
?>