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
    $ur_here = 'Compiler List';
    $smarty->assign('ur_here', $ur_here);

    $action_link = array('href' => 'compiler.php?act=add', 'text' => 'Add Compiler');

    $smarty->assign('action_link', $action_link);

    $compilers = $db->getAll("select * from language");
    $smarty->assign('compilers', $compilers);

    $smarty->display('compiler_list.tpl');
}

elseif ($_REQUEST['act'] == 'del') {
    $id = isset($_GET['id']) ? intval($_GET['id']) : 0;
    $db->query("delete from language where id='$id'");

    clear_all_files();
    header("Location: compiler.php?act=list");
}

elseif ($_REQUEST['act'] == 'add' || $_REQUEST['act'] == 'edit') {
    if($_REQUEST['act'] == 'edit') {
        $id = isset($_GET['id']) ? intval($_GET['id']) : 0;
        $compiler = $db->getRow("select * from language where id='$id'");
        if(!$compiler) {
            sys_msg('There is no such compiler');
            exit;
        }

        $smarty->assign('ur_here', 'Edit Compiler');
        $smarty->assign('compiler', $compiler);
        $smarty->assign('id', $id);
        $smarty->assign('form_act', 'update');
    }else{
        $smarty->assign('ur_here', 'Add Compiler');
        $smarty->assign('form_act', 'insert');
    }

    $smarty->display('compiler_info.tpl');
}

elseif ($_REQUEST['act'] == 'update' || $_REQUEST['act'] == 'insert') {
    $displayName = isset($_POST['displayName']) ? trim($_POST['displayName']) : '';
    $compileCmd = isset($_POST['compileCmd']) ? trim($_POST['compileCmd']) : '';
    $executeCmd = isset($_POST['executeCmd']) ? trim($_POST['executeCmd']) : '';
    $ext = isset($_POST['ext']) ? trim($_POST['ext']) : '';

    if(empty($displayName)) {
        sys_msg('Please input the DisplayName');
    }

    if(empty($executeCmd)) {
        sys_msg('Please input the ExecuteCmd');
    }

    if(empty($ext)) {
        sys_msg('Please input the Ext');
    }

    if($_REQUEST['act'] == 'update') {
        $id = isset($_POST['id']) ? intval($_POST['id']) : 0;
        $db->query("update language set displayName='$displayName',compileCmd='$compileCmd',executeCmd='$executeCmd',ext='$ext' where id='$id'");

        clear_all_files();

        $links[] = array('href' => 'compiler.php?act=edit&id='.$id, 'text' => 'Continue Editing Compiler');
        $links[] = array('href' => 'compiler.php?act=list', 'text' => 'Compiler List');
        sys_msg('Compiler Info Updated', 0, $links);
    }else{
        $db->query("insert into language (id,displayName,compileCmd,executeCmd,ext) values (NULL,'$displayName','$compileCmd','$executeCmd','$ext')");

        clear_all_files();

        $links[] = array('href' => 'compiler.php?act=add', 'text' => 'Continue Adding Compiler');
        $links[] = array('href' => 'compiler.php?act=list', 'text' => 'Compiler List');
        sys_msg('Compiler Info Added', 0, $links);
    }
}
?>

