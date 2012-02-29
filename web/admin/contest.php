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

$_REQUEST['act'] = empty($_REQUEST['act']) ? 'list' : trim($_REQUEST['act']);

if($_REQUEST['act'] == 'list') {
    $ur_here = 'Contest List';
    $smarty->assign('ur_here', $ur_here);

    $action_link = array('href' => 'contest.php?act=add', 'text' => 'Add Contest');
    $smarty->assign('action_link', $action_link);

    $contest_list = contest_list();

    $smarty->assign('contest_list', $contest_list['contest']);
    $smarty->assign('filter', $contest_list['filter']);
    $smarty->assign('record_count', $contest_list['record_count']);
    $smarty->assign('page_count', $contest_list['page_count']);
    $smarty->assign('full_page', 1);
    $smarty->display('contest_list.tpl');
}

elseif ($_REQUEST['act'] == 'add' || $_REQUEST['act'] == 'edit') {
    if($_REQUEST['act'] == 'add') {
        $smarty->assign('ur_here', 'Add Contest');
        $smarty->assign('form_act', 'insert');
    } else {
        $id = isset($_GET['id']) ? intval($_GET['id']) : 0;
        $contest = $db->getRow("select * from contest where id='$id'");

        if(!$contest) {
            sys_msg('There is no such contest');
        }

        $smarty->assign('id', $id);
        $smarty->assign('contest', $contest);
        $smarty->assign('form_act', 'update');
        
    }

    $smarty->display('contest_info.tpl');
}

elseif ($_REQUEST['act'] == 'insert' || $_REQUEST['act'] == 'update') {
    $title = isset($_POST['title']) ? trim($_POST['title']) : '';
    $startTime = isset($_POST['startTime']) ? strtotime($_POST['startTime']) : 0;
    $endTime = isset($_POST['endTime']) ? strtotime($_POST['endTime']) : 0;
    $private = isset($_POST['private']) ? intval($_POST['private']) : 0;
    
    if(empty($title) || empty($startTime) || empty($endTime)) {
        sys_msg('Please Provide Full Information', 1);
    }

    if($startTime >= $endTime) {
        sys_msg('The start time must be earlier then end time', 1);
    }

    if($_REQUEST['act'] == 'insert') {
        $db->query("insert into contest (title,startTime,endTime,private) values ('$title', '$startTime', '$endTime', '$private')");
        clear_all_files();

        $links[] = array('href' => 'contest.php?act=list', 'text' => 'Contest List');
        sys_msg('The contest is added!', 0, $links);
    }else{
        $id = isset($_POST['id']) ? intval($_POST['id']) : 0;
        $db->query("update contest set title='$title', startTime='$startTime', endTime='$endTime', private='$private' where id='$id'");
        clear_all_files();

        $links[] = array('href' => 'contest.php?act=list', 'text' => 'Contest List');
        sys_msg('The contest is updated!', 0, $links);
    }
}

elseif ($_REQUEST['act'] == 'remove') {
    $id = intval($_REQUEST['id']);

    $db->query("delete from contest where id='$id'");
    clear_all_files();

    $url = 'contest.php?act=query&' . str_replace('act=remove', '', $_SERVER['QUERY_STRING']);
    header("Location: $url\n");
    exit;
}

elseif ($_REQUEST['act'] == 'query') {
    $contest_list = contest_list();

    $smarty->assign('contest_list', $contest_list['news']);
    $smarty->assign('filter', $contest_list['filter']);
    $smarty->assign('record_count', $contest_list['record_count']);
    $smarty->assign('page_count', $contest_list['page_count']);

    make_json_result($smarty->fetch('contest_list.tpl'), '',
        array('filter' => $contest_list['filter'], 'page_count' => $contest_list['page_count']));
}

elseif ($_REQUEST['act'] == 'problems') {
    $cid = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;

    $ret = contest_problems($cid);

    $smarty->assign('full_page', 1);
    $smarty->assign('problems', $ret['problems']);
    $smarty->assign('contest', $ret['contest']);
    $smarty->assign('ur_here', $ret['contest']['title']." 's problems"); 
    $action_link = array('text' => 'Contest List', 'href' => 'contest.php?act=list');
    $smarty->assign('action_link', $action_link);
    $smarty->display('contest_problem.tpl');
}

elseif ($_REQUEST['act'] == 'remove_contest_problem') {
    $cid = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;
    $id = isset($_REQUEST['id']) ? intval($_REQUEST['id']) : 0;

    $db->query("delete from contest_problem where id='$id'");
    clear_all_files();

    $ret = contest_problems($cid);

    $smarty->assign('problems', $ret['problems']);
    $smarty->assign('contest', $ret['contest']);
    make_json_result($smarty->fetch('contest_problem.tpl'));
}

elseif ($_REQUEST['act'] == 'add_contest_problem') {
    $problemId = isset($_REQUEST['pid']) ? intval($_REQUEST['pid']) : 0;
    $contestId = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;
    $sort_order = isset($_REQUEST['sort_order']) ? intval($_REQUEST['sort_order']) : 0;

    include_once("../includes/cls_json.php");
    $json = new JSON;
    $res = array('error' => '0', 'message' => '', 'content' => '');

    $count = $db->getOne("select count(*) from problem where problemId='$problemId'");
    if($count == 0) {
        $res['error'] = 1;
        $res['message'] = "There is no such problem";
        die($json->encode($res));
    }

    $count = $db->getOne("select count(*) from contest_problem where problemId='$problemId' and contestId='$contestId'");
    if($count > 0) {
        $res['error'] = 1;
        $res['message'] = "The problem is already in the contest";
        die($json->encode($res));
    }

    $db->query("insert into contest_problem (problemId,contestId,sort_order) values ('$problemId', '$contestId', '$sort_order')");
    clear_all_files();

    $ret = contest_problems($contestId);

    $smarty->assign('problems', $ret['problems']);
    $smarty->assign('contest', $ret['contest']);
    make_json_result($smarty->fetch('contest_problem.tpl'));
}

function contest_list() {
    $sql = "select count(*) from contest";
    $filter['record_count'] = $GLOBALS['db']->getOne($sql);
    
    $filter = page_and_size($filter);
    
    $sql = "select * from contest order by id desc limit $filter[start],$filter[page_size]";

    $rows = $GLOBALS['db']->getAll($sql);

    return array('contest' => $rows, 'filter' => $filter, 'page_count' => $filter['page_count'], 'record_count' => $filter['record_count']);
}

function contest_problems($cid) {
    $contest = $GLOBALS['db']->getRow("select * from contest where id='$cid'");

    if(!$contest) {
        sys_msg('There is no such contest');
    }

    $problems = $GLOBALS['db']->getAll("select c.*,p.problemName from contest_problem c inner join problem p on p.problemId=c.problemId where c.contestId='$cid' order by c.sort_order");
    foreach($problems as $key => &$p) {
        $p['name'] = chr(65+$key);
    }

    return array('contest' => $contest, 'problems' => $problems);
}
?>

