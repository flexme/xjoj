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

$err_info = $db->getOne("select errInfo from status where runId='$runId'");

$err_info = htmlspecialchars($err_info);
$smarty->assign('err_info', $err_info);
$smarty->display('show_compileinfo.tpl');
?>

