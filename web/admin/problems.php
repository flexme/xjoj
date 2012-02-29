<?php
define('IN_XJOJ', true);
require "init.php";

$_REQUEST['act'] = empty($_REQUEST['act']) ? 'list' : trim($_REQUEST['act']);

include_once ROOT_PATH."includes/cls_json.php";
$json = new JSON;
$result = array('error' => 0, 'err_msg' => '', 'content' => '');

if($_REQUEST['act'] == 'list') {
	$problemId = empty($_REQUEST['problemId']) ? 0 : intval($_REQUEST['problemId']);
	$problemName = empty($_REQUEST['problemName']) ? '' : trim($_REQUEST['problemName']);

	$ur_here = 'Problem List';
	$smarty->assign('ur_here', $ur_here);

	$action_link = array('href' => 'problems.php?act=add', 'text' => 'Add Problem');
	$smarty->assign('action_link', $action_link);

	$problem_list = problem_list();

	$smarty->assign('problem_list', $problem_list['problems']);
	$smarty->assign('filter', $problem_list['filter']);
	$smarty->assign('record_count', $problem_list['record_count']);
	$smarty->assign('page_count', $problem_list['page_count']);
	$smarty->assign('full_page', 1);

	$smarty->display('problem_list.tpl');
}

elseif ($_REQUEST['act'] == 'edit_problem_name') {
	$problemId = intval($_POST['id']);
	$problemName = trim($_POST['val']);

	$db->query("update problem set problemName='$problemName' where problemId='$problemId'");
	$result['content'] = $problemName;

    	clear_all_files();
	gen_problem_by_id($problemId);
	die($json->encode($result));
}

elseif ($_REQUEST['act'] == 'edit_problem_time') {
	$problemId = intval($_POST['id']);
	$time = intval($_POST['val']);

	$db->query("update problem set timelimit='$time' where problemId='$problemId'");
	$result['content'] = $time;

    	clear_all_files();
	gen_problem_by_id($problemId);
	die($json->encode($result));
}

elseif ($_REQUEST['act'] == 'edit_problem_memory') {
	$problemId = intval($_POST['id']);
	$memory = intval($_POST['val']);

	$db->query("update problem set memorylimit='$memory' where problemId='$problemId'");
	$result['content'] = $memory;

    	clear_all_files();
	gen_problem_by_id($problemId);
	die($json->encode($result));
}

elseif ($_REQUEST['act'] == 'toggle_spj') {
	$problemId = intval($_REQUEST['id']);
	$spj = intval($_REQUEST['val']);
	
	$db->query("update problem set spj='$spj' where problemId='$problemId'");
	$result['content'] = $spj;

    	clear_all_files();
	gen_problem_by_id($problemId);
	die($json->encode($result));
}

elseif ($_REQUEST['act'] == 'toggle_show') {
    	$problemId = intval($_POST['id']);
    	$show = intval($_POST['val']);

    	$db->query("update problem set `show`='$show' where problemId='$problemId'");
    	$result['content'] = $show;

    	clear_all_files();

    	set_problem_show($problemId, $show);
    	die($json->encode($result));
}

elseif ($_REQUEST['act'] == 'add') {
	include_once(ROOT_PATH.'fckeditor/fckeditor.php');

	$action_link = array('href' => 'problems.php?act=list', 'text' => 'Problem List');
	$smarty->assign('action_link', $action_link);
	$smarty->assign('ur_here', 'Add Problem');
	$smarty->assign('form_act', 'insert');
	$smarty->assign('html_editor', create_html_editor('description', '', true));
    	$smarty->assign('input_editor', create_html_editor('input', '', true));
    	$smarty->assign('output_editor', create_html_editor('output', '', true));
    	$smarty->assign('hint_editor', create_html_editor('hint', '', true));
	$smarty->display('problem_info.tpl');
}

elseif ($_REQUEST['act'] == 'edit') {
	include_once(ROOT_PATH.'fckeditor/fckeditor.php');

	$problemId = isset($_GET['problemId']) ? intval($_GET['problemId']) : 0;

	$problem = $db->getRow("select * from problem where problemId = '$problemId'");

	if($problem) {
		$action_link = array('href' => 'problems.php?act=list', 'text' => 'Problem List');
		$smarty->assign('action_link', $action_link);
		$smarty->assign('ur_here', 'Edit Problem');

		$smarty->assign('problem', $problem);
		$smarty->assign('html_editor', create_html_editor('description', $problem['description'], true, '90%'));
        	$smarty->assign('input_editor', create_html_editor('input', $problem['input'], true));
        	$smarty->assign('output_editor', create_html_editor('output', $problem['output'], true));
        	$smarty->assign('hint_editor', create_html_editor('hint', $problem['hint'], true));
		$smarty->assign('form_act', 'update');
		$smarty->display('problem_info.tpl');
	}else{
		sys_msg("Wrong Problem Id");
	}
}

