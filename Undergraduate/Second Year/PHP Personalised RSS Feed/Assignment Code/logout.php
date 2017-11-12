<?php
    //delete and destroy all php sessions
    session_start();
    session_unset();
    session_destroy();
    //redirect user to login page
    header('Location: index.php');
    exit;
?>