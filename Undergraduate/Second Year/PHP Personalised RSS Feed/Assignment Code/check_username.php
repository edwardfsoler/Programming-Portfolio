<?php
    //connect to database  
     $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bddef64b53f7a1";
    $password = "9d65b26f";
    $schemaName = "acsm_07f2d8d959cb2ba";

    $conn = mysqli_connect($localhost, $username, $password, $schemaName) or die("I couldn't connect to your database, please make sure your info is correct!");

    mysqli_select_db($conn, $schemaName) or die("I couldn't find the database table make sure it's spelt right!"); 
      
    //get the username  
    $username = $_POST['username'];  
      
    //mysql query to select field username if it's equal to the username that we check '  
    $sql = "select email from rsslogin where email = '". $username ."'";  
    $result = $conn->query($sql);
      
    //if number of rows fields is bigger them 0 that means it's NOT available '  
    if($result->num_rows > 0){  
        //and we send 0 to the ajax request  
        echo 0;  
    }else{  
        //else if it's not bigger then 0, then it's available '  
        //and we send 1 to the ajax request  
        echo 1;  
    }  
?>

