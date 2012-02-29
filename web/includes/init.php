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

if(!defined('IN_XJOJ')) {
	die('Access denied');
}

error_reporting(E_ALL);

define('ROOT_PATH', str_replace('includes/init.php', '', str_replace('\\', '/', __FILE__)));

@ini_set('session.cache_expire',  180);
@ini_set('session.use_trans_sid', 0);
@ini_set('session.use_cookies',   1);
@ini_set('session.auto_start',    0);
@ini_set('display_errors',        1);

require(ROOT_PATH . "config.inc.php");
require(ROOT_PATH . "includes/lib_common.php");
require(ROOT_PATH . "includes/lib_main.php");
require(ROOT_PATH . "includes/Smarty.class.php");

if(!get_magic_quotes_gpc()) {
	if(!empty($_GET)) {
		$_GET = addslashes_deep($_GET);
	}
	if(!empty($_POST)) {
		$_POST = addslashes_deep($_POST);
	}

	$_COOKIE   = addslashes_deep($_COOKIE);
	$_REQUEST  = addslashes_deep($_REQUEST);
}

if(PHP_VERSION >= '5.1' && !empty($timezone)) {
	date_default_timezone_set($timezone);
}

require(ROOT_PATH . 'includes/cls_mysql.php');
$db = new cls_mysql($db_host, $db_user, $db_password, $db_name);
$db_host = $db_user = $db_password = $db_name = NULL;

$smarty = new Smarty();
$smarty->debugging = false;
$smarty->caching = 2;
$smarty->config_load('xjoj.conf');

$login_user = array();

session_start();
ob_start();

if(isset($_COOKIE['user_id']) && !empty($_COOKIE['user_id'])) {
	$sql = "select * from author where password='$_COOKIE[password]' and user_id='$_COOKIE[user_id]'";
	//使用建立缓存的sql查询
	$login_user = $db->getRow($sql);
	if(empty($login_user['user_id'])) {
		setcookie('user_id', '');
		setcookie('password', '');
		$login_user = array();
	}
}

$smarty->assign('login_user', $login_user);
?>

