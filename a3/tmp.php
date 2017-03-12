<?php
	exec('./a.out 2>&1', $ret);
	print_r($ret);
?>