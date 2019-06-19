<?php
/**
 * Lista de Estados
 *
 * @category Lista
 * @package  A1
 * @author   Marcus Rocha
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 28-09-2015
 * @link     no link
 */
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

// Municipio.
$_POST['itrid'] = 1;
$controllerInicio = new Par3_Controller_Inicio();
$modelInicio = new Par3_Model_Inicio();
echo $simec->title('Associar página inicial ao perfil', '');
if ($_REQUEST['req'] == 'salvar') {
    $controllerInicio->salvar($_REQUEST);
    die();
}


?>
<style>
    .wrapper-content .chosen-container{
        width:1100px!important;
    }
</style>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">

        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Lista de Estados</h5>

                <div class="ibox-tools">
                    <a class="collapse-link"> <i class="fa fa-chevron-up"></i>
                    </a>
                </div>
            </div>
            <div class="ibox-content">
                <form method="post">
                    <input type="hidden" name="req" value="salvar">
                    <table class="table table-hover dataTable" id="table">
                        <thead>
                        <tr>
                            <th>Perfil</th>
                            <th>Página inicial</th>
                        </tr>
                        </thead>
                        <?php
                        $listaInicio = $modelInicio->lista();
                        $listaInicio = $listaInicio ? $listaInicio : array();

                        $sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from seguranca.perfil where pflstatus='A' and sisid=231 order by pfldsc ";
                        $dados = $db->carregar($sql);
                      //  ver($dados,d);
                        foreach ($dados as $perfil) :

                            $sql1 = "SELECT
	                            menu.mnuid as CODIGO,
	                            mnulink as DESCRICAO
	                        FROM seguranca.menu
	                        LEFT JOIN seguranca.perfilmenu p on menu.mnuid = p.mnuid AND p.pflcod=" . $perfil['codigo'] . "
	                        WHERE  mnustatus = 'A' ANd menu.sisid=231
	                        ORDER BY mnucod";
                            $listaMenus = $db->carregar($sql1);
                            ?>
                            <tr>
                                <td><?php echo $perfil['descricao'] ?></td>

                                <?php foreach($listaInicio as $key){
                                    if($key['pflcod'] == $perfil['codigo']){
                                      $value = $key['mnuid'];
                                    }
                                }?>

                                <td><?php echo $simec->select("inicio[$perfil[codigo]]", "",$value, $sql1,array(),array('style'=>"width:3000px;")); ?></td>
                            </tr>
                        <?php endforeach; ?>
                    </table>
                    <input type="submit"  class="btn btn-success" value="Salvar">
                </div>
            </form>
        </div>
    </div>
</div>
<script type="text/javascript">
    $(document).ready(function () {
        $('#formulario').submit(function () {
// 		$("#grupo option").attr("selected", "selected").trigger('liszt:updated');
        });
        $('#table').dataTable( {
    	  "pageLength": 1000,
    	  "lengthChange":false
    	} );
    });

    function dadosUnidade(inuid) {
        var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=' + inuid;
        $(location).attr('href', url);
    }
</script>