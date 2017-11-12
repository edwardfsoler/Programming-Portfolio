<?php
    //retrieve post data and copy to variables
    if($_SERVER["REQUEST_METHOD"] == "POST") {
        $fname = testinput($_POST["fname"]);
        $lname = testinput($_POST["lname"]);
        $company = testinput($_POST["company"]);
        $email = testinput($_POST["email"]);
        $price = testinput($_POST["biddingPrice"]);
        $slot = testinput($_POST["slot"]);
    }

    //function to test that input is safe
    function testinput($data) {
        $data = htmlspecialchars($data);
        return $data;
    }
    //set target directory of file
	$target_dir = "D:/home/site/wwwroot/uploads".$slot."/";
    //set directory of file
	$target_file = $target_dir . basename($_FILES["adFile"]["name"]);
	$uploadOk = 1; 

	// Check if file already exists
	if (file_exists($target_file)) {
		echo "<h3 class='notice'>This file has already been uploaded</h3>";
		$uploadOk = 0;
	}

    //test for correct mp3 file type
    if($_FILES["adFile"]["type"] != "audio/mpeg" && $_FILES["adFile"]["type"] != "audio/x-mp3" && $_FILES["adFile"]["type"] != "audio/mpg"
     && $_FILES["adFile"]["type"] != "audio/x-mpeg" && $_FILES["adFile"]["type"] != "audio/mpeg3" && $_FILES["adFile"]["type"] != "audio/x-mpg"
      && $_FILES["adFile"]["type"] != "audio/mp3" && $_FILES["adFile"]["type"] != "audio/x-mpeg3" && $_FILES["adFile"]["type"] != "audio/x-mpegaudio")
    {
        $uploadOk = 0;
        echo "<h3 class='notice'>Incorrect file type</h3>";
    }

	// Check file size
	if ($_FILES["adFile"]["size"] > 500000000) {
		echo "<h3 class='notice'>Sorry, your file is too large</h3>";
		$uploadOk = 0;
	}
    
	// Check if $uploadOk is set to 0 by an error
	if ($uploadOk == 0) {
		error_log("File not uploaded");
	// if everything is ok, try to upload file
	} else {
        //try to move file to directory
		if (!move_uploaded_file($_FILES["adFile"]["tmp_name"], $target_file)) {
			echo "<h3 class='notice'>There was an error uploading your file</h3>";
         //if move succeeds, enter  
		} else {
            echo "<h3 class='notice'>The file '". basename($_FILES["adFile"]["name"]). "' has been uploaded</h3>";
            //connect to database
			$localhost = "eu-cdbr-azure-north-e.cloudapp.net";
            $username = "bf002e968c272d";
            $password = "7abde923";
            $schemaName = "GAPT";

            $con = mysqli_connect($localhost, $username, $password) or die("I couldn't connect to your database, please make sure your info is correct!");

            mysqli_select_db($con, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");
            
            //add new bid to database
            $add = mysqli_query($con,"INSERT INTO bidders(firstname, lastname, email, company, biddingPrice, advertFile, slot) VALUES('".$fname."','".$lname."','".$email."','".$company."',$price,'".$target_file."',$slot);");
         
            if($add){
                //if bid successfully added, then send email to bidder
                $url = 'https://api.sendgrid.com/';

                $params = array(
                    'api_user'  => 'azure_aecf75e6f7fff2f44b43c3263b79ab2b@azure.com',
                    'api_key'   => 'AIFORTHEWIN0',
                    'to'        => $email,
                    'subject'   => 'Bid Submission',
                    'html'      => "Dear Mr/Ms $lname,<br/>
                                    <br/>
                                    The following bid has successfuly been submitted to GAPT Supermarkets:<br/>
                                    <br/>
                                    Company : $company<br/>
                                    Slot: $slot<br/>
                                    Price: €$price<br/>          
                                    <br/>
                                    You shall be notified when bidding has come to an end.<br/>
                                    <br/>
                                    Regards, <br/>
                                    GAPT Team",
                    'text'      => '',
                    'from'      => 'aigroupapt@gmail.com',
                  );

                $url2 = 'https://api.sendgrid.com/api/mail.send.json';

                $options = array(
                    'http' => array(
                        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
                        'method'  => 'POST',
                        'content' => http_build_query($params),
                    ),
                );
                $context  = stream_context_create($options);
                $result = file_get_contents($url2, false, $context);
		    }		
        }
    }
?>
<!DOCTYPE>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="redirect.css" />
        <!--import jQuery script-->
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script>
        <script type="text/javascript">
            $(document).ready(function () {
                setTimeout(function () {
                    window.location = "index.html";
                }, 3000);
            });
        </script>
    </head>
</html>