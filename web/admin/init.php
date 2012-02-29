<?php

if(!defined('IN_XJOJ')) {
	die("Access denied");
}

error_reporting(E_ALL);

define('ROOT_PATH', str_replace('admin/init.php', '', str_replace('\\', '/', __FILE__)));

@ini_set('session.cache_expire',  180);
@ini_set('session.use_trans_sid', 0);
@ini_set('session.use_cookies',   1);
@ini_set('session.auto_start',    0);
@ini_set('display_errors',        1);

require(ROOT_PATH . "config.inc.php");
require(ROOT_PATH . "includes/lib_common.php");
require(ROOT_PATH . "includes/Smarty.class.php");
require("lib_main.php");

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
$db = new cls_mysql($db_host, $db_user, $db_password, $db_name, 'utf8', 0, 1);
$db_host = $db_user = $db_password = $db_name = NULL;

$smarty = new Smarty();
$smarty->debugging = false;

$smarty->template_dir = ROOT_PATH . 'admin/templates';
$smarty->compile_dir = ROOT_PATH . 'templates_c/admin';
$smarty->config_dir = ROOT_PATH . 'configs';
$smarty->config_load('xjoj.conf');

ob_start();

session_start();

$_REQUEST['act'] = isset($_REQUEST['act']) ? trim($_REQUEST['act']) : '';

if ((!isset($_SESSION['admin_id']) || intval($_SESSION['admin_id']) <= 0) &&
    $_REQUEST['act'] != 'login' && $_REQUEST['act'] != 'signin') {
	header("Location: user.php?act=login\n");
}

header('content-type: text/html; charset=utf-8');
header('Expires: Fri, 14 Mar 1980 20:53:00 GMT');
header('Last-Modified: ' . gmdate('D, d M Y H:i:s') . ' GMT');
header('Cache-Control: no-cache, must-revalidate');
header('Pragma: no-cache');

?>
