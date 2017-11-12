<?php
    //connect to database
    $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bddef64b53f7a1";
    $password = "9d65b26f";
    $schemaName = "acsm_07f2d8d959cb2ba";

    //test whether  page works following fixing of database
    $conn = new mysqli($localhost, $username, $password, $schemaName);

    if ($conn->connect_error) {
        echo "error";
        die("Connection failed: " . $conn->connect_error);
    } 
    
    //get each userId, feedId pair from datababse
    $sql = "SELECT id, userId FROM rsslinksconnect";
    $result2 = $conn->query($sql);

    while($row = $result2->fetch_assoc())
    {
        $linkId = $row["id"];
        $id = $row["userId"];
        //retrieve priority of current feed for current user
        $sql = "SELECT priority FROM rsslinksconnect WHERE userId=".$id." AND id=".$linkId;
        $result = $conn->query($sql);
        
        if($result->num_rows != 1)
        {
            echo "Something went wrong";
        }
        else
        {
            $row2 = $result->fetch_assoc();
            $priority = $row2["priority"];

            //if priority is less than 500, decrement by 1%
            if($priority < 500)
            {
                $priority = round($priority * 0.99);
            }
            //if priority is greater than or equal to 500, decrement by 2%
            else
            {
                $priority = round($priority * 0.98);
            }

            //update feed priority
            $sql = "UPDATE rsslinksconnect SET priority=".$priority." WHERE id=".$linkId." AND userId=".$id;  

            $result = $conn->query($sql);

            if($result != TRUE)
            {
                echo "Something went wrong";
            }
        }
    }
?>