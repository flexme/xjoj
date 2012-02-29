{include file="pageheader.tpl"}
{literal}
<style text="text/css">
.label{width:20% !important;}
</style>
{/literal}
<div class="main-div">
    <form action="news.php" method="post" name="theForm" enctype="multipart/form-data">
    	<table width="100%" id="general-table">
		<tr>
			<td class="label">Title</td>
           	<td>
				<input type="text" name="title" value="{$news.title}" size="30" />
			</td>
		</tr> 
        <tr>
            <td class="label">Content</td>
            <td>
                {$html_editor}
            </td>
        </tr>
        </table>
        <div class="button-div">
            <input type="submit" value="Submit" class="button" />
            <input type="reset" value="Reset" class="button" />
        </div>
        <input type="hidden" name="act" value="{$form_act}" />
        <input type="hidden" name="id" value="{$news.id}" />
    </form>
</div>
{include file="pagefooter.tpl"}

