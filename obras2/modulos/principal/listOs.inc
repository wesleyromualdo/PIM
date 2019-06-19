<?php

require APPRAIZ . 'obras2/includes/principal/listaEmpreendimentoEmpresa/ctrl.php';


if ($_POST['requisicaoAjax']) {
    $_POST['requisicaoAjax']();
    die;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

criaAbaOS();
monta_titulo_obras('Lista de OS', '');
?>
<style>
    .link {
        cursor: pointer
    }
</style>


<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

<?php //ver($_POST); ?>
<form name="formulario_acao" id="formulario_acao" method="post" action="">
    <input type="hidden" name="sosid" id="sosid" value=""/>
    <input type="hidden" name="op" id="op" value=""/>
    <input type="hidden" name="estuf" id="estuf" value="SP"/>
    <input type="hidden" name="sgrid" id="sgrid" value="12"/>
    <div class="form-filters">
        <div class="row">
            <div class="col-lg-6">
                
                <div class="row">
                    <div class="col-lg-4">
                    Empresa
                    </div>
                    <div class="col-lg-8" id="tdComboEmpresa">
                         
                        <?php
                        $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
                        $empresas = $supervisao_Grupo_Empresa->listaComboEntidades();
                        echo $db->monta_combo("entid", $empresas, 'S', 'Selecione...', 'carregaDependenciaEmpresa', '', '', 200, 'N', 'entid');
                        ?>
                    </div> 
                </div>
                <div class="row">
                    <div class="col-lg-4">
                        Nota de Empenho
                    </div>

                    <div class="col-lg-8" id="tdEmpenho">
                        <?php
                        if (!empty($semid)) {
                            $empenho = new Supervisao_Empenho();
                            $dados = $empenho->listaComboPorEmpresa(array('sgrid' => $_POST['sgrid'], 'sgeid' => $_POST['sgeid']));

                            $db->monta_combo("semid", $dados, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'semid');
                        } else {
                            echo "Selecione a empresa.";
                        }
                        ?>
                    </div>
                </div>

                <div class="row">
                    <div class="col-lg-4">
                        OS Nº
                    </div>
                    <div class="col-lg-8"> 
                        <?= campo_texto('sosnum', 'N', "S", '', 12, 12, '[#]', '', 'right', '', 0, 'id="sosnum" ', ''); ?>
                    </div>
                </div>
                <div class="row">
                    <div class="col-lg-4">
                        Situação da OS
                    </div>
                    <div class="col-lg-8">
                        <?php criaComboWorkflow(TPDID_OS) ?>
                    </div>
                </div>
                <div class="row">
                    <div class="col-lg-4">
                      Município
                    </div>
                    <div class="col-lg-8">
                    <?php
                    $municipio2 = new Municipio();
                                $dado      = $municipio2->listaCombo( array('estuf' => "SP") );
                                $habMun    = 'S';
                                echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'N','muncod');
                    ?>
                    </div>
                </div> 
                <div class="row">
                    <div class="col-lg-4">
                        Possui supervisão atrasada?
                    </div>
                    <div class="col-lg-8">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">
                                <input type='radio' name='rdb_atrasada'
                                       id='rdb_atrasada_s' <?php echo $_POST['rdb_atrasada'] == "sim" ? "checked='checked'" : "" ?>
                                       value='sim'/>Sim
                            </label>
                            <label class="btn btn-default">
                                <input type='radio' name='rdb_atrasada'
                                       id='rdb_atrasada_n' <?php echo $_POST['rdb_atrasada'] == "nao" ? "checked='checked'" : "" ?>
                                       value='nao'/>Não
                            </label>
                            <label class="btn btn-default <?= empty($_POST['rdb_atrasada'])? 'active':'' ?> ">
                                <input type='radio' name='rdb_atrasada'
                                       id='rdb_atrasada_t' <?php echo $_POST['rdb_atrasada'] == "todas" ? "checked='checked'" : "" ?>
                                       value='todas'/>Todas
                            </label>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-lg-4">
                        Supervisão homologada com atraso
                    </div>
                    <div class="col-lg-8">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">
                                    <input type='radio' name='h_atrasada' id='h_atrasada_s' <?php echo $_POST['h_atrasada'] == "sim" ? "checked='checked'" : "" ?> value='sim'/>Sim
                            </label>
                            <label class="btn btn-default">
                                <input type='radio' name='h_atrasada'
                                       id='h_atrasada_n' <?php echo $_POST['h_atrasada'] == "nao" ? "checked='checked'" : "" ?>
                                       value='nao'/>Não
                            </label>
                            <label class="btn btn-default <?= (empty($_POST['h_atrasada']) || $_POST['h_atrasada'] == 'todas') ? 'active' : '' ?>">
                                <input type='radio' name='h_atrasada'
                                       id='h_atrasada_t' <?php echo $_POST['h_atrasada'] == "todas" ? "checked='checked'" : "" ?>
                                        value='todas'/>Todas
                            </label>
                        </div>
                    </div>
                </div>


            </div>

            <div class="col-lg-6">
            
                <div class="row">
                    <div class="col-lg-4">
                        Emergencial
                    </div>
                    <div class="col-lg-8">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">
                                <input type="radio" <?php echo($sosemergencial == 't' ? 'checked="checked"' : ''); ?>
                                       name="sosemergencial" id='sosemergencial_s' value="t"/>Sim
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" <?php echo($sosemergencial == 'f' ? 'checked="checked"' : ''); ?>
                                       name="sosemergencial" id='sosemergencial_n' value="f"/>Não
                            </label>
                            <label class="btn btn-default <?= (empty($sosemergencial) || $sosemergencial == 'todas') ? 'active' : '' ?>">
                                <input type="radio" <?php echo($sosemergencial == 'todas' ? 'checked="checked"' : ''); ?>
                                       name="sosemergencial" id='sosemergencial_t' value="todas"/>Todas
                            </label>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-lg-4"> 
                        OS prorrogada
                    </div>
                    <div class="col-lg-8">            
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">
                                <input type="radio" <?php echo($sosprorrogada == 't' ? 'checked="checked"' : ''); ?>name="sosprorrogada"
                               id="sosprorrogada_s" value="t"/>Sim
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" <?php echo($sosprorrogada == 'f' ? 'checked="checked"' : ''); ?>name="sosprorrogada"
                               id="sosprorrogada_n" value="f"/>Não
                            </label>
                            <label class="btn btn-default <?= (empty($sosprorrogada) || $sosprorrogada == 'todas') ? 'active' : '' ?>">
                                <input type="radio" <?php echo($sosprorrogada == 'todas' ? 'checked="checked"' : ''); ?>
                               name="sosprorrogada" id="sosprorrogada_t" value="todas"/>Todas
                            </label>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-lg-4"> Início da Execução:
                    </div>
                    <div class="col-lg-8">  
                        de: <input type="text" id="inicio_exec_de" name="inicio_exec_de" value="<?php echo $inicio_exec_de; ?>"
                                   size="15" maxlength="10" class="normal calendar">
                        &nbsp;
                        até: <input type="text" id="inicio_exec_ate" name="inicio_exec_ate"
                                    value="<?php echo $inicio_exec_ate; ?>" size="15" maxlength="10" class="normal calendar">
                    </div>
                </div>

                <div class="row">
                    <div class="col-lg-4">
                        Término da Execução:
                    </div>
                    <div class="col-lg-8"> 
                        de: <input type="text" id="termino_exec_de" name="termino_exec_de"
                                   value="<?php echo $termino_exec_de; ?>" size="15" maxlength="10" class="normal calendar">
                        &nbsp;
                        até: <input type="text" id="termino_exec_ate" name="termino_exec_ate"
                                    value="<?php echo $termino_exec_ate; ?>" size="15" maxlength="10" class="normal calendar">
                    </div>
                </div>
                <div class="row">
                    <div class="col-lg-4">
                        Data de Emissão:
                    </div>
                    <div class="col-lg-8"> 
                        de: <input type="text" id="dt_emissao_de" name="dt_emissao_de" value="<?php echo $dt_emissao_de; ?>"
                                   size="15" maxlength="10" class="normal date">
                        &nbsp;
                        até: <input type="text" id="dt_emissao_ate" name="dt_emissao_ate" value="<?php echo $dt_emissao_ate; ?>"
                                    size="15" maxlength="10" class="normal date">
                    </div>
                </div>

                <div class="row">
                    <div class="col-lg-4">Tipo de Supervisão
                    </div>
                    <div class="col-lg-8">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">                
                                <input type="radio" name="sosterreno" id=""
                                   value="f" <?= ($sosterreno == 'f') ? 'checked="checked"' : '' ?> > Obra
                            </label>
                            <label class="btn btn-default">                
                                <input type="radio" name="sosterreno" id=""
                                   value="t" <?= ($sosterreno == 't') ? 'checked="checked"' : '' ?> > Implantação de Obra
                            </label>
                            <label class="btn btn-default <?= (empty($sosterreno)) ? 'active' : '' ?>">                
                                <input type="radio" name="sosterreno" id=""
                                   value="" <?= (empty($sosterreno)) ? 'checked="checked"' : '' ?> > Todas
                            </label>
                        </div>
                    </div>
                </div>

            </div>
            
        </div>
    </div>


    <div class="row form-filters text-center">
        <div class="col">
            <?php
            if (possui_perfil(PFLCOD_SUPER_USUARIO)):
                ?>
                <div class="novaObra btn btn-info" onclick="location.href='?modulo=principal/cadOs&acao=A'">
                    <span class="glyphicon glyphicon-plus"></span> Nova Orderm de Serviço
                </div>
            <?php endif; ?>
            <button type="button" name="pesquisar" class="btn btn-success pesquisar" onclick="validaFiltro(this);" id="pesquisar" >
                <span class="glyphicon glyphicon-search"></span> Pesquisar
            </button>

            <button type="button" name="btn_limpar" class="btn btn-primary limpar" onclick="limparOS()" id="limpar" >
                <span class="glyphicon glyphicon-stop"></span> Limpar
            </button>


            <button type="button" name="btn_exportar" class="btn btn-primary btnEexcel" value="Exportar" onclick="validaFiltro(this);" id="excel" >
                <span class="glyphicon glyphicon-download-alt"></span> Exportar
            </button>
 
        </div>
    </div>
 
