<?php
// empreendimento || obra || orgao
verificaSessao('orgao');

require APPRAIZ . 'obras2/includes/principal/vistoria/ctrl.php';


?>
<script src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloDireita" width="20%">Mostrar vistorias realizadas por:</td>
        <td>
            <?php foreach ($responsaveisPossiveis as $responsavelPossivel) {
                if( $responsavelPossivel['rsuid'] != 4 ){?>

                    <span style="margin-left:5px">
                    <input type="checkbox" id="chamada_responsavel_<?php echo$responsavelPossivel['rsuid']?>" name="chamada_responsavel_<?php echo$responsavelPossivel['rsuid']?>" value="responsavel_<?php echo$responsavelPossivel['rsuid']?>" onclick="javascript:toggleParaResponsabilidades( this.value )" <?php echo($responsavelPossivel['rsuid']==1 || $responsavelPossivel['rsuid']==3|| $responsavelPossivel['rsuid']==2)?'checked':''?>/>
                        <?php echo $responsavelPossivel['rsudsc'] ?></span>

                <?php }} ?>
<!--            <span style="margin-left:5px">-->
<!--                    <input type="checkbox" id="chamada_responsavel_4" name="chamada_responsavel_4" value="responsavel_4" onclick="javascript:toggleParaResponsabilidades(this.value)" --><?php //echo($_REQUEST['chamada_responsavel_4']==1)?'checked':''?><!--/>-->
<!--                    Empresa MI-->
<!--                </span>-->
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="20%">Mostrar Documentos ART?</td>
        <td>
            <span style="margin-left:5px">
            <input type="checkbox" id="chamada_documentos_art" name="chamada_documentos_art" value="documentos_art" checked onclick="javascript:toggleParaResponsabilidades( this.value )" /> Mostrar
        </td>
    </tr>
    <?php

    $dadosObraVinculada = $db->pegaLinha(getSqlDadosObraVinculada($obrid));

    if (!empty($dadosObraVinculada['obridvinculado'])) {
        ?>
        <tr>
            <td class="SubTituloDireita" width="20%">Obra Vinculada:</td>
            <td>
                <span style="margin-left:5px">
                <input type="checkbox" id="chamada_obra_vinculada" onclick="javascript:toggleParaObraVinculada( jQuery(this).parents('table:first') )" /> Mostrar
                </span>
            </td>
        </tr>
        <?php
    }
    ?>

