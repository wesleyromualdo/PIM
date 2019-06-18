<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/27/2017
 * Time: 2:20 PM
 */
require APPRAIZ.'includes/cabecalho.inc';
require APPRAIZ.'includes/Agrupador.php';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

$controleUnidade         = new Par3_Controller_InstrumentoUnidade();
$controllerObra = new  Par3_Controller_Obra();
$controllerMunicipio = new Par3_Controller_Municipio();
$controllerEstadoDocumento = new Par3_Controller_EstadoDocumento();
$controllerSegurancaPerfilUsuario = new Par3_Controller_PerfilUsuario();
$controllerSegurancaUsuario = new Par3_Controller_Usuario();
$listagemSimec = new Simec_Listagem();
$controllerObraTipo = new Par3_Controller_ObraTipo();
$controllerEtapaEnsino = new Par3_Controller_EnsinoEtapa();
$controllerModalidade = new Par3_Controller_Modalidade();
$controllerAgrupador = new Par3_Controller_Agrupador();
$modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);
$controllerDocumentoTermo = new Par3_Controller_DocumentoTermo();


if (!$arrPermissao || !$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}

/*************************Alterações no Formulário de acorco com o perfil******************************************/
$disableEstado = false; //Desabilita os campos de pesquisa
$disableMunicipio = false; //Desabilita os campos de pesquisa
$sqlPelaunidade = false;
$permissao = $controllerObra->permissaoAcessoObrasMenuPlanejamento();
switch($permissao){
    case 1:
        $disableEstadoMunicipio = false;
        $sqlPelaunidade = false;
        break;
    case 2:
        $disableEstadoMunicipio = 'disabled';
        $arrEstadoMunicipioByCPFUsuario = $controllerSegurancaUsuario->getDadosDoUsuarioByCPF($_SESSION['usucpf']);
        $esferaUnidade=$controleUnidade->buscaEsferaUnidade($_REQUEST['inuid']);
        if($esferaUnidade == 'E' || $esferaUnidade == 'D'){
            $disableEstado =  'disabled';
            $_REQUEST['estuf'] = $arrEstadoMunicipioByCPFUsuario['regcod'];
            $_POST['estuf'] = array('estuf' => $arrEstadoMunicipioByCPFUsuario['regcod']);
        }else if($esferaUnidade == 'M' ){
            $_REQUEST['estuf'] = $arrEstadoMunicipioByCPFUsuario['regcod'];
            $_REQUEST['muncod'] = $arrEstadoMunicipioByCPFUsuario['muncod'];

            $_POST['estuf'] = array('estuf' => $arrEstadoMunicipioByCPFUsuario['regcod']);
            $_POST['muncod'] = array('muncod' => $arrEstadoMunicipioByCPFUsuario['muncod']);

            $disableEstado = 'disabled';
            $disableMunicipio = 'disabled';
        }


        $sqlPelaunidade = true;
    case 0:
        simec_redirecionar('par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='.$_REQUEST['inuid'], 'error', 'Acesso negado.');
        $disableEstadoMunicipio = 'disabled';
        $sqlPelaunidade = true;
        break;
}

switch ($_REQUEST['method']) {
    case 'visualizarTermoPar3':
        ob_clean();
        $mdoconteudo = $controllerDocumentoTermo->pegaTermoCompromissoArquivo($_REQUEST['dotid']);
        $mdoconteudo = str_ireplace('\"', '"', $mdoconteudo);
        echo simec_html_entity_decode($mdoconteudo);

        exit();
        break;
    case 'imprimir':
        ob_clean();
        $controllerDocumentoTermo->imprimirTermoPDF($_REQUEST['dotid']);
        exit();
        break;
}

/*************************END Alterações no Formulário de acorco com o perfil******************************************/


