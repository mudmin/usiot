<?php
//Please note that this is just a demo and you probably want to do a little more security stuff on this.
//this file goes in your root directory

require_once 'users/init.php';
$db = DB::getInstance();

$temp = Input::get('temp');

if(is_numeric($temp)){
if(!empty($temp)){
  $fields = array(
    'temp'    => $temp,
    'timestamp'=> date("Y-m-d H:i:s"),
  );

    $db->update('temperature',1,$fields);
}
}


?>
