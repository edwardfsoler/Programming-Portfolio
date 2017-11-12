<?php
    //start PHP session
    session_start();

//login validation
if(!isset($_SESSION["login_user"]) || empty($_SESSION["login_user"]))
{
    echo "Please log in to continue";
}
else
{
    //connect to database
    $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
    $username = "bddef64b53f7a1";
    $password = "9d65b26f";
    $schemaName = "acsm_07f2d8d959cb2ba";

    $conn = mysqli_connect($localhost, $username, $password, $schemaName) or die("I couldn't connect to your database, please make sure your info is correct!");

    mysqli_select_db($conn, $schemaName) or die("I couldn't find the database table make sure it's spelt right!");

    //select user credentials for current user
    $sql = "SELECT id, name FROM rsslogin WHERE email = '".$_SESSION["login_user"]."'";
    $result = $conn->query($sql);

    if($result->num_rows == 1)
    {
        $row = $result->fetch_assoc();
        $id = $row["id"];
        $name = $row["name"];
    }
    else
    {
        echo "Something went wrong logging in";
        return;
    }
?>

<!DOCTYPE html>
<html>
<head>
    <!--import jQuery library-->
     <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script>
<script>
    //function to dynamically call PHP script to load RSS Feeds
    function showRSS(str) {
        if (str.length == 0) {
            document.getElementById("rssOutput").innerHTML = "";
            return;
        }
        if (window.XMLHttpRequest) {
            // code for IE7+, Firefox, Chrome, Opera, Safari
            xmlhttp = new XMLHttpRequest();
        } else {  // code for IE6, IE5
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                document.getElementById("rssOutput").innerHTML = xmlhttp.responseText;

                $('#footer').show();
            }
        }
        xmlhttp.open("GET", "getrss.php?q=" + str, true);
        xmlhttp.send();
    }

    //function to call individual feed script
    function showIndividualRss(str) {
        if (str.length == 0) {
            document.getElementById("rssOutput").innerHTML = "";
            return;
        }
        if (window.XMLHttpRequest) {
            // code for IE7+, Firefox, Chrome, Opera, Safari
            xmlhttp = new XMLHttpRequest();
        } else {  // code for IE6, IE5
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                document.getElementById("rssOutput").innerHTML = xmlhttp.responseText;
            }
        }
        xmlhttp.open("GET", "individualRSS.php?q=" + str, true);
        xmlhttp.send();
    }

    $(document).ready(function () {

        //function to dynamically add RSS Feed
        $('#addRssSubmit').on("click", function () {
            var tester = true;
            //disable submit button as soon as it is clicked to avoid multiple registrations
            $('#addRssSubmit').attr("disabled", "disabled");

            var x = $("#addNewRssForm").serializeArray();

            $.each(x, function (i, field) {
                if (field.value.length < 1) {
                    tester = false;
                }
            });

            if (tester == true) {
                addRSSAjax();
            }
            else {
                document.getElementById("rssAddOutput").innerHtml = "Please fill in all fields before submitting";
            }
            $('#addRssSubmit').removeAttr("disabled");
        });
    });

    //function to call clicked script when an article is read
    function titleClick(index, unique, title) {
        $('#' + unique).fadeTo(1000, 0.3);

        if (window.XMLHttpRequest) {
            // code for IE7+, Firefox, Chrome, Opera, Safari
            xmlhttp = new XMLHttpRequest();
        } else {  // code for IE6, IE5
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                //document.getElementById("rssOutput").innerHTML = xmlhttp.responseText;
            }
        }
        xmlhttp.open("POST", "watched.php", true);
        xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xmlhttp.send("feedIndex=" + index + "&unique=" + unique + "&title=" + title);
    }

    //call PHP script to add new rss feed
    function addRSSAjax() {
        var pointer = document.getElementById("addNewRssForm");
        if (window.XMLHttpRequest) {
            // code for IE7+, Firefox, Chrome, Opera, Safari
            xmlhttp = new XMLHttpRequest();
        } else {  // code for IE6, IE5
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                //document.getElementById("rssOutput").innerHTML = xmlhttp.responseText;
            }
        }
        xmlhttp.open("POST", "addRSS.php", true);
        xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xmlhttp.send("category=" + pointer["category"].value + "&name=" + pointer["rssName"].value + "&link=" + pointer["rssLink"].value);
    }


</script>

    <link rel="stylesheet" type="text/css" href="style.css" />
</head>
<body>
    <div id="header">
        <?php
            echo "<p id='WelcomeNote'>Hello, ".$name."</p>";
        ?>
        <a href="logout.php" id="logOutButton">Log Out</a>
		    <a href="rssloader.php" id="WebTitle"><h1>Yours Daily</h1></a>
        <script>showRSS("General");</script>
        <!--Drop down menu to select category to view-->
        <form id="categorySelector">
        <select onchange="showRSS(this.value)">
        <option value="General">Recommended</option>
        <option value="Local">Local News</option>
        <option value="International">International News</option>
        <option value="Sports">Sports</option>
        <option value="Finance">Finance</option>
        <option value="Science">Science & Technology</option>
        <option value="Politics">Politics</option>
        </select>
        </form>
        <br>    
	</div>
    
	    <div id="content">
		    <div id="rssLinkURL">	
                
			    <form id="linkAlignment" action="deleteLink.php" method="post">
                <?php
                    //get the title of each link under the current user
                    $sql = "SELECT title FROM rsslinksconnect WHERE userId = ".$id;
                    $result = $conn->query($sql);

                    echo "<br/>";

                    //output div with rss feed title and button to delete it
                    while($row = $result->fetch_assoc())
                    {
                        echo "<div id='deleteBar'>";
                            echo "<a onclick='showIndividualRss(\"".$row["title"]."\")'><span class='deleteLinkTitle Class1'>".$row["title"]."</span></a>";
                            echo "<button id='deleteLink'class='deleteClass Class1' name='deleteLink' value='".$row["title"]."'>delete</button>";
                        echo "</div>";
                    }
                ?>
			    </form>
			    <div id="toggleText">
			    <h5> Select a category for the new RSS link:</h5>
					
                <!-- Form to add a new RSS feed-->
			    <form name="addNewRssForm" id="addNewRssForm" method="post" onsubmit=" $('#addNewRssForm').each(function(){this.reset();}); return false;">
				    <input type="radio" name="category" value="2" required>Sports<br>
				    <input type="radio" name="category" value="5">Politics<br>
				    <input type="radio" name="category" value="4">Science and Technology<br>
				    <input type="radio" name="category" value="3">Finance<br>
				    <input type="radio" name="category" value="0">Local<br>
                    <input type="radio" name="category" value="1">International<br>
			    
			        <h5>Title of new RSS link:</h5>
			        <input class="addnew" type="text" placeholder="Enter a name for this RSS link" name="rssName" required/>
			        <h5>RSS link:</h5>
			        <input class="addnew" type="text" placeholder="Enter the RSS link" name="rssLink" required/>
                    
			        <button id="addRssSubmit">Add</button>
                </form>
                    <p id="rssAddOutput"></p>					
			    </div>
		    </div>
	</div>

        <span id="rssOutput"></span>
        <div id="footer">
		        Copyright &copy 2016 Jenny Attard, Kristina Catania & Edward Fleri Soler.
        </div>	
    </body>
    <?php
        }
        ?>
</html> 