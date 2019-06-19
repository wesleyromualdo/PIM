<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 24/07/2018
 * Time: 10:17
 */
?>


<?php
include APPRAIZ . 'par3/classes/helper/ProgramaFPMHelper.class.inc';
include APPRAIZ . 'par3/classes/model/Programa.class.inc';

$helper = (empty($helper)) ? new Par3_Helper_ProgramaFPMHelper() : $helper;
$fpmController = (empty($fpmController)) ?  new Par3_Controller_ProgramaFPMMunicipio() : $fpmController;
$model = new Par3_Model_ProgramaFPMMunicipio();
$modelPrograma = new Par3_Model_Programa();
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
if (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis)
    || in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO_DESENVOLVEDOR, $perfis)
    || in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis)
    || in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) {
    $dadosProAdesao = $modelProadesao->pegaLinha("select * from par3.proadesao where prgid = 54");
    $docidInserido = wf_cadastrarDocumento(WF_TPDID_FPM, 'Documento de Adesão ao Programa FPM', PAR3_FLUXO_FPM_NAO_INICIADO);
    $fluxoIniciado = $modelAdesaoPrograma->recuperaPorInuid($_GET['inuid'],$dadosProAdesao['pfaid'],2018);
    if(!$fluxoIniciado){
        $arrayNaoIniciado = array(
            'adpano' => $_SESSION['exercicio'],
            'inuid'  => $_GET['inuid'],
            'usucpf'  => $_SESSION['usucpf'],
            'pfaid'  => $dadosProAdesao['pfaid'],
            'docid'  => $docidInserido,
            'adpano_ciclo'  => 2018,
            'tapid'  => 1,
        );
        $modelAdesaoPrograma->popularDadosObjeto($arrayNaoIniciado);
        $modelAdesaoPrograma->salvar();
        $modelAdesaoPrograma->commit();
    }
    //ACEITOU OU NÃO ACEITOU
    if(!empty($_REQUEST['aceite'])){
        if($_REQUEST['aceite'] == 'S'){
            $docid = $modelAdesaoPrograma->pegaLinha("select * from par3.prodesaoprograma as p inner join workflow.documento as doc on doc.docid = p.docid where inuid = {$_GET['inuid']} and pfaid = 11 ");
            if($docid['docid']){
                if($docid['esdid'] == PAR3_FLUXO_FPM_NAO_INICIADO){
                    $alterou = wf_alterarEstado($docid['docid'],WF_AEDID_FPM_NAOINICIADO_EMCADASTRAMENTO,"Secretário aceitou o termo.",array(),array());
                }elseif($docid['esdid'] == PAR3_FLUXO_FPM_NAO_ACEITOU_TERMO){
                    $alterou = wf_alterarEstado($docid['docid'],WF_AEDID_FPM_NAOACEITOU_EMCADASTRAMENTO,"Secretário aceitou o termo.",array(),array());
                }
            }
        }/*elseif($_REQUEST['aceite'] == 'N'){

                $docid = $modelAdesaoPrograma->pegaLinha("select * from par3.prodesaoprograma where inuid = {$_GET['inuid']} and pfaid = 11 ");
                wf_alterarEstado($docid['docid'],WF_AEDID_FPM_NAOINICIADO_NAOACEITOUOTERMO,"Secretário não aceitou o termo.",array(),array());
            }*/
    }
}


if($_REQUEST['requisicao'] == "nao_aceita" && $_REQUEST['prgid'] == PRGID_APOIO_EMERGENCIAL_MUNICIPIOS){
    ob_clean();
    //NÃO ACEITOU
    if(!empty($_REQUEST['aceite'])){
        if($_REQUEST['aceite'] == 'N'){
            $docid = $modelAdesaoPrograma->pegaLinha("select * from par3.prodesaoprograma where inuid = {$_GET['inuid']} and pfaid = 11 ");
            $alterou = wf_alterarEstado($docid['docid'],WF_AEDID_FPM_NAOINICIADO_NAOACEITOUOTERMO,"Secretário não aceitou o termo.",array(),array());
            $dadosAdesaoPrograma = $modelAdesaoPrograma->pegaLinha("select * from par3.prodesaoprograma where inuid = {$_REQUEST['inuid']} and pfaid = 11");
            $arrayProadesaoPrograma = array();
            if($dadosAdesaoPrograma['adpid']){
                $arrayProadesaoPrograma = array(
                    'adpid' =>   $dadosAdesaoPrograma['adpid'],
                    'adpdataresposta' => date("Y-m-d H:i:s"),
                    'adpresposta' => "N",
                );
                $modelAdesaoPrograma->popularDadosObjeto($arrayProadesaoPrograma);
                $modelAdesaoPrograma->salvar();
                $modelAdesaoPrograma->commit();
            }
            if($alterou == true){
                echo "T";
            }else{
                echo "F";
            }
        }
    }
    die();
}