</table>
<div id="documentos_art" style="">
    <?php

    monta_titulo_obras( 'Documentos ART', '' );
    ?>
    <form id="formulario_art" name="formulario_art" method="post" enctype="multipart/form-data" action="?modulo=principal/vistoria&acao=A">
        <input type="hidden" id="obrid"    name="obrid"    value="<?php echo $obrid;?>" />
        <input type="hidden" id="operacao" name="operacao" value="arquivoArt" />
        <input type="hidden" id="req"      name="req"      value="" />
        <input type="hidden" id="arqid"    name="arqid"    value="" />
        <input type="hidden" id="oarid"    name="oarid"    value="" />

        <?php if( $habilitado_art ){ ?>
            <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">

                <?php
                $crtid    = $obra->pegaContratoPorObra( $obrid );
                $contrato = new Contrato( $crtid );

                $dados    = $contrato->getDados();
                $empresa_contratada_id = $dados['entidempresa'];
                $empresa 	= new Entidade( $empresa_contratada_id );
                $empresa_contratada = $empresa->entnome;

                if (!empty($obra->empid)){
                    
                    $dadosFiscal = $db->pegaLinha(getSqlDadosFiscal($obra->empid));
                    $fiscal_responsavel = $dadosFiscal['usunome'];
                }else{
                    echo "<script>
                    alert('Faltam parâmetros para acessar essa tela.');
                    location.href = '?modulo=principal/inicioLista&acao=A';
                    </script>";
                    die();
                }
                ?>

                <tr>
                    <td class="SubTituloDireita" width="20%">Empresa contratada:</td>
                    <td>
                        <?php if (obraMi($obra->obrid)){ $obrigacao_mi = "N";}else{$obrigacao_mi = "S";}?>
                        <?php echo campo_texto('empresa_contratada',$obrigacao_mi, "S",'',43,100,'','', '', '', '', 'readonly="readonly"', '');?>
                        <input type="hidden" name="empresa_contratada_id" value="<?php echo $empresa_contratada_id?>">
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%">Fiscal Responsável:</td>
                    <td>
                        <?php echo campo_texto('fiscal_responsavel','S', 'S','',43,100,'','', '', '', '', 'readonly="readonly"', '');?>
                        <input type="hidden" name="usucpf_fiscal" value="<?php echo $dadosFiscal['usucpf']?>">
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%">Descrição do Anexo ART de Execução:</td>

                    <td><?php echo campo_texto('oardesc1','S', 'S','',43,100,'','', '', '', '', 'id="oardesc1"', '');?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Tipo:</td>
                    <td>
                        <?php
                        $tipoArquivo = new TipoArquivo();
                        $sql = $tipoArquivo->listaCombo(array('tpaid=25'));
                        $db->monta_combo("tpaid1", $sql, 'S', "", "", '', '', '', 'S', 'tpaid1','', 25);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Arquivo ART de Execução Paga e Assinada:</td>
                    <td>
                        <input type="file" name="arquivo1" id="arquivo1" class="obrigatorio"/>
                        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%">Descrição do Anexo ART de Fiscalização:</td>
                    <td><?php echo campo_texto('oardesc2','S','S','',43,100,'','', '', '', '', 'id="oardesc2"', '');?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Tipo:</td>
                    <td>
                        <?php
                        $tipoArquivo = new TipoArquivo();
                        $sql = $tipoArquivo->listaCombo(array('tpaid=26'));
                        $db->monta_combo("tpaid2", $sql, 'S', "", "", '', '', '', 'S', 'tpaid2','', 26);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Arquivo ART de Fiscalização Paga e Assinada:</td>
                    <td>
                        <input type="file" name="arquivo2" id="arquivo2" class="obrigatorio"/>
                        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                    </td>
                </tr>
                <tr class="divTituloPadrao">
                    <td colspan="3" align="center">
                        <?php if( $habilitado_art ){ ?>
                            <input type="button" name="salvar" class="incluir_art" value="Anexar Arquivo"/>
                        <?php }?>
                    </td>
                </tr>
            </table>
            <?php

        }


        $obrasArquivos 	= new ObrasArquivos();

        $param = array("obrid" => $obrid/*, "not(img)" => true*/);

        if ( $habilitado_art == false ){
            $param['block_img_excluir'] = true;
        }

        $param['tpaid(IN)'] = '25,26';

        $sql = $obrasArquivos->listaSqlVistoria( $param );

        //        ver($sql);

        $cabecalho 	= array("Ação11", "Tipo Arquivo", "Descrição", "Arquivo", "Data de Inclusão", "Gravado por","Fiscal","Empresa");
        $db->monta_lista($sql,$cabecalho,50,5,'N','center','', "formulario");
        //        $db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2, "formulario");

        ?>

    </form>
