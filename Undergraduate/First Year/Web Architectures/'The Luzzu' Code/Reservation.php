<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8" />
        <title>Reservation</title>

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
            
            $name = $surname = $telephone = $email = $number = $date = $time = $comments = "";     

            if($_SERVER["REQUEST_METHOD"] == "POST") {
                
                echo "<script type='text/javascript'>alert('Your reservation has been processed');</script>";
                                
                $name = testinput($_POST["name"]);
                $surname = testinput($_POST["surname"]);
                $telephone = testinput($_POST["telephone"]);
                $email = testinput($_POST["email"]);
                $number = testinput($_POST["number"]);
                $date = testinput($_POST["date"]);
                $time = testinput($_POST["time"]);
                $comments = testinput($_POST["comments"]);
            }

            function testinput($data) {
                $data = trim($data);
                $data = stripslashes($data);
                $data = htmlspecialchars($data);
                return $data;
            }

            $to = "example@example.com";
            $subject = "Reservation for " .$number." at ".$time;
            $message = "        ----- RESERVATION -----\n
                        Name: " . $name . "\n" .
                       "Surname: " . $surname . "\n" .
                       "Telephone: " . $telephone . "\n" .
                       "Email: " . $email . "\n".
                       "Seating: " . $number . "\n".
                       "Date: " . $date . "\n".
                       "Time: " . $time . "\n".
                       "Comments: " . $comments;

            $from = "webmaster@reservations.com";

            mail($to,$subject,$message, $from);

            
        ?>

    </head>
    <body>
        <form method="post" id="reservation" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"])?>" onsubmit="return confirm('Confirm reservation?')">
            <label>Name *</label><input type="text" name="name" value="<?php echo $name;?>" required/><br/><br/>
            <label>Surname *</label><input type="text" name="surname" value="<?php echo $surname;?>" required/><br><br/>
            <label>Mobile/Telephone *</label><input type="tel" name="telephone" value="<?php echo $telephone;?>" required/><br/><br/>
            <label>Email </label><input type="email" name="email" value="<?php echo $email;?>"/><br/><br/>
            <label>Table for: *</label>
            <input type="range" oninput="outFunc(this.value)"name="number" value="4" min="1" max="20" step="1" required>
            <span id="rangeval">4</span><br/><br/>
            <label>Date *</label><input type="date" name="date" value="<?php echo $time?>" required/><br/><br/>
            <label >Time *</label>
            <input type="number" name="hour" value="12" required min="12" max="23">
            <h5 style="display: inline">:</h5><input type="number" name="minute" value="0" required min="0" max="59"><br/><br/>
            <label>Comments </label><textarea name="comments"></textarea></textarea><br/><br/>
            <input type="submit" value="Book!" onsubmit="return isValidForm()"/>
           
        </form>

         <script type="text/javascript">
            function outFunc(myValue){
              document.getElementById("rangeval").innerHTML = myValue;
            }
        </script>
        
    </body>
</html>
