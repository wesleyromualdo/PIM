<?php

require_once APPRAIZ.'includes/workflow.php';

$controleUnidade         = new Par3_Controller_InstrumentoUnidade();
$controleUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelUnidade            = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$modelAuditoriaUnidade 	 = new Par3_Model_AuditoriaUnidade();
$modelCACS				 = new Par3_Model_CACS();
$controlCAE 	 		 = new Par3_Controller_CAE();
$modelIniciativa         = new Par3_Model_Iniciativa();
$controleIniciativa      = new Par3_Controller_Iniciativa();
$usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();

$disabled = $_REQUEST['habilitado'] == 'true' ? '' : 'disabled="disabled"';

switch($_REQUEST['req']){
	case 'carregaIniciativa':
	    ob_clean();
	    $controleIniciativa->formSubacao($_REQUEST, $disabled);
	    die();
	    break;
	case 'vinculaItemComposicao':
	    ob_clean();
	    $controleIniciativa->vinculaItemComposicao($_REQUEST);
	    die();
	    break;
	case 'vinculaEscolaSubacao':
	    ob_clean();
	    $controleIniciativa->vinculaEscolaSubacao($_REQUEST);
	    die();
	    break;
	case 'listaItensIniciativa':
	    ob_clean();
	    $controleIniciativa->formItensIniciativa($_REQUEST, $disabled);
	    die();
	    break;
	case 'listaEscolasIniciativa':
	    ob_clean();
	    $controleIniciativa->listaEscolasIniciativa($_REQUEST);
	    die();
	    break;
	case 'formItensIniciativaEscola':
	    ob_clean();
	    $controleIniciativa->formItensIniciativaEscola($_REQUEST, $disabled);
	    die();
	    break;
	case 'formEscolasMunicipio':
	    ob_clean();
	    $controleIniciativa->formEscolasMunicipio($_REQUEST, $disabled);
	    die();
	    break;
	case 'buscaPTRESPI':
	    ob_clean();
	    $controleIniciativa->montaSelectPTRESPI($_REQUEST);
	    die();
	    break;
	case 'salvaIniciativa':
	    ob_clean();
	    $controleIniciativa->salvaIniciativa($_REQUEST);
	    die();
	    break;
	case 'enviarAnalise':
	    ob_clean();
	    $modelIniciativa->aprovarIniciativas($_REQUEST);
	    $url = 'par3.php?modulo=principal/planoTrabalho/programas&acao=A&inuid=%inuid%';
        $controleUnidadeEntidade->tramitarWorkflowDocumento($_REQUEST['inuid'], PAR3_WORKFLOW_FINALIZAR_FASE_DE_ELABORACAO, $url);
	    die();
	    break;
	case 'retornarElaboracao':
	    ob_clean();
	    $modelIniciativa->reprovarIniciativas($_REQUEST);
	    $url = 'par3.php?modulo=principal/planoTrabalho/programas&acao=A&inuid=%inuid%';
        $controleUnidadeEntidade->tramitarWorkflowDocumento($_REQUEST['inuid'], PAR3_WORKFLOW_RETORNAR_FASE_DE_ELABORACAO, $url);
	    die();
	    break;
}

checkParamInuid();

$disabled = 'disabled="disabled"';

$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);
$estado = wf_pegarEstadoAtual($modelUnidade->docid);
$esdid = $estado['esdid'];

if (!$arrPermissao || !$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}

if ($arrPermissao['booEscrita'] && $esdid == PAR3_WORKFLOW_FLUXO_DO_PAR_ELABORACAO) {
    $disabled = '';
}

$retornaElaboracao = false;
if (
    $usuarioResponsabilidade->possuiPerfil([Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO])
    && $esdid == PAR3_WORKFLOW_FLUXO_DO_PAR_ANALISE
) {
    $retornaElaboracao = true;
}

$dscPreencher   = $disabled == '' ? 'PREENCHER' : '<i class="fa fa-arrow-right" aria-hidden="true"></i>';

$itrid  = $controleUnidade->pegarItrid($_REQUEST['inuid']);
$esfera = $controleUnidade->pegarDescricaoEsfera($_REQUEST['inuid']);

$perfils = pegaPerfils($_SESSION['usucpf']);

$arrSemResponsabilidade = array(
    Par3_Model_UsuarioResponsabilidade::EQUIPE_MUNICIPAL,
    Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL,
    Par3_Model_UsuarioResponsabilidade::PREFEITO,
    Par3_Model_UsuarioResponsabilidade::EQUIPE_ESTADUAL,
    Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL,
    Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
);

