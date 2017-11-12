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

    $sql = "CREATE TABLE Menu (
            id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            course INT(1) NOT NULL,
            description VARCHAR(1000) NOT NULL,
            image VARCHAR(255),
            price DECIMAL(6,2) NOT NULL,
            reg_date TIMESTAMP
            )";

if ($conn->query($sql) === TRUE) {
    echo "Success";
} else {
    echo "Error creating table";
}

    $conn->close();

?>

</body>

</html>