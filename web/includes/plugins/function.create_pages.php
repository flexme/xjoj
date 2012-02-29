<?php
/**
 * Smarty plugin
 * @package Smarty
 * @subpackage plugins
 */


/**
 * Smarty {html_options} function plugin
 *
 * Type:     function<br>
 * Name:     html_options<br>
 * Input:<br>
 *           - name       (optional) - string default "select"
 *           - values     (required if no options supplied) - array
 *           - options    (required if no values supplied) - associative array
 *           - selected   (optional) - string default not set
 *           - output     (required if not options supplied) - array
 * Purpose:  Prints the list of <option> tags generated from
 *           the passed parameters
 * @link http://smarty.php.net/manual/en/language.function.html.options.php {html_image}
 *      (Smarty online manual)
 * @author Monte Ohrt <monte at ohrt dot com>
 * @param array
 * @param Smarty
 * @return string
 * @uses smarty_function_escape_special_chars()
 */
function smarty_function_create_pages($params, &$smarty)
{
	require_once $smarty->_get_plugin_filepath('shared','escape_special_chars');

	$page = null;
	$count = null;

	foreach($params as $_key => $_val) {
		switch($_key) {
			case 'page':
				$$_key = intval($_val);
				break;

			case 'count':
				$$_key = intval($_val);
				break;
		}
	}

	if (empty($page))
	{
		$page = 1;
	}

	if (!empty($count))
	{
		$str = "<option value='1'>1</option>";
        $min = min($count - 1, $page + 3);
        for ($i = $page - 3 ; $i <= $min ; $i++)
        {
			if ($i < 2)
            {
				continue;
			}
            $str .= "<option value='$i'";
            $str .= $page == $i ? " selected='true'" : '';
            $str .= ">$i</option>";
        }
        if ($count > 1)
        {
			$str .= "<option value='$count'";
			$str .= $page == $count ? " selected='true'" : '';
            $str .= ">$count</option>";
        }
	}
	else
    {
		$str = '';
	}

	return $str;
}
?>
