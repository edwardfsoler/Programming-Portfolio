<?php
    //start PHP session
    session_start();
        
    //store post value 'action' as i
    $i = $_POST['action'];
    //connect to database
    $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bf002e968c272d";
    $password = "7abde923";
    $schemaName = "GAPT";

    $con = mysqli_connect($localhost, $username, $password) or die("I couldn't connect to your database, please make sure your info is correct!");

    mysqli_select_db($con, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");
    
    //select all rows with id = i
    $sql = "SELECT * FROM bidders WHERE id=".$i;
    $result = $con->query($sql);
    //if one row is found then enter, otherwise error
    if ($result->num_rows == 1) {
            
        //get current row
        $row = $result->fetch_assoc();

        //store copy of values
        $name = $row["advertFile"];
        $email = $row["email"];
        $fname = $row["firstname"];
        $lname = $row["lastname"];
        $company = $row["company"];
        $slot = $row["slot"];
        $price = $row["biddingprice"];

        //Find position of '/' before song name
        $firstOcc = stripos($name,'/uploads')+9;

        //test for slot value to add file to
        if($slot == 1)
        {
            //if session count1 is not set then initialise to 1
            if(!isset($_SESSION["count1"]))
            {
                $_SESSION["count1"] = 1;
            }
            //otherwise increment
            else
            {
                $_SESSION["count1"] += 1;
            }
            //if more than 5 ads have been 
            if($_SESSION["count1"] > 5)
            {
                echo "Something went wrong";
            }
        }
        //repeat same procedure for other slots
        else if($slot == 2)
        {
            if(!isset($_SESSION["count2"]))
            {
                $_SESSION["count2"] = 1;
            }
            else
            {
                $_SESSION["count2"] += 1;
            }

            if($_SESSION["count2"] > 5)
            {
                echo "Something went wrong";
            }
        }
        else if($slot == 3)
        {
            if(!isset($_SESSION["count3"]))
            {
                $_SESSION["count3"] = 1;
            }
            else
            {
                $_SESSION["count3"] += 1;
            }

            if($_SESSION["count3"] > 5)
            {
                echo "Something went wrong";
            }
        }

        //set current bid to mark as accepted
        $sql = "UPDATE bidders SET accepted=1 WHERE id=".$i;
        $result = $con->query($sql);

        if($result != TRUE)
        {
            error_log("Change to value 1 failed");
        }

        $newDirectory = "D:/home/site/wwwroot/Slot".$slot.substr($name,$firstOcc);
           
        //move the advert file from uploads*slotNo* to Slot*slotNo*
        if(!rename($name, $newDirectory))
        {
            echo "Error moving file";
            exit;
        }
        else
        {
            $sql="UPDATE bidders SET advertFile = '".$newDirectory."' WHERE id=".$i;
            $result = $con->query($sql);

            if($result != TRUE)
            {
                echo "Error renaming directory in database";
            }
        } 

        //send email to user stating that their advert was accepted
        $params = array(
            //fill in form details for email to be sent
            'api_user'  => 'azure_aecf75e6f7fff2f44b43c3263b79ab2b@azure.com',
            'api_key'   => 'AIFORTHEWIN0',
            'to'        => $email,
            'subject'   => 'Bid Accepted',
            'html'      => "Dear Mr/Ms $lname,<br/>
                            <br/>
                            Congratulations, your bid has been selected to play at our outlets.<br/>
                            <br/>
                            Company : $company<br/>
                            Slot: $slot<br/>
                            Price: €$price<br/>          
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
        //send out email
        $context  = stream_context_create($options);
        $result = file_get_contents($url2, false, $context);
    }
    else
    {
        echo "Error: multiple records with given id";
    }
?>
