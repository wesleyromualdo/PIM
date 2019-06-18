<?php 
verificaSessao( 'obra' );
require APPRAIZ . 'obras2/includes/principal/cadObraFotos/ctrl.php';
require APPRAIZ . 'obras2/includes/principal/cadObraFotos/css.php';

?>

<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form method="post" name="formObraArquivos" id="formObraArquivos" enctype="multipart/form-data">
	<input type="hidden" name="req"  	id="req"	value="" />
	<input type="hidden" name="arqid" 	id="arqid"	value="" />
	<input type="hidden" name="oarid"	id="oarid"	value="" />
	<input type="hidden" name="demid" 	value="<?=$_SESSION['obras2']['demid']?>" />
<?php 
	if( $habilitado ): 
?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="20%">Descrição:</td>
			<td><?=campo_texto('oardesc','S',$habilita,'',43,100,'','', '', '', '', 'id="oardesc"', '');?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data da Foto:</td>
			<td><?=campo_data2('oardata','S',$habilita,'','##/##/####','','', '','', '', 'oardata' );?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Arquivo (foto):
			</td>
			<td>
				<input type="file" name="arquivo" id="arquivo" class="obrigatorio"/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
			</td>
			<td>
				<span style="color:red">O tamanho máximo para upload de arquivos é de 10Mb</span>
			</td>
		</tr>
		<tr class="divTituloPadrao">
			<td colspan="3" align="center">
				<input type="button" name="salvar" class="incluir" value="Anexar Foto"/>
			</td>
		</tr>
	</table>
<?php
	endif;
?>
</form>

<?php
        $objObras = new Obras();
        $objObras->carregarPorIdCache($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formObraArquivos'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                   }
                 ";
            echo '</script>';
        }
?>

<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>

		<td colspan="2" class="SubTituloCentro">
			<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
			<script type="text/javascript" src="../includes/remedial.js"></script>
			<script type="text/javascript" src="../includes/superTitle.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
			<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
			
			<?
			$arParam = array('obrid' => $_SESSION['obras2']['obrid']);
			$sql = $obrasArquivos->listaFotosSQL( $arParam );
			$fotos = ($db->carregar($sql));				

			if( $fotos ){

				for( $k=0; $k < count($fotos); $k++ ){

					$obraArquivo = new ObrasArquivos();
					$arDados = $obraArquivo->buscaDadosPorArqid( $fotos[$k]["arqid"] );
					
					$pathObras 	= APPRAIZ."arquivos/obras/".floor($fotos[$k]["arqid"]/1000)."/";
					$pathObras2 = APPRAIZ."arquivos/obras2/".floor($fotos[$k]["arqid"]/1000)."/";
					
					if( $_SESSION['usucpf'] == '' ){
						if( !is_file($pathObras2.$fotos[$k]["arqid"])
							&&  is_file($pathObras.$fotos[$k]["arqid"]) ){
							if( !is_dir($pathObras2) ){
								mkdir($pathObras2, 0777);
							}
							link( $pathObras.$fotos[$k]["arqid"], $pathObras2.$fotos[$k]["arqid"]);
						}
					}
					
					$divExcluir = "";
					
					if( $habilitado && $blockEdicao == false){
						$divExcluir = "<div class=\"excluir\" id=\"".$fotos[$k]["oarid"]."\" style=\"float:right;\">
				    						<img style=\"cursor:pointer; position:relative; 
				    									 z-index:10; top:0px; left:0px; \" 
				    							 width=\"20px\" src=\"../imagens/excluir_2.gif\" border=0 title=\"Excluir\" />
				    					</div>";
					}
					
					echo "<div class=\"div\" style=\"float:left; width:100px; height:122px; text-align:center; margin:10px;\" >
							<img width='64' height='64' title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' 
								 src='../slideshow/slideshow/verimagem.php?arqid=".$fotos[$k]["arqid"]."&_sisarquivo=" . $eschema . "' 
								 hspace='0' vspace='0' style='position:relative; z-index:5; float:left; width:97px; height:97px;cursor:pointer;' 
								 onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" 
								 onclick=\"abrirGaleria(".$fotos[$k]["arqid"].")\"/> 
							<br>
							<div class=\"download\" id=\"".$fotos[$k]["arqid"]."\" style=\"float:left;margin-top:2px;\">
	    						<img style=\"cursor:pointer; position:relative; 
	    									 z-index:10; top:0px; left:0px; float:left;\" 
	    							 src=\"../imagens/salvar.png\" border=0 title=\"Doanload da Imagem\" />
	    					</div>
	    					<div style=\"float:left;margin-left:7px;margin-top:7px;\">
							" . $fotos[$k]["data"] . " 
							</div>
	    					$divExcluir
						  </div>";
					
				}
				
			}else {
				echo "Não existem fotos cadastradas";
			}
			?>
		</td>
	</tr>
</table>

<?php  
	require APPRAIZ . 'obras2/includes/principal/cadObraFotos/js.php';
?>