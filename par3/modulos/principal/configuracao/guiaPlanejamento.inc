<?php

require_once APPRAIZ . 'includes/cabecalho.inc';

echo $simec->title ( "Guia Planejamento" );
?>
<script src="js/par3.js"></script>
<input type="hidden" id="itrid" name="itrid"
	value="<?php echo $itrid; ?>">
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
    	<?php
					$url = 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A';
					$abasGuias = array (
							0 => array (
									"descricao" => "Guia Diagnóstico",
									"link" => "par3.php?modulo=principal/configuracao/guia&acao=A"
							),
							1 => array (
									"descricao" => "Guia Planejamento",
									"link" => "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A"
							)
					);
					echo $simec->tab ( $abasGuias, $url );

					$url = 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A';
					$iniid = $_REQUEST['iniid']?"&iniid={$_REQUEST['iniid']}":'';
					$abasIniciativas = array (
							0 => array (
									"descricao" => "Buscar Iniciativas",
									"link" => $url."&guia=listaIniciativas"
							)
					);
					if($_REQUEST['guia'] && $_REQUEST['guia'] != 'listaIniciativas'){
						array_push($abasIniciativas, array (
                    									"descricao" => "Incluir Iniciativa",
                    									"link" => $url."&guia=cadastroIniciativas".$iniid
                        							)
						);
					}
					if($_REQUEST['iniid']){
						array_push($abasIniciativas, array (
                                                        "descricao" => "Iniciativa Detalhe",
                                                        "link" => $url."&guia=cadastroIniciativaDetalhe".$iniid
                                                    )
						);
						array_push($abasIniciativas, array (
                                                        "descricao" => "Iniciativa - Itens de Composição",
                                                        "link" => $url."&guia=cadastroIniciativaItensComposicao".$iniid
                                                    )
						);
 						array_push($abasIniciativas, array (
                                                         "descricao" => "Informações Demográficas e Educacionais",
                                                         "link" => "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=informacaoDemograficaEducacional".$iniid
                                                     )
 						);
					}
					echo '<br>';
//					$url = $_REQUEST ['itrid'] ? 'par3.php?modulo=principal/configuracao/guia&acao=A&itrid=' . $_REQUEST ['itrid'] : 'par3.php?modulo=principal/configuracao/guia&acao=A&itrid=1';
					$_REQUEST['guia'] =  $_REQUEST['guia'] ?  $_REQUEST['guia'] : 'listaIniciativas';
					echo $simec->tab( $abasIniciativas, $url . '&guia=' . $_REQUEST['guia'].$iniid );

					switch($_REQUEST['guia']){
						case 'listaIniciativas':
							require_once('listaIniciativas.inc');
							break;
						case 'cadastroIniciativas':
							require_once('cadastroIniciativas.inc');
							break;
						case 'cadastroIniciativaDetalhe':
							require_once('cadastroIniciativaDetalhe.inc');
							break;
                        case 'cadastroIniciativaItensComposicao':
                            require_once('cadastroIniciativaItensComposicao.inc');
                            break;
                        case 'informacaoDemograficaEducacional':
                            require_once('informacaoDemograficaEducacional.inc');
                            break;
                        default:
                        	// Se não identificar nenhuma aba, direciona para a primeira aba (lista de iniciativas).
							if($_SERVER['REQUEST_URI'] == '/par3/par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A'){
								header('location:par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=listaIniciativas');
							}
							break;
					}
					?>
        <div class="ibox float-e-margins">

		</div>
	</div>
</div>

<div id="modal-form" class="modal fade" aria-hidden="true">
	<div class="modal-dialog">
		<div class="ibox-title">
			<h5 id="html_modal-title"></h5>
			<button type="button" class="close" data-dismiss="modal"
				aria-label="Close">
				<span aria-hidden="true">&times;</span>
			</button>
		</div>
		<div id="html_modal-form"></div>
	</div>

	<div id="modal-form-large" class="modal fade" aria-hidden="true">
		<div class="ibox-title">
			<h5 id="html_modal-title-large"></h5>
			<button type="button" class="close" data-dismiss="modal"
				aria-label="Close">
				<span aria-hidden="true">&times;</span>
			</button>
		</div>
		<div id="html_modal-form-large"></div>
	</div>
</div>

<script>
    function popupGuia(acao, tipo, id, itrid) {
        var titulo = 'Gerenciar dimensão';

        switch (tipo) {
            case 'area':
                titulo = 'Gerenciar área';
                break;
            case 'indicador':
                titulo = 'Gerenciar indicador';
                break;
            case 'criterio':
                titulo = 'Gerenciar componentes do indicador';
                break;
        }

        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/configuracao/popupGuia&acao=A',
            data: '&acaoGuia=' + acao + '&tipoGuia=' + tipo + '&codigo=' + id + '&itrid=' + itrid,
            async: false,
            success: function (resp) {
                $('#html_modal-form').html(resp);
                $('#html_modal-title').html(titulo);
                $('#modal-form').modal();
            }
        });
    }
</script>