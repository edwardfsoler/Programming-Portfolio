<?php
    //Delete all files from Slot1
    $files = glob('D:/home/site/wwwroot/Slot1/*'); // get all file names
    foreach($files as $file){ // iterate files
      if(is_file($file))
        unlink($file); // delete file
    }

    //Delete all files from Slot2
    $files = glob('D:/home/site/wwwroot/Slot2/*'); // get all file names
    foreach($files as $file){ // iterate files
      if(is_file($file))
        unlink($file); // delete file
    }

    //Delete all files from Slot3
    $files = glob('D:/home/site/wwwroot/Slot3/*'); // get all file names
    foreach($files as $file){ // iterate files
      if(is_file($file))
        unlink($file); // delete file
    }
?>