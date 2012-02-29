<div class="rectdiv">
    <div class="cat-head">::Link</div>
    <ul>
        <li><a href="index.php">home</a></li>
        <li><a href="contest.php?act=home&cid={$contest.id}">problems</a></li>
        <li><a href="submit.php?cid={$contest.id}">submit</a></li>
        <li><a href="status.php?cid={$contest.id}">contest status</a></li>
        {if $login_user}
        <li><a href="status.php?cid={$contest.id}&authorId={$login_user.user_id}">my status</a></li>
        {/if}
        <li><a href="standing.php?cid={$contest.id}">standing</a></li>
    </ul>
</div>

{insert name="memberinfo"}


