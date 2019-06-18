<?php
// empreendimento || obra || orgao
verificaSessao('obra');

require APPRAIZ . 'obras2/includes/principal/etapas_da_obra/ctrl.php';

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>


<form method="post" id="formulario" name="formulario">
    <input type="hidden" name="evt" id="evt" value="">
    <input type="hidden" name="vl_contrato_atualizado" id="vl_contrato_atualizado" value="0">
    <input type="hidden" name="atualizarNivel" 	       id="atualizarNivel"         value="0">
    <input type="hidden" name="dtInicioObra" 	       id="dtInicioObra" 	   value="">
    <input type="hidden" name="dtTerminoObra" 	       id="dtTerminoObra" 	   value="">
    <input type="hidden" name="xls" 	               id="xls"                    value="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbEtapasDaObra">
        <tr>
            <center><label class="TituloTela" style="color:#000000;">Lista das Etapas da Obra</label></center>
            <!--<td class="SubTituloTelaCentro" colspan="2">Lista das Etapas da Obra</td>-->
        </tr>
        <tr>
            <td colspan="2">
                <table id="tabela_etapas" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                    <thead>
                        <tr id="cabecalho">
                            <td width="5%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Ordem</strong></td>
                            <td width="5%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Ação</strong></td>
                            <td width="30%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Descrição</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Data de Início</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Data de Término</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Valor do Item (R$)</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>(%) Referente a Estrutura</strong></td>
                        </tr>
                    </thead>
                    <tbody>
                         
                            <tr id="tr_matrix" style="display:none;">
                                <td align="center"></td>
                                <td align="center">
                                    <span>
                                        <img src='/imagens/excluir.gif' <?php echo (!$habilitado ? 'disabled="disabled"' : '' ) ?> style='cursor:pointer;' border='0' title='Excluir'>
                                    </span>
                                </td>
                                <td>
                                    <span id="itcdesc[]"></span>
                                    <input type="hidden" name="itcid[]" value="">
                                </td>
                                <td align="center"><?php echo campo_data2('icodtinicioitem[]', 'S', $habilita, '', 'S', '', ' verificaDataInicio(this); ', '', ' verificaDataInicio(this); '); ?></td>
                                <td align="center"><?php echo campo_data2('icodterminoitem[]', 'S', $habilita, '', 'S', '', ' verificaDataTermino(this); ', '', ' verificaDataTermino(this); '); ?></td>
                                <td align="center"><?php echo campo_texto('icovlritem[]', 'S', $habilita, '', 14, 14, '', '', 'right', '', 0, 'onkeypress="calculoEdificacao.setDefaultValue( this )";', 'calculoEdificacao.manager( this );'); ?></td>
                                <td align="center"><?php echo campo_texto('icopercsobreestrutura[]', 'N', 'N', '', 5, 6, '', '', 'right', '', 0); ?> %</td>
                            </tr>

                    </tbody>
                    <tfoot>
                        <tr id="tr_total" bgcolor="#FFFFFF">
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td align="right"><strong>Total</strong></td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='totalv' size='15' maxlength='14' value='<?php echo number_format($somav, 2, ',', '.'); ?>' disabled="disabled">
                            </td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='total' size='6' maxlength='6' value='<?php echo number_format($soma, 2, ',', '.'); ?>' disabled="disabled"> %
                            </td>
                        </tr>
                        <tr id="tr_vlcontrato" bgcolor="#FFFFFF">
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td align="right"><strong><?php if($obraMI){echo "Valor do Cronograma Padrão";}else{echo "Valor do Contrato";}?></strong></td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' name='vl_contrato' id='vl_contrato' size='15' maxlength='14' value='<?php echo number_format($ocrvalorexecucao, 2, ',', '.'); ?>' disabled="disabled">
                            </td>
                            <td align="center">
                                <input class=disabled style="text-align:right" type='text' id='vl_porcento' size='6' maxlength='6' value='100,00' disabled="disabled"> %
                            </td>
                        </tr>
                        <tr id="tr_vlrestante" bgcolor="#FFFFFF">
                            <td colspan="4">

                            <? if($obra->getTravaCronograma($obrid) == 'f' && possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE))): ?>
                                <button id="editar_cronograma" onclick="window.location="><img src="/imagens/lapis.png" style="cursor:pointer;" border="0" title="Editar Cronograma">&nbsp;&nbsp;Editar Cronograma</button>
                            <? endif; ?>
                            <?php if ($habilitado && !$desabilitaWF): ?>
                                    <a href="#" onclick="inserirEtapas();">
                                        <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Etapa">&nbsp;&nbsp;Inserir Etapa
                                    </a>
                            <?php endif; ?>
                            </td>
                            <td align="right"><strong>Valor Restante</strong></td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='rest_totalv' size='15' maxlength='14' value='<?php echo number_format($ocrvalorexecucao - $somav, 2, ',', '.'); ?>' disabled="disabled">
                            </td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='rest_total' size='6' maxlength='6' value='<?php echo number_format(100 - $soma, 2, ',', '.'); ?>' disabled="disabled"> %
                            </td>
                        </tr>
                    </tfoot>
                </table>
            </td>
            <td rowspan="3" align="right" valign="top" width="1">
            <?php
                // Barra de estado WORKFLOW
                if ($habilitado && !$desabilitaWF) {
                    wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid));
                }
            ?>
            </td>
        </tr>
        <?php if($_GET['acao'] != 'E'){ ?>
            <tr>
                <td class="SubTituloDireita" width="30%" nowrap="nowrap">TODOS OS SERVIÇOS QUE COMPOEM A PLANILHA PACTUADA COM O FNDE FORAM CONTRATADOS?</td>
                <td>
                    <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_s" value="S"
                    <?php echo (!$habilitado ? 'disabled="disabled"' : '' ) ?>
                    <?php echo $obrcronogramaservicocontratado == 'S' ? 'checked="checked"' : '' ?>
                    onclick="managerServicoContratado('S');">
                    <label for="obrcronogramaservicocontratado_s">Sim</label>
                    <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_n" value="N"
                           <?= (!$habilitado ? 'disabled="disabled"' : '' ) ?> <?php echo $obrcronogramaservicocontratado == 'N' ? 'checked="checked"' : '' ?> onclick="managerServicoContratado('N');">
                    <label for="obrcronogramaservicocontratado_n">Não</label>
                </td>
            </tr>

            <?php
                if(possuiPerfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO)) && possuiTravaCronograma($obrid)){
            ?>
            <tr>
                <td class="SubTituloDireita" width="30%" nowrap="nowrap">CRONOGRAMA TRAVADO PARA EDIÇÃO DO SUPERVISOR ?</td>
                <td>
                    <input type="radio" name="obrtravaedicaocronograma" id="obrtravaedicaocronograma_s" value="S"
                    <?php
                          echo $flag_obrtravaedicaocronograma == 't' ? 'checked="checked"' : '' ?> >
                    <label for="obrtravaedicaocronograma_s">Sim</label>
                    <input type="radio" name="obrtravaedicaocronograma" id="obrtravaedicaocronograma_n" value="N"
                    <?php
                          echo ($flag_obrtravaedicaocronograma == 'f' || $flag_obrtravaedicaocronograma == NULL) ? 'checked="checked"' : '' ?> >
                    <label for="obrtravaedicaocronograma_n">Não</label>
                </td>
            </tr>

            <?php } ?>

            <tr id="tr_servico_contratado_justificativa" style="display: <?php echo ($obrcronogramaservicocontratado == 'N' ? '' : 'none') ?>">
                <td class="SubTituloDireita" width="30%" nowrap="nowrap" valign="top">Justificativa:</td>
                <td>
                <?php
                    echo campo_textarea('obrcronogramaservicocontratadojustificativa', 'S', 'S', '', '70', '8', '2000', '', 0, '', '', false, '', '');
                ?>
                </td>
            </tr>

                    <tr class="divTituloPadrao">
                        <td colspan="2"> 
                            <center>
                                <?php
                                if ($habilitado || (possuiPerfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO)) && possuiTravaCronograma($obrid))){
                                    ?>
                                <input type="button" id="salvar" value="Salvar" style="cursor: pointer" onclick="validaForm();">
                                <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href = '?modulo=principal/cadObra&acao=A'">
                                <?php
                                }
                                ?>
                                <input type="button" id="btnGerarXls" value="Gerar XLS" style="cursor: pointer" onclick="javascript: gerarXls()">
                            </center>
                            <!--</div>-->
                        </td>
                    </tr>
 
        <?php }else{
            if ($habilitado):
                ?>
                    <tr bgcolor="#C0C0C0">
                        <td colspan="2">
                            <center>
                                <input type="button" id="salvar" value="Salvar" style="cursor: pointer" onclick="validaForm();">
                                <input type="button" value="Voltar" style="cursor: pointer" onclick="javascript: window.close()">
                            </center>
                        </td>
                    </tr>
            <?php
        endif;
        } ?>
    </table>
 
