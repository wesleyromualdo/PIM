<input type="hidden" id="itrid" name="itrid" value="<?php echo $itrid; ?>">
    <div class="row">
        <div class="col-md-12">
        <?php
        $controllerIniciativa = new Par3_Controller_Iniciativa();
        //print_r($controllerIniciativa->recuperar(array('iniid'=>5))); exit();
        //print_r($controllerIniciativa->inativar(array('iniid'=>5))); exit();
        function retornaRelatorio($arrPost)
        {
            $controllerIniciativa = new Par3_Controller_Iniciativa();
            $controllerIniciativa->listar($arrPost);
        }

        $mInto = new Par3_Model_IniciativaTiposObjeto();
        $mInta = new Par3_Model_IniciativaTiposAtendimento();
        $mCic  = new Par3_Model_CicloPar();
        $mIar  = new Par3_Model_IniciativaAreas();
        $mDms  = new Par3_Model_Dimensao();

        switch ($_REQUEST ['req']) {
            case 'salvar' :
                ob_clean();
                $resp = $controllerIniciativa->salvar($_REQUEST);
                echo simec_json_encode($resp);
                // ver($resp,simec_json_encode($resp),d);
                break;
            case 'atualizarListagem' :
                ob_clean();
                echo $controllerIniciativa->listar($_REQUEST);
                die ();
                break; 
            case 'pesquisar' :
                ob_clean();
                echo $controllerIniciativa->listar($_REQUEST);
                die ();
                break;
            case 'xls' :
                ob_clean();
                $controllerIniciativa->listar($_REQUEST);
                header("Content-Disposition: attachment; filename=Relatorio_Iniciativa.xls");
                die ();
                break;
            case 'imprimir' :
                ob_clean();
                retornaRelatorio($_REQUEST);
                // $controllerIniciativa->listaIniciativaDescricao($_REQUEST);
                die ();
                break;
            case 'remover' :
                $retorno = $controllerIniciativa->inativar($_REQUEST);
                ob_clean();
                echo $retorno;die;
                break; 
            case 'recuperar' :
                ob_clean();
                $iniciativa = $controllerIniciativa->recuperar($_REQUEST);
                $iniciativa = $iniciativa[0];
                echo simec_json_encode($iniciativa);
                die ();
                break;
            default :
                break;
        }
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade())
            $disabled = '';
        ?>
        <div class="ibox">
            <div class="ibox-title">
                <div class="row">
                    <div class="col-md-12">
                        <h3 class="center">Buscar Iniciativas</h3>
                    </div>
                </div>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <form name="form-filtro" id="form-filtro" class="form-horizontal">
                        <div class="row">
                            <div class="col-md-offset-3 col-md-6 center">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Nome da Iniciativa:</label>
                                    <div class="col-sm-8 col-md-9">
                                        <input type="text" id="inddsc" name="inddsc" maxlength="255" class="form-control"/>
                                    </div>
                                </div>
                                <!--filtros-->
                                <div class="ibox float-e-margins">
                                    <div class="ibox-content" id="avancado" style="display: none;">
                                    	<div class="form-group">
		                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
		                                    <div class="col-sm-8 col-md-9">
		                                        <input type="number" id="iniid" name="iniid" class="form-control"/>
		                                    </div>
		                                </div>
                                        <?php
                                           global $simec;
                                           echo $simec->select('intoid', 'Tipo de Objeto', '',$mInto->pegarSQLSelectCombo(), array('maxlength' => '255'), array('input-size' => '9'));
                                           echo $simec->select('dimid', 'Dimensões', '', $mDms->listarSelect(), array('maxlength' => '255'), array('input-size' => '9'));
                                           echo $simec->select('intaid', 'Tipo de Atendimento', '', $mInta->pegarSQLSelectCombo(), array('maxlength' => '255'), array('input-size' => '9'));
                                           echo $simec->select('cicid', 'Ciclo PAR', '', $mCic->pegarSQLSelectCombo(), array('maxlength' => '255'), array('input-size' => '9'));
                                           echo $simec->select('iarid', 'Áreas Relacionadas', '', $mIar->pegarSQLSelectComboDescricao(), array('maxlength' => '255'), array('input-size' => '9'));
                                           echo $simec->input('prgdsc', 'Programa', '', null, array('maxlength' => '255'), array('input-size' => '9'));
                                        ?>
                                        <div class="form-group">
                                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Situação: *</label>
                                            <div class="col-sm-9 col-md-9 col-lg-3 ">
                                                <div id="inisituacao" style="text-align: left;">
                                                    <div class="radio radio-default radio-inline ">
                                                        <input type='radio' checked name='inisituacao' id='inisituacao1' value='A'" <?php echo $iniciativa['inisituacao'] == "A" ? " checked" : ""; ?>>
                                                        <label for="inisituacao1">Ativo</label>
                                                    </div>
                                                    <div class="radio radio-default radio-inline ">
                                                        <input type="radio" name="inisituacao" id="inisituacao2" value="I" <?php echo $iniciativa['inisituacao'] == "I" ? " checked" : ""; ?>>
                                                        <label for="inisituacao2">Inativo</label>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Obra Proinfância: </label>
                                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                                <div id="inivalidapendencia" style="text-align: left;">
                                                    <div class="radio radio-default radio-inline ">
                                                        <input type='radio' checked name='iniobraproinfancia' id='iniobraproinfancia1' value='true'" <?php echo $iniciativa['iniobraproinfancia'] == "t" ? " checked" : ""; ?>>
                                                        <label for="iniobraproinfancia1">Sim</label>
                                                    </div>
                                                    <div class="radio radio-default radio-inline ">
                                                        <input type="radio" name="iniobraproinfancia" id="iniobraproinfancia2" value="false" <?php echo $iniciativa['iniobraproinfancia'] == 'f' ? " checked" : ""; ?>>
                                                        <label for="iniobraproinfancia2">Não</label>
                                                    </div>
                                                    <div class="radio radio-default radio-inline ">
                                                        <input type="radio" name="iniobraproinfancia" id="iniobraproinfancia2" value="" <?php echo $iniciativa['iniobraproinfancia'] == '' ? " checked" : ""; ?>>
                                                        <label for="iniobraproinfancia2">Todos</label>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="ibox-title">
                                        <div class="ibox-tools">
                                            <a id="btn-avancado"> Opções Avançadas <i id="chevron" class="fa fa-chevron-down"></i></a>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </form>
                </div>
                <div class="ibox-footer">
                    <div class="center">
                        <button type="button" class="btn btn-primary" id="pesquisar-iniciativa"><i class="fa fa-search"></i> Pesquisar</button>
                        <a href="par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativas">
                            <button type="submit" class="btn btn-success" id="btn-incluir"><i class="fa fa-plus"></i> Incluir</button>
                        </a>
                        <button type="submit" class="btn btn-primary xls" style="width: 110px;"><i class="fa fa-file-excel-o"></i> XLS</button>
                        <button type="button" class="btn btn-warning" style="width: 110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i>Imprimir</button>
						|
		                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar </button>
                    </div>
                </div>
            </div>
            <div class="ibox-content">
                <div id="iniciativaListagem">
                    <?php
                   	$controllerIniciativa->listar($_REQUEST);
                    ?>
                </div>
            </div>
        </div>

        <div id="div-impressao" style="display: none">
            <div id="div_cabecalho" class="row col-lg-offset-2">
                <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align: center;">
                    <tr bgcolor="#ffffff">
                        <td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
                        <td nowrap align="left" valign="middle" height="1" style="padding: 5px 0 0 0;">MEC/FNDE <br> Relatório de Iniciativas<br></td>
                        <td align="right" valign="middle" height="1" style="padding: 5px 0 0 0;">Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/></td>
                    </tr>
                </table>
                <hr style="color: black; width: 95%; margin-left: -18px;">
            </div>
            <div class="clearfix" style="margin-bottom: 10px;"></div>
            <div id="impressao-content"></div>
        </div>
        <br>
        <!--modal-->
        <div class="ibox float-e-margins animated modal" id="modal_iniciativa"
             role="dialog" aria-hidden="true">
            <div class="modal-dialog">
                <form method="post" name="formIniciativa" id="formIniciativa"
                      class="form form-horizontal">
                    <div class="modal-content">
                        <div class="ibox-title">
                            <button type="button" class="close" data-dismiss="modal"
                                    aria-label="Close">
                                <span aria-hidden="true">&times;</span>
                            </button>
                            <h3 id="modal_titulo">Incluir - Iniciativa</h3>
                        </div>
                        <div class="ibox-content" id="conteudo-modal">
