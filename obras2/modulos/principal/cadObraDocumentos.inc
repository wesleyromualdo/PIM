<?php 
verificaSessao( 'obra' );

require APPRAIZ . 'obras2/includes/principal/cadObraDocumentos/ctrl.php';


echo cabecalhoObra($obrid);
echo "<br>";

monta_titulo_obras( 'Documentos da Obra', '<div class="row" style="background: #b94a48; color: #fff"><img src="/imagens/atencao.png" /> 
                                                    Insira documentos em formato .pdf, .docx, Documentos não assinados não tem validade.</div>' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form method="post" name="formObraArquivos" id="formObraArquivos" enctype="multipart/form-data">
	<input type="hidden" name="req"  	id="req"	value="" />
	<input type="hidden" name="arqid" 	id="arqid"	value="" />
	<input type="hidden" name="oarid" 	id="oarid"	value="" />
	<input type="hidden" name="demid" value="<?=$_SESSION['obras2']['demid']?>" />
	<?php if( $habilitado ): ?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td class="SubTituloDireita" width="20%">Descrição:</td>
                        <td><?php echo campo_texto('oardesc','S',$habilita,'',43,100,'','', '', '', '', 'id="oardesc"', '');?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tipo:</td>
                        <td>
                        <?php
                            $tipoArquivo = new TipoArquivo();
                            $sql = $tipoArquivo->listaCombo();
                            $db->monta_combo("tpaid", $sql, 'S', "Selecione...", "", '', '', '', 'S', 'tpaid');
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arquivo" id="arquivo" class="obrigatorio"/>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        </td>
                    </tr>
                    <tr class="divTituloPadrao">
                        <td colspan="3" align="center">
                            <?php if( $habilitado ){ ?>
                            <input type="button" name="salvar" class="incluir" value="Anexar Arquivo"/>
                            <?php }?>
                        </td>
                    </tr>
		</table>
	<?php endif; ?>
</form>

<?php
        $objObras = new Obras();
        $objObras->carregarPorIdCache($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();

        require APPRAIZ . 'obras2/includes/principal/cadObraDocumentos/listagem.php';
        require APPRAIZ . 'obras2/includes/principal/cadObraDocumentos/js.php';

?>

