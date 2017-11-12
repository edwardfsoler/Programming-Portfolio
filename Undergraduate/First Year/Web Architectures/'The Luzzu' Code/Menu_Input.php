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

    $sql = "INSERT INTO Menu(name, course, description, image, price)
            VALUES('Pizza Margeritha', 2, 'Crusty pizza dough topped with freshly made tomato salsa and mozzarella',
            'https://encrypted-tbn1.gstatic.com/images?q=tbn:ANd9GcQTxZql6DrIyE_usBWReSIFGNbi6Ua7O2SseMTtvpihECoPEago', 9.99)";

if ($conn->query($sql) === TRUE) {
    echo "Success";
} else {
    echo "Error creating record";
}

    $conn->close();

?>

</body>

</html>