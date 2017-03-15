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
	exec('./a2/post "'.$user.'" '.$textStream.' '.$textData . ' 2>&1', $info);
	echo end($info) . '<br>';
}
else if (isset($_POST["streamChange"]) && $_POST["streamChange"] == 1)
{
	echo 'new stream is: '.$stream.' <br>';
	exec('./a2/view.py changeStream '.$user.' '.$stream.' 2>&1', $streamInfo);
	foreach ($streamInfo as $tmp)
		echo $tmp.'<br>';
	//load read index and num posts
}

echo 'Logged in as: ' . $user . ' in stream: ' . $stream;
exec('./a3 display.wpml ' . $user . ' ' . $stream . ' '. $index .' '. $size .' '. $order .' 2>&1', $index);
foreach($index as $i)
	echo $i;
?>