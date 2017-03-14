<?php
exec('./a3 index.wpml n n 0 0 0', $index);
foreach($index as $i)
	echo $i;
?>