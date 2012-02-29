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

function user_login($username = '', $password = '', $ret = false) {
	if(empty($username)) {
		if(!$ret) {
			show_message('Please enter your username!');
		}else{
			$GLOBALS['smarty']->assign('error_mesg', 'Please enter your username!');
			return false;
		}
	}

	if(empty($password)) {
		if(!$ret) {
			show_message('Please enter your password!');
		}else{
			$GLOBALS['smarty']->assign('error_mesg', 'Please enter your password!');
			return false;
		}
	}

	$password = md5($password);
	$user = $GLOBALS['db']->getRow("select * from author where username='$username' and password='$password'");
	if(empty($user['user_id'])) {
		if(!$ret) {
			show_message('Wrong username or password!');
		}else{
			$GLOBALS['smarty']->assign('error_mesg', 'Wrong username or password!');
			return false;
		}
	}

	setcookie('user_id', $user['user_id'], time() + $GLOBALS['cookie_expire'] * 24 * 60 * 60);
	setcookie('password', $user['password'], time() + $GLOBALS['cookie_expire'] * 24 * 60 * 60);
	$GLOBALS['login_user'] = $user;
	$GLOBALS['smarty']->assign('login_user', $user);
	return true;
}

function get_compilers() {
	$rows = $GLOBALS['db']->getAllCached("select * from language");
	$compilers = array();
	foreach($rows as $row) {
		$compilers[$row['id']] = $row['displayName'];
	}
	return $compilers;
}

function get_contest_status($contest) {
    $nowtime = time();
    if($contest['startTime'] > $nowtime) {
        return 'Pending';
    } else if($contest['endTime'] < $nowtime) {
        return 'Ended';
    } else {
        return '<font color="red">Running</font>';
    }
}

function insert_memberinfo() {
    if($_COOKIE['user_id']) {
        //已经登录
        return $GLOBALS['smarty']->fetch('member.tpl');
    }else{
        return $GLOBALS['smarty']->fetch('login.tpl');
    }
}

function insert_time() {
	return date("Y-m-d H:i:s");
}

