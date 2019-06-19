<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 9/13/2017
 * Time: 3:18 PM
 */
require APPRAIZ.'includes/cabecalho.inc';
require APPRAIZ.'includes/Agrupador.php';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';
require_once APPRAIZ.'/includes/workflow.php';

//ver(Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST); exit();
echo $simec->title('Lista de Obras', '');
$controllerObra = new  Par3_Controller_Obra();
$controllerMunicipio = new Par3_Controller_Municipio();
$controllerEstadoDocumento = new Par3_Controller_EstadoDocumento();
$controllerSegurancaPerfilUsuario = new Par3_Controller_PerfilUsuario();
$listagemSimec = new Simec_Listagem();
$controllerObraTipo = new Par3_Controller_ObraTipo();
$controllerEtapaEnsino = new Par3_Controller_EnsinoEtapa();
$controllerModalidade = new Par3_Controller_Modalidade();
$controllerAgrupador = new Par3_Controller_Agrupador();
$obraDocumento = new Par3_Controller_ObraDocumentos();
$controllerDocumentoTermo = new Par3_Controller_DocumentoTermo();


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

$arrRequisicoes = array('salvarDocumentoAnalise', 'modal_analise_profe', 'download','verificaraprovacaoemlote','carregarformtramitacao','tramitarobrasemlote','historicoSituacao','recuperarcomentario','xls');
if(isset($_REQUEST['toReturn']) && $_REQUEST['toReturn'] == 'formListaObra' && !in_array($_REQUEST['method'],$arrRequisicoes)){
    $_REQUEST = $_SESSION['toReturn'];
}

