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
else if (isset($_POST["allRead"]) && $_POST["allRead"] == 1)
{
	exec('./a2/view.py markAllRead '.$user.' '.$stream.' '.$size.' 2>&1', $allReadInfo);
	foreach ($allReadInfo as $allRead)
		echo $allRead.'<br>';	
}
else if (isset($_POST["next"]) && $_POST["next"] == 1)
{
	//load next post
	echo 'Need to load next post <br>';
	unset($_POST['next']);
}
else if (isset($_POST["prev"]) && $_POST["prev"] == 1)
{
	//load prev post
	echo 'Need to load prev post <br>';
	unset($_POST['prev']);
}

echo 'Logged in as: ' . $user . ' in stream: ' . $stream;
exec('./a3 display.wpml ' . $user . ' ' . $stream . ' '. $index .' '. $size .' '. $order .' 2>&1', $index);
foreach($index as $i)
	echo $i;
?>