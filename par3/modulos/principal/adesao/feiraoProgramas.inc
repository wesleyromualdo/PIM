<?php
if (!is_numeric($_GET['inuid']) || $_GET['inuid'] < 1) {
    header('location: /par3/par3.php?modulo=principal/listaMunicipios&acao=A');
}

require APPRAIZ . 'includes/cabecalho.inc';

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_GET['inuid']);
$modelDocumento = new Workflow_Model_Documento($modelInstrumentoUnidade->docid);
$proadesao = new Par3_Model_Proadesao();
$programas = $proadesao->pegarProgramas($_GET['inuid']);

$possuiPendencia = $controleUnidade->verificaPendencias(array('cacs', 'monitoramento'));
$arrDadosOrcamentarios = $modelInstrumentoUnidade->carregaOrcamentoUnidade($_GET['inuid']);

//PEGA O TIPO DE ESFERA QUE É A UNIDADE
$esferaUnidade = (!empty($_GET['inuid'])) ? $controleUnidade->pegarItrid($_GET['inuid']) : null;
//OS PROGRAMAS TEM UMA FLAG pfaesfera que E = ESTADUAL, T=TODAS e M=MUNICIPAL
//$esferaUnidade com valor 3 = Distrito Federal
$tipoAdesaoDaUnidade = "T";
if($esferaUnidade == 1 || $esferaUnidade == 3){
    $tipoAdesaoDaUnidade = "E";
}elseif($esferaUnidade == 2){
    $tipoAdesaoDaUnidade = "M";
}
?>
<style>
.programasPar {
    cursor: pointer;
}
#parent {
    display: table;
    padding: 5px!important;
    width: 100%!important;
}
.vertical-align {
    display: table-cell;
    vertical-align: middle;
}
/* clear first in row in ie 8 or lower */
.multi-columns-row .first-in-row {
    clear: left;
}
/* clear the first in row for any block that has the class "multi-columns-row" */
.multi-columns-row .col-xs-6:nth-child(2n + 3) { clear: left; }
.multi-columns-row .col-xs-4:nth-child(3n + 4) { clear: left; }
.multi-columns-row .col-xs-3:nth-child(4n + 5) { clear: left; }
.multi-columns-row .col-xs-2:nth-child(6n + 7) { clear: left; }
.multi-columns-row .col-xs-1:nth-child(12n + 13) { clear: left; }

@media (min-width: 768px) {
    /* reset previous grid */
    .multi-columns-row .col-xs-6:nth-child(2n + 3) { clear: none; }
    .multi-columns-row .col-xs-4:nth-child(3n + 4) { clear: none; }
    .multi-columns-row .col-xs-3:nth-child(4n + 5) { clear: none; }
    .multi-columns-row .col-xs-2:nth-child(6n + 7) { clear: none; }
    .multi-columns-row .col-xs-1:nth-child(12n + 13) { clear: none; }

    /* clear first in row for small columns */
    .multi-columns-row .col-sm-6:nth-child(2n + 3) { clear: left; }
    .multi-columns-row .col-sm-4:nth-child(3n + 4) { clear: left; }
    .multi-columns-row .col-sm-3:nth-child(4n + 5) { clear: left; }
    .multi-columns-row .col-sm-2:nth-child(6n + 7) { clear: left; }
    .multi-columns-row .col-sm-1:nth-child(12n + 13) { clear: left; }
}
@media (min-width: 992px) {
    /* reset previous grid */
    .multi-columns-row .col-sm-6:nth-child(2n + 3) { clear: none; }
    .multi-columns-row .col-sm-4:nth-child(3n + 4) { clear: none; }
    .multi-columns-row .col-sm-3:nth-child(4n + 5) { clear: none; }
    .multi-columns-row .col-sm-2:nth-child(6n + 7) { clear: none; }
    .multi-columns-row .col-sm-1:nth-child(12n + 13) { clear: none; }

    /* clear first in row for medium columns */
    .multi-columns-row .col-md-6:nth-child(2n + 3) { clear: left; }
    .multi-columns-row .col-md-4:nth-child(3n + 4) { clear: left; }
    .multi-columns-row .col-md-3:nth-child(4n + 5) { clear: left; }
    .multi-columns-row .col-md-2:nth-child(6n + 7) { clear: left; }
    .multi-columns-row .col-md-1:nth-child(12n + 13) { clear: left; }
}
@media (min-width: 1200px) {
    /* reset previous grid */
    .multi-columns-row .col-md-6:nth-child(2n + 3) { clear: none; }
    .multi-columns-row .col-md-4:nth-child(3n + 4) { clear: none; }
    .multi-columns-row .col-md-3:nth-child(4n + 5) { clear: none; }
    .multi-columns-row .col-md-2:nth-child(6n + 7) { clear: none; }
    .multi-columns-row .col-md-1:nth-child(12n + 13) { clear: none; }

    /* clear first in row for large columns */
    .multi-columns-row .col-lg-6:nth-child(2n + 3) { clear: left; }
    .multi-columns-row .col-lg-4:nth-child(3n + 4) { clear: left; }
    .multi-columns-row .col-lg-3:nth-child(4n + 5) { clear: left; }
    .multi-columns-row .col-lg-2:nth-child(6n + 7) { clear: left; }
    .multi-columns-row .col-lg-1:nth-child(12n + 13) { clear: left; }
}
</style>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2>Adesão de Programas - <?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h2>
    </div>
