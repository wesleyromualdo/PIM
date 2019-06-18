<?php


/**
 * Tela de dados do prefeito
 *
 * @category visao
 * @package  A1
 * @author   Hemerson Morais
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05/07/2017
 * @link     no link
 */

checkParamInuid();
$inuid = $_REQUEST['inuid'];

$renderEntidade                       = new Par3_Controller_Entidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

include_once APPRAIZ .'par3/classes/controller/DocumentoLegado.class.inc';
?>
<script language="javascript" src="js/documentoLegado.js"></script>
<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="inuid"   id="inuid"  value="<?php echo $inuid?>"/>
    <input type="hidden" name="req"                 value="salvar"/>
    <input type="hidden" name="tenid"   id="tenid"  value="<?php echo $tenid; ?>"/>
    <input type="hidden" name="menu"                value="<?php echo $_REQUEST['menu']; ?>"/>

    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-12" >
        	       <h3 id="entidade">Documentos de Obras do Par</h3>
                </div>
    		</div>
    	</div>

    	<div class="ibox-content" id="div_prefeito">
            <div class="ibox-content" id="ObrPAC" style="overflow: auto;">
                                            <?php echo Par3_Controller_DocumentoLegado::listaObrasPAR(array('terstatus'=>'A', 'inuid'=>$inuid)); ?>
                                        </div>
        </div>

			</div>


</form>
</div>

<script>
    $(document).ready(function()
    {
        $('.next').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        $('.previous').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });
    });
</script>
