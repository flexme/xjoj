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

$cache_id = 'index';
$cache_id = sprintf("%X", crc32($cache_id));

if(!$smarty->is_cached('index.tpl', $cache_id)) {
	$news_list = $db->getAllCached("select * from news order by id desc limit 0,3");
	$contest_list = $db->getAllCached("select * from contest order by startTime desc limit 0,3");
	$nowtime = time();
	foreach($contest_list as &$contest) {
		$contest['status'] = get_contest_status($contest);
	}

	$smarty->assign('news_list', $news_list);
	$smarty->assign('contest_list', $contest_list);
}
$smarty->display('index.tpl', $cache_id);
?>

