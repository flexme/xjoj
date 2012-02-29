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
include "includes/render.class.php";

$tex_code = isset($_GET['tex_code']) ? trim($_GET['tex_code']) : '';
$tex_code = str_replace("\\\\", "\\", $tex_code);

$render = new render();
$hash = md5($tex_code);
$full_name = $render->CACHE_DIR ."/$hash.png";

if(!is_file($full_name)) {
    $render->render_latex($tex_code, $hash);
}
header("Content-Type: image/jpg");
readfile($full_name);
exit;
?>