</form>
<script>
$1_11(document).ready(function () {
    $1_11('select[name="sgrid"]').chosen({width:'100%'});
    $1_11('select[name="entid"]').chosen({width:'100%'});
    $1_11('select[name="semid"]').chosen({width:'100%'});
    $1_11('select[name="esdid"]').chosen({width:'100%'});
    $1_11('select[name="estuf"]').chosen({width:'100%'});
    $1_11('select[name="muncod"]').chosen({width:'100%'});
});
</script>
<?php

if (isset($_POST['op']) && !empty($_POST['op'])) {

    if ($_POST['op'] == 'apagar') {
        ver('here', d);
        $os = new Supervisao_Os($_POST['sosid']);
        $os->sosstatus = 'I';
        $os->salvar();

        $db->commit();
        die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/listOs&acao=A';
			 </script>");
    }

    $arrFiltros = array(
        $sgrid = $_POST['sgrid_disable'],
        $entid = $_POST['entid'],
        $sosnum = $_POST['sosnum'],
        $esdid = $_POST['esdid'],
        //$listobraestuf = $_POST['listobraestuf'],
        $rdb_atrasada_s = $_POST['rdb_atrasada_s'],
        $rdb_atrasada_n = $_POST['rdb_atrasada_n'],
        $rdb_atrasada_t = $_POST['rdb_atrasada_t'],
        $h_atrasada_s = $_POST['h_atrasada_s'],
        $h_atrasada_n = $_POST['h_atrasada_n'],
        $h_atrasada_t = $_POST['h_atrasada_t'],
        $sosemergencial_s = $_POST['sosemergencial_s'],
        $sosemergencial_n = $_POST['sosemergencial_n'],
        $sosemergencial_t = $_POST['sosemergencial_t'],
        $sosprorrogada_s = $_POST['sosprorrogada_s'],
        $sosprorrogada_n = $_POST['sosprorrogada_n'],
        $sosprorrogada_t = $_POST['sosprorrogada_t'],
        $inicio_exec_de = $_POST['inicio_exec_de'],
        $inicio_exec_ate = $_POST['inicio_exec_ate'],
        $termino_exec_de = $_POST['termino_exec_de'],
        $termino_exec_ate = $_POST['termino_exec_ate']
    );

    $ocorrenciasFiltro = 0;
    $filtrosQuatidade = count($arrFiltros);

    for ($i = 0; $i < $filtrosQuatidade; $i++) {

        if ($arrFiltros[$i] != '') {
            $ocorrenciasFiltro++;
        }
    }

    if ($ocorrenciasFiltro > 0) {
        if ($_POST['op'] == 'pesquisar') {
            $_POST['sgrid'] = $_POST['sgrid_disable'] ? $_POST['sgrid_disable'] : $_POST['sgrid'];
            $_POST['sgeid'] = $_POST['sgeid_disable'] ? $_POST['sgeid_disable'] : $_POST['sgeid'];
            extract($_POST);
            $arrWhere = $_POST;
            $os = new Supervisao_Os();

            $os->listaOS($arrWhere);
        }

        if ($_POST['op'] == 'excel') {
            $_POST['sgrid'] = $_POST['sgrid_disable'] ? $_POST['sgrid_disable'] : $_POST['sgrid'];
            $_POST['sgeid'] = $_POST['sgeid_disable'] ? $_POST['sgeid_disable'] : $_POST['sgeid'];
            extract($_POST);
            $arrWhere = $_POST;
            $os = new Supervisao_Os();
            $arrWhere = $arrWhere ? $arrWhere : array();
            $os->listaOS($arrWhere, true);
        }

    } else {
        echo '<script>alert("Para realizar a pesquisa deve ser preenchido ao menos um filtro");</script>';
    }
}

