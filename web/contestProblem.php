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

$cid = isset($_GET['cid']) ? intval($_GET['cid']) : 0;
$pid = isset($_GET['pid']) ? intval($_GET['pid']) : 0;

$smarty->caching = 2;
$smarty->cache_lifetime = -1;

$count = $db->getOneCached("select count(*) from contest_problem where problemId='$pid' and contestId='$cid'");
if(empty($count)) {
    show_message('Wrong cid and pid');
    exit;
}

$contest = $db->getRowCached("select * from contest where id='$cid'");
if(time() < $contest['startTime']) {
    show_message('There is no such problem');
}

$cache_id = $cid . '-'. $pid;
$cache_id = sprintf('%X', crc32($cache_id));

if(!$smarty->is_cached('contest_problem.tpl', $cache_id)) {
    $problems = $db->getAllCached("select c.*,p.problemName from contest_problem c  inner join problem p on p.problemId=c.problemId where contestId='$cid' order by sort_order");
    foreach($problems as $key => &$p) {
        $p['name'] = chr(65 + $key);
        if($p['problemId'] == $pid) {
            $smarty->assign('this_name', $p['name']);
        }
    }

    $problem = $db->getRowCached("select * from problem where problemId='$pid'");

    $smarty->assign('problems', $problems);
    $smarty->assign('problem_info', $problem);
    $smarty->assign('cid', $cid);
}

$smarty->display('contest_problem.tpl', $cache_id);

?>

