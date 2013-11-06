<html>
<head>
<title>processing username and password</title>
</head>
<style>
body
{
background-color:whitesmoke;;;
}
</style>
<body>


<?php
$username1="root";
$password1="abc";
$database="pom_project";
$user_name2=$_POST['name'];
$password2=$_POST['password'];

mysql_connect('localhost',$username1,$password1);
@mysql_select_db($database) or die( "Unable to select database");
$query="SELECT * FROM guard_login";
$result=mysql_query($query);

$num=mysql_numrows($result);

mysql_close();



$i=0;
$check_flag=0;
while ($i < $num) {

$user_name=mysql_result($result,$i,"name");
$password=mysql_result($result,$i,"password");

if($user_name2==$user_name && $password==$password2)
{  $check_flag=1;
	echo "<h1 align ='center'>Thanks $user_name2 for the authorisation Process  </h1></br>";
	echo "</br>";
    echo "</br>";

     echo "</br>";
   break;}
  

$i++;
}
 if($check_flag==0)
 {
  	echo "<h1> ERROR...!!!Username and password combination is not correct, contact Administration </h1><br>";
 }
  

?>
<h1 align="center">Please click <a href="main.php"> here  </a> to proceed further  </h1></br>";
 
</body>

</html>
