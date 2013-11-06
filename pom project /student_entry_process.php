<?php
$username1="root";
$password1="abc";
$database="pom_project";


$college_id =  $_POST['college_id'];
$name = $_POST['name'];
$address = $_POST['address'];
$mobile = $_POST['mobile_no'];
$email = $_POST['email'];
if($college_id !="" && $name !="" && $address !="" && $mobile !="" && $email !="") {
mysql_connect('localhost',$username1,$password1);
@mysql_select_db($database) or die( "Unable to select database");

$query="INSERT INTO student_info VALUES('$college_id','$name','$mobile','$address','$email')";
mysql_query($query) or die (mysql_error());
 if ($_FILES["image"]["error"] > 0)
    {
    echo "Return Code: " . $_FILES["image"]["error"] . "<br>";
    }
    else {
move_uploaded_file($_FILES['image']['tmp_name'],"upload/" . $college_id.".jpg") or die("unable to upload image");
      //echo "Stored in: " . "upload/" . $college_id.".jpg"; 
      }  
      mysql_close();
      echo "thanks for your cooperation. You can close this page now";
      } //ending of if
      else { echo "this was not a valid response please go back to previous page and fill your details again <br> P.S You can not leave any field empty"; }




?>