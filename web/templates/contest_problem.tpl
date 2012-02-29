{include file="header.tpl"}
{literal}
<style type="text/css">
A.pa:link{color:#FFFFFF;text-decoration:none;}
A.pa:visited{color:#FFFFFF;text-decoration:none;}
A.pa:hover{color:#FFFFFF;text-decoration:underline;}
</style>
{/literal}

<div class="cat-head" style="text-align:center;padding:4px;font-weight:bold;margin-top:5px;padding:5px;">
<div style="float:left;font-size:15px;"><a class="pa" href="contest.php?act=home&cid={$cid}">[Contest Home Page]</a></div>
<table align="center" border="0" cellpadding="0" cellspacing="0" style="display:block;float:left"><tr>
{foreach from=$problems item=p}
<td width="75"><a class="pa" href="?cid={$cid}&pid={$p.problemId}">{$p.name}</a></td>
{/foreach}
</tr></table>
</div>

<h3 style="margin-bottom:0;text-align:center;">Problem {$this_name} -- {$problem_info.problemName}</h3>
<div style="text-align:center">Time Limit:{$problem_info.timelimit}ms&nbsp;&nbsp;&nbsp;Memory Limit:{$problem_info.memorylimit}KB{if $problem_info.spj}&nbsp;&nbsp;<font color="red">Special Judge</font>{/if}
	</div>
	<hr/>
	<div style="text-align:left">
<h4>Description</h4>
<div>{$problem_info.description}</div>
<h4>Input</h4>
<div>{$problem_info.input}</div>
<h4>Output</h4>
<div>{$problem_info.output}</div>
<h4>Sample Input</h4>
<div><pre>{$problem_info.sample_input}</pre></div>
<h4>Sample Output </h4>
<div><pre>{$problem_info.sample_output}</pre></div>
<h4>Hint</h4>
<div>{$problem_info.hint}</div>
</div>
<hr>
<a href="contest.php?act=home&cid={$cid}">[Home]</a>
<a href="submit.php?proId={$problem_info.problemId}&cid={$cid}">[Submit your code]</a>
</div>
{literal}
<script type="text/javascript">
    var TEX_PHP_SRC = "tex.php";
</script>
<script type="text/javascript" src="tex.js"></script>
<script type="text/javascript">
gettex();
</script>
{/literal}
{include file="footer.tpl"}