</form>


<script type="text/javascript">
<?php
$cronograma = new Cronograma();
$itens   = new ItensComposicaoObras();
$arItens = $itens->getItemComposicaoByObra($obrid);

if (is_array($arItens) && count($arItens) > 0) {
    foreach ($arItens as $key => $data) {

        $item = $cronograma->verificaCronogramaExecucao($obrid, $data['icoid']);

        if($item[0]['situacao'] == 'V')
            $data['itcdesc'] = "<span class='item-vermelho' style='color: #EF0303; font-weight: ;' title='{$item[0]['observacao']}'>" . $data['itcdesc'] . "</span>";
        else if($item[0]['situacao'] == 'D')
            $data['itcdesc'] = "<span class='item-amarelo' style='color: #f0ad4e; font-weight: ;' title='{$item[0]['observacao']}'>" . $data['itcdesc'] . "</span>";

        $data['icodtinicioitem'] = formata_data($data['icodtinicioitem']);
        $data['icodterminoitem'] = formata_data($data['icodterminoitem']);
//         $data = array_map('utf8_encode', $data);
        echo "gerenteEdificacao.addLine(" . simec_json_encode($data) . "); ";
    }
}
?>

    $(function(){
        $('.item-vermelho').parents('td').next().next().find('input').css({borderColor: '#EF0303',borderLeft: '#EF0303 3px solid'})
        $('.item-amarelo').parents('td').next().next().find('input').css({borderColor: '#f0ad4e',borderLeft: '#f0ad4e 3px solid'})
    });
