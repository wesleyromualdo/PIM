<?php
set_time_limit(0);
include_once '_funcoes_orcamento.php';

$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);

switch ($_REQUEST['requisicao']) {
    case 'carregaMunicipios':
        $municipio = new Territorios_Model_Municipio();
        print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
        die;
        break;
    case 'carregaEmpenhoCancelado':
        $empid = $_REQUEST['dados'][0];
        echo carregaEmpenhoCanceladoPorEmpid($empid);
        die;
        break;
    case 'carregaDadosObra':
        echo carregaDadosObraLista($_REQUEST['proid']);
        die;
        break;
}

if ($_REQUEST['requisicao']) {
    header('content-type: text/html; charset=utf-8');
	
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
include APPRAIZ."includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
echo'<br>';
?>
<div id="debug"></div>
<script src="/includes/funcoes.js"></script>

<div class="ibox float-e-margins animated modal" id="dvvisualizarxmlsigef" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title"><h5><i class="fa fa-angle-double-right"></i> Visualizar XML SIGEF</h5></div>
            <div class="row">
                <div class="ibox-content" id="dvdadosvisualizarxmlsigef" style="height: 500px; overflow: auto;"></div>
            </div>
            <div class="ibox-footer">
                <div class="form-actions col-md-offset-2">
                    <div class="row">
                    <button type="button" class="btn btn-white" onclick="$('#dvvisualizarxmlsigef').modal('toggle');$('#modal-form-large').modal('show');$('#dvconfirmarempenho').modal('show');">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>


<div id="modal-form" class="modal fade" aria-hidden="true">
    <div id="modal-form-large" class="modal fade" aria-hidden="true">
        <div class="modal-content" id="dv-form-large"></div>
    </div>
</div>
<style>
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
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Lista de Empenhos</h2>
            </div>
        </div>
    </div>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <div class="ibox-content">
        <input type="hidden" name="tipodetalhado" id="tipodetalhado" value="nao" />
        <input type="hidden" name="tipovisualizacao" id="tipovisualizacao" value="html" />
        <div class="row">
            <div class="col-lg-6">
                <?php
                //$arrOptions = array('OBRA' => 'Obras', 'PAR' => 'PAR');
                //echo $simec->radio('tipo', 'Tipo', ($_POST['tipo']?:'PAR'), $arrOptions, array(), array('input-size' => '4','label-size' => '3'));
                ?>
                <?php
                //número do processo
                $filtro = simec_htmlentities($_POST['numeroprocesso']);
                $numeroprocesso = $filtro;
                echo $simec->input('numeroprocesso', 'Número do Processo', $numeroprocesso, array('maxlength' => '21', 'class' => 'inteiro', 'autofocus'), array('input-size' => '6'));

                //nota de empenho
                echo $simec->input('notaempenho', 'Nota do Empenho', $_POST['notaempenho'], array(), array('input-size' => '6'));
                ?>
                <?php echo $simec->input('termo', 'Número do Termo', $_POST['termo'], array('type' => 'number'), array('input-size' => '6')); ?>
                <?php echo $simec->input('proid', 'Código do Processo', $_POST['proid'], array('type' => 'number'), array('input-size' => '6')); ?>
                <?php echo $simec->input('obr_inp_id', 'Código Obra/Iniciativa', $_POST['obr_inp_id'], array('type' => 'number'), array('input-size' => '6')); ?>
                <div class="form-group" >
                    <label class=" col-sm-3 control-label">Termo Validado:</label>
                    <div class="col-sm-12 col-md-9 col-lg-4">
                        <div id="termo_validado">
                            <div class="radio radio-default radio-inline ">
                                <input type='radio' name='termo_validado' id='termo_validado_s' value="S" <?= $_POST['termo_validado'] == 'S'?'checked':'';?>>
                                <label for="termo_validado_s">Sim</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="termo_validado" id="termo_validado_n" value="N"  <?= $_POST['termo_validado'] == 'N'?'checked':'';?>>
                                <label for="termo_validado_n">Não</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="termo_validado" id="termo_validado_t" value="T"  <?= $_POST['termo_validado'] != 'S' && $_POST['termo_validado'] != 'N'?'checked':'';?>>
                                <label for="termo_validado_t">Todos</label>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="form-group" >
                    <label class=" col-sm-3 control-label">Termos com contrato:</label>
                    <div class="col-sm-12 col-md-9 col-lg-4">
                        <div id="termocontrato_group_id">
                            <div class="radio radio-default radio-inline ">
                                <input type='radio' name='termocontrato' id='termocontrato_s' value="S" <?= $_POST['termocontrato'] == 'S'?'checked':'';?>>
                                <label for="termocontrato_s">Sim</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="termocontrato" id="termocontrato_n" value="N"  <?= $_POST['termocontrato'] == 'N'?'checked':'';?>>
                                <label for="termocontrato_n">Não</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="termocontrato" id="termocontrato_t" value="T"  <?= $_POST['termocontrato'] != 'S' && $_POST['termocontrato'] != 'N'?'checked':'';?>>
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
                                <input type='radio' name='termonota' id='termonota_s' value="S" <?= $_POST['termonota'] == 'S'?'checked':'';?>>
                                <label for="termonota_s">Sim</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="termonota" id="termonota_n" value="N"  <?= $_POST['termonota'] == 'N'?'checked':'';?>>
                                <label for="termonota_n">Não</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="termonota" id="termonota_t" value="T"  <?= $_POST['termonota'] != 'S' && $_POST['termonota'] != 'N'?'checked':'';?>>
                                <label for="termonota_t">Todos</label>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-lg-6">
                <?php echo $simec->radio('itrid', 'Esfera', ($_POST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                <div class="row">
                    <div class="col-lg-4 estadoClass" style="margin-left: 17%;">
                        <?php echo $simec->select('estuf', 'Estado', $_POST['estuf'], $arrEstado, array('maxlength' => '255'), array()); ?>
                    </div>
                    <div class="col-lg-5" id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;">
                        <?php
                        $_REQUEST['muncod'] = tratarArrayParaMultiSelect($_REQUEST['muncod']);
                        if ($_REQUEST['estuf']) {
                            $municipio = new Territorios_Model_Municipio();
                            $arrMunicipio = simec_preparar_array($municipio->lista(array('muncod as codigo', 'mundescricao as descricao'), array("estuf = '{$_REQUEST['estuf']}'")));
                        }
                        echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255','multiple' => 'multiple'), array()); ?>
                    </div>
                </div>
                <div>
                    <?php
                    $arrTipoObjeto = Par3_Model_IniciativaPlanejamento::getTipoObjetoporIniciativa();
                    echo $simec->select('intoid', 'Tipo do Objeto', $_POST['intoid'], $arrTipoObjeto, array('maxlength' => '255'), array('input-size' => '4'));
                    ?>
                    <?php
                    $arrOptions = array('O' => 'Obras', 'P' => 'PAR','T' => 'Todos');
                    echo $simec->radio('tipo_processo', 'Tipo de Processo', $_REQUEST['tipo_processo']?:'T', $arrOptions, array(), array());
                    ?>
                    <div id="div_os_validada" style="display: <?php echo ($_REQUEST['tipo_processo'] == 'O' ? '' : 'none')?>;">
                            <?php echo $simec->radio('os_validada', 'OS Validada', $_REQUEST['os_validada'], ['S' => 'Sim','N' => 'Não','' => 'Todos'], array(), array()); ?>
                        </div>
                </div>
                <?php
                $anosprocesso = "SELECT DISTINCT substring(pronumeroprocesso,12,4) as codigo,  substring(pronumeroprocesso,12,4) as descricao FROM par3.processo WHERE substring(pronumeroprocesso,12,4) <> '' AND prostatus = 'A' ";
                echo $simec->select('anoprocesso', 'Ano', $_POST['anoprocesso'], $anosprocesso, array(), array('input-size' => '4'));
                ?>
                <div id="divTiposAssistencia">
                    <?php
                    $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
                    $mInta = $controllerIniciativaTiposAssistencia->recuperar();
                    $arrTiposAssistencia = simec_preparar_array($mInta->recuperarTodosFormatoInput('intadsc'));
                    $arrTiposAssistencia[0] = 'Todos';
                    echo $simec->select('intaid', 'Tipos de Assistência', $_POST['intaid'], $arrTiposAssistencia, ['placeHolder' => 'Selecione'], array('input-size' => '4','label-size' => '3'));
                    ?>
                </div>
                <?php
                $sql = "SELECT DISTINCT empsituacao AS codigo, empsituacao AS descricao FROM par3.empenho";
                echo $simec->select('empenhopagamento[]', 'Situação', $_POST['empenhopagamento'], $sql, array('maxlength' => '255', 'multiple' => 'multiple'), array('input-size' => '4','label-size' => '3')); ?>
            </div>
        </div>
    </div>
    <div class="ibox-footer">
        <div class="col-md-offset-4">
            <button type="submit" class="btn btn-info" name="pesquisar" onclick="$('#tipovisualizacao').val('html');"><i class="fa fa-search" aria-hidden="true"></i> Pesquisar</button> |
            <button type="button" class="btn btn-primary" name="excel" ><i class="fa fa-file-excel-o" aria-hidden="true"></i> Gerar Excel</button> |
            <button type="button" class="btn btn-warning" name="imprimir"><i class="fa fa-print" aria-hidden="true"></i> Imprimir</button> |
            <button type="button" class="btn btn-default-bright" name="limpar" onclick="window.location='par3.php?modulo=principal/orcamento/empenhoPar&acao=A';"><i class="fa fa-eraser" aria-hidden="true"></i> Limpar</button>
        </div>
    </div>
</form>
<div id="div-impressao" style="display: none">
        <div id="div_cabecalho" class="row col-lg-offset-2">
            <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
                <tr bgcolor="#ffffff">
                    <td valign="top" width="50" rowspan="2">
                        <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                    </td>
                    <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                        MEC/FNDE <br>
                        Relatório Emendas<br>
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
        <div id="impressao-content">

        </div>
</div>
<div class="float-e-margins" id="listagem-obras">
    <div class="ibox-content" id="div-listagem-obras" style="display: block;">
<?php

if ($_POST['tipovisualizacao']) {
    if ($_POST['numeroprocesso']) {
        $wh[] = "pro.pronumeroprocesso ilike '%".$_POST['numeroprocesso']."%'";
    }

    //if($_POST['tipo']) $wh[] = "vp.tipo = '".strtolower($_POST['tipo'])."'";
    if ($_POST['itrid']) {
        if ($_POST['itrid'] == 1) {
            $wh[] = "iu.itrid in(1,3)";
        } else {
            $wh[] = "iu.itrid={$_POST['itrid']}";
        }
    }
    if ($_POST['anoprocesso']) {
        $wh[] = "substring(pro.pronumeroprocesso,12,4)='".$_POST['anoprocesso']."'";
    }

    if ($_POST['estuf']) {
        $wh[] = "iu.estuf='".$_POST['estuf']."'";
    }
    
    if ($_POST['muncod'] && count($_POST['muncod']) != 0) {
        $arrMuncod = array_filter($_POST['muncod'], function ($val) {
            return !empty($val);
        });
            
            if (count($arrMuncod) != 0) {
                $wh[] = "iu.muncod::integer in (".implode(',', $arrMuncod).")";
            }
    }
    
    if ($_POST['proid']) {
        $wh[] = "pro.proid = {$_POST['proid']}";
    }
    
    if ($_POST['obr_inp_id']) {
        $whPlan = "AND a.inpid = {$_POST['obr_inp_id']}";
        $whObra = "AND o.obrid = {$_POST['obr_inp_id']}";
    };
    
    if ($_POST['intaid']) {
        $wh[] = " pro.intaid = {$_POST['intaid']} ";
    }

    if ($_POST['intoid']) {
        $wh[] = "obj.intoid = {$_POST['intoid']}";
    }

    if ($_POST['notaempenho']) {
        $wh[] = "pro.pronumeroprocesso IN (SELECT empnumeroprocesso FROM par3.empenho WHERE empnumero = '{$_POST['notaempenho']}' AND empstatus = 'A')";
    }

    if ($_POST['termo_validado'] == 'S') {
        $wh[] = "pro.proid in(select
                 dt.proid
                 from par3.documentotermo dt
                 inner join par3.documentotermovalidacao dtv ON dtv.dotid = dt.dotid
                 where dotstatus = 'A' ) ";
    }
    if ($_POST['termo_validado'] == 'N') {
        $wh[] =  "pro.proid not in(select dt.proid
                  from par3.documentotermo dt
                  inner join par3.documentotermovalidacao dtv ON dtv.dotid = dt.dotid
                  where dotstatus = 'A' ) ";
    }

    if ($_POST['termonota'] == 'S') {
        $wh[] =  "pro.proid in(select proid from par3.documentotermo inner join par3.execucao_notafiscal using (proid) where dotstatus = 'A' ) ";
    }

    if ($_POST['termonota'] == 'N') {
        $wh[] =  "pro.proid not in(select proid from par3.documentotermo inner join par3.execucao_notafiscal using (proid) where dotstatus = 'A' ) ";
    }
    
    if ($_POST['termo']) {
        $wh[] = "pro.proid IN (SELECT proid FROM par3.documentotermo WHERE dotnumero = '{$_POST['termo']}' AND dotstatus = 'A')";
    }

    $innerTermo = '';
    if ($_POST['termocontrato'] == 'S' || $_POST['termocontrato'] == 'N') {
        $termoWhere = '';
        switch ($_POST['termocontrato']) {
            case 'S':
                $termoWhere = 'and qtdcontrato.qtd > 0';
                break;
            case 'N':
                $termoWhere = 'and qtdcontrato.qtd = 0';
                break;
            default:
                break;
        }
        $innerTermo .= "inner JOIN (
                           select
                           count(ec.ecoid) as qtd,proc.proid,doc.dotnumero
                           FROM par3.processo proc
                           inner join par3.documentotermo doc ON doc.proid = proc.proid AND doc.dotstatus = 'A'
                           inner join par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = proc.pronumeroprocesso
                           left join par3.execucao_contrato ec ON ec.proid = proc.proid
                           group by proc.proid,doc.dotnumero
                       ) as qtdcontrato on qtdcontrato.proid = pro.proid {$termoWhere}";
    }
    if ($_POST['empenhopagamento']) {
        if (count($_POST['empenhopagamento']) > 1 || (count($_POST['empenhopagamento']) == 1 && $_POST['empenhopagamento'][0] != 'sem_empenho')) {
            $arrEmpSituacao = $_POST['empenhopagamento'];
            $empsituacao  = "(";
            $empsituacao .= ($arrEmpSituacao[0] == 'sem_empenho'? ' emps.empsituacao IS NULL ' :" emps.empsituacao ilike '%{$arrEmpSituacao[0]}%' ");
            unset($arrEmpSituacao[0]);
            if (count($arrEmpSituacao) == 1) {
                $empsituacao .= ($arrEmpSituacao[1] == 'sem_empenho'? 'OR emps.empsituacao IS NULL' :" OR emps.empsituacao ilike '%{$arrEmpSituacao[1]}%' ");
            } else {
                $empsituacao .= implode(' ', array_map(function ($val) {
                    return $val == 'sem_empenho'? 'OR emps.empsituacao IS NULL ':"OR emps.empsituacao ilike '%{$val}%' ";
                }, $arrEmpSituacao));
            }
        } else {
            $empsituacao ='(emps.empsituacao IS NULL ';
        }
        $empsituacao .= ")";
        $wh[] = $empsituacao;
    }
    
    $colunaEmpenhoExcel = '';
    $joinEmpenhoExcel = '';
    $groupxls = '';
    if ($_POST['tipovisualizacao'] == 'excel') {
        
        $colunaEmpenhoExcel .= ",CASE WHEN obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
                            	CASE WHEN cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
                            	CASE WHEN pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
                            	CASE WHEN siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
                            	CASE WHEN habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao,
                            	CASE WHEN contas = 't' THEN 'Sim' ELSE 'Não' END AS contas,
                                ARRAY_TO_STRING(array_agg(DISTINCT(
                                        case when obj.intoid = 1 then 
                                            (SELECT 
                                            CASE WHEN val.vldstatushomologacao = 'S' THEN 'Sim' ELSE 'Não' END 
                                            FROM obras2.obras obr
                                            INNER JOIN obras2.validacao val ON val.obrid = obr.obrid
                                            INNER JOIN par3.processoobracomposicao pp ON pp.obrid = obr.obrid_par3 AND pp.pocstatus = 'A'
                                            WHERE obr.obrstatus = 'A' AND obr.obridpai IS NULL AND pp.proid = pro.proid limit 1) else '-' 
                                        end 
                                    )), ', ', NULL) as os_validada";
        $joinEmpenhoExcel .= " left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = pro.inuid ";
       
        $groupxls =<<<GROUPXLS
       ,contas,
        vm.obras_par,
        vm.cacs,
        vm.pne,
        vm.siope,
        vm.habilitacao        
GROUPXLS;
    }

    if ($_POST['tipovisualizacao'] !='excel') {
        if($_REQUEST['tipo_processo'] != 'T'){
            if($_REQUEST['tipo_processo'] == 'P'){
                $wh[] = "tipo_processo = 'P' ";
            }
    
            if($_REQUEST['tipo_processo'] == 'O'){
                $wh[] = "tipo_processo = 'O' ";
            }
        }
    }
    if ($_REQUEST['tipo_processo'] == 'O'){
        if( $_REQUEST['os_validada'] == 'S' ){
            $whObra .= "AND pp.obrid IN (SELECT obrid_par3 FROM obras2.obras obr
					    							INNER JOIN obras2.validacao val ON val.obrid = obr.obrid
					    						WHERE obr.obrstatus = 'A' AND obr.obridpai IS NULL AND obrid_par3 IS NOT NULL
					    							AND val.vldstatushomologacao = 'S')";
        }
        if( $_REQUEST['os_validada'] == 'N' ){
            $whObra .= "AND pp.obrid IN (SELECT obrid_par3 FROM obras2.obras obr
					    							INNER JOIN obras2.validacao val ON val.obrid = obr.obrid
					    						WHERE obr.obrstatus = 'A' AND obr.obridpai IS NULL AND obrid_par3 IS NOT NULL
					    							AND val.vldstatushomologacao = 'N')";
        }
    }
    
    $sql = "SELECT DISTINCT 
                pro.proid,
                pro.proid as id,
                pro.pronumeroprocesso as processo,
                CASE WHEN iu.muncod IS NULL THEN est.estuf ELSE mun.estuf END AS uf,
            	CASE WHEN iu.muncod IS NULL THEN est.estdescricao ELSE mun.mundescricao END AS entidade,
                 array_to_string(array(SELECT DISTINCT t.intodsc FROM par3.iniciativa_tipos_objeto t
                INNER JOIN(
                SELECT intoid, proid FROM par3.processoparcomposicao WHERE ppcstatus = 'A'
                UNION ALL
                SELECT intoid, proid FROM par3.processoobracomposicao WHERE pocstatus = 'A'
                ) pp ON pp.intoid = t.intoid
                WHERE t.intostatus = 'A'
                AND pp.proid = pro.proid), ', ') AS tipo_obrjeto,
                sum(vip.vlriniciativa::NUMERIC(20,2)) AS valor_processo,
            	vs.vlrempenho,
                CASE WHEN sum(vip.vlriniciativa::NUMERIC(20,2)) > 0 THEN 
                ((coalesce(vs.vlrempenho,0) / sum(vip.vlriniciativa::NUMERIC(20,2))) * 100)::numeric(20,2)||'%' ELSE 0||'%' END AS percent,
                vs.vlr_pg_efetivado as vlrpago,
                ta.intadsc as tipo,
            	CASE WHEN emps.empsituacao is null then 'Sem empenho' else emps.empsituacao end as empsituacao
                {$colunaEmpenhoExcel},
                pro.inuid as inuid_pendencia
            FROM par3.processo pro
                INNER JOIN(
                  	SELECT 
                  	  pp.proid,
                  	  sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa,
                  	  pp.intoid,
                      'P' as tipo_processo
            	    FROM par3.iniciativa_planejamento_item_composicao ipi
            			INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
            			INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
            			INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid and pp.ppcstatus = 'A'
            		WHERE ipi.ipistatus = 'A' $whPlan
            		GROUP BY pp.proid, pp.intoid
                  	UNION ALL
                     SELECT 
                        proid,
                        sum(vlr_obra) AS vlriniciativa,
                        intoid,
                        'O' as tipo_processo
                    FROM(
                     	SELECT pp.proid, (o.obrvalor - coalesce(o.obrvalor_contrapartida,0) ) AS vlr_obra, pp.intoid FROM par3.obra o
                     		INNER JOIN par3.processoobracomposicao pp ON pp.obrid = o.obrid AND pp.pocstatus = 'A'
                     	WHERE o.obrstatus = 'A' $whObra
                     ) AS foo
                     GROUP BY proid, intoid
                 ) vip ON vip.proid = pro.proid
                 $innerTermo
                 $joinEmpenhoExcel
                 INNER JOIN par3.instrumentounidade iu ON iu.inuid = pro.inuid AND iu.inustatus = 'A'
                 INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = pro.intaid
                 INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = vip.intoid
                 left JOIN par3.vm_situacao_empenho_pagamento_processo emps ON emps.empnumeroprocesso = pro.pronumeroprocesso
                 LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
                 LEFT JOIN territorios.estado est ON est.estuf = iu.estuf
                 LEFT JOIN par3.v_saldo_empenho_do_processo vs ON vs.processo = pro.pronumeroprocesso
              WHERE pro.prostatus::text = 'A'
                ".(($wh)?" and ".implode(" AND ", $wh):"")."";
    $sql .=<<<GROUPBY
            
            GROUP BY
            pro.proid,
            processo,
            uf,
            iu.muncod,
            est.estdescricao,
            mun.mundescricao,
            vs.vlrempenho,
            vs.vlrempenho,
            vs.vlr_pg_efetivado,
            ta.intadsc,
            emps.empsituacao
            $groupxls