?>
<script type="text/javascript">

    function imprimirOs(sosid) {
        return windowOpen('?modulo=principal/popupImpressaoOS&acao=A&sosid=' + sosid, 'blank',
            'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function editarOs(sosid) {
        location.href = '?modulo=principal/cadOs&acao=E&sosid=' + sosid;
    }

    function excluirOs(sosid) {
        if (confirm("Deseja realmente excluir essa OS?")) {
            $('#sosid').val(sosid);
            $('#op').val('apagar');
            $('#formulario_acao').submit();
        }
    }

    //function carregaDependenciaEmpresa( sgeid ){
    //	var sgrid = $('#sgrid').val();
    //	if ( sgeid ){
    //		$.post("?modulo=principal/cadOs&acao=A", {"sgeid":sgeid, "ajax":"carregaEmpenho"}, function(data){
    //			$('#tdEmpenho').html( data );
    //		});
    //
    //		var orgid = $('#orgid').val();
    //		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "sgeid":sgeid, "ajax":"carregaEmpresaAndListaObra", "not(comboGrupoEmpresa)":true}, function(data){
    //			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
    //			$('#divListaObra').html( listaObra );
    //		});
    //	}else{
    //		$('#tdEmpenho').html('Selecione a empresa.');
    //		if ( sgrid ){
    //			carregaDependenciaGrupo( sgrid );
    //		}
    //	}
    //}
    function carregaDependenciaEmpresa(entid) {
        var sgrid = $('#sgrid').val();
        if (entid) {
            $.post("?modulo=principal/cadOs&acao=A", {
                "entid": entid,
                'sgrid': sgrid,
                "ajax": "carregaEmpenhoPorEmpresa"
            }, function (data) {
                $('#tdEmpenho').html(data);
            });

            var orgid = $('#orgid').val();
            $.post("?modulo=principal/cadOs&acao=A", {
                "sgrid": sgrid,
                "sgeid": sgeid,
                "ajax": "carregaEmpresaAndListaObra",
                "not(comboGrupoEmpresa)": true
            }, function (data) {
                var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
                $('#divListaObra').html(listaObra);
            });
        } else {
            $('#tdEmpenho').html('Selecione a empresa.');
            if (sgrid) {
                carregaDependenciaGrupo(sgrid);
            }
        }
    }

    function carregaDependenciaGrupo(sgrid) {
        if (sgrid) {
            var orgid = $('#orgid').val();
            $.post("?modulo=principal/cadOs&acao=A", {
                "sgrid": sgrid,
                "ajax": "carregaEmpresaAndListaObra",
                "orgid": orgid
            }, function (data) {
//			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
//			$('#tdComboEmpresa').html( comboEmpresa );

                var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
                $('#divListaObra').html(listaObra);

            });
        } else {
            $('#tdComboEmpresa').html('Selecione o grupo.');
        }
    }



    function limparOS() {
        window.location.href = window.location;
    }

    function abrePopupProrrogacao(sosid) {
        popupProrrogacao = window.open('obras2.php?modulo=principal/cadOS_prorrogacao&acao=E&sosid=' + sosid, 'Prorrogacao', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }
</script>
<script type="text/javascript">
    function validaFiltro(botao) {

        var sgrid = document.getElementById('sgrid').value;
        var entid = document.getElementById('entid').value;
        var sosnum = document.getElementById('sosnum').value;
        var esdid = document.getElementById('esdid').value;
        //var listobraestuf = document.getElementById('listobraestuf').value;
        var rdb_atrasada_s = document.getElementById('rdb_atrasada_s').checked;
        var rdb_atrasada_n = document.getElementById('rdb_atrasada_n').checked;
        var rdb_atrasada_t = document.getElementById('rdb_atrasada_t').checked;
        var h_atrasada_s = document.getElementById('h_atrasada_s').checked;
        var h_atrasada_n = document.getElementById('h_atrasada_n').checked;
        var h_atrasada_t = document.getElementById('h_atrasada_t').checked;
        var sosemergencial_s = document.getElementById('sosemergencial_s').checked;
        var sosemergencial_n = document.getElementById('sosemergencial_n').checked;
        var sosemergencial_t = document.getElementById('sosemergencial_t').checked;
        var sosprorrogada_s = document.getElementById('sosprorrogada_s').checked;
        var sosprorrogada_n = document.getElementById('sosprorrogada_n').checked;
        var sosprorrogada_t = document.getElementById('sosprorrogada_t').checked;
        var inicio_exec_de = document.getElementById('inicio_exec_de').value;
        var inicio_exec_ate = document.getElementById('inicio_exec_ate').value;
        var termino_exec_de = document.getElementById('termino_exec_de').value;
        var termino_exec_ate = document.getElementById('termino_exec_ate').value;
        var botao = botao.id;
        var filtros =
            [sgrid,
                entid,
                sosnum,
                esdid,
                //listobraestuf,
                rdb_atrasada_s,
                rdb_atrasada_n,
                rdb_atrasada_t,
                h_atrasada_s,
                h_atrasada_n,
                h_atrasada_t,
                sosemergencial_s,
                sosemergencial_n,
                sosemergencial_t,
                sosprorrogada_s,
                sosprorrogada_n,
                sosprorrogada_t,
                inicio_exec_de,
                inicio_exec_ate,
                termino_exec_de,
                termino_exec_ate];

        var filtrosQuatidade = filtros.length;

        var ocorrenciasFiltro = 0;
        for (i = 0; i < filtrosQuatidade; i++) {

            if (filtros[i] != '' || filtros[i] != false) {
                ocorrenciasFiltro++;
            }
        }

        if (ocorrenciasFiltro > 0) {

            if (botao == 'pesquisar') {
                document.getElementById('op').value = 'pesquisar';
                document.formulario_acao.submit();
            } else if (botao == 'excel') {
                document.getElementById('op').value = 'excel';
                document.formulario_acao.submit();
            }

        } else {
            alert('Para realizar a pesquisa deve ser preenchido ao menos um filtro.');
            return false;
        }
    }

    $(function () {

        jQuery(function ($) {
            $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro', 'Fevereiro', 'Mar&ccedil;o', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
                monthNamesShort: ['Jan', 'Fev', 'Mar', 'Abr', 'Mai', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'],
                dayNames: ['Domingo', 'Segunda-feira', 'Ter&ccedil;a-feira', 'Quarta-feira', 'Quinta-feira', 'Sexta-feira', 'S&aacute;bado'],
                dayNamesShort: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
                dayNamesMin: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''
            };
            $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
        });


        jQuery('.calendar').mask('99/99/9999');
        jQuery('.calendar').datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
    });
</script>