if($_GET['requisicao'] == 'acessarobras2'){
    $obrid = $_GET['obrid'];
    $_SESSION['obras2']['orgid'] = $obrid;
    ob_clean();
    echo '<script type="text/javascript">window.location.href=\''.$_SERVER['HTTP_ORIGIN'].'/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='.$obrid.'\'</script>';exit();
}
//ajax Carregar os dados do Form
switch ($_REQUEST['method']){
    case 'carregaMunicipios':
        ob_clean();
        $municipio = new Territorios_Model_Municipio();
        echo simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
        die;
        break;
    case 'modal_analise_profe':
        ob_clean();
        $obraDocumento->montaModalAnaliseProfe($_REQUEST['obrid'], $_REQUEST['inuid']);
        die();
        break;
    case 'salvarDocumentoAnalise':
        ob_clean();
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $retorno = false;
        if( $_REQUEST['requisicao'] == 'S' ){
            $retorno = $obraDocumento->salvarAnaliseDocumento( $_REQUEST );
        }
        if( $_REQUEST['requisicao'] == 'R' ){
            $retorno = $obraDocumento->recusarAnaliseDocumento( $_REQUEST );
        }
        if( $_REQUEST['requisicao'] == 'A' ){
            $retorno = $obraDocumento->aprovarAnaliseDocumento( $_REQUEST );
        }
        echo $retorno;
        die();
        break;
    case 'download':
        ob_clean();
        $arqid = $_GET['arqid'];
        if ($arqid) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec(null, null, "par3");
            $file->getDownloadArquivo($arqid);
        }
        die();
        break;
    case 'xls':
        ob_clean();
        echo $controllerObra->imprimeObrasXls($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Lista_de_Obras.xls");
        die;
        break;
    case 'verificaraprovacaoemlote':
        ob_clean();
        $arrObras = $_POST['obras'];
        echo $controllerObra->validarAprovacaoObras($arrObras);die;
        break;
    case 'carregarformtramitacao':
        ob_clean();
        require 'formComentarioTramitacao.inc';die;
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

$arrPerfil = pegaPerfils($_SESSION['usucpf']);
if ( in_array(PAR3_PERFIL_ANALISTA_PROFE, $arrPerfil) || in_array(PAR3_PERFIL_PROCURADOR_PROFE, $arrPerfil) ) {
    $_POST['esdid'][0] = 2047; #10 - Aguardando Análise da PROFE
    $_POST['filtrar'] = 'filtrar';
}

if(isset($_POST['filtrar'])){
    $_SESSION['toReturn'] = $_POST;
    $listagemSimec = new Simec_Listagem();
    $arListagem = $controllerObra->getSqlListagemObras( $_POST );
//    $countRows = sizeof( $arListagem );

//    if($countRows > 0){
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
        $arrayEsconder = array('obrdsc','unidade','iniciativapla','edit','situacao_estado_documento_id', 'obrid','situacao_estado_documento','obrcheck','obra2','obra2_esddsc','dotid');
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($arListagem);

        if($permiteTramitar) {
            $listagemSimec->addCallbackDeCampo('obrcheck', 'checkbox_aprovar_em_lote');
            unset($arrayEsconder[7]);
            array_unshift($arrayCabecalho,'');
        }

        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($arrayEsconder);
//        $listagemSimec->addAcao('edit',array('func' => 'js_btnEditarObraListaDeObras', 'extra-params' => array('unidade','iniciativapla','preid')));
        $listagemSimec->addAcao('file', array('func' => 'modal_analise_profe', 'cor' => 'success', 'extra-params' => array('preid')));
        $listagemSimec->setFormFiltros('formulario-filtro-obras');
        $listagemSimec->addCallbackDeCampo('desdobramentos', 'listaDesdobramentosListaObras');
        $listagemSimec->addCallbackDeCampo('linkobra2', 'linkObras2');
        $listagemSimec->addCallbackDeCampo('escnome', 'iconEscolaListaObras');
        $listagemSimec->addCallbackDeCampo('linkeditarobr', 'btnEditarObraListaDeObras');
        $listagemSimec->addCallbackDeCampo('docid', 'popOverSituacaoObra');
        $listagemSimec->addCallbackDeCampo('obrvalor','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('obrvalor_contrapartida','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('Empenho','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('Termo','formataTipoDocumentoParams');
        $listagemSimec->addCallbackDeCampo('valor_fnde','par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo(['vlrpago'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS,'preid');
        $listagemSimec->addCallbackDeCampo('Processo','formata_numero_processo');
        $listagemSimec->setAcaoComoCondicional('file', [['campo' => 'situacao_estado_documento_id', 'valor' => '2047', 'op' => 'igual']]);
        $listagemSimec->setId('listaObras');
        $_REQUEST['filtroObras'] = $_POST;
//    }
}

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
<div id="debug"></div>
<!-- Filtro -->
<div class="float-e-margins" id="filtro-obras">
    <div class="ibox-title" id="btn-filtro-obras">
        <div class="ibox-tools">
            <!--<a>Opções Avançadas <i id="i-filtro-obras" class="fa fa-chevron-down"></i></a>-->
        </div>
        <h3 class="">Filtro</h3>
    </div>
    <div class="ibox-content" id="div-filtro-obras" style="display: block;">
        <form method="post" name="formulario-filtro-obras" id="formulario-filtro-obras" class="form form-horizontal">
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
                    $arrEstado = Territorios_Model_Estado::pegarSQLSelect ($_POST);
                    $arrMunicipio = $controllerMunicipio->getFormListaMunicipioByUF();

                    echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'), array());
                    echo $simec->select('muncod', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255'), array());

                    ?>

                    <?php
                    $sqlAnosObras = $controllerObra->getSqlAnosExistentesObrasPar();
                    $arrAttrAnoObras = array (
                        'data-placeholder' => 'Selecione',
                    );
                    echo $simec->input('obrano','Ano',$_REQUEST['obrano'],$arrAttrAnoObras);

                    //situações par3 obras
                    $sqlSituacaoObra = $controllerEstadoDocumento->getSqlSituacaoEstadoDocumentoPar();
                    $arrAttrSituacao = array (
                        'placeHolder'  => 'Selecione',
                        'multiple' => 'multiple'
                    );

                    if ( in_array(PAR3_PERFIL_ANALISTA_PROFE, $arrPerfil) ) {
                        $arrAttrSituacao = array (
                            'placeHolder'  => 'Selecione',
                            'multiple' => 'multiple',
                            'disabled' => 'disabled'
                        );
                    }

                    echo $simec->select('esdid','Situação',$_POST['esdid'],$sqlSituacaoObra,$arrAttrSituacao);
                    //END FILTRO COMUM
                    ?>
                    <div class="" id="div-filtro-obras-op-avancada" style="display: none;">
                        <?php

                        //#altrar sql quando for decidido quais perfis são técnicos
                        $sqlTecnicos = $controllerSegurancaPerfilUsuario->getSqlPerfilTecnicoAnaliseEfetuada();
                        $arrAttrTecnicos = array(
                            'placeHolder' => 'Selecione',
                        );
                        //OPÇÕES AVANÇADAS
                        ?>
                        <div class="row">
                                <div class="col-md-12">
                                    <div class="col-md-3" style="padding-left: 24px;"><strong style="margin-top:14px;">Técnicos - análises efetuadas: </strong></div>
                                    <div class="col-md-6 " style="margin-left:-10px;">
                                <?php
                               // echo $simec->select ('tecnicoanalista','Técnicos - analises efetuadas','',$sqlTecnicos,$arrAttrTecnicos);
                                echo "<div id='inputSelectTecnicos'>".$simec->select('tecnicoanalista', '',$_REQUEST['tecnicoanalista'],$sqlTecnicos, $arrAttrTecnicos)."</div>";
                                /*Aguardado especificação */
                                ?>
                                </div>
                                    <div class="col-md-3 col-sm-pull-1">
                                        <div class="form-group">
                                                <div id="statusTecnico">
                                                    <div class="radio radio-default radio-inline" >
                                                        <input type='radio' name='statusTecnico' id='statusTecnicoAtivo' value='P'>
                                                        <label for="tecnicos-inativos" for="statusTecnicoAtivo">Técnicos Inativos</label>
                                                    </div>
                                                    <div class="radio radio-default radio-inline ">
                                                        <input type="radio" name="statusTecnico" id="statusTecnicoTodos" value="T" checked>
                                                        <label for="tecnicos-ativos"  for="statusTecnicoTodos">Todos</label>
                                                    </div>
                                                </div>
                                        </div>
                                    </div>
                                </div>
                        </div>

                         <?php
                        $arrNumTermo = array (
                            'placeHolder' => '',
                            'type'        => 'number',
                            'min'         => '1',
                            'max'         => '99999999',
                            'maxlength'   => '9',
                            'data-inputmask' => '\'alias\': \'numeric\', \'rightAlign\': false, \'allowMinus\': false, \'allowPlus\': false, \'digits\': 0',
                            'data-msg-min'=> "Quantidade mínima permitida: 1",
                            'data-msg-max'=> "Quantidade máxima permitida: 99999999"
                        );
                        echo $simec->input('dotnumero', 'N° Termo', $_REQUEST['dotnumero'], $arrNumTermo);

                        $arrNumProcesso = array (
                            'placeHolder' => '',
                        );
                        echo $simec->input('pronumeroprocesso', 'N° Processo', $_REQUEST['pronumeroprocesso'], $arrNumProcesso);

                        ?>

                        <div class="form-group">
                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Esfera:</label>
                            <div class="col-sm-12 col-md-9 col-lg-8 ">
                                <div id="esfera_group_id">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='iniesfera' id='esfera_estadual' value='E' <?=$_POST['iniesfera'] == 'E' ? 'checked' : ''?>>
                                        <label for="esfera_estadual">Estadual</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="iniesfera" id="esfera_municipal" value="M" <?=$_POST['iniesfera'] == 'M' ? 'checked' : ''?>>
                                        <label for="esfera_municipal">Municipal</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="iniesfera" id="esfera_todos" value="" <?=empty($_POST['iniesfera']) ? 'checked' : ''?>>
                                        <label for="esfera_todos">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>

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
                                    <div class="checkbox checkbox-default checkbox-inline">
                                        <input type='checkbox'  name='pagamento[]' id='pagamento_solicitado' value='enviado ao sigef' <?=empty($_POST['pagamento'] == 'enviado ao sigef') ? 'checked' : ''?>>
                                        <label for="pagamento_solicitado">Pagamento Solicitado</label>
                                    </div>
                                    <div class="checkbox checkbox-default checkbox-inline">
                                        <input type="checkbox" name="pagamento[]" id="pagamento_n_solicitado" value="não solicitado" <?=empty($_POST['pagamento'] == 'não solicitado' ) ? 'checked' : ''?>>
                                        <label for="pagamento_n_solicitado">Pagamento Não Solicitado</label>
                                    </div>
                                    <div class="checkbox checkbox-default checkbox-inline">
                                        <input type="checkbox" name="pagamento[]" id="pagamento_efetivado" value="efetivado" <?=empty ($_POST['pagamento'] == 'efetivado')? 'checked' : ''?>>
                                        <label for="pagamento_efetivado">Pagamento Efetivado</label>
                                    </div>
                                    <div class="checkbox checkbox-default checkbox-inline">
                                        <input type="checkbox" name="pagamento" id="pagamento_todos" value="" <?=empty($_POST['pagamento']) ? 'checked' : ''?>>
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
                             echo $simec->select('etaid','Etapa',$_REQUEST['etaid'],$sqlEnsinoEtapa,$arrEnsinoEtapa);
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
                            echo $simec->select("modid","Modalidade",$_REQUEST['etaid'],$sqlModalidade,$arraModalidade);
                            ?>
                        </div>
                        <?php
                        //pendente - funcionalidade do agrupador não desenvolvida,

                        $arrAgrupador = array (
                            "data-placeholder" => 'Selecione',
                            "multiple" => "multiple"
                        );
                        echo $simec->select ('agrid','Agrupador',$_REQUEST['etaid'],$controllerAgrupador->getListSelectFormAgrupador(),$arrAgrupador);
                        ?>

                        <div id="divTiposAssistencia">
                            <?php
                            $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
                            $mInta = $controllerIniciativaTiposAssistencia->recuperar();
                            $arrTiposAssistencia = simec_preparar_array($mInta->recuperarTodosFormatoInput('intadsc'));
                            $arrTiposAssistencia[0] = 'Todos';
                            echo $simec->select('intaid','Tipos de Assistência',$_REQUEST['intaid'],$arrTiposAssistencia,['placeHolder' => 'Selecione']);
                            ?>


                        </div>
                    </div> <!-- Fim filtro avançado-->
                    <div class="ibox-title" id="btn-filtro-obras-op-avancada">
                        <div class="ibox-tools">
                            <a>Opções Avançadas <i id="i-filtro-obras-op-avancada" class="fa fa-chevron-down"></i></a>
                        </div>
                    </div>
                    <div class="row center">
                        <div class="col-md-12">
                            <div class="col-md-4"></div>
                            <div class="col-md-6">
                                <button type="button" class="btn btn-info" id="btn-pesquisa"><i class="fa fa-search"></i>Pesquisar</button> |
                                <?php if($permiteTramitar):?>
                                <button type="button" class="btn btn-warning" id="btn-aprovar-em-lotes"><i class="fa fa-check"></i>Tramitar Em Lote</button> |
                                <?php endif;?>
                                <button type="button" class="btn btn-primary xls" id="btn-xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                            </div>
                            <div class="col-md-4"></div>
                        </div>
                    </div>
                </div>
                <div class="col-md-2"></div>
            </div>
        </form>
    </div>
</div>
<!-- End Filtro -->

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
                        $listagemSimec->render();
                    }
                        ?>
                </div>
            </div>
            <tr>
                <td>
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



<script>
    jQuery(function ($) {
        "use strict";
        var form = $("#formulario-filtro-obras");
        $("#btn-pesquisa").on('click', function(e) {
            e.preventDefault();
            if (form.valid()) {
                $(this).submit();
            }
        });

        form.validate({
            rules: {
                dotnumero:{
                    number: true,
                    max: 9999999999,
                    maxlength: 10,
                    min: 1
                }
            },submitHandler: function(form) {
                form.submit();
            }
        });
    });

    $(".xls").on("click", function (e) {
            e.preventDefault();
        var form = $("#formulario-filtro-obras");
        if (form.valid()) {
            window.location.assign(window.location.href + "&method=xls&" + jQuery('#formulario-filtro-obras').serialize() + '&' + $('.form-listagem').serialize());
            }
    });
    //Ao carregar a página, simular a mudança nos municípios
    $(document).ready(function(){
        $('body').on('click','.termo_detalhe', function(){
            id = $(this).find('.dotid').attr('value');
            visualizarTermoPar3(id);
        });
    	if(jQuery('[name="estuf"]').val() != '') {
            carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
        }

        jQuery('select[name=estuf]').change(function(){
            carregarMunicipio(this.value);
        });
    });

    function carregarMunicipio(estuf, muncod) {
    	if(estuf != '') {
    		var options = jQuery('#muncod');
    		options.empty();
    		options.append(new Option('', ''));
    		jQuery.post('', 'method=carregaMunicipios&estuf='+estuf, function(retorno) {
    			options.append(new Option('', ''));
    			$.each(JSON.parse(retorno), function() {
    				options.append(new Option(this.mundescricao, this.muncod));
    			});
    			options.focus();
    			options.val(muncod);
    			options.trigger('chosen:updated');
    		});
    	}
    }

    function salvarDocumentoAnalise(tipo){
    	if( jQuery('[name="adpparecer"]').val() == '' ){
            $("#modal-visualiza-analise").hide();
            swal({
                title: "Error",
                text: "O campo <b>Parecer PF-FNDE</b> é de preenchimento obrigatório!",
                type: "warning",
                html: true,
                confirmButtonColor: "#DD6B55",
                confirmButtonText: "Ok",
                closeOnConfirm: true
                }, function (isConfirm) {
                    if (isConfirm) {
                    	$("#modal-visualiza-analise").modal("show");
                    }
                }
            );
            return false;
        }
        if( jQuery('[name="arqid"]').val() == '' && jQuery('[name="adpid"]').val() == '' ){
            swal({
                title: "Error",
                text: "O campo <b>Nota PF-FNDE</b> é de preenchimento obrigatório!",
                type: "warning",
                html: true,
                confirmButtonColor: "#DD6B55",
                confirmButtonText: "Ok",
                closeOnConfirm: true
                }, function (isConfirm) {
                    if (isConfirm) {
                    	$("#modal-visualiza-analise").modal("show");
                    }
                }
            );
            return false;
        }

        // divCarregando();
        jQuery('#requisicao').val(tipo);
        jQuery('#method').val('salvarDocumentoAnalise');

        var action = window.location.href;
        //Como implementar submit de formulários enctype="multipart/form-data"
        var data = new FormData($('#form-propriedade').get(0));
        $.ajax({
            type: "POST",
            url: action,
            contentType: false,//necessário para fazer upload
            processData: false,//necessário para fazer upload
            data: data,
            success: function (resp) {
                console.log(resp);
                if(resp == 1){
                    swal({
                            title: "Sucesso",
                            html: true,
                            text: 'Operação realizada com sucesso.',
                            type: "success",
                            allowEscapeKey: false
                        },
                        function() {
                            $('#modal_tramitar').modal('hide');
                            $(document).find('#formulario-filtro-obras').submit();
                        }
                    );
                }else{
                    swal({
                            title: "Erro",
                            html: true,
                            text: 'Ocorreu um erro',
                            type: "error"},
                        function() {
                            $('.modal').modal('hide');
                        }
                    );
                }
            }
        });
	}

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

    function downloadDocumento( arqid ){
        window.location.href = 'par3.php?modulo=principal/listaObras&acao=A&method=download&arqid='+arqid;
    }

    function modal_analise_profe( inuid, obrid ){
		var caminho = 'par3.php?modulo=principal/listaObras&acao=A';
        var action  = '&method=modal_analise_profe&obrid='+obrid+'&inuid='+inuid;

       	jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery(".div_html").html(resp);
                //jQuery("#debug").html(resp);
            }
        });
        $("#modal-visualiza-analise").modal("show");
	}

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
     /*$('#estuf').change(function(){
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
        });*/

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
        window.open('par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid='+inu+'&inpid='+inp+'&obrid='+obrid+'&toReturn=formListaObra','_blank');
    }
    $(document).ready(function(){
        $('.i-checks').iCheck({//Função para utilizar o checkbox estilizado
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green',
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
        jQuery('.btn-aceite-termo-manifesto').hide();
        jQuery("#modal-visualiza-termo").modal("show");
    }

    function imprimirTermo(){
        var dotid = jQuery('[name="dotid"]').val();
        var domid = jQuery('[name="domid"]').val();
        window.location.href = window.location+'method=imprimir&dotid='+dotid;
    }

    // Desmarcar todos se uma das outras opções for desmarcada
    $('#pagamento_solicitado').click(function () {
        if ($('#pagamento_solicitado').prop("checked") === false) {
            $('#pagamento_todos').prop("checked", false);
        }
    });

    $('#pagamento_n_solicitado').click(function () {
        if ($('#pagamento_n_solicitado').prop("checked") === false) {
            $('#pagamento_todos').prop("checked", false);
        }
    });

    $('#pagamento_efetivado').click(function () {
        if ($('#pagamento_efetivado').prop("checked") === false) {
            $('#pagamento_todos').prop("checked", false);
        }
    });
    // FIM Desmarcar todos se uma das outras opções for desmarcada

    // Marcar e desmarcas demais checkbox quando a todos estiver selecionada.
    $("#pagamento_todos").change(function () {
        $("input:checkbox").prop('checked', $(this).prop("checked"));
    });

    $("#pagamento_todos").click(function(){
        $('input:checkbox').not(this).prop('checked', this.checked);
    });

    var checkTodos = $("#pagamento_todos");
    checkTodos.click(function () {
        if ( $(this).is(':checked') ){
            $('input:checkbox').prop("checked", true);
        }else{
            $('input:checkbox').prop("checked", false);
        }
    });
    // Fim marcar/desmarcar todos
</script>
