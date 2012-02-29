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

error_reporting(E_ERROR);
if(!defined('IN_XJOJ')) {
	die('Access denied');
}

function sys_msg($msg_detail, $msg_type = 0, $links = array(), $auto_redirect = true)
{
    if (count($links) == 0)
    {
        $links[0]['text'] = 'Go Back';
        $links[0]['href'] = 'javascript:history.go(-1)';
    }

    $GLOBALS['smarty']->assign('ur_here',     'System Message');
    $GLOBALS['smarty']->assign('msg_detail',  $msg_detail);
    $GLOBALS['smarty']->assign('msg_type',    $msg_type);
    $GLOBALS['smarty']->assign('links',       $links);
    $GLOBALS['smarty']->assign('default_url', $links[0]['href']);
    $GLOBALS['smarty']->assign('auto_redirect', $auto_redirect);

    $GLOBALS['smarty']->display('message.tpl');

    exit;
}

function page_and_size($filter) {
	if (isset($_REQUEST['page_size']) && intval($_REQUEST['page_size']) > 0)
    {
        $filter['page_size'] = intval($_REQUEST['page_size']);
    }
	else
    {
        $filter['page_size'] = 15;
    }

	$filter['page'] = (empty($_REQUEST['page']) || intval($_REQUEST['page']) <= 0) ? 1 : intval($_REQUEST['page']);

	$filter['page_count'] = (!empty($filter['record_count']) && $filter['record_count'] > 0) ? ceil($filter['record_count'] / $filter['page_size']) : 1;

	if ($filter['page'] > $filter['page_count'])
	{
        $filter['page'] = $filter['page_count'];
    }

	$filter['start'] = ($filter['page'] - 1) * $filter['page_size'];

	return $filter;
}

function create_html_editor($input_name, $input_value = '', $is_ret = false, $width="500", $height='300')
{
    global $smarty;

    $editor = new FCKeditor($input_name);
    $editor->BasePath   = '../fckeditor/';
    $editor->ToolbarSet = 'Default';
    $editor->Width      = $width;
    $editor->Height     = $height;
    $editor->Value      = $input_value;
    $FCKeditor = $editor->CreateHtml();
	if($is_ret) return $FCKeditor;
    $smarty->assign('FCKeditor', $FCKeditor);
}

function make_json_response($content='', $error="0", $message='', $append=array())
{
	if(file_exists('../includes/cls_json.php')) {
		include_once('../includes/cls_json.php');
	}else{
		include_once('../../includes/cls_json.php');
	}
    $json = new JSON;

    $res = array('error' => $error, 'message' => $message, 'content' => $content);

    if (!empty($append))
    {
        foreach ($append AS $key => $val)
        {
            $res[$key] = $val;
        }
    }

    $val = $json->encode($res);

    exit($val);
}

function make_json_result($content, $message='', $append=array())
{
    make_json_response($content, 0, $message, $append);
}

function gen_problem_html($problem) {
	global $smarty;

	$ori_template_dir = $smarty->template_dir;
	$smarty->template_dir = ROOT_PATH . "templates";
	$smarty->assign('is_html', 1);

	$smarty->assign('problem_info', $problem);
	$fp = fopen(ROOT_PATH . "problem_html/". $problem['problemId'].".html", "w+");
	fwrite($fp, $smarty->fetch("problem.tpl"));
	fclose($fp);

	$smarty->template_dir = $ori_template_dir;
}

function gen_problem_by_id($problemId) {
	$problem = $GLOBALS['db']->getRow("select * from problem where problemId='$problemId'");
	if($problem) {
		if($problem['show'] > 0) {
			gen_problem_html($problem);
		}else{
			 unlink("../problem_html/". $problemId. ".html");
		}
	}
}

