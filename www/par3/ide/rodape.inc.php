</div>
</div>
<div class="col-md-3 col-lg-3 text-center" style="display:none">
    <img style="width: 95%; margin-bottom: 10px; height: 110px;" class="img-circle profile-pic" src="/seguranca/imagemperfil.php" alt="<?php echo ucwords(strtolower($usuarioNome)); ?>">
    <a style="padding-left: 5px;" href="#" class="profile-picture">Editar foto</a>
</div>
</body>
</html>

<!-- Custom and plugin javascript -->
<script src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/slimscroll/jquery.slimscroll.min.js"></script>

<!-- Chosen -->
<script src="/zimec/public/temas/simec/js/plugins/chosen/chosen.jquery.js"></script>

<!-- JSKnob -->
<script src="/zimec/public/temas/simec/js/plugins/jsKnob/jquery.knob.js"></script>

<!-- Switcher -->
<script src="/zimec/public/temas/simec/js/plugins/bootstrap-switch/bootstrap-switch.js"></script>

<!-- NanoScroller -->
<script src="/zimec/public/temas/simec/js/plugins/nanoscroll/jquery.nanoscroller.min.js"></script>

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

<!-- Sweet Aler -->
<script src="/zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js"></script>

<!-- Add fancyBox main JS and CSS files -->
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen"/>

<!-- Add Button helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css"/>
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

<!-- Add Thumbnail helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css"/>
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>

<!-- Add Media helper (this is optional) -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>

<!-- SIMEC -->
<script src="/zimec/public/temas/simec/js/theme.js"></script>
<script src="/zimec/public/temas/simec/js/simec.js"></script>
<script src="/zimec/public/temas/simec/js/simec-eventos.js"></script>

<script>
    $(function () {
        $('.menu').on('click', function () {
            var id = $(this).attr('href');
            $('.wrapper').find('table').removeClass('alert alert-warning');

            $(id).next().find('table').addClass('alert alert-warning');
            var options = {};
            $(id).next().find('table').effect('highlight', options, 1000);
            $(id).next().find('th').effect('highlight', options, 1000);
            $(id).next().find('.panel-body').effect('highlight', options, 1000);
        });

        $('#textFind').keyup(function () {
            var valThis = $(this).val().toLowerCase();
            $('.nav>li.tabela>a>span').each(function () {
                var text = $(this).text().toLowerCase();
                var match = text.indexOf(valThis);
                if (match >= 0) {
                    $(this).closest('li').show();
                } else {
                    $(this).closest('li').hide();
                }
            });
        });
    });
</script>