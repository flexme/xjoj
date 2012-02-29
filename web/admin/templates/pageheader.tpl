<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>{#webname#}{if $ur_here} - {$ur_here} {/if}</title>
<meta name="robots" content="noindex, nofollow">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link href="styles/general.css" rel="stylesheet" type="text/css" />
<link href="styles/main.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/utils.js"></script>
<script type="text/javascript" src="js/transport.js"></script>
<script type="text/javascript" src="js/listtable.js"></script>
</head>
<body>
<h1>
{if $action_link}
<span class="action-span"><a href="{$action_link.href}">{$action_link.text}</a></span>
{/if}
{if $action_link2}
<span class="action-span"><a href="{$action_link2.href}">{$action_link2.text}</a>&nbsp;&nbsp;</span>
{/if}
<span><a href="index.php?act=main">{#webname#}</a> {if $ur_here} - {$ur_here} {/if}</span>
</h1>