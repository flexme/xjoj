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

$page = isset($_GET['page']) ? intval($_GET['page']) : 1;
if($page < 1) {
    $page = 1;
}
$pagesize = 50;

/*
$cache_id = $page . '-' . $pagesize;
$cache_id = sprintf('%X', crc32($cache_id));
*/

//if(!$smarty->is_cached('rank.tpl', $cache_id)) {
    $smarty->caching = 0;
    $total_authors = $db->getOne("select count(*) from author");
    $total_pages = $total_authors / $pagesize;
    $total_pages = intval($total_pages);
    if($total_authors % $pagesize) {
        $total_pages++;
    }
    if($page > $total_pages) {
        $page = $total_pages;
    }
    
    $offset = ($page - 1) * $pagesize;

    /*
    $sql = "SELECT * ".
            "FROM author a ".
            "LEFT JOIN ( ".

                "SELECT s.authorId, count( * ) AS ac_submit ".
                "FROM ( ".

                    "SELECT DISTINCT authorId, problemId ".
                    "FROM status WHERE judgeStatus = 'Accepted' ".
                ") AS s ".
            "GROUP BY s.authorId ".
            ") AS ts ON a.user_id = ts.authorId ".
            "ORDER BY ts.ac_submit DESC , a.total_submit ASC ".
            "LIMIT $offset , $pagesize";*/

    $sql = "SELECT * FROM author ORDER BY ac_total DESC, total_submit ASC LIMIT $offset, $pagesize";
    $ranklist = $db->getAll($sql);
    $smarty->assign('ranklist', $ranklist);
    $smarty->assign('total_pages', $total_pages);
    $smarty->assign('page', $page);
    $smarty->assign('offset', $offset);
    $smarty->assign('total_authors', $total_authors);
//}

$smarty->display('rank.tpl'/*, $cache_id*/);
?>


