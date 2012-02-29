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

$action = isset($_GET['act']) ? trim($_GET['act']) : 'show_volumes';

$smarty->assign('action', $action);

if($action == 'show_volumes') {
    $cache_id = 'show_volumes';
    $cache_id = sprintf('%X', crc32($cache_id));

    if(!$smarty->is_cached('problems.tpl', $cache_id)) {
    	$count = $db->getOne("select count(*) from problem where `show`='1'");
	    $volumes = intval($count / 100) + (($count % 100) ? 1 : 0);
	    for($i = 1; $i <= $volumes; $i++) {
		    $row = $db->getRow("select min(c.problemId) as minr,max(c.problemId) as maxr from (select problemId from problem where `show`='1' order by problemId limit ".(($i - 1) * 100) .",100) as c");
		    $volumes_range[$i] = $row;
	    }

		$assemb = $db->getAllCached("select * from assemb order by id desc");

	    $smarty->assign('volumes', $volumes);
        $smarty->assign('volumes_range', $volumes_range);
		$smarty->assign('assemb', $assemb);
    }
	$smarty->display('problems.tpl', $cache_id);
}

elseif ($action == 'show_problems') {
	$volume = isset($_GET['volume']) ? intval($_GET['volume']) : 1;
	if($volume <= 0) {
		$volume = 1;
    }

    $sql = "select p.problemId,p.problemName,p.spj,p.ac_total as accepted,p.total_submit as total,0 as solved from problem p where p.show='1' order by problemId limit ". (($volume-1)*100). ",100";
    $problems = $db->getAll($sql);

    if($login_user) {
        $ptotal = count($problems);
        $min_pid = $problems[0]['problemId'];
        $max_pid = $problems[$ptotal - 1]['problemId'];
        $solved = $db->getAll("select distinct problemId from status where authorId={$login_user['user_id']} and judgeStatus='Accepted' and problemId>='{$min_pid}' and problemId<='{$max_pid}' order by problemId");
        $scount = count($solved);
        $i = 0;
        $j = 0;
        while($i < $ptotal && $j < $scount) {
            $p1 = $problems[$i]['problemId'];
            $p2 = $solved[$j]['problemId'];
            if($p1 == $p2) {
                $problems[$i]['solved'] = 1;
                $i++;
                $j++;
                continue;
            }
            if($p1 < $p2)
                $i++;
            else
                $j++;
        }
    }

    $smarty->assign('volume', $volume);
    $smarty->assign('problems', $problems);
    $smarty->caching = 0;
	$smarty->display('problems.tpl');
}

else if($action == 'show_asm_problems') {
	$assembId = isset($_GET['assembId']) ? intval($_GET['assembId']) : 0;
    $order = isset($_GET['order']) ? trim($_GET['order']) : '';

    $assemb_tile = $db->getOneCached("select title from assemb where id={$assembId}");
    $sql = "select p.problemId,p.problemName,p.spj,a.level,p.ac_total as accepted,p.total_submit as total,0 as solved from assemb_problem a inner join problem p on p.problemId=a.problemId and p.show='1' where a.assembId={$assembId} order by a.problemId";
    $problems = $db->getAll($sql);

    if($login_user) {
        $ptotal = count($problems);
        $min_pid = $problems[0]['problemId'];
        $max_pid = $problems[$ptotal - 1]['problemId'];
        $solved = $db->getAll("select distinct problemId from status where authorId={$login_user['user_id']} and judgeStatus='Accepted' and problemId>='{$min_pid}' and problemId<='{$max_pid}' order by problemId");
        $scount = count($solved);
        $i = 0;
        $j = 0;
        while($i < $ptotal && $j < $scount) {
            $p1 = $problems[$i]['problemId'];
            $p2 = $solved[$j]['problemId'];
            if($p1 == $p2) {
                $problems[$i]['solved'] = 1;
                $i++;
                $j++;
                continue;
            }
            if($p1 < $p2)
                $i++;
            else
                $j++;
        }
    }

    if($order == 'level') {
        usort($problems, "level_cmp");
    }

    $smarty->assign('assemb_title', $assemb_tile);
    $smarty->assign('assembId', $assembId);
    $smarty->assign('problems', $problems);
    $smarty->caching = 0;
	$smarty->display('problems.tpl');
}

function level_cmp($p1, $p2) {
    if($p1['level'] == $p2['level'])
        return 0;

    return $p1['level'] < $p2['level'] ? -1 : 1;
}
?>

