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
require "init.php";

$_REQUEST['act'] = empty($_REQUEST['act']) ? 'list' : trim($_REQUEST['act']);

if($_REQUEST['act'] == 'list') {
    $ur_here = 'News List';
    $smarty->assign('ur_here', $ur_here);

    $action_link = array('href' => 'news.php?act=add', 'text' => 'Add News');
    $smarty->assign('action_link', $action_link);

    $news_list = news_list();

    $smarty->assign('news_list', $news_list['news']);
    $smarty->assign('filter', $news_list['filter']);
    $smarty->assign('record_count', $news_list['record_count']);
    $smarty->assign('page_count', $news_list['page_count']);
    $smarty->assign('full_page', 1);
    $smarty->display('news_list.tpl');
}

elseif ($_REQUEST['act'] == 'add' || $_REQUEST['act'] == 'edit') {
    include_once(ROOT_PATH.'fckeditor/fckeditor.php');

    if($_REQUEST['act'] == 'add') {
        $smarty->assign('ur_here', 'Add News');
        $smarty->assign('form_act', 'insert');
        $smarty->assign('html_editor', create_html_editor('content', '', true));
    }else{
        $id = isset($_GET['id']) ? intval($_GET['id']) : 0;
        $news = $db->getRow("select * from news where id='$id'");
        if(!$news) {
            sys_msg('There is no such news');
            exit;
        }

        $smarty->assign('ur_here', 'Edit News');
        $smarty->assign('news', $news);
        $smarty->assign('id', $id);
        $smarty->assign('form_act', 'update');
        $smarty->assign('html_editor', create_html_editor('content', $news['content'], true));
    }

    $smarty->display('news_info.tpl');
}

elseif ($_REQUEST['act'] == 'update' || $_REQUEST['act'] == 'insert') {
    $title = isset($_POST['title']) ? htmlspecialchars(trim($_POST['title'])) : '';
    $content = isset($_POST['content']) ? trim($_POST['content']) : '';
    $nowtime = time();

    if(empty($title) || empty($content)) {
        sys_msg('Please provide full information');
    }

    $links[] = array('text' => 'News List', 'href' => 'news.php?act=list');

    if($_REQUEST['act'] == 'insert') {
        $sql = "insert into news (title,content,addtime) values ('{$title}', '{$content}','{$nowtime}')";
        $db->query($sql);
        clear_all_files();

        sys_msg('News is added!', 0, $links);
    }else{
        $id = isset($_POST['id']) ? intval($_POST['id']) : 0;
        $sql = "update news set title='$title',content='$content' where id='$id'";
        $db->query($sql); 
        clear_all_files();

        sys_msg('News is updated!', 0, $links);
    }
}

elseif ($_REQUEST['act'] == 'query') {
    $news_list = news_list();

    $smarty->assign('news_list', $news_list['news']);
    $smarty->assign('filter', $news_list['filter']);
    $smarty->assign('record_count', $news_list['record_count']);
    $smarty->assign('page_count', $news_list['page_count']);

    make_json_result($smarty->fetch('news_list.tpl'), '',
        array('filter' => $news_list['filter'], 'page_count' => $news_list['page_count']));
}

elseif ($_REQUEST['act'] == 'remove') {
    $id = intval($_REQUEST['id']);

    $db->query("delete from news where id='$id'");
    clear_all_files();

    $url = 'news.php?act=query&' . str_replace('act=remove', '', $_SERVER['QUERY_STRING']);
    header("Location: $url\n");
    exit;
}

function news_list() {
    $sql = "select count(*) from news";
    $filter['record_count'] = $GLOBALS['db']->getOne($sql);
    
    $filter = page_and_size($filter);
    
    $sql = "select * from news order by id desc limit $filter[start],$filter[page_size]";

    $rows = $GLOBALS['db']->getAll($sql);

    return array('news' => $rows, 'filter' => $filter, 'page_count' => $filter['page_count'], 'record_count' => $filter['record_count']);
}

