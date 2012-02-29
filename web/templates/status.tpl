{include file="header.tpl"}
	<div id="main-left">
        {if $query.cid}
        {include file="contest_left.tpl"}
        {else}
		{include file="main_left.tpl"}
        {/if}
	</div>
	<div id="main-right">
		<div class="rectdiv">
		<div class="cat-head">::Status Online</div>
		<div style="text-align:center;padding:10px 0;">
			<h4 style="margin:0;padding:0;">{$smarty.now|date_format:"%Y-%m-%d %H:%M:%S"}</h4>
			<table width="100%" align="center" style="font-size:13px;">
			<tr>
            <td width="78%">
{if $enable_search}
                <form action="status.php" method="get">
                <table align="left">
                <tr><td>
                Problem ID:<input type="text" name="proId" value="{$query.proId}" style="width:30px;" />
                User ID:<input type="text" name="author" value="{$query.author}" style="width:70px;"/>
                Result:
                <select name="status" style="width:130px;">
                    <option value="">All</option>
                    <option value="Accepted" {if $query.status eq Accepted}selected{/if}>Accepted</option>
                    <option value="Presentation Error" {if $query.status eq 'Presentation Error'}selected{/if}>Presentation Error</option>
                    <option value="Time Limit Exceeded" {if $query.status eq 'Time Limit Exceeded'}selected{/if}>Time Limit Exceeded</option>
                    <option value="Memory Limit Exceeded" {if $query.status eq 'Memory Limit Exceeded'}selected{/if}>Memory Limit Exceeded</option>
                    <option value="Wrong Answer" {if $query.status eq 'Wrong Answer'}selected{/if}>Wrong Answer</option>
                    <option value="Runtime Error" {if $query.status eq 'Runtime Error'}selected{/if}>Runtime Error</option>
                    <option value="Runtime Error(SEGMENT)" {if $query.status eq 'Runtime Error(SEGMENT)'}selected{/if}>Runtime Error(SEGMENT)</option>
                    <option value="Output Limit Exceeded" {if $query.status eq 'Output Limit Exceeded'}selected{/if}>Output Limit Exceeded</option>
                    <option value="Floating Point Error" {if $query.status eq 'Floating Point Error'}selected{/if}>Floating Point Error</option>
                    <option value="Restricted Function" {if $query.status eq 'Restricted Function'}selected{/if}>Restricted Function</option>
                    <option value="Compilation Error" {if $query.status eq 'Compilation Error'}selected{/if}>Compilation Error</option>
                    <option value="Server Error" {if $query.status eq 'Server Error'}selected{/if}>Server Error</option>
                </select>
                Lang:
                <select name="langId">
                    <option value="">All</option>
                    {foreach from=$compilers item=compiler key=langId}
                    <option value="{$langId}" {if $query.langId eq $langId}selected{/if}>{$compiler}</option>
                    {/foreach}
                </select>
				<input type="hidden" name="enable_search" value="{$enable_search}" />
                <input type="submit" value="GO"/>
                </td></tr>
                </table>
                </form>
{/if}
            </td>
            <td>
				<table align="right">
				<tr><td>
					<a href="status.php?proId={$query.proId}&runId={$query.runId}&authorId={$query.authorId}&cid={$query.cid}&author={$query.author}&langId={$query.langId}&status={$query.status}&enable_search={$enable_search}">TOP</a>
					<a href="status.php?proId={$query.proId}&runId={$query.runId}&authorId={$query.authorId}&cid={$query.cid}&author={$query.author}&langId={$query.langId}&status={$query.status}&start={$firstRunId}&m=p&enable_search={$enable_search}">&lt;&lt;Prev</a>
					<a href="status.php?proId={$query.proId}&runId={$query.runId}&authorId={$query.authorId}&cid={$query.cid}&author={$query.author}&langId={$query.langId}&status={$query.status}&start={$lastRunId}&m=n&enable_search={$enable_search}">Next&gt;&gt;</a>
				</td></tr>
				</table>
            </td></tr>
            <tr><td colspan="2">
				<table border="0" cellpadding="4" cellspacing="1" width="99%" align="center">
				<tr style="font-size:15px;">
					<td width="7%" class="th">ID: </td>
					<td width="17%" class="th">TIME: </td>
					<td width="19%" class="th">NAME: </td>
					<td width="5%" class="th">TASK: </td>
                    <td class="th">LANGUAGE:</td>
					<td width="20%" class="th">STATUS: </td>
					<td width="5%" class="th">CASE: </td>
					<td width="7%" class="th">TIME: </td>
					<td width="7%" class="th">MEMORY: </td>
				</tr>
				<!-- {foreach from=$status item=row key=i} -->
				<tr class="st{$i%2}">
					<td>{$row.runId}</td>
					<td>{$row.submitTime|date_format:"%Y-%m-%d %H:%M:%S"}</td>
					<td><a href="user.php?act=detail&authorId={$row.authorId}">{$row.username}</a></td>
					<td>
                    <a href="{if $query.cid}contestProblem.php?cid={$contest.id}&pid={$row.problemId}{else}problem_html/{$row.problemId}.html{/if}">{if $query.cid}{$ename[$row.problemId]}{else}{$row.problemId}{/if}</a></td>
                    <td>{if $login_user.user_id eq $row.authorId}<a href="showCode.php?runId={$row.runId}" target="_blank">{$row.displayName}</a>{else}{$row.displayName}{/if}</td>
					<td>{if $row.judgeStatus eq Accepted}<font color="blue">{$row.judgeStatus}</font>{elseif $row.judgeStatus eq "Compilation Error"}<a href="showcompileinfo.php?runId={$row.runId}" target="_blank" style="color:red;">{$row.judgeStatus}</a>{else}<font color="red">{$row.judgeStatus}</font>{/if}</td>
					<td>{$row.case}</td>
					<td>{$row.time}MS</td>
					<td>{$row.memory}KB</td>
				</tr>
				<!-- {/foreach} -->
				</table>
			</td></tr></table>
		</div>
	</div>
	<div class="clear"></div>
</div>
{include file="footer.tpl"}