//"Controller"
if(isset($_REQUEST['toReturn']) && $_REQUEST['toReturn'] == 'formListaObraPlanejamento'){
    $_REQUEST = $_SESSION['formListaObraPlanejamento'];
    $_REQUEST['inuid'] = $_GET['inuid'];

}
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
$perfisPermitidos = [
    PAR3_PERFIL_SUPER_USUARIO,
    PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR,
    PAR3_PERFIL_COORDENADOR_CGEST
];
$permiteTramitar = false;
if(array_intersect($perfisPermitidos, $perfis)){
    $permiteTramitar = true;
}
if(isset($_POST['filtrar'])){
    $_SESSION['toReturn'] = $_POST;
    $_POST['inuid'] = $_REQUEST['inuid'];
    $listagemSimec = new Simec_Listagem();
    $arListagem = $controllerObra->getSqlListagemObras( $_POST,false);


        $arrayCabecalho = array(
            '<small>Pré-Obra</small>',
            '<small>Nome</small>',
            '<small>Tipo<br/>obra</small>',
            '<small>Esfera</small>',
            '<small>Município</small>',
            '<small>UF</small>',
            '<small>Escola</small>',
            '<small>Situação</small>',
            '<small>Obras 2.0</small>',
            '<small>Processo</small>',
            '<small>Etapa</small>' ,
            '<small>Ano</small>',
            '<small>Contrapartida</small>',
            '<small>Valor<br/>MEC/FNDE</small>',
            '<small>Valor<br/>Obra</small>',
            '<small>Pagamento</small>',
            '<small>Termo</small>',
            '<small>Empenho</small>'
        );
        $arrayEsconder = array('obrdsc','unidade','iniciativapla','edit','situacao_estado_documento_id', 'obrid','situacao_estado_documento','obrcheck','obra2','obra2_esddsc', 'dotid');
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($arListagem);

        if($permiteTramitar) {
            $listagemSimec->addCallbackDeCampo('obrcheck', 'checkbox_aprovar_em_lote');
            unset($arrayEsconder[7]);
            array_unshift($arrayCabecalho,'');
        }

        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($arrayEsconder);
        $listagemSimec->setFormFiltros('formulario-filtro-obras');
//        $listagemSimec->addAcao('edit',array('func' => 'js_btnEditarObraListaDeObras', 'extra-params' => array('unidade','iniciativapla','preid')));
        $listagemSimec->addCallbackDeCampo('desdobramentos', 'listaDesdobramentosListaObras');
        $listagemSimec->addCallbackDeCampo('escnome', 'iconEscolaListaObras');
        $listagemSimec->addCallbackDeCampo('linkeditarobr', 'btnEditarObraPlanejamento');
        $listagemSimec->addCallbackDeCampo('docid', 'popOverSituacaoObra');
        $listagemSimec->addCallbackDeCampo('obrvalor','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('obrvalor_contrapartida','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('Empenho','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('linkobra2', 'linkObras2');
        $listagemSimec->addCallbackDeCampo('valor_fnde','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('vlrpago','formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo('Termo','formataTipoDocumentoParams');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS,'preid');
        $listagemSimec->addCallbackDeCampo('Processo','formata_numero_processo');
        $listagemSimec->setId('listaObras');
        $_REQUEST['filtroObras'] = $_POST;
}

//IMPRESSÃO XLS
if($_GET['method'] == 'xls'){
    ob_clean();
    $_POST['inuid'] = $_REQUEST['inuid'];
    echo $controllerObra->imprimeObrasXls($_REQUEST,false);
    header("Content-Disposition: attachment; filename=Relatorio_Lista_de_Obras.xls");
    die;
}

if($_GET['requisicao'] == 'acessarobras2'){
    $obrid = $_GET['obrid'];
    $_SESSION['obras2']['orgid'] = $obrid;
    ob_clean();
    echo '<script type="text/javascript">window.location.href=\''.$_SERVER['HTTP_ORIGIN'].'/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='.$obrid.'\'</script>';exit();
}

switch ($_POST['method']){
    case 'getListaMunicipioByUF':
        ob_clean();
        if(isset($_POST['estuf'])){
            $arrAttr = array (
                'data-placeholder' => 'Selecione',
                'multiple' => 'multiple'
            );
            /*Monta o input do tipo select com a lista de municípios com/sem UF*/
            $sql = $controllerMunicipio->getFormListaMunicipioByUF($_POST['estuf']);
            echo $simec->select('muncod','Municípios','Selecione', $sql,$arrAttr);
        }elseif ($_POST['estuf'] == "null"){
            $arrAttr = array (
                'data-placeholder' => 'Selecione',
                'placeHolder' => 'Selecione',
                'multiple' => 'multiple',
            );
            /*Monta o input do tipo select com a lista de municípios com/sem UF*/
            $sql = $controllerMunicipio->getFormListaMunicipioByUF();
            echo $simec->select('muncod','Municípios','Selecione', $sql,$arrAttr);
        }

        die();
        break;
    case 'getTecnicosStatus':
        ob_clean();
        if(isset($_POST['tecnicosStatus'])){
            $arrAttrTecnicos = array(
                'placeHolder' => 'Selecione',
            );
            $sqlTecnicos = $controllerSegurancaPerfilUsuario->getSqlPerfilTecnicoAnaliseEfetuada($_POST['tecnicosStatus']);
            echo $simec->select('usucpf', '','',$sqlTecnicos, $arrAttrTecnicos);
        }
        die();
        break;
    case 'verificaraprovacaoemlote':
        ob_clean();
        $arrObras = $_POST['obras'];
        echo $controllerObra->validarAprovacaoObras($arrObras);die;
        break;
    case 'carregarformtramitacao':
        ob_clean();
        require APPRAIZ.'/par3/modulos/principal/formComentarioTramitacao.inc';die;
        break;
    case 'tramitarobrasemlote':
        ob_clean();
        echo $controllerObra->tramitarEmLote($_POST['arrdocid'],$_POST['aedid'],$_POST['docdsc']);die;
        break;
    case 'historicoSituacao':
        ob_clean();
        echo $controllerObra->listarHistoricoTramitacaoModal($_POST['docid']);die;
        break;
    case 'recuperarcomentario':
        ob_clean();
        $comentario = $db->recuperar("SELECT cmddsc FROM workflow.comentariodocumento WHERE hstid = {$_POST['hstid']}");
        echo simec_json_encode($comentario['cmddsc']);die;
        break;
    case 'getModalidadesByEnsinoEtapa':
        ob_clean();
        $sqlModalidadeByEtapa = '';
        $arrayModalidade = array(
            'data-placeholder' => 'Selecione',
            'multiple' => 'multiple'
        );
        if(isset($_POST['etaid']) && count($_POST['etaid']) > 0){
            $sqlModalidadeByEtapa = $controllerModalidade->getFormListModalidadeByEnsinoEtapa($_POST['etaid']);
        }else{
            $sqlModalidadeByEtapa = $controllerModalidade->getFormListModalidade();
        }
        echo $simec->select('modid','Modalidade','Selecione',$sqlModalidadeByEtapa,$arrayModalidade);
        die();
        break;
}

if($permissao == 1 || $permissao == 2){
?>
    <style>
        .input_text{
            border: 1px solid #e5e6e7;
        }
        #listaObras a{
            color:blue;
            height:100%;
            width:100%;
            display: block;
            padding:10px;
        }
        #listaObras td {
            margin: 0px;
            padding:2px;
            padding-top:0px;
            padding-botton:0px;
        }
        #listaObras a:hover{
            color:cornflowerblue;

        }
    </style>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
            <div class="ibox-content">
                <?php if(!$_REQUEST['requisicao']) $controleUnidade->cabecalhoUnidade(); ?>

                    <div class="row">
                        <div class="ibox">

                                <div class="row">
                                    <div class="col-md-12">

                                        <!-- Filtro -->
                                        <div class="float-e-margins" id="filtro-obras">
                                            <div class="ibox-title" id="btn-filtro-obras">
                                                   <h3 class="">Filtro</h3>
                                            </div>
                                            <div class="ibox-content" id="div-filtro-obras" style="display: block;">
                                                <form method="post" name="formulario-filtro-obras" id="formulario-filtro-obras" class="form form-horizontal">
                                                    <input type="hidden" name="submited" id="submited" value="t">
                                                    <div class="row">
                                                        <div class="col-md-2"></div>
                                                        <div class="col-md-8">
                                                            <input type="hidden" value="filtrar" name="filtrar">
                                                            <?php
                                                            echo $simec->input('preid', 'Pré-Obra', $_REQUEST['preid'], array()); //Obras que estão cadastradas em par3.obras
                                                            ?>
                                                            <div id="divCategoriasObra">
                                                                <?php
                                                                $_REQUEST['octid'] = tratarArrayParaMultiSelect($_REQUEST['octid']);
                                                                $cTipoObra = new Par3_Controller_ObraTipo();
                                                                $arrCategorias = simec_preparar_array($cTipoObra->pegarSelectCategoriasComboArray());
                                                                echo $simec->select('octid', 'Categoria da Obra', $_REQUEST['octid'], $arrCategorias, array('maxlength' => '255','multiple' => 'multiple'), array());
                                                                ?>
                                                            </div>
                                                            <?php
                                                            $sqlTipoObra = $controllerObraTipo->getSQLInputTipoObrasTipo();
                                                            $arrAttrTipoObra = array (
                                                                'data-placeholder' => 'Selecione',
                                                                'multiple' => 'multiple'
                                                            );
                                                            echo $simec->select ( 'otpid', 'Tipo de Obra', $_REQUEST['otpid'], $sqlTipoObra, $arrAttrTipoObra ); //estado 304 tipos de obras no workflow.estadodocumento
                                                            ?>

                                                            <?php
                                                            $sqlAnosObras = $controllerObra->getSqlAnosExistentesObrasPar();
                                                            $arrAttrAnoObras = array (
                                                                'data-placeholder' => 'Selecione',
                                                            );
                                                            echo $simec->input('obrano','Ano',$_REQUEST['obrano'],$arrAttrAnoObras);


                                                            ?>

                                                            <?php
                                                            $arrNumTermo = array (
                                                                'placeHolder' => 'Informe o nr° do termo',
                                                            );
                                                            echo $simec->input('dotnumero', 'N° Termo', $_REQUEST['dotnumero'], $arrNumTermo);

                                                            $arrNumProcesso = array (
                                                                'placeHolder' => 'Informe o nr° do processo',
                                                            );
                                                            echo $simec->input('pronumeroprocesso', 'N° Processo', $_REQUEST['pronumeroprocesso'], $arrNumProcesso);
                                                            ?>

                                                            <div class="form-group">
                                                                <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Empenho:</label>
                                                                <div class="col-sm-12 col-md-9 col-lg-8 ">
                                                                    <div id="empenho_group_id">
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type='radio'  name='empenhosituacao' id='empenho_empenhada' value='A' <?=$_POST['empenhosituacao'][0] == 'A' ? 'checked' : ''?>>
                                                                            <label for="empenho_empenhada">Obra empenhada</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type="radio" name="empenhosituacao" id="empenho_nempenhada" value="I" <?=$_POST['empenhosituacao'][0] == 'I' ? 'checked' : ''?>>
                                                                            <label for="empenho_nempenhada">Obra não empenhada</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type="radio" name="empenhosituacao" id="empenho_todos" value=""  <?=empty($_POST['empenhosituacao'][0]) ? 'checked' : ''?>>
                                                                            <label for="empenho_todos">Todos</label>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <label class=" col-sm-3 control-label">Pendências no Monitoramento de Obras:</label>
                                                                <div class="col-sm-12 col-md-9 col-lg-4">
                                                                    <div id="pendenciamonitoramento_group_id disabled">
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type='radio' name='pendenciamonitoramento' id='pendenciamonitoramento_sim' value='S' <?=$_POST['pendenciamonitoramento'] == 'S' ? 'checked' : ''?>>
                                                                            <label for="pendenciamonitoramento_sim">Sim</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type="radio" name="pendenciamonitoramento" id="pendenciamonitoramento_nao" value="N" <?=$_POST['pendenciamonitoramento'] == 'N' ? 'checked' : ''?>>
                                                                            <label for="pendenciamonitoramento_nao">Não</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type="radio" name="pendenciamonitoramento" id="pendenciamonitoramento_todos" value="" <?=empty($_POST['pendenciamonitoramento']) ? 'checked' : ''?>>
                                                                            <label for="pendenciamonitoramento_todos">Todos</label>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <label class=" col-sm-3 control-label">Termo:</label>
                                                                <div class="col-sm-9">
                                                                    <div id="pendenciamonitoramento_group_id">
                                                                        <div class="radio radio-default radio-inline">
                                                                            <input type='radio'  name='dotstatus' id='termo_gerado' value='A' <?=$_POST['dotstatus'] == 'A' ? 'checked' : ''?>>
                                                                            <label for="termo_gerado">Gerado</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline">
                                                                            <input type="radio" name="dotstatus" id="termo_nao_gerado" value="I" <?=$_POST['dotstatus'] == 'I' ? 'checked' : ''?>>
                                                                            <label for="termo_nao_gerado">Não Gerado</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline">
                                                                            <input type="radio" name="dotstatus" id="termo_todos" value="" <?=empty($_POST['dotstatus']) ? 'checked' : ''?>>
                                                                            <label for="termo_todos">Todos</label>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <label class=" col-sm-3 control-label">Pagamento:</label>
                                                                <div class="col-sm-9">
                                                                    <div id="pendenciamonitoramento_group_id">
                                                                        <div class="radio radio-default radio-inline">
                                                                            <input type='radio'  name='pagamento' id='pagamento_solicitado' value='enviado ao sigef' <?=$_POST['pagamento'] == 'enviado ao sigef' ? 'checked' : ''?>>
                                                                            <label for="pagamento_solicitado">Pagamento Solicitado</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline">
                                                                            <input type="radio" name="pagamento" id="pagamento_n_solicitado" value="não solicitado" <?=$_POST['pagamento'] == 'não solicitado' ? 'checked' : ''?>>
                                                                            <label for="pagamento_n_solicitado">Pagamento Não Solicitado</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline">
                                                                            <input type="radio" name="pagamento" id="pagamento_efetivado" value="efetivado" <?=$_POST['pagamento'] == 'efetivado' ? 'checked' : ''?>>
                                                                            <label for="pagamento_efetivado">Pagamento Efetivado</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline">
                                                                            <input type="radio" name="pagamento" id="pagamento_todos" value="" <?=empty($_POST['pagamento']) ? 'checked' : ''?>>
                                                                            <label for="pagamento_todos">Todos</label>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <!-- Obras Migradas para sistema do Obras 2-->
                                                            <div class="form-group" >
                                                                <label class=" col-sm-3 control-label">Obras Migradas para sistema do Obras 2:</label>
                                                                <div class="col-sm-12 col-md-9 col-lg-4">
                                                                    <div id="pendenciamonitoramento_group_id">
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type='radio' name='obrid2' id='obrasmigradas_s' value='S' <?=$_POST['obrid2'] == 'S' ? 'checked' : ''?> >
                                                                            <label for="inisituacao1">Sim</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type="radio" name="obrid2" id="obrasmigradas_n" value="N" <?=$_POST['obrid2'] == 'N' ? 'checked' : ''?> >
                                                                            <label for="inisituacao2">Não</label>
                                                                        </div>
                                                                        <div class="radio radio-default radio-inline ">
                                                                            <input type="radio" name="obrid2" id="obrasmigradas_t" value="T" <?=empty($_POST['obrid2']) ? 'checked' : ''?> >
                                                                            <label for="inisituacao2">Todos</label>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                            </div>
                                                            <div id="DivInputEtaid">
                                                                <?php
                                                                //Ensino Etapa
                                                                $sqlEnsinoEtapa = $controllerEtapaEnsino->getListEnsinoEtapa();
                                                                $arrEnsinoEtapa = array(
                                                                    'placeHolder' => 'Selecione',
                                                                    'multiple' => 'multiple'
                                                                );
                                                                echo $simec->select('etaid','Etapa',$_POST['etaid'],$sqlEnsinoEtapa,$arrEnsinoEtapa);
                                                                ?>
                                                            </div>
                                                            <div id="divInputModalidade">
                                                                <?php
                                                                //Modalidade
                                                                $sqlModalidade = $controllerModalidade->getFormListModalidade();
                                                                $arraModalidade = array(
                                                                    "data-placeholder" => 'Selecione',
                                                                    "multiple" => "multiple"
                                                                );
                                                                echo $simec->select("modid","Modalidade",$_POST['modid'],$sqlModalidade,$arraModalidade);
                                                                ?>
                                                            </div>
                                                            <?php
                                                            //pendente - funcionalidade do agrupador não desenvolvida,
                                                        ?>
                                                            <div id="divTiposAssistencia">
                                                                <?php
                                                                $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
                                                                $mInta = $controllerIniciativaTiposAssistencia->recuperar();
                                                                $arrTiposAssistencia = simec_preparar_array($mInta->recuperarTodosFormatoInput('intadsc'));
                                                                $arrTiposAssistencia[0] = 'Todos';
                                                                echo $simec->select('intaid','Tipos de Assistência',$_POST['intaid'],$arrTiposAssistencia,['placeHolder' => 'Selecione']);
                                                                ?>
                                                            </div>
                                                            <div class="row center">
                                                                <div class="col-md-12">
                                                                    <div class="col-md-4"></div>
                                                                    <div class="col-md-6">
                                                                        <button type="submit" class="btn btn-info" id="btn-pesquisa"><i class="fa fa-search"></i>Pesquisar</button>
                                                                        <?php if($permiteTramitar):?>
                                                                            <button type="button" class="btn btn-warning" id="btn-aprovar-em-lotes"><i class="fa fa-check"></i>Tramitar Em Lote</button>
                                                                        <?php endif;?>
                                                                        <button type="button" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                                                                    </div>
                                                                    <div class="col-md-4"></div>
                                                                </div>
                                                            </div>
                                                        <div class="col-md-2"></div>
                                                    </div>
                                                </form>
                                            </div>
                                        </div>



                                        <!-- Listagem  -->
                                        <div class="float-e-margins" id="listagem-obras">
                                            <div class="ibox-title" id="btn-listagem-obras">
                                                <h3 class="">Listagem</h3>
                                            </div>
                                            <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                                                <div class="table-responsive" style="overflow: scroll; padding-bottom: 5px;">
                                                    <div style="padding-top:100px;">
                                                        <div id="listaObrasID" class="table-responsive">
                                                            <?php

                                                            if(isset($_POST['filtrar'])) {
                                                                $_SESSION['filtroObraPlanejamento'] = $_POST;
                                                                    $listagemSimec->render();
                                                            }else{
                                                                ?>
                                                                <div class='row'>
                                                                    <div class='col-md-4'></div>
                                                                    <div class='col-md-4'>
                                                                        <div class="alert alert-info" role="alert">Nenhum registro encontrado!</div>
                                                                    </div>
                                                                    <div class='col-md-4'></div>
                                                                </div>
                                                            <?php } ?>
                                                        </div>
                                                    </div>
                                                    <tr><td>
                                                            <p>Legenda:</p>
                                                            <span style="color: green;">(EP) = Emenda Parlamentar</span>
                                                        </td>
                                                    </tr>
                                                </div>
                                                <br>
                                                <br>
                                            </div>
                                        </div>
                                        <!-- END Listagem  -->


                                    </div>
                                </div>

                            </div>
                    </div>

            </div>
        </div>
    </div>