GROUPBY;
    $sql.= " ORDER BY uf, entidade, processo ";
//    ver($sql,d);
    if ($_POST['tipovisualizacao'] == 'pdf') {
        ob_clean();
        echo relatorioEmpenhoPDF($sql);
        die;
    }

    if ($_POST['tipovisualizacao']=='excel') {
        ob_clean();
        if ($_POST['tipodetalhado'] == 'sim') {
            $cabecalho = array(
                'Planejamento', 'Ano', 'Termo','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Usuário', 'Data Criação', 'Data Vigência',
                'Valor Processo', 'Valor Termo', 'Nota do empenho', 'Valor Empenho', '% Empenho', 'Valor Pagamento', 'Assistência', 'Situação última solicitação', 
                'Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita', 'Contas', 'ID (PAR3)', 'ID (OBRAS2)', 'Situação PAR3', 'Situação Obras2', 'Validação da OS', '% Físico total', '% Executado Instituição'
            );
            
            $campoNumerico = ['valor_processo', 'dotvalortermo', 'vlrempenho', 'vlrpago'];
            $sqlP = sqlListarExcelDetalhadoPlanejamento($wh, $whPlan);
            $sqlO = listarExcelDetalhadoObra($wh, $whObra);
            
            if( $_REQUEST['tipo_processo'] == 'O' ){
                $sql = $sqlO;
            }elseif( $_REQUEST['tipo_processo'] == 'P' ){
                $sql = $sqlP;
            } else {
                $sql = '('.$sqlO.') union all ('.$sqlP.')';
            }
            
        } else {
            $cabecalho = ["Código", "Processo","UF","Entidade", "Tipo Objeto", "Valor Processo","Valor Empenho","% Empenho", 
                          "Valor Pagamento", "Assistência","Situação última solicitação",
                'Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita', 'Contas', 'OS validada'];
            $campoNumerico = false;
        }
        relatorioEmpenhoExcel($sql, $cabecalho, $campoNumerico);
        exit();
    } else {    
        $cabecalho = array("Processo","UF","Entidade", "Tipo Objeto", "Valor Processo","Valor Empenho","% Empenho", "Valor Pagamento", "Assistência","Situação", "Bloqueio PAR");
        $listagem = new Simec_Listagem();
        $listagem->esconderColunas('itrid', 'obr_inp_id', 'muncod','tipo_processo', 'id');
        $listagem->setCabecalho($cabecalho);
        $listagem->turnOnPesquisator();
        $listagem->setQuery($sql);
        $listagem->setId('listagemEmpenhos');
        $listagem->addAcao('plus', array('func' => 'detalharPagamento', 'extra-params' => array('tipo','processo')));
        $listagem->addAcao('edit', array('func' => 'carregarProcesso', 'extra-params' => array('tipo_obrjeto'), 'titulo' => 'Editar'));
        $listagem->setFormFiltros('formulario');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->setTamanhoPagina(50);
        $listagem->setCampos($cabecalho);
        $listagem->addCallbackDeCampo('valor_processo', 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo(['vlrempenho', 'vlrpago'], 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo('processo', 'formata_numero_processo_component');
        $listagem->addCallbackDeCampo('tipo_obrjeto', 'carregaHistoricoObra');
        $listagem->addCallbackDeCampo(['inuid_pendencia'], 'popOverPendenciarsPar');
        $listagem->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
}

?>
        </div>
    </div>
</div>

<script type="text/javascript">
    var Empenho = {
        iniciar:function(){
            $('button[name="excel"]').on('click',function(){
                Empenho.gerarExcel();
            });
        },
        gerarExcel:function(){
            /**/
            swal("Selecione o leitaute para gerar o relatório.", {
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
                        break;
                    case 'sintetico':
                        $('#tipodetalhado').val('nao');
                        break;
                    default:
                        return;
                }
                $('#tipovisualizacao').val('excel');
                $('form[name="formulario"]').submit();
            });
        }
    };
    jQuery(document).ready(function(){
        Empenho.iniciar();
        jQuery("[data-id-lista='listagemEmpenhos']").remove();
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

function carregarDadosPlanoInterno(pi,ptres) {
    ajaxatualizar('requisicao=carregarFonteRecurso&inplintdsc='+pi+'&inplintptres='+ptres,'dvfonterecurso');
}

function carregarDadosPtres(id, proid) {
    ajaxatualizar('requisicao=carregarPlanoInterno&proid='+proid+'&inplintptres='+id,'dvplanointerno');
}

function exibirConsultarEmpenho(empid) {
    $('#empid_autenticacao').val(empid);
    $('#dvautenticacaosigef').modal();
    jQuery('#div_botao_consulta').show();
    jQuery('#div_botao_cancela').hide();
    jQuery('#div_cancelar').hide();
    jQuery(".loading-dialog-par4").show();
}

function consultarEmpenho(proid) {
    if( $('form[name="fmautenticar"] [name="wsusuario"]').val() == '' ){
        alert('Informe o Usuário');
    }else if( $('form[name="fmautenticar"] [name="wssenha"]').val() == '' ){
        alert('Informe a Senha');
    } else {
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: 'requisicao=consultarEmpenho&'+$('#fmautenticar').serialize(),
            async: false,
            success: function(html){
                swal("CONSULTA DE EMPENHO", html);
                //$('#debug').html(html);
            }
        });
        
        $('#dvautenticacaosigef').modal('toggle');
        ajaxatualizar('requisicao=solicitarEmpenhoPar&proid='+proid,'dv-form-large');
        $('#modal-form-large').modal('show');
    }
}

function cancelarEmpenhoWS( proid ) {
    var empid = $('#empid_autenticacao').val();
    var protocolo = $('#protocolo_autenticacao').val();
    var valor_empenho = $('#valor_empenho_cancel').val();
    
    /*if( $('input[name=cancelar_empenho]:checked').length == 0 ){
        swal("CANCELAR EMPENHO", 'Informe o tipo de cancelamento de empenho(Parcial ou Total)');
        return false;
    } else*/
    if( $('#valor_cancelado').val() == '' || parseFloat($('#valor_cancelado').val()) <= parseFloat(0) ){
        swal("CANCELAR EMPENHO", 'É necessário informar os campos obrigatórios!');
        return false;
    } else {
        if( $('form[name="fmautenticar"] [name="wsusuario"]').val() == '' ){
            alert('Informe o Usuário');
        }else if( $('form[name="fmautenticar"] [name="wssenha"]').val() == '' ){
            alert('Informe a Senha');
        } else {
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: 'requisicao=cancelarEmpenho&='+$('#fmautenticar').serialize()+'&empid='+$('#empid_autenticacao').val()+'&proid='+proid,
                async: false,
                success: function(html){
                    swal("CANCELAR EMPENHO", html);
                    //$('#debug').html(html);
                }
            });
        
            $('#dvautenticacaosigef').modal('toggle');
            ajaxatualizar('requisicao=solicitarEmpenhoPar&proid='+proid,'dv-form-large');
            $('#modal-form-large').modal('show');
        }
    }
}

