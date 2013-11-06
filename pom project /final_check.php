<html>
<head>
<title>Final Call </title>

</head>
<body>








<?php
$username1="root";
$password1="jindmahi";
$database="pom_project";



mysql_connect("localhost",$username1,$password1);
@mysql_select_db($database) or die( "Unable to select database");

$query="select * from  checkin_info,student_info where status ='0'and checkin_info.college_id=student_info.college_id";
mysql_query($query) or die (mysql_error());
$result=mysql_query($query);

$num=mysql_fetch_array($result, $result_type = null);
mysql_query($query) or die (mysql_error());
$result=mysql_query($query);
$i=0;
echo "the details of the student are";
echo "<br>";
$num=mysql_num_rows($result);
while ($i < $num)
{$college_id=mysql_result($result,$i,"college_id");
 $name=mysql_result($result,$i,"name");
$phone_no=mysql_result($result,$i,"phone_no");
$address=mysql_result($result,$i,"address");
$email=mysql_result($result,$i,"email");
$status=mysql_result($result,$i,"status");

echo "<table> 
<tr>
<td> college_id :</td> <td> $college_id </td> 
</tr>
<tr>
<td> phone :</td> <td> $phone_no </td> 
</tr>
<tr>
<td> name :</td> <td> $name </td> 
</tr>"; 
// echo "<b>$college_id<br>$name<br>$phone_no<br>$address<br>$email<br></b>";
$i++;
}


mysql_close();

?>


</body>


</html>




