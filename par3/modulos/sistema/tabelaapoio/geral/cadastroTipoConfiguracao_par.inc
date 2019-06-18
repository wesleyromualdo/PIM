<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="form" id="form-tipo-configuracao" class="form form-horizontal">
            <div class="modal-content animated flipInY">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><i class="fa fa-close"></i></button>
                    <h3>Adicionar Tipo de Configuração</h3>
                </div>
                <div class="ibox-content">
                    <?php echo $controllerTipoConfiguracoes->formNovoTipoConfiguracao($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-10 col-md-offset-10 col-lg-offset-10">
                        <button class="btn btn-success" id="btn-salvar-tipoConf" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>
    jQuery(function () {
        jQuery('#btn-salvar-tipoConf').click(function (evt) {
            evt.preventDefault();
            $('#form-tipo-configuracao').isValid(salvar);
        });

        function salvar(isValid) {
            if (isValid) {
                window.location.assign(window.location.href + "&req=salvarTipoConf&" + jQuery('#form-tipo-configuracao').serialize());
            }
            return false;
        }
    });

</script>