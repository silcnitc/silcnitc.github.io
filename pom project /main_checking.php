<html>
<head>
<title> Student Info </title>

</head>
<body>








<?php
$username1="root";
$password1="abc";
$database="pom_project";


$roll_no =  $_POST['roll_no'];

mysql_connect("localhost",$username1,$password1);
@mysql_select_db($database) or die( "Unable to select database");

$query="select * from  student_info where college_id = '$roll_no'";
mysql_query($query) or die (mysql_error());
$result=mysql_query($query);

$num=mysql_fetch_array($result, $result_type = null);

mysql_query($query) or die (mysql_error());
$result=mysql_query($query);

$num=mysql_numrows($result);
$i=0;
echo "the details of the student are";
echo "<br>";
while ($i < $num)
{$college_id=mysql_result($result,$i,"college_id");
 $name=mysql_result($result,$i,"name");
$phone_no=mysql_result($result,$i,"phone_no");
$address=mysql_result($result,$i,"address");
$email=mysql_result($result,$i,"email");


echo "<table> 
<tr>
<td> college_id :</td> <td> $college_id </td> 
</tr>"; 
// echo "<b>$college_id<br>$name<br>$phone_no<br>$address<br>$email<br></b>";
$i++;
}
echo "<img src = 'upload/".$roll_no.".jpg' height = '250px'>";

mysql_close();

?>

<table align="center"  border="1" cellpadding="2" cellspacing="2">

<form action="checking_info.php" method="POST">

<tr>
<td><input type="submit" value="CHECKOUT">
</td>
<td><input type="hidden" value="<?php echo $college_id ;?>" name="college_id"> </td>
</tr>
</form>
</table>

</body>


</html>