</div>
    <div class="ibox float-e-margins animated modal conteudo" id="modal-visualiza-analise" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
            <div class="modal-content">
                <div class="ibox-content">
                    <div class="row">
                        <div class="ibox-content div_html" >
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="ibox float-e-margins animated modal" id="modal_aprovacao_lote" role="dialog" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo" align="center">Tramitar em Lote</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal"></div>
                <div class="ibox-footer">
                    <div class="row">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Fechar
                        </button>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="ibox float-e-margins animated modal" id="modal_historico" role="dialog" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo" align="center">Histórico</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal"></div>
                <div class="ibox-footer">
                    <div class="row">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Fechar
                        </button>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="ibox float-e-margins animated modal" id="modal_tramitar" role="dialog" aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 align="center">Confirmar Tramitação</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal"></div>
            </div>
        </div>
    </div>

    <!-- Modal Termo -->
    <div class="ibox float-e-margins animated modal conteudo" id="modal-visualiza-termo" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
            <div class="modal-content">
                <div class="ibox-footer notprint" style="text-align: center;">
                    <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                    <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
                </div>
                <div class="ibox-content">
                    <div class="row">
                        <div class="ibox-content" >
                            <?php echo $controllerDocumentoTermo->montaHtmlTermo(); ?>
                        </div>
                    </div>
                </div>
                <div class="ibox-footer notprint" style="text-align: center;">
                    <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                    <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
                </div>
            </div>
        </div>
    </div>

