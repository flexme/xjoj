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

$problemId = isset($_GET['proId']) ? intval($_GET['proId']) : 0;
$order = isset($_GET['order']) ? trim($_GET['order']) : 'time';

if(!in_array($order, array('time', 'memory'))) {
    $order = 'time';
}

$cache_id = $problemId . '-' . $order;
$cache_id = sprintf('%X', crc32($cache_id));

if(!$smarty->is_cached('problemStatus.tpl', $cache_id)) {
    $next_order = '';
    $next_order_desc = '';
    if($order == 'time') {
        //first sort by time,and then memory
        $sort_order = 'time, it.memory';
        $next_order= 'memory';
        $next_order_desc = 'Run Memory';
    } else {
        //first sort by memory,and then time
        $sort_order = 'memory , it.time';
        $next_order = 'time';
        $next_order_desc = 'Run Time';
    }

    $sql = "SELECT * FROM (".
	   "SELECT ri.*, au.username, l.displayName ".
           "FROM (".
                "SELECT i . * ".
                "FROM status AS i ".
                "INNER JOIN (".

                    "SELECT min( a.runId ) AS runId ".
                    "FROM (".

                        "SELECT s.authorId, s.runId ".
                        "FROM status AS s ".
                        "INNER JOIN (".

                            "SELECT min( it.{$order} ) AS {$order}, it.authorId ".
                            "FROM status AS it ".
                            "WHERE it.problemId = {$problemId} ". 
                            "AND it.judgeStatus = 'Accepted' ". 
                            "GROUP BY it.authorId ".
                            "ORDER BY {$sort_order} ".
                            "LIMIT 0 , {$top_show} ".
                        ") AS t ON s.authorId = t.authorId ".
                        "AND s.{$order} = t.{$order} ".
                        "AND s.problemId = {$problemId} ".
			"AND s.judgeStatus = 'Accepted' ".
                    ") AS a ".
                    "GROUP BY a.authorId ".
                ") AS r ON i.runId = r.runId ".
            ") AS ri ". 
            "INNER JOIN author AS au ON au.user_id = ri.authorId ".
            "INNER JOIN language AS l ON l.id = ri.languageId".
	    ") AS e ORDER BY e.{$order}";

    $status = $db->getAll($sql);
    foreach($status as &$s) {
        $s['total'] = $db->getOne("SELECT count(*) FROM status ".
                                  " WHERE judgeStatus = 'Accepted' ".
                                  " AND problemId = '{$problemId}' ".
                                  " AND authorId = '$s[authorId]'");
    }

    $problemName = $db->getOneCached("SELECT problemName FROM problem WHERE problemId='{$problemId}'");

    $needs_count = array('AC' => 'Accepted',
        'WA' => 'Wrong Answer',
        'PE' => 'Presentation Error',
        'FPE' => 'Floating Point Error',
        'SF' => 'Runtime Error(SEGMENT)',
        'RE' => 'RUntime Error',
        'TLE' => 'Time Limit Exceeded',
        'MLE' => 'Memory Limit Exceeded',
        'CE' => 'Compilation Error');

    $counts = array();
    foreach($needs_count as $key => $value) {
        $counts[$key]['total'] = $db->getOne("SELECT COUNT(*) FROM status ".
                                    " WHERE judgeStatus = '$value' ".
                                    " AND problemId = '$problemId' ");
    }
    $counts['total'] = $db->getOne("SELECT COUNT(*) FROM status WHERE problemId = '$problemId'");
    foreach($needs_count as $key => $value) {
        if($counts['total'] > 0) {
            $counts[$key]['percent'] = intval($counts[$key]['total'] * 100 / $counts['total']);
        }else{
            $counts[$key]['percent'] = 0;
        }
    }

    $smarty->assign('problemId', $problemId);
    $smarty->assign('problemName', $problemName);
    $smarty->assign('status', $status);
    $smarty->assign('counts', $counts);
    $smarty->assign('needs_count', $needs_count);
    $smarty->assign('next_order', $next_order);
    $smarty->assign('next_order_desc', $next_order_desc);
}

$smarty->display('problemStatus.tpl', $cache_id);
?>