</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
            <?php
            //        ver($obrid, d);
            foreach( $responsaveisPossiveis as $responsavelPossivel ){

                if( $responsavelPossivel['rsuid'] != 4 )
                    monta_titulo( $responsavelPossivel['rsudsc'], '' );

                if( $responsavelPossivel['rsuid'] == 1 ){ // Instituição ############################################################################
                    $param               = array();
                    $param['not(emsid)'] = true;
                    $param['not(smiid)'] = true;

                    if( possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_CALL_CENTER) ){
                        $dados = $vistoria->listaDadosGestorUnidade( $obrid, $param );
                    }
                    else{
                        $dados = $vistoria->listaDados( $obrid, $param );
                    }

                    $cabecalho = array( "Ação",
                        //"Aditivo Vinculado",
                        "Ordem",
                        "ID",
                        "Data Acompanhamento",
                        "Data Inclusão",
                        "Inserido Por",
                        "Situação da Obra",
                        "Responsável",
                        "Realizada Por",
                        "% do Acompanhamento");

                    if( possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_CALL_CENTER) )
                        array_push( $cabecalho, "Validada pelo Supervisor" );
                    ?>

                <form name="formulario" id="formulario" method="post" onSubmit="return Validacao();" action="<?php echo $caminho_atual;?>acao=A">
                    <input type="hidden" name="requisicao" id="requisicao" value="executar"/>

                    <table class="tabela" align="center" style="width: 98%;<?php echo($responsavelPossivel['rsuid']!=1)?'display:none':''?>" id="responsavel_<?php echo$responsavelPossivel['rsuid']?>">
                        <tr>
                            <?php
                            $i = 1;
                            if(is_array($cabecalho)){
                                foreach($cabecalho as $indices => $titulos){
                                    echo '<th>'.$titulos.'</th>';

                                }
                            }
                            ?>
                        </tr>
                        <?php
                        if(is_array($dados)){

                            $stBotaoExcluir           = '<img src="/imagens/excluir_01.gif" style="cursor:pointer;" title="Não é possível excluir esse Acompanhamento" border="0" onclick=" alert(\'Não é possível excluir essa Acompanhamento\')"> ';
                            $stBotaoExcluirHabilitado = '<img src="/imagens/excluir.gif" 	style="cursor:pointer;" title="Excluir"                                    border="0" onclick="javascript:ExcluirVistoria(\'' . $caminho_atual . 'acao=A\', %s );">';

                            $inRegistros = count( $dados );

                            if( count($dados) < 1 ){
                                echo "<tr><td colspan='10' style='text-align:center'>Nenhum registro encontrado.</td></tr>";
                            }

                            $msgObs = false;
                            $data = getUltimaEdicaoCronograma($obrid);
                            foreach($dados as $chave){

                                $obs = '';
//                            ver($data, $chave['supdtinclusao']);
                                if($data !== false){
                                    if(strtotime($chave['supdtinclusao']) < strtotime($data)){

                                        if(verificaValorCronogramaSupervisao($obrid, $chave['supid'])) {
                                            $obs = '<span style="color: red">*</span>';
                                            $msgObs = true;
                                        }
                                    }
                                }


                                // Não informado
                                if( $chave["realizacaosupervisao_rsuid"] != $responsavelPossivel['rsuid'] )
                                    continue;
                                ?>

                                <tr <?php if($i%2) print("bgcolor=#f0f0f0") ?>>
                                    <td align="center">
                                        <?php

                                        //RN - Não permitir a edição/exclusão de vistorias pelo supervisor unidade ou gestor unidade 7 dias após a data de inclusão.
                                        $dataInicial  = explode('/',$chave["dtinclusao"]);
                                        $dataInicial  = $dataInicial[2].'-'.$dataInicial[1].'-'.$dataInicial[0];
                                        $dataFinal    = date('Y-m-d');
                                        $diffDatas    = ((((strtotime($dataFinal) - strtotime($dataInicial))/60)/60)/24);

                                        $validado     = (strtolower($chave["validadapelosupervisorunidade"]) == 's') ? true : false;

                                        $habilitaMods = ( ($diffDatas >= 7) && (possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE))  && ($validado == true) ) ? false : true;

                                        if ( $_GET['acao'] != 'V' && $habilitaMods === true ){

                                            echo '<img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;" onclick="javascript:AtualizarVistoria(\'?modulo=principal/inserir_vistoria&acao=A\', '.$chave["supid"].');">';


                                            /**
                                             * DEMANDA #229881
                                             */
                                            $perfil_cadastro = pegaPerfil($chave["usucpf"]);
                                            $perfil_sessao   = $_SESSION['usucpf'];
                                            $permissao_perfil_edicao = false;

                                            if($perfil_cadastro == PFLCOD_GESTOR_UNIDADE && $perfil_sessao == PFLCOD_SUPERVISOR_UNIDADE){
                                                $permissao_perfil_edicao = true;
                                            }

                                            if ( (possui_perfil(array(PFLCOD_SUPER_USUARIO)) && $inRegistros == $i) ){
                                                echo sprintf( $stBotaoExcluirHabilitado, $chave['supid'] );
                                            }else{
                                                echo $stBotaoExcluir;
                                            }
                                        }else{
                                            echo '<img src="/imagens/alterar.gif" border="0" title="Visualizar vistoria" style="cursor:pointer;" onclick="javascript:AtualizarVistoria(\'?modulo=principal/inserir_vistoria&acao=V\', '.$chave["supid"].');">';
                                            echo $stBotaoExcluir;
                                        }

                                        ?>

                                    </td>
                                    <td align="center">
                                        <?php print($i); ?>
                                    </td>
                                    <td align="center">
                                        <?php print($chave["supid"]); ?>
                                    </td>
                                    <td align="center">
                                        <?php print($chave["dtvistoria"]); ?>
                                    </td>
                                    <td align="center">
                                        <?php print($chave["dtinclusao"]); ?>
                                    </td>
                                    <td>
                                        <?php if( possuiPerfil(array(PFLCOD_SUPERVISOR_MEC, PFLCOD_EMPRESA_VISTORIADORA_GESTOR, PFLCOD_ADMINISTRADOR) ) ){ ?>
                                            <img border="0" onclick='envia_email("<?php echo $chave["usucpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                                        <?php } ?>
                                        <?php print($chave["usunome"]); ?>
                                    </td>
                                    <td>
                                        <?php print($chave["stadesc"]); ?>
                                    </td>
                                    <td>
                                        <?php if( !empty($chave["vistoriador"]) && ( possuiPerfil(array(PFLCOD_SUPERVISOR_MEC, PFLCOD_EMPRESA_VISTORIADORA_GESTOR, PFLCOD_ADMINISTRADOR) ) ) ){ ?>
                                            <img border="0" onclick='envia_email("<?php echo $chave["cpfvistoriador"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                                        <?php } ?>
                                        <?php !empty($chave["vistoriador"]) ? print($chave["vistoriador"]) : print('NÃO INFORMADO'); ?>
                                    </td>
                                    <td>
                                        <?php print($chave["responsavel"]); ?>
                                    </td>
                                    <td align="center">
                                        <?php
                                        $percentual = $chave["percentual"];
                                        $percentual = $percentual > 100.00 ? 100.00 : $percentual;
                                        print(number_format($percentual,2,',','.'));
                                        ?> % <?=$obs?>
                                    </td>
                                    <?php if( possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_CALL_CENTER) ): ?>
                                        <td align="center"><?php echo$chave['validado']?></td>
                                    <?php endif; ?>
                                </tr>
                                <?php $i++;
                            }
                        } ?>
                        <? if($msgObs): ?>
                            <tr>
                                <td colspan="11"> <span style="color: red"> *  Supervisão afetada pela alteração do cronograma. </span></td>
                            </tr>
                        <? endif;?>
                    </table>
                </form><?php

                }
                else if( $responsavelPossivel['rsuid'] == 2 ){ // MEC (FNDE) ############################################################################

                $supervisaoFnde = new SupervisaoFNDE();
                $param          = array();
                $param['obrid'] = $obrid;
                $sql = $supervisaoFnde->listaSql( $param );
                $cabecalho = array( "Ação",
                    "Situação da Supervisão",
                    "Data da Supervisão",
                    "Data de Inclusão",
                    "Responsável",
                    "Cargo do Responsável",
                    "Inserido Por",
                    "Unidade em Funcionamento?");

                $dados = $db->carregar( $sql ); ?>

                    <form id="formSupervisaoFnde" action="" method="post">
                        <input type="hidden" name="operacao" id="operacao" />
                        <input type="hidden" name="sfndeid" id="sfndeid" />
                    </form>
   
                    <table class="tabela" align="center" style="width: 98%;" id="responsavel_<?php echo$responsavelPossivel['rsuid']?>">
                        <tr>
                            <?php foreach($cabecalho as $titulos){ ?>
                                <th><?php echo  $titulos  ?></th>
                            <?php } ?>
                        </tr>

                        <?php
                        $totReg = count( $dados );
                        $i      = 1;

                        if( !$dados[0]['acao'] ){
                            echo "<tr><td colspan='10' style='text-align:center'>Nenhum registro encontrado.</td></tr>";
                        }

                        if( $dados[0]['acao'] ){
                            foreach($dados as $chave){ ?>

                                <tr <?php if( $i%2 ) print("bgcolor=#f0f0f0") ?>>
                                    <td align="center"><?php echo $chave['acao'] ?></td>
                                    <td align="center"><?php echo $chave['esddsc'] ?></td>
                                    <td align="center"><?php echo $chave['sfndedtsupervisao'] ?></td>
                                    <td align="center"><?php echo $chave['sfndedtcadastro'] ?></td>
                                    <td align="center"><?php echo $chave['entnome'] ?></td>
                                    <td align="center"><?php echo $chave['sfndecargorepresentante'] ?></td>
                                    <td align="center"><?php echo $chave['usunome'] ?></td>
                                    <td align="center"><?php echo $chave['sfndefuncionamento'] ?></td>
                                </tr>
                                <?php $i++;

                            }
                        }?>
                    </table> <?php

                }
                else if( $responsavelPossivel['rsuid'] == 3 ){ // Empresa ############################################################################

                $param             = array();
                $param['obrid']    = $obrid;
                $supervisaoEmpresa = new SupervisaoEmpresa();
                $dados             = $supervisaoEmpresa->listaDados($param);
                ?>

                    <form name="formularioEmpresa" id="formularioEmpresa" method="post" action="">
                        <input type="hidden" name="sueid"    id="sueid" value=""/>
                        <input type="hidden" name="operacao" id="operacao" value=""/>
                    </form>

                    <table class="tabela" align="center" style="width: 98%;" id="responsavel_<?php echo$responsavelPossivel['rsuid']?>">
                        <tr>
                            <?php
                            $cabecalho = array( "Ação",
                                "Nº OS",
                                "Situação da Supervisão",
                                "Data da Supervisão",
                                "Responsável",
                                "Cargo do Responsável",
                                "Inserido Por",
                                "% do Acompanhamento",
                                "Unidade em Funcionamento?");

                            foreach($cabecalho as $titulos){ ?>
                                <th><?php echo  $titulos  ?></th>
                            <?php } ?>
                        </tr>

                        <?php
                        $btnExc    = '<img src="/imagens/excluir_01.gif" border="0" style="cursor:pointer;" title="Não é possível excluir essa supervisão" onclick=" alert(\'Não é possível excluir essa supervisão\')"> ';
                        $btnExcHab = '<img src="/imagens/excluir.gif"    border="0" style="cursor:pointer;" title="Excluir"                                onclick="javascript:excluirSupervisao( %s );">';
                        $btnEdt    = '<img src="/imagens/alterar_pb.gif" border="0" style="cursor:pointer;" title="Não é possível editar essa supervisão"  onclick=" alert(\'Não é possível editar essa supervisão\')"> ';
                        $btnEdtHab = '<img src="/imagens/alterar.gif"    border="0" style="cursor:pointer;" title="Editar"                                 onclick="javascript:editarSupervisao( %s );">';
                        $btnPrtHab = '<img src="/imagens/print.png"      border="0" style="cursor:pointer;" title="Imprimir"                               onclick="javascript:imprimirLaudo( %s );"> ';

                        $btnPrintQuestionario = '<img style="cursor: pointer;" title="Imprimir questionário respondido" onclick="imprimirQuestionarioRespondido(%s, %s);" src="../imagens/print.png"/>';

                        $totReg = count( $dados );
                        $i      = 1;

                        if( count($dados) < 1 ){
                            echo "<tr><td colspan='10' style='text-align:center'>Nenhum registro encontrado.</td></tr>";
                        }

                        foreach($dados as $chave){ ?>

                            <tr <?php if( $i%2 ) print("bgcolor=#f0f0f0") ?>>
                                <td align="center">
                                    <?php

                                    $perfis_empresa = array( PFLCOD_EMPRESA_CONTRATADA,
                                        PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                                        PFLCOD_EMPRESA_VISTORIADORA_FISCAL);
                                    if($chave['esdid'] != WF_ESDID_LAUDO_SUPERVISAO_CANCELADO){
                                        if ($chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO && !possuiPerfil(PFLCOD_SUPER_USUARIO) && possuiPerfil($perfis_empresa)){
                                            echo $btnEdt . "&nbsp;";
                                            echo $btnPrtHab;
                                            //                                echo sprintf( $btnPrtHab, $chave['sueid'] );
                                        }else{
                                            if(!possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL))){

                                                if ( $totReg != $i || possui_perfil(array(PFLCOD_CALL_CENTER)) ){
                                                    echo $btnEdt . "&nbsp;";
                                                    echo $btnExc;
                                                }else{
                                                    echo sprintf($btnEdtHab . "&nbsp;", $chave['sueid']);
                                                    echo sprintf($btnExcHab, $chave['sueid']);
                                                }
                                                if(!empty($chave['esdid'])){echo '&nbsp;'.sprintf( $btnPrtHab, $chave['sueid'] );};
                                            }

                                            if ( ($chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO || $chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO || $chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_PAGO ) && is_numeric($chave['sueid']) && is_numeric($chave['sosid'])) {
                                                echo '&nbsp;'.sprintf( $btnPrintQuestionario, $chave['sueid'], $chave['sosid']);
                                            }
                                        }
                                    }
                                    ?>
                                </td>
                                <td>
                                    <?php print($chave["sosnum"]); ?>
                                </td>
                                <td>
                                    <?php print($chave["esddsc"]); ?>
                                </td>
                                <td align="center">
                                    <?php print($chave["suedtsupervisao"]); ?>
                                </td>
                                <td>
                                    <img border="0" onclick="envia_email('<?php echo $chave["entcpf"] ?>');"  title="Enviar e-mail ao Responsável" src="../imagens/email.gif" style="cursor: pointer;"/>
                                    <?php print($chave["entnome"]); ?>
                                </td>
                                <td>
                                    <?php print($chave["suecargovistoriador"]); ?>
                                </td>
                                <td>
                                    <img border="0" onclick="envia_email('<?php echo $chave["usucpf"] ?>');" title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                                    <?php print($chave["usunome"]); ?>
                                </td>
                                <td align="center">
                                    <?php $percentual = $chave["percentual"];
                                    $percentual = $percentual > 100.00 ? 100.00 : $percentual;
                                    print(number_format($percentual,2,',','.')); ?> %
                                </td>
                                <td align="center">
                                    <?php print($chave["suefuncionamento"]); ?>
                                </td>
                            </tr>
                            <?php $i++;

                        } ?>
                    </table> <?php

                } // else if( $responsavelPossivel['rsuid'] == 4 ){ // Não informado (Instituição que não foi informado) ###############################
                // }


            }

