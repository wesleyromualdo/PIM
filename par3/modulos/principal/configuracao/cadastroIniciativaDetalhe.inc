<?php
/**
 * Sistema Par3
 *
 * @author Philipe Allan Almeida <philipeph3@gmail.com>.
 * Date: 23/06/2017
 *
 */

//instanciamento da classe Par3_Controller_Iniciativa
$objControllerIniciativa = new Par3_Controller_Iniciativa();

$mIni = new Par3_Model_Iniciativa($_GET['iniid']);
$iniciativa = $mIni->pegarIniciativa(array('iniid'=>$_GET['iniid']));
$arrDimensao['itrid'] = $mIni->iniesfera == 'M'? '2':'';

if($mIni->iniesfera == 'E'){
    $arrDimensao['itrid'] = '1';
    $iiestuf   = new Par3_Model_IniciativaIniciativasEstados();
    $rsIiestuf = $iiestuf->carregar($iiestuf->recuperarIniciativaEstado(array('iniid' => $mIni->iniid)));
    
    if($iniciativa[0]['initodos_estuf'] == 'S'){
    	$arrDimensao['itrid'] = '1,3';
    } else {
	    foreach($rsIiestuf as $iiestuf){
	        if($iiestuf['estuf'] == 'DF'){
	            $arrDimensao['itrid'] = '1,3';break;
	        }
	    }    	
    }
}

//instanciamento da classe Par3_Model_EnsinoNivel
$objEnsinoNivel = new Par3_Model_EnsinoNivel();

//instanciamento da classe Par3_Model_IniciativaDesdobramentos
$objIniciativaDesdobramentos = new Par3_Model_IniciativaDesdobramentos();

//instanciamento da classe Par3_Model_Dimensao
$objDimensao = new Par3_Model_Dimensao();

//Verifica se há requisições para retorno de dados Json
if (isset($_GET['opcao']) && isset($_GET['opcao']) && $_GET['metodo'] == 'json') {
    exit($objControllerIniciativa->controlarRequisicaoJson($_GET['opcao']));
}

//Controller responsável pela Iniciativa de Indicadores
$controllerIniciativaIndicadores = new Par3_Controller_IniciativaIndicadores();

//Controller responsável pela Iniciativa de Modalidades
$controllerIniciativaModalidades = new Par3_Controller_IniciativaModalidades();

//Realiza o controle das requisições de modalidade
if (isset($_POST['form']) && $_POST['form'] === "iniciativa-modalidades") {

    switch ($_POST['acao']) {
        case 'salvar':
            $controllerIniciativaModalidades->salvar($_POST);
            break;
        case 'inativar':
            $id = $_POST['imoid'];
            $controllerIniciativaModalidades->inativar($id);
            break;
        case 'validar':
            $controllerIniciativaModalidades->validar($_POST);
            //print_r($controllerIniciativaModalidades->validar($_POST)); exit();
            break;
    }

}

//Realiza o controle das requisições de indicadores
if (isset($_POST['form']) && $_POST['form'] === "iniciativa-indicadores") {

    switch ($_POST['acao']) {
        case 'salvar':
            $controllerIniciativaIndicadores->salvar($_POST);
            break;
        case 'inativar':
            $id = $_POST['idiid'];
            $controllerIniciativaIndicadores->inativar($id);
            break;
        case 'validar':
            $controllerIniciativaIndicadores->validar($_POST);
            break;
    }
}

//Verifica se já possui uma Iniciativa
if (!isset($_GET['iniid'])) {
    $url = "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativas";
    simec_redirecionar($url, 'error', "É necessário incluir a Iniciativa.");
}

?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Iniciativa - Detalhe</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-iniciativa-modalidades" id="form-iniciativa-modalidades" method="post"
                  class="form-horizontal">

                <input type="hidden" name="form" value="iniciativa-modalidades">
                <input type="hidden" id="form-modalidades-acao" name="acao" value="salvar">
                <input type="hidden" id="form-modalidades-imoid" name="imoid" value="">
                <input type="hidden" name="iniid" value="<?= $_GET['iniid'] ?>">

                <!-- Nível: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Nível:
                            </label>
                            <div class="col-sm-6">
                                <select name="nivid" id="nivid" class="chosen-select" data-placeholder="Selecione"
                                        required>
                                    <option></option>
                                    <?php
                                    $niveis = $objEnsinoNivel->listaEnsinoNivel();
                                    foreach ($niveis as $nivel) {
                                        ?>
                                        <option value="<?= $nivel['codigo'] ?>"><?= $nivel['descricao'] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                <!--  Etapa: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Etapa:
                            </label>
                            <div class="col-sm-6">
                                <select name="etaid" id="etaid" class="chosen-select" data-placeholder="Selecione"
                                        required>
                                    <option>Selecione Nível</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Modalidade: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Modalidade:
                            </label>
                            <div class="col-sm-6">
                                <select name="modid" id="modid" class="chosen-select" data-placeholder="Selecione"
                                        required>
                                    <option>Selecione Etapa</option>
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Desdobramento: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Desdobramento:
                            </label>
                            <div class="col-sm-6">
                                <select name="desid" id="desid" class="chosen-select" data-placeholder="Selecione"
                                        required>
                                    <option></option>
                                    <?php
                                    $desdobramentos = $objIniciativaDesdobramentos->listaDesdobramentos();
                                    if ($desdobramentos) {
                                        foreach ($desdobramentos as $desdobramento) {
                                            ?>
                                            <option value="<?= $desdobramento['codigo'] ?>"><?= $desdobramento['descricao'] ?></option>
                                        <?php }
                                    } ?>
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-sm-12 text-center">
                        <button type="button" id="salvar-iniciativa-modalidades"
                                class="btn btn-success">
                            <i class="fa fa-plus"></i> Adicionar
                        </button>
                    </div>
                </div>
                <hr>
                <div class="row">
                    <div class="col-sm-12 text-center">
                        <?php
                        $controllerIniciativaModalidades->listar($_GET['iniid']);
                        ?>
                    </div>
                </div>
            </form>
        </div>

    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-iniciativa-indicadores" id="form-iniciativa-indicadores" method="post"
                  class="form-horizontal">
                <input type="hidden" name="form" value="iniciativa-indicadores">
                <input type="hidden" id="form-indicadores-acao" name="acao" value="salvar">
                <input type="hidden" id="form-indicadores-idiid" name="idiid" value="">
                <input type="hidden" name="iniid" value="<?= $_GET['iniid'] ?>">

                <!-- Dimensão: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Dimensão:
                            </label>
                            <div class="col-sm-6">
                                <select name="dimid" id="dimid" class="chosen-select" data-placeholder="Selecione">
                                    <option></option>
                                    <?php
                                    $dimensoes = $objDimensao->listaDimensoes($arrDimensao);
                                    foreach ($dimensoes as $dimensao) {
                                        ?>
                                        <option value="<?= $dimensao['codigo'] ?>"><?= $dimensao['descricao'] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Área: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Área:
                            </label>
                            <div class="col-sm-6">
                                <select name="areid" id="areid" class="chosen-select" data-placeholder="Selecione">
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Indicador: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Indicador:
                            </label>
                            <div class="col-sm-6">
                                <select name="indid" id="indid" class="chosen-select" data-placeholder="Selecione">
                                </select>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Componentes do Indicador: Campo de seleção única, Campo obrigatório; -->
                <div class="row">
                    <div class="col-sm-12">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">
                                Componentes do Indicador:
                            </label>
                            <div class="col-sm-6">
                                <select name="crtid" id="crtid" class="chosen-select" data-placeholder="Selecione">
                                </select>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-sm-12 text-center">
                        <button type="button" id="salvar-iniciativa-indicadores"
                                class="btn btn-success">
                            <i class="fa fa-plus"></i> Adicionar
                        </button>
                    </div>
                </div>
        </div>
        <hr>
        <div class="row">
            <?php
            $controllerIniciativaIndicadores->listar($_GET['iniid']);
            ?>
        </div>
        </form>
    </div>

</div>
<div class="ibox-footer">
    <div class="center">
        <a href="par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativas&iniid=<?= $_GET['iniid'] ?>"
           class="btn btn-primary xls"
           style="width: 110px;">
            <i class="fa fa-mail-reply"></i> Voltar
        </a>
        <button type="button" id="bt-proximo" class="btn btn-success xls" style="width: 110px;"
        	onclick="window.location.href='par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaItensComposicao&iniid=<?php echo $_GET['iniid']; ?>'"
        >
            <i class="fa fa-mail-forward"></i> Próximo
        </button>
    </div>
</div>
</div>
<script>
            
    /**
     * Validação do formulário Iniciativa Modalidades.
     */
    $('#salvar-iniciativa-modalidades').click(function () {

        if ($('#nivid').val().length <= 0) {
            alert("O campo Nível é  obrigatório e não foi preenchido.");
            return false;
        }

        if ($('#etaid').val().length <= 0) {
            alert("O campo Etapa é  obrigatório e não foi preenchido.");
            return false;
        }

        if ($('#modid').val().length <= 0) {
            alert("O campo Modalidade é  obrigatório e não foi preenchido.");
            return false;
        }

        if ($('#desid').val().length <= 0) {
            alert("O campo Desdobramento é  obrigatório e não foi preenchido.");
            return false;
        }

        var dados = $('#form-iniciativa-modalidades').serialize();

        $.ajax({
            url: "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe",
            data: {
                dados: dados,
                acao: 'validar',
                form: 'iniciativa-modalidades',
            },
            dataType: "json",
            type: "POST",
            success: function (data) { 
                //console.log('success data - salvar-iniciativa-modalidades: '); console.log(data);            
                verificarValidacao(data, '#form-iniciativa-modalidades');   
            },
            error: function (data) { 
				console.log('error data - salvar-iniciativa-modalidades: '); console.log(data);
            }
        });

        return true;
    });

    /**
     * Validação do formulário Iniciativa Indicadores.
     */
    $('#salvar-iniciativa-indicadores').click(function () {

        if ($('#dimid').val().length <= 0) {
            alert("O campo Dimensão é  obrigatório e não foi preenchido.");
            return false;
        }

        var dados = $('#form-iniciativa-indicadores').serialize();

        $.ajax({
            url: "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe",
            data: {
                dados: dados,
                acao: 'validar',
                form: 'iniciativa-indicadores',
            },
            dataType: "json",
            type: "POST",
            success: function (data) {
            	//console.log('success data - salvar-iniciativa-indicadores: '); console.log(data);
                verificarValidacao(data, '#form-iniciativa-indicadores');
            },
            error: function (data) { 
				console.log('error data - salvar-iniciativa-indicadores: '); console.log(data);
            }
        });

        return true;
    });

    /**
     * Verificar o retorno e enviar o formulário
     */
    function verificarValidacao(data, idForm) {
        var status = data.status;
        var mensagem = data.mensagem;
        if (status === "false") {
            alert(mensagem);
            return false;
        } else {
            $(idForm).submit();
        }
    }

    $(document).ready(function () {
        $('#consultarMytable').DataTable();
    });

    /**
     * Busca a combo Etapa
     */
    $('#nivid').change(function () {
        var nivid = $(this).val();
        if (nivid > 0) {
            montaComboEtapa(nivid);
        } else {

            //limpa a combo Etapa
            $('#etaid').empty();
            $('#etaid').trigger("chosen:updated");

            //Limpa a combo modalidade
            $('#modid').empty();
            $('#modid').trigger("chosen:updated");
        }
    });

    /**
     * Busca a combo Modalidade
     */
    $('#etaid').change(function () {
        var etaid = $(this).val();
        if (etaid > 0) {
            montaComboModalidade(etaid);
        } else {
            $('#modid').empty();
            $('#modid').trigger("chosen:updated");
        }
    });

    /**
     * Busca a combo Área
     */
    $('#dimid').change(function () {
        var dimid = $(this).val();
        if (dimid > 0) {
            montaComboArea(dimid);
        } else {

            //Limpa a combo Área
            $('#areid').empty();
            $('#areid').trigger("chosen:updated");

            //Limpa a combo Indicador
            $('#indid').empty();
            $('#indid').trigger("chosen:updated");

            //Limpa a combo Componentes do Indicador
            $('#crtid').empty();
            $('#crtid').trigger("chosen:updated");
        }
    });

    /**
     * Busca a combo Indicador
     */
    $('#areid').change(function () {
        var areid = $(this).val();
        if (areid > 0) {
            montaComboIndicador(areid);
        } else {
            $('#indid').empty();
            $('#indid').trigger("chosen:updated");
        }
    });


    /**
     * Busca a combo Componentes do Indicador
     */
    $('#indid').change(function () {
        var indid = $(this).val();
        if (indid > 0) {
            montaComboComponentesIndicador(indid);
        } else {
            $('#crtid').empty();
            $('#crtid').trigger("chosen:updated");
        }
    });


    /**
     * Monta a combo Etapa
     *
     * @param nivid
     */
    function montaComboEtapa(nivid) {

        //Envia o código via Ajax pelo método Get
        $.getJSON('par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&metodo=json&opcao=etapas&nivid=' + nivid, function (data, textStatus) {
            if (textStatus == 'success') { //verifica se o status está tudo ok!
                var option = '<option value label selected=selected ></option>';
                //Monta a combo
                $.each(data, function (i, obj) {
                    option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                })

                //Envia as opções para o select
                $('#etaid').empty();
                var newOption = $(option);
                $('#etaid').append(newOption);
                $('#etaid').trigger("chosen:updated");

                //Limpa a combo modalidade
                $('#modid').empty();
                $('#modid').trigger("chosen:updated");
            }
        });

    }

    /**
     * Monta a combo Modalidade
     *
     * @param etaid
     */
    function montaComboModalidade(etaid) {

        //Envia o código via Ajax pelo método Get
        $.getJSON('par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&metodo=json&opcao=modalidades&etaid=' + etaid, function (data, textStatus) {
            if (textStatus == 'success') { //verifica se o status está tudo ok!
                var option = '<option value label selected=selected ></option>';
                //Monta a combo
                $.each(data, function (i, obj) {
                    option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                })

                //Envia as opções para o select
                $('#modid').empty();
                var newOption = $(option);
                $('#modid').append(newOption);
                $('#modid').trigger("chosen:updated");
            }
        });

    }

    /**
     * Monta a combo Área
     *
     * @param dimid
     */
    function montaComboArea(dimid) {

        //Envia o código via Ajax pelo método Get
        $.getJSON('par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&metodo=json&opcao=areas&dimid=' + dimid, function (data, textStatus) {
            if (textStatus == 'success') { //verifica se o status está tudo ok!
                var option = '<option value label selected=selected ></option>';
                //Monta a combo
                $.each(data, function (i, obj) {
                    option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                })

                //Envia as opções para o select
                $('#areid').empty();
                var newOption = $(option);
                $('#areid').append(newOption);
                $('#areid').trigger("chosen:updated");

                //Limpa a combo Indicador
                $('#indid').empty();
                $('#indid').trigger("chosen:updated");

                //Limpa a combo Componentes do Indicador
                $('#crtid').empty();
                $('#crtid').trigger("chosen:updated");
            }
        });

    }

    /**
     * Monta a combo Indicador
     *
     * @param areid
     */
    function montaComboIndicador(areid) {

        //Envia o código via Ajax pelo método Get
        $.getJSON('par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&metodo=json&opcao=indicadores&areid=' + areid, function (data, textStatus) {
            if (textStatus == 'success') { //verifica se o status está tudo ok!
                var option = '<option value label selected=selected ></option>';
                //Monta a combo
                $.each(data, function (i, obj) {
                    option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                })

                //Envia as opções para o select
                $('#indid').empty();
                var newOption = $(option);
                $('#indid').append(newOption);
                $('#indid').trigger("chosen:updated");

                //Limpa a combo Componentes do Indicador
                $('#crtid').empty();
                $('#crtid').trigger("chosen:updated");
            }
        });

    }

    /**
     * Monta a combo Componentes do Indicador
     *
     * @param areid
     */
    function montaComboComponentesIndicador(indid) {

        //Envia o código via Ajax pelo método Get
        $.getJSON('par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&metodo=json&opcao=componentesIndicador&indid=' + indid, function (data, textStatus) {
            if (textStatus == 'success') { //verifica se o status está tudo ok!
                var option = '<option value label selected=selected ></option>';
                //Monta a combo
                $.each(data, function (i, obj) {
                    option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                })

                //Envia as opções para o select
                $('#crtid').empty();
                var newOption = $(option);
                $('#crtid').append(newOption);
                $('#crtid').trigger("chosen:updated");
            }
        });

    }

    /**
     * Excluir Iniciativa Modalidade.
     *
     * @param id
     */
    function removerModalidades(id) {
        swal({
                title: " Excluir",
                text: "Tem certeza que deseja excluir o item selecionado?",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }
            , function (isConfirm) {
                if (isConfirm) {
                    $("#form-modalidades-imoid").val(id);
                    $("#form-modalidades-acao").val('inativar');
                    $("#form-iniciativa-modalidades").submit();
                }
            });
    }

	
    
    /**
     * Excluir Iniciativa Indicadores.
     *
     * @param id
     */
    function removerIndicadores(id) {
        swal({
                title: " Excluir",
                text: "Tem certeza que deseja excluir o item selecionado?",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }
            , function (isConfirm) {
                if (isConfirm) {
                    $("#form-indicadores-idiid").val(id);
                    $("#form-indicadores-acao").val('inativar');
                    $("#form-iniciativa-indicadores").submit();
                }
            });
    }


</script>

