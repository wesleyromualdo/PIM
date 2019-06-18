<?php
require_once APPRAIZ . 'par3/classes/model/adesao_programa/EscolasMaisAlfabetizacao.class.inc';
require_once APPRAIZ . 'par3/classes/model/adesao_programa/Adesaoalfabetizacao.class.inc';

$objOrientacao = new Par3_Model_Orientacoesprograma();
$objExcecao = new Par3_Model_Excecaoescolasprograma();
$objEscolas = new Par3_Model_EscolasMaisAlfabetizacao();
$objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();

$dadosUnidade = $objIstrumentoUnidade->getDadosUnidade($_REQUEST['inuid']);

$itrid = $dadosUnidade['itrid'];
$estuf = $dadosUnidade['estuf'];
$local = $dadosUnidade['inudescricao'];

$arrayParams['itrid'] = $itrid;
$arrayParams['estuf'] = $estuf;
$arrayParams['inuid'] = $_REQUEST['inuid'];
$arrayParams['adpid'] = $_REQUEST['adpid'];
$arrayParams['adpano_ciclo'] = $_SESSION['par3']['adpano_ciclo'];
$arrayParams['prgid'] = $_REQUEST['prgid'];
$arrayParams['inudescricao'] = $local;
$arrayParams['muncod'] = $dadosUnidade['muncod'];
$arrayParams['aeacargahoraria'] = $_REQUEST['carga_horaria'];

$arrayParams['filtro_extra'] = <<<SQL
emacodinep NOT IN (
   SELECT escm.emacodinep
  FROM par3.escolasmaisalfabetizacao escm
  LEFT JOIN par3.adesaoescolamaisalfabetizacao ad ON ad.codinep =  escm.emacodinep
  WHERE ad.codinep  IS NOT NULL
  AND (emanaoapta IS FALSE OR emanaoapta  IS NULL)
 )
SQL;


$arrayParams['aeacargahoraria'] = '10';
$arrayParams['carga_horaria'] = '10';
$listaEscolasG1 = $objEscolas->getListaEscolasElegiveis($arrayParams);

$arrayParams['aeacargahoraria'] = '5';
$arrayParams['carga_horaria'] = '5';
$listaEscolasG2 = $objEscolas->getListaEscolasElegiveis($arrayParams);

$cabecalho1 = $listaEscolasG1['cabecalho'];
$cabecalho2 = $listaEscolasG2['cabecalho'];

$perfis = pegaPerfilGeral($_SESSION['usucpf']);
$prgid = $_REQUEST['prgid'];

$modelProadesao = new Par3_Model_Proadesao();
$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid, $_SESSION['par3']['adpano_ciclo']);

$bloqueiaSave = false;

if ((strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$_SESSION['par3']['adpano_ciclo']] . ' 23:59:00') < strtotime(date('Y-m-d G:i:s')))) {
    $bloqueiaSave = true;
    $disabledCarga = 'disabled ';
}


$sql = <<<SQL
          SELECT * FROM par3.excecaoescolasprograma
          WHERE prgid = {$prgid} AND inuid = {$arrayParams['inuid']};
SQL;

global $db;
$dadosEscolasExcecao = $db->pegaLinha($sql);