</script>

<?php
        $objObras = new Obras();
        $objObras->carregarPorIdCache($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#btnGerarXls').attr('disabled', false);
                      jQuery('#xls').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>


<? // ver($arItens); ?>

<?php
/**
 * Demanda #231610
 */
if($obraMI){
    $ordemServico = new OrdemServicoMI();
    $dados_os_espelho = $ordemServico->getArrayServicosExternosImplantacao($obrid);

    if(!empty($dados_os_espelho)){
?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td>
                   <center><label class="TituloTela" style="color:#000000;">Serviços Externos</label></center>
                </td>
            </tr>
            <tr>
                <td>
                    <table class="tabela listagem" style="text-align: center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                         <thead>
                             <tr>
                                 <td><b>Item</b></td>
                                 <td><b>Valor Unitário</b></td>
                                 <td><b>Quantidade</b></td>
                                 <td><b>Unidade de medida</b></td>
                                 <td><b>Valor Total</b></td>
                             </tr>
                         </thead>
                         <tbody>
                             <?php
                                $vl_tot = 0;
                                foreach ($dados_os_espelho as $servico) {
                                    echo '<tr>';
                                    echo '<td>'.$servico['item'].'</td>';
                                    echo '<td>'.$servico['valor_unitario'].'</td>';
                                    echo '<td>'.$servico['quantidade'].'</td>';
                                    echo '<td>'.$servico['unidade_medida'].'</td>';
                                    echo '<td>'.$servico['valor_total'].'</td>';
                                    echo '</tr>';

                                    $vl = str_replace('.', '', $servico['valor_total']);
                                    $vl = str_replace(',', '.', $vl);
                                    $vl = (float)$vl;
                                    $vl_tot += $vl;
                                }
                                echo ' 
                                     <tr>
                                         <th><b>TOTAL</b></th>
                                         <td>&nbsp;</td>
                                         <td>&nbsp;</td>
                                         <td>&nbsp;</td>
                                         <td>'.number_format($vl_tot,2,',','.').'</td>
                                     </tr>  
                                     ';
                             ?>
                         </tbody>
                    </table>
                </td>
            </tr>
        </table>
<?php }}
    require APPRAIZ . 'obras2/includes/principal/etapas_da_obra/js.php';
?>

