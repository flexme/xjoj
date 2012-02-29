{if $full_page}
{include file="pageheader.tpl"}

<form method="post" action="" name="listForm">
	<div class="list-div" id="listDiv">
{/if}
<table cellpadding="3" cellspacing="1">
    <tr>
        <th>ID</th>
        <th width="50%">Title</th>
        <th>Operate</th>
    </tr>
    {foreach from=$assemb_list item=assemb}
    <tr>
        <td>{$assemb.id}</td>
        <td>{$assemb.title}</td>
        <td align="center">
        <a title="problems" href="assemb.php?act=problems&cid={$assemb.id}"><img src="images/icon_view.gif" width="16" height="16" border="0"/></a>
        <a title="edit" href="assemb.php?act=edit&id={$assemb.id}"><img src="images/icon_edit.gif" width="16" height="16" border="0"/></a>
	    <a href="javascript:;" onclick="listTable.remove({$assemb.id}, 'Sure to delete it?')" title="delete"><img src="images/icon_drop.gif" width="16" height="16" border="0"/></a></td>
    </tr>
    {foreachelse}
    <tr><td class="no-records" colspan="3">No Records</td></tr>
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

