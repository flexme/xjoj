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

include_once ROOT_PATH."includes/cls_json.php";
$json = new JSON;
$result = array('error' => 0, 'err_msg' => '', 'content' => '');

if (empty($_REQUEST['act']))
{
    $_REQUEST['act'] = 'login';
}
else
{
    $_REQUEST['act'] = trim($_REQUEST['act']);
}

if ($_REQUEST['act'] == 'logout')
{
    $_SESSION['admin_id'] = '';

    $_REQUEST['act'] = 'login';
}

if ($_REQUEST['act'] == 'login') {
	header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
    header("Cache-Control: no-cache, must-revalidate");
    header("Pragma: no-cache");

	$smarty->display('login.tpl');
}

elseif ($_REQUEST['act'] == 'signin') {
	$_POST['username'] = isset($_POST['username']) ? trim($_POST['username']) : '';
    $_POST['password'] = isset($_POST['password']) ? trim($_POST['password']) : '';

	$sql = "select user_id,username,password from admin where username='". $_POST['username']. "' and password='" .md5($_POST['password'])."'";
	$row = $db->getRow($sql);

	if($row) {
		$_SESSION['admin_id'] = $row['user_id'];

		$db->query("update admin set last_login='". time(). "', last_ip='". real_ip(). "' where user_id='$_SESSION[admin_id]'");

		header('Location: ./index.php');

		exit;
	} else {
		sys_msg('Login Failed', 1);
	}
}

elseif ($_REQUEST['act'] == 'modify') {
    $user_id = isset($_REQUEST['user_id']) ? intval($_REQUEST['user_id']) : $_SESSION['admin_id'];
    $user = $db->getRow("select * from admin where user_id='$user_id'");

    $smarty->assign('ur_here', 'Edit Admin User');
    $action_link = array('href' => 'user.php?act=list', 'text' => 'Admin User List');
    $smarty->assign('action_link', $action_link);

    $smarty->assign('user', $user);
    $smarty->assign('form_act', 'update');
    $smarty->display('user_info.tpl');

}

elseif ($_REQUEST['act'] == 'add') {
    $smarty->assign('ur_here', 'Add Admin User');
    $action_link = array('href' => 'user.php?act=list', 'text' => 'Admin User List');
    $smarty->assign('action_link', $action_link);

    $smarty->assign('form_act', 'insert');
    $smarty->display('user_info.tpl');
}

elseif ($_REQUEST['act'] == 'insert') {
    $username = isset($_REQUEST['username']) ? trim($_REQUEST['username']) : '';
    $password = isset($_REQUEST['password']) ? trim($_REQUEST['password']) : '';

    if(empty($username)) {
        sys_msg('Please input the username');
    }
    if(empty($password)) {
        sys_msg('Please input the password');
    }

    $count = $db->getOne("select count(*) from admin where username='$username'");
    if($count > 0) {
        sys_msg('The username has already used');
    }

    $password = md5($password);
    $sql = "insert into admin (username,password) values ('$username','$password')";
    $db->query($sql);
    $links[] = array('href' => 'user.php?act=list', 'text' => 'Admin User List');
    $links[] = array('href' => 'user.php?act=add', 'text' => 'Continue Add Admin User');
    sys_msg('Admin Account has Created!', 0, $links);
}

elseif ($_REQUEST['act'] == 'update') {
    $user_id = isset($_POST['user_id']) ? intval($_POST['user_id']) : $_SESSION['user_id'];
    $username = isset($_POST['username']) ? trim($_POST['username']) : '';
    $old_password = isset($_POST['old_password']) ? trim($_POST['old_password']) : '';
    $password = isset($_POST['password']) ? trim($_POST['password']) : '';

    if(empty($username)) {
        sys_msg('Please input the username');
    }

    $sql = "update admin set username='$username'";

    if(!empty($password)) {
        $p = $db->getOne("select password from admin where user_id='$user_id'");
        if($p != md5($old_password)) {
            sys_msg('The old password is wrong');
        }

        $password = md5($password);
        $sql .= ",password='$password'";
    }

    $sql .= " where user_id='$user_id'";

    $db->query($sql);

    $links[] = array('href' => 'user.php?act=modify&user_id='. $user_id, 'text' => 'Continue Edit');
    $links[] = array('href' => 'user.php?act=list', 'text' => 'Admin User List');

    sys_msg('Admin User Info has updated!', 0, $links);

}

elseif ($_REQUEST['act'] == 'list') {
    $users = $db->getAll("select * from admin");
    $smarty->assign('users', $users);

    $smarty->assign('ur_here', 'Admin User List');
    $action_link = array('href' => 'user.php?act=add', 'text' => 'Add Admin User');
    $smarty->assign('action_link', $action_link);
    $smarty->display('user_list.tpl');
}

elseif ($_REQUEST['act'] == 'delete') {
    $user_id = isset($_REQUEST['user_id']) ? intval($_REQUEST['user_id']) : 0;
    $db->query("delete from admin where user_id='$user_id'");
    header('Location: user.php?act=list');
}

elseif ($_REQUEST['act'] == 'user_source') {
    $smarty->assign('ur_here', 'User Source');
    $smarty->display('user_source.tpl');
}

elseif ($_REQUEST['act'] == 'get_source') {
    $runId = isset($_REQUEST['runId']) ? intval($_REQUEST['runId']) : 0;
    $sql = "select c.code,s.judgeStatus,a.user_id,a.username,s.problemId,s.time,s.memory,l.displayName,l.ext from source_code c ".
            " inner join status s on s.runId = c.runId ".
            " inner join author a on a.user_id = s.authorId ".
            " inner join language l on l.id = s.languageId ".
            " where c.runId = ". $runId;
    $row = $db->getRow($sql);
    if(!$row) {
        $res['error'] = 1;
        $res['err_msg'] = "No such runId";
    }else{
        $row['code'] = htmlspecialchars($row['code']);
        $res['content'] = $row;
    }

    die($json->encode($res));
}
?>

