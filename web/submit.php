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

$action = empty($_REQUEST['act']) ? 'submit' : trim($_REQUEST['act']);
$cid = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;

$smarty->caching = 0;

if($cid) {
    $problems = $db->getAllCached("select c.problemId from contest_problem c where contestId='$cid' order by sort_order");
    foreach($problems as $key => &$p) {
        $p['name'] = chr($key + 65);
    }
    $smarty->assign('problems', $problems);
}

if($action == 'submit') {
	$problemId = isset($_GET['proId']) ? intval($_GET['proId']) : '';

	if(empty($problemId)) {
		$problemId = '';
	}

	$smarty->assign('compilers', get_compilers());
	$smarty->assign('problemId', $problemId);
    $smarty->assign('cid', $cid);

    if($cid) {
        $contest = $db->getRowCached("select * from contest where id='$cid'");
        $smarty->assign('contest', $contest);
    }
	$smarty->display('submit.tpl');
}

elseif($action == 'sendfile') {
    $problemId = isset($_POST['problemId']) ? intval($_POST['problemId']) : 0;
    $cid = isset($_POST['cid']) ? intval($_POST['cid']) : 0;
	$languageId = isset($_POST['languageId']) ? intval($_POST['languageId']) : 0;
    $code = isset($_POST['source']) ? $_POST['source'] : '';
	$enable_search = 1;

    $compilers = get_compilers();

    $lst = isset($_SESSION['lst']) ? intval($_SESSION['lst']) : 0;
    $smarty->assign('cid', $cid);

    if($cid) {
		$enable_search = 0;
        $contest = $db->getRowCached("select * from contest where id='$cid'");
        if(!$contest) {
            $smarty->assign('error_mesg', 'There is no such contest');
            $_POST['source'] = stripslashes($_POST['source']);
            $smarty->assign('submit', $_POST);
            $smarty->assign('compilers', $compilers);
            $smarty->display('submit.tpl');
            exit;
        }

        $now = time();
        if($now < $contest['startTime'] || $now > $contest['endTime']) {
            $smarty->assign('error_mesg', 'The Contest is not running');
            $_POST['source'] = stripslashes($_POST['source']);
            $smarty->assign('submit', $_POST);
            $smarty->assign('compilers', $compilers);
            $smarty->display('submit.tpl');
            exit;
        }

        $count = $db->getOneCached("select count(*) from contest_problem where contestId='$cid' and problemId='$problemId'");
        if(empty($count)) {
            $smarty->assign('error_mesg', 'There is no such problem in the contest');
            $_POST['source'] = stripslashes($_POST['source']);
            $smarty->assign('submit', $_POST);
            $smarty->assign('compilers', $compilers);
            $smarty->display('submit.tpl');
            exit;
        }

        $smarty->assign('contest', $contest);
    }else{
		/*
        $show = $db->getOne("select `show` from problem where problemId='$problemId'");
        if(!$show) {
            $smarty->assign('error_mesg', 'There is no such problem');
            $_POST['source'] = stripslashes($_POST['source']);
            $smarty->assign('submit', $_POST);
            $smarty->assign('compilers', $compilers);
            $smarty->display('submit.tpl');
            exit;
        }
		*/
    }

    if(time() - $lst < $submit_interval) {
        $smarty->assign('error_mesg', 'submit too fast,please wait a while'); 
        $_POST['source'] = stripslashes($_POST['source']);
        $smarty->assign('submit', $_POST);
        $smarty->assign('compilers', $compilers);
        $smarty->display('submit.tpl');
        die();
    }

	if(empty($_COOKIE['user_id'])) {
		$username = isset($_POST['username']) ? trim($_POST['username']) : '';
		$password = isset($_POST['password']) ? trim($_POST['password']) : '';
		if(user_login($username, $password, true)) {
			
		}else{
            $_POST['source'] = stripslashes($_POST['source']);
			$smarty->assign('submit', $_POST);
			$smarty->assign('compilers', $compilers);
			$smarty->display('submit.tpl');
			exit;
		}
    }

    if($cid && $contest['private'] && !$login_user['super']) {
	$smarty->assign('submit', $_POST);
	$smarty->assign('compilers', $compilers);
	$smarty->assign('error_mesg', 'You are not allowed to submit in the contest');
	$smarty->display('submit.tpl');
	exit;
    }

    if(empty($problemId)) {
        $smarty->assign('error_mesg', 'Please input the Problem Id');
        $_POST['source'] = stripslashes($_POST['source']);
        $smarty->assign('submit', $_POST);
        $smarty->assign('compilers', $compilers);
        $smarty->display('submit.tpl');
        exit;
    }

    $count = $db->getOneCached("select count(*) from problem where problemId='$problemId'");
    if($count <= 0) {
        $smarty->assign('error_mesg', 'There is no such problem');
        $_POST['source'] = stripslashes($_POST['source']);
        $smarty->assign('submit', $_POST);
        $smarty->assign('compilers', $compilers);
        $smarty->display('submit.tpl');
        exit;
    }

    $compiler = $db->getRowCached("select * from language where id='$languageId'");

    if(!$compiler) {
        $smarty->assign('error_mesg', 'There is no such compiler');
        $_POST['source'] = stripslashes($_POST['source']);
        $smarty->assign('submit', $_POST);
        $smarty->assign('compilers', $compilers);
        $smarty->display('submit.tpl');
        exit;
    }

    $code = trim($code);
    if(empty($code)) {
        $smarty->assign('error_mesg', 'Please input the source code');
        $smarty->assign('submit', $_POST);
        $smarty->assign('compilers', $compilers);
        $smarty->display('submit.tpl');
        exit;
    }

    $db->query("insert into status (problemId,languageId,authorId,judgeStatus,`case`,memory,time,submitTime,cid,poolStatus) values ('$problemId','$languageId','$login_user[user_id]','','0','0','0','".time()."','$cid','0')");
    $runId = $db->insert_id();
    $db->query("insert into source_code (runId,code) values ('$runId','$code')");

    $code = $db->getOne("select code from source_code where runId='$runId'");
    $filename = $runId . ".". $compiler['ext'];
    $fp = fopen($src_dir.$filename, "w+");
    fputs($fp, $code);
    fclose($fp);

    $_SESSION['lst'] = time(); 

    $db->query("update status set judgeStatus='Waiting' where runId='$runId'");

    $smarty->assign('runId', $runId);
	$smarty->assign('enable_search', $enable_search);
    $smarty->display('submit.tpl');
}
?>

