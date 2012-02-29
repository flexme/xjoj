{include file="pageheader.tpl"}
{literal}
<style text="text/css">
.label{width:20% !important;}
</style>
{/literal}
<div class="main-div">
	<form action="problems.php" method="post" name="theForm" enctype="multipart/form-data">
	<table width="100%" id="general-table">
		<tr>
			<td class="label">Problem Name</td>
			<td>
				<input type="text" name="problemName" value="{$problem.problemName}" size="30" />
				<font color="red">*</font>
			</td>
		</tr>
		<tr>
			<td class="label">Time Limit</td>
			<td>
				<input type="text" name="timelimit" value="{$problem.timelimit}" /> MS
				<font color="red">*</font>
			</td>
		</tr>
		<tr>
			<td class="label">Memory Limit</td>
			<td>
				<input type="text" name="memorylimit" value="{$problem.memorylimit}" /> KB
				<font color="red">*</font>
			</td>
		</tr>
		<tr>
			<td class="label">SPJ</td>
			<td>
				<input type="checkbox" name="spj" value="1" {if $problem.spj}checked="checked"{/if} />
			</td>
		</tr>
		<tr>
			<td class="label">SHOW</td>
			<td>
				<input type="checkbox" name="show" value="1" {if $problem.show}checked="checked"{/if} />
			</td>
		</tr>
		<tr>
			<td class="label">Description</td>
			<td>
			    {$html_editor}
			</td>
		</tr>
		<tr>
			<td class="label">Input</td>
			<td>
                {$input_editor}
			</td>
		</tr>
		<tr>
			<td class="label">Output</td>
			<td>
                {$output_editor}
			</td>
		</tr>
		<tr>
			<td class="label">Sample Input</td>
			<td>
				<textarea rows="4" cols="50" name="sample_input">{$problem.sample_input}</textarea>
			</td>
		</tr>
		<tr>
			<td class="label">Sample Output</td>
			<td>
				<textarea rows="4" cols="50" name="sample_output">{$problem.sample_output}</textarea>
			</td>
		</tr>
		<tr>
			<td class="label">Hint</td>
			<td>
                {$hint_editor}
			</td>
		</tr>
	</table>
	<div class="button-div">
		<input type="submit" value="Submit" class="button" />
		<input type="reset" value="Reset" class="button" />
	</div>
	<input type="hidden" name="act" value="{$form_act}" />
	<input type="hidden" name="problemId" value="{$problem.problemId}" />
	</form>
</div>
{include file="pagefooter.tpl"}