function exibirCancelarEmpenho(empid, protocolo, valorsaldo, numeroempenho) {

    var retorno = validaCancelamentoEmpenho( empid );

    if( retorno == 'S' ){
        swal("Ação não Permitida!", "Não é possivel cancelar o empenho "+numeroempenho+", pois este existe Termo ativo!", "error");
    } else {
        $('#empid_autenticacao').val(empid);
        $('#protocolo_autenticacao').val(protocolo);
        $('#valor_empenho_cancel').val(valorsaldo);
        $('#dvautenticacaosigef').modal();
        jQuery('#div_botao_consulta').hide();
        jQuery('#div_botao_cancela').show();
        jQuery('#div_cancelar').show();

        $('.loading-dialog-par3').show();
    
        jQuery(".loading-dialog-par4").show();
    
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: 'requisicao=lista_empenho_cancela&empid='+empid,
            async: true,
            success: function(html){
                $('#lista_empenho').html(html);
                $('.loading-dialog-par3').hide();
            }
        });
    }
}

function validaCancelamentoEmpenho( empid ){
    var retorno = '';
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: 'requisicao=valida_empenho_cancela&empid='+empid,
        async: false,
        success: function(html){
            //swal("Ação não Permitida!", html, "error");
            //$('#lista_empenho').html(html);
            retorno = html;
        }
    });
    return retorno;
}


