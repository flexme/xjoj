{include file="pageheader.tpl"}
<script type="text/javascript" src="js/utils.js"></script>
<div class="main-div">
    <form action="assemb.php" method="post" name="theForm">
    <table width="100%" id="general-table">
    <tr>
        <td class="label">Assemble Title</td>
        <td>
            <input type="text" name="title" value="{$assemb.title}" size="30" />
        </td>
    </tr>
    </table>
    <div class="button-div">
        <input type="submit" value="Submit" class="button" />
        <input type="reset" value="Reset" class="button" />
    </div>
    <input type="hidden" name="act" value="{$form_act}" />
    <input type="hidden" name="id" value="{$assemb.id}" />
    </form>
</div>
{include file="pagefooter.tpl"}

