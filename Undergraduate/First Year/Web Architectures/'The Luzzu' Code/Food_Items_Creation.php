<!DOCTYPE html>
<html>
<head>

<title>Database Creation</title>
    
</head>
<body>
    <?php
    $servername = "localhost";
    $username = "root";
    $password = "qwerty1996";
    $dbname = "food_items";

    $conn = new mysqli($servername, $username,$password);

    if($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    $sql = "CREATE DATABASE " . $dbname;

if ($conn->query($sql) === TRUE) {
    echo "Success";
} else {
    echo "Error creating database";
}

    $conn->close();

?>

</body>

</html>