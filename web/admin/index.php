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

if (!isset($_REQUEST['act']) || $_REQUEST['act'] == '') {
    $smarty->display('index.tpl');
}

elseif ($_REQUEST['act'] == 'top') {
	$smarty->assign('admin_id', $_SESSION['admin_id']);
	$smarty->display('top.tpl');
}

elseif ($_REQUEST['act'] == 'menu') {
	$smarty->display('menu.tpl');
}

elseif ($_REQUEST['act'] == 'main') {
	$mysql_ver = $db->version();

	$sys_info['os']            = PHP_OS;
	$sys_info['ip']            = $_SERVER['SERVER_ADDR'];
	$sys_info['web_server']    = $_SERVER['SERVER_SOFTWARE'];
	$sys_info['php_ver']       = PHP_VERSION;
	$sys_info['mysql_ver']     = $mysql_ver;
	$sys_info['zlib']          = function_exists('gzclose') ? 'YES':'NO';
	$sys_info['safe_mode']     = (boolean) ini_get('safe_mode') ?  'YES':'NO';
    $sys_info['safe_mode_gid'] = (boolean) ini_get('safe_mode_gid') ? 'YES' : 'NO';
    $sys_info['timezone']      = function_exists("date_default_timezone_get") ? date_default_timezone_get() : 'No Timezone';
	$sys_info['socket']        = function_exists('fsockopen') ? 'YES' : 'NO';
	$sys_info['max_filesize'] = ini_get('upload_max_filesize');

	$smarty->assign('sys_info', $sys_info);
	$smarty->display('main.tpl');
}

elseif ($_REQUEST['act'] == 'clear_cache')
{
    clear_all_files();

    sys_msg('Caches Cleared');
}

?>