function selecionarTipoCancelamento( codigo ) {
    if( jQuery('[name="codigo[]"]:checked').length > 0 ){
        $('#valor_cancelar_'+codigo).prop("readonly",false);
        $('#porc_cancelar_'+codigo).prop("readonly",false);
        $('#valor_cancelar_'+codigo).val( '' );
        $('#porc_cancelar_'+codigo).val('');
    }else {
        $('#valor_cancelar_'+codigo).prop("readonly",true);
        $('#porc_cancelar_'+codigo).prop("readonly",true);
        $('#valor_cancelar_'+codigo).val('');
        $('#porc_cancelar_'+codigo).val('');
        var valor = 0;
        jQuery('[name="codigo[]"]').each(function(){
            var cod = jQuery(this).val();
            valor = parseFloat(valor) + parseFloat(replaceAll(replaceAll($('#valor_cancelar_'+cod).val(),'.',''),',','.'));
        });
        jQuery('[name="valor_cancelado"]').val( number_format(valor, 2, ',', '.') );
    }
}

function calcularValorCancelar(tipo, codigo){

    var valorempenhado = $('#valorempenhado_cancel_'+codigo).val();
    var vlr_pago = $('#vlr_pago_'+codigo).val();
    //var valor_empenho_cancel = $('#valor_empenho_cancel').val();

    var valor_empenho_cancel = parseFloat(valorempenhado) - parseFloat(vlr_pago);
      console.log(valor_empenho_cancel)
    if( valor_empenho_cancel > 0 ){
        switch(tipo) {
            case 'valor':
                var valor_cancelar = parseFloat(replaceAll(replaceAll($('#valor_cancelar_'+codigo).val(),'.',''),',','.'));
    
                if(parseFloat(valor_cancelar) > parseFloat(valor_empenho_cancel)){
                    valor_cancelar = valor_empenho_cancel;
    
                    jQuery('#valor_cancelar_'+codigo).val(  number_format(valor_cancelar, 2, ',', '.') );
                }
                
                var por_emp = Math.floor((valor_cancelar/valor_empenho_cancel)*100);
                $('#porc_cancelar_'+codigo).val(por_emp);
            break;
            case 'porcentagem':
                var percentual = parseFloat(replaceAll(replaceAll($('#porc_cancelar_'+codigo).val(),'.',''),',','.'));
                
                if( percentual > 100 ){
                    percentual = 100;
                    jQuery('#porc_cancelar_'+codigo).val(100)
                }
                var valorCalculo = ((percentual/100)*valor_empenho_cancel);
    
                jQuery('#valor_cancelar_'+codigo).val(  number_format(valorCalculo, 2, ',', '.') );
            break;
        }
    } else {
        $('#valor_cancelar_'+codigo).val('0');
        $('#porc_cancelar_'+codigo).val('0');
    }
    
    var valor = 0;
    jQuery('[name="codigo[]"]').each(function(){
        var cod = jQuery(this).val();
        var valor_cancelar = replaceAll(replaceAll($('#valor_cancelar_'+cod).val(),'.',''),',','.');
        if( valor_cancelar == '' ) valor_cancelar = 0;
        valor = parseFloat(valor) + parseFloat(valor_cancelar);
    });
    jQuery('[name="valor_cancelado"]').val( number_format(valor, 2, ',', '.') );
}

