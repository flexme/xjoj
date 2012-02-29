<?php

header('Content-type: application/x-javascript; charset=utf-8');

include_once('calendar_lang.php');

foreach ($_LANG['calendar_lang'] AS $cal_key => $cal_data)
{
    echo 'var ' . $cal_key . " = \"" . $cal_data . "\";\r\n";
}

include_once('calendar/calendar.js');

?>
