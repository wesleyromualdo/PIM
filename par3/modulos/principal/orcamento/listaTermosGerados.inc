<?php

$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$arrIniciativaAno = Par3_Model_IniciativaPlanejamento::getInicitivaAno();
$obDocumento = new Par3_Controller_DocumentoTermo();

/*$obPendenciaBloqueio = new Par3_Controller_Pendencia();
$temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('termo', 2223, '', 5160, true);
ver($temPendencia,d);*/

switch ($_REQUEST['requisicao']) {
    case 'carregaDadosObra':
        echo carregaDadosObraLista($_REQUEST['proid']);
        die;
        break;
    case 'pegaDadosDatas':
        
        $retorno = $db->pegaUm("select '{$_REQUEST['data']}'::date >= now()::date");
        if( $retorno == 'f' ){
            echo '<span style="color: red;">A data de vigência <b>menor que a data atual</b> caracteriza <b>rescisão do termo</b>. A operação não poderá ser desfeita!</span>';
        }
        die;
        break;
    case 'verifica_pendencia':
        /*$boEmenda = $db->pegaUm("SELECT count(proid) FROM par3.processo WHERE intaid = 2 and proid = {$_REQUEST['proid']}");
        
        if (((int)$boEmenda > (int)0)) {
            $temPendencia == false;
        } else {*/
            $obPendenciaBloqueio = new Par3_Controller_Pendencia();
            $temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('termo', $_REQUEST['inuid'], '', $_REQUEST['proid'], false);
        //}
        
        if ($temPendencia == false) {
            echo 'N';
        } else {
            echo 'S';
        }
        exit();
        break;
    case 'deletar':
        echo $obDocumento->excluirDocumento($_REQUEST['dotid']);
        exit();
        break;
    case 'pega_dotid':
        $dotid = $db->pegaUm("SELECT dotid FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} AND dotstatus in ('A', 'B')");
        if( empty($dotid) ){
            $dotid = $db->pegaUm("SELECT dotid, dotstatus FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} ORDER BY dotid DESC LIMIT 1");
        }
        echo $dotid;
        exit();
        break;
    case 'carregaMun':
        $municipio = new Territorios_Model_Municipio();
        print simec_json_encode($municipio->lista(array('muncod', 'mundescricao||\' - \'||estuf as mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
        die;
        break;
    case 'visualiza_termo':
        ob_clean();
        $mdoconteudo = $obDocumento->pegaTermoCompromissoArquivo($_REQUEST['dotid']);
        $mdoconteudo = str_ireplace('\"', '"', $mdoconteudo);
        echo simec_html_entity_decode($mdoconteudo);
        exit();
        break;
    case 'visualiza_sigarp':
        ob_clean();
        echo $obDocumento->visualizaDadosSigarp($_REQUEST['proid'], $_REQUEST['dotid']);
        exit();
        break;
    case 'verifica_reenvio':
        ob_clean();
        echo $obDocumento->podeReenviarSigarp($_REQUEST['dotid']);
        exit();
        break;
    case 'reenviarSigarp':
        ob_clean();
        include_once APPRAIZ . 'par3/classes/controller/WS_Sigarp_FNDE.class.inc';

        //WS_USUARIO_SIGEF, WS_SENHA_SIGEF
        $obSigarp = new Par3_Controller_WS_Sigarp_FNDE();
        $obSigarp->dotid = $_REQUEST['dotid'];
        $obSigarp->ws_usuario = WS_USUARIO_SIGEF;
        $obSigarp->ws_senha = WS_SENHA_SIGEF;

        if ($_REQUEST['reformular'] == 'S') {
            $obSigarp->reformular = 'S';
            $arRetorno = $obSigarp->enviarItensSigarpReformulacao();
        } else {
            $arRetorno = $obSigarp->enviarItensSigarp();
        }
        $tipo = 'success';
        if ($arRetorno['erro'] == 't') {
            $tipo = 'error';
        }

        $urlM = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A&proid=' . $_REQUEST['proid'];
        simec_redirecionar($urlM, $tipo, $arRetorno['retorno']);
        exit();
        break;
    case 'reformularSigarp':
        ob_clean();
        include_once APPRAIZ . 'par3/classes/controller/WS_Sigarp_FNDE.class.inc';

        //WS_USUARIO_SIGEF, WS_SENHA_SIGEF
        $obSigarp = new Par3_Controller_WS_Sigarp_FNDE();
        $obSigarp->dotid = $_REQUEST['dotid'];
        $obSigarp->ws_usuario = WS_USUARIO_SIGEF;
        $obSigarp->ws_senha = WS_SENHA_SIGEF;

        $obSigarp->reformular = 'S';
        $arRetorno = $obSigarp->enviarItensSigarpReformulacao();

        $tipo = 'success';
        if ($arRetorno['erro'] == 't') {
            $tipo = 'error';
        }

        $urlM = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A&proid=' . $_REQUEST['proid'];
        simec_redirecionar($urlM, $tipo, $arRetorno['retorno']);
        exit();
        break;
    case 'imprimir':
        ob_clean();
        $obDocumento->imprimirTermoPDF($_REQUEST['dotid']);
        exit();
        break;
    case 'imprimir-listatermo':
        $obDocumento->listar($_REQUEST, $_REQUEST['tipovisualizacao']);
        exit();
        break;
    case 'excel':
        $obDocumento->listar($_REQUEST, $_REQUEST['tipovisualizacao']);
        exit();
        break;
    case 'detalharProcesso':
        echo $obDocumento->listaTermoPorProcesso($_REQUEST);
        exit();
        break;
    case 'montar_tela_termo':
        ob_clean();
        $obDocumento->montarTelaTermoExOficio($_REQUEST['dotid_filho'], $_REQUEST['dotid_original'], $_REQUEST['inuid'], $_REQUEST['proid'], $_REQUEST['tipo']);
    exit();
    break;
    case 'salvar-termo':
        /* $_REQUEST['aditivo'] = 'S';
         $_REQUEST['exoficio'] = 'S';*/
        $_REQUEST['bloqueado'] = 'S';
        unset($_REQUEST['refid']);
        $mdoconteudo = $obDocumento->salvar_termo($_REQUEST);
    die;
    break;
    case 'gerar_termo_oficio':
        ob_clean();
        
        if( empty($_REQUEST['dotid']) ){
            $db->executar("DELETE FROM par3.documentotermoarquivo WHERE dotid IN (SELECT dotid FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} AND dotstatus = 'B' AND dotidpai IS NOT NULL);
                        DELETE FROM par3.termocomposicao WHERE dotid IN (SELECT dotid FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} AND dotstatus = 'B' AND dotidpai IS NOT NULL);
                        DELETE FROM par3.documentotermo WHERE dotid IN (SELECT dotid FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} AND dotstatus = 'B' AND dotidpai IS NOT NULL);");
        } else {
            $_REQUEST['dotid_original'] = $db->pegaUm("select dotidpai from par3.documentotermo where dotid = {$_REQUEST['dotid']}");
        }
        
        if( $_REQUEST['tipo'] == 'par' ) $_REQUEST['tipo_processo'] = 'P';
        else $_REQUEST['tipo_processo'] = 'O';
        
        $_REQUEST['requisicao'] = 'carregar-termo';
        $obDocumento = new Par3_Controller_DocumentoTermo();
        
        $_REQUEST['bloqueado'] = 'S';
        $_REQUEST['aditivo'] = 'S';
        $_REQUEST['exoficio'] = 'S';
        $_REQUEST['tirid'] = 3;
        $_REQUEST['dotid_ref'] = $_REQUEST['dotid_original'];
        
        $arDataTermo = $db->pegaLinha("select to_char(dotdatainiciovigencia, 'DD/MM/YYYY') as dotdatainiciovigencia, dotdatafimvigencia from par3.documentotermo where dotid = {$_REQUEST['dotid_original']};");
        $_REQUEST['dotdatainiciovigencia'] = $arDataTermo['dotdatainiciovigencia'];
        
        $dotid = $obDocumento->salvar_termo($_REQUEST);
        
        $_REQUEST['dotid'] = $dotid;
        $_REQUEST['aditivo'] = 'N';
        $_REQUEST['reformulacao'] = 'O';
        $_REQUEST['dotid_ref'] = '';
        $mdoconteudo = $obDocumento->alteraMacroDocumento($_REQUEST);
        $_REQUEST['mdoconteudo_t'] = $mdoconteudo;
        $obDocumento->salvar_termo($_REQUEST);
        $mdoconteudo = str_replace('"', "'", $mdoconteudo);
        echo simec_json_encode( array( 'texto' => simec_html_entity_decode($mdoconteudo), 'dotid' => $dotid) );
    exit();
    break;
}

require APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

$db->cria_aba($abacod_tela, $url, '');

?>
<style>
    .municipio_detalhe {
        cursor: pointer;
        color: blue;
    }
    .navbar-listagem {margin-top: 0px;float: right;width: 500px;}
    
    .popover-header {
        padding: .5rem .75rem;
        margin-bottom: 0;
        font-size: 1rem;
        color: inherit;
        background-color: #f7f7f7;
        border-bottom: 1px solid #ebebeb;
        border-top-left-radius: calc(.3rem - 1px);
        border-top-right-radius: calc(.3rem - 1px);
    }
    .swal-footer {
        text-align: center;
    }
    .fa-circle-o:before {
      content: "\f10c";
      position: relative;
      top: -3px;
    }
    .circle-text {
        position: relative;
        top: -3px;
        left: 0px;
        font-size: 10px;
    }
    .fa-stack {
        position: relative;
        display: inline-block;
        width: 35px;
        height: 30px;
        line-height: 2em;
        vertical-align: middle;
    }
</style>

<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>

<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Lista de Termos</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-pesquisa-termo" id="form-pesquisa-termo" class="form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value="filtrar"/>
                <input type="hidden" name="tipovisualizacao" id="tipovisualizacao" value="html" />
                <input type="hidden" name="tipodetalhado" id="tipodetalhado" value="nao" />
                
                <div class="row">
                    <div class="col-lg-6">
                        <?php
                        $filtro = simec_htmlentities($_POST['numeroprocesso']);
                        $numeroprocesso = $filtro;

                        $config = array();
                        $attribs = array('maxlength' => '255',
                            'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                            'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);", 'autofocus');

                        echo $simec->input('numeroprocesso', 'Número de Processo', $numeroprocesso, $attribs, array('input-size' => '6'));

                        $_REQUEST['tipo_processo'] = (empty($_REQUEST['tipo_processo']) ? 'T' : $_REQUEST['tipo_processo']);

                        $arrOptions = array('O' => 'Obras', 'P' => 'PAR', 'T' => 'Todos');
                        ?>
                        <?php echo $simec->input('dotnumero', 'Número do Termo', $_REQUEST['dotnumero'], array('type' => 'text'), array('input-size' => '6')); ?>
                        <?php echo $simec->input('proid', 'Código do Processo', $_POST['proid'], array('type' => 'number'), array('input-size' => '6')); ?>
                        <?php echo $simec->input('codigo', 'Código Obra/Iniciativa', $_REQUEST['codigo'], array('type' => 'number'), array('input-size' => '6')); ?>

                        <?php
                        $mInp = new Par3_Controller_IniciativaPlanejamento();
                        $arrIniciativa = $mInp->getIniciativaPesquisaIndicadorSelect();
                        echo $simec->select('iniid', 'Iniciativa', $_REQUEST['iniid'], $arrIniciativa, array('maxlength' => '255'), array('input-size' => '6'));
                        ?>


                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termo Validado:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="termo_validado">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termo_validado' id='termo_validado_s' value="S" <?= $_REQUEST['termo_validado'] == 'S'?'checked':'';?>>
                                        <label for="termo_validado_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_validado" id="termo_validado_n" value="N"  <?= $_REQUEST['termo_validado'] == 'N'?'checked':'';?>>
                                        <label for="termo_validado_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_validado" id="termo_validado_t" value="T"  <?= $_REQUEST['termo_validado'] != 'S' && $_REQUEST['termo_validado'] != 'N'?'checked':'';?>>
                                        <label for="termo_validado_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termos rejeitados:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="termonota">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termorejeitado' id='termorejeitado_s' value="S" <?= $_REQUEST['termorejeitado'] == 'S'?'checked':'';?>>
                                        <label for="termorejeitado_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termorejeitado" id="termorejeitado_n" value="N"  <?= $_REQUEST['termorejeitado'] == 'N'?'checked':'';?>>
                                        <label for="termorejeitado_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termorejeitado" id="termorejeitado_t" value="T"  <?= $_REQUEST['termorejeitado'] != 'S' && $_REQUEST['termorejeitado'] != 'N'?'checked':'';?>>
                                        <label for="termorejeitado_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Bloqueio do PAR:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="bloqueio_par">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='bloqueio_par' id='bloqueio_par_s' value="S" <?= $_REQUEST['bloqueio_par'] == 'S'?'checked':'';?>>
                                        <label for="bloqueio_par_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="bloqueio_par" id="bloqueio_par_n" value="N"  <?= $_REQUEST['bloqueio_par'] == 'N'?'checked':'';?>>
                                        <label for="bloqueio_par_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="bloqueio_par" id="bloqueio_par_t" value="T"  <?= $_REQUEST['bloqueio_par'] != 'S' && $_REQUEST['bloqueio_par'] != 'N'?'checked':'';?>>
                                        <label for="bloqueio_par_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termos gerados:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="termo_gerado">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termo_gerado' id='termo_gerado_s' value="S" <?= $_REQUEST['termo_gerado'] == 'S'?'checked':'';?>>
                                        <label for="termo_gerado_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_gerado" id="termo_gerado_n" value="N"  <?= $_REQUEST['termo_gerado'] == 'N'?'checked':'';?>>
                                        <label for="termo_gerado_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_gerado" id="termo_gerado_t" value="T"  <?= $_REQUEST['termo_gerado'] != 'S' && $_REQUEST['termo_gerado'] != 'N'?'checked':'';?>>
                                        <label for="termo_gerado_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Prorrogação de Ofício:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="mdoex_oficio">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='mdoex_oficio' id='mdoex_oficio_s' value="S" <?= $_REQUEST['mdoex_oficio'] == 'S'?'checked':'';?>>
                                        <label for="mdoex_oficio_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="mdoex_oficio" id="mdoex_oficio_n" value="N"  <?= $_REQUEST['mdoex_oficio'] == 'N'?'checked':'';?>>
                                        <label for="mdoex_oficio_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="mdoex_oficio" id="mdoex_oficio_t" value="T"  <?= $_REQUEST['mdoex_oficio'] != 'S' && $_REQUEST['mdoex_oficio'] != 'N'?'checked':'';?>>
                                        <label for="mdoex_oficio_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6">
                        <?php echo $simec->radio('itrid', 'Esfera', ($_REQUEST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                        <div class="row">
                            <div class="col-lg-5" style="margin-left:55.5px;">
                                <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'), array('input-size' => '6', 'label-size' => '5')); ?>
                            </div>
                            <div class="col-lg-5" id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;">
                                <?php
                                $_REQUEST['muncod'] = tratarArrayParaMultiSelect($_REQUEST['muncod']);
                                if ($_REQUEST['estuf']) {
                                    $municipio = new Territorios_Model_Municipio();
                                    $arrMunicipio = simec_preparar_array($municipio->lista(array('muncod as codigo', 'estuf ||\'-\'|| mundescricao as descricao'), array("estuf = '{$_REQUEST['estuf']}'")));
                                }
                                echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255','multiple' => 'multiple'), array()); ?>
                            </div>
                        </div>

                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Período de Vigência:</label>
                            <div class="col-lg-5 col-md-5 col-sm-5">
                                <div class="" id="data_1">
                                    <div class="input-group m-b" id="datepicker">
                                        <span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;height:34px;"><i class="fa fa-calendar"></i></span>
                                        <input type="text" class="form-control datemask daterange" autocomplete="off" name="vigenciainicio" id="vigenciainicio" value="<?= $_REQUEST['vigenciainicio']; ?>">
                                        <span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;height:34px;">a</span>
                                        <input type="text" class="form-control datemask daterange" autocomplete="off" name="vigenciafim" id="vigenciafim" value="<?= $_REQUEST['vigenciafim'];?>">
                                    </div>
                                </div>
                            </div>
                        </div>

                        <?php echo $simec->radio('tipo_processo', 'Tipo de Processo', $_REQUEST['tipo_processo'], $arrOptions, array(), array()); ?>
                        <div id="div-intoid" style="display:<?= $_REQUEST['tipo_processo'] == 'O'?'none;':'';?>">
                            <?php
                            $arrTipoObjeto = Par3_Model_IniciativaPlanejamento::getTipoObjetoporIniciativa(['it.intoid <> 1']);
                            echo $simec->select('intoid', 'Tipo do Objeto', $_REQUEST['intoid'], $arrTipoObjeto, array('maxlength' => '255'), array('input-size' => '4'));
                            ?>
                        </div>
                        <div id="divTiposAssistencia">
                            <?php
                            $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
                            $mInta = $controllerIniciativaTiposAssistencia->recuperar();
                            $arrTiposAssistencia = simec_preparar_array($mInta->recuperarTodosFormatoInput('intadsc'));
                            $arrTiposAssistencia[0] = 'Todos';
                            echo $simec->select('intaid', 'Tipos de Assistência', $_REQUEST['intaid'], $arrTiposAssistencia, ['placeHolder' => 'Selecione'], array('input-size' => '4','label-size' => '3'));
                            ?>
                        </div>
                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termos com contrato:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="pagsituacaopagamento_group_id">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termocontrato' id='termocontrato_s' value="S" <?= $_REQUEST['termocontrato'] == 'S'?'checked':'';?>>
                                        <label for="termocontrato_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termocontrato" id="termocontrato_n" value="N"  <?= $_REQUEST['termocontrato'] == 'N'?'checked':'';?>>
                                        <label for="termocontrato_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termocontrato" id="termocontrato_t" value="T"  <?= $_REQUEST['termocontrato'] != 'S' && $_REQUEST['termocontrato'] != 'N'?'checked':'';?>>
                                        <label for="termocontrato_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termos com nota fiscal:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="termonota">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termonota' id='termonota_s' value="S" <?= $_REQUEST['termonota'] == 'S'?'checked':'';?>>
                                        <label for="termonota_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termonota" id="termonota_n" value="N"  <?= $_REQUEST['termonota'] == 'N'?'checked':'';?>>
                                        <label for="termonota_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termonota" id="termonota_t" value="T"  <?= $_REQUEST['termonota'] != 'S' && $_REQUEST['termonota'] != 'N'?'checked':'';?>>
                                        <label for="termonota_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-info btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button> |
                    <button type="button" id="btn-excel" data-dismiss="modal" class="btn btn-primary btn-excel" data-loading-text="Gerando, aguarde ..."><i class="fa fa-file-excel-o"></i> Gerar Excel</button> |
                    <button type="button" class="btn btn-warning" name="imprimir"><i class="fa fa-print" aria-hidden="true"></i> Imprimir</button> |
                    <button type="reset" class="btn btn-success btn-limpar" id="limpar"><i class="fa fa-eraser"></i>Limpar</button>
                </div>
            </div>
            <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                    <div class="table-responsive">
                        <div>
                            <div id="testeSQL">
                                <?php
                                if ($_REQUEST['requisicao'] == 'filtrar' || $_REQUEST['proid']) {
                                    echo $obDocumento->listar($_REQUEST);
                                }
                                ?>
                            </div>
                        </div>
                    </div>
                    <br>
                    <br>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
    $('[data-toggle="popover"]').popover();
    $('[data-toggle="popover"]').mouseleave(function(e) {
        e.stopPropagation();
        $(document).find('.popover').remove();
    });
    jQuery(document).keypress(function(e) {
        if(e.which == 13){
             jQuery('.btn-pesquisar').click();
        }
    });

    jQuery('.class_dropdown').click(function(e){
   	 var proid = jQuery(this).attr("proid");

   	 jQuery('.dados_obras').html('');
   	    var caminho = window.location.href;
   		var action  = '&requisicao=carregaDadosObra&&proid='+proid;
   		
   	   jQuery.ajax({
   	        type: "POST",
   	        url: caminho,
   	        data: action,
   	        async: true,
   	        success: function (resp) {
   	            jQuery(".dados_obras").html(resp);
   	        }
   	    });
   	}
   );

    jQuery(function () {
        $('#data_1 .daterange').datepicker({
            format: "dd/mm/yyyy",
            language: "pt-BR",
            constrainInput: false
        });
        // $(window).on('beforeunload',function(){
        //     $('.loading-dialog-par3').show();
        // });
        $(window).on('click','#btn-pesquisar #limpar',function(){
            $('.loading-dialog-par3').show();
        });
        $(window).load(function(){
            $('.loading-dialog-par3').hide();
        });
        //renderizeMunicipio();

        if (jQuery('[name="estuf"]').val() != '') {
            console.log(<?= json_encode($_REQUEST['muncod']);?>);
            carregarMun(jQuery('[name="estuf"]').val(), <?= json_encode($_REQUEST['muncod']);?>);
        }

        jQuery("input:radio[name=itrid], select[name=estuf]").change(function () {
            //renderizeMunicipio();
        });

        jQuery('select[name=estuf]').change(function () {
            carregarMun(this.value);
        });

        function renderizeMunicipio() {
            var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
            if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=estuf]').val()) {
                filtroMunicipio.slideUp();
            } else {
                filtroMunicipio.slideDown();
            }
        }
        jQuery("[data-id-lista='listagemTermos']").remove();
        /*jQuery('.glyphicon-plus').on('click', function(){
        	jQuery("[data-id-lista='listagemTermos']").remove();
        });*/

    });

    function verificaDataInformada( data ){

    	var caminho = window.location.href;
   		var action  = '&requisicao=pegaDadosDatas&&data='+data;
   		
   	   jQuery.ajax({
   	        type: "POST",
   	        url: caminho,
   	        data: action,
   	        async: true,
   	        success: function (resp) {
   	            jQuery("#div_msg").html(resp);
   	        }
   	    });
    }
	

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').val('');
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });

    /*$('[name=tipo_processo]').change(function(){
        if($(this).val() == 'O'){
            $('#div-intoid').hide();
            $('#intoid').val('');
            $('#intoid').trigger("chosen:updated");

        }else{
            $('#div-intoid').show();
        }
    });*/

    jQuery('.municipio_detalhe').click(function () {
        var inuid = jQuery(this).attr('id');

        var url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid=' + inuid;
        window.open(url, '_blank');
    });

    function carregarMun(estuf, muncod) {
        if (estuf != '') {
            var options = jQuery('#muncod');
            options.empty();
            options.append(new Option('', ''));
            jQuery.post('', 'requisicao=carregaMun&estuf=' + estuf, function (retorno) {
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function () {
                    options.append(new Option(this.mundescricao, this.muncod));
                });
                options.focus();
                options.val(muncod);
                options.trigger('chosen:updated');
            });
        }
    }

    function gerarTermo(proid, tipo, inuid) {

    	divCarregando();
        var caminho = window.location.href;
        var action = '&requisicao=verifica_pendencia&proid='+proid+'&inuid='+inuid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //jQuery('#debug').html(resp);
                if( resp == 'N' ){
                    window.location.href = "par3.php?modulo=principal/orcamento/gerarTermo&acao=A&proid=" + proid;
                } else {
                    swal("Pendencias do PAR", "Não é possível Gerar Termo. Para Gerar o Termo, será necessário resolver as Pendências do PAR", "error");
                    divCarregado();
                }
            }
        });
    }

    function deletarTermo(dotid, validado, tempagamento) {

        if( tempagamento == 'S' ){
            var msg = "Para este termo, já existe pagamento vinculado. Não sendo possível realizar a exclusão!";

            swal("Excluir Termo", msg, "error");
            
        } else {
            if (validado == 'S') {
                var msg = "Este termo consta na situação <b>Documento Validado</b>, mesmo assim deseja exclui-lo?";
            } else {
                var msg = "Realmente deseja excluir o termo?";
            }
            swal({
                    title: "Confirmar",
                    text: msg,
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                },function (isConfirm) {
                    if (isConfirm) {
                        window.location.href = "par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A&requisicao=deletar&dotid=" + dotid;
                    }
                    return false;
                }
            );
        }
    }

    function visualizarSigarp(dotid, proid) {
        var caminho = window.location.href;
        var action = '&requisicao=visualiza_sigarp&dotid=' + dotid + '&proid=' + proid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery(".dados-sigarp").html(resp);

            }
        });
        $("#modal-visualiza-sigarp").modal("show");
    }

    function carregarTermo(dotid, proid) {
        window.location.href = "par3.php?modulo=principal/orcamento/gerarTermo&acao=A&proid=" + proid + '&dotid=' + dotid;
    }

    function visualizaTermoTela(proid, tipo, inuid) {
    	var caminho = window.location.href;
        var action = '&requisicao=pega_dotid&proid=' + proid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
            	visualizaTermo(resp);
            }
        });
        $("#modal-visualiza-modelo").modal("show");
    }
    
    function visualizaTermo(dotid) {
        var caminho = window.location.href;
        var action = '&requisicao=visualiza_termo&dotid=' + dotid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#div_conteudo_termo").html(resp);
                jQuery('[name="dotid"]').val(dotid);

            }
        });
        $("#modal-visualiza-modelo").modal("show");
    }

    //visualizaTermo

    jQuery(".btn-limpar").click(function () {
        window.location.href = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A';
    });

    jQuery(".btn-pesquisar").click(function () {
        $btn = jQuery(this).button('loading');
        jQuery('[name="requisicao"]').val('filtrar');
        jQuery('[name="form-pesquisa-termo"]').submit();
    });

    function trataEnvioWorkflowOficio(dotid_filho, dotid_original, proid, inuid, tipo) {
    	expandirLinha('detalharProcesso', [proid, tipo], proid);
	
    	abrirModalTermoOficio(dotid_filho, dotid_original, proid, inuid, tipo);
    }
    
    function abrirModalTermoOficio(dotid_filho, dotid_original, proid, inuid, tipo) {

    	tinyMCE.execCommand('mceRemoveControl', true, 'mdoconteudo');
    	$("#modal-form-de-oficio").modal("hide");
    	jQuery("#termo-modal").html('');
    	var caminho = window.location.href;
        var action = '&requisicao=montar_tela_termo&dotid_original='+dotid_original+'&tipo='+tipo+'&inuid='+inuid+'&proid='+proid+'&dotid_filho='+dotid_filho;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#termo-modal").html(resp);
                //tinyMCE.get('mdoconteudo').setContent( jQuery('[name="mdoconteudo_t"]').val() );
            }
        });
        $("#modal-form-de-oficio").modal("show");

        jQuery('#mdoconteudo_tbl').css('width', '100%'); 
        jQuery('#mdoconteudo_tbl').css('height', '546px'); 

        jQuery('#mdoconteudo_ifr').css('width', '100%'); 
        jQuery('#mdoconteudo_ifr').css('height', '477px');
        
        var habilita = jQuery('[name="form-gerar-termo"]').find('[name="habilita"]').val();
        
        if( habilita == 'N' ){
        	jQuery('#btn-carregar-termo').attr('disabled', true);
        	jQuery('#btn-salvar-termo').attr('disabled', true);
        } else {
        	jQuery('#btn-carregar-termo').attr('disabled', false);
        	jQuery('#btn-salvar-termo').attr('disabled', false);
		}
    }

    function reformularSigarp() {
        var dotid = jQuery('[name="dotid_sigarp"]').val();
        var proid = jQuery('[name="proid_sigarp"]').val();
        divCarregando();
        window.location.href = window.location + '&requisicao=reformularSigarp&dotid=' + dotid + '&proid=' + proid + '&reformular=S';
    }

    function reenviarSigarp() {
        var dotid = jQuery('[name="dotid_sigarp"]').val();
        var proid = jQuery('[name="proid_sigarp"]').val();
        divCarregando();
        var caminho = window.location.href;
        var action = '&requisicao=verifica_reenvio&dotid=' + dotid + '&proid=' + proid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //$('#debug').html(resp);

                if (resp == 'G') {
                    swal("Ação não Permitida!", "Alguns itens não possuem id SIGARP vinculado!", "error");
                    divCarregado();
                } else if (resp == 'A') {
                    window.location.href = window.location + '&requisicao=reenviarSigarp&dotid=' + dotid + '&proid=' + proid + '&reformular=S';
                } else {
                    if (resp == 'S') {
                        window.location.href = window.location + '&requisicao=reenviarSigarp&dotid=' + dotid + '&proid=' + proid;
                    } else {
                        swal("Ação não Permitida!", "Os itens já foram enviados anteriormente!", "error");
                        divCarregado();
                    }
                }
            }
        });
    }

    jQuery("[name=imprimir]").on("click", function () {
    	jQuery('#tipovisualizacao').val('pdf');
    	jQuery('#requisicao').val('imprimir-listatermo');
        var action = jQuery('#form-pesquisa-termo').serialize();
        jQuery.ajax({
            type		: "POST",
            url			: window.location.href,
            data		: action,
            dataType	: 'html',
            async		: true,
            success: function (resp)
            {
            	jQuery("#impressao-content").html(resp);
                var container = jQuery("#impressao-content");
                var table = container.find('table');
                var th = container.find('th');
                var td = container.find('td');
                table.css('width', '100%')
                table.css('border-collapse', 'collapse');
                container.find(".popover,button,.modal").remove();
                container.find("table,td,th").css('border', '1px solid black');
                container.find('tr:even').css('background-color', 'silver');
                th.css('height', '50px');
                td.css('text-align', 'center');
                var w = window.open();
                w.document.write(jQuery('#div-impressao').html());
                w.document.title = 'Lista de Termos';
                w.print();
                w.close();
                jQuery("#impressao-content").html("");
                jQuery('#tipovisualizacao').val('html');
                return true;
            }
        });
    });

    /*function gerarExcel()
    {
    	jQuery('#requisicao').val('excel');	
    	jQuery('#tipovisualizacao').val('excel');
    	jQuery('#form-pesquisa-termo').submit();
    }*/
    jQuery(".btn-excel").click(function () {
        /**
         * script carregado dinamicamente para evitar conflito com a versão padrão
         * do sweet alert usada no restante da página
         */
    	$.getScript('../zimec/public/temas/simec/js/plugins/sweetalert/swal.js',function() {
            swal("Selecione o tipo de relatório:", {
                buttons: {
                    cancel: "Cancelar",
                    catch: {
                        text: "Detalhado",
                        value: "detalhado",
                    },
                    defeat: {
                        text: "Sintético",
                        value: "sintetico",
                    },
                },
            })
                .then((value) => {
                    switch(value){
                        case 'detalhado':
                            $('#tipodetalhado').val('sim');
                            /*if( jQuery('[name="tipo_processo"]:checked').val() == 'T' ){
                                swal("Ação não Permitida!", "Para gerar o relatório detalhando é necessário selecionar o tipo de processo!", "error");
                                return false;
                            }*/
                            break;
                        case 'sintetico':
                            $('#tipodetalhado').val('nao');
                            break;
                        default:
                            return;
                    }
                    //$btn = jQuery(this).button('loading');
                    jQuery('#requisicao').val('excel');
                    jQuery('#tipovisualizacao').val('excel');
                    jQuery('#form-pesquisa-termo').submit();
                });
        }).done(function () {
            /**
             * recarrega a versão antiga do swal após finalizar a execução
             */
            $.getScript('../zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js',function() {});
        });

    });

    function carregarTermoAditivo(){

        var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): <br> <br>';
        var validacao = true;
        
        if( jQuery('#mdoid').val() == '' ){
            mensagem += 'Modelo de documento <br>';
            validacao = false;
        }
        if( jQuery('[name="dotjustificativa_de_oficio"]').val() == '' ){
        	mensagem += 'Justificativa <br>';
            validacao = false;
        }
        
    	if( jQuery('[name="dotdata_de_oficio"]').val() == '' ){
    		mensagem += 'Data <br>';
            validacao = false;
        }
        if( !validacao ){
            alert(mensagem);
            return false;
        }else{
        	jQuery('.loading-dialog-par3').show();
        	
            //$btn = jQuery('#btn-salvar-termo').button('loading');
            jQuery('[name="requisicao"]').val('carregar-termo-oficio');

            var mdoid = jQuery('[name="form-gerar-termo"]').find('[name="mdoid"]').val();
            var dotid_original = jQuery('[name="form-gerar-termo"]').find('[name="dotid_original"]').val();
            var proid = jQuery('[name="form-gerar-termo"]').find('[name="proid"]').val();
            var inuid = jQuery('[name="form-gerar-termo"]').find('[name="inuid"]').val();
            var dotid = jQuery('[name="form-gerar-termo"]').find('[name="dotid"]').val();
            var tipo = jQuery('[name="form-gerar-termo"]').find('[name="tipo"]').val();
            
            var dotjustificativa = jQuery('[name="form-gerar-termo"]').find('[name="dotjustificativa_de_oficio"]').val();
            var dotdata_de_oficio = jQuery('[name="form-gerar-termo"]').find('[name="dotdata_de_oficio"]').val();
            
            //jQuery('[name="form-gerar-termo"]').submit();
            var caminho = window.location.href;
            var action = '&requisicao=gerar_termo_oficio&dotid_original='+dotid_original+'&inuid='+inuid+'&proid='+proid+'&mdoid='+mdoid+'&dotid='+dotid+'&dotjustificativa_de_oficio='+dotjustificativa+'&dotdata_de_oficio='+dotdata_de_oficio+'&tipo='+tipo;
            jQuery.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: true,
                success: function (resp) {
                    //jQuery('#debug').html(resp);
                    
                	var arRetorno = jQuery.parseJSON(resp);
                    //console.log(arRetorno['texto']);
                    
                	jQuery('[name="mdoconteudo_t"]').val( arRetorno['texto'] );
                	jQuery('[name="form-gerar-termo"]').find('[name="dotid"]').val( arRetorno['dotid'] );
                	tinyMCE.get('mdoconteudo').setContent( jQuery('[name="mdoconteudo_t"]').val() );
                	jQuery('.loading-dialog-par3').hide();
                	jQuery("#modal-form-de-oficio").modal("hide");
                	expandirLinha('detalharProcesso', [proid, tipo], proid);
            		abrirModalTermoOficio(arRetorno['dotid'], dotid_original, proid, inuid, tipo);
                }
            });
        }
    }

    function salvarTermoOficio(){
        var texto = tinyMCE.get('mdoconteudo').getContent();

        jQuery('#mdoconteudo_t').val(texto);

        if( texto != '' ){
            $btn = jQuery(this).button('loading');
            jQuery('[name="requisicao"]').val('salvar-termo');
            jQuery('[name="form-gerar-termo"]').submit();
        } else {
            alert('É necessário carregar o documento antes de salvar.');
            jQuery(this).button('reset')
            return false;
        }
    }

    function imprimirTermo() {
        var dotid = jQuery('[name="dotid"]').val();
        window.location.href = window.location + '&requisicao=imprimir&dotid=' + dotid;
    }
    $('[data-toggle="popover"]').popover();
