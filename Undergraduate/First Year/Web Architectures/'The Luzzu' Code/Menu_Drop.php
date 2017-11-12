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

    $conn = new mysqli($servername, $username, $password, $dbname);

    if($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    $sql = "DROP TABLE Menu";

if ($conn->query($sql) === TRUE) {
    echo "Success";
} else {
    echo "Error dropping table";
}

    $conn->close();

?>

</body>

</html>