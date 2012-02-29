{include file="pageheader.tpl"}
<div class="main-div">
    <form action="compiler.php" method="post" name="theForm">
    <table width="100%" id="general-table">
        <tr>
            <td class="label">DisplayName</td>
            <td><input type="text" name="displayName" value="{$compiler.displayName}" size="30"/>
            </td>
        </tr>
        <tr>
            <td class="label">CompileCmd</td>
            <td><input type="text" name="compileCmd" value="{$compiler.compileCmd}" size="40" />
            </td>
        </tr>
        <tr>
            <td class="label">ExecuteCmd</td>
            <td><input type="text" name="executeCmd" value="{$compiler.executeCmd}" size="30" />
        </tr>
        <tr>
            <td class="label">Ext</td>
            <td><input type="text" name="ext" value="{$compiler.ext}" size="30" />
            </td>
        </tr>
    </table>
    <div class="button-div">
        <input type="submit" value="Submit" />
        <input type="reset" value="Reset" />
    </div>
    <input type="hidden" name="act" value="{$form_act}" />
    <input type="hidden" name="id" value="{$id}" />
    </form>
</div>
{include file="pagefooter.tpl"}

