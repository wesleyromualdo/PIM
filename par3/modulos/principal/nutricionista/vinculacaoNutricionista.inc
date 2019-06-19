<?php
/**
 * Lista de Municipios
 *
 * @category Lista
 * @package  A1
 * @author   Marcus Rocha <marcusrocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

$modelDadosnutricionista = new Par3_Model_DadosNutricionista($_REQUEST['dnid']);

$renderNutricionista    = new Par3_Controller_Entidade();
$controleUnidade        = new Par3_Controller_InstrumentoUnidade();

switch ($_REQUEST['requisicao']) {
    case 'salvarVinculacao':

        $vinculacaonutricionista = new Par3_Model_VinculacaoNutricionista($_REQUEST['vnid']);
        $vinculacaonutricionista->vndatavinculacao      = $_REQUEST['vndatavinculacao'];
        $vinculacaonutricionista->vncargahorariasemanal = $_REQUEST['vncargahorariasemanalo'];
        $vinculacaonutricionista->vnatuacaoexclusivaei  = (($_REQUEST['vnatuacaoexclusivaei'] == 'undefined') || ($_REQUEST['vnatuacaoexclusivaei'] == 'f')) ? 'f' : 't';
        $vinculacaonutricionista->dntvid                = $_REQUEST['dntvido'];
        $vinculacaonutricionista->salvar();
        $vinculacaonutricionista->commit();

        echo "success";
        die();

        break;
    case 'validacaoNutricionista':
        ob_clean();

        $controleUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
        $dados['vnid'] = $_POST['vnid'];
        $dados['snid'] = $_POST['snid'];
        $controleUnidadeEntidade->validacaoNutricionista($dados);
        die();
        break;
    case 'gerarTermoRT':
        ob_clean();

        $arrValidacao = explode('/', $_REQUEST['vndatavinculacao']);

        $teste = checkdate($arrValidacao[1],$arrValidacao[0],$arrValidacao[2]);
        if(!$teste) die('Data inválida');

        $controleVinculacao = new Par3_Controller_VinculacaoNutricionista();
        $controleVinculacao->gerarTermoRT($_REQUEST);
        die();
        break;
    case 'aceitarRT':
        $modelDadosnutricionista = new Par3_Model_DadosNutricionista();
        $modelDadosnutricionista->carregarDadosPorCPF($_SESSION['usucpf']);

        $vinculacaonutricionista = new Par3_Model_VinculacaoNutricionista($_REQUEST['vnid']);
        $vinculacaonutricionista->snaceito = $_REQUEST['vnaceito'];
        if($modelDadosnutricionista->danid){
            $vinculacaonutricionista->snid = 1;
        }
        $vinculacaonutricionista->salvar();
        $vinculacaonutricionista->commit();

        $url = "par3.php?modulo=principal/nutricionista/vinculacaoNutricionista&acao=A";
        simec_redirecionar($url, 'success');
        die();
    case 'vertermodesvinculado':
        ob_clean();
        $vinculacaonutricionista = new Par3_Model_VinculacaoNutricionista($_POST['vnid']);
        echo $vinculacaonutricionista->recuperarTermoDesviculacao();
        die;
        break;
    default:
        $abas = array(
            0 => array("descricao" => "Vinculação",     "link" => "par3.php?modulo=principal/nutricionista/vinculacaoNutricionista&acao=A"),
            1 => array("descricao" => "Dados Pessoais", "link" => "par3.php?modulo=principal/nutricionista/cadNutricionista&acao=A"),
            2 => array("descricao" => "Desvinculação",  "link" => "par3.php?modulo=principal/nutricionista/desvinculacaoNutricionista&acao=A"),
        );
        break;
}

echo $simec->title('Lista Cadastro Nutricionista', '');
?>
<style>
    .chosen-container {
        width: 170px !important;
    }
    .tabela-listagem2 td {
    	vertical-align: middle !important;
    }
    .tabela-listagem2 td .form-group {
    	margin-bottom: 4px;
    }
</style>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <input type="hidden" name="req" value="salvarDadosnutricionista"/>
        <?php echo $simec->tab($abas, 'par3.php?modulo=principal/nutricionista/vinculacaoNutricionista&acao=A'); ?>
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Vinculação de nutricionista</h5>

                <div class="ibox-tools">
                    <a class="collapse-link"> <i class="fa fa-chevron-up"></i>
                    </a>
                </div>
            </div>
            <div class="ibox-content">
                <input type="hidden" name="req" value="salvar">
                <table class="table tabela-listagem2">
                    <thead>
                    <tr>
                        <th style="vertical-align: bottom;" width="10%">Entidade</th>
                        <th style="vertical-align: bottom;" width="5%">Vinculado</th>
                        <th style="vertical-align: bottom;" width="5%">Desvinculado</th>
                        <th style="vertical-align: bottom;" width="5%">Validação</th>
                        <th style="vertical-align: bottom;" width="15%">Data de vinculação</th>
                        <th style="vertical-align: bottom;" width="5%">Carga Horária Semanal</th>
                        <th style="vertical-align: bottom;" width="10%">Data de desvinculação</th>
                        <th style="vertical-align: bottom;" width="15%">Tipo de vínculo</th>
                        <th style="vertical-align: bottom;" width="15%">Cargo/Função</th>
                        <th style="vertical-align: bottom;" width="15%">Termo de Desvinculação</th>

                        <!--                        <th style="vertical-align: bottom;" width="15%">Atuação exclusiva</th>-->
<!--                        <th style="vertical-align: bottom;" width="10%">Responsável Técnico</th>-->
                    </tr>
                    </thead>
                    <tbody>
                    <?php
                    $simec->setFormTipo(Simec_View_Helper::K_FORM_TIPO_VERTICAL);
                    $iue = new Par3_Model_InstrumentoUnidadeEntidade();
                    $vinculacao = new Par3_Model_VinculacaoNutricionista();
                    $arrFiltro = array(
                        "vncpf = '{$_SESSION['usucpf']}'",
                        "t1.tenid IN (7,8,16)",
                    );
                    $dados = $vinculacao->recuperarTodos('t1.* , t2.tendsc as tendsc', $arrFiltro, null, ['join' => 'tenid']);
                    foreach ($dados as $key) {
                        $desvinculado = $key['vnstatus'] == 'I';
                        $dadosEntidade = $iue->carregarDadosPorCpf($key['vncpf'], $key['inuid']);
                        $enabled = '';
                        $display = 'block';
                        if (($key['snid'] == 2) || ($key['snid'] == 3)) {
                            $enabled = 'disabled=\"disabled\"';
                            $display = 'none';
                            if ($key['snid'] == 3) {
                                $valBtValidacao = 'Validar';
                                $style = '';
                            } else {
                                $valBtValidacao = 'Não validado';
                            }
                        } elseif (($key['snid'] == 1) || ($key['snid'] == 6) || ($key['snid'] == 4) || ($key['snid'] == 5)) {
                            $valBtValidacao = 'Validado';
                        } elseif ($key['datavinculacao'] == '') {
                            $enabled = 'disabled=\"disabled\"';
                        }
                        ?>
                        <tr <?php echo (($desvinculado) ? 'style="background-color:#ffcccc"' : '')?>
                            <?php
                            if(!$desvinculado){
		                            if (($key['snid'] == 1) || ($key['snid'] == 6) || ($key['snid'] == 4) || ($key['snid'] == 5) || ($key['snid'] == 3)) {
		                                echo 'data-acao="' . $key['vnid'] . '"';
		                            }
                              }
                            ?>>

                            <td><?php echo $unidadeAtual = $controleUnidade->pegarNomeEntidade($key['inuid']) ?></td>
                            <td style="text-align: center">
                                <?php if (!$desvinculado) { ?>
                                    <icon class="fa fa-check primary"></icon> <?php } ?>
                            </td>
                            <td>
                                <?php if ($desvinculado) { ?>
                                    <icon class="fa fa-check primary"></icon> <?php } ?>
                            </td>
                            <td>
                                <?php if (!($desvinculado && $key['snid'] == 3)) { ?>
                                    <button class="btn btn-success" id="bt_valida_<?php echo $key['vnid'] ?>"
                                            data-toggle="modal"
                                            data-target="#modal<?php echo $key['vnid'] ?>"><?php echo $valBtValidacao ?></button>
                                    <div class="ibox float-e-margins animated modal"
                                         id="modal<?php echo $key['vnid'] ?>" tabindex="-1" role="dialog"
                                         aria-hidden="true" style="display: none">
                                        <div class="modal-dialog">
                                            <div class="modal-content animated flipInY">
                                                <div class="ibox-title esconde " tipo="integrantes">
                                                    <button type="button" class="close" data-dismiss="modal"
                                                            aria-label="Close"><span
                                                            aria-hidden="true">&times;</span></button>
                                                    <h3>Validação</h3>
                                                </div>
                                                <div class="ibox-content">

                                                    <div
                                                        style="font: 12pt Arial,verdana;  text-align: justify !important; ">
                                                        Declaro, sob as penas da lei, que as informações aqui prestadas
                                                        são a expressão da verdade e que o profissional cadastrado
                                                        atende ao disposto na Resolução nº 465/2010 do Conselho Federal
                                                        de Nutrição.
                                                    </div>
                                                    <?php $enabledRadio = ($key['snid'] != '3' || $desvinculado) ? "disabled=disabled" : ""; ?>
                                                    <br><input type="radio" <?= $enabledRadio ?>  value="6"
                                                               name="snid_<?= $key['vnid'] ?>"
                                                               id="snid_<?= $key['vnid'] ?>" <?php echo ($key['snid'] == '1' || $key['snid'] == '4' || $key['snid'] == '5' || $key['snid'] == '6') ? "checked=checked" : ""; ?>
                                                               name="snid_<?= $key['vnid'] ?>"
                                                               onclick="defineValidacao(this.value, <?= $key['vnid'] ?>, '<?=  addslashes($unidadeAtual)  ?>');">Validar
                                                    &nbsp;&nbsp;
                                                    <input type="radio" <?= $enabledRadio ?>  value="2"
                                                           name="snid_<?= $key['vnid'] ?>"
                                                           id="snid_n_<?= $key['vnid'] ?>" <?php echo ($key['snid'] == '2') ? "checked=checked" : ""; ?>
                                                           onclick="defineValidacao(this.value, <?= $key['vnid'] ?>,"<?=  addslashes($unidadeAtual)  ?>");">Não
                                                    Validar
                                                </div>
                                                <div class="ibox-footer">
                                                    <div class="mensagemOK<?= $key['vnid'] ?>"
                                                         style="border: 3px solid #5DD65D;padding: 5px;text-align: center;background: #E6FFD4;font-size: 14px;display: none">
                                                        Procedimento realizado com sucesso!
                                                    </div>
                                                    <div align="right">
                                                        <button onclick="location.reload()" class="btn btn-success">
                                                            Fechar
                                                        </button>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                <?php } ?>
                            </td>
                            <td>
                                <?php
                                $readOnly = "";
                                    $dadosInput = array();
                                    $dadosInput['required'] = 'required';
                                    $dadosInput['maxlength'] = '100';
                                    $dadosInput['style'] = 'width:100px';
                                    if ($key['snid'] != 6) {
                                        $readOnly = 'readonly';
//                                        $dadosInput['readonly'] = 'readonly';
                                        $enabledRadio2 = 'readonly="readonly"';
                                    }
//                                    echo $simec->data('vndatavinculacao_'.$key['vnid'], '',$key['vndatavinculacao'], $dadosInput);
                                    $dataVal = formata_data($key['vndatavinculacao']);
                                if($desvinculado){
                                    echo $dataVal;
                                }else{
                                ?>
                                <div class="form-group" id="data_1">
                                    <div class="input-group date">
                                        <span class="input-group-addon"><i class="fa fa-calendar"></i></span>
                                        <input type="text" name="<?php echo 'vndatavinculacao_'.$key['vnid']?>"
                                               class="form-control" value="<?php echo $dataVal ?>"
                                               <?php echo $readOnly?>
                                        />
                                    </div>
                                </div>
                                <?php } ?>
                            </td>
                            <td>
                                <?php
                                	$dadosInput = array();
                                    $dadosInput['style'] = 'width:70px';
                                    $dadosInput['type'] = 'number';
                                    if($desvinculado){
                                        echo $key['vncargahorariasemanal'];
                                    }else{
                                        echo $simec->input('vncargahorariasemanalo_'.$key['vnid'], '', $key['vncargahorariasemanal'], $dadosInput);
                                    }
                                ?>
                            </td>
                            <td><?php echo formata_data($key['vndatadesvinculacao']) ?></td>
                            <td>
                                <?php
                                    $sql = "SELECT
                                                dntvid as codigo,
                                                dntvdescricao as descricao
                                            FROM
                                                par3.dadosnutricionistatipovinculo ";
                                    if($desvinculado){
                                        echo $key['dntvid'] != '' ? $modelDadosnutricionista->pegaUm("SELECT dntvdescricao FROM par3.dadosnutricionistatipovinculo WHERE dntvid = ".$key['dntvid']) : '';
                                    }else{
                                        echo $simec->select("dntvido_".$key['vnid'], null, $key['dntvid'], $sql, array('maxlength' => '255', ($key['vnstatus'] == 'I' ? 'disabled' : null), 'required'));
                                    }
                                ?>
                            </td>
                            <td><?php echo $key['tendsc'];?></td>
                            <td>
                                <?php if($desvinculado): ?>
                                    <button class="btn  btn-success" onclick="verTermo(<?php echo $key['vnid']?>)">Ver Termo</button>
                                <?php endif; ?>
                            </td>
<!--                            <td>-->
<!--                                --><?php
//                                if($key['vnstatus'] == 'I'){
//                                    echo (($key['vnatuacaoexclusivaei'] == 't') ? 'Sim' : 'Não');
//                                } else { ?>
<!--                                <label style="font-weight: normal;">-->
<!--                                	<input-->
<!--	                                	type="radio"-->
<!--	                                    value="t"-->
<!--	                                	id="vnatuacaoexclusivaei_--><?php //echo $key['vnid'] ?><!--"-->
<!--	                                    --><?php //echo ($key['vnatuacaoexclusivaei'] == 't') ? 'checked="checked"' : '' ?><!-- -->
<!--	                                    name="vnatuacaoexclusivaei_--><?php //echo $key['vnid'] ?><!--"> Sim </label>-->
<!--								<label style="font-weight: normal;">-->
<!--	                                <input-->
<!--	                                	type="radio"-->
<!--	                                    value="f"-->
<!--	                                	id="vnatuacaoexclusivaei_--><?php //echo $key['vnid'] ?><!--"-->
<!--	                                    --><?php //echo ($key['vnatuacaoexclusivaei'] == 'f') ? 'checked="checked"' : '' ?><!-- -->
<!--	                                    name="vnatuacaoexclusivaei_--><?php //echo $key['vnid'] ?><!--"> Não-->
<!--                                </label>-->
<!--                                --><?php //} ?>
<!--                            </td>-->
<!--                            <td>-->
<!--                                <input-->
<!--                                    type="hidden"-->
<!--                                    name="tenid[--><?php //echo $key['vnid']; ?><!--]"-->
<!--                                    id="tenid_--><?php //echo $key['vnid']?><!--"-->
<!--                                    value="--><?php //echo $key['tenid']?><!--" />-->
<!--                                --><?php
//                                if($key['tenid'] == 7 ){
//                                    if($key['vnstatus'] == 'A'){?>
<!--                                <input-->
<!--                                    type="hidden"-->
<!--                                    name="vnaceito[--><?php //echo $key['vnid']; ?><!--]"-->
<!--                                    id="vnaceito_--><?php //echo $key['vnid']?><!--"-->
<!--                                    value="--><?php //echo $key['snaceito']?><!--" />-->
<!--                                --><?php //  if($key['snaceito'] != 't'){?>
<!--                                    <button-->
<!--                                        class="btn btn-success aceitar"-->
<!--                                        vnid="--><?php //echo $key['vnid'];?><!--" >Aceitar</button>-->
<!--                                --><?php
//                                        }else{?>
<!--                                    <button-->
<!--                                        class="btn btn-success aceitar"-->
<!--                                        vnid="--><?php //echo $key['vnid'];?><!--" >Aceito</button>-->
<!--                                --><?php
//                                        }
//                                    }else{
//                                        if($key['vnaceito'] == 't'){
//                                            echo '<label class="label label-success">Aceito</label>';
//                                        }else{
//                                            echo '<label class="label label-danger">Não aceito</label>';
//                                        }
//                                    }
//                                }?>
<!--                            </td>-->
                        </tr>
                    <?php } ?>
                    </tbody>
                </table>
                <div class="ibox-footer center text-center">
                    <button class="btn btn-success" id="salvar">Prosseguir</button>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="ibox float-e-margins animated modal" id="modal_assinar" tabindex="-1" role="dialog" aria-hidden="true" style="display: none"></div>


<div class="ibox float-e-margins animated modal" id="modaltermodesvinculacao" tabdesex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="formArea" id="formArea" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Termo de Desvinculação</h3>
                </div>
                <div class="ibox-content" id="conteudo_modal"></div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                        <button type="button" id="cancelarArea" data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Fechar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>

    $("document").ready(function () {

    	$('.tabela-listagem').DataTable({
			'bFilter': false,
			'bInfo': false,
			'bLengthChange': false,
			'bSort': false,
			'aoColumnDefs' : [{
				'bSortable' : false,
				'aTargets' : [ 'unsorted' ]
			},{
                'targets': 0,
                'searchable': false,
                'orderable': false,
                'className': 'dt-body-center',
            },{
                "targets": [ 1 ],
                "visible": false,
                "searchable": false
            }],
			'oLanguage' : {
				'sProcessing' : "Processando...",
				'sLengthMenu' : "Mostrar _MENU_ registros",
				'sZeroRecords' : "N&atilde;o foram encontrados resultados",
				'sInfo' : "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
				'sInfoEmpty' : "Mostrando de 0 at&eacute; 0 de 0 registros",
				'sInfoFiltered' : "(filtrado de _MAX_ registros no total)",
				'sInfoPostFix' : ".",
				'sSearch' : "Pesquisar :&nbsp;&nbsp;",
				'sUrl' : "",
				'oPaginate' : {
					'sFirst' : "Primeiro",
					'sPrevious' : "Anterior",
					'sNext' : "Seguinte",
					'sLast' : "&Uacute;ltimo"
				}
			}
		});
        $("#salvar").click(function () {

            var count = 0;
            var rt = 0;
            $.each($('tr[data-acao]'), function () {

                var vnid = $(this).attr('data-acao');

            

                var vndatavinculacao        = $("input[name=vndatavinculacao_" + vnid + "]").val();
                var vncargahorariasemanalo  = $("input[name=vncargahorariasemanalo_" + vnid + "]").val();
//                 var vnatuacaoexclusivaei    = $("input[name=vnatuacaoexclusivaei_" + vnid + "]:checked").val();
                var dntvido                 = $("select[name=dntvido_" + vnid + "]").val();


                if($("input[name=vndatavinculacao_" + vnid + "]").length === 0
                	|| $("input[name=vncargahorariasemanalo_" + vnid + "]").length === 0
                	|| /*vnatuacaoexclusivaei === '' ||*/ $("select[name=dntvido_" + vnid + "]").length === 0 ){
                    count++;
                }

                //var tenid    = $("input[id=tenid_" + vnid + "]").val();
                //var vnaceito = $("[id=vnaceito_" + vnid + "]").val();

