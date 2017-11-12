<?php
    //start PHP session
   session_start();
   //connect to database
   $db = mysqli_connect("eu-cdbr-azure-north-e.cloudapp.net","bddef64b53f7a1","9d65b26f","acsm_07f2d8d959cb2ba");
   
   //test whether POST data is set
   if($_SERVER["REQUEST_METHOD"] == "POST") {

      // username and password sent from form 
      $myusername = mysqli_real_escape_string($db,$_POST['username']);
      $mypassword = mysqli_real_escape_string($db,$_POST['password']); 
      
      //get id of current user
      $sql = "SELECT id FROM rsslogin WHERE email = '$myusername' and password = '$mypassword'";
      $result = mysqli_query($db,$sql);
      $row = mysqli_fetch_array($result,MYSQLI_ASSOC);
      //get number of records returned
      $count = mysqli_num_rows($result);
      
      // If result matched $myusername and $mypassword, table row must be 1 row
      if($count == 1) 
      {
          //set session to store current username
         $_SESSION['login_user'] = $myusername;
         //create session to store list of watched articles and index pointer
         $_SESSION['index'] = 0;
         $arr = array("","","","","","","","","","","","","","","","","","","","");
         $_SESSION['watched'] = $arr;
         //redirect to rssloader.php
         header("location: rssloader.php");
      }else {
         $error = "Your Login Name or Password is invalid";
      }
   }
?>
<html>
<head>
    <title>Login Page</title>
    <link rel="stylesheet" type="text/css" href="index.css" />
</head>
<body>
    <div id="header">
        <a href="index.php" id="WebTitle"><h1>Yours Daily</h1></a>
        <br>    
	</div>
	<div align = "center" >
        <div style = "width:300px; border: solid 1px #333333; " align = "left" id="loginB">
            <div style = "background-color:#333333; color:#FFFFFF; padding:3px;">
                <b>Login</b>
            </div>
			    <div style = "margin:30px">
                    <form action = "" method = "post">
                  <label>Username  :</label><input type = "text" name = "username" class = "box"/><br /><br />
                  <label>Password  :</label><input type = "password" name = "password" class = "box" /><br/><br />
            
                        <div align = "center">
                  
                            <input type = "submit" value = " Submit "/><br />
             </form>
          </div>
               <div style = "font-size:11px; color:#cc0000; margin-top:10px">

<div align = "center">

</div>
					
            </div>
				
         </div>
			
      </div>

   </body>
 <br></br>

	
      <div align = "center" >
         <div style = "width:200px; border: solid 1px #333333; " align = "left" id="createB">
            <div style = "background-color:#333333; color:#FFFFFF; padding:3px;"><b>Create an Account</b></div>				
            <div style = "margin:30px">
               
<div align = "center">
                  <label>Create an Account</label>
</div>
<br />
<div align = "center">
    <form action="create.php">
        <input type="submit" value="Create a new account"/>
    </form>
               
               
              </div>
					
            </div>
         </div>
      </div>
</html>