<!--                            --><?php //echo $controllerIniciativa->formIniciativa($_REQUEST); ?>
                        </div>
                        <div class="ibox-footer">
                            <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                                <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default"
                                    <?php echo $disabled; ?>>
                                    <i class="fa fa-times-circle-o"></i> Cancelar
                                </button>
                                <button type="submit" id="salvarDescIniciativa" class="btn btn-success" <?php echo $disabled; ?>><i class="fa fa-plus-square-o"></i> Salvar</button>
                            </div>
                        </div>
                    </div>
                </form>
            </div>
        </div>

        <script>

	        jQuery("#limpar").click(function(){
	            $('#inddsc').val('');
	            $('#iniid').val('');				
            	$("#intoid").val('').trigger('chosen:updated');
            	$("#dimid").val('').trigger('chosen:updated');
            	$("#intaid").val('').trigger('chosen:updated');
            	$("#cicid").val('').trigger('chosen:updated');
            	$("#cicid").val('').trigger('chosen:updated');
            	$("#iarid").val('').trigger('chosen:updated');
            	$("#prgdsc").val('');
            	$("#form-filtro").find('#inisituacao3').prop('checked',true);
                $("#pesquisar-iniciativa").click();
	        });
        
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

            function editarIniciativa(id) {
                var caminho = 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativas&iniid=' +id;
                window.location.href = caminho;
            }

            function removerIniciativa(id) {
                var caminho = window.location.href;
                var action = '&req=recuperar&iniid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: true,
                    success: function (resp) {
                        var ini = $.parseJSON(resp);
                        var confirmado = false;
                        swal({
                            title: "Remover Iniciativa",
                            text: "Tem certeza que deseja remover: <b>" + ini.codigo + " - " + ini.descricao + "</b> ?",
                            html: true,
                            type: "warning",
                            showCancelButton: true,
                            cancelButtonText: "Não",
                            confirmButtonText: "Sim",
                            closeOnConfirm: false
                        }, function (isConfirm) {
                            if (isConfirm) {
                                var caminho = window.location.href;
                                var action = '&req=remover&iniid=' + id;
                                $.ajax({
                                    type: "POST",
                                    url: caminho,
                                    data: action,
                                    async: false,
                                    success: function (resp) {
                                        if (!isNaN(parseInt(resp)) && isFinite(resp)) {
                                            msgSuccess(caminho,"Iniciativa removida com sucesso");
                                            return false;
                                        }
                                        swal("Erro.",resp, "error");
                                        return false;
                                    }
                                });
                            }
                        })
                    }
                });
            }

            $(".xls").on("click", function () {
                window.location.assign(window.location.href + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
            });

            $("#imprimir").on("click", function () {
                var caminho = window.location.href;
                var action = '&req=imprimir&' + $("#form-filtro").serialize() + '&' + $('.form-listagem').serialize();
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                    	//console.log('resp-impressao:'); console.log(resp);
                        $("#impressao-content").html(resp);
                        var container = $("#impressao-content");
                        var table = container.find('table');
                        var th = container.find('th');
                        var td = container.find('td');
                        var tr = container.find('tr');
                        var penultimoTr = tr.eq(tr.length - 2);
                        var ultimoTr = tr.eq(tr.length - 1);
                     	// Deixa a penúltima linha como totalizador pois foi inserido manualmente para informar corretamente o valor.
                        penultimoTr.find('td').attr('colspan',10);
                        // Remove o totalizador original pois informa total incorreto.
                        ultimoTr.remove();
                        //table.css('max-width', '50px');
                        table.css('width', '100%');
                        table.css('border-collapse', 'collapse');
                        container.find(".popover,button,.modal").remove();
                        container.find("table,td,th").css('border', '1px solid black');
                        container.find('tr:even').css('background-color', 'silver');
                        th.css('height', '50px');
                        td.css('text-align', 'center');
						// Faz o processo de remover contornos das células sem valores
						count = 1;
                        $.each(tr, function(i, v){
                            tdAtual = $(v).find('td');
                            switch(count){
                            	case 1:
                                	tdAtual.css('font-weight', 'bold');
                                	break;
                            	case 3:
                            		tdAtual.css('font-weight', 'bold');
                            		tdAtual.eq(0).css('border', 'none');
                            		tdAtual.eq(1).css('border', 'none');
                        	   		break;
                        		case 4:
                        			tdAtual.eq(0).css('border', 'none');
                        			tdAtual.eq(1).css('border', 'none');
									break;
                        		case 5:
                        			tdAtual.css('font-weight', 'bold');
                        			tdAtual.eq(0).css('border', 'none');
                        			tdAtual.eq(1).css('border', 'none');
                        			tdAtual.eq(5).attr('colspan', 2);
									break;
                        		case 6:
                        			tdAtual.eq(0).css('border', 'none');
                        			tdAtual.eq(1).css('border', 'none');
                        			tdAtual.eq(5).attr('colspan', 2);
                        			$(v).css('height', '25px');
                        			$(v).append('<tr style="height:30px"><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>');
									break;
                            }

                            count++;

                            if(count == 7){
                                count = 1;
                            }
                        });

                        var w = window.open();
                        w.document.write($('#div-impressao').html());
                        w.document.title = 'Relatório - Iniciativas';
                        w.print();
                        w.close();
                        $("#impressao-content").html("");
                        atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                        return true;
                    }
                });
            });

            function atualizarListagem(params ="") {
                if (params) {
                    params = '&' + params
                }
                var caminho = window.location.href;
                var action = '&req=atualizarListagem' + params;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        $("#iniciativaListagem").html(resp);
                        return true;
                    }
                });
            }

            $("#pesquisar-iniciativa").click(function (evt) {
                console.log($("#form-filtro").serialize());
                evt.preventDefault();
                var caminho = window.location.href;
                var action = '&req=pesquisar&' + $("#form-filtro").serialize();
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        $("#iniciativaListagem").html(resp);
                        return true;
                    }
                });
            });

            function atualizarForm(params = "") {
                if (params) {
                    params = '&' + params
                }

                var caminho = window.location.href;
                var action = '&req=atualizarForm' + params;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        $("#conteudo-modal").html(resp);
                        return true;
                    }
                });
            }

            function atualizarFormFiltro() {
                $("#form-filtro").find('[name=inddsc]').val("");
                $("#form-filtro").find('[name=indid]').val("");
                $("#form-filtro").find('#indsituacao1').removeAttr("checked");
                $("#form-filtro").find('#indsituacao2').removeAttr("checked");
                $("#form-filtro").find('#indsituacao3').prop('checked', true);
            }
        </script>
        </div>
    </div>