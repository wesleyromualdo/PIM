<?php
/**
 * Criação de rodapé do SiMEC.
 *
 * Escolhe o layout correto do rodapé com base nas configurações atuais do módulo carregado.
 * @package Simec\Core
 */ 

if (!empty($dadosLayout['version_layout']))
{

    include_once 'rodape_' . $dadosLayout['version_layout'];
}

else if ( in_array($_SESSION['sisid'], array('283','284' )) )
{
    include_once 'rodape_bootstrap_v4.inc';
}
else if ($_SESSION['sislayoutbootstrap'] == 't')
{
    include_once 'rodape_bootstrap.inc';
}
else if ($_SESSION['sislayoutbootstrap'] == 'zimec')
{
   	include_once 'rodape_bootstrap_v3.inc';
}
else
{
	if ($cabecalho_painel==true){
		include APPRAIZ . '/www/painel/novo/rodape_painel.php';
		exit();
	}

$linkmanual = montaLinkManual( $_SESSION['sisid'] );

// -- Caso a página requisitada seja uma página existente, realiza o log de estatísticas - Verificação necessária pois
// -- ainda não foi possível reproduzir o erro no sistema financeiro que faz com que todas as imagens do tema do sistema
// -- sejam requeridas pelo browers como um módulo. Esta mesma verificação ? feita no controleAcesso no momento de
// -- incluir os arquivos.
if (file_exists(realpath(APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc"))) {
    if (isset($_SESSION['mnuid']) && !empty($_SESSION['mnuid'])) {
        simec_gravar_estatistica();
    }
}
?>
<? if ($cabecalho_sistema<>"") : ?>
					</td>
				</tr>
			</table>
		</td>
		</tr>
			<tr>
				<td>

					<?php include_once APPRAIZ . "includes/estouvivo.php"; ?>

					<script type="text/javascript" language="javascript">
						function abrirUsuariosOnline()
						{
							window.open(
								'../geral/usuarios_online.php',
								'usuariosonline',
								'height=500,width=600,scrollbars=yes,top=50,left=200'
							);
						}
						</script>

				</td>
			</tr>
		</table>

			<table class="rodape notprint" align='center' border="0" cellspacing="0" cellpadding="2" class="notprint" style="width:100%; height: 45px; color: #667; background-color: #000;">
						<tr>
							<td colspan="2" height="2"></td>
						</tr>
						<tr>
							<td align="left" height="15">
								Data: <?= date("d/m/Y - H:i:s") ?> / Último acesso (<?= formata_data( $_SESSION['usuacesso'] ); ?>) -
								<a href="javascript:abrirUsuariosOnline();" >
									<span id="rdpUsuariosOnLine"><?= $_SESSION['qtdusuariosonline'][$_SESSION['sisid']];?></span>
									Usuários On-Line
								</a>
							</td>
							<?if( $db->testa_superuser() ): ?>
							<td style="text-align: left"><a href="javascript: abrirRegras( <?=$_SESSION['mnuid']; ?> );">Visualizar Regras</a></td>
							<?endif; ?>
							<td align="right" height="15">
                                    <!-- Caso sistema seja PAR 2011 ou Obras botão abrir? o fale conosco do PAR3-->
                                    <?php if($_SESSION['sisid'] == 23 ||
                                                $_SESSION['sisid'] == 147 ||
                                                $_SESSION['sisid'] == 13 ||
                                                $_SESSION['sisid'] == 57){

                                        switch ($_SESSION['sisid']){
                                            case 23:
                                                $id = $_SESSION['par']['inuid'];
                                                break;
                                            case 13:
                                                $id = $_SESSION['inuid'];
                                                break;
                                            case 147:
                                                $id = $_GET['obrid'];
                                                break;
                                            case 57:
                                                $id = '';
                                                break;
                                        }


//                                        //caso sistema seja par passa o inuid
//                                        if($_SESSION['sisid'] == 23){
//                                            $id = $_SESSION['par']['inuid'];
//                                        }
//                                        // caso sistema seja Plano de Metas passa o inuid
//                                        elseif ($_SESSION['sisid'] == 13){
//                                            $id = $_SESSION['inuid'];
//                                        }
//                                        // caso sistema seja Obras2 passa o obrid
//                                        else{
//                                            $id = $_GET['obrid'];
//                                        }

                                        ?>
                                        SIMEC | &nbsp;&nbsp;&nbsp; <a href="javascript:abrirFaleConosco(<?= $_SESSION['sisid'] .",". $id;?>);">Fale Conosco</a>
									<?php }
									else{?>
                                        SIMEC | &nbsp;&nbsp; <a href="javascript:janela('/geral/fale_conosco.php?uc=<?= $_SESSION['usucpf']; ?>',550,600)">Fale Conosco</a>
                                    <?php }?>
									 &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp; <?= $linkmanual; ?> | &nbsp;&nbsp; Tx<?php echo $cache_sistema ?>.: <?= number_format( ( getmicrotime() - $Tinicio ), 4, ',', '.' ); ?>s

							</td>
						</tr>
                        <?php if (1 == $_SESSION['superuser']): ?>
                        <tr>
                            <td colspan="3">
                                Revisão: <?php require_once APPRAIZ . '/version.txt'; ?>
                            </td>
                        </tr>
                        <?php endif; ?>
					</table>

		<div id="avisochat">
			<div style="padding: 0px; width: 110px; padding: 0; margin: 0; height: 100px; overflow: none; position-y: absolute;display:none;" id="avisochat_lista">
				<table border="0" cellpadding="2" cellspacing="2">
				<tr>
					<td  align="left" width="110" bgcolor="#FFF7AF" style="border-top: 1px solid #808080; border-right: 1px solid #808080;border-bottom: 1px solid #808080;">
						<span onclick="avisoChatMostrarEsconder();"><img src="/imagens/balaochat.gif" border="0" align="absmiddle"><font color="#000000"><strong>&nbsp;Mensagens</strong></font><br></span>
						<div style="overflow: auto; position-y: absolute;border-top: 1px solid #808080; height: 100px;">
							<table id="avisochat_tabela" width="100%" cellpadding="0" cellspacing="0"></table>
						</div>
					</td>
				</tr>
				</table>
			</div>
		</div>

		<?php if ($_SESSION['sissnalertaajuda'] == 't'): ?>
			
			<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>
			<script type="text/javascript" language="javascript">
					if (!$)
						$ = $1_11;
					

					$(document).ready(function() {
						if ($().bt) {
							$('#bt_help_sistema').bt('<span style="color:red;font-weight:bold;">Duvidas?</span><br>Consulte as informações de ajuda para esta tela.',
							  {
							    fill: '#FFF',
							    cornerRadius: 10,
							    strokeWidth: 0,
							    shadow: true,
							    shadowOffsetX: 3,
							    shadowOffsetY: 3,
							    shadowBlur: 8,
							    shadowColor: 'rgba(0,0,0,.9)',
							    shadowOverlap: false,
							    noShadowOpts: {strokeStyle: '#999', strokeWidth: 2},
							    positions: ['left']
							  });
							$('#bt_help_sistema').btOn();
							window.setTimeout("$('#bt_help_sistema').btOff()", 10000);
						}
					});
			</script>
		<?php endif; ?>

		<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
	</body>
    </html>
    <script language="JavaScript">
		if(typeof $ != 'undefined' && typeof $() != 'undefined'){
	        /** Função de fazer com que o sistema informe que esta havendo uma requisição ajax */
	        $(document).ajaxSend(function (e, jqxhr, settings) {
	            $("body").prepend('<div class="ajaxCarregando"></div>');
	            $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();

	        }).ajaxStop(function(){
	                $(".ajaxCarregando").fadeOut().remove();
	            });

	        /** Mensagem de carregando quando houver requisições em ajax */
	        $.ajaxSetup({
	            timeout: 0,
	            error: function(xhr, status, error) {
	                console.log("Ocorrência de erro no retorno AJAX: " + status + "\nError: " + error);
	                $(".ajaxCarregando").fadeOut().remove();

	                $("body").prepend('<div class="ajaxCarregandoErro"></div>');
	                $(".ajaxCarregandoErro").hide().html('Erro ao carregar...').fadeIn();

	                setTimeout(function(){$(".ajaxCarregandoErro").fadeIn().remove();}, 1300);
	            }
	        });
		}
        function abrirFaleConosco(sisid, id)
        {
            if(sisid == 23){
                window.open(
                    '../geral/fale_conosco_par.php?sisid=par&inuid='+id,
                    'faleconoosco',
                    'height=515,width=900,scrollbars=yes,top=50,left=200'
                );
            }
            else if (sisid == 147){
                window.open(
                    '../geral/fale_conosco_par.php?sisid=obras&obrid='+id,
                    'faleconoosco',
                    'height=515,width=900,scrollbars=yes,top=50,left=200'
                );
            }
            else if (sisid == 13){
                window.open(
                    '../geral/fale_conosco_par.php?sisid=cte&inuid='+id,
                    'faleconoosco',
                    'height=515,width=900,scrollbars=yes,top=50,left=200'
                );
            }
            else if (sisid == 57){
                window.open(
                    '../geral/fale_conosco_par.php?sisid=emendas&inuid='+id,
                    'faleconoosco',
                    'height=515,width=900,scrollbars=yes,top=50,left=200'
                );
            }
        }
    </script>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
	<? if ( $ajustaTela ) : ?>
		<script type="text/javascript" language="javascript">
			window.scrollBy( 0, 112 );
		</script>
	<? endif; ?>
<?php endif; ?>
<?php } ?>
<?php
if ($acessoRapidoCabecalhoIncluido) {
    $acessoRapidoControlador = AcessoRapido\Controlador::pegarInstancia();
    if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
        echo $acessoRapidoControlador->montarCaixaBootstrap();
    } else {
        echo $acessoRapidoControlador->montarCaixa();
    }
}



?>
<?php $db->close(); ?>
