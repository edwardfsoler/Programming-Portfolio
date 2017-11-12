<?php
    session_start(); //start user session
?>

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

    $sql = "SELECT id, name, course, description, image, price FROM Menu";
    $result = $conn->query($sql);

    $checked = FALSE;

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {

        $id = $row["id"];
        $image = $row["image"];
        $name = $row["name"];
        $description = $row["description"];
        $course = $row["course"];
        $price = $row["price"];

        ?>
        <!--design here-->

        <h2><?php echo $id?></h2>
        <img src="<?php echo $image ?>" alt="<?php echo $name?>" style="width:115px;height:86px">
        <h3 style="display: inline"><?php echo $name?></h3>
        <h5><?php echo $course?></h5>
        <p><?php echo $description?></p>
        <h4 style="color:gray">&euro;<?php echo $price?></h4>

        <form method="post">
        <input type="number" id="quantity" name="quantity" min="1" max="10" value="1"/>
        <input type="hidden" id="id" name="id" value="<?php echo $row["id"]?>"/>
        <input type="submit" value="Add to Cart"/>
        </form>

    <?php
        
        if(!$checked && isset($_POST["quantity"])) {
        $checked = TRUE;
        cart_Create($_POST["id"], $_POST["quantity"]); 
       }
       
    }

}
else {
    echo "0 results";
}

    $conn->close();


    function cart_Create($id, $quantity) {
        
        if(empty($_SESSION["cart"])) {
           
            $cart = array();

            array_push($cart,"i".$id,"q".$quantity);

            $_SESSION["cart"] = $cart;
        }
        else {

            $pos = array_search("i".$id,$_SESSION["cart"]);
            

            if($pos !== FALSE) {

                $_SESSION["cart"][$pos+1] = "q".$quantity; //if item already in cart override quantity                
            }
            else {
                array_push($_SESSION["cart"],"i".$id,"q".$quantity);
            }
        }
    }

?>
    
    
</body>

</html>