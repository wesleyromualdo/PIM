<?php 
require APPRAIZ . 'obras2/includes/principal/inauguracao/ctrl.php';

?>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet">
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>
$(function(){
    $('#capacidade').keyup();
    $('#investimentototal').keyup();
    $('#alunosmatriculados').keyup();
    $('#quantidadehabitantes').keyup();
    $('#distancia').keyup();
    
    $('#total_file_obra_funcionamento').val();
    $('#total_file_obra_inauguracao').val();
    
    $('#adicionar_anexo_obra_concluida').click(function (e) {
        if($('#total_file_obra_funcionamento').val() < 9){
            $('#total_file_obra_funcionamento').val(parseInt($('#total_file_obra_funcionamento').val()) + 1);
            $('#table_anexos_obra_concluida').append($('<tr class="anexos anexos-base-obr-concluida"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_obra_concluida" href="">Excluir</a></span></td></td><td class="SubTituloEsquerda"><td class="SubTituloEsquerda"><input onchange="validarEscolhaArquivo(this)" <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_obra_concluida[]" id="arquivo_obr_concluida"/></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> maxlength="255" size="" type="text" name="arquivo_descricao_obra_concluida[]" id="arquivo_descricao_obr_concluida"/></td></tr>').removeClass('anexos-base-obr-concluida'));
            e.preventDefault();
        } else {
            alert('Limite de 10 arquivos simultâneos!');
            return false;
        }
    });
    
    $('#adicionar_anexo_inauguracao').click(function (e) {
        if($('#total_file_obra_inauguracao').val() < 9){
            $('#total_file_obra_inauguracao').val(parseInt($('#total_file_obra_inauguracao').val()) + 1);
            $('#table_anexos_inauguracao').append($('<tr class="anexos anexos-base-inauguracao"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_inauguracao" href="">Excluir</a></span></td><td class="SubTituloEsquerda"></td><td class="SubTituloEsquerda"><input onchange="validarEscolhaArquivo(this)" <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_inauguracao[]" id="arquivo_inauguracao"/></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> maxlength="255" size="" type="text" name="arquivo_descricao_inauguracao[]" id="arquivo_descricao_inauguracao"/></td></tr>').removeClass('anexos-base-inauguracao'));
            e.preventDefault();
        } else {
            alert('Limite de 10 arquivos simultâneos!');
            return false;
        }
    });
    
    $('.excluir_anexo_obra_concluida').live('click',function (e) {
        $('#total_file_obra_funcionamento').val(( parseInt( $('#total_file_obra_funcionamento').val() ) - 1));
        $(this).parents('tr.anexos').remove();
        e.preventDefault();
    });
    
    $('.excluir_anexo_inauguracao').live('click',function (e) {
        $('#total_file_obra_inauguracao').val(( parseInt( $('#total_file_obra_inauguracao').val() ) - 1));
        $(this).parents('tr.anexos').remove();
        e.preventDefault();
    });
});

function validarFormatoArquivo(arquivo) {
    if ( /\.(jpe?g|png|gif|bmp)$/i.test(arquivo.files[0].name) === false ) { 
        alert("Somente imagens são permitidas!"); 
        arquivo.value = null;
        return false;
    }
}

