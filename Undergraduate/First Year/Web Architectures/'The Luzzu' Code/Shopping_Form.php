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
        <title>Shopping Cart</title>

        <style>
            
            label{
                display: inline-block;
                float: left;
                clear: left;
                width: 150px;
                text-align: left;
            }
            
            input {
              display: inline-block;
            }
            
            input[type="number"] {
               width:50px;
            }
            
        </style>

        <?php
            
        $servername = "localhost";
        $username = "root";
        $password = "qwerty1996";
        $dbname = "food_items";

        $conn = new mysqli($servername, $username, $password, $dbname);

        if($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }
            
                                                
            $name = $surname = $telephone = $date = $hour = $minute = $add1 = $add2 = "";  
            
            function testinput($data) {
                $data = trim($data);
                $data = stripslashes($data);
                $data = htmlspecialchars($data);
                return $data;
            }   

            if($_SERVER["REQUEST_METHOD"] == "POST") {
                
                $name = testinput($_POST["name"]);
                $surname = testinput($_POST["surname"]);
                $telephone = testinput($_POST["telephone"]);
                $date = testinput($_POST["date"]);
                $hour = testinput($_POST["hour"]);
                $minute = testinput($_POST["minute"]);
                $add1 = testinput($_POST["add1"]);
                $add2 = testinput($_POST["add2"]);
            
            
            $address = $add1 . " " . $add2;
            $time = $hour . ":" . $second;

            
                $order = "";

                for($i=0; $i < count($_SESSION["cart"]); $i += 2) {
                $sql = "SELECT name FROM Menu WHERE id=".substr($_SESSION["cart"][$i],1);
                $result = $conn->query($sql);
                $row = $result->fetch_assoc(); 
                $order = $order.substr($_SESSION["cart"][$i],1).")     ".$row["name"]."    X    ".substr($_SESSION["cart"][$i+1],1)."\n"; 
                }

            $to = "example@example.com";
            $sub = "Order on " . $date . " at " . $time;
            $message = "        ----- Order -----\n
                        Name: " . $name . "\n
                        Surname: " . $surname . "\n
                        Telephone: " . $telephone . "\n
                        Address: " . $address . "\n
                        Date: " . $date . "\n
                        Time: " . $time . "\n
                        Order: \n\n".$order;
                                                
            $from = "webmaster@reservations.com";

            if(mail($to,$sub,$message, $from)) {
              //change once mail actually works------------------------------------\/   
            }
            else {
                session_unset();
                session_destroy();
                header('location:http://localhost:50420/Shopping_Cart.php?msg=success');
            }
            }
    }

        ?>
        

    </head>
    <body>

        <form method="post" id="order" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"])?>" onsubmit="return confirm('Confirm Order?')">
            <label>Name *</label><input type="text" name="name" value="<?php echo $name;?>" required/><br/><br/>
            <label>Surname *</label><input type="text" name="surname" value="<?php echo $surname;?>" required/><br><br/>
            <label>Address *</label><br/>
            <label style="white-space: pre">        Line 1</label><input type="text" name="add1" value="<?php echo $add1;?>" required/><br/>
            <label style="white-space: pre">        Line 2</label><input type="text" name="add2" value="<?php echo $add1;?>" required/><br/><br/>
            <label>Mobile/Telephone *</label><input type="tel" name="telephone" value="<?php echo $telephone;?>" required/><br/><br/>
            <label>Date *</label><input type="date" name="date" value="<?php echo $date?>" required/><br/><br/>
            <label >Time *</label>
            <input type="number" name="hour" value="12" required min="12" max="23">
            <h5 style="display: inline">:</h5><input type="number" name="minute" value="0" required min="0" max="59"><br/><br/>
            <input type="submit" value="Place order" onsubmit="return isValidForm()"/>

        </form>
                
    </body>
</html>
