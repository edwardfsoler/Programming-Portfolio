<?php
    //start PHP session
    session_start();
    //get post data and store in variables
    if($_SERVER["REQUEST_METHOD"] == "POST") {
        $category = testinput($_POST["category"]);
        $name = testinput($_POST["name"]);
        $link = testinput($_POST["link"]);
    }

    //test that post data is secure
    function testinput($data) {
        $data = htmlspecialchars($data);
        return $data;
    }

    //connect to database
    $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bddef64b53f7a1";
    $password = "9d65b26f";
    $schemaName = "acsm_07f2d8d959cb2ba";

    $con = mysqli_connect($localhost, $username, $password, $schemaName) or die("I couldn't connect to your database, please make sure your info is correct!");

    mysqli_select_db($con, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");
    
    //get id of current link to be added
    $sql = "SELECT id FROM rsslinks WHERE link='".$link."'";
         
    $result = $con->query($sql);

    //if current feed id exists, then retrieve its user
    if($result->num_rows == 1)
    {
        $row = $result->fetch_assoc();
        $id = $row["id"];
    }
    //otherwise create a new feed entry
    else if($result->num_rows == 0)
    {
        //insert new link into links table
        $sql = "INSERT INTO rsslinks(link) VALUES('".$link."')";
              
        if($con->query($sql) != TRUE)
        {
            echo "ERROR Inserting new link into rsslinks table";
        }

        //get id of newly inserted entry
        $sql = "SELECT id FROM rsslinks WHERE link='".$link."'";
        $result = $con->query($sql);

        if($result->num_rows == 1)
        {
            $row = $result->fetch_assoc();
            $id = $row["id"];
        }
        else
        {
            echo "Error: ~";
        }
    }
    else
    {
        echo "Error: ~";
    }

    //make call to get id of user that is currently logged in
    $sql = "SELECT id FROM RSSLogin WHERE email ='".$_SESSION['login_user']."'";
    $result = $con->query($sql);

    if($result->num_rows == 1)
    {
        $row = $result->fetch_assoc();
        $userId = $row["id"];
              
        //set priority of 550
        $priority = 550;
        //insert new record into rsslinksconnect, connectin rss feed to current user
        $sql = "INSERT INTO rsslinksconnect VALUES(".$id.",".$userId.",".$category.",".$priority.",'".$name."')";
        $result = $con->query($sql);

        if($result != TRUE)
        {
            echo "Error: Unable to add RSS Feed to category";
        }
    }
    else
    {
        echo "Something went wrong";
        return;
    }
?>