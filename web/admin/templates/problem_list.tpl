{if $full_page}
{include file="pageheader.tpl"}

<div class="form-div">
	<form action="javascript:searchProblems()" name="searchForm">
	<img src="images/icon_search.gif" width="26" height="22" border="0" alt="SEARCH" />
	Problem Name<input type="text" name="problemName" size="15" />
	Problem ID<input type="text" name="problemId" size="8" />
	 <input type="submit" value="Search" class="button" />
	</form>
</div>

<form method="post" action="" name="listForm">
	<div class="list-div" id="listDiv">
{/if}
<table cellpadding="3" cellspacing="1">
	<tr>
		<th>ID</th>
		<th>Problem Name</th>
		<th>Time Limit</th>
		<th>Memory Limit</th>
		<th>SPJ?</th>
        <th>Show?</th>
		<th>Operate</th>
	</tr>
	{foreach from=$problem_list item=problem}
	<tr>
		<td>{$problem.problemId}</td>
		<td><span onclick="listTable.edit(this,'edit_problem_name',{$problem.problemId})">{$problem.problemName}</span></td>
		<td align="center"><span onclick="listTable.edit(this,'edit_problem_time',{$problem.problemId})">{$problem.timelimit}</span> MS</td>
		<td align="center"><span onclick="listTable.edit(this,'edit_problem_memory',{$problem.problemId})">{$problem.memorylimit}</span> KB</td>
		<td align="center"><img src="images/{if $problem.spj}yes{else}no{/if}.gif" onclick="listTable.toggle(this,'toggle_spj',{$problem.problemId})"/></td>
        <td align="center"><img src="images/{if $problem.show}yes{else}no{/if}.gif" onclick="listTable.toggle(this,'toggle_show',{$problem.problemId})" /></td>
		<td align="center"><a title="view" target="_blank" href="../problem_html/{$problem.problemId}.html"><img src="images/icon_view.gif" width="16" height="16" border="0" /></a>
		<a title="edit" href="problems.php?act=edit&problemId={$problem.problemId}"><img src="images/icon_edit.gif" width="16" height="16" border="0"/></a>
		 <a href="javascript:;" onclick="listTable.remove({$problem.problemId}, 'Sure to delete it?')" title="delete"><img src="images/icon_drop.gif" width="16" height="16" border="0"/></a></td>
	</tr>
	{foreachelse}
	<tr><td class="no-records" colspan="6">No Records</td></tr>
	{/foreach}
</table>

<!-- paging -->
<table id="page-table" cellspacing="0">
  <tr>
    <td align="right" nowrap="true">
    {include file="page.tpl"}
    </td>
  </tr>
</table>

{if $full_page}
</div>
</form>

<script type="text/javascript">
listTable.recordCount = {$record_count};
listTable.pageCount = {$page_count};
{foreach from=$filter item=item key=key}
listTable.filter.{$key} = '{$item}';
{/foreach}
</script>

{literal}
<script language="JavaScript">
    function searchProblems()
    {
		listTable.filter['problemId'] = document.forms['searchForm'].elements['problemId'].value;
		listTable.filter['problemName'] = document.forms['searchForm'].elements['problemName'].value;
		listTable.filter['page'] = 1;
		listTable.loadList();
    }
</script>
{/literal}

{include file="pagefooter.tpl"}
{/if}
