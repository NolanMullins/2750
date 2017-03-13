<?php
exec("./a2/view.py listStream nolan", $index);
foreach($index as $i)
	echo $i;
?>