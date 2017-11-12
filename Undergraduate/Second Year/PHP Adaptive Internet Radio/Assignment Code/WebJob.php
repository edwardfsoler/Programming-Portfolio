<?php
    //WebJob script to be remotely run by schedular each morning at 3:30 am    

    //connect to database
    $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bf002e968c272d";
    $password = "7abde923";
    $schemaName = "GAPT";

    $con = mysqli_connect($localhost, $username, $password) or die("I couldn't connect to your database, please make sure your info is correct!");

    mysqli_select_db($con, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");

    //select all elements in the table bidders
    $sql = "SELECT * FROM bidders where accepted=1";
    $result = $con->query($sql);

    //create text file called 'bids.txt'
    $name = "bids.txt";
    //open file in directory 'BidLogs'
    $file = fopen("D:/home/site/wwwroot/BidLogs/".$name, "w");

    //loop through each row in the database
    while($row = $result->fetch_assoc())
    {
        //write each row to files
        $log = $row["advertFile"];
        fwrite($file, $log."\n");
    }
    //close file
    fclose($file);

    //truncate database of all records
    $sql = "TRUNCATE TABLE bidders";
    $result = $con->query($sql);

    if($result != TRUE)
    {
        echo "Error Truncating Database";
    }
    else
    {
        echo "Truncate Succeeded";
    }

    //Delete all files from uploads1
    $files = glob('D:/home/site/wwwroot/uploads1/*'); // get all file names
    foreach($files as $file){ // iterate files
      if(is_file($file))
        unlink($file); // delete file
    }

    //Delete all files from uploads2
    $files = glob('D:/home/site/wwwroot/uploads2/*'); // get all file names
    foreach($files as $file){ // iterate files
      if(is_file($file))
        unlink($file); // delete file
    }

    //Delete all files from uploads3
    $files = glob('D:/home/site/wwwroot/uploads3/*'); // get all file names
    foreach($files as $file){ // iterate files
      if(is_file($file))
        unlink($file); // delete file
    }

    //select all bids which have neighter been accepted nor declined
    $sql = "SELECT * FROM bidders WHERE accepted=0";

    $result = $con->query($sql);

    while($row = $result->fetch_assoc())
    {
        $email = $row["email"];
        $lname = $row["lastname"];

        //send email to user stating that their advert was outbid
        $params = array(
            //fill in form details for email to be sent
            'api_user'  => 'azure_aecf75e6f7fff2f44b43c3263b79ab2b@azure.com',
            'api_key'   => 'AIFORTHEWIN0',
            'to'        => $email,
            'subject'   => 'Bid Accepted',
            'html'      => "Dear Mr/Ms $lname,<br/>
                            <br/>
                            Unfortunately your advert has been outbid and therefore was not selected.<br/>
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
?>