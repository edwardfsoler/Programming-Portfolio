<?php
    //start PHP session
    session_start();
    if (isset($_POST['deleteLink']) && !empty($_POST['deleteLink'])) {

        function testinput($data) {
            $data = htmlspecialchars($data);
            return $data;
        }
        //get post data
        if($_SERVER["REQUEST_METHOD"] == "POST") {
            $dname = testinput($_POST["deleteLink"]);
        }
        //connect to database
        $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
        $username = "bddef64b53f7a1";
        $password = "9d65b26f";
        $schemaName = "acsm_07f2d8d959cb2ba";

        $conn = mysqli_connect($localhost, $username, $password, $schemaName) or die("I couldn't connect to your database, please make sure your info is correct!");

        mysqli_select_db($conn, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");

        //get id of current user
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
        //get id of current link under current user
        $sql = "SELECT id FROM rsslinksconnect WHERE userId = ".$id." AND title = '".$dname."'";
        $result = $conn->query($sql);
        if($result->num_rows == 1)
        {
            $row = $result->fetch_assoc();
            $linkid = $row["id"];
        }
        else
        {
            echo "Something went wrong fetching link";
            return;
        }

        //delete current link
        $sql = "DELETE FROM rsslinksconnect WHERE userId = ".$id." AND id = ".$linkid;
        $result = $conn->query($sql);

        if($result != TRUE)
        {
            echo "Something went wrong deleting from rsslinksconnect";
            return;
        }

        //test if feed link is unique to current user
        $sql = "SELECT * FROM rsslinksconnect WHERE id = ".$linkid;
        $result = $conn->query($sql);

        //if so delete, otherwise leave it
        if($result->num_rows == 0)
        {
            $sql = "DELETE FROM rsslinks WHERE id = ".$linkid;
            $result = $conn->query($sql);

            if($result != TRUE)
            {
                echo "Something went wrong testing for delete from rsslinks";
            }
        }
    }
    //redirect to rssloader.php
    header('Location: rssloader.php');
?>