<?php } ?>
<script>
    $(".xls").on("click", function () {
        window.location.assign(window.location.href + "&method=xls&" + jQuery('#formulario-filtro-obras').serialize() + '&' + $('.form-listagem').serialize());
    });
    //desabilita o botão next por ser o último item do menu
    $(window).load(function() {
        setTimeout(function(){ $('#btn-proximo-nav').attr('disabled','disabled'); }, 2000);

        <?php if(empty($_POST['submited'])): ?>
            divCarregando();
            $("#btn-pesquisa").trigger("click");
        <?php endif; ?>
    });

        //mostra/esconde ibox Execução
    jQuery("#btn-execucao-obras").click(function () {
        $('#div-execucao-obras').slideToggle();
        //chevron up/down
        $('#i-execucao-obras').toggleClass(function () {
            if ($('#i-execucao-obras').is(".fa-chevron-down")) {
                $('#i-execucao-obras').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-execucao-obras').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //mostra/esconde ibox Filtro
    jQuery("#btn-filtro-obras").click(function () {
        $('#div-filtro-obras').slideToggle();
        //chevron up/down
        $('#i-filtro-obras').toggleClass(function () {
            if ($('#i-filtro-obras').is(".fa-chevron-down")) {
                $('#i-filtro-obras').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-filtro-obras').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //mostra/esconde ibox opções avançadas do filtro
    jQuery("#btn-filtro-obras-op-avancada").click(function () {
        $('#div-filtro-obras-op-avancada').slideToggle();
        //chevron up/down
        $('#i-filtro-obras-op-avancada').toggleClass(function () {
            if ($('#i-filtro-obras-op-avancada').is(".fa-chevron-down")) {
                $('#i-filtro-obras-op-avancada').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-filtro-obras-op-avancada').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });


    /**
     * onchanger no campo estado
     */
    $('#estuf').change(function(){
        var estuf = $("#estuf").val();
        var data = '&method=getListaMunicipioByUF&estuf='+estuf;
        $.ajax({
            type: "POST",
            url: window.location,
            data: data,
            async: false,
            success: function (resp) {
                $("#inputMunicipio").html(resp);
            }
        });
    });

    /**
     * onchanger no campo Técnicos Ativos / Todos
     */
    $('#statusTecnico').click(function(){
        var statusTecnico = $( "input[type=radio][name=statusTecnico]:checked" ).val();
        var data = '&method=getTecnicosStatus&tecnicosStatus='+statusTecnico;
        $.ajax({
            type: "POST",
            url: window.location,
            data: data,
            async: false,
            success: function (resp) {
                $("#inputSelectTecnicos").html(resp);
            }
        });
    });

    /**
     * onchanger no campo Técnicos Ativos / Todos
     */
    $('#etaid').change(function(){
        var action = window.location;
        var data = '&method=getModalidadesByEnsinoEtapa&modalidade='+$('#formulario-filtro-obras').serialize();
        $.ajax({
            type: "POST",
            url: window.location,
            data: data,
            async: false,
            success: function (resp) {
                $("#divInputModalidade").html(resp);
            }
        });
    });


    /**
     * valida campo da obra para aceitar vírgula
     */
    $("#preid").keyup(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorClean = js_limpaValoresInputAceitarSomenteNumerosVirgula($this.val());
        $this.val(valorClean);
    });

    /**
     * valida campo da obra para aceitar vírgula
     */
    $("#obrano").keyup(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorClean = js_limpaValoresInputAceitarSomenteNumerosVirgula($this.val());
        $this.val(valorClean);
    });

    /**
     * valida campo preid quando perder o focu
     */
    $("#preid").blur(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorAoPerderFocu = $this.val();
        cleanValue = js_limpaStringCaractereInicialFinal(valorAoPerderFocu,',');
        $this.val(cleanValue);
    });

    /**
     * valida campo preid quando perder o focu
     */
    $("#obrano").blur(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorAoPerderFocu = $this.val();
        cleanValue = js_limpaStringCaractereInicialFinal(valorAoPerderFocu,',');
        $this.val(cleanValue);
    });

    function js_editarListaObra(value){
        console.log(value);
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param valor | string <String separada por determinado caracteres 'qualCaractere'>
     * @param qualCaractere | String <String que deve ser tirada do início e do fim>
     * @example stringVal: ,10 ,50 ,87,15      ,18,
     *          função: js_limpaStringCaractereInicialFinal(stringVal,',');
     *          retorno: 10,50,87,15,18
     *
     */
    function js_limpaStringCaractereInicialFinal(valor,qualCaractere){
        valor = valor.replace(/\s/g,''); //remove espaços em branco
        valor = valor.toLowerCase(); //passa todas as letras para
        console.log(valor);
        arrayValor = valor.split(qualCaractere);
        cleanVal = arrayValor.filter(function(her){
            return her !== '';
        });
        return cleanVal.join(',');
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param stringSuja
     * @return {string}
     * @description Esta função limpa string permitindo somente números e vírgula
     */
    function js_limpaValoresInputAceitarSomenteNumerosVirgula(stringSuja){
        var valor = stringSuja.toLowerCase();
        //var valor = valor.replace(new RegExp('[áàâãéêèëíìïîóòöõúùü~´`]','gi'), '');
        //var valor = valor.replace(new RegExp('[^><:;/¹²³?]','gi'), '');
        //var valor = valor.replace(new RegExp('[!@#$%¨&*+_-¬£¢çÇº°ª{}.()]','gi'), '');
        var valor = valor.replace(/[a-zA-Z]+$|[/aáàâãéêèëíìïîãoóòöõúùüýç~´`!*+.'"!@#$%¨&*()_{}^´´.><:;/°ºª¹²³|?\-\=\[\]]|[\b]+,/,'');
        var valor = valor.replace(' ',',');
        var valor = valor.replace('  ','');
        var valor = valor.replace(',,',',');
        return valor;
    }

    /**
     *
     * @param id
     * @param inu
     * @param inp
     * @param obrid
     */
    function js_btnEditarObraListaDeObras(id,inu,inp,obrid){
       window.open('par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid='+inu+'&inpid='+inp+'&obrid='+obrid+'&toReturn=formListaObraPlanejamento','_blank');
    }

    // /**
    //  *
    //  * @param id
    //  * @param inu
    //  * @param inp
    //  * @param obrid
    //  */
    // function js_btnEditarObraListaDeObras(id,inu,inp,obrid){
    //     window.location.href = 'par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid='+inu+'&inpid='+inp+'&obrid='+obrid+'&toReturn=formListaObra';
    // }

    $(document).ready(function(){
        $('.i-checks').iCheck({//Função para utilizar o checkbox estilizado
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green',
        });

        $('body').on('click','.termo_detalhe', function(){
            id = $(this).find('.dotid').attr('value');
            visualizarTermoPar3(id);
        });
    });





    function ajax_verificarObrasAprovacaoEmLote(obras){
        var action = window.location.href;
        var data = {method:'verificaraprovacaoemlote',obras:obras};
        $.ajax({
            type: "POST",
            url: action,
            data: data,
            async: false,
            success: function (resp) {
                if(resp == '0'){
                    alert('<h3>Selecione apenas Obras com a mesma situação.</h3>');
                    return false;
                }
                $('#modal_aprovacao_lote').find('#conteudo-modal').html('');
                $('#modal_aprovacao_lote').modal();
                $('#modal_aprovacao_lote').find('#conteudo-modal').html(resp);
            }
        });
    }

    $('#btn-aprovar-em-lotes').on('click',function(event){
        event.preventDefault();
        var obras = [];
        obras = $('input[name=obrcheck]:checked').map(function(){
            return $(this).val();
        }).get();

        if(obras.length <= 0){
            alert('<h3>Nenhuma obra selecionada</h3>');
            return false;
        }
        ajax_verificarObrasAprovacaoEmLote(obras);
    });


    function ajax_informarSituacao(docid,aedid,comentario)
    {
        var data = {method:'carregarformtramitacao',arrdocid:docid,aedid:aedid,comentario:comentario};
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: data,
            async: false,
            success: function (resp) {
                $('#loading').hide();
                $("#modal_tramitar").find('#conteudo-modal').html('');
                $("#modal_tramitar").modal();
                $('#modal_aprovacao_lote').modal('hide');
                $("#modal_tramitar").find('#conteudo-modal').html(resp);
            }
        });
    }

    $(document).on('click','#btnFecharTramitacao',function(){
        $('#modal_tramitar').modal('hide');
        $("#modal_tramitar").find('#conteudo-modal').html('');
        $('#modal_aprovacao_lote').modal('show');
    });



    $('[data-toggle="popover"]').popover();

    function mostrarHistoricoSituacao(docid)
    {
        console.log(docid);
        var data = {method:'historicoSituacao',docid:docid};
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: data,
            async: false,
            success: function (resp) {
                $('#loading').hide();
                $("#modal_historico").find('#conteudo-modal').html('');
                $("#modal_historico").modal();
                $("#modal_historico").find('#conteudo-modal').html(resp);
            }
        });
    }

    $("#modal-visualiza-analise").on('hidden.bs.modal',function(){
        $(this).find('.div_html').html('');
    });

    function historicoBootstrapComentario(hstid) {
        var id = '#arow-' + hstid;
        console.log(hstid);
        var parentTr = $(id).closest('tr');

        if ($(id + ' span').hasClass('open')) {
            parentTr.next().remove();
            $(id + ' span').removeClass('open').removeClass('btn-default').addClass('btn-info');
        } else {
            $(id + ' span').addClass('open').removeClass('btn-info').addClass('btn-default');
            ;
            var numCols = $('td', parentTr).length;
            numAcao = parentTr.parents('table').attr('data-qtd-acoes');
            td_acao = '<td colspan="' + numAcao + '">&nbsp;</td>';

            var caminho = window.location.href;
            var action = '&method=recuperarcomentario&hstid=' + hstid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $('#loading').hide();
                    var comentario = $.parseJSON(resp);
                    console.log(comentario);
                    parentTr.after('<tr>' + td_acao + '<td colspan="' + numCols + '"><blockquote style="text-align:left">' + comentario + '</blockquote></td></tr>');
                }
            });
        }
    }

    function visualizarTermoPar3(id) {
        console.log(id);
        //jQuery('.btn-aceite-termo').button('reset');
        jQuery('[name="nome_termo"]').val(id);

        var caminho = window.location.href;
        var action  = 'method=visualizarTermoPar3&dotid='+id;
        jQuery.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#div_conteudo_termo").html(resp);
                jQuery('[name="dotid"]').val(id);
            }
        });
        jQuery("#modal-visualiza-termo").modal("show");
    }

    function imprimirTermo(){
        var dotid = jQuery('[name="dotid"]').val();
        var domid = jQuery('[name="domid"]').val();
        window.location.href = window.location+'&method=imprimir&dotid='+dotid;
    }
</script>