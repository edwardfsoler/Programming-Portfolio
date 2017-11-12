<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8" />
        <title>Query</title>

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
              float: left;
            }
            
        </style>

        <?php
            
            $name = $surname = $telephone = $email = $subject = $query = "";     

            if($_SERVER["REQUEST_METHOD"] == "POST") {
                
                $name = testinput($_POST["name"]);
                $surname = testinput($_POST["surname"]);
                $telephone = testinput($_POST["telephone"]);
                $email = testinput($_POST["email"]);
                $subject = testinput($_POST["subject"]);
                $query = testinput($_POST["query"]);
                
            }

            function testinput($data) {
                $data = trim($data);
                $data = stripslashes($data);
                $data = htmlspecialchars($data);
                return $data;
            }

            $to = "example@example.com";
            $sub = "Query: ". $subject;
            $message = "        ----- Query -----\n
                        Name: " . $name . "\n" .
                       "Surname: " . $surname . "\n" .
                       "Telephone: " . $telephone . "\n" .
                       "Email: " . $email . "\n".
                       $subject . "\n".
                       $query;
                       

            $from = "webmaster@reservations.com";

            mail($to,$sub,$message, $from);
            
            $to = $email;
            $sub = "Query Processed";
            $message = "Dear Mr/Ms " . $surname . "\n
                        The following query was succesfully processed on " . date("w \the j \of F Y") .
                        " at " . date("G\:i") . ":\n
                        Subject: " . $subject . "\n" . 
                        $complaint . "\n\n
                        Your query will be acknowledged by personnel at Luzzu Restaurant Malta at the soonest possible moment,\n
                        however please allow 72 hours to pass before further contacting us on this matter.\n
                        Please feel free to contact us on +356 21459182 or email us at luzzumalta@example.com\n\n";

            $from =    "Regards,\n
                        Joshua Abela,\n
                        PR Manager at Luzzu Restaurant Malta";

            mail($to,$sub,$message, $from);
            
        ?>

    </head>
    <body>
        <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"])?>" onsubmit="return confirm('Proceed?')">
            <label>Name *</label><input type="text" name="name" value="<?php echo $name;?>" required/><br/><br/>
            <label>Surname *</label><input type="text" name="surname" value="<?php echo $surname;?>" required/><br><br/>
            <label>Mobile/Telephone </label><input type="tel" name="telephone" value="<?php echo $telephone;?>"/><br/><br/>
            <label>Email *</label><input type="email" name="email" value="<?php echo $email;?>" required/><br/><br/>
            <label>Subject *</label><input type="text" name="subject" value="<?php echo $subject?>" required/><br/><br/>
            <label>Query *</label><textarea name="query" required></textarea><br/><br/>
            <input type="submit" value="Place complaint" onsubmit="return isValidForm()"/>
           
        </form>
                
    </body>
</html>
