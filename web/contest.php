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

$_REQUEST['act'] = isset($_REQUEST['act']) ? trim($_REQUEST['act']) : 'list';

if($_REQUEST['act'] == 'list') {
    $cache_id = 'contest-list';
    $cache_id = sprintf('%X', crc32($cache_id));

    if(!$smarty->is_cached('contest_list.tpl', $cache_id)) {
        $contest_list = $db->getAllCached("select * from contest order by startTime desc");
        foreach($contest_list as &$contest) {
            $contest['status'] = get_contest_status($contest);
        }

        $smarty->assign('contest_list', $contest_list);
    }

    $smarty->display('contest_list.tpl', $cache_id);
}

elseif ($_REQUEST['act'] == 'home') {
    $cid = isset($_REQUEST['cid']) ? intval($_REQUEST['cid']) : 0;
    $contest = $db->getRowCached("select * from contest where id='$cid'");
    if(!$contest) {
        show_message("There is no such contest");
    }

    $contest['status'] = get_contest_status($contest);

    if($contest['status'] != 'Pending') {
        $problems = $db->getAllCached("select c.*,p.problemName from contest_problem c  inner join problem p on p.problemId=c.problemId where contestId='$cid' order by sort_order");
        foreach($problems as $key => &$p) {
            $p['name'] = chr(65 + $key);
        }
        $smarty->assign('problems', $problems);
    }

    $smarty->caching = 0;
    $smarty->assign('contest', $contest);
    $smarty->display('contest_home.tpl');
}
?>

