<?php
/*
 *  Copyright (C) <2008>  <chenkun cs.kunchen@gmail.com>

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

define('IN_XJOJ', true);
require "init.php";

error_reporting(0);

if (empty($_REQUEST['act']))
{
    $_REQUEST['act'] = 'client_info';
}
else
{
    $_REQUEST['act'] = trim($_REQUEST['act']);
}

if($_REQUEST['act'] == 'client_info') {
    $socket = socket_create(AF_INET, SOCK_STREAM, 0) or sys_msg("can't create socket", 1);

    socket_connect($socket, $client_info_host, $client_info_port) or sys_msg("can't connect to clientinfo server", 1);

    $info = '';
    while(($str = socket_read($socket, 1024)) != "") {
        $info .= $str;
    }

    socket_close($socket);

    $info = explode("\n", $info);
    foreach($info as &$item) {
        if($item == '')
            unset($item);
        $item = explode('|', $item);
        if($item[3] == "-1") {
            $item[3] = 'Waiting for task';
        }else{
            $item[3] = 'Juding runId:'. $item[3];
        }
    }

    $smarty->assign('client_info', $info);
    $smarty->assign('ur_here', 'Client Info');
    $smarty->display('clientinfo.tpl');
}

?>


