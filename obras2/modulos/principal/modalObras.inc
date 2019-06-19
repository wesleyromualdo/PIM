<?php
/**
 * Modal OBRAS
 */

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=utf-8');
        $estuf = $_REQUEST['estuf'];
        ?>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>
        <select name="muncod[]" class="chosen-select municipios form-control" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
}
$query = null;
if($_POST['pesquisar']){
    $param = $_POST;
$query = (new AlertaObra())
    ->retornaListaDeObrasFiltradas($param, ['sql' => true]);
}

?>
<html>
    <head>
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="/library/bootstrap-3.0.0/js/bootstrap.js" type="text/javascript"></script>
        <link href="../library/bootstrap-3.0.0/css/bootstrap.css" rel="stylesheet"  media="screen" >
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script>

            function parametrosFiltro(){
                var params = {};
                $('#modal_obras #formulario select').each(function() {
                    var inputName = $(this).attr('name'),
                        inputValue = $(this).val();
                    params[inputName] = inputValue;
                });
                params['convenio'] = $('#modal_obras #formulario [name=convenio]').val();
                params['ano_convenio'] = $('#modal_obras #formulario [name=ano_convenio]').val();
                params['processo'] = $('#modal_obras #formulario [name=processo]').val();
                params['ano_processo'] = $('#modal_obras #formulario [name=ano_processo]').val();
                params['obrid'] = $('#modal_obras #formulario [name=obrid]').val();
                params['pesquisar'] = $('#modal_obras #formulario [name=pesquisar]').val();
                return params;
            }

            function delegatePaginacao(){
                $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
                    // -- definindo a nova página
                    var novaPagina = $(this).attr('data-pagina');
                    var params = parametrosFiltro();
                    params['listagem[p]'] = novaPagina;
                    atualizaListagem(params);
                });
            }

            $(document).ready(function(){
                $('#modal_obras .btn-info').click(function(e) {
                    e.stopPropagation();
                    var params = parametrosFiltro();
                    atualizaListagem(params);
                    return false;
                });
            });

            function atualizaListagem(params){
                $('#modal_obras #modal_body').html('');
                $.post('obras2.php?modulo=principal/modalObras&acao=A', params, function(response) {
                    $('#modal_obras #modal_body').html(response);
                    delegatePaginacao();
                });
            }
        </script> 
    </head>
    <body>
    <section class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                <div class="form-group">
                    <label for="inputObrid" class="col-lg-2 control-label">ID da Obra:</label>
                    <div class="col-lg-10">
                        <?=campo_texto('obrid', 'N', 'S', '', 20, 20, '#################', '', 'right', '', 0, '','',$_REQUEST['obrid'])?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Situação:</label>
                    <div class="col-lg-10">
                        <select name="strid[]" class="chosen-select" multiple data-placeholder="Selecione">
                            <?php $situacaoRegistro = new SituacaoRegistro();
                            foreach ($situacaoRegistro->listaCombo() as $key) {
                                ?>
                                <option
                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['strid']) && in_array($key['codigo'], $_POST['strid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Tipologia:</label>
                    <div class="col-lg-10">
                        <select name="tpoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                            <?php   $tipologiaObra = new TipologiaObra();
                            $param = array("orgid" => $_SESSION['obras2']['orgid']);
                            $dados = $tipologiaObra->listaCombo($param, false);
                            foreach ($dados as $key) {
                                ?>
                                <option
                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tpoid']) && in_array($key['codigo'], $_POST['tpoid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Programa:</label>
                    <div class="col-lg-10">
                        <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                            <?php  $programa = new ProgramaFonte();
                            $param = array("orgid" => $_SESSION['obras2']['orgid']);
                            foreach ($programa->listacombo($param, false) as $key) {
                                ?>
                                <option
                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['prfid']) && in_array($key['codigo'], $_POST['prfid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Fonte:</label>
                    <div class="col-lg-10">
                        <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                            <?php  $tipoOrigemObra = new TipoOrigemObra();
                            $param = array();
                            foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                                ?>
                                <option
                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tooid']) && in_array($key['codigo'], $_POST['tooid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">UF(s):</label>
                    <div class="col-lg-10">
                        <select name="estuf[]" class="chosen-select estados" multiple data-placeholder="Selecione">
                            <?php  $uf = new Estado();
                            foreach ($uf->listaCombo() as $key) {
                                ?>
                                <option
                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['estuf']) && in_array($key['codigo'], $_POST['estuf'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Município(s):</label>
                    <div class="col-lg-10 td_municipio" id="">
                        <?php if (!empty($_POST['estuf'])) { ?>
                            <select name="muncod[]" class="chosen-select municipios form-control" multiple data-placeholder="Selecione">
                                <?php   $municipio = new Municipio();
                                foreach ($municipio->listaComboMulti($_POST['estuf']) as $key) {
                                    ?>
                                    <option
                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['muncod']) && in_array($key['codigo'], $_POST['muncod'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                <?php } ?>
                            </select>
                        <?php } ?>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Esfera:</label>
                    <div class="col-lg-10">
                        <?php
                        $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                            Array('codigo' => 'M', 'descricao' => 'Municipal'));
                        $db->monta_combo('empesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'empesfera',false,$_POST['empesfera']);
                        ?>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Convênio/Termo:</label>
                    <div class="col-lg-10" style="padding-top:4px;">
                        Número:&nbsp;
                        <?php
                        echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '', '',$_POST['convenio']);
                        ?>
                        Ano:&nbsp;
                        <?php
                        echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '','',$_POST['ano_convenio']);
                        ?>
                    </div>
                </div>

                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Processo:</label>
                    <div class="col-lg-10" style="padding-top:4px;">
                        Número:&nbsp;
                        <?php
                        echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '','',$_POST['processo']);
                        ?>
                        Ano:&nbsp;
                        <?php
                        echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '','',$_POST['ano_processo']);
                        ?>
                    </div>
                </div>

                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <input hidden="pesquisar" value="pesquisar" name="pesquisar" />
                        <input type="submit" class="btn btn-info" value="Pesquisar" name="botao" />
                    </div>
                </div>
            </form>
        </fieldset>
    </section>
    <section class="form-group">
        <div id="list-render">
            <?php
            if(!empty($query)){
                require_once APPRAIZ . "includes/library/simec/Listagem.php";
                $cabecalho = array ("Obrid","Obra");
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
                $listagem->setIdLinha();
                $listagem->addAcao('select', array('func' => 'resultado'/*, 'extra-params' => array('ptres')*/));
                $listagem->setCabecalho($cabecalho);
                #$listagem->turnOnPesquisator();
                #$listagem->addCallbackDeCampo(array('dotacaoinicial','det_subacao','nao_det_subacao','empenhado','nao_empenhado'), 'mascaraMoeda');
                #ver($sql_lista);
                $listagem->setQuery($query);
                $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
                    ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::MARCAR_TODOS)
                    ->setFormFiltros('formulario');

                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            }
            
            ?>
        </div>
        <style>
            .control-label{
                font-size:13px;
            }
        </style>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="strid[]"]').chosen();
                $1_11('select[name="tpoid[]"]').chosen();
                $1_11('select[name="prfid[]"]').chosen();
                $1_11('select[name="tooid[]"]').chosen();
                $1_11('select[name="estuf[]"]').chosen();
                $1_11('select[name="muncod[]"]').chosen();
                $1_11('select[name="empesfera"]').chosen();

                $1_11(".estados").chosen().change(function (e, params) {
                    values = $1_11(".estados").chosen().val();
                    carregarMunicipio(values);
                });
            });

            function carregarMunicipio(estuf) {
                var td = $1_11('.td_municipio');
                if (estuf != '') {
                    var url = 'obras2.php?modulo=principal/modalObras&acao=A';
                    $1_11.ajax({
                        url: url,
                        type: 'post',
                        data: {
                            ajax: 'municipio',estuf: values
                        },
                        dataType: "html",
                        async: false,
                        beforeSend: function () {
                            td.find('select option:first').attr('selected', true);
                        },
                        error: function () {
                            alert(2);
                        },
                        success: function (data) {
                            td.html(data);
                        }
                    });
                } else {
                    td.find('select option:first').attr('selected', true);
                    td.find('select').attr('selected', true)
                        .attr('disabled', true);
                }
            }

            jQuery(document).ready(function() {
                apresentaLinhasSelecionadas($('#obra_lista'), 3);
            });

            function resultado(id, selecionado, adicionais)
            {
                var obrid = $('tr#' + id + ' td:nth-child(2)').text();
                if (!obrid) {
                    return false;
                }

                if (selecionado == false) {
                    var tabelaorigem = document.getElementById('obra_lista');
                    var colunaObrid = document.getElementById('obrid_' + id);
                    if (colunaObrid != null) {
                        tabelaorigem.deleteRow(document.getElementById('obrid_' + id).rowIndex);
                    }

                    // -- Criando a nova linha na tabela da página de origem
                    var linha = tabelaorigem.insertRow(3);
                    linha.id = "obrid_" + id;

                    // -- O número da primeira linha da tabela onde serão inseridos os novos dados
                    var linha = 3;

                    // setando a obra
                    var celObrid = tabelaorigem.rows[linha].insertCell(0);
                    celObrid.style.textAlign = "left";
                    celObrid.innerHTML = obrid;

                    var celObra = tabelaorigem.rows[linha].insertCell(1);
                    celObra.style.textAlign = "left";
                    var obra = $('tr#' + id + ' td:nth-child(3)').text();
                    celObra.innerHTML = obra + "<input type='hidden' name='obrid[]' data-obrid='" + obrid + "' value ='"+obrid+"' />";
                } else {
                    var tabelaorigem = document.getElementById('obra_lista');
                    var obrid_out = $('#obrid_'+id).find('td input[name="obrid_in[]"]').val();
                    if(obrid_out) {
                        $('#form_notificacao').prepend("<input type='hidden' name='obrid_out[]' value ='"+obrid_out+"' />");
                    }
                    tabelaorigem.deleteRow(document.getElementById('obrid_' + id).rowIndex);
                }
            }

            function apresentaLinhasSelecionadas(tabelaOrigem,apartirDe){
                var val;
                for (i = apartirDe; i < $(tabelaOrigem).find('tr').length; i++) {
                    val = $(tabelaOrigem).find('tr')[i].id.split("_");
                    if (document.getElementById(val[1])) {
                        campo = $('tr#' + val[1] + ' td:nth-child(1)').find('span');
                        campo.attr('class','glyphicon glyphicon-remove');
                        campo.attr('title','Remover item');
                        campo.css('color','gray');
                    }
                }
            }
        </script>
    </section>
    </body>
    </html>
<?php
unset($_REQUEST['prgcod']);
unset($_REQUEST['acacod']);
exit();
