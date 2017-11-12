<?php
    //start PHP session
    session_start();

    //test whether user is logged in
    if(!isset($_SESSION["login_user"]) || empty($_SESSION["login_user"]))
    {
        echo "Please log in to continue";
    }
    else
    {
        //get the q parameter from URL
        $q=$_GET["q"];
        //connect to database
        $localhost = "eu-cdbr-azure-north-e.cloudapp.net";
        $username = "bddef64b53f7a1";
        $password = "9d65b26f";
        $schemaName = "acsm_07f2d8d959cb2ba";

        //test whether  page works following fixing of database
        $conn = new mysqli($localhost, $username, $password, $schemaName);

        if ($conn->connect_error) {
            echo "error";
            die("Connection failed: " . $conn->connect_error);
        } 
        //function to get article image (same as in getrss.php)
        function getImage($i, $x, $feedId)
          {
              if($feedId == 1)
              {
                  $item_desc=$x->item($i)->getElementsByTagName('description')->item(0)->childNodes->item(0)->nodeValue;
          
                  $firstOcc2 = stripos($item_desc, '<p>');
                  $lastOcc2 = stripos($item_desc, '</p>');

                  $firstOcc = stripos($item_desc, 'src="');
                  $lastOcc = stripos($item_desc, '" ');

                  if($firstOcc2 != FALSE && $lastOcc2 != FALSE)
                  {
                      $lastOcc2 += 4;
                      $length2 = $lastOcc2 - $firstOcc2;
                      $x->item($i)->getElementsByTagName('description')->item(0)->childNodes->item(0)->nodeValue = substr_replace($item_desc,"",$firstOcc2, $length2);
                  }
              
               
                  if($firstOcc != FALSE && $lastOcc != FALSE)
                  {
                      $firstOcc+=5;
                      $length = $lastOcc - $firstOcc;
                      $src = substr($item_desc, $firstOcc, $length);
                      return $src;     
                  }
                  else
                  {
                      return "http://www.unistudentdiscounts.com/wp-content/uploads/2016/01/yd-logo-white-on-black.jpg";
                  }
              }
              else if($feedId == 2 || $feedId == 11 || $feedId == 14)
              {
                  $item_desc=$x->item($i)->getElementsByTagName('description')->item(0)->childNodes->item(0)->nodeValue;
                  $item_img2=$x->item($i)->getElementsByTagName('enclosure');

                  foreach($item_img2 as $item)
                  {
                      $item_img = $item->getAttribute('url');
                  }
          
                   $firstImg = stripos($item_desc, "<img");
                   $lastImg = stripos($item_desc, "/>")+2; 

                  while($firstImg != FALSE && $lastImg != FALSE)
                  {
                     $item_desc = $x->item($i)->getElementsByTagName('description')->item(0)->childNodes->item(0)->nodeValue = substr_replace($item_desc,"",$firstImg, $lastImg - $firstImg);
                     $firstImg = stripos($item_desc, "<img");
                     $lastImg = stripos($item_desc, "/>")+2;   
                  }

                  if(strlen($item_img) == 0)
                  {
                       $item_img = "http://www.unistudentdiscounts.com/wp-content/uploads/2016/01/yd-logo-white-on-black.jpg";
                  }
                  return $item_img;
              }
              else
              {
                  $src = "http://www.unistudentdiscounts.com/wp-content/uploads/2016/01/yd-logo-white-on-black.jpg";
                  return $src;
              }
          }
      
              //make call to get id of user that is currently logged in
              $sql = "SELECT id, name FROM RSSLogin WHERE email ='".$_SESSION["login_user"]."'";
              $result = $conn->query($sql);

              if($result->num_rows == 1)
              {
                  $row = $result->fetch_assoc();
                  $id = $row["id"];
                  $name = $row["name"];
              }
              else
              {
                  echo "Something went wrong";
                  return;
              }
          
              $sql = "UPDATE RSSLinksConnect SET priority = priority + 8   WHERE priority < 992 AND userId =".$id." AND title = '".$q."'";
              $result = $conn->query($sql);
      
          
                  //get the id and priority of all the links which the user holds
                  $sql = "SELECT id, priority FROM RSSLinksConnect WHERE userId =".$id." AND title = '".$q."'";

                  $result = $conn->query($sql);

                  $rssLinks = array();

                  if ($result->num_rows > 0)
                  {
                      while($row = $result->fetch_assoc())
                      {
                          //store link id and link priority in an array
                          array_push($rssLinks, $row["id"], $row["priority"]);
                      }
                  }
                  else
                  {
                      echo "Something went wrong";
                      return;
                  }
      
                $articles = array();

                $counter = 0;
                $newcounter = 0;

        //traverse the array to get the link of each rss feed
        for($i = 0; $i < (count($rssLinks)/2); $i++)
        {
                $index = $i * 2;

                $linkId = $rssLinks[$index];
        
                //get the link of each rss feed
                $sql = "SELECT link FROM RSSLinks WHERE id =".$linkId;
                $result = $conn->query($sql);

                if($result->num_rows == 1)
                {
                    $row = $result->fetch_assoc();
                    $link = $row["link"];
                }
                else
                {
                    echo "Something went3 wrong";
                }

                $sql = "SELECT title FROM RSSLinksConnect WHERE id =".$linkId." AND userid=".$id;
                $result = $conn->query($sql);
                $Ltitle = "";

                if($result->num_rows == 1)
                {
                    $row = $result->fetch_assoc();
                    $Ltitle = $row["title"];
                }
                else
                {
                    echo "Something went wrong";
                }

                //get xml data from link
                $xmlDoc = new DOMDocument();
                $xmlDoc->load($link);

                //get and output "<item>" elements
                $x = $xmlDoc->getElementsByTagName('item');

                $xpath = new DOMXPath($xmlDoc);
                $count = $xpath->evaluate('count(channel/item)');
        

                //create 2D array to store all articles
                for ($i2 = 0; $i2 < $count; $i2++) {
        
                    $image2 = null;
                    $t = $x->item($i2)->getElementsByTagName('title')->item(0)->childNodes->item(0)->nodeValue;
                    $a = $x->item($i2)->getElementsByTagName('link')->item(0)->childNodes->item(0)->nodeValue;
                
                    $image = getImage($i2, $x, $linkId);  
                
                    $d = $x->item($i2)->getElementsByTagName('description')->item(0)->childNodes->item(0)->nodeValue;
            
                    $d = strip_tags($d);

                    if(strlen($d) > 300)
                    {
                        $add = TRUE;
                        $d = substr($d, 0, 300);
                    }
                    else{
                        $add = FALSE;
                    }

                    if($add)
                    {
                        $d = $d ."...";
                    }

                    $p = $x->item($i2)->getElementsByTagName('pubDate')->item(0)->childNodes->item(0)->nodeValue;

                    //calculate time difference in hours between when the article was published, and the current time
                    date_default_timezone_set("Europe/Malta");
                    $date1 = date_create(date(DATE_RFC2822));
                    $date2 = new DateTime($p);
                    $diff = date_diff($date1,$date2);
                    $timeDiff = $diff->format("%h");

                    //decremenet priority of article by 5% for every hour that it is old
                    $priority = round($rssLinks[($i*2) + 1] * pow(0.95,$timeDiff));

                    if($priority < 0)
                    {
                        $priority = 1;
                    }

                    $pub = date_format($date2, "j/n/Y  g:ia");

                    $seen = FALSE;

                    if(in_array($t, $_SESSION['watched']))
                    {
                        $seen = TRUE;
                    }

                    if($cat == 6)
                    {
                        if($counter < 15)
                        {
                             array_push($articles,array($t,$a,$image,$d,$pub,$priority,$Ltitle, $linkId, $newcounter, $seen));
                             $counter++;
                        }
                    }
                    else
                    {
                        array_push($articles,array($t,$a,$image,$d,$pub,$priority,$Ltitle,$linkId, $newcounter, $seen));
                    }

                    $newcounter++;
                }
        }

        //function for sorting of priority
        function priority_compare($a, $b)
        {
            $t1 = $a[5];
            $t2 = $b[5];
            return $t2 - $t1;
        }  

        //sort all articles in order of priority
        usort($articles, 'priority_compare');

        function getArticle($title, $link, $image, $description, $pubDate, $priority, $Ltitle, $rssIndex, $newcounter, $seen)
          {
              //edit pubdate to be more readable
              if(strlen($description)  < 1)
              {
                $description = "Click on the link to learn more";
              }
          
              if($seen)
              {
                  echo "<div id='".$newcounter."' style='opacity:0.3' class='rss'>";
              }
              else
              {
                    echo "<div id='".$newcounter."' class='rss'>";
              }
			        echo "<div id='rssPic'>
                                <img src='".$image."' height='100%' width='100%' alt='".$image."'/>
                            </div>  
                    
                                <div id='rsstitle'>".$Ltitle."</br>".$pubDate."</div>
                        
			                <div id='rssTitle'>
                                <h3 class='title'><a href='" .$link. "' alt='articlelink' target='_blank' class='".$rssIndex." rssRedirect' onclick='titleClick(".$rssIndex.",".$newcounter.",\"".$title."\")'>" .$title. "</a></h3></br>
                            </div>
                            <div id='rssContent'>".$description."</div>					
		                </div>";
          
          }

        for($i = 0; $i < count($articles); $i++)
        {
            getArticle($articles[$i][0], $articles[$i][1], $articles[$i][2], $articles[$i][3], $articles[$i][4], $articles[$i][5], $articles[$i][6], $articles[$i][7], $articles[$i][8], $articles[$i][9]);
        } 
    }
?>