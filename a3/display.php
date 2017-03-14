<?php
$user = $_POST["user"];
$stream = $_POST["stream"];
$index = $_POST["index"];
$size = $_POST["size"];
$order = $_POST["order"];

if (isset($_POST["textStream"]))
{
	$textData = $_POST["textData"];
	$textStream = $_POST["textStream"];
	exec('./a2/post "'.$user.'" '.$textStream.' '.$textData);
}

echo 'Logged in as: ' . $user . ' in stream: ' . $stream;
exec('./a3 display.wpml ' . $user . ' ' . $stream . ' '. $index .' '. $size .' '. $order .' 2>&1', $index);
foreach($index as $i)
	echo $i;
?>