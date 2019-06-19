    <?php if ($_SESSION['mostramsg']) : ?>
        <script>window.open("../geral/ctrlmensagens.php?tot=<?=$_SESSION['mostramsg']?>", "VtrlMensagem", "width=420,height=300,menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes");</script>
    <?php endif; unset($_SESSION['mostramsg']); ?>
    