//            monta_titulo_obras('Empresa MI', '');
            ?>

            <table class="tabela" style="width: 98%;display:none" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="responsavel_4">
                <thead>
                <tr>
                    <th>Ação</th>
                    <th>Data da Evolução</th>
                    <th>Data de Inclusão</th>
                    <th>Responsável</th>
                    <!--  (Edificação) -->
                    <th>% Medido (Edificação)</th>
                    <th>% Validado (Edificação)</th>
                    <th>Valor Medido (R$) (Edificação)</th>
                    <th>Valor Validado (R$) (Edificação)</th>
                    <!--  (Serviços Externos) -->
                    <th>% Medido (Serviços Externos)</th>
                    <th>% Validado (Serviços Externos)</th>
                    <th>Valor Medido (R$) (Serviços Externos)</th>
                    <th>Valor Validado (R$) (Serviços Externos)</th>
                    <th>Possui Foto</th>
                    <th>Situação</th>
                    <th>Última tramitação</th>
                </tr>
                </thead>
                <tbody>
                <?php

                $param['where']['obrid'] = $obrid;
                $param['where']['emistatus'] = 'A';
                $evmi = new EvolucaoMi();
                $lista = $evmi->listaArray($param, true);

                if (empty($lista)) {
                    echo '<tr>
                      <td colspan="15" style="color:red; text-align: center"> Nenhuma evolução cadastrada. </td>
                  </tr>';
                } else {
                    $counter = 0;
                    $str_bg  = '';
                    foreach ($lista as $k => $v) {
                        $counter++;
                        $str_bg = ($counter % 2 == '') ? '' :' bgcolor="#FFFFFF" ';

                        $datae = explode('-', $v['emidtevolucao']);
                        $datae = $datae[2].'/'.$datae[1].'/'.$datae[0];

                        $datai = explode('-', $v['emidtinclusao']);
                        $datai = $datai[2].'/'.$datai[1].'/'.$datai[0];

                        echo '
                    <tr'.$str_bg.'>
                        <td style="width: 50px;text-align: center">'.$v['acao'].'</td>
                        <td style="text-align: center">'.$datae.'</td>
                        <td style="text-align: center">'.$datai.'</td>
                        <td style="text-align: center">'.$v['usunome'].'</td>
                        <!--  (Edificação) -->
                        <td style="text-align: center">'.number_format($v['emipecentmedidoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emipecentvalidadoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlmedidoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlvalidadoedif'], 2, ',','.').'</td>
                        <!--  (Serviços Externos) -->
                        <td style="text-align: center">'.number_format($v['emipecentmedidoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emipecentvalidadoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlmedidoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlvalidadoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.$v['possuifoto'].'</td>
                        <td style="text-align: center">'.$v['situacao'].'</td>
                        <td style="text-align: center">'.$v['ultima_tramitacao'].'</td>
                    </tr>
                  ';
                    }
                }
                ?>
                </tbody>
                <tfoot>
                <tr>
                    <th colspan="15">&nbsp;</th>
                </tr>
                <tr>
                    <td colspan="15"> &nbsp; </td>
                </tr>
                <tr>
                    <td colspan="14" style="text-align: center">
                        <input type="button" value="Cadastrar Nova Evolução" class="novaEvolucao">
                        <input type="button" value="Voltar para a Obra"     class="goObra">
                    </td>
                </tr>
                </tfoot>
            </table>

            <form id="formSupervisaoMi" action="" method="post">
                <center>
                    <input type="hidden" name="op" id="op">
                    <input type="hidden" name="smiid" id="smiid">
                </center>
            </form>
        </td>
    </tr>
    <?php
    if ($habilitado) {
        echo '
    <tr class="divTituloPadrao">
        <td>
            <div style="float: left;">';
        $obraContrato 	  = new ObrasContrato();
        $ocrvalorexecucao = $obraContrato->getValorContrato( $obrid );

        $itemComposicao = new ItensComposicaoObras();
        $somaIcoValor   = $itemComposicao->getSomaEtapaByObra( $obrid );

        // !possui_perfil(PFLCOD_GESTOR_UNIDADE)
        if ( ($ocrvalorexecucao == $somaIcoValor && $esdid != ESDID_OBJ_ADITIVO) || ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) ){
            if( possuiPerfil(array(PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_CALL_CENTER)) && !possuiPerfil(array(PFLCOD_SUPER_USUARIO)) ) {
                echo '<input '.$desabilitaBotao.' style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Acompanhamento" onclick="window.location=\'?modulo=principal/cadEvolucaoMi&acao=A\'" />';
            }else{

                //$obrasArquivos 	= new ObrasArquivos();
                $param = array("obrid" => $obrid/*, "not(img)" => true*/);

                //valida tipo execução
                $param['tpaid(IN)'] = '25';
                $sql = $obrasArquivos->listaSqlVistoria( $param );

                $valida_execucao = $db->carregar($sql);
                //var_dump($valida_execucao);die();

                //valida tipo fiscalização
                $param['tpaid(IN)'] = '26';
                $sql = $obrasArquivos->listaSqlVistoria( $param );

                $valida_fiscalizacao = $db->carregar($sql);
                //ver($valida_fiscalizacao,d);

                if((!$valida_execucao || !$valida_fiscalizacao) && $esdid != ESDID_OBJ_INACABADA)
                {
                    $desabilitaBotao ='disabled="disabled"';
                }

                if($esdid == ESDID_OBJ_REFORMULACAO){
                    $desabilitaBotao ='disabled="disabled"';
                }

                echo '<input '.$desabilitaBotao.' style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Acompanhamento" onclick="window.location=\'?modulo=principal/inserir_vistoria&acao=A\'" />';

                if((!$valida_execucao || !$valida_fiscalizacao) && $esdid != ESDID_OBJ_INACABADA)
                {
                    echo "<span style='margin-left: 10px;font-weight: bold;color: #F00000'>Para inserir um acompanhamento é necessário inserir o ART de Execução e Fiscalização.</span>";
                }
            }
        }elseif ( $esdid == ESDID_OBJ_ADITIVO ){
            echo '&nbsp;&nbsp;&nbsp;<font style="color: red;">Não é possivel inserir vistoria quando a obra está em celebração de aditivo.</font>';
        }else{
            echo '&nbsp;&nbsp;&nbsp;<font style="color: red;">Valor do <b>cronograma</b> deve ser igual ao valor do <b>contrato</b> para liberar a <b>vistoria</b>.</font>';
        }
        echo '
            </div>
        </td>
    </tr>';
    }elseif($str_trava_cronograma_vistoria != ''){
        echo '
    <tr bgcolor="#C0C0C0">
        <td>
            <div style="float: left;">
            &nbsp;&nbsp;&nbsp;
            <font style="color: red;">'.$str_trava_cronograma_vistoria.'</font>
            </div>
        </td>
    </tr>';
    }else{
        echo '
    <tr bgcolor="#C0C0C0">
        <td>
            <div style="float: left;">
            &nbsp;&nbsp;&nbsp;
            <font style="color: red;">Só é possível inserir uma vistoria quando a obra estiver em execução, paralisada, inacabada ou concluída.</font>
            </div>
        </td>
    </tr>';
    } ?>
</table>

<?php

require APPRAIZ . 'obras2/includes/principal/vistoria/js.php';

$objObras = new Obras();
$objObras->carregarPorIdCache($obrid);
$blockEdicao = $objObras->verificaObraVinculada();
if($blockEdicao){
    echo '<script type="text/javascript">';
    echo " setTimeout(function (){
                        var forms = ['formulario_art','formulario','formSupervisaoFnde','formularioEmpresa','formSupervisaoMi'];
                        for(i=0; i<forms.length; i++ ){
                            bloqueiaForm(forms[0]);
                        }
                   }, 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).attr('disabled','disabled');
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('[name*=\"responsavel_\"]').find('img').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                   }
                 ";
    echo '</script>';
}
?>


