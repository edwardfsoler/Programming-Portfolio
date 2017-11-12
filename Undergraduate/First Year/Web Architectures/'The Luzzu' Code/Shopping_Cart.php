<?php
    session_start();
    if(!isset($_SESSION["cart"]) && ($_GET["msg"] == "success")) {
        echo "Thank you for ordering from Luzzu Malta";
    }
    elseif(!isset($_SESSION["cart"]) && !($_GET["msg"] == "success")) {
       
        echo "Shopping Cart is empty";
    }
    else {
?>

<!DOCTYPE html>
<html>
<head>

<title>Shopping Cart</title>


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

    $cart = $_SESSION["cart"];
    $total_price = 0;

    if(count($cart) == 0) {
        echo "Shopping cart is empty";
    }
    else {

        for($i=0; $i<count($cart); $i += 2) {
    
            $temp_id = substr($cart[$i],1); //ignore 'i'
    
            $sql = "SELECT * FROM Menu WHERE id=".$temp_id;   
    
            $result = $conn->query($sql);

            if ($result->num_rows > 0) {

                $row= $result->fetch_assoc();
                //design here
                echo '<h2>' . $row["id"] . '</h2>';
                echo '<a href="http://localhost:50420/Shopping_Cart-Drop_Item.php?id='.$row["id"].'"><img src="https://cdn2.iconfinder.com/data/icons/ui-kit/100/icon_error-512.png" alt="remove item" width="20px" height="20px"></img></a>';
                echo '<img src="' . $row["image"] . '" alt="' . $row["name"] . '" style="width:115px;height:86px">';
                echo '<h3 style="display: inline">' . $row["name"] . '</h3>';
                echo '<h5>' . $row["course"] . '</h5>';
                echo '<p>' . $row["description"] . '</p>';
                echo '<h4 color="gray">&euro;' . $row["price"] . '</h4>';
                echo '<h4>' . substr($cart[$i+1],1) . '</h4>';
                echo '<h5>' . $row["order_time"] . '</h5>';

                $total_price += ($row["price"] * substr($cart[$i+1],1)); //price * quantity

            }
        
    
        }
        echo "Total price: &euro;" . $total_price;
        ?>
            <a href="http://localhost:50420/Shopping_Form.php"><img src="http://www.evucc.org/images/stories/check-it-out.png" alt="Check Out!"width="50px" height="35px"/></a>
    <?php
    }

    $conn->close();
    
    }//end if-else
           
        ?>
    
</body>

</html>