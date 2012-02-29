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

$contest = $db->getRowCached("select * from contest where id='$cid'");

$t = time();

if(!$contest) {
    show_message('There is no such contest');
}

if($is_static_board) {
    //每隔一段时间刷新下board
    $smarty->caching = 2;

    //每隔5分钟刷新一下board
    $smarty->cache_lifetime = 5 * 60;
} else {
    $smarty->caching = 0;
}

if($is_close_board) {
    //比赛结束前1个小时封闭排名榜
    if($t >= $contest['endTime'] - 60 * 60) {
        $smarty->caching = 2;
        $smarty->cache_lifetime = -1;
    }
}

if($t > $contest['endTime']) {
    $smarty->caching = 2;
    $smarty->cache_lifetime = -1;
}

if($t < $contest['startTime']) {
    $smarty->caching = 0;
}

if($smarty->caching > 0) {
    $cache_id = 'contest-'. $cid;
    $cache_id = sprintf('%X', crc32($cache_id));
}

if($smarty->caching == 0 || !$smarty->is_cached('standing.tpl', $cache_id)) {
    //problems list in the contest
    $problems = $db->getAllCached("select c.problemId from contest_problem c where contestId='$cid' order by sort_order");
    //the number of problems
    $problems_tot = count($problems);
    foreach($problems as $key => &$pp) {
        $pp['name'] = chr(65 + $key);
    }

    $users = $db->getAll("select s.user_id,u.username,u.truename,s.ac_total as acceptes,".
                        "(s.penalty-s.ac_total*$contest[startTime]) as penaltyTime from contest_standing s ".
                        "inner join author u on u.user_id=s.user_id where s.contest_id='$cid' ".
                        "order by s.ac_total desc,s.penalty asc");

    foreach($users as &$user) {
        $user['penaltyTime'] = getTime($user['penaltyTime']);

        $user['problems'] = array();
        $user_submit_problems = $db->getAll("select problemId,ac_time-$contest[startTime] as acTime,submits as totSubmit from contest_submit where user_id='$user[user_id]' and contest_id='$cid'"); 
        
        foreach($user_submit_problems as $up) {
            if($up['acTime'] > 0) {
                $user['problems'][$up['problemId']]['acTime'] = getTime($up['acTime']);
            }else{
                $user['problems'][$up['problemId']]['acTime'] = '';
            }
            $user['problems'][$up['problemId']]['totSubmit'] = $up['totSubmit'];
        }
    }

/*
//the list of users who have attended in the contest
$users = $db->getAll("select distinct s.authorId,a.username from status s inner join author a on a.user_id=s.authorId where s.cid='$cid'");

$solved = $db->getAll("select authorId,problemId,min(submitTime) as submitTime from status where cid='$cid' and judgeStatus='Accepted' group by authorId,problemId");
$solved_time = array();

foreach($solved as $s) {
    $solved_time[$s['authorId']][$s['problemId']] = $s['submitTime'];    
}
unset($solved);

$submits = $db->getAll("select authorId,problemId,count(*) as tot from status where cid='$cid' group by authorId,problemId");
$submit_times = array();

foreach($submits as $s) {
    $submit_times[$s['authorId']][$s['problemId']] = $s['tot'];
}
unset($submits);

foreach($users as &$user) {
    //$user['problems'] = array();
    $user['acceptes'] = 0;
    $user['penalty'] = 0;
    
    foreach($GLOBALS['problems'] as $p) {
        if(isset($solved_time[$user['authorId']][$p['problemId']])) {
            //AC
            $user['acceptes']++; 
            $user['problems'][$p['problemId']]['ac'] = 1;
            $ret =  $db->getOne("select count(*) as totSubmit from status where cid='$cid' and authorId='$user[authorId]' and submitTime<'". $solved_time[$user['authorId']][$p['problemId']]. "'");
            $user['problems'][$p['problemId']]['totSubmit'] = $ret;
            $user['problems'][$p['problemId']]['acTime'] = $solved_time[$user['authorId']][$p['problemId']] - $contest['startTime'];

            $user['penalty'] +=  $user['problems'][$p['problemId']]['acTime'] + 20 * 60 * $ret; 

            $user['problems'][$p['problemId']]['acTime'] = getTime($user['problems'][$p['problemId']]['acTime']);
        }else{
            //NOT AC
            @$user['problems'][$p['problemId']]['totSubmit'] = intval($submit_times[$user['authorId']][$p['problemId']]);
            $user['problems'][$p['problemId']]['ac'] = 0;
        }
    }

    $user['penaltyTime'] = getTime($user['penalty']);
}

usort($users, 'mysort');

function mysort(&$user1, &$user2) {
    if($user1['acceptes'] != $user2['acceptes']) {
        return $user1['acceptes'] > $user2['acceptes'] ? -1 : 1;
    }

    return $user1['penalty'] < $user2['penalty'] ? -1 : 1;
}

*/

    $snap_info = '';
    if($t >= $contest['startTime'] && $t <= $contest['endTime'] && $smarty->caching > 0) {
        $snap_info = "<font color='red' style='font-size:12px;'>[SNAPPED AT ". date('H:i:s')."]</font>";
    }
    $smarty->assign('snap_info', $snap_info);
    $smarty->assign('problems', $problems);
    $smarty->assign('contest', $contest);
    $smarty->assign('users', $users);
}

if($smarty->caching == 0) {
    $smarty->display('standing.tpl');
}else{
    $smarty->display('standing.tpl', $cache_id);
}

function getTime($sec) {
    $ret = "";
    $h = intval($sec / 3600);
    $sec -= $h * 3600;
    $m = intval($sec / 60);
    $sec -= $m * 60;
    return $ret.$h.":".$m.":".$sec;
}
?>

