<?php
    session_start();
    if(!isset($_SESSION["cart"])) {
       
        echo "Shopping Cart is empty";
    }
    else {
?> 

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8" />
        <title>Shopping Cart Drop Item</title>

        
        <?php
            
        if(empty($_GET["id"])) {
            echo "Error - nothing to remove";
        }
        else {
            $temp_id = $_GET["id"];
            $temp_search = array_search("i".$temp_id, $_SESSION["cart"]);
            $temp_length = count($_SESSION["cart"]);
            
            if($temp_search === FALSE) {
                echo "Item ".$temp_id." is not in cart";
            }
            else {
                
                array_splice($_SESSION["cart"],$temp_search,2); //deletes id and quantity of item
                
                if(count($_SESSION["cart"]) == $temp_length-2) {
                    echo "Item deleted";
                }
                else {
                    echo "Failed to delete item";
                }
            }
        }
            
    } //end if-else
        ?>
        

    </head>
    <body>
        
    </body>
</html>