$verificaInicioPar = checaPerfilValido($arrSemResponsabilidade, $perfils) == 'S' ? $verificaInicioPar : false;

$arrProgramas        = $modelIniciativa->recuperarProgramas($_REQUEST);
$arrOrcamentoUnidade = $modelUnidade->carregaOrcamentoUnidade($_REQUEST['inuid']);

require APPRAIZ.'includes/cabecalho.inc';
?>
<style>
.iconUnidade{
    margin-top: 0px !important;
    margin-left: 0px !important;
    font-size: 15px !important;
    background-color:black;
    border-radius: 20px 20px 20px 20px;
    -moz-border-radius: 20px 20px 20px 20px;
    -webkit-border-radius: 20px 20px 20px 20px;
    color: yellow !important;
}

.menuUnidade{
    border-bottom: 1px solid #e3e3e3;
    padding: 5px;
    cursor:pointer;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
}

.menuUnidade:hover{
    font-weight: bold;
}

.menuSelecionado{
    font-weight: bold;
    cursor: default !important;
}

@media (max-width: 1450px) {
    .ibox-content-round-gray{
        margin-top: 0.5%
    }
}

@media (max-width: 1200px) {
    .i1450 {
        display: inline;
    }
    .custom-col-10{
        padding: 5px 0px 0px 3.8%;
    }
    .marcador{
        position:absolute;
        border-radius: 100% 0% 0% 100%;
        -moz-border-radius: 100% 0% 0% 100%;
        -webkit-border-radius: 100% 0% 0% 100%;
        border: 0px;
        width:50px;
        height:10px;
        margin-top:21px;
        margin-left:-1%;
    }
}
</style>
<script>
$(document).ready(function(){

	var inuid = '<?php echo $_REQUEST['inuid']?>';

    $('.subacao').click(function(){

    	$('#modal_html').html(' ');

    	var sbaid = $(this).attr('sbaid');
    	var ppsid = $(this).attr('ppsid');
    	var habilitado = $(this).attr('habilitado');
        var url = window.location.href+'&req=carregaIniciativa';

        $('#modal_html').load(url+'&ppsid='+ppsid+'&sbaid='+sbaid+'&inuid='+inuid+'&habilitado='+habilitado);
    });

    $('#btnFinalizar').click(function(){

    	swal({
            title: "Prezado(a) Dirigente",
            text: 'Deseja enviar as iniciativas elaboradas para análise? Se o fizer não poderá elaborar mais nenhuma iniciativa até o novo ciclo do PAR.',
            type: "warning", showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            console.log(isConfirm);
            if (isConfirm) {
                var url = window.location.href+'&inuid=<?php echo $_REQUEST['inuid']?>&req=enviarAnalise';
                $(location).attr('href', url);
            }
        });
    });

    $('#btnRetornar').click(function(){

    	swal({
            title: "Prezado(a) Super usuário",
            text: 'Deseja retornar a unidade para \'Elaboração\'?.',
            type: "warning", showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                var url = window.location.href+'&inuid=<?php echo $_REQUEST['inuid']?>&req=retornarElaboracao';
                $(location).attr('href', url);
            }
        });
    });

});
</script>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <div class="col-md-12 colunaForm" id="print_area">
                        <div class="ibox">
                        	<div class="ibox-title">
                                <div class="row">
                                	<div class="col-md-4" >
                            	       <div class="widget style1 yellow-bg"
                            			     style="background: #1fa67a url(../par3/imagens/<?php echo $programa['pfabackground']; ?>) center no-repeat; padding: 20px;">
                            				<div class="row vertical-align">
                            					<div class="col-lg-3 col-md-3 col-xs-3" style="width: 25%; padding-left: 10px; padding-right: 0px;">
                            					   <i class="fa fa-university" aria-hidden="true" style="font-size: 40px;"></i>
                            					</div>
                            					<div class="col-lg-9 col-md-9 col-xs-9 text-left" style="width: 75%">
                            						<h3 style="margin: 6px 0px">Iniciativas do PAR</h3>
                            					</div>
                            				</div>
                            			</div>
                                    </div>
                                	<div class="col-md-8" >
                                        <table class="table" style="font-size: 14px;">
                                            <thead>
                                                <tr>
                                                    <td>Orçamento</td>
                                                    <td>Valor</td>
                                                    <td>Valor Utilizado</td>
                                                    <td>Valor Restante</td>
                                                </tr>
                                            </thead>
                                            <tbody>
                                                <tr>
                                                    <td><?php echo $arrOrcamentoUnidade[1]['descricao']?></td>
                                                    <td>R$ <?php echo formata_numero_monetario(($arrOrcamentoUnidade[1]['valor']))?></td>
                                                    <td>R$ <?php echo formata_numero_monetario(($arrOrcamentoUnidade[1]['valor_utilizado']))?></td>
                                                    <td>R$ <?php echo formata_numero_monetario(($arrOrcamentoUnidade[1]['valor_restante']))?></td>
                                                </tr>
                                                <tr>
                                                    <td><?php echo $arrOrcamentoUnidade[2]['descricao']?></td>
                                                    <td>R$ <?php echo formata_numero_monetario(($arrOrcamentoUnidade[2]['valor']))?></td>
                                                    <td>R$ <?php echo formata_numero_monetario(($arrOrcamentoUnidade[2]['valor_utilizado']))?></td>
                                                    <td>R$ <?php echo formata_numero_monetario(($arrOrcamentoUnidade[2]['valor_restante']))?></td>
                                                </tr>
                                            </tbody>
                                        </table>
                                    </div>
                        		</div>
                        	</div>
                        	<?php
                        	if($retornaElaboracao){
                        	?>
                        	<div class="ibox-content">
                                <button id="btnRetornar" class="btn btn-success btn-lg" type="button">
                                    <i class="fa fa-undo"></i> Retornar para Elaboração
                                </button>
                            </div>
                        	<?php
                        	}
                        	?>
                        	<div class="ibox-content">
                        	<?php
                        	if(!$disabled){
                        	?>
                                <button id="btnFinalizar" class="btn btn-success btn-lg" type="button">
                                    <i class="fa fa-check-circle-o"></i> Finalizar Elaboração
                                </button>
                        	<?php
                        	}else{
                        	?>
                                <div class="widget style1 yellow-bg"
                                     style="background: #1fa67a center no-repeat; padding: 10px;">
                                	<div class="row vertical-align">
                                		<div class="col-lg-12 col-md-12 col-xs-12 text-left" style="width: 75%">
                                			<h3 style="margin: 6px 0px">Situação do PAR: <?php echo $estado['esddsc']; ?></h3>
                                		</div>
                                	</div>
                                </div>
                        	<?php
                        	}
                        	?>
                        	</div>
                        	<div class="ibox-content">
                        	<?php
                        	$prgid = '';
                        	$qtd = 0;
                        	foreach($arrProgramas as $k => $programas){

                                $dados = array(
                                    'prgid' => $programas['codigo'],
                                    'inuid' => $_REQUEST['inuid'],
                                );

                                $arrIniciativas = $modelIniciativa->recuperarIniciativasPrograma($dados);

                                foreach($arrIniciativas as $y => $iniciativa){
                                    if($qtd == 0) echo '<div class="row">';
                                    $qtd = $qtd+1;
                                    ?>
                                    <div class="col-lg-6">
            							<div class="widget style1 yellow-bg" style="display: table; height: 100px; width: 100%;">
            								<div class="row" style="height: 60px;">
            								    <div class="col-lg-12 col-md-12 col-xs-12 text-left" >
            										<h4 style="margin: 6px 0px"><?php echo $iniciativa['descricao'];?></h4>
            									</div>
            								</div>
            								<div class="row">
            								    <div class="col-lg-9 col-md-9 col-xs-9 text-left" >
            										<span>
                                                        <?php echo $iniciativa['ptsdescricao'];?> (<?php echo $iniciativa['cronograma'];?>)
                                                        <br> Valor utilizado: R$ <?php echo formata_numero_monetario($iniciativa['valor_iniciativa']); ?>
                                                        - <?php echo $iniciativa['tordescricao']; ?>
                                                    </span>
            									</div>
            								    <div class="col-lg-3 col-md-3 col-xs-3 text-left" >
            										<button class="btn btn-success subacao pull-right"
            										        data-toggle="modal"
            										        data-target="#modal"
            										        habilitado=<?php echo $disabled != '' ? 'false' : 'true';?>
            										        ppsid="<?php echo $iniciativa['ppsid'];?>"
            										        sbaid="<?php echo $iniciativa['sbaid'];?>" ><?php echo $dscPreencher; ?></button>
            									</div>
            								</div>
            							</div>
                                    </div>
                                <?php
                                    if( $qtd % 2 == 0 ) echo '</div><div class="row">';
                                }
                            }
                            ?>
                    		</div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog  modal-lg" id="modal_html">
    </div>
</div>