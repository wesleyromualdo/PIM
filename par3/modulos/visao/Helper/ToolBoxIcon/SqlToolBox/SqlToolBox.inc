<a>
    <i class="fa fa-database toolboxsql" id="<?= $this->id;?>"></i>
    <input type="hidden" id="input-<?= $this->id;?>" value="<pre><?= $this->sql?></pre>">
</a>
<script>
    $('.toolboxsql').on('click',function() {
        var id = $(this).attr('id');
        console.log(id);
        var sql = $('#input-'+id).val();
        var modal = $('#modal_bootstrap_v1');
        modal.find('#conteudo-modal').html(sql);
        modal.find('#titulo-modal').html('SQL');
        modal.modal();
    });
</script>