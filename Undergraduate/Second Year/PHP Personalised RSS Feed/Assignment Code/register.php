<?php
    $fname = $lname = $email = $pass = "";

    //retrieve post data and store in variables
    if($_SERVER["REQUEST_METHOD"] == "POST") {
        $fname = testinput($_POST["fname"]);
        $lname = testinput($_POST["lname"]);
        $email = testinput($_POST["email"]);
        $pass = testinput($_POST["passw"]);
    }
    //test validity of post data
    function testinput($data) {
        $data = htmlspecialchars($data);
        return $data;
    }
    //start PHP session
    session_start();
    //set user session
    $_SESSION['login_user'] = $email;

    //connect to database
    $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bddef64b53f7a1";
    $password = "9d65b26f";
    $schemaName = "acsm_07f2d8d959cb2ba";

    $conn = mysqli_connect($localhost, $username, $password, $schemaName) or die("I couldn't connect to your database, please make sure your info is correct!");

    mysqli_select_db($conn, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");

    //create new user entry in database
    $sql = "INSERT INTO rsslogin(email, password, name, surname) VALUES('".$email."','".$pass."','".$fname."','".$lname."')";
    $result = $conn->query($sql);

    if($result != TRUE)
    {
        echo "Something went wrong";
    }
    else
    {
        //get user id
        $sql = "SELECT id FROM rsslogin WHERE email = '".$_SESSION["login_user"]."'";
        $result = $conn->query($sql);

            if($result->num_rows == 1)
            {
                $row = $result->fetch_assoc();
                $id = $row["id"];
            }
            else
            {
                echo "Something went wrong logging in";
                return;
            }

        //initialise all default articles with respective priority for user
        $sql = "INSERT INTO rsslinksconnect VALUES(1, $id, 0, 500, 'Times of Malta')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(2, $id, 0, 500, 'Independent')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(3, $id, 0, 500, 'Malta Today')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(4, $id, 1, 500, 'Reuters')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(5, $id, 1, 500, 'Aljazeera')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(6, $id, 1, 500, 'BBC')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(7, $id, 4, 500, 'New Scientist')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(8, $id, 4, 500, 'ACM')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(9, $id, 3, 500, 'CNBC')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(10, $id, 3, 500, 'Financial Times')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(11, $id, 2, 500, 'Eurosport')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(12, $id, 2, 500, 'Auto Sport')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(13, $id, 5, 500, 'Fox News')";
        $result = $conn->query($sql);

        $sql = "INSERT INTO rsslinksconnect VALUES(14, $id, 5, 500, 'Huffington Post')";
        $result = $conn->query($sql);
    }
 ?>