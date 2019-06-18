<?php

/**
 * Lista de Estados
 *
 * @category Lista
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 28-09-2015
 * @link     no link
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
require APPRAIZ.'includes/cabecalho.inc';
require APPRAIZ.'includes/Agrupador.php';

switch ($_REQUEST['req']) {
    case 'visualisarNutricionista':
        $danid = $_REQUEST['danid'];
        $tipoId = $_REQUEST['tipo_id'];

        $controllerDadosNutricionista = new Par3_Controller_DadosNutricionista();
        ob_clean();
        $controllerDadosNutricionista->visualisarNutricionista($danid, $tipoId);
        die();
        break;
    case 'gerar_xls':
        ob_clean();
        $controllerVinculacao = new Par3_Controller_VinculacaoNutricionista();
        $controllerVinculacao->imprimirXlsUnidadesVinculo($_REQUEST);
        die();
        break;
    case 'analisar':
        $vinculo = new Par3_Model_VinculacaoNutricionista($_REQUEST['vnid']);
        $vinculo->snid = $_REQUEST['snid'];
        $vinculo->usucpfalteracao = $_SESSION['usucpf'];
        $vinculo->salvar();
        $vinculo->commit();
        if ($_REQUEST['snid'] == 4) {
            echo 'aprovado';
        } else {
            echo 'reprovado';
        }
        die();
        break;
    case 'importarcfn':
        ob_clean();
        $controller = new Par3_Controller_CfnNutricionistaArquivos();
        echo simec_json_encode($controller->salvar($_POST));
        die;
        break;
    case 'listarArquivos':
        $controller = new Par3_Controller_CfnNutricionistaArquivos();
        ob_clean();
        echo $controller->listarHistoricoArquivos();
        die;
        break;
    case 'download':
        ob_clean();
        $arqid = $_GET['arqid'];
        if ($arqid) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file         = new FilesSimec(null, null, "par3");
            $file->getDownloadArquivo($arqid);
        }
        die();
        break;
    case 'carregarMunicipiosAnalise':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        echo simec_json_encode($municipio->carregar($municipio->pegarSQLSelect($_GET['estuf'])));
        die;
        break;
    case 'carregarTodosMunicipios':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        $arrMun = $municipio->recuperarListaMunicipios();
        echo simec_json_encode($arrMun);
        die;
        break;
    case 'listagemAjax':
        ob_clean();
        $controller = new Par3_Controller_CfnNutricionistaArquivos();
        echo $controller->getListagemAjax($_GET);
        die;
        break;
    case 'xls-situacao-entidade':
        ob_clean();
        $controllerVinculacao = new Par3_Controller_VinculacaoNutricionista();
        $controllerVinculacao->gerarRelatorioXlsEntidades($_POST['snentidadedsc']);
        die;
        break;
    case 'gerarCsv':
        ob_clean();
        $controllerVinculacao = new Par3_Controller_VinculacaoNutricionista();
        $controllerVinculacao->gerarCsv($_POST);
        die;
        break;
    case 'carregarGraficos':
        ob_clean();
        require_once APPRAIZ . '/includes/library/simec/Grafico.php';
        include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/graficos-panel.php';
        die;
        break;
    case 'xlsnaoatualizados':
        ob_clean();
        $cfnid = $_GET['cfnid'];
        $controllerVinculacao = new Par3_Controller_CfnNutricionistaArquivos();
        $controllerVinculacao->gerarXlsNutricionistasNaoAtualizados($cfnid);
        die;
        break;
    default:
        $controllerVinculacao = new Par3_Controller_VinculacaoNutricionista();
        break;
}



if ($_POST['xls'] == 1) {
    ob_clean();
    $controllerVinculacao->listaVinculos($_POST);
    die();
    //die;
}

echo $simec->title('Analisar Nutricionista - FNDE', '');
?>
<script>
    $(document).ready(function(){

        $('.gera_xls').click(function(){
            var url = 'par3.php?modulo=principal/acompanhamento/analisarNutricionista&acao=A';
            var param = '&req=gerar_xls&tipo='+$(this).attr('tipo');
            window.open(
                url+param,
                "_blank",
                "toolbar=no, scrollbars=no, resizable=yes, top=10, left=10, width=10, height=10"
            );
        });

        $('.limpar').click(function() {

            /* não está funcionando utilizando .val('') no atributo nome */
            $('#nome').attr('value','');
            $('#dancrnuf').val('').trigger('chosen:updated');
            $("#snid").val('Selecione').trigger('chosen:updated');
            $("#estuf").val('Selecione').trigger('chosen:updated');
            $("#muncod").empty().trigger('chosen:updated');

            $("#tenid").val('Selecione').trigger('chosen:updated');
            $("input[name='validacao']:checked").attr('checked',false).parent('label').removeClass('active');
            $("input[name='validacao']").filter("[value='']").attr('checked',true).parent('label').addClass('active');

            $("input[name='vnstatus']:checked").attr('checked',false).parent('label').removeClass('active');
            $("input[name='vnstatus']").filter("[value='']").attr('checked',true).parent('label').addClass('active');

            $("input[name='itrid']:checked").attr('checked',false).parent('label').removeClass('active');
            $("input[name='itrid']").filter("[value='0']").attr('checked',true).parent('label').addClass('active');
        });

        $('.analisar').click(function(){

            var vnid = $(this).attr('vnid');
            var snid = $(this).val();
            var snid_anterior = $('#snid_anterior').val();

            if(snid == '4'){
                var acao = 'aprovar';
            }else{
                var acao = 'reprovar';
            }

            if(confirm('Deseja '+acao+' o nutricionista?')){
                $.ajax({
                    type: 'post',
                    url: window.location.href,
                    data: 'req=analisar&vnid='+vnid+'&snid='+snid,
                    async: false,
                    success: function (res) {
                        if(res == 'aprovado'){
                            $('#snid-5').parent().parent().parent().parent().css('background-color','#99ff99');
                            swal('Sucesso','Nutricionista aprovado','success');
                        }else if(res == 'reprovado'){
                            $('#snid-5').parent().parent().parent().parent().css('background-color','#ffcccc');
                            swal('Sucesso','Nutricionista reprovado','success');
                        }else{
                            swal('Erro','Occoreu um erro no sistema','error');
                            $(this).attr('checked',false);
                            $('[name="snid"][value="'+snid_anterior+'"]').prop('checked', true);
                        }
                    }
                });
            }else{
                $(this).attr('checked',false);
                $('[name="snid"][value="'+snid_anterior+'"]').prop('checked', true);
            }
        });

        $("#bt_gerar_xls").click(function(){
            $("#xls").val('1');
        });
        $("#bt_pesquisa").click(function(){
            $("#xls").val('0');
            // $('#muncod option').prop('selected', true);
        });






    });

