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

$action = empty($_REQUEST['act']) ? 'register' : trim($_REQUEST['act']);

$smarty->assign('action', $action);

//the fllowing actions need the user be logined 
$check_login_action = array('logout', 'update', 'message', 'do_update');

if(in_array($action, $check_login_action) && !$login_user) {
    show_message('To do this operate, You must log in first!'); 
}

if($action == 'register') {
    $smarty->caching = 0;
    $smarty->assign('compilers', get_compilers());
	$smarty->display('register.tpl');
}

//register form is submitted
elseif ($action == 'act_register') {
	$username = isset($_POST['username']) ? htmlspecialchars(trim($_POST['username'])) : '';
	$email = isset($_POST['email']) ? trim($_POST['email']) : '';
	$languageId = isset($_POST['languageId']) ? intval($_POST['languageId']) : 0;
	$password = isset($_POST['password']) ? $_POST['password'] : '';
	$confirm_password = isset($_POST['confirm_password']) ? $_POST['confirm_password'] : '';
	$plan = isset($_POST['plan']) ? htmlspecialchars(trim($_POST['plan'])) : '';

	$check = true;

    $smarty->caching = 0;
	$smarty->assign('compilers', get_compilers());

	if(empty($username)) {
		$smarty->assign('error_msg', 'please provide your login username,Thank you!');
		$check = false;
	}

	if($check && empty($email)) {
		$smarty->assign('error_msg', 'please provide your email address,Thank you!');
		$check = false;
	}

        if($check && empty($languageId)) {
        	$smarty->assign('error_msg', 'please select your default compiler,Thank you!');
        	$check = false;
    	}

	if($check && !is_email($email)) {
		$smarty->assign('error_msg', "the email address <font color='blue'>{$email}</font> is not valid!");
		$check = false;
	}

	if($check && strlen($password) < 6) {
		$smarty->assign('error_msg', 'the password must be at leat 6 characters,Thank you!');
		$check = false;
	}

	if($check && $password != $confirm_password) {
		$smarty->assign('error_msg', 'password and confirm_password is not the same!');
		$_POST['password'] = '';
		$_POST['confirm_password'] = '';
		$check = false;
	}

	if(!$check) {
		$smarty->assign('user', $_POST);
		$smarty->display('register.tpl');
		exit;
	}

	$count = $db->getOne("select count(*) from author where username = '{$username}'");
	if($count > 0) {
		$check = false;
		$smarty->assign('error_msg', 'the username is already used');
		$smarty->assign('user', $_POST);
		$smarty->display('register.tpl');
		exit;
	}

	$count = $db->getOne("select count(*) from author where email = '{$email}'");
	if($count > 0) {
		$check = false;
		$smarty->assign('error_msg', 'the email address is already used');
		$smarty->assign('user', $_POST);
		$smarty->display('register.tpl');
		exit;
	}

	$password = md5($password);
	$db->query("insert into author (username,password,email,plan,defaultLanguageId) values ('$username','$password','$email','$plan','$languageId')");

	show_message('Your account is created! Now you can login and enjoy yourself!','Go To Home Page','index.php');
}

elseif ($action == 'login') {
	$username = isset($_POST['username']) ? htmlspecialchars(trim($_POST['username'])) : '';
	$password = isset($_POST['password']) ? $_POST['password'] : '';

    $smarty->caching = 0;

	user_login($username, $password);
	header('location: index.php');
}

elseif ($action == 'logout') {
	setcookie('user_id', '');
	setcookie('password', '');
	header('location: index.php');
}

elseif ($action == 'update') {
    $smarty->assign('title', 'Update Person Info');
    $smarty->assign('user_info', $login_user);
    $smarty->assign('compilers', get_compilers());
    $smarty->caching = 0;
    $smarty->display('user.tpl');
}

elseif ($action == 'detail') {
    $user_id = isset($_GET['authorId']) ? intval($_GET['authorId']) : 0;
    if(empty($user_id) && !empty($login_user)) {
        $user_id = $login_user['user_id'];
    }

    $user_info = $db->getRow("select * from author where user_id='$user_id'");
    if(!$user_info) {
        show_message('There is no such user');
    }

    $smarty->assign('user_info', $user_info);

    $solved_problems = $db->getAll("select distinct problemId from status where judgeStatus='Accepted' and authorId='$user_id' order by problemId asc");

    $smarty->assign('solved_problems', $solved_problems);
    $smarty->assign('title', 'User Statistics');
    $smarty->caching = 0;
    $smarty->display('user.tpl');
}

elseif ($action == 'do_update') {
    $email = isset($_POST['email']) ? trim($_POST['email']) : '';
    $languageId = isset($_POST['languageId']) ? intval($_POST['languageId']) : 0;
    $password = isset($_POST['password']) ? $_POST['password'] : '';
    $old_password = isset($_POST['old_password']) ? $_POST['old_password'] : '';
    $plan = isset($_POST['plan']) ? htmlspecialchars(trim($_POST['plan'])) : '';

    $smarty->caching = 0;
    $check = true;

	if($check && empty($email)) {
		$smarty->assign('error_msg', 'please provide your email address,Thank you!');
		$check = false;
	}

	if($check && !is_email($email)) {
		$smarty->assign('error_msg', "the email address <font color='blue'>{$email}</font> is not valid!");
		$check = false;
	}

	if($check && strlen($password) > 0 && strlen($password) < 6) {
		$smarty->assign('error_msg', 'the password must be at leat 6 characters,Thank you!');
		$check = false;
	}

	if(!$check) {
		$smarty->assign('user_info', $login_user);
        $smarty->assign('action', 'update');
		$smarty->display('user.tpl');
		exit;
    }

    if($email != $login_user['email']) {
        $count = $db->getOne("select count(*) from author where email = '{$email}'");
	    if($count > 0) {
		    $check = false;
		    $smarty->assign('error_msg', 'the email address is already used');
		    $smarty->assign('user_info', $login_user);
            $smarty->assign('action', 'update');
		    $smarty->display('user.tpl');
		    exit;
	    }
    }

    if(!empty($password)) {
        if($login_user['password'] != md5($old_password)) {
            $smarty->assign('error_msg', 'Old password is wrong'); 
            $smarty->assign('user_info', $login_user);
            $smarty->assign('action', 'update');
            $smarty->display('user.tpl');
            exit;
        }else{
            $password = md5($password);
            $db->query("update author set password='$password' where user_id='$login_user[user_id]'");
        }
    }

    $db->query("update author set email='$email',plan='$plan',defaultLanguageId='$languageId' where user_id='$login_user[user_id]'");
    show_message('Your personal info is updated!');
}
?>

