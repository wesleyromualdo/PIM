</div>


<div class="modal fade" id="modal-alert" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
    <div class="modal-dialog">
        <div class="modal-content text-center">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title" style="text-align:left">Aviso!</h4>
            </div>
            <div class="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-danger" data-dismiss="modal">Ok</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

<div class="modal fade" id="modal-confirm">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Aviso!</h4>
            </div>
            <div class="modal-body">
                <p></p>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Não</button>
                <button type="button" class="btn btn-primary">Sim</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->
</body>
</html>
<script language="JavaScript">
    /** Função de fazer com que o sistema informe que esta havendo uma requisição ajax */
    $(document).ajaxSend(function (e, jqxhr, settings) {
        jQuery("#loading").fadeIn();
    }).ajaxStop(function(){
        jQuery("#loading").fadeOut();
        attachInit();
        attachEvents();
    });

    /** Mensagem de carregando quando houver requisições em ajax */
    $.ajaxSetup({
        timeout: 0,
        error: function(xhr, status, error) {
            console.log("Ocorrência de erro no retorno AJAX: " + status + "\nError: " + error);
            jQuery("#loading").fadeOut();
            jQuery("#loading").fadeIn();

            setTimeout(function(){ jQuery("#loading").fadeOut();}, 1300);
        }
    });

    // -- Substituíndo o alert do browser.
    window.alert = function(texto)
    {
        jQuery('#modal-alert .modal-body').html(texto);
        jQuery('#modal-alert').modal();
    };
</script>

<?php
$mensagemCarregar = $_SESSION['mensagem_carregar'];
unset($_SESSION['mensagem_carregar']);
?>

<!-- Custom and plugin javascript -->
<script src="/zimec/public/temas/simec/js/plugins/slimscroll/jquery.slimscroll.min.js"></script>

<!-- Chosen -->
<script src="/zimec/public/temas/simec/js/plugins/chosen/chosen.jquery.js"></script>

<!-- JSKnob -->
<script src="/zimec/public/temas/simec/js/plugins/jsKnob/jquery.knob.js"></script>

<!-- Switcher -->
<script src="/zimec/public/temas/simec/js/plugins/bootstrap-switch/bootstrap-switch.js"></script>

<!-- NanoScroller -->
<script src="/zimec/public/temas/simec/js/plugins/nanoscroll/jquery.nanoscroller.min.js"></script>

<!-- FullCalendar -->
<script src="/zimec/public/temas/simec/js/plugins/fullcalendar/fullcalendar.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/fullcalendar/moment.min.js"></script>

<!-- Switchery -->
<script src="/zimec/public/temas/simec/js/plugins/switchery/switchery.js"></script>

<!-- JsTree -->
<script src="/zimec/public/temas/simec/js/plugins/jstree/jstree.min.js"></script>

<!-- iCheck -->
<script src="/zimec/public/temas/simec/js/plugins/iCheck/icheck.min.js"></script>

<!-- File Input -->
<script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput_locale_pt-BR.js"></script>

<!-- Summernote -->
<script src="/zimec/public/temas/simec/js/plugins/summernote/summernote.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/summernote/summernote-pt-BR.js"></script>

<!-- Flot -->
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.tooltip.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.spline.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.resize.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.pie.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.symbol.js"></script>

<!-- Dropzone -->
<script src="/zimec/public/temas/simec/js/plugins/dropzone/dropzone.js"></script>

<!-- Masked Input -->
<script src="/zimec/public/temas/simec/js/plugins/maskedinput/jquery.maskedinput.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/jquery.inputmask.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.numeric.extensions.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.regex.extensions.min.js"></script>

<!-- Menu -->
<script src="/zimec/public/temas/simec/js/plugins/metisMenu/jquery.metisMenu.js"></script>

<!-- Datatables -->
<script src="/zimec/public/temas/simec/js/plugins/dataTables/jquery.dataTables.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/dataTables/dataTables.bootstrap.js"></script>

<!-- Bootstrap Tour -->
<script src="/zimec/public/temas/simec/js/plugins/bootstrap-tour/bootstrap-tour.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/bootstrap-tour/bootstrap-tour-standalone.min.js"></script>

<!-- Gritter -->
<script src="/zimec/public/temas/simec/js/plugins/gritter/jquery.gritter.min.js"></script>

<!-- Bootbox -->
<script src="/zimec/public/temas/simec/js/plugins/bootbox/bootbox.min.js"></script>

<!-- Ion Range Slider -->
<script src="/zimec/public/temas/simec/js/plugins/ionRangeSlider/ion.rangeSlider.min.js"></script>

<!-- Validate -->
<script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.extend.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/validate/jquery.form.min.js"></script>

<!-- SIMEC -->
<script src="/zimec/public/temas/simec/js/theme.js"></script>
<script src="/zimec/public/temas/simec/js/simec.js"></script>
<script src="/zimec/public/temas/simec/js/simec-eventos.js"></script>
<script src="/zimec/public/temas/simec/js/simec-mascaras.js"></script>

<script>
    $(document).ready(function() {
        <?php if (!empty($mensagemCarregar) && isset($mensagemCarregar['text'])) { ?>
        swal({title: "", text: "<?php echo $mensagemCarregar['text']; ?>", type: "<?php echo $mensagemCarregar['type']; ?>"});
        <?php } ?>
    });
</script>