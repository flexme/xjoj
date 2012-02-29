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

$cache_id = $page;
$cache_id = sprintf('%X', crc32($cache_id));

if(!$smarty->is_cached('news.tpl', $cache_id)) {
    $total = $db->query("select count(*) from news");
    $total_pages = $total / $perpage;
    $total_pages = intval($total_pages);
    if($total % $perpage) {
        $total_pages++;
    }

    if($page > $total_pages) {
        $page = $total_pages;
    } 

    $offset = ($page - 1) * $perpage;
    $news_list = $db->getAll("select * from news order by id desc limit $offset, $perpage");
    $smarty->assign('total', $total);
    $smarty->assign('total_pages', $total_pages);
    $smarty->assign('page', $page);
    $smarty->assign('news_list', $news_list);
}

$smarty->display('news.tpl', $cache_id);
?>