function validarEscolhaArquivo(arquivo){
    
    validarFormatoArquivo(arquivo);
    
    if(arquivo.files[0] && arquivo.files[0].size > 4194304){     
        alert('O tamanho máximo do arquivo deve ser de 4 MB.');
        arquivo.value = null;
        return false;
    }
}
</script>
<form method="post" action="" name="form_inauguracao" id="form_inauguracao" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" id="requisicao" value="salvar_inauguracao">
    <input type="hidden" name="obrid" id="obrid" value="<?php echo $obrid; ?>">
    <input type="hidden" name="iobid" id="iobid" value="<?php echo $arrDadosInauguracao['iobid']?>">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtitulodireita" width="250px">Capacidade de atendimento:</td>
            <td>
                <?php echo campo_texto('capacidade', 'S', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="capacidade"','',$arrDadosInauguracao['capacidade'], '', array('width' => '30%')); ?>
            </td>
        </tr>

        <tr>
            <td class="subtitulodireita" width="250px">Investimento Total:</td>
            <td>
                <?php echo campo_texto('investimentototal', 'S', $habil, '', 20, 20, '[.###],##', '', 'right', '', 0, 'id="investimentototal"','',$arrDadosInauguracao['investimentototal'], '', array('width' => '30%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Início de Funcionamento:</td>
            <td>
                <?php echo campo_data2('dtiniciofuncionamento', 'S', $habil, '', 'S', '', ' ', $arrDadosInauguracao['dtiniciofuncionamento'], ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Previsão de Inauguração:</td>
            <td>
                <?php echo campo_data2('dtprevisaoinauguracao', 'S', $habil, '', 'S', '', ' ', $arrDadosInauguracao['dtprevisaoinauguracao'], ''); ?>
            </td>
        </tr>
        <tr class="justificativaprevisao" style="display:none">
            <td class="subtitulodireita" width="250px">Justificativa:</td>
            <td>
                <?=campo_textarea('justificativaprevisao', 'S', $habil, '', 50, 4, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $arrDadosInauguracao['justificativaprevisao'], '100%', 'justificativaprevisao'); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Alunos Matriculados:</td>
            <td>
                <?php echo campo_texto('alunosmatriculados', 'S', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="alunosmatriculados"','',$arrDadosInauguracao['alunosmatriculados'], '', array('width' => '30%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Possuem profissionais contratados:</td>
            <td>
                <input type="radio" name="statuspc" id="statuspc" <?=!$habilitado ? 'disabled' : '' ?> value="S" <?=$arrDadosInauguracao['statuspc'] && $arrDadosInauguracao['statuspc']=='S' ? 'checked' : ''?> /> Sim &nbsp;
                <input type="radio" name="statuspc" id="statuspc" <?=!$habilitado ? 'disabled' : '' ?> value="N" <?=$arrDadosInauguracao['statuspc'] && $arrDadosInauguracao['statuspc']=='S' ? '' : 'checked'?> /> Não
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Quantidade de habitantes no município:</td>
            <td>
                <?php echo campo_texto('quantidadehabitantes', 'S', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="quantidadehabitantes"','',$arrDadosInauguracao['quantidadehabitantes'], '', array('width' => '30%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Distância entre capital e município:</td>
            <td>
                <?php echo campo_texto('distancia', 'S', $habil, '', 20, 20, '[.###]', '', 'right', '', 0, 'id="distancia"','', (int) $arrDadosInauguracao['distancia'], '', array('width' => '30%')); ?> KM
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Aeroportos mais próximos:</td>
            <td>
                <?php echo campo_texto('aeroportos', 'S', $habil, '', 10, 250, '', '', 'left', '', 0, 'id="aeroportos"','',$arrDadosInauguracao['aeroportos'], '', array('width' => '100%')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Outras informações:</td>
            <td>
                <?php echo campo_textarea( 'outras', 'S', $habil, '', 50, 4, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $arrDadosInauguracao['outras'], '100%', 'outras', $opcoes = null); ?>
            </td>
        </tr>

        <?php $empreendimento = new Empreendimento($obra->empid); ?>
        <tr>
            <td class="subtitulodireita" width="250px">Os recursos do mobiliário já foram repassados?</td>
            <td>
            <?php $arrDadosInauguracao['recursos_repassados'] = ($arrDadosInauguracao['recursos_repassados']) ? $arrDadosInauguracao['recursos_repassados'] : 'P';?>
                <input type="radio" name="recursos_repassados" id="recursos_repassados" <?=!$habilitado ? 'disabled' : '' ?> value="S" <?=$arrDadosInauguracao['recursos_repassados']=='S' ? 'checked' : ''?> /> Sim &nbsp;
                <input type="radio" name="recursos_repassados" id="recursos_repassados" <?=!$habilitado ? 'disabled' : '' ?> value="N" <?=$arrDadosInauguracao['recursos_repassados']=='N' ? 'checked' : ''?> /> Não
                <input type="radio" name="recursos_repassados" id="recursos_repassados" <?=!$habilitado ? 'disabled' : '' ?> value="P" <?=$arrDadosInauguracao['recursos_repassados']=='P' ? 'checked' : ''?> /> Não se aplica
            </td>
        </tr>
		
        <tr>
            <td class="subtitulodireita" width="250px">O mobiliário já foi adquirido?</td>
            <td>
            	<?php $arrDadosInauguracao['mobiliario_adquirido'] = ($arrDadosInauguracao['mobiliario_adquirido']) ? $arrDadosInauguracao['mobiliario_adquirido'] : 'P';?>
                <input type="radio" onclick="alteraMobAdquirido(this.value);" name="mobiliario_adquirido" id="mobiliario_adquirido" <?=!$habilitado ? 'disabled' : '' ?> value="S" <?=$arrDadosInauguracao['mobiliario_adquirido']=='S' ? 'checked' : ''?> /> Sim &nbsp;
                <input type="radio" onclick="alteraMobAdquirido(this.value);" name="mobiliario_adquirido" id="mobiliario_adquirido" <?=!$habilitado ? 'disabled' : '' ?> value="N" <?=$arrDadosInauguracao['mobiliario_adquirido']=='N' ? 'checked' : ''?> /> Não
                <input type="radio" onclick="alteraMobAdquirido(this.value);" name="mobiliario_adquirido" id="mobiliario_adquirido" <?=!$habilitado ? 'disabled' : '' ?> value="P" <?=$arrDadosInauguracao['mobiliario_adquirido']=='P' ? 'checked' : ''?> /> Não se aplica
                <br>
                <div style="display: none" id="div_conformidade">
                <?php if($arrDadosInauguracao['iobid']){?>
                	<button type="button" onclick="abreDeclaracaoConformidade();">Declaração de Conformidade</button>
                <?php }else{?>
                	<button type="button" disabled="disabled" title="É necessário salvar o formulário de Funcionamento da Obra antes de realizar a Declaração de Conformidade">Declaração de Conformidade</button>
                <?php }?>
                </div>
            </td>
        </tr>

        <tr>
            <td class="subtitulodireita" width="250px">O mobiliário se encontra entregue no município?</td>
            <td>
            <?php $arrDadosInauguracao['mobiliario_entregue'] = ($arrDadosInauguracao['mobiliario_entregue']) ? $arrDadosInauguracao['mobiliario_entregue'] : 'P';?>
                <input type="radio" name="mobiliario_entregue" id="mobiliario_entregue" <?=!$habilitado ? 'disabled' : '' ?> value="S" <?=$arrDadosInauguracao['mobiliario_entregue']=='S' ? 'checked' : ''?> /> Sim &nbsp;
                <input type="radio" name="mobiliario_entregue" id="mobiliario_entregue" <?=!$habilitado ? 'disabled' : '' ?> value="N" <?=$arrDadosInauguracao['mobiliario_entregue']=='N' ? 'checked' : ''?> /> Não
                <input type="radio" name="mobiliario_entregue" id="mobiliario_entregue" <?=!$habilitado ? 'disabled' : '' ?> value="P" <?=$arrDadosInauguracao['mobiliario_entregue']=='P' ? 'checked' : ''?> /> Não se aplica
            </td>
        </tr>
        
         <tr>
            <td class="subtitulodireita" width="250px">Empreendimento apto a funcionar?</td>
            <td>
                <input type="radio" name="empreendimento_apto" id="empreendimento_apto" <?=!$habilitado ? 'disabled' : '' ?> value="S" <?=$arrDadosInauguracao['empreendimento_apto']=='S' ? 'checked' : ''?> /> Sim &nbsp;
                <input type="radio" name="empreendimento_apto" id="empreendimento_apto" <?=!$habilitado ? 'disabled' : '' ?> value="N" <?=$arrDadosInauguracao['empreendimento_apto']=='N' ? 'checked' : ''?> /> Não
            </td>
        </tr>
        
        <tr>
            <td class="subtitulodireita" width="250px">Habite-se</td>
            <td>
                <input type="file" <?=!$habilitado ? 'disabled' : ''?> name="arquivo_obra_habite_se" id="arquivo_obra_habite_se"/>
            </td>
        </tr>

        <tr>
            <td class="subtitulodireita" width="250px">Anexos Habite-se</td>
            <td>
			    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			        <tr>
			            <td class="subtitulodireita" ><center>Ação </center></td>
			            <td class="subtitulodireita" ><center>Responsável </center></td>
			            <td class="subtitulodireita" ><center>Data de Upload </center></td>
			        </tr>
			        <?php 
			        if( count($arrArquivosHabitese) > 0)
			        {
			        	foreach($arrArquivosHabitese as $k => $v)
			        	{
			        ?>
					        <tr>
					            <td class="" width="20%" ><center> <?php echo $v['acao']; ?> </center></td>
					            <td class="" width="40%"><?php echo $v['responsavel']; ?></td>
					            <td class="" width="30%"><?php echo $v['data_inclusao']; ?></td>
					        </tr>
			       <?php 
			        	}
			        }
			        else 
			        {?>
			        	<tr>
				            <td colspan="3" ><center> <b style="color:red;">Nenhum registro encontrado</b> </center></td>
				        </tr>
			        <?php 
			        }
			        ?>
				</table>
            </td>
        </tr>
    </table>
    <?php
    $title = 'Obra Funcionamento';
    $diaUm = new DateTime('2016-06-15');
    foreach (is_array($arrDadosFotos)?$arrDadosFotos:[] as $dados) {
        $dataInclusao = new DateTime($dados['fdtinclusao']);
        if ($dataInclusao < $diaUm) {
            $title = 'Obra Concluída';
            break;
        }
    }
    ?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr bgcolor="">
            <td class="subtitulocentro" colspan="2">Fotos</td>
        </tr>
        <tr>
            <td class="subtitulocentro"><?=$title?></td>
            <td class="subtitulocentro">Inauguração</td>
        </tr>
        <tr>
            <td width="50%">
                <div>
                    <table id="table_anexos_obra_concluida" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                        <tr class="divTituloPadrao">
                            <?php if ($habilitado): ?>
                            <td class="" style="width:64px;">
                                <div style="width:64px;"><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo_obra_concluida">Adicionar</a></div>
                            </td>
                            <?php endif?>
                            <td>Pré-visualizaçao</td>
                            <td>Nome</td>
                            <td>Descrição</td>
                        </tr>
                        <?php
                        if (!empty($arrDadosFotos)):
                            foreach ($arrDadosFotos as $arquivo): if($arquivo['tipo'] == 'I') continue;?>
                                <tr class="anexos" style="background-color: #FFFFff">
                                    <?php if ($habilitado):?>
                                    <td class="SubTituloEsquerda" style="width: 56px; background-color: #FFFFff">
                                        <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_obra_concluida" href="">Excluir</a></span>
                                    </td>
                                    <?php endif?>
                                    <td  style="background-color: #FFFFff">
                                        <?php if(file_exists((APPRAIZ."arquivos/obras2/".floor($arquivo['arqid']/1000)."/".$arquivo['arqid']))): 

                                        		$imgend = APPRAIZ."arquivos/obras2/".floor($arquivo['arqid']/1000)."/".$arquivo['arqid'];
		                                        
		                                        if (is_file($imgend)) {
		                                        	
	                                        		$img_max_dimX = 100;
	                                        		$img_max_dimY = 85;
	                                        	
	                                        		$imginfo = getimagesize($imgend);
	                                        	
	                                        		$width = $imginfo[0];
	                                        		$height = $imginfo[1];
	                                        	
	                                        		if (($width > $img_max_dimX) or ($height > $img_max_dimY)) {
	                                        			if ($width > $height) {
	                                        				$w = $width * 0.9;
	                                        				while ($w > $img_max_dimX) {
	                                        					$w = $w * 0.9;
	                                        				}
	                                        				$w = round($w);
	                                        				$h = ($w * $height) / $width;
	                                        			} else {
	                                        				$h = $height * 0.9;
	                                        				while ($h > $img_max_dimY) {
	                                        					$h = $h * 0.9;
	                                        				}
	                                        				$h = round($h);
	                                        				$w = ($h * $width) / $height;
	                                        			}
	                                        		} else {
	                                        			$w = $width;
	                                        			$h = $height;
	                                        		}
	                                        	
	                                        		$tamanho = " width=\"$w\" height=\"$h\" ";
		                                        	
		                                        }                                        
                                        ?>
                                        	
                                            <img src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arquivo['arqid'] ?>&newwidth=100&_sisarquivo=<?php echo $eschema ?>" style="width:97px; height:97px;" alt=""/>
                                        <?php endif;?>
                                    </td>
                                    <td class="SubTituloEsquerda" style="background-color: #FFFFff">
                                        <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo_obra_concluida[]" id="arquivo_obr_concluida"/>
                                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/inauguracao&acao=A&download=<?=$arquivo['arqid']?>">
                                            <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                        </a>
                                    </td>
                                    <td class="SubTituloEsquerda" style="background-color: #FFFFff"><input type="text" maxlength="255" size="" value="<?=$arquivo['arqdescricao']?>" name="arquivo_descricao_obra_concluida[]" id="arquivo_descricao_obr_concluida"/></td>
                                </tr>
                            <?php
                            endforeach;
                        endif;
                        ?>
                        <tr class="anexos">
                            <?php if ($habilitado):?>
                            <td class="SubTituloEsquerda" style="width: 56px; background-color: #FFFFff;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_obra_concluida" href="">Excluir</a></span></td>
                            <?php endif?>
                            <td class="SubTituloEsquerda" style="background-color: #FFFFff"></td>
                            <td class="SubTituloEsquerda" style="background-color: #FFFFff"><input onchange="validarEscolhaArquivo(this)"  type="file" <?=!$habilitado ? 'disabled' : ''?> name="arquivo_obra_concluida[]" id="arquivo_obr_concluida"/></td>
                            <td class="SubTituloEsquerda" style="background-color: #FFFFff"><input maxlength="255" <?=!$habilitado ? 'disabled' : ''?> type="text" name="arquivo_descricao_obra_concluida[]" id="arquivo_descricao_obr_concluida"/></td>
                        </tr>
                    </table>
                </div>
            </td>
            <td>
                <div>
                    <table id="table_anexos_inauguracao" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                        <tr class="divTituloPadrao">
                            <?php if ($habilitado):?>
                            <td class="" style="width:64px;">
                                <div style="width:64px;"><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo_inauguracao">Adicionar</a></div>
                            </td>
                            <?php endif?>
                            <td>Pré-visualizaçao</td>
                            <td>Nome</td>
                            <td>Descrição</td>
                        </tr>
                        <?php
                        if (!empty($arrDadosFotos)):
                            foreach ($arrDadosFotos as $arquivo): if($arquivo['tipo'] == 'C') continue;?>
                                <tr class="anexos">
                                    <td class="SubTituloEsquerda" style="width: 56px;">
                                        <?php if ($habilitado):?>
                                        <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_inauguracao" href="">Excluir</a></span>
                                        <?php endif ?>
                                    </td>
                                    <td class="SubTituloEsquerda">
                                        <?php if(file_exists((APPRAIZ."arquivos/obras2/".floor($arquivo['arqid']/1000)."/".$arquivo['arqid']))): ?>
                                            <?php 
                                            $imgend = APPRAIZ."arquivos/obras2/".floor($arquivo['arqid']/1000)."/".$arquivo['arqid'];
                                            
                                            if (is_file($imgend)) {
                                            	 
                                            	$img_max_dimX = 100;
                                            	$img_max_dimY = 85;
                                            
                                            	$imginfo = getimagesize($imgend);
                                            
                                            	$width = $imginfo[0];
                                            	$height = $imginfo[1];
                                            
                                            	if (($width > $img_max_dimX) or ($height > $img_max_dimY)) {
                                            		if ($width > $height) {
                                            			$w = $width * 0.9;
                                            			while ($w > $img_max_dimX) {
                                            				$w = $w * 0.9;
                                            			}
                                            			$w = round($w);
                                            			$h = ($w * $height) / $width;
                                            		} else {
                                            			$h = $height * 0.9;
                                            			while ($h > $img_max_dimY) {
                                            				$h = $h * 0.9;
                                            			}
                                            			$h = round($h);
                                            			$w = ($h * $width) / $height;
                                            		}
                                            	} else {
                                            		$w = $width;
                                            		$h = $height;
                                            	}
                                            
                                            	$tamanho = " width=\"$w\" height=\"$h\" ";
                                            	 
                                            }
                                            
                                            ?>
                                            
                                            <img src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arquivo['arqid'] ?>&newwidth=100&_sisarquivo=<?php echo $eschema ?>" style="width:97px; height:97px;" alt=""/>
                                        <?php endif;?>
                                    </td>
                                    <td class="SubTituloEsquerda">
                                        <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo_inauguracao[]" id="arquivo_inauguracao"/>
                                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/inauguracao&acao=A&download=<?=$arquivo['arqid']?>">
                                            <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                        </a>
                                    </td>
                                    <td class="SubTituloEsquerda"><input type="text" maxlength="255" size="" value="<?=$arquivo['arqdescricao']?>" name="arquivo_descricao_inauguracao[]" id="arquivo_descricao_inauguracao"/></td>
                                </tr>
                            <?php
                            endforeach;
                        endif;
                        ?>
                        <tr class="anexos">
                            <?php if ($habilitado):?>
                            <td class="SubTituloEsquerda" style="width: 56px; background-color: #FFFFff;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_inauguracao" href="">Excluir</a></span></td>
                            <?php endif ?>
                            <td class="SubTituloEsquerda" style="background-color: #FFFFff"></td>
                            <td class="SubTituloEsquerda" style="background-color: #FFFFff"><input onchange="validarEscolhaArquivo(this)" type="file" name="arquivo_inauguracao[]" <?=!$habilitado ? 'disabled' : ''?> id="arquivo_inauguracao"/></td>
                            <td class="SubTituloEsquerda" style="background-color: #FFFFff"><input maxlength="255" size="" type="text" <?=!$habilitado ? 'disabled' : ''?> name="arquivo_descricao_inauguracao[]" id="arquivo_descricao_inauguracao"/></td>
                        </tr>
                    </table>
                </div>
            </td>
        </tr>
        <tr class="divTituloPadrao">
        <input type="hidden" id="total_file_obra_funcionamento" value="0">
        <input type="hidden" id="total_file_obra_inauguracao" value="0">
            <td colspan="2" style="text-align: center">
                <input type="submit" value="Salvar" id="registraMobiliario" style="cursor:pointer;" <?=!$habilitado ? 'disabled' : ''?>/>
            </td>
        </tr>
    </table>
</form>





<div id="dialog-alerta-repasse" style="display: none;">
    <div>
        <p style="text-align:justify">
            <h4>Prezado(a),</h4>

            <p style="margin: 10px 0px;">
                Para ter direito ao repasse de recursos de apoio à manutenção e desenvolvimento da educação infantil, o município (ou DF) , logo que iniciar o funcionamento da nova unidade deverá cadastrar, no SIMEC (http://simec.mec.gov.br) "Módulo E. I. Manutenção - Unidades do Proinfância", as novas matrículas oferecidas nesse novo estabelecimento do PROINFÂNCIA, conforme previsto na Lei nº 12.499 de 29 de setembro de 2011 e Resolução CD/FNDE nº 15, de 16 de maio de 2013.
                <br>
                É vedada a inclusão de matrículas de crianças já computadas no âmbito do Fundeb.
                <br>
                O valor do apoio financeiro será calculado com base no mês de registro no SIMEC.
            </p>
        </p>
    </div>
</div>




<?php
$previsaoObra = (new Supervisao())->dataPrevistaDeConclusaoReferenteUltimaVistoriaInstitucional($obrid);

if (!empty($previsaoObra)): ?>
<script type="text/javascript">

$1_11(function(){

    jQuery('#dialog-alerta-repasse').dialog(
        {
            title: 'ATENÇÃO',
            resizable: false,
            modal: true,
            draggable: false,
            buttons: {
                "Li e entendi": function() {
                    $( this ).dialog( "close" );
                },
            },
            maxWidth:600,
            width: 600,
        }
    )

	if( $('input[type=radio][name="mobiliario_adquirido"]:checked' ).val() !== undefined )
	{
		var valorCampo = $('input[type=radio][name="mobiliario_adquirido"]:checked' ).val();
		
		if(valorCampo == "S")
		{
			$("#div_conformidade").show();
		}
		else
		{	
			$("#div_conformidade").hide();
		}
	}
	
    if ('' === $('#justificativaprevisao').val()) {
        $('#justificativaprevisao').removeClass('obrigatorio');
    } else {
        $('.justificativaprevisao').show();
    }

    $1_11('body').on('click', '#calendarDiv td:not(.calendar_week_column), #calendarDiv #todaysDateString', function(){
        if ('dtprevisaoinauguracao' === window.returnDateTo.id) {
            setTimeout(verificaPrevisao, 300);
        }
    });

    $1_11('#dtprevisaoinauguracao').removeAttr('onblur');
    $1_11('#dtprevisaoinauguracao').blur(function(){
        MouseBlur(this);
        $1_11(this).val(mascaraglobal('##/##/####', $1_11(this).val()));
    });
    $1_11('#dtprevisaoinauguracao').blur(function(){
        if (validando_data(this)) {
            verificaPrevisao();
        }
    });

    $1_11('#form_inauguracao').submit(function(){
        $('#registraMobiliario').attr('disabled', true);
        var ok = true;
        var okDC = true;
        $1_11('#form_inauguracao .obrigatorio').each(function(){
            if ('' === $1_11(this).val()) {
                $1_11(this).focus();
                ok = false;
                return false;
            }
        });


        var iobid = $('#iobid').val();

        if(iobid != '')
        {
        	var valorCampo = $('input[type=radio][name="mobiliario_adquirido"]:checked' ).val();
    		
    		if(valorCampo == "S")
    		{
		        $.ajax({
					type: "POST",
					data: {req : 'ajaxDeclaracaoConformidade'},
					url: 'obras2.php?modulo=principal/inauguracao&acao=A&iobid='+iobid,
					async: false,
					success: function(retornoajax) {

						if(retornoajax == 'erro')
						{
							alert('Ao selecionar a opção "Sim" no campo "O mobiliário já foi adquirido?" é necessário assinar a Declaração de Conformidade.');
							okDC = false;
							ok = false;
							return false;
						}
					}
				});  
		        return okDC;
    		}
    		else
    		{
    			if (!ok) {
                	window.alert('Todos os campos obrigatórios devem ser preenchidos.');
                        $('#registraMobiliario').attr('disabled', false);
            	}
    			return ok;
    		}
    		
        		
        }
        else
        {
        	if (!ok) {
                    window.alert('Todos os campos obrigatórios devem ser preenchidos.');
                    $('#registraMobiliario').attr('disabled', false);
        	}
        	return ok;
        }
        
    });
});

function verificaPrevisao()
{

	<?php if($previsaoObra['ano'] != ''){ ?>
	
    var dataPrevista = new Date(<?=$previsaoObra['ano']?>, <?=$previsaoObra['mes']?>, <?=$previsaoObra['dia']?>);
    dataPrevista.setFullYear(dataPrevista.getFullYear() + 1);
    var dataInauguracao = $1_11('#dtprevisaoinauguracao').val();

    dataInauguracao = new Date(
        dataInauguracao.substr(6, 4),
        parseInt(dataInauguracao.substr(3, 2)) - 1,
        dataInauguracao.substr(0, 2)
    );

    if (dataInauguracao.getTime() > dataPrevista.getTime()) {
        if (window.confirm('A data de previsão de inauguração informada é um ano superior à data prevista de conclusão da obra. Isso está correto?')) {
            window.alert('Por favor, preencha o campo de justificativa.');
            $1_11('.justificativaprevisao').show();
            $1_11('#justificativaprevisao').focus();
            $1_11('#justificativaprevisao').addClass('obrigatorio');
            $1_11('.justificativaprevisao > img').show();
            $1_11('#justificativaprevisao').text($1_11('#justificativaprevisao').attr('data-justificativa'));
        } else {
            $1_11('.justificativaprevisao').hide();
            window.alert('Então, por favor, ajuste a data de previsão de inauguração corretamente.');
            $1_11('#dtprevisaoinauguracao').val('');
            $1_11('#justificativaprevisao').attr('data-justificativa', $1_11('#justificativaprevisao').val());
            $1_11('#justificativaprevisao').text('');
            $1_11('.justificativaprevisao > input').removeClass('obrigatorio');
            $1_11('.justificativaprevisao > img').hide();
            if (0 === $1_11('#calendarDiv').length) {
                $1_11('#dtprevisaoinauguracao').next().click();
            }
        }
    } else {
        $1_11('.justificativaprevisao').hide();
        $1_11('#justificativaprevisao').removeClass('obrigatorio');
        $1_11('.justificativaprevisao > img').hide();
        $1_11('#justificativaprevisao').attr('data-justificativa', $1_11('#justificativaprevisao').val());
        $1_11('#justificativaprevisao').text('');
    }
    <?php 
	}
	else 
	{
    ?>
    	return false;
	<?php 
	}
	?>
}
</script>
<?php endif; ?>

<script>
function alteraMobAdquirido(valor)
{
	if(valor == "S")
	{
		$("#div_conformidade").show();
	}
	else
	{	
		$("#div_conformidade").hide();
	}	
}
function abreDeclaracaoConformidade()
{
	var iobid = $('#iobid').val();
	var obrid = $('#obrid').val();
	return windowOpen( '?modulo=principal/popUpDeclaracaoConformidade&acao=A&iobid='+iobid+'&obrid='+obrid,'blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	
}

function downloadArquivo( arqid )
{
	window.location='?modulo=principal/inauguracao&acao=A&downloadHabitese=S&arqid=' + arqid
}

</script>