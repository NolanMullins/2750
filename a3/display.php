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
	/*foreach ($streamInfo as $sInfo)
	{
		echo $sInfo.' <br>';
	}*/
	$index = $streamInfo[0];
	$size = $streamInfo[1];
	exec('./a2/view.py nextPost '.$user.' '.$stream.' '.$size.' '.$index.' '.$order.' 2>&1', $nextInfo);
	foreach ($nextInfo as $nInfo)
		echo $nInfo.'<br>';
	//foreach ($streamInfo as $tmp)
	//	echo $tmp.'<br>';
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
	if ($index+1 >= $size)
	{
		echo 'End has been reached <br>';
	}
	else
	{
		$index = $index + 1;
		exec('./a2/view.py nextPost '.$user.' '.$stream.' '.$size.' '.$index.' '.$order.' 2>&1', $nextInfo);
		foreach ($nextInfo as $nInfo)
			echo $nInfo.'<br>';	
	}
	
	unset($_POST['next']);
}
else if (isset($_POST["prev"]) && $_POST["prev"] == 1)
{
	//load prev post
	if ($index <= 0)
	{
		echo 'Already at first post <br>';
	}
	else
	{
		$index = $index - 1;
		exec('./a2/view.py prevPost '.$user.' '.$stream.' '.$size.' '.$index.' '.$order.' 2>&1', $prevInfo);
		foreach ($prevInfo as $pInfo)
			echo $pInfo.'<br>';	
	}
	
	unset($_POST['prev']);
}
echo 'Logged in as: ' . $user . ' in stream: ' . $stream;
//echo '<br> ./a3 display.wpml ' . $user . ' ' . $stream . ' - '. $index .' - '. $size .' - '. $order.' <br';
exec('./a3 display.wpml ' . $user . ' ' . $stream . ' '. $index .' '. $size .' '. $order .' 2>&1', $index);
foreach($index as $i)
	echo $i;
?>