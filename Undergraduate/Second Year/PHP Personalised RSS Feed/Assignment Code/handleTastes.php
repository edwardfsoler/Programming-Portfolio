<?php
    //start PHP session
    session_start();

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
    $email = $_SESSION["login_user"];

    //get id of current user
    $sql = "SELECT id FROM RSSLogin WHERE email ='".$email."'";
    $result = $conn->query($sql);

    if($result->num_rows == 1)
    {
        $row = $result->fetch_assoc();
        $id = $row["id"];
    }
    else
    {
        echo "Something went wrong: You are not logged in";
        return;
    }
    //get post data
    if($_SERVER["REQUEST_METHOD"] == "POST") {
        
        if(isset($_POST["lnews"]) && testinput($_POST["lnews"]) == "lnews")
        { 
            //update priority of links found to be liked by the user
            $sql = "UPDATE rsslinksconnect SET priority = priority + 50 WHERE userid = $id AND category = 0";
            $result = $conn->query($sql);

            if($result != TRUE)
            {
                echo "Something went wrong here";
            }
        }
        //repeat for all possible categories
        if(isset($_POST["inews"]) && testinput($_POST["inews"]) == "inews")
        {
              $sql = "UPDATE rsslinksconnect SET priority = priority + 50 WHERE userid = $id AND category = 1";
              $result = $conn->query($sql);

              if($result != TRUE)
              {
                  echo "Something went wrong here";
              }
        }

        if(isset($_POST["sports"]) && testinput($_POST["sports"]) == "sports")
        {
            $sql = "UPDATE rsslinksconnect SET priority = priority + 50 WHERE userid = $id AND category = 2";
              $result = $conn->query($sql);

              if($result != TRUE)
              {
                  echo "Something went wrong here";
              }
        }

        if(isset($_POST["finance"]) && testinput($_POST["finance"]) == "finance")
        {
            $sql = "UPDATE rsslinksconnect SET priority = priority + 50 WHERE userid = $id AND category = 3";
              $result = $conn->query($sql);

              if($result != TRUE)
              {
                  echo "Something went wrong here";
              }
        }

        if(isset($_POST["tech"]) && testinput($_POST["tech"]) == "tech")
        {
            $sql = "UPDATE rsslinksconnect SET priority = priority + 50 WHERE userid = $id AND category = 4";
              $result = $conn->query($sql);

              if($result != TRUE)
              {
                  echo "Something went wrong here";
              }
        }

        if(isset($_POST["politics"]) && testinput($_POST["politics"]) == "politics")
        {
            $sql = "UPDATE rsslinksconnect SET priority = priority + 50 WHERE userid = $id AND category = 5";
              $result = $conn->query($sql);

              if($result != TRUE)
              {
                  echo "Something went wrong here";
              }
        }
    }
    //redirect the user
    header("location: rssloader.php");
         
    //test the POST data for validity
    function testinput($data) {
        $data = htmlspecialchars($data);
        return $data;
    }
?>