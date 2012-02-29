{include file="header.tpl"}
<h3 style="margin-bottom:0;text-align:center;">{$problem_info.problemName}</h3>
<div style="text-align:center">Time Limit:{$problem_info.timelimit}ms&nbsp;&nbsp;&nbsp;Memory Limit:{$problem_info.memorylimit}KB{if $problem_info.spj}&nbsp;&nbsp;<font color="red">Special Judge</font>{/if}
	</div>
<div>
<a href="../index.php">[Home]</a>
<a href="../submit.php?proId={$problem_info.problemId}">[Submit you code]</a>
<a href="../problemStatus.php?proId={$problem_info.problemId}">[Status]</a>
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
<a href="../index.php">[Home]</a>
<a href="../submit.php?proId={$problem_info.problemId}">[Submit you code]</a>
<a href="../problemStatus.php?proId={$problem_info.problemId}">[Status]</a>
</div>
{literal}
<script type="text/javascript">
    var TEX_PHP_SRC = "../tex.php";
</script>
<script type="text/javascript" src="../tex.js"></script>
<script type="text/javascript">
gettex();
</script>
{/literal}
{include file="html_footer.tpl"}

