<?php
$username1="root";
$password1="abc";
$database="pom_project";


$college_id =  $_POST['college_id'];
mysql_connect('localhost',$username1,$password1);
@mysql_select_db($database) or die( "Unable to select database");

$query="INSERT INTO checkin_info(college_id,status) VALUES('$college_id','0')";
mysql_query($query) or die (mysql_error());
header("location:main.php");

?>