</div>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
	    <div class="ibox-content">
	    	<div class="row  multi-columns-row">
                    <?php
	    		require_once APPRAIZ . 'includes/workflow.php';
	    		$controllerInstrumentoUnidade = new Par3_Controller_InstrumentoUnidade();
	    		
	    		$itrid = $controllerInstrumentoUnidade->pegarItrid($_GET['inuid']);
	    		
	    		$rede = ($itrid == '3' || $itrid == '1') ? 'Estado' : 'Município';

	    		if( is_array($programas) ){
                    foreach( $programas as $programa ){
                        //CASO O PROGRAMA SEJA T ou ESTEJA DE ACORDO COM A ESFERA DA UNIDADE É MOSTRADO O PROGRAMA
                        if($programa['pfaesfera'] == $tipoAdesaoDaUnidade || $programa['pfaesfera'] == "T"){
                           if($programa['vigencia']!='INATIVO'){
                                if (
                                    (strpos($programa['prgabreviatura'], 'Novo Mais Educação')  !== false ) ||
                                    (strpos($programa['prgabreviatura'], 'PROEMI')  !== false ) ||
                                    (strpos($programa['prgabreviatura'], 'Mais Alfabetização')  !== false ))
                                {
                                    $docid ='';
                                    $modelAdesaoPrograma =  new Par3_Model_AdesaoPrograma();

                                    $onclick = "direcionarAdesao({$programa['prgid']});";

                                    $pfaid = $programa['pfaid'];
                                    $inuid = $_REQUEST['inuid'];
                                    if(($pfaid) && ($inuid))
                                    {
                                        if(strtotime($programa['pfadatafinal']. ' 12:00:00') < strtotime(date('Y-m-d G:i:s')) )
                                        {
                                            $corSituacao = '#FF0000';
                                            $textoSituacao = "A adesão ao programa encerrou dia " . formata_data($programa['pfadatafinal']);
                                        }
                                        else
                                        {
                                            $resultado = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid);

                                            if( $resultado )
                                            {
                                                $docid = $resultado['docid'];

                                                if($docid)
                                                {
                                                    // por se tratar de foreach zero pra garantir
                                                    $tpdidAtual = '';
                                                    $esdid = '';

                                                    $resultadoTipo = wf_retornaTipoByDocid($docid);
                                                    $tpdidAtual = $resultadoTipo['tpdid'];

                                                    $situacaoAtual = wf_pegarEstadoAtual($docid);

                                                    $esdid = $situacaoAtual['esdid'];

                                                    if( $tpdidAtual == WF_TPDID_PROEMI)
                                                    {
                                                        if($esdid == WF_ESDID_ENVIADOPARAOMEC_PROEMI)
                                                        {
                                                            $corSituacao = "#1cc684";
                                                            $textoSituacao = "O {$rede} aderiu ao programa";
                                                        }
                                                        else
                                                        {

                                                            $corSituacao = "#1c84c6";
                                                            $textoSituacao = "Adesão ao programa aberta";
                                                        }
                                                        $onclick = "direcionarAdesao({$programa['prgid']});";
                                                    }
                                                    else if( $tpdidAtual == WF_TPDID_ABCMAIS )
                                                    {

                                                        if($esdid == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS)
                                                        {
                                                            $corSituacao = "#1cc684";
                                                            $textoSituacao = "O {$rede} aderiu ao programa";
                                                        }
                                                        else
                                                        {
                                                            $corSituacao = "#1c84c6";
                                                            $textoSituacao = "Adesão ao programa aberta";
                                                        }

                                                    }else if( $tpdidAtual == WF_TPDID_MAISALFABETIZACAO )
                                                    {
                                                        if($esdid == WF_ESDID_ENVIADOPARAOMEC_MAISALFABETIZACAO)
                                                        {
                                                            $corSituacao = "#1cc684";
                                                            $textoSituacao = "O {$rede} aderiu ao programa";
                                                        }
                                                        else
                                                        {
                                                            $corSituacao = "#1c84c6";
                                                            $textoSituacao = "Adesão ao programa aberta";
                                                        }
                                                    }
                                                    else
                                                    {
                                                        $corSituacao = "#1c84c6";
                                                        $textoSituacao = "Adesão ao programa aberta";
                                                        $onclick = "direcionarAdesao({$programa['prgid']});";
                                                    }

                                                }
                                            }
                                            else
                                            {
                                                $corSituacao = "#1c84c6";
                                                $textoSituacao = "Adesão ao programa aberta" . $docid;
                                            }
                                        }
                                    }

                                }
                                else
                                {
                                    if (empty($programa['pfafundocor'])){
                                        $programa['pfafundocor'] = "#444444";
                                    }
                                    if ($programa['pfaid'] == 4)
                                    {
                                        $corSituacao = "#ff3300";
                                        $textoSituacao = "<b style='color:#ff3300'> A Resolução FNDE nº 02/2016 foi revogada, portanto, esta adesão perdeu efeito. Por favor, faça adesão ao Programa Novo Mais Educação, disponível nesta tela.</b>";
                                    }
                                    else if ($programa['vigencia']=='ATIVO'){
                                        //ver($programa);
                                        $corSituacao = "#1cc684";
                                        $textoSituacao = "Adesão ao programa aberta";
                                        $onclick = "direcionarAdesao({$programa['prgid']});";
                                    } else {
                                        if($programa['vigencia']=='CADASTRADO'){
                                            $corSituacao = "#1c84c6";
                                            $textoSituacao = "$rede aderiu ao programa";
                                            $onclick = "direcionarAdesao({$programa['prgid']});";
                                        }else{
                                            // $onclick = "alert('O município não aderiu ao programa. {$programa['vigencia']}');";
                                            $onclick = "direcionarAdesao({$programa['prgid']});";
                                            $corSituacao = "#ff3300";
                                            $textoSituacao = $programa['vigencia'];
                                        }
                                    }
                                }

                                ?>
                                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                                    <div id="parent" class="box-pendencia widget style1 yellow-bg" style="min-height: 140px!important; background: <?php echo $programa['pfafundocor']; ?> url(../par3/imagens/<?php echo $programa['pfabackground']; ?>) center no-repeat; padding: 20px;" onclick="<?php echo $onclick; ?>" >
                                        <div class="row vertical-align">
                                            <?php if($programa['pfaicone'] != '')
                                            { ?>
                                                <div class="col-lg-3 col-md-3 col-xs-3" style="width: 25%; padding-left: 10px; padding-right: 0px;">
                                                    <img class="img-responsive" width="100%" src="../par3/imagens/<?php echo $programa['pfaicone']; ?>" />
                                                </div>
                                                <?php
                                            }?>
                                            <div class="col-lg-9 col-md-9 col-xs-9 text-left" style="width: 75%">
                                                <h3 style="margin: 6px 0px"><?php echo $programa['prgabreviatura']; ?></h3>
                                                <span><?php echo $programa['prgdsc']; ?></span>
                                            </div>
                                        </div>
                                    </div>
                                    <?php if ($programa['pfaid'] != 4) { ?>
                                        <div style="width:10px;height:10px;float:left;background-color:<?php echo $corSituacao; ?>;margin-top:9px;margin-left:15px;"></div>
                                    <?php } ?>
                                    <h5 style="float:left;margin-top:7px;margin-left:5px;"><?php echo $textoSituacao; ?></h5>
                                </div>
                                <?php
                            }
                        }

                    }
                } else { ?>
                    <div class="alert alert-danger">
                        <center><i class="fa fa-warning"></i> No momento não existem programas abertos para adesão </center>
                    </div>
        <?php   } ?>

        <!-- Termo de mainfesto não faz parte de um programa específico, mas veio parar aqui a pedido do gestor. -->
                <?php if($tipoAdesaoDaUnidade === 'E'){

                    $obDocumento = new Par3_Controller_DocumentoTermo();
                    $dadosTermo = $obDocumento->recuperaTermoManifestacao($_GET['inuid']);
                    if($dadosTermo['domid'] > 0){

                ?>
                    <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                        <div class="box-pendencia widget style1 yellow-bg" style="background: #545759 center no-repeat; padding: 20px;" onclick="window.location.href = '/par3/par3.php?modulo=principal/adesao/termos/manifestacao&acao=A&inuid=<?= $_GET['inuid'] ?>';">
                            <div class="row vertical-align">
                                <div class="col-lg-9 col-md-9 col-xs-9 text-left" style="width: 75%">
                                    <h3 style="margin: 6px 0px">Centro Nacional de Mídias da Educação
                                </div>
                            </div>
                        </div>
                        <div style="width:10px;height:10px;float:left;background-color:#fef6ff;margin-top:9px;margin-left:15px;"></div>
                        <?php

                        $cor = '';
                        $texto = '';
                        switch ($dadosTermo['domopcao']){
                            case '1':
                                echo '<div style="width:10px;height:10px;float:left;background-color:#088e0c;margin-top:9px;margin-left:15px;"></div>';
                                echo '<h5 style="float:left;margin-top:7px;margin-left:5px;"> Interesse integral</h5>';
                                break;
                            case '2':
                                echo '<div style="width:10px;height:10px;float:left;background-color:#fba407;margin-top:9px;margin-left:15px;"></div>';
                                echo '<h5 style="float:left;margin-top:7px;margin-left:5px;"> Interesse Parcial</h5>';
                                break;
                            case '3':
                                echo '<div style="width:10px;height:10px;float:left;background-color:#ef3106;margin-top:9px;margin-left:15px;"></div>';
                                echo '<h5 style="float:left;margin-top:7px;margin-left:5px;"> Sem interesse</h5>';
                                break;
                            default:
                                echo '<h5 style="float:left;margin-top:7px;margin-left:5px;">Acesse para manifestação de interesse</h5>';
                        }
                        ?>
                        <h5 style="float:left;margin-top:7px;margin-left:5px;"></h5>
                    </div>
                <?php } //fim condição estados
                } //fim condição exite documento ?>
            </div>
        </div>
    </div>
    
    <?php if($modelInstrumentoUnidade->itrid != '2'){?>
    <div class="row">
        <div class="ibox-content">
            <div class="row">
                <?php
                /* Disponibilização dessa funcoinalidade para municípios e somente se tiver orçamento liberado na SS 4912*/
                if(
                ($arrDadosOrcamentarios[1]['valor_restante']+$arrDadosOrcamentarios[2]['valor_restante'])>0
            ){
                $msg = 'O Diagnóstico deve ser finalizado para desbloquear esse menu.';
                if($modelInstrumentoUnidade->itrid == '2') $msg = 'O Município não pode possuir pendências e deve finalizar o diagnostico para desbloquear esse menu.';

                if(
                    !in_array($modelDocumento->esdid, array(PAR3_WORKFLOW_FLUXO_DO_PAR_PREPARATORIA_DIAGNOSTIGO, PAR3_WORKFLOW_FLUXO_DO_PAR_DIAGNOSTIGO))
                    && ( ( $modelInstrumentoUnidade->itrid == '2' && $possuiPendencia == 100 ) || $modelInstrumentoUnidade->itrid != '2')
                ){
            ?>
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="box-pendencia widget style1 yellow-bg" style="background: #1fa67a; padding: 20px;" onclick="window.location.href='par3.php?modulo=principal/planoTrabalho/programas&acao=A&inuid=<?php echo $_REQUEST['inuid'];?>'">
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
            <?php
                } else {
            ?>
                <div class="col-lg-3 col-md-3 col-sm-6 col-xs-12">
                    <div class="box-pendencia widget style1 yellow-bg" style="background: red; padding: 20px;" onclick="alert('O Diagnótico deve ser finalizado para desbloquear esse menu.');">
                        <div class="row vertical-align">
                            <div class="col-lg-3 col-md-3 col-xs-3" style="width: 25%; padding-left: 10px; padding-right: 0px;">
                               <i class="fa fa-university" aria-hidden="true" style="font-size: 40px;"></i>
                            </div>
                            <div class="col-lg-9 col-md-9 col-xs-9 text-left" style="width: 75%">
                                <h3 style="margin: 6px 0px">Iniciativas do PAR</h3>
                                <span><?php echo $msg; ?></span>
                            </div>
                        </div>
                    </div>
                </div>
            <?php
                }
                }
                ?>
            </div>
        </div>
    </div>
    <?php } ?>
