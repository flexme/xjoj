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

$db_host = 'localhost';
$db_user = 'root';
$db_password = 'woshi1988';
$db_name = 'oj';
$client_info_host = 'localhost'; 
$client_info_port = 1215;

$timezone = 'Asia/Shanghai';
$cookie_expire = 24;
$perpage = 15;                      
$top_show = 20;
//提交程序的时间间隔
$submit_interval = 5;

//是否每隔一段时间更新一下比赛的排名榜
$is_static_board = false;

//是否比赛结束前1个小时封闭排名榜
$is_close_board = true;

//一定要以/结尾
$src_dir = '/usr/local/xjoj/data/src/';
?>
