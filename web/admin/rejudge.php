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

if (empty($_REQUEST['act']))
{
	$_REQUEST['act'] = 'list';
}
else
{
	$_REQUEST['act'] = trim($_REQUEST['act']);
}

include_once ROOT_PATH. "includes/cls_json.php";
$json = new JSON;
$result = array('error' => 0, 'err_msg' => '', 'content' => '');

if($_REQUEST['act'] == 'list') {
	$smarty->display('rejudge.tpl');
}

elseif($_REQUEST['act'] == 'rejudge_runId') {
	$runId = isset($_GET['runId']) ? intval($_GET['runId']) : 0;
	if($runId <= 0) {
		$result['error'] = 1;
		$result['err_msg'] = 'please input the runId need to be rejudged';
		die($json->encode($result));
	}

	$db->query("call rejudge_runId(". $runId. ")");
	die($json->encode($result));
}

elseif($_REQUEST['act'] == 'rejudge_contest') {
	$cid = isset($_GET['cid']) ? intval($_GET['cid']) : 0;
	if($cid <= 0) {
		$result['error'] = 1;
		$result['err_msg'] = 'please input the contestId need to be rejudged';
		die($json->encode($result));
	}

	$db->query("call rejudge_contest(". $cid. ")");
	die($json->encode($result));
}

?>

