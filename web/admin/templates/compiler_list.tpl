{include file="pageheader.tpl"}

<div class="list-div" id="listDiv">
<table cellspacing="1" cellpadding="3">
    <tr>
        <th>ID</th>
        <th>DisplayName</th>
        <th>CompileCmd</th>
        <th>Ext</th>
        <th>Operate</th>
    </tr>
    {foreach from=$compilers item=compiler}
    <tr>
        <td>{$compiler.id}</td>
        <td>{$compiler.displayName}</td>
        <td>{$compiler.compileCmd}</td>
        <td>{$compiler.ext}</td>
        <td align="center">
        <a title="edit" href="compiler.php?act=edit&id={$compiler.id}"><img src="images/icon_edit.gif" width="16" height="16" border="0" /></a>
        <a title="delete" href="compiler.php?act=del&id={$compiler.id}"><img src="images/icon_drop.gif" width="16" height="16" border="0" /></a>
        </td>
    </tr>
    {foreachelse}
    <tr><td class="no-records" colspan="5">No Records</td></tr>
    {/foreach}
</table>
</div>

{include file="pagefooter.tpl"}

