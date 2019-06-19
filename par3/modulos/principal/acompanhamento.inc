<?php
/**
 * Lista de Estados
 *
 * @category Lista
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 28-09-2015
 * @link     no link
 */
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

$helperAcompanhamento = new Par3_Helper_Acompanhamento();
$arrMenus = $helperAcompanhamento->recuperarMenusPorCpf($_SESSION['usucpf']);
echo $simec->title('Acompanhamento', '');
?>
<style>
    .ibox-content-round-gray{
        width:99%;
        background-color: #f3f3f4;
        float: center;
        border-radius: 10px 10px 10px 10px;
        -moz-border-radius: 10px 10px 10px 10px;
        -webkit-border-radius: 10px 10px 10px 10px;
        border: 0px solid #000000;
    }
</style>
<script>
    $(document).ready(function () {
        $('.box-pendencia').click(function () {
            var url = $(this).data('url');
            window.location.href = url;
        });
    });
</script>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <div class="ibox-content">
                <div class="ibox-content-round-gray ibox-content">
                    <div class="row" >
                        <?php if (is_array($arrMenus)) { ?>
                            <?php foreach ($arrMenus as $k => $menu) { ?> 
                                <div class="col-lg-2">
                                    <div class="box-pendencia <?php echo $activeBox; ?> widget style1 yellow-bg" data-url="<?php echo $menu['mnulink']; ?>" style="background-color: <?php echo "#" . $menu['mnucor']; ?>">
                                        <div class="row vertical-align">
                                            <div class="col-xs-12 text-center">
                                                <h4><i class="fa <?php echo $menu['mnuicone']; ?> fa-3x"></i></h4>
                                                <span style="font-size:16px"><?php echo $menu['mnudsc']; ?></span>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            <?php } ?>
                        <?php } ?>
                    </div>
                </div>
            </div>
        </div>
    </div>