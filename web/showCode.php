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


$runId = isset($_GET['runId']) ? intval($_GET['runId']) : 0;

if(!$login_user) {
    show_message('You must login first');
    exit;
}

$row = $db->getRow("select c.code,l.ext,l.displayName,s.authorId,p.problemName,p.problemId,s.time,s.memory,s.judgeStatus from source_code c,status s,language l,problem p where c.runId=s.runId and s.languageId=l.id and s.problemId=p.problemId and c.runId='$runId'");

if(!$row) {
    show_message('Wrong runId');
}

if($row['authorId'] != $login_user['user_id']) {
    show_message("You can't view other's code");
}

$row['code'] = htmlspecialchars($row['code']);
$smarty->assign('info', $row);
$smarty->display('show_code.tpl');
?>