elseif ($_REQUEST['act'] == 'insert' || $_REQUEST['act'] == 'update') {
	$problemName = isset($_POST['problemName']) ? htmlspecialchars(trim($_POST['problemName'])) : '';
	$timelimit = isset($_POST['timelimit']) ? intval($_POST['timelimit']) : 0;
	$memorylimit = isset($_POST['memorylimit']) ? intval($_POST['memorylimit']) : 0;
	$spj = isset($_POST['spj']) ? intval($_POST['spj']) : 0;
	$show = isset($_POST['show']) ? intval($_POST['show']) : 0;
	$description = isset($_POST['description']) ? trim($_POST['description']) : '';
	$input = isset($_POST['input']) ? (trim($_POST['input'])) : '';
	$output = isset($_POST['output']) ? (trim($_POST['output'])) : '';
	$sample_input = isset($_POST['sample_input']) ? htmlspecialchars(trim($_POST['sample_input'])) : '';
	$sample_output = isset($_POST['sample_output']) ? htmlspecialchars(trim($_POST['sample_output'])) : '';
	$hint = isset($_POST['hint']) ? (trim($_POST['hint'])) : '';

	if(empty($problemName)) {
		sys_msg('Please input the problem name');
	}
	if(empty($timelimit) || $timelimit <= 0) {
		sys_msg('Time Limit must be a positive integer');
	}
	if(empty($memorylimit) || $memorylimit <= 0) {
		sys_msg('Memory Limit must be a positive integer');
	}

	$links = array();

	if($_REQUEST['act'] == 'insert') {
		$sql = "insert into problem (problemName,description,timelimit,memorylimit,input,output,sample_input,sample_output,hint,spj,`show`) values ('$problemName','$description','$timelimit','$memorylimit','$input','$output','$sample_input','$sample_output','$hint','$spj','$show')";

		$links[] = array('text' => 'Continue Add Problem', 'href' => 'problems.php?act=add');
	} else {
		$problemId = isset($_POST['problemId']) ? intval($_POST['problemId']) : 0;

		$sql = "update problem set problemName='$problemName',description='$description',timelimit='$timelimit',memorylimit='$memorylimit',input='$input',output='$output',sample_input='$sample_input',sample_output='$sample_output',hint='$hint',spj='$spj',`show`='$show' where problemId='$problemId'";

		$links[] = array('text' => 'Continue Edit Problem', 'href' => 'problems.php?act=edit&problemId='. $problemId);
	}

	$links[] = array('text' => 'Problem List', 'href' => 'problems.php?act=list');

	$db->query($sql);

    	clear_all_files();

	if($_REQUEST['act'] == 'insert') {
		$problemId = $db->insert_id();
		gen_problem_by_id($problemId);
		set_problem_show($problemId, $show);
		sys_msg('Success to add problem', 0, $links);
	}else{
		gen_problem_by_id($problemId);
		set_problem_show($problemId, $show);
		sys_msg('Success to edit problem', 0, $links);
	}
}

elseif ($_REQUEST['act'] == 'remove'){
	$problemId = intval($_REQUEST['id']);

	$db->query("delete from problem where problemId='$problemId'");
    	unlink("../problem_html/". $problemId. ".html");

    	clear_all_files();

	$url = 'problems.php?act=query&'. str_replace('act=remove', '', $_SERVER['QUERY_STRING']);

	unlink(ROOT_PATH ."problem_html/".$problemId.".html");
	header("Location: $url\n");
	exit;
}

elseif ($_REQUEST['act'] == 'query') {
	$problem_list = problem_list();

	$smarty->assign('problem_list', $problem_list['problems']);
	$smarty->assign('filter', $problem_list['filter']);
	$smarty->assign('record_count', $problem_list['record_count']);
	$smarty->assign('page_count', $problem_list['page_count']);
	
	make_json_result($smarty->fetch('problem_list.tpl'), '',
		array('filter' => $problem_list['filter'], 'page_count' => $problem_list['page_count']));
}

elseif ($_REQUEST['act'] == 'gen_html') {
	$start = isset($_REQUEST['start']) ? intval($_REQUEST['start']) : 1;
	$end = isset($_REQUEST['start']) ? intval($_REQUEST['end']) : 0;

	if($start > $end) {
		$result['error'] = 1;
		$result['err_msg'] = 'Wrong Range';
		die($json->encode($result));
	}

	$problems = $db->getAll("select * from problem where `show`='1' and problemId>='$start' and problemId<='$end'");

	foreach($problems as $problem) {
		gen_problem_html($problem);
	}

	die($json->encode($result));
}

elseif ($_REQUEST['act'] == 'problem_html') {
	$total = $db->getOne("select count(*) from problem");
	$range = $db->getRow("select min(problemId) as minr, max(problemId) as maxr from problem");
	$smarty->assign('total', $total);
	$smarty->assign('range', $range);
	$smarty->display('problem_html.tpl');
}

function problem_list() {
	$filter['problemId'] = empty($_REQUEST['problemId']) ? 0 : intval($_REQUEST['problemId']);
	$filter['problemName'] = empty($_REQUEST['problemName']) ? '' : trim($_REQUEST['problemName']);

	$where = '';
	if(!empty($filter['problemId'])) {
		$where .= " and problemId='$filter[problemId]'";
	}
	if(!empty($filter['problemName'])) {
		$where .= " and problemName like '%$filter[problemName]%'";
	}

	$sql = "select count(*) from problem where 1=1 $where";
	$filter['record_count'] = $GLOBALS['db']->getOne($sql);

	$filter = page_and_size($filter);

	$sql = "select * from problem where 1=1 $where order by problemId desc limit ".$filter['start']. ",$filter[page_size]";

	$rows = $GLOBALS['db']->getAll($sql);

	return array('problems' => $rows, 'filter' => $filter, 'page_count' => $filter['page_count'], 'record_count' => $filter['record_count']);
}

function set_problem_show($problemId, $show) {
    if($show) {
        gen_problem_by_id($problemId);
    }else{
        unlink("../problem_html/". $problemId. ".html");
    }
}
?>

