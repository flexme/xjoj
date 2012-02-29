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
    $ur_here = 'Assemble List';
    $smarty->assign('ur_here', $ur_here);

    $action_link = array('href' => 'assemb.php?act=add', 'text' => 'Add Assemble');
    $smarty->assign('action_link', $action_link);

    $assemb_list = assemb_list();

    $smarty->assign('assemb_list', $assemb_list['assemb']);
    $smarty->assign('filter', $assemb_list['filter']);
    $smarty->assign('record_count', $assemb_list['record_count']);
    $smarty->assign('page_count', $assemb_list['page_count']);
    $smarty->assign('full_page', 1);
    $smarty->display('assemb_list.tpl');
}

elseif ($_REQUEST['act'] == 'add' || $_REQUEST['act'] == 'edit') {
    if($_REQUEST['act'] == 'add') {
        $smarty->assign('ur_here', 'Add Assemble');
        $smarty->assign('form_act', 'insert');
    } else {
        $id = isset($_GET['id']) ? intval($_GET['id']) : 0;
        $assemb = $db->getRow("select * from assemb where id='$id'");

        if(!$assemb) {
            sys_msg('There is no such assemb');
        }

        $smarty->assign('id', $id);
        $smarty->assign('assemb', $assemb);
        $smarty->assign('form_act', 'update');
        
    }

    $smarty->display('assemb_info.tpl');
}

elseif ($_REQUEST['act'] == 'insert' || $_REQUEST['act'] == 'update') {
    $title = isset($_POST['title']) ? trim($_POST['title']) : '';
    
    if(empty($title)) {
        sys_msg('Please Provide Full Information', 1);
    }

    if($_REQUEST['act'] == 'insert') {
        $db->query("insert into assemb (title) values ('$title')");
        clear_all_files();

        $links[] = array('href' => 'assemb.php?act=list', 'text' => 'Assemb List');
        sys_msg('The assemb is added!', 0, $links);
    }else{
        $id = isset($_POST['id']) ? intval($_POST['id']) : 0;
        $db->query("update assemb set title='$title' where id='$id'");
        clear_all_files();

        $links[] = array('href' => 'assemb.php?act=list', 'text' => 'Assemb List');
        sys_msg('The assemb is updated!', 0, $links);
    }
}

elseif ($_REQUEST['act'] == 'remove') {
    $id = intval($_REQUEST['id']);

    $db->query("delete from assemb where id='$id'");
    clear_all_files();

    $url = 'assemb.php?act=query&' . str_replace('act=remove', '', $_SERVER['QUERY_STRING']);
    header("Location: $url\n");
    exit;
}

elseif ($_REQUEST['act'] == 'query') {
    $assemb_list = assemb_list();

    $smarty->assign('assemb_list', $assemb_list['assemb']);
    $smarty->assign('filter', $assemb_list['filter']);
    $smarty->assign('record_count', $assemb_list['record_count']);
    $smarty->assign('page_count', $assemb_list['page_count']);

    make_json_result($smarty->fetch('assemb_list.tpl'), '',
        array('filter' => $assemb_list['filter'], 'page_count' => $assemb_list['page_count']));
}

elseif ($_REQUEST['act'] == 'problems') {
    $cid = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;

    $ret = assemb_problems($cid);

    $smarty->assign('full_page', 1);
    $smarty->assign('problems', $ret['problems']);
    $smarty->assign('assemb', $ret['assemb']);
    $smarty->assign('ur_here', $ret['assemb']['title']." 's problems"); 
    $action_link = array('text' => 'Assemb List', 'href' => 'assemb.php?act=list');
    $smarty->assign('action_link', $action_link);
    $smarty->display('assemb_problem.tpl');
}

elseif ($_REQUEST['act'] == 'remove_assemb_problem') {
    $cid = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;
    $id = isset($_REQUEST['id']) ? intval($_REQUEST['id']) : 0;

    $db->query("delete from assemb_problem where id='$id'");
    clear_all_files();

    $ret = assemb_problems($cid);

    $smarty->assign('problems', $ret['problems']);
    $smarty->assign('assemb', $ret['assemb']);
    make_json_result($smarty->fetch('assemb_problem.tpl'));
}

elseif ($_REQUEST['act'] == 'add_assemb_problem') {
    $problemId = isset($_REQUEST['pid']) ? intval($_REQUEST['pid']) : 0;
    $assembId = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;
	$level = isset($_REQUEST['level']) ? intval($_REQUEST['level']) : 0;

    include_once("../includes/cls_json.php");
    $json = new JSON;
    $res = array('error' => '0', 'message' => '', 'content' => '');

    $count = $db->getOne("select count(*) from problem where problemId='$problemId'");
    if($count == 0) {
        $res['error'] = 1;
        $res['message'] = "There is no such problem";
        die($json->encode($res));
    }

    $count = $db->getOne("select count(*) from assemb_problem where problemId='$problemId' and assembId='$assembId'");
    if($count > 0) {
        $res['error'] = 1;
        $res['message'] = "The problem is already in the assemb";
        die($json->encode($res));
    }

    $db->query("insert into assemb_problem (problemId,assembId,level) values ('$problemId', '$assembId', '$level')");
    clear_all_files();

    $ret = assemb_problems($assembId);

    $smarty->assign('problems', $ret['problems']);
    $smarty->assign('assemb', $ret['assemb']);
    make_json_result($smarty->fetch('assemb_problem.tpl'));
}

function assemb_list() {
    $sql = "select count(*) from assemb";
    $filter['record_count'] = $GLOBALS['db']->getOne($sql);
    
    $filter = page_and_size($filter);
    
    $sql = "select * from assemb order by id desc limit $filter[start],$filter[page_size]";

    $rows = $GLOBALS['db']->getAll($sql);

    return array('assemb' => $rows, 'filter' => $filter, 'page_count' => $filter['page_count'], 'record_count' => $filter['record_count']);
}

function assemb_problems($cid) {
    $assemb = $GLOBALS['db']->getRow("select * from assemb where id='$cid'");

    if(!$assemb) {
        sys_msg('There is no such assemb');
    }

    $problems = $GLOBALS['db']->getAll("select a.*,p.problemName from assemb_problem a inner join problem p on p.problemId=a.problemId where a.assembId='$cid'");

    return array('assemb' => $assemb, 'problems' => $problems);
}
?>