</div>

<?php
$url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='.$_REQUEST['inuid'];

if ( $_REQUEST['inuid'] !== '') {
    $urlCalendario = 'par3.php?modulo=principal/evento/calendario&acao=A&inuid='.$_REQUEST['inuid'];
} else {
    $urlCalendario = 'par3.php?modulo=principal/evento/calendario&acao=A';
}
$obModelo = new Par3_Model_InstrumentoUnidade();
?>
<div id="materialButton" class="click-to-toggle fixed-action-btn" style="bottom: 45px; right: 24px;">
    <a class="btn_par-floating btn_par-large red">
        <i class="large material-icons ">view_headline</i>
    </a>
    <ul>
      <li><a class="btn_par-floating tourn-btn indigo" title="Tour"><i class="material-icons">language</i></a></li>
      	<li><a class="btn_par-floating green" href="javascript:window.print();"><i class="material-icons">print</i></a></li>
      	<?php if($obModelo->testa_superuser()){ ?>
      		<li><a class="btn_par-floating amber" title="Calendário" href="<?php echo $urlCalendario; ?>"><i class="material-icons">today</i></a></li>
      	<?php } ?>
      	<li><a class="btn_par-floating blue" href="<?php echo $url; ?>"><i class="material-icons">visibility</i></a></li>
    </ul>
</div>

<script>
$(document).ready(function(){

    $('.btn_par-large').click(function(){
        if( $('#materialButton').hasClass("active") ){
        	$('.click-to-toggle').closeFAB();
        }else{
        	$('.click-to-toggle').openFAB();
        }
    });

    $('.box-pendencia').hover(function(){
        $(this).find('.iconeSpin').addClass('fa-spin');
    });

    $('.box-pendencia').mouseleave(function(){
        $(this).find('.iconeSpin').removeClass('fa-spin');
    });
});


function direcionarAdesao(prgid){

	location.href = 'par3.php?modulo=principal/adesao/termo&acao=A&prgid='+prgid+'&inuid=<?php echo $_REQUEST['inuid']; ?>';
}

</script>