function visualizarXMLSigef() {

    if($('#entcnpj').val()=='') {
        alert('CNPJ é obrigatório');
        return false;
    }

    if($('#pronumeroprocesso').val()=='') {
        alert('Número do Processo é obrigatório');
        return false;
    }

    if($('#valortotalempenho').val()=='') {
        alert('Valor Total do Empenho é obrigatório');
        return false;
    }

    if($('#ptres').val()=='') {
        alert('PTRes é obrigatório');
        return false;
    }

    if($('#planointerno').val()=='') {
        alert('Plano Interno é obrigatório');
        return false;
    }

    if($('#fonterecurso').val()=='') {
        alert('Fonte de Recurso é obrigatório');
        return false;
    }

    if($('#centrogestao').val()=='') {
        alert('Centro de Gestão é obrigatório');
        return false;
    }

    ajaxatualizar('visualizarxmlsigef=1&'+$('#fmconfirmarempenho').serialize(),'dvdadosvisualizarxmlsigef');
    
    $('#dvvisualizarxmlsigef').modal();
    $('#modal-form-large').modal('hide');
    $('#modal-form-large').modal('hide');

    
}

function selecionarSubacaoEmpenho(id) {
    if($('input[id=chk_'+id+']:checked').length) {
        $('#valor_empenhar_'+id).prop("readonly",false);
        $('#porc_empenhar_'+id).prop("readonly",false);
    } else {
        $('#valor_empenhar_'+id).val('');
        $('#porc_empenhar_'+id).val('');
        $('#valor_empenhar_'+id).prop("readonly",true);
        $('#porc_empenhar_'+id).prop("readonly",true);
        calcularValorTotalEmpenho();
    }
}

