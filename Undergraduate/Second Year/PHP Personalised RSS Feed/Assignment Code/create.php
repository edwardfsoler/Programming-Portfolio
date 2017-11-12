<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8" />
        <title>Registration</title>
        <link rel="stylesheet" type="text/css" href="Registration.css">
        <!--jQuery link-->
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script>
        <script type="text/javascript">
            $(document).ready(function () {
                //function to switch form output on screen and call PHP script on first form submit
                $(document).on("submit", "#registrationForm", function () {
                    //disable submit button as soon as it is clicked to avoid multiple registrations
                    $('#nextB').attr("disabled", "disabled");
                    var data = $(this).serialize();

                    $.ajax({
                        type: 'POST',
                        url: 'register.php',
                        data: data,
                        success: function () {
                            switchd();
                        }
                    });
                    return false;
                });
                //function to prevent duplicate submission caused by fadeOut delay
                $(document).on("submit", "#tastes2", function () {
                    //disable submit button as soon as it is clicked to avoid multiple registrations
                    $('#nextC').attr("disabled", "disabled");
                });

                var checking_html = 'Checking...';

                //Check email availability  
                $('#email').change(function () {
                    //run the character number check 
                    check_availability();
                });
            });
            //function to test the length of the input password
            function passLength() {
                var pass = document.forms["registrationForm"]["passw"];

                if (pass.value.length < 6 || pass.value.length > 16) {
                    document.getElementById("passWarning").style.display = "inline";
                    document.forms["registrationForm"]["passw2"].disabled = true;
                    return false;
                }
                else {
                    document.getElementById("passWarning").style.display = "none";
                    document.forms["registrationForm"]["passw2"].disabled = false;
                    return true;
                }
            }

            //function to test whether input passwords match
            function passMatch() {
                var pass = document.forms["registrationForm"]["passw2"].value;
                var pass2 = document.forms["registrationForm"]["passw"].value;

                if (pass === pass2) {
                    document.getElementById("passMismatchWarning").style.display = "none";
                    document.forms["registrationForm"]["submit"].disabled = false;
                }
                else {
                    document.getElementById("passMismatchWarning").style.display = "inline";
                    document.forms["registrationForm"]["submit"].disabled = true;
                }
            }
            
            //function to switch form on screen
            function switchd() {

                $("#register").fadeOut(300);
                setTimeout(function () { $("#tastes").fadeIn(300); }, 300);
            }


            //function to check username availability  
            function check_availability() {

                //get the username  
                var username = $('#email').val();
                //use ajax to run the check  
                $.post("check_username.php", { username: username },
                    function (result) {
                        //if the result is 1  
                        if (result == 1) {
                            //show that the username is available  
                            $('#username_availability_result').html(username + ' is Available');
                            $('#nextB').removeAttr("disabled");
                        } else {
                            //show that the username is NOT available  
                            $('#username_availability_result').html(username + ' is not Available');
                            $('#nextB').attr("disabled", "disabled");
                        }
                    });
            } 
        </script>
    </head>
    <body>
        <div id="header">
            <a href="index.php" id="WebTitle"><h1>Yours Daily</h1></a>
            <br>    
	    </div>
        <div id="register">
            <!--Registration form -->
            <form name="registrationForm" id="registrationForm">
                First Name: <br/>
                <input type="text" name="fname" placeholder="First Name" id="fname" required><br/><br/>

                Last Name: <br/>
                <input type="text" name="lname" placeholder="Last Name" id="lname" required><br/><br/>

                Email address: <br/>
                <input type="email" name="email" placeholder="example@abc.com" id="email" required> <div id='username_availability_result'></div><br/>

                Password: <br/>
                <input type="password" name="passw" id="pass1" pattern=".{6,16}" onchange="passLength()" required>
                <p id="passWarning">Password must be between 6 and 16 characters long.</p><br/><br/>

                Confirm Password: <br/>
                <input type="password" name="passw2" id="pass2" pattern=".{6,16}" onchange="passMatch()" required>
                <p id="passMismatchWarning">Passwords do not match.</p>

                <input type="submit" name="submit" value="Next" id="nextB">
            </form>
        </div>

        <div id="tastes">
            <h2>Interests</h2>
            <h3>Please select any topics which you are interested in: </h3>
            <!-- Explicit tastes form-->
            <form method="post" action="handleTastes.php" name="tastes" id="tastes2">
                <input type="checkbox" name="lnews" value="lnews" id="lnewscb">
                <label for="lnewscb">Local News</label></br>

                <input type="checkbox" name="inews" value="inews" id="inewscb">
                <label for="inewscb">International News</label></br>

                <input type="checkbox" name="sports" value="sports" id="sportscb">
                <label for="sportscb">Sports</label></br>

                <input type="checkbox" name="finance" value="finance" id="financecb">
                <label for="financecb">Finance</label></br>

                <input type="checkbox" name="tech" value="tech" id="techcb">
                <label for="techcb">Technology</label></br>

                <input type="checkbox" name="politics" value="politics" id="politicscb">
                <label for="politicscb">Politics</label></br>
                </br>
                <input type="submit" value="Next" id="nextC">
            </form>
        </div>
    </body>
</html>