if($_REQUEST['requisicao'] == "imprimir_termo_escola"){
    ob_end_clean();
    $dadosUnidadeImprimir = $modelProadesao->pegaLinha("SELECT * FROM par3.instrumentounidade where inuid = {$_GET['inuid']};");
    $nomeMunicipio = $dadosUnidadeImprimir['inudescricao'];
    $nomeUsuarioLogado = $_SESSION['usunome'];
    $dadosDaAdesaoImprimir = $modelAdesaoPrograma->getDadosDoUsuarioAssinou($_GET['inuid']);
    $nomeSecretario = $dadosDaAdesaoImprimir['usunome'];
    $dataAssinou = $dadosDaAdesaoImprimir['adpdataresposta'];
    $dataAssinouFormatada = " - ";
    $dataImpressao = " - ";



    if(!empty($dataAssinou)){
        $data = new DateTime($dataAssinou);
        $dataAssinouFormatada = $data->format("d-m-Y");
    }
    $usuarioOnline = $_SESSION['usunome'];
    $cpfUsuarioOnline = $_SESSION['usucpf'];

    $htmlAssinou = "Assinado por {$nomeSecretario} em {$dataAssinouFormatada}";
    $htmlImpressoPor = "Impresso por {$usuarioOnline} - CPF: {$cpfUsuarioOnline}";
    $dadosEscolasDaUnidade = $fpmController->cerregaValoresPorEscola($_GET['inuid']);
    $dadosEscolasDaUnidadeJaComValores = $fpmController->cerregaValoresPorEscolaJaComValores($_GET['inuid']);
    $helper->setListaEscolasDaUnidade($dadosEscolasDaUnidade);
    $htmlDadosTabelaEscola = $helper->getHtmlTabelaEscolaImpressao();
    $valorCamposMatricula = $model->carregaValueCamposMatriculas($_GET['inuid']);
    $dadosFndeDoMunicipio = $fpmController->getDadosFnde2018ByUnidade($_GET['inuid']);
    $somaMatriculasTodasAsEscolas = $fpmController->getTomaTotalMatriculaAtendida($_GET['inuid']);
    $textoTermo = $modelPrograma->pegaUm("select pfasobre from par3.proadesao where prgid = 54");
    $htmlDadosMatricula   = $helper->getDadosMatriculaImpressao($valorCamposMatricula,$dadosFndeDoMunicipio,$somaMatriculasTodasAsEscolas,$textoTermo);

    html2Pdf("
                        <html>
                          <head>
                            <style>
                              
                              .footer { position: fixed; bottom: 0px; }
                            </style>
                          </head>
                          <body>
                          $htmlDadosMatricula
                          <br>
                            $htmlDadosTabelaEscola
                          </body>
                        </html>",
        "termoAdesao");
}

if($_REQUEST['requisicao'] == "imprimir_termo_fpm"){
    ob_end_clean();
   $dadosUnidadeImprimir = $modelProadesao->pegaLinha("SELECT * FROM par3.instrumentounidade where inuid = {$_GET['inuid']};");
   $dadosProgramaImprimir = $modelPrograma->pegaUm("select pfatermomunicipio from par3.proadesao where prgid  = 54;");
   $nomeMunicipio = $dadosUnidadeImprimir['inudescricao'];
   $nomeUsuarioLogado = $_SESSION['usunome'];
   $dadosDaAdesaoImprimir = $modelAdesaoPrograma->getDadosDoUsuarioAssinou($_GET['inuid']);
   $tpAssinatura = "N";
   $resposta = "";
   if(!empty($dadosDaAdesaoImprimir['adpresposta'])){
    $resposta = ($dadosDaAdesaoImprimir['adpresposta'] == "N")? "Não Aceito e " : "Aceito e ";
   }
   $nomeSecretario = $dadosDaAdesaoImprimir['usunome'];
   $dataAssinou = $dadosDaAdesaoImprimir['adpdataresposta'];
   $dataAssinouFormatada = " - ";
   $dataImpressao = " - ";
   $textTermo = $dadosProgramaImprimir;
   if(!empty($dataAssinou)){
       $data = new DateTime($dataAssinou);
       $dataAssinouFormatada = $data->format("d-m-Y");
   }
   $usuarioOnline = $_SESSION['usunome'];
   $cpfUsuarioOnline = $_SESSION['usucpf'];

   $htmlAssinou = "Assinado por {$nomeSecretario} em {$dataAssinouFormatada}";
   $htmlImpressoPor = "Impresso por {$usuarioOnline} - CPF: {$cpfUsuarioOnline}";
    $hrImpressao = date('d/m/Y - H:i:s');
    html2Pdf("<div id=\"div-impressao\">
    <div id=\"div_cabecalho\" class=\"row col-lg-offset-2\">
        <table width=\"92%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"text-align:center;\">
            <tr bgcolor=\"#ffffff\">
                <td nowrap align=\"left\" valign=\"middle\" height=\"1\" style=\"padding:5px 0 0 0;\">
                    MEC/FNDE <br>
                    Impressão Fomento à execução do Programa Novo Mais Educação<br>
                </td>
                <td align=\"right\" valign=\"middle\" height=\"1\" style=\"padding:5px 0 0 0;\">
                    Impresso por: <b> {$_SESSION['usunome']}</b><br/>
                    Hora da Impressão: {$hrImpressao} <br/>
                </td>
            </tr>
        </table>
        <hr style=\"color:black; width:95%; margin-left:-18px;\">
    </div>
    <div class=\"clearfix\" style=\"margin-bottom:10px;\"></div>
    <div id=\"impressao-content\">
    <p>$textTermo</p>
          <div class='footer'><i style='color:red; font-size: 10px;'>$resposta $htmlAssinou</i><br><br><i style='color:red; font-size: 10px;'>$htmlImpressoPor</i></div>
    </div>
</div>",
        "termoAdesao");
}

/**
 * Retorna a somatória das vagas disponibilizadas por escola
 */
if($_REQUEST['requisicao'] ==  "somatoria_vl_escola_do_municipio" && !empty($_REQUEST['inuid'])){
    ob_clean();
    $vl_total_escolas_ja_cadastrado = $fpmController->getSomaVlJaCadastradosNasEscolas($_REQUEST['inuid']);
    $vl_total_escolas_ja_cadastrado = (empty($vl_total_escolas_ja_cadastrado)) ? "0" : $vl_total_escolas_ja_cadastrado;
    echo $vl_total_escolas_ja_cadastrado;
    die();
}

if($_REQUEST['requisicao'] ==  "valida_soma_total_por_escola" && !empty($_REQUEST['inuid'])){
    ob_clean();

    die();
}
?>
