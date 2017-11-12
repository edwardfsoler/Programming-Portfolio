<?php
    //start PHP session
    session_start();

if (ISSET($_GET['login'])) {
     //if correct username and password is entered, enter
     if ($_POST['username'] == 'USERNAME' && $_POST['password'] == 'PASSWORD') {
         //set session variable to indicate authorisation
         $_SESSION['loggedin'] = 1;
         //redirect user to viewBids.php page
         header("Location: viewBids.php");
         exit;
     }
}
?>

<!DOCTYPE html>
<html>
	<head>
		<title> GAPT </title>
		<link rel="stylesheet" type="text/css" href="login.css" />
	</head>
	<body>
	    <div id="bg">
	        <div id="header">
			    <h1>Login</h1>
		    </div>		
		    <style type = "text/css">
			    body {
				    font-family:Arial, Helvetica, sans-serif;
				    font-size:14px;
			    }
         
			    label {
				    font-weight:bold;
				    width:100px;
				    font-size:14px;
			    }
         
			    .box {
				    border:#666666 solid 1px;
			    }
      
		    </style>
            <div id="form">
		        <form action="?login=1" method="post">
				        <br/><label>Username</label>
                        <input type = "text" name = "username" class = "box"/><br /><br />
				        <label>Password</label>
                        <input type = "password" name = "password" class = "box" /><br/><br />
				        <div>
					        <input type = "submit" value = "Submit" /><br/> 
				        </div>
                </form>
            </div>
        </div>
	</body>
</html>