?>
    <script>
        function negaExcecao(eepid) {
            $('#justifica_negacao_' + eepid).show();
            $('#botao_aceita_' + eepid).hide();
        }

        function confirmaExcecao(eepid) {

            if ($('#justifica_negacao_textarea_' + eepid).val() == '') {
                alert('Campo Justificativa obrigatório');
                return false;
            }
            if (confirm('Confirma a não aceitação da solicitação Nº' + eepid + '?')) {

                var param = new Array();

                param.push(
                    {name: 'requisicao', value: 'aceita_solicitacao_excecao'},
                    {name: 'decisao', value: 'N'},
                    {name: 'justificativa', value: $('#justifica_negacao_textarea_' + eepid).val()},
                    {name: 'eepid', value: eepid}
                );

                $.ajax({
                    type: "POST",
                    dataType: "json",
                    url: window.location.href,
                    data: param,
                    success: function (data) {

                        if (data[0].status == "success") {
                            $('#botao_aceita_' + eepid).hide();
                            $('#botao_nega_' + eepid).hide();
                            $('#justifica_negacao_' + eepid).hide();
                            $('#sucesso_negado_' + eepid).show();
                            $('#just_' + eepid).html('<b>Justificativa:</b>' + $('#justifica_negacao_textarea_' + eepid).val())
                        }
                        else {
                            alert('Erro ao validar solicitação');
                        }
                    },
                    error: function (data) {

                    }
                });


            }
        }

        function cancelaExcecao(eepid) {
            $('#justifica_negacao_' + eepid).show();
            $('#botao_aceita_' + eepid).show();
            $('#justifica_negacao_' + eepid).hide();
        }

        function aceitaExcecao(eepid) {
            if (confirm('Confirma que está ciente da solicitação Nº' + eepid + '?')) {
                var param = new Array();

                param.push(
                    {name: 'requisicao', value: 'aceita_solicitacao_excecao'},
                    {name: 'decisao', value: 'S'},
                    {name: 'justificativa', value: ''},
                    {name: 'eepid', value: eepid}
                );

                $.ajax({
                    type: "POST",
                    dataType: "json",
                    url: window.location.href,
                    data: param,
                    success: function (data) {

                        if (data[0].status == "success") {
                            $('#botao_aceita_' + eepid).hide();
                            $('#botao_nega_' + eepid).hide();
                            $('#sucesso_aceita_' + eepid).show();
                        }
                        else {
                            alert('Erro ao validar solicitação');
                        }
                    },
                    error: function (data) {

                    }
                });


            }
        }

        function validaEnvio() {
            var submete = true;
            var erroTxt = '';

            if ($('#formulario_escolas_excecao').find('[name*="escola_checada"]:checked').length < 1) {
                erroTxt += '* Escolha pelo menos uma escola.<br>';
                submete = false;
            }
            if ($('#justificativa').val() == '') {
                erroTxt += '* Campo \'Justificativa\' é obrigatório.';
                submete = false;
            }
            if (submete == true) {
                document.formulario_escolas_excecao.submit();
            } else {
                alert(erroTxt);
            }
        }

        function removeLinha(input) {
            if ($('#tabelaEscolas tr').length > 3) {
                var tr = jQuery(input).closest('tr');
                tr.remove();
                return false;
            }
            else {
                return false;
            }
        }

        function retornaInfoEscola(input) {
            if ($(input).val().length == 8) {
                var inuid = $("#inuid").val();
                var adpid = $("#adpid").val();
                var cod_inep = $(input).val()

                var param = new Array();

                param.push(
                    {name: 'requisicao', value: 'retorna_dados_escola'},
                    {name: 'inuid', value: inuid},
                    {name: 'adpid', value: adpid},
                    {name: 'cod_inep', value: cod_inep}
                );

                $.ajax({
                    type: "POST",
                    dataType: "json",
                    url: window.location.href,
                    data: param,
                    success: function (data) {
                        if (data[0].status == "success") {
                            if (data[0].grupo != 0) {
                                $(input).parent().parent().find("input[tag=nome_escola_atual]").val(data[0].nome);
                                $(input).parent().parent().find("input[tag=grupo]").val(data[0].grupo);
                                $(input).parent().parent().find("p[tag=label_grupo]").html(data[0].grupo);
                                $(input).parent().parent().find("p[tag=label_escola]").html(data[0].nome)

                                if (data[0].resposta_carga_horaria == 'S') {
                                    $(input).parent().parent().find("select[tag=carga_horaria_atual]").removeAttr('disabled');
                                    $(input).parent().parent().find("input[tag=arrPossuiCargaHoraria]").val('s');

                                }
                                else {
                                    $(input).parent().parent().find("select[tag=carga_horaria_atual]").attr('disabled', 'disabled');
                                    $(input).parent().parent().find("input[tag=arrPossuiCargaHoraria]").val('n');
                                }
                            }
                            else {
                                alert('Escola não apta');
                            }


                        }
                        else {
                            $(input).val("");
                            $(input).parent().parent().find("input[tag=nome_escola_atual]").val("");
                            $(input).parent().parent().find("p[tag=label_escola]").html("")
                            $(input).parent().parent().find("select[tag=carga_horaria_atual]").attr('disabled', 'disabled');
                            $(input).parent().parent().find("select[tag=carga_horaria_atual]").val('');

                            alert('Código INEP não encontrado');
                        }
                    },
                    error: function (data) {

                    }
                });

            }
            else {
                $(input).val("");
                $(input).parent().parent().find("input[tag=nome_escola_atual]").val("");
                $(input).parent().parent().find("p[tag=label_escola]").html("")
                $(input).parent().parent().find("select[tag=carga_horaria_atual]").attr('disabled', 'disabled');
                $(input).parent().parent().find("select[tag=carga_horaria_atual]").val('');
                alert('O código INEP deve ser composto por 8 dígitos');
            }
        }

        $(document).ready(function () {

            $('.add').click(function () {

                var $campos = $('#tbodyTabela');
                console.log($campos);

                $tr = $campos.find('tr:first').clone();

                $tr.find("input").val("");
                $tr.find("select").attr('disabled', 'disabled');
                ;
                $tr.find("select").val("");
                $tr.find("hidden").val("");
                $tr.find("label").html("");
                $tr.find("p").html("");
                $campos.append($tr);
                return false;
            });

            $('.btn-xls').hide();
            $('.btn-query').hide();

            if ($("#tipoPesquisa2").val() == 'lista') {
                window.location.hash = '#resultado';
            }

            $('.ordenavel').click(function () {
                var campo = $(this).attr('data-campo-ordenacao');
                $('#ordenacao').val(campo);
                $('#filtrar_pesquisa').click()

            });

        });
    </script>

    <div class="ibox-content">
        <div class="tabs-container">
            <ul class="nav nav-tabs">
                <li id="li1" class="active"><a data-toggle="tab" href="#pop-1" aria-expanded="true">Orientações</a></li>
                <li id="li2" class=""><a data-toggle="tab" href="#pop-2" aria-expanded="false">Escolas</a>
                    <?php
                    if (
                    in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) ||
                    in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis)
                    ):
                    ?>
                <li id="li3" class=""><a data-toggle="tab" href="#pop-3" aria-expanded="false">Validar Solicitações</a>
                <li id="li4" class=""><a data-toggle="tab" href="#pop-4" aria-expanded="false">Solicitações Validadas</a>
                    <?php endif; ?>
            </ul>

            <div class="tab-content">

                <div id="pop-1" class="tab-pane active">
                    <div class="panel-body">
                        <?php $objOrientacao->getOrientacaoByID(ORIENTACOES_MAISALFABETIZACAO_EXCESAO_ESCOLAS, "Escolas"); ?>
                    </div>
                </div>

                <div id="pop-2" class="tab-pane">
                    <div class="panel-body">
                        <?php require_once APPRAIZ . "par3/modulos/principal/adesao/termos/mais_alfabetizacao/2018/_solicitacao_inclusao_escola/_escolas.php" ?>
                    </div>
                </div>

                <div id="pop-3" class="tab-pane">
                    <div class="panel-body">
                        <?php require_once APPRAIZ . "par3/modulos/principal/adesao/termos/mais_alfabetizacao/2018/_solicitacao_inclusao_escola/_validar_solitacoes.php" ?>
                    </div>
                </div>
                <div id="pop-4" class="tab-pane">
                    <div class="panel-body">
                        <?php require_once APPRAIZ . "par3/modulos/principal/adesao/termos/mais_alfabetizacao/2018/_solicitacao_inclusao_escola/solicitacoes_validas.php" ?>
                    </div>
                </div>
            </div>
        </div>
    </div>

<?php die(); ?>