</script>

<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Lista de Termos<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <div id="impressao-content"></div>
</div>

<div class="ibox float-e-margins animated modal conteudo" id="modal-visualiza-modelo" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
        <div class="modal-content">
            <div class="ibox-content">
                <div class="row">
                    <div class="ibox-content">
                        <?php echo $obDocumento->montaHtmlTermo(); ?>
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

<div class="ibox float-e-margins animated modal conteudo" id="modal-visualiza-sigarp" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
        <div class="modal-content">
            <div class="ibox-content">
                <div class="row">
                    <div class="ibox-content dados-sigarp">

                    </div>
                </div>
            </div>
            <div class="ibox-footer notprint" style="text-align: center;">
                <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                <button type="button" id="btn-reenviar" data-dismiss="modal" class="btn btn-warning" onclick="reenviarSigarp();"><i class="fa fa-times-circle-o"></i> Reenviar</button>
<?php           if (in_array_perfil(array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR))) {?>
                    <button type="button" id="btn-reformular" data-dismiss="modal" class="btn btn-danger" onclick="reformularSigarp();"><i class="fa fa-times-circle-o"></i> Reformular</button>
<?php           }?>
            </div>
        </div>
    </div>
</div>

<div class="ibox float-e-margins animated modal conteudo" id="modal-form-de-oficio" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;" id="printable">
        <div class="modal-content">
            <div class="ibox-content" style="overflow: scroll; height: 800px;">
                <div class="row">
                    	<div id="termo-modal"></div>
                </div>
            </div>
            <div class="ibox-footer notprint" style="text-align: center;">
                <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                <button type="button" id="btn-salvar-termo" class="btn btn-primary" onclick="salvarTermoOficio();" data-loading-text="Salvando aguarde ..."><i class="fa fa-save"></i> Salvar Termo</button>
            </div>
        </div>
    </div>
</div>