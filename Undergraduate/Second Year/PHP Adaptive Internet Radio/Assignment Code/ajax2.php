<?php
//if post value is set then enter
if (isset($_POST['action'])) {
    //connect to database
    $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bf002e968c272d";
    $password = "7abde923";
    $schemaName = "GAPT";

    //store copy of post value
    $i = $_POST['action'];

    $con = mysqli_connect($localhost, $username, $password) or die("I couldn't connect to your database, please make sure your info is correct!");

    mysqli_select_db($con, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");

    //select all rows from bidders table with id i
    $sql = "SELECT * FROM bidders WHERE id=".$i;
    $result = $con->query($sql);

    //if number of rows is 1, enter, otherwise error
    if ($result->num_rows == 1) {
        $row = $result->fetch_assoc();
        //store copy of row values
        $email = $row["email"];
        $fname = $row["firstname"];
        $lname = $row["lastname"];

        //send email to bidder stating that their bid was declined
        $url = 'https://api.sendgrid.com/';

            $params = array(
                'api_user'  => 'azure_aecf75e6f7fff2f44b43c3263b79ab2b@azure.com',
                'api_key'   => 'AIFORTHEWIN0',
                'to'        => $email,
                'subject'   => 'Bid Declined',
                'html'      => "Dear Mr/Ms $lname,<br/>
                                <br/>
                                Unfortunately, your bid has been rejected.</br>
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
        
        //set accept value for current bid to 2
        $sql = "UPDATE bidders SET accepted=2 WHERE id=".$i;
        $result = $con->query($sql);

        if($result != TRUE)
        {
            error_log("Change to value 2 failed");
        }
    }
}
else
{
    echo "Error: No post value set";
}
?>