function ajaxatualizar(params,iddestinatario) {
    jQuery("#loading").show();
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: params,
        async: false,
        success: function(html){
            if(iddestinatario!='') {
                document.getElementById(iddestinatario).innerHTML = html;
            }
        }
    });

}

function carregarProcesso(proid, tipo) {
    ajaxatualizar('requisicao=solicitarEmpenhoPar&proid='+proid+'&tipoobjeto='+tipo,'dv-form-large');
    $("#modal-form-large").modal();

    if( parseFloat(jQuery('#td_cancelamento_valor').html()) > parseFloat(0) ){
        jQuery('.td_cancelamento').show();
    } else {
    	jQuery('.td_cancelamento').hide();
    }
}

function trocarIcone() {
    if($("#demo").attr('class')=='ibox-content collapse in') {
        $("#ic_avancadas").attr('class', 'fa fa-chevron-up');
    } else {
        $("#ic_avancadas").attr('class', 'fa fa-chevron-down');
    }
}

function calcularIniciativas(id, tipo) {

    switch(tipo) {
        case 'valor':
            var vlr_tot = parseFloat($('#totalitem_'+id).val());
            if($('#valorempenhado_'+id).val()==''){var vlr_emp = 0;}else{var vlr_emp = parseFloat($('#valorempenhado_'+id).val());}
            if($('#porcempenhado_'+id).val()=='') {var por_emo = 0;} else {var por_emo = parseInt($('#porcempenhado_'+id).val());}
            var vlr_ini = parseFloat(replaceAll(replaceAll($('#valor_empenhar_'+id).val(),'.',''),',','.'));
            var saldo = (vlr_tot-vlr_emp);
            if(vlr_ini >= saldo) {
                $('#valor_empenhar_'+id).val(mascaraglobal('###.###.###.###,##', saldo.toFixed(2)));
                vlr_ini = saldo;
                var por_emp = (100-por_emo);
            } else {var por_emp = Math.floor((vlr_ini/vlr_tot)*100);}
            $('#porc_empenhar_'+id).val(por_emp);
            break;
        case 'porcentagem':
            
            var vlr_tot = parseFloat($('#totalitem_'+id).val());
            
            if($('#valorempenhado_'+id).val()=='') {var vlr_emp = 0;} else {var vlr_emp = parseFloat($('#valorempenhado_'+id).val());}
            if($('#porcempenhado_'+id).val()=='') {var por_emo = 0;} else {var por_emo = parseInt($('#porcempenhado_'+id).val());}
            
            
            var por_emp = parseInt($('#porc_empenhar_'+id).val());
            var vlr_ini = vlr_tot*(por_emp/100);
            var saldo = (vlr_tot-vlr_emp);

            if((por_emo+por_emp)==100) {
                $('#valor_empenhar_'+id).val(mascaraglobal('###.###.###.###,##', saldo.toFixed(2)));
            } else if(vlr_ini > saldo) {
                $('#valor_empenhar_'+id).val(mascaraglobal('###.###.###.###,##', saldo.toFixed(2)));
                calcularIniciativas(id, 'valor');
            } else {
                $('#valor_empenhar_'+id).val(mascaraglobal('###.###.###.###,##', vlr_ini.toFixed(2)));
            }
            
            break;
    }

    calcularValorTotalEmpenho();
}

