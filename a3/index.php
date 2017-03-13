<?php
exec("./a3 index.wpml", $index);
foreach($index as i)
	echo i;
?>