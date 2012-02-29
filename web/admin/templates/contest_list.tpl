{if $full_page}
{include file="pageheader.tpl"}

<form method="post" action="" name="listForm">
	<div class="list-div" id="listDiv">
{/if}
<table cellpadding="3" cellspacing="1">
    <tr>
        <th>ID</th>
        <th width="30%">Title</th>
        <th>Start Time</th>
        <th>End Time</th>
        <th>Operate</th>
    </tr>
    {foreach from=$contest_list item=contest}
    <tr>
        <td>{$contest.id}</td>
        <td>{$contest.title}</td>
        <td>{$contest.startTime|date_format:"%Y-%m-%d %H:%M:%S"}</td>
        <td>{$contest.endTime|date_format:"%Y-%m-%d %H:%M:%S"}</td>
        <td align="center">
        <a title="problems" href="contest.php?act=problems&cid={$contest.id}"><img src="images/icon_view.gif" width="16" height="16" border="0"/></a>
        <a title="edit" href="contest.php?act=edit&id={$contest.id}"><img src="images/icon_edit.gif" width="16" height="16" border="0"/></a>
	    <a href="javascript:;" onclick="listTable.remove({$contest.id}, 'Sure to delete it?')" title="delete"><img src="images/icon_drop.gif" width="16" height="16" border="0"/></a></td>
    </tr>
    {foreachelse}
    <tr><td class="no-records" colspan="4">No Records</td></tr>
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

{include file="pagefooter.tpl"}
{/if}