function calcularValorTotalEmpenho() {

    var total = 0;
    
    $("[id^='chk_']:checked").each(function() {
        var vlr_ini = parseFloat(replaceAll(replaceAll($('#valor_empenhar_'+$(this).val()).val(),'.',''),',','.'));

        total += vlr_ini;
        
    });
    
    if(total > 0) {
        $('#valortotalempenho').val(mascaraglobal('###.###.###.###,##', total.toFixed(2)));
    } else {
        $('#valortotalempenho').val('');
    }
}

function solicitarEmpenho() {

    if($('#valortotalempenho').val()=='') {
        swal("Alerta", "Selecione as iniciativas a serem empenhadas", "error")
        return false;
    }
    
    $('#dvconfirmarempenho').modal();
    
}


function exibirComposicaoEmpenho(empid) {

    ajaxatualizar('requisicao=exibirComposicaoEmpenho&empid='+empid,'dvdadoscomposicaoempenho');
    $('#dvcomposicaoempenho').modal();
}

function exibirComposicaoIniciativaPar(ppcid) {

    ajaxatualizar('requisicao=exibirComposicaoIniciativaPar&ppcid='+ppcid,'dvdadoscomposicaoiniciativa');
    $('#dvcomposicaoiniciativa').modal();

}

function exibirComposicaoIniciativaObra(pocid) {

    ajaxatualizar('requisicao=exibirComposicaoIniciativaObra&pocid='+pocid,'dvdadoscomposicaoiniciativa');
    $('#dvcomposicaoiniciativa').modal();

}

