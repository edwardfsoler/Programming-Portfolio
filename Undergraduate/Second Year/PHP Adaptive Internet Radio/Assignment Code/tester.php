<?php
require_once 'WindowsAzure.php';

use WindowsAzure\Common\ServicesBuilder;
use WindowsAzure\Common\ServiceException;
$connectionString = DefaultEndpointsProtocol=http;AccountName=airgapt;AccountKey=jwB9LoeFiHDCRvC4j4iTzmNy+TYfDkKdcTe92B2TuHP/HkEF65F/QaY3xnU5w26Aaou87IbPr9ZPrjqc7ztBHw==

// Create blob REST proxy.
$blobRestProxy = ServicesBuilder::getInstance()->createBlobService($connectionString);


$content = fopen("D:/home/site/wwwroot/uploads3/Jack-Garratt-Breathe-Life.mp3", "r");
$blob_name = "myblob";

try {
    //Upload blob
    $blobRestProxy->createBlockBlob("adverts", $blob_name, $content);
}
catch(ServiceException $e){
    // Handle exception based on error codes and messages.
    // Error codes and messages are here:
    // http://msdn.microsoft.com/library/azure/dd179439.aspx
    $code = $e->getCode();
    $error_message = $e->getMessage();
    echo $code.": ".$error_message."<br />";
}
?>
