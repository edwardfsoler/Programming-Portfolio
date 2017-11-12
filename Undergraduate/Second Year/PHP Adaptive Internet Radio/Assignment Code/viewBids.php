<?php
    //start PHP session
    session_start();
    /*$_SESSION["count1"] = 0;
    $_SESSION["count2"] = 0;
    $_SESSION["count3"] = 0;*/
 
    //if user is not logged  in then redirect
    if ($_SESSION['loggedin'] != 1) {
        header("Location: login.php");
    }
    //otherwise display page
    else
    {
?>
 <!DOCTYPE html>
 <html>
	<head>
		<title>Bid Submissions</title>
        <!--import jQuery script-->
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script>
        <link rel="stylesheet" type="text/css" href="viewBids.css" />
        <script>
            $(document).ready(function () {
                //store copy of time from localStorage variable
                var d = localStorage.time;
                //get current date-time
                var a = new Date();
                var c = a.toLocaleDateString();
                //if localStorage was not previously set, then set to current time
                if (localStorage.getItem("time") == null) {
                    localStorage.setItem("time", c);
                }
                //if the last time this page has been entered was atleast yesterday, reset variables to 0
                if (d != c) {                               //== must be set to != to work. Was only set to == while testing
                    localStorage.setItem("count1", 0);
                    localStorage.setItem("count2", 0);
                    localStorage.setItem("count3", 0);
                    localStorage.setItem("del", 0);
                    var b = new Date();
                    var h = a.toLocaleDateString();
                    //set last entered time to current time
                    localStorage.setItem("time", h);
                }
                console.log(localStorage.count1);

                //when accept/decline button is clicked, enter
                $('.button').click(function () {
                    //get whether the button clicked was accept or decline
                    var clickBtnValue = $(this).val();
                    //get button id
                    var i = $(this).attr('id');
                    //hide button to avoid double click
                    $(this).attr("disable", "disable");
                    //get slot number of bid clicked on through the button id
                    var slotNo = $('.button').attr('class').split(' ')[1];

                    //if the button clicked was the accept, then enter
                    if (clickBtnValue == "accept") {
                        //fade out div once accepted
                        $("#div" + i).fadeOut(1000);
                        var ajaxurl = 'ajax1.php';
                        //call ajax1.php script with respective parameters according to slot number and bid id
                        if (slotNo == 1) {
                            if (localStorage.count1 < 5) {
                                //update counter of local storage for current slot
                                localStorage.setItem("count1", parseInt(localStorage.count1) + 1);
                                $.post(ajaxurl, { action: i }, function (response) {
                                });
                            }
                        }
                        else if (slotNo == 2) {
                            if (localStorage.count2 < 5) {
                                localStorage.setItem("count2", parseInt(localStorage.count2) + 1);
                                $.post(ajaxurl, { action: i }, function (response) {
                                });
                            }
                        }
                        else if (slotNo == 3) {
                            if (localStorage.count3 < 5) {
                                localStorage.setItem("count3", parseInt(localStorage.count3) + 1);
                                $.post(ajaxurl, { action: i }, function (response) {
                                });
                            }
                        }
                        //invalid slot number
                        else {
                            console.log("Error");
                        }
                    }
                    //button clicked was decline
                    else {
                        //fade out div
                        $("#div" + i).fadeOut(1000);
                        //call ajax2.php script passing the id of the bid being declined
                        var ajaxurl = 'ajax2.php';
                        $.post(ajaxurl, { action: i }, function (response) {
                        });
                        $('.' + slotNo + (parseInt(localStorage.del) + 2)).fadeIn(1000);
                        localStorage.setItem("del", parseInt(localStorage.del) + 1);
                    }
                });
            });
            //function to log user out of account on button click
            function logout() {
                //call php script to change session value
                var ajaxurl = 'logout.php';
                $.post(ajaxurl, function (response) {
                    //redirect user to home page
                    window.location = 'index.html';
                });
            }
        </script>
	</head>
	<body>
			<div id="header">
        <button name="logout" id="logout" onclick="logout()">Log Out</button><br/><br/>
        <h1 id="pageTitle">Current Bids</h1>
                <!--Log out Button-->
			</div>
        <?php
        //connect to database
        $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
        $username = "bf002e968c272d";
        $password = "7abde923";
        $schemaName = "GAPT";

        $con = mysqli_connect($localhost, $username, $password) or die("I couldn't connect to your database, please make sure your info is correct!");

        mysqli_select_db($con, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");

        //loop through each slot number
        for($slotNo = 1; $slotNo < 4; $slotNo++)
        {
            //select all bid records which have not been accepted/declined and are in the current slot. Order records by descending bidding price
            $sql = "SELECT * FROM bidders WHERE slot = ".$slotNo." AND accepted=0 ORDER BY biddingprice DESC;";
            $result = $con->query($sql);
            
            echo "<div class='slotContainer'>";
            //output slot number
            echo "<h3 class='slotTitle'>Slot Number: ".$slotNo."</h3>";
            
            //if records in current slot exist, enter
            if (($number = $result->num_rows) > 0) {
                $i = 0;
                if($session["count".$slotNo] == 5)
                {
                    echo "<p class='noBids'>There are no more bids for this slot</p>";
                }

                //show n bids for each slot, were n starts at 5 and decrements with each bid accept
                while($i < $number - $_SESSION["count".$slotNo]) // should work
                {
                    //get row values from database
                    $row = $result->fetch_assoc();
                    //if record is within the first 5 then output
                    if($i < 5)
                    {
                        echo    "<div class='showBids ".$slotNo.$i."' id='div".$row["id"]."'>";
                    }
                    //otherwise output but hide
                    else
                    {
                        echo    "<div class='showBids ".$slotNo.$i."' style='display:none' id='div".$row["id"]."'>";
                    }
                    //find first position of 'uplaods' in advertFile name
                    $linkPos = stripos($row['advertFile'],"uploads");
                    //extract substring from advertFile starting from 'uploads'
                    $link = substr($row['advertFile'],$linkPos);
                    
                    //output bid data with accept/decline button
                    echo   "<div id='nameGroup'>
                                <div id='boxName'>Name: ".$row['firstname']."</div>
					            <div id='boxSurname'>Surname: ".$row['lastname']."</div>
                            </div>
					        <div id='compGroup'>
                                <div id='boxCompany'>Company: ".$row['company']."</div>
					            <div id='boxEmail'>Email: ".$row['email']."</div>
					        </div>
                            <div id='buttonsGroup'>
                                <div id='biddingPrice'>Bidding Price: ".$row['biddingprice']." euro</div>
                                <input type='button' class='button ".$slotNo."' name='accept' value='accept' id='".$row['id']."'/>	
					            <input type='button' class='button ".$slotNo."' name='decline' value='decline' id='".$row['id']."'/>
					        </div>
                            <audio controls id='audioplayer'>
                                <source src='".$link."' type='audio/mpeg'>
						        Your browser does not support the audio element.
					        </audio>				
			            </div>";
                    //increment bid output counter
                    $i++;
                    echo "<br>";
                }
            } else {
                echo "<p class='noBids'>There are no more bids for this slot</p>";
            }
            echo "</div><br><br>";
        }
        //end connection
        $con->close();
    }
    ?>
    </body>
</html>