function carregarMunicipio(estuf, muncod) {
    if(estuf != '') {
        var options = jQuery('#muncod');
        options.empty();
        options.append(new Option('', ''));
        jQuery.post('', 'requisicao=carregaMunicipios&estuf='+estuf, function(retorno) {
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

$(function(){
    jQuery('select[name=estuf]').change(function(){
        carregarMunicipio(this.value);
    });

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').val('');
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });

    /*$('[name=tipo]').change(function(){
        if($(this).val() == 'OBRA'){
            $('#div-intoid').hide();
            $('#intoid').val('');
            $('#intoid').trigger("chosen:updated");
        }else{
            $('#div-intoid').show();
        }
    });*/

    //tela de loading
    $('[name=excel]').click(function () {
        $('.loading-dialog-par3').hide();
    });
    $(window).on('beforeunload',function(){
        if($('#tipovisualizacao').val() != 'excel'){
            $('.loading-dialog-par3').show();
        }
    });
    $(window).on('click','#pesquisar #limpar',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).load(function(){
        $('.loading-dialog-par3').hide();
    });


    $("[name=imprimir]").on("click", function () {
        $('#tipovisualizacao').val('pdf');
        var action = $('#formulario').serialize();
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            dataType: 'html',
            async: false,
            success: function (resp) {
                $("#impressao-content").html(resp);
                var container = $("#impressao-content");
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
                w.document.write($('#div-impressao').html());
                w.document.title = 'Relatório - Emendas';
                w.print();
                w.close();
                $("#impressao-content").html("");
                $('#tipovisualizacao').val('html');
                return true;
            }
        });
    });
});

$('[name=tipo_processo]').change(function () {
    let osValidada = $('#div_os_validada');
    if ($(this).val() == 'O') {
        osValidada.show();
    }else{
        osValidada.hide();
    }
});
</script>
<script src='../zimec/public/temas/simec/js/plugins/sweetalert/swal.js'></script>