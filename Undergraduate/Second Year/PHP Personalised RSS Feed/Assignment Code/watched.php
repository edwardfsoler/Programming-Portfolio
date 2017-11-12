<?php
    //start PHP session
    session_start();
    //get post data
    if($_SERVER["REQUEST_METHOD"] == "POST") {
        $unique = testinput($_POST["unique"]);
        $feedIndex = testinput($_POST["feedIndex"]);
        $title = testinput($_POST["title"]);
    }

    //test validity of input
    function testinput($data) {
        $data = htmlspecialchars($data);
        return $data;
    }
    //store copy of index
    $ind = $_SESSION['index'];
    //wrap index round if at end of array
    if($_SESSION['index'] == 19)
    {
        $_SESSION['index'] = 0;
    }
    else
    {
        //increment index otherwise
        $_SESSION['index'] = $_SESSION['index'] + 1;
    }

    //add title of article to watched session array
    $_SESSION['watched'][$ind] = $title;

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

    //get id of current user
    $sql = "SELECT id FROM rsslogin WHERE email = '".$_SESSION['login_user']."'";
    $result = $conn->query($sql);

    if($result->num_rows != 1)
    {
        echo "Error Accessing database";
    }
    else
    {
        $row = $result->fetch_assoc();
        $userId = $row['id'];
    }

    //update priority of feed from which current article came by 15
    $sql = "UPDATE rsslinksconnect SET priority = priority + 15 WHERE userId =".$userId." AND id=".$feedIndex;
    $result = $conn->query($sql);

    if($result != TRUE)
    {
        echo "Error incrementing priority";
    }
?>