</script>
<div class="wrapper wrapper-content">
    <div class="row">
        <div class="ibox">
            <div class="ibox-title">
                <center>
                    <h5>Gráficos</h5>
                </center>
                <div class="ibox-tools">
                    <a id="btn-avancado-graficos">Mostrar/Esconder Gráficos
                        <i id="chevron-grafico" class="fa fa-chevron-up"></i>
                    </a>
                </div>
            </div>
            <div class="ibox-content">
                <div class="row" id="graficos" style="">
                <?php
                    require_once APPRAIZ . '/includes/library/simec/Grafico.php';
                    include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/graficos-panel.php';
                ?>
                </div>
                <form id='tmpform' name='formulario' method='post' style="display: none;"></form>

                <form id='tmpformentidade' name='formulario' method='post' style="display: none;">
                    <input type="hidden" name="req" value="xls-situacao-entidade"/>
                    <select name="snentidadedsc[]" id="snentidadedsc" multiple="multiple"></select>
                </form>

            </div>
        </div>
    </div>

    <div class="row">
        <div class="ibox">
            <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                <div class="ibox-title">
                    <center>
                        <h5>Filtros</h5>
                    </center>
                </div>
                <div class="ibox-content">
                    <div class="row">
                        <div class="col-lg-6">
                            <?php
                            $usucpf = simec_htmlentities($_POST['usucpf']);
                            $config = array();
                            $attribs = array('maxlength' => '255',
                                'onkeyup' => "this.value=mascaraglobal('###.###.###-##',this.value)",
                                'onblur' => "MouseBlur(this);this.value=mascaraglobal('###.###.###-##',this.value);");

                            echo $simec->input('usucpf', 'CPF', $usucpf, $attribs, array('input-size' => '6'));

                            echo $simec->input('nome', 'Nome', $_POST['nome'], array('maxlength' => '255'));

                            $arrEstado = Territorios_Model_Estado::pegarSQLSelect($_POST);

                            echo $simec
                                ->select(
                                    "dancrnuf",
                                    'UF da CRN',
                                    $_POST['dancrnuf'],
                                    $arrEstado,
                                    array('maxlength' => '255')
                                );
                            ?>

                            <?php
                            $radioValidacao = array('s' => 'Sim', 'n' => 'Não', '' => 'Todos');
                            echo $simec
                                    ->radio(
                                        'validacao',
                                        'Entidade Disponível para validação',
                                        ($_POST['validacao']?:''),
                                        $radioValidacao,
                                        array()
                                    );

                            $radioTenid = array(
                                Par3_Model_InstrumentoUnidadeEntidade::NUTRICIONISTA_RESPONSAVEL
                                    => 'Responsável Técnico / Nutricionista',
                                Par3_Model_InstrumentoUnidadeEntidade::NUTRICIONISTA_QUADRO
                                    => 'Quadro Técnico/Nutricionistas',
                                Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_INFANTIL
                                    => 'Exclusivo na Modalidade Educação Infantil',
                                 '' => 'Todos'
                            );
                            echo $simec
                                    ->select(
                                        'tenid[]',
                                        'Cargo/Função',
                                        $_POST['tenid'],
                                        $radioTenid,
                                        array(),
                                        array(
                                                'type' => 'radio radio-info radio',
                                                'multiple' => 'multiple',
                                                'style' => 'inline'
                                            )
                                    );
                            ?>
                        </div>
                        <div class="col-lg-6">
                            <?php
                            echo $simec
                                    ->radio(
                                        'itrid',
                                        'Esfera',
                                        ($_POST['itrid']?:0),
                                        [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'],
                                        array('maxlength' => '255')
                                    );

                            echo $simec
                                    ->select(
                                        'estuf',
                                        'Estado',
                                        $_POST['estuf'],
                                        $arrEstado,
                                        array('maxlength' => '255','multiple' => 'multiple'),
                                        array()
                                    );
                            ?>

                            <div class="" id="div-muncod" style="display:<?= $_POST['itrid'] == 1 ?'none':''?>;">
                                <?php
                                $arrMunicipio = array();
                                if ($_POST['muncod'] && $_POST['itrid'] != 1) {
                                    $_POST['muncod'] = tratarArrayParaMultiSelect($_POST['muncod']);//trata o array
                                    $municipio = new Territorios_Model_Municipio();//instancia a classe município
                                    //lista todos os municípios relacionados ao Estado(estuf) informado
                                    $arrMunicipio = $municipio->pegarSQLSelect($_POST['estuf']);
                                }
                                echo $simec
                                    ->select(
                                        'muncod[]',
                                        'Município',
                                        $_POST['muncod'],
                                        $arrMunicipio,
                                        array(
                                                'maxlength' => '255',
                                                'multiple'  => 'multiple',
                                                'onclick'   => 'listarMunicipios()'
                                        ),
                                        array()
                                    );
                                ?>
                            </div>

                            <?php
                            $sql = "SELECT
                                    	snid as codigo,
                                    	sndescricao as descricao
                                    FROM
                                    	par3.situacaonutricionista ";

                            echo $simec
                                    ->select(
                                        "snid[]",
                                        'Situação',
                                        $_POST['snid'],
                                        $sql,
                                        array(
                                                'maxlength' => '255',
                                                null
                                            )
                                    );

                            $radioVinculacao = array('A' => 'Vinculado', 'I' => 'Desvinculado', '' => 'Todos');
                            echo $simec
                                    ->radio(
                                        'vnstatus',
                                        'Vinculação',
                                        ($_POST['vnstatus']?:''),
                                        $radioVinculacao,
                                        array()
                                    );
                            ?>
                        </div>
                    </div>
                </div>
                <div class="ibox-footer">
                    <div class="row">
                        <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                            <input type="hidden" name="xls" id="xls" value="0">
                            <button type="submit" class="btn btn-info" id="bt_pesquisa" >
                                <i class="fa fa-search"></i>Pesquisar
                            </button> |
                            <button class="btn btn-primary" id="bt_gerar_xls" value="xls"><i class="fa fa-file-excel-o">
                                </i> Gerar Excel
                            </button> |
                            <button type="button" class="btn btn-warning" id="importar-cfn" >
                                <i class="fa fa-upload"></i> Importar Arquivo CFN
                            </button> |
                            <button type="reset" class="btn btn-default-bright limpar">
                                <i class="fa fa-eraser"></i>Limpar
                            </button>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
    <div class="row">
        <div class="ibox">
            <div class="ibox-content">

                <?php
                if ($_POST) {
                    // ob_clean();
                    $controllerVinculacao->listaVinculos($_POST);
                    //die;
                }
                ?>
            </div>
        </div>
    </div>
    <!--modal-->
    <div class="ibox float-e-margins animated modal" id="modal_importar_cfn" role="dialog" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <form
                    method="post"
                    name="formimportarcfn"
                    id="formimportarcfn"
                    class="form form-horizontal"
                    enctype="multipart/form-data"
            >
                <div class="modal-content">
                    <div class="ibox-title">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                    aria-hidden="true">&times;</span></button>
                        <h3 id="modal_titulo" class="center">Importar arquivo de Nutricionistas do CFN</h3>
                    </div>
                    <div class="ibox-content" id="conteudo-modal">
                        <div class="col-lg-offset-3">
                        <label for="">Anexo*:</label>
                            <?=
                            $simec
                                ->input(
                                    'arqid',
                                    '',
                                    null,
                                    array(
                                        'type'=>'file',
                                        'required' => 'required',
                                        'help' =>'Formatos permitidos: csv (colunas separadas pela opção informada no campo delimitador)'
                                    ),
                                    array('input-size'=>'8')
                                )
?>
                            <label for="">Delimitador:</label>
                            <?= $simec
                                    ->select(
                                        "delimitador",
                                        '',
                                        'P',
                                        ['P' =>'Ponto e Vírgula (;)','V' => 'Vírgula (,)'],
                                        array('maxlength' => '255'),
                                        array('input-size'=>'4')
                                    );
?>
                        </div>
                        <div class="col-lg-offset-3">
                            <label for="">Observação:</label>
                            <?php inputTextArea(
                                'cfnobservacao',
                                'Observação',
                                'cfnobservacao',
                                200,
                                array('obrig' => 'S', 'habil' => 'S','cols' => 55, 'rows' => 4, 'width' => null)
                            );
?>
                        </div>
                        <br>
                        <br>
                        <div class="ibox-title">
                            <div class="ibox-tools">
                                <a id="btn-avancado" >Histórico <i id="chevron" class="fa fa-chevron-down"></i></a>
                            </div>
                        </div>
                        <div id="avancado" class="container-fluid" style="display: none;">
                            <table class="table table-hover table-bordered" style="border:1px solid #999" id="listaCfn">
                                <thead>
                                <tr>
                                    <th class="center"></th>
                                    <th class="center">Arquivo</th>
                                    <th class="center">Observação</th>
                                    <th class="center">Data de Inclusão</th>
                                    <th class="center">Usuário</th>
                                    <th class="center">Total de Registros</th>
                                    <th class="center">Não Atualizados</th>
                                </tr>
                                </thead>
                            </table>
                        </div>
                    </div>
                    <div class="ibox-footer">
                        <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                            <button type="button" class="btn btn-primary" id="importar">
                                <i class="fa fa-upload"></i> Importar
                            </button>
                            <button type="reset" id="" class="btn btn-success" data-dismiss="modal">
                                <i class="fa fa-times-circle-o"></i> Fechar
                            </button>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>
<style>
    .popover-content {
        word-wrap: break-word;
    }
</style>
<script>
        // $('#listagemcfn thead th').each( function () {
        //     var title = $(this).text();
        //     $(this).html( '<input type="text" placeholder="Procurar '+title+'" />' );
        // } );

    // }
        function teste(id,event){
            console.log(id);
            var arrdsc = [];
            var select  = '';
            switch(id){
                case 'countTipoNutricionistas':    select = $('#tenid');break;
                case 'countSituacaoNutricionistas':select = $('#snid') ;break;
                case 'countSituacaoEntidades': break;
                default:break;
            }
            setTimeout(
                function(){
                    if(select !== ''){
                        select.focus();
                        select.val('');
                        select.trigger('chosen:updated');
                    }
                    event.series.data.forEach(function(sr){
                        if(sr.visible){
                            console.log(sr.name, "visible!");
                            arrdsc.push(sr.name);
                        }
                    });
                    switch(id){
                        case 'countTipoNutricionistas':atualizarSelect(arrdsc,'tenid');break;
                        case 'countSituacaoNutricionistas':atualizarSelect(arrdsc,'snid');break;
                        case 'countSituacaoEntidades': atualizarSelectEntidade(arrdsc);break;
                        default:break;
                    }

                }, 100);
        }

        function filtrarChart(id)
        {
            var arrDados  = [];
            var select  = $('#'+id);
            select.find('option').map(function(index,val){
                arrDados.push(val.value);
            });
            var input = $('#formulario').find('#'+id);
            var form = $("#tmpform");
            form.append(input);
            form.submit();

        }

        function gerarRelatorio() {
            $('#tmpformentidade').submit();
        }

        function atualizarSelectEntidade(arrdsc) {
            var arrDados  = [];
            var select  = $('#snentidadedsc');
            select.empty();
            arrdsc.map(function(index,val){
                select.append(new Option(index,index,true,true));
            });
        }

        function atualizarSelect(arrdsc,id) {
            var arrDados  = [];
            var select  = $('#'+id);
            select.focus();
            select.val('');
            select.trigger('chosen:updated');
            select.find('option').map(function(index,val){
                var i = arrdsc.indexOf(val.innerHTML);
                if(i !==  -1){
                    arrDados.push(val.value);
                }else{
                    console.log(val.innerHTML,val.value);
                }
            });
            select.focus();
            select.val(arrDados);
            select.trigger('chosen:updated');
        }

    //Validações
        $(document).ready(function(){
            //mostra/esconde ibox
            $("#btn-avancado-graficos").click(function () {
                $('#graficos').slideToggle();
                $('#loading').hide();
                //chevron up/down
                $('#chevron-grafico').toggleClass(function () {
                    if ($(this).is(".fa-chevron-down")) {
                        $('#chevron-grafico').removeClass('fa-chevron-down');
                        return 'fa-chevron-up';
                    } else {
                        $('#chevron-grafico').removeClass('fa-chevron-up');
                        return 'fa-chevron-down';
                    }
                });
            });

            // $("#btn-avancado-graficos").one('click',function () {
            //     var action = window.location.href+'&req=carregarGraficos';
            //     $.ajax({
            //         type: "GET",
            //         url: action,
            //         beforeSend: function () {
            //             $(document).find('#loading').hide();
            //          },
            //         success: function (res) {
            //             $('#graficos').html(res);
            //          }
            //     });
            // });

            $('#countTipoNutricionistas').on('legendItemClick',function(){
            });
            // // Setup - add a text input to each footer cell
            // $('#listaCfn tfoot th').each( function () {
            //     var title = $(this).text();
            //     $(this).html( '<input type="text" placeholder="Search '+title+'" />' );
            // } );
            $('#listaCfn').DataTable({
                destroy : true,
                "lengthMenu": [[5, 10, 25,50], [5, 10, 25,50]],
                processing: true,
                serverSide: true,
                paginate: true,
                searching: false,
                paging: true,
                responsive: true,
                searchDelay: 1000,
                retrieve: true,
                language: {
                    'sProcessing': "Processando...",
                    'sLengthMenu': "Mostrar _MENU_ registros",
                    'sZeroRecords': "N&atilde;o foram encontrados resultados",
                    'sInfo': "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
                    'sInfoEmpty': "Mostrando de 0 at&eacute; 0 de 0 registros",
                    'sInfoFiltered': "(filtrado de _MAX_ registros no total)",
                    'sInfoPostFix': ".",
                    'sSearch': "Pesquisar :&nbsp;&nbsp;",
                    'sUrl': "",
                    'oPaginate': {
                        'sFirst': "Primeiro",
                        'sPrevious': "Anterior",
                        'sNext': "Seguinte",
                        'sLast': "&Uacute;ltimo"
                    }
                },
                columnDefs: [
                    {name: "arqid",          "targets" : 0},
                    {name: "arquivo",        "targets" : 1},
                    {name: "cfnobservacao",  "targets" : 2},
                    {name: "cfndtinclusao",  "targets" : 3},
                    {name: "usunome",        "targets" : 4},
                    {name: "totalregistros", "targets" : 5},
                    {name: "naoatualizados", "targets" : 6},
                    { "sClass": "center",targets: [0,1,2,3,4,5,6]},
                    {
                        "targets": 0,
                        "orderable": false,
                        "searchable": false,
                        "render": function (data, type, row) {
                            var downloadBtn  = '<a class="btn btn-warning" onclick="downloadArquivo(' + data + ')">';
                                downloadBtn += '<i class="fa fa-download"></i></a>';
                            return downloadBtn;
                        }
                    },
                    {
                        "targets": 2,
                        "render": function (data, type, row) {
                            var obsBtn  = '<span';
                            obsBtn += ' data-toggle="popover"';
                            obsBtn += ' data-html="true" title="Observação"';
                            obsBtn += ' data-trigger="hover" data-content=\''+data+'\' ';
                            obsBtn += ' data-placement=\'auto left\'><i class="fa fa-eye" style="font-size:2em;"></i>';
                            obsBtn += '</span>';
                            return obsBtn;
                        }
                    },
                    {
                        "targets": 6,
                        "render": function (data, type, row) {
                            if(data != 0){
                                var obsBtn = '<a class="btn btn-danger" ' +
                                'title="Clique para fazer download a lista de nutricionistas que não foram atualizados" ' +
                                'onclick="listarNutricionistasNaoAtualizados(' + data + ')"><i class="fa fa-file-excel-o">' +
                                '</i></a>';
                                return obsBtn;
                            }
                            return ' - ';
                        }
                    }
                ],
                ajax: {
                    url: window.location.href + '&req=listagemAjax',
                    type: 'get',
                    beforeLoad : function() {
                        $('#loading').hide();
                    },
                    complete : function(){
                        $('[data-toggle="popover"]').popover({html:true, animation:true, delay:0});
                    },
                    error: function () {

                    },
                }
            });
        });

        function listarNutricionistasNaoAtualizados(cfnid)
        {
            window.location.href = window.location.href +'&req=xlsnaoatualizados&cfnid='+cfnid;
        }

    //mostra/esconde ibox
    $("#btn-avancado").click(function () {
        $('#avancado').slideToggle();
        //chevron up/down
        $('#chevron').toggleClass(function () {
            if ($(this).is(".fa-chevron-down")) {
                $('#chevron').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#chevron').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    $('#importar-cfn').click(function (evt) {
        evt.preventDefault();
        $('#modal_importar_cfn').modal();
        $('#loading').hide();
    });

    $("#modal_importar_cfn").on('hidden.bs.modal',function(){
        $('#formimportarcfn').find('label.error').remove();
        $(this).find("#arqid").val('');
        $(this).find("#cfnobservacao").val('');
        // $(this).find('#avancado').html('');
        $(this).find('div').removeClass('has-error');
        $(this).find('.badge-danger').remove();
        $('#avancado').slideUp();
        //chevron up/down
        $('#chevron').toggleClass(function () {
            if ($(this).is(".fa-chevron-down")) {
                $('#chevron').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#chevron').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    $('#importar').click(function(evt){
        evt.preventDefault();

        var file = document.getElementById('arqid');
        var filearq = file.files[0];
        if(filearq){
            console.log(filearq.type);
        }
        $(document).find('#loading').hide();
        salvarArquivo();
    });

    function salvarArquivo(){
        if(!$('#formimportarcfn').valid()){
            return;
        }
        var action = window.location.href+'&req=importarcfn';
        //Como implementar submit de formulários enctype="multipart/form-data"
        var data = new FormData($('#formimportarcfn').get(0));
        $.ajax({
            type: "POST",
            url: action,
            contentType: false,//necessário para fazer upload
            processData: false,//necessário para fazer upload
            data: data,
            beforeSend: function () {
                $(document).find('#loading').hide();
                $('.loading-dialog-par3').show();
            },
            success: function (resp) {
                $('#loading-dialog-par3').hide();
                $('#loading').hide();
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    $(this).find('div').removeClass('has-error');
                    $(this).find('.badge-danger').remove();
                    $("#modal_importar_cfn").modal("hide");
                    msgSuccess(null,'Processamento realizado com sucesso');
                    return false;
                }
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formimportarcfn").find("input[name=" + err + "]").parent("div");
                        divFormInput.closest('.form-group').addClass('has-error');
                        divFormInput.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='badge badge-danger'>" + errMsg + "</span>");
                    });
                });
            },
            complete : function() {
                $('.loading-dialog-par3').hide();
            },
            error : function () {
                $('.loading-dialog-par3').hide();
                $(document).find('#loading').hide();
                alert('Ocorreu um erro.');
            }
        });
    }

    //Validações
    jQuery(function ($) {
        "use strict";
        $.validator.addMethod('filetype', function (value, element, arg) {
            console.log(arg);
            if(element.files[0].type.indexOf(arg)){
                return true;
            }
            return false;
        });

        $.validator.addMethod('filesize', function (value, element, arg) {
            console.log(element.files[0].size);
            console.info(arg);
            if((element.files[0].size / 1000) <= arg){
                return true;
            }
            return false;
        });

        $("#formimportarcfn" ).validate({
            rules: {
                arqid:{
                    required:true,
                    filetype: ['text/csv','application/vnd.ms-excel'],
                    filesize: 10000
                }
            },messages: {
                arqid:{
                    filesize:"Tamanho máximo permitido: 10mb",
                    filetype:"Adicione somente arquivos .csv.",
                    required:"Adicione um arquivo."
                }
            },
            submitHandler: function(form) {
                form.submit();
            }
        });
    });

    function downloadArquivo(arqid)
    {
        window.location.href = window.location.href +'&req=download&arqid='+arqid;
    }

    jQuery('#estuf').change(function(){
        if($('[name=itrid]:checked').val() == 1){
            return;
        }else{
            if(!Array.isArray($(this).val())){
                listarMunicipios();
            }else{
                carregarMunicipioAnalise($('#estuf').val(),$('#muncod').val());
            }
        }
    });

    function listarMunicipios()
    {
        console.log('testes');
        var options = jQuery('#muncod');
        options.empty();
        options.trigger('chosen:updated');
        options.append(new Option('', ''));
        jQuery.ajax({
            type: "GET",
            url: window.location.href,
            data: { req:'carregarTodosMunicipios'},
            success: function(retorno){
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.descricao, this.codigo));
                });
                options.val(muncod);
                options.trigger('chosen:updated');
            }
        });
    }

    function carregarMunicipioAnalise(estuf, muncod) {
        if(estuf != '' && $('[name=itrid]:checked').val() != 1) {
            var options = jQuery('#muncod');
            options.empty();
            options.trigger('chosen:updated');
            if(!Array.isArray(estuf)){
                return;
            }
            $('#loading').hide();
            options.append(new Option('', ''));
            jQuery.ajax({
                type: "GET",
                url: window.location.href,
                data: { req:'carregarMunicipiosAnalise',estuf : estuf },
                success: function(retorno){
                    options.append(new Option('', ''));
                    $.each(JSON.parse(retorno), function() {
                        options.append(new Option(this.descricao, this.codigo));
                    });
                    options.focus();
                    options.val(muncod);
                    options.trigger('chosen:updated');
                }
            });
        }
    }

    $('#muncod').one('focus',function() {
        console.log('on focus');
        if(Array.isArray($('#estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('#div-muncod').one('click',function() {
        console.log('on click');
        if(Array.isArray($('#estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').empty();
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });
</script>