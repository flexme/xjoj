{include file="pageheader.tpl"}
<div class="main-div">
    <form action="user.php" method="post" name="theForm">
        <table width="100%">
        <tr>
            <td class="label">Username:</td>
            <td><input type="text" name="username" value="{$user.username}" />
        </tr>
        {if $form_act eq update}
        <tr>
            <td class="label">Old Password</td>
            <td><input type="password" name="old_password" /></td>
        </tr>
        {/if}
        <tr>
            <td class="label">{if $form_act eq update}New{/if}Password:</td>
            <td><input type="password" name="password" />
        </tr>
        </table>
        <div class="button-div">
            <input type="submit" value="Submit" class="button" />
            <input type="reset" value="Reset" class="button" />
        </div>
        <input type="hidden" name="act" value="{$form_act}" />
        <input type="hidden" name="user_id" value="{$user.user_id}" />
    </form>
</div>
{include file="pagefooter.tpl"}

