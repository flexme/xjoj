{if $full_page}
{include file="pageheader.tpl"}

<form method="post" action="" name="listForm">
	<div class="list-div" id="listDiv">
{/if}
<table cellpadding="3" cellspacing="1">
    <tr>
        <th>ID</th>
        <th>Title</th>
        <th>Add Time</th>
        <th>Operate</th>
    </tr>
    {foreach from=$news_list item=news}
    <tr>
        <td>{$news.id}</td>
        <td>{$news.title}</td>
        <td>{$news.addtime|date_format:"%Y-%m-%d"}</td>
        <td align="center"><a title="edit" href="news.php?act=edit&id={$news.id}"><img src="images/icon_edit.gif" width="16" height="16" border="0"/></a>
		 <a href="javascript:;" onclick="listTable.remove({$news.id}, 'Sure to delete it?')" title="delete"><img src="images/icon_drop.gif" width="16" height="16" border="0"/></a></td>
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


