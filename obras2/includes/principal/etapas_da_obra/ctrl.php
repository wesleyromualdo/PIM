<?php /* ****arquivo vazio**** */

require APPRAIZ . 'obras2/includes/principal/etapas_da_obra/funcoes.php';


$_SESSION['obras2']['obrid'] = ( $_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid'] );
$obrid                       = $_SESSION['obras2']['obrid'];

if($_POST['xls'] == 'gerar'){
    gerarXlsItens();
}

if ($_POST['evt'] == 'salvar') {

    $cronograma = new Cronograma();
    $cronograma->cronogramaObra($obrid);


    //verificar se as data de inicio precisa ser alterada
    if (!empty($_POST['dtInicioObra'])) {
        $arObra['obrinicio'] = formata_data_sql($_POST['dtInicioObra']);
    }

    //verificar se as data de termino precisa ser alterada
    if (!empty($_POST['dtTerminoObra'])) {
        $arObra['obrtermino'] = formata_data_sql($_POST['dtTerminoObra']);
    }

    $arCamposNulo = array();
    if ($_POST['obrcronogramaservicocontratado']) {
        $arObra['obrcronogramaservicocontratado'] = $_POST['obrcronogramaservicocontratado'];
    } else {
        $arCamposNulo[] = 'obrcronogramaservicocontratado';
        $arObra['obrcronogramaservicocontratado'] = null;
    }

    if ($_POST['obrcronogramaservicocontratadojustificativa']) {
        $arObra['obrcronogramaservicocontratadojustificativa'] = $_POST['obrcronogramaservicocontratadojustificativa'];
    } else {
        $arCamposNulo[] = 'obrcronogramaservicocontratadojustificativa';
        $arObra['obrcronogramaservicocontratadojustificativa'] = null;
    }

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    /**
     * Ao se fazer o update do cronograma, deve-se travar novamente o mesmo quando houver vistorias ou quando o GESTOR MEC/SUPERUSER travar no checkbox;
     */
    if ($_POST['obrtravaedicaocronograma'] || $_POST['obrcronogramaservicocontratado'] || $_POST['obrcronogramaservicocontratadojustificativa']) {
        $arObra['obrtravaedicaocronograma'] = 't';

        if ($_POST['obrtravaedicaocronograma'] == 'N'){
            $arObra['obrtravaedicaocronograma'] = 'f';
        }

        $obra->popularDadosObjeto($arObra)->salvar(true, true, $arCamposNulo);
        $obra->commit();
    }


    $itens    = new ItensComposicaoObras();
    $allItens = $itens->getAllIcoid($obrid);

    $removeItens = array_diff($allItens, (is_array($_POST['icoid']))?$_POST['icoid']:array($_POST['icoid']) );

    if($_GET['acao'] != 'E') {
        $itens->desabilitarItensPeloIcoid($removeItens, $obrid);
    }

    $itens_modificados = array();

    for ($i = 0; $i < count($_POST['itcid']); $i++) {
        if (!empty($_POST['itcid'][$i])) {
            $arDados = array(
//	          'atiid'            					=> $atiid,
                'icoid'                 => ($_POST['icoid'][$i] ? $_POST['icoid'][$i] : null),
                'itcid'                 => ($_POST['itcid'][$i] ? $_POST['itcid'][$i] : null),
                'obrid'                 => $obrid,
                'icopercsobreestrutura' => ($_POST['icopercsobreestrutura'][$i] ? desformata_valor($_POST['icopercsobreestrutura'][$i]) : null),
                'icovlritem'            => ($_POST['icovlritem'][$i] ? desformata_valor($_POST['icovlritem'][$i]) : null),
                'icodtinicioitem'       => ($_POST['icodtinicioitem'][$i] ? formata_data_sql($_POST['icodtinicioitem'][$i]) : null),
                'icodterminoitem'       => ($_POST['icodterminoitem'][$i] ? formata_data_sql($_POST['icodterminoitem'][$i]) : null),
                'icostatus'             => 'A',
                'icodtinclusao'         => (empty($_POST['icoid'][$i]) ? 'NOW()' : null),
                'icoordem'              => ($i + 1),
                'croid'                 => $cronograma->croid
            );

            array_push($itens_modificados, $arDados['icoid']);

            $itens->popularDadosObjeto($arDados)->salvar();
            $itens->clearDados();

        }
    }

    $itens->commit();

    /**
     * @description A aba de cronograma tem edição livre desde que a obra esteja em
     *              contratação ou execução, então faz-se importante que seja gravado
     *              no registro de atividades as alterações do cronograma.
     */

    $sql_empreendimento_id = 'SELECT emp.empid 
                              FROM obras2.empreendimento emp
                              INNER JOIN obras2.obras oi ON  oi.empid = emp.empid
                              WHERE  oi.obrid = '.$obrid;

    $empid = $db->pegaUm($sql_empreendimento_id);

    include_once APPRAIZ . "obras2/classes/modelo/RegistroAtividade.class.inc";
    $regAtividade                 = new RegistroAtividade();
    $arDado                       = array();
    $arDado['arqid']              = null;
    $arDado['obrid']              = $obrid;
    $arDado['usucpf']             = $_SESSION['usucpf'];
    $arDado['empid']              = $empid;
    $arDado['rgadscsimplificada'] = 'Atualização dos dados do cronograma.';
    $arDado['rgadsccompleta']     = 'Foram modificados os seguintes itens do cronograma: ' . implode(' - ', $itens_modificados);
    $arDado['rgaautomatica']      = true;
    $arCamposNulo2                = array();

    if ( empty($arDado['arqid']) ){
        $arCamposNulo2[] = 'arqid';
    }

    $rga = $regAtividade->popularDadosObjeto( $arDado )
                        ->salvar(true, true, $arCamposNulo2);
    $regAtividade->commit();


    $solicitacaoDesembolso = new SolicitacaoDesembolso();
    $solicitacaoDesembolso->controlaInconformidadeCronogramaDesatualizado($obrid, false, true);

    if($_GET['acao'] == 'E'){
        die("<script>
                alert('Operação realizada com sucesso!');
                window.location.href = window.location.href;
             </script>");
    }else{
        die("<script>
                alert('Operação realizada com sucesso!');
                window.location = '?modulo=principal/etapas_da_obra&acao=A&obrid={$obrid}';
             </script>");

    }


}

$obraContrato = new ObrasContrato();
$obra         = new Obras();
$obra->carregarPorIdCache($obrid);
$obraMI       = (($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) && !$obra->obridvinculado) ? true : false;

if($obraMI){
    $itensComposicao  = new ItensComposicaoObras();
    $ocrvalorexecucao = $itensComposicao->getValorTotalItens($obrid);
    $osMI = new OrdemServicoMI();
    $arr_servicos_externos = $osMI->getArrayServicosExternosImplantacao($obrid);

} else {
    $ocrvalorexecucao = $obraContrato->getValorContrato($obrid);
}

if ($obraMI) {
    $ufObra = $obra->pegaUfObra($obrid);

    // Se tem aditivo, usa o valor do contrato
    if (!function_exists('obraAditivada')) {
        echo '<script> alert("Escola sem valor Aditivo, procure o administrador do seu sistema"); window.history.back();</script>';
        die();

    }
    if (!obraAditivada($obrid)) {
        $ocrvalorexecucao = $obraContrato->valorContratoObraMi($obra->tpoid, $ufObra);
    }
}


$dadosContrato = $obraContrato->pegaDadosContratoObra($obrid);
$dtLimiteInicioEtapa = formata_data($dadosContrato['ocrdtordemservico']);
$dtLimiteFimEtapa = formata_data($dadosContrato['crtdttermino']);

$obrcronogramaservicocontratadojustificativa = $obra->obrcronogramaservicocontratadojustificativa;
$obrcronogramaservicocontratado              = $obra->obrcronogramaservicocontratado;

$docid = pegaDocidObra($obrid);
//$_GET['acao'] = 'V';

if($_GET['acao'] == 'E') {

    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $habilitado = false;
    $habilita = 'N';

} elseif ($_GET['acao'] != 'V') {
    // Inclusão de arquivos padrão do sistema
    include APPRAIZ . 'includes/cabecalho.inc';
    // Cria as abas do módulo
    echo '<br>';
    if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
    } else {
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
    }

    $contrato = new Contrato();
    $crtid    = $contrato->pegaCrtidPorObrid($obrid);

    if ($crtid) {
        $habilitado = true;
        $habilita = 'S';
    } else {
        $habilitado = false;
        $habilita = 'N';
    }
} else{

    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $db->cria_aba($abacod_tela, $url, $parametros);
    $habilitado = false;
    $habilita = 'N';
}

$desabilitaWF = false;

if($_GET['acao'] != 'E') {


    if (possui_perfil(array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) {
        $habilitado = false;
        $habilita = 'N';
    }

    if(possuiTravaCronograma($obrid)) {
        $habilitado = false;
        $habilita = 'N';
    }

    $esd = wf_pegarEstadoAtual($docid);
    if($esd['esdid'] == ESDID_OBJ_CONTRATACAO){
        $habilitado = true;
        $habilita = 'S';
    }

    if($obraMI){
        $habilitado = false;
        $habilita = 'N';
    }

    $flag_obrtravaedicaocronograma = $obra->getTravaCronograma($obrid);

    echo cabecalhoObra($obrid);

    echo alertaObraMi($obrid);

    monta_titulo_obras($titulo_modulo, '<b>Após inserir, atualizar ou excluir uma etapa, clique em salvar.</b>
                    <div class="row" style="background: #b94a48; color: #fff; margin: 3px;"><img src="/imagens/atencao.png" /> Se o total dos itens do 
                    cronograma for diferente do valor do contrato lançado na aba Contratação, o sistema bloqueará a inserção de vistorias. 
                    Para a tramitação para execução o valor restante no cronograma deve ser sempre igual à zero.</div>');

    echo cabecalhoCronograma($obrid);

}else{

    echo cabecalhoObra($obrid);
    monta_titulo_obras($titulo_modulo, '<b>Após atualizar uma etapa, clique em salvar.</b>');
    $habilitado = true;
    $habilita = 'S';
    $desabilitaWF = true;

}