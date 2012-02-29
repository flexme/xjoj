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

require "includes/init.php";

$problemId = isset($_REQUEST['proId']) ? intval($_REQUEST['proId']) : 0;
$runId = isset($_REQUEST['runId']) ? intval($_REQUEST['runId']) : 0;
$authorId = isset($_REQUEST['authorId']) ? intval($_REQUEST['authorId']) : 0;
$cid = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;
$author = isset($_REQUEST['author']) ? trim($_REQUEST['author']) : '';
$status = isset($_REQUEST['status']) ? trim($_REQUEST['status']) : '';
$start = isset($_REQUEST['start']) ? intval($_REQUEST['start']) : 0;
$langId = isset($_REQUEST['langId']) ? intval($_REQUEST['langId']) : 0;

$enable_search = isset($_REQUEST['enable_search']) ? intval($_REQUEST['enable_search']) : 0;

$query['proId'] = $problemId;
if($query['proId'] == 0) {
    $query['proId'] = '';
}
$query['runId'] = $runId;
$query['authorId'] = $authorId;
$query['cid'] = $cid;
$query['author'] = $author;
$query['status'] = $status;
$query['langId'] = $langId;

$sql = "select s.*,a.username,l.displayName from status s".
		" inner join author a on s.authorId=a.user_id {are}".
		" inner join language l on l.id=s.languageId where 1 = 1";

if(!empty($problemId)) {
	$sql .= " and s.problemId = '$problemId'";
}

if(!empty($runId)) {
	$sql .= " and s.runId = '$runId'";
}

if(!empty($authorId)) {
	$sql .= " and s.authorId = '$authorId'";
}

if(!empty($cid)) {
    $sql .= " and s.cid = '$cid'";
}

if(!empty($langId)) {
    $sql .= " and s.languageId = '$langId'";
}

if(!empty($author)) {
    $sql = str_replace('{are}', " and a.username = '$author'", $sql);
}else{
    $sql = str_replace('{are}', "", $sql);
}

if(!empty($status)) {
	$sql .= " and s.judgeStatus = '$status'";
}

if(empty($start)) {
	$sql .= " order by s.runId desc limit $perpage";
    $status = $db->getAll($sql);
} else {
    $m = isset($_GET['m']) ? trim($_GET['m']) : 'n';
    if($m != 'n' && $m != 'p') {
        //next or prev
        $m = 'n';
    }
    if($m == 'n') {
        //next
    	$sql .= " and s.runId < '$start' order by s.runId desc limit $perpage";
        $status = $db->getAll($sql);
    }else{
        //prev
        $sql .= " and s.runId > '$start' order by s.runId asc limit $perpage";
        $status = $db->getAll($sql);
        $status = array_reverse($status);
    }
}

$firstRunId = 0;
$lastRunId = 0;
if($status) {
    $firstRunId = $status[0]['runId'];
    $lastRunId = $status[count($status) - 1]['runId'];
}

$smarty->assign('firstRunId', $firstRunId);
$smarty->assign('lastRunId', $lastRunId);
$smarty->assign('query', $query);
$smarty->assign('status', $status);

if($cid) {
    $contest = $db->getRowCached("select * from contest where id='$cid'");
    $problems = $db->getAllCached("select c.*,p.problemName from contest_problem c  inner join problem p on p.problemId=c.problemId where contestId='$cid' order by sort_order");
    $ename = array();

    foreach($problems as $key => $p) {
        $ename[$p['problemId']] = chr(65 + $key);
    }

    $smarty->assign('contest', $contest);
    $smarty->assign('ename', $ename);
}

$smarty->assign('compilers', get_compilers());
$smarty->assign('enable_search', $enable_search);
$smarty->caching = 0;
$smarty->display('status.tpl');
?>