//                 if(tenid == 7 && vnaceito == ''){
//                     rt++;
//                 }
            });

            if(rt > 0){
                swal('Atenção', 'Antes de salvar essa tela você deve aceitar ou desvincular todos os pedidos de Responsável Técnico. A desvinculação é feita na aba \'Desvinculação\'.', 'warning');
                return false;
            }else if (count > 0) {
                swal({title: "", text: "Por favor preencha todos os campos solicitados", type: "error"});
            } else {

                $.each($('tr[data-acao]'), function () {
                    var vnid = $(this).attr('data-acao');

                    var vndatavinculacao = $("input[name=vndatavinculacao_" + vnid + "]").val();
                    var vncargahorariasemanalo = $("input[name=vncargahorariasemanalo_" + vnid + "]").val();
                    var vnatuacaoexclusivaei = $("input[name=vnatuacaoexclusivaei_" + vnid + "]:checked").val();
                    var dntvido = $("select[name=dntvido_" + vnid + "]").val();

                    jQuery.ajax({
                        type: 'post',
                        url: 'par3.php?modulo=principal/nutricionista/vinculacaoNutricionista&acao=A',
                        data: 'requisicao=salvarVinculacao&vndatavinculacao=' + vndatavinculacao + '&vncargahorariasemanalo=' + vncargahorariasemanalo + '&vnatuacaoexclusivaei=' + vnatuacaoexclusivaei + '&vnid=' + vnid + '&dntvido=' + dntvido,
                        async: false,
                        success: function (res) {
                            window.location = "par3.php?modulo=principal/nutricionista/cadNutricionista&acao=A"
                        }

                    });
                });
            }

        });

        $('.aceitar').click(function(){

        	var vnid = $(this).attr('vnid');

            var vndatavinculacao       = $('#vndatavinculacao_'+vnid).val();
            var vncargahorariasemanalo = $('#vncargahorariasemanalo_'+vnid).val();
            var dntvido                = $('#dntvido_'+vnid).val();
            var vnatuacaoexclusivaei   = $("input[name=vnatuacaoexclusivaei_" + vnid + "]:checked").val();

            if(vndatavinculacao == ''){
            	swal('Pendência', 'Favor informar a Data de vinculação deste vinculo.', 'error');
            	$('#vndatavinculacao_'+vnid).focus();
            	return false;
            }

            if(vncargahorariasemanalo == ''){
                swal('Pendência', 'Favor informar a Carga Horária Semanal deste vinculo.', 'error');
            	$('#vncargahorariasemanalo_'+vnid).focus();
            	return false;
            }

            if(dntvido == ''){
            	swal('Pendência', 'Favor informar a Tipo deste vinculo.', 'error');
            	return false;
            }

            if(!vnatuacaoexclusivaei){
            	swal('Pendência', 'Favor informar a Atuação deste vinculo.', 'error');
            	$("input[name=vnatuacaoexclusivaei_" + vnid + "]:checked").focus();
                return false;
            }

            dados = {requisicao:'gerarTermoRT',
            	    vnid : vnid,
            	    vndatavinculacao : vndatavinculacao,
            	    vncargahorariasemanalo : vncargahorariasemanalo,
            	    dntvido : dntvido,
            	    vnatuacaoexclusivaei : vnatuacaoexclusivaei};

        	jQuery.ajax({
                type: 'post',
                url: window.location.href,
                data: dados,
                async: false,
                success: function (res) {
                    $('#modal_assinar').html(res);
                    $('#modal_assinar').modal();
                }
            });
        });
    });

    function verTermo(vnid){
        console.log(vnid)
        jQuery.ajax({
            type: 'post',
            url: window.location.href,
            data: 'requisicao=vertermodesvinculado&vnid='+vnid,
            async: false,
            success: function (res) {
                $('#modaltermodesvinculacao').find('#conteudo_modal').html(res);
                $('#modaltermodesvinculacao').modal('show');
            }
        });
    }
    function defineValidacao(val, vnid, unidade) {

        if (val == 6) {
            validacao = 'validar';
            id_campo = "#snid_" + vnid
        }
        else {
            validacao = 'invalidar';
            id_campo = "#snid_n_" + vnid
        }

        if (confirm('Tem certeza que deseja ' + validacao + ' o vínculo com a entidade ' + unidade + '?')) {
            jQuery.ajax({
                type: 'post',
                url: 'par3.php?modulo=principal/nutricionista/vinculacaoNutricionista&acao=A',
                data: 'requisicao=validacaoNutricionista&snid=' + val + '&vnid=' + vnid,
                async: false,
                success: function (res) {

                    if (res == 'sucesso'||res == 'Erro ao enviar Email') {
                        if (val == 6)//Validou
                        {
                            jQuery('#bt_valida_' + vnid).val('Validado');

                            //jQuery('#formulario_validado').show();
                            jQuery("#data_vincula_" + vnid).attr("readonly", false);
                            jQuery("#vnid_" + vnid).attr("readonly", false);

                            jQuery("#carga_horaria_" + vnid).attr("readonly", false);
                            jQuery("#vnatuacaoexclusivaei_n_" + vnid).attr("readonly", false);
                            jQuery("#vnatuacaoexclusivaei_" + vnid).attr("readonly", false);

                            jQuery("#dntvid_" + vnid).attr("readonly", false);

                            if (jQuery("#data_vincula_" + vnid).val() != '') {
                                jQuery("#snaceito_" + vnid).attr("readonly", false);
                            }

                            jQuery('.mensagemOK' + vnid).show();
                        }
                        else// Não validou
                        {
                            //jQuery('#formulario_validado').hide();
                            jQuery('#bt_valida_' + vnid).val('Não Validado');
                            jQuery(".tabs-container input").attr("readonly", true);
                            jQuery('#snid_' + vnid).attr("readonly", true);
                            jQuery('#snid_n_' + vnid).attr("readonly", true);
                            jQuery('.mensagemOK' + vnid).show();
                        }
                    }
                    else {
                        alert(res);
                        jQuery(id_campo).removeAttr("checked");
                    }

                }
            });
        }
        else {

            jQuery(id_campo).removeAttr("checked");

        }

    }
</script>