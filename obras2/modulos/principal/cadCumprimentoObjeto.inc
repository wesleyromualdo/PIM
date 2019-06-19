<?php
    require APPRAIZ . 'obras2/includes/principal/cadCumprimentoObjeto/ctrl.php';
?>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

    <?php 
        require APPRAIZ . 'obras2/includes/principal/cadCumprimentoObjeto/css.php';
    ?>

    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

    
    <div id="modalEsclarecimento" style="display:none" title="Esclarecimentos Adicionais">
        <?= campo_textarea('textoModalEsclarecimento', 'N', 'S', '', 100, 20, '', '', '', '', '', '', '', $param); ?>
    </div>

<?php  

    $detalheTitulo = '
        <b>
            1 - O Cumprimento de Objeto faz parte da Prestação de Contas;<br>
            2 - É necessário que não haja pendências nesta aba, situação que ocorre
                quando o ícone <img src="/imagens/valida1.gif"> aparece no campo "observação";<br>
            3 - Ao concluir o preenchimento desta aba, a aba "Questionário" será liberada para preenchimento;<br>
            4 - Caso o ente não possua o documento solicitado deverá inserir declaração
                justificando o fato, assinada pelo Gestor responsável (Prefeito ou Secretário de Educação no caso de Estado), na respectiva questão;<br>
            5 - Após responder todas as questões da aba "Questionário" deverá ser
                tramitada para análise do FNDE. <br>
            6 - As fotos de todos os ambientes da obra concluída devem ser inseridas na aba "Vistoria".  Caso essa aba não esteja disponível, 
                inserir um relatório fotográfico, em formato .pdf, na aba "Documentos"; <br>
            7 - Em caso de dúvidas, entrar em contato pelo e-mail <a href="mailto:cumprimentodoobjeto@fnde.gov.br" target="_top">cumprimentodoobjeto@fnde.gov.br</a>
        </b>
        ';

    monta_titulo('Documentação', $detalheTitulo);


?>

<?php if (!$habilPag && possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
    <div class="row">
        <div class="col-md-12" align="center">
            <span style="background: #CCC; color: #fff; padding:10px; text-align: center;">
                Deseja forçar o liberamento do Cumprimento desta obra (<?= $obra->obrid ?>) <?= $obra->obrnome ?>? <button
                        type="button" id="liberaCOO" class="btn btn-xs blue"> SIM </button>
            </span>
        </div>
    </div>

    <?php
else :
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) { ?>
        <div class="row">
            <div class="col-md-12" align="center">
            <span style="background: #CCC; color: #fff; padding:10px; text-align: center;">
                 Deseja forçar o bloqueio do Cumprimento desta obra (<?= $obra->obrid ?>) <?= $obra->obrnome ?>? <button
                        type="button" id="bloqueioCOO" class="btn btn-xs blue"> SIM </button>
            </span>
            </div>
        </div>
        <script type="text/javascript">
            $(function () {
                $('#bloqueioCOO').click(function (e) {
                    location.href = location.href + '&bloqueiacoo=true';
                })
            });
        </script>
        <br>
    <?php } ?>

    <div class="col-md-12" align="center">
        <span>
        <?php include APPRAIZ . 'obras2/modulos/principal/contadorDiasCumprimentoObjeto.inc'; ?>
        </span>
    </div>

    <?php
   
    $questionarioDocumento = (object)$cumprimento['questionario'];
    $questionarioConstrutora = (object)$cumprimento['questionarioConstrutora'];
    $vinculadas = $cumprimento['vinculadas'];
    $construtorasextras = $cumprimento['extras'];

    if ($wfEstadoAtualCumprimento['esdid'] == ESDID_CUMPRIMENTO_DILIGENCIADO) {
        $comentario = wf_pegarComentarioEstadoAtual($cumprimentoObjeto->docid); ?>
        <div class="col-md-12" style="">
            <div class="alert alert-danger text-center">
                O Cumprimento do Objeto foi diligênciado pelo motivo: <br/>
                <h3><?= $comentario ?></h3>
            </div>
        </div>
        <?php
    } else {
        if ($wfEstadoAtualCumprimento['esdid'] == ESDID_CUMPRIMENTO_CADASTRAMENTO) {
            if (condicaoQuestionarioCumprimentoObjeto($cumprimentoObjeto->docid) !== true) {
                ?>
                <div class="col-md-12" style="">
                    <div class="alert alert-warning text-center">
                        A Aba "Documentação" está completamente preenchida. <br/>
                        <a style="color: #c09853"
                           href="obras2.php?modulo=principal/cadQuestionarioCumprimentoObjeto&acao=A" alt=""
                           title="Enviar para Validação FNDE"><h3>Clique aqui para preencher a aba "Questionário"</h3>
                        </a>
                    </div>
                </div>
                <?php
            } else {
                if (condicaoCumprimentoObjeto($cumprimentoObjeto->docid) === true) {
                    ?>
                    <div class="col-md-12" style="">
                        <div class="alert alert-success text-center">
                            O Cumprimento do Objeto esta completamente preenchido. <br/>
                            <a style="color: #468847" href="#" alt="Enviar para Validação FNDE"
                               title="Enviar para Validação FNDE" onclick="$('#td_acao_2913 a').click();"><h3>Clique
                                    aqui para enviar para Validação do FNDE</h3></a>
                        </div>
                    </div>
                    <?php
                }
            }
        }
    }
    ?>
    <div class="row" style="position:static;">
        <div class="col-md-10" style="position:inherit;">
            <!-- Formulário de Documentos da Obra -->
            <form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
                <input type="hidden" name="req" value="salvar">
                <input type="hidden" name="codid" value="<?= $questionarioDocumento->codid; ?>">
                <input type="hidden" name="coid" value="<?= $coid ?>">
                <input type="hidden" name="obrid" value="<?= $questionarioDocumento->obrid; ?>">
                <input type="hidden" name="itcid" value="<?= $questionarioDocumento->itcid; ?>">
                <table class="tabela documentos" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
                    <thead>
                    <tr>
                        <th colspan="8"><?= $questionarioDocumento->descricaoItem; ?></th>
                    </tr>
                    <tr>
                        <td style="text-align:center;font-weight: bold;">Questão</td>
                        <td style="text-align:center;font-weight: bold;" colspan="3">Resposta</td> 
                        <td style="text-align:center;font-weight: bold;">Situação do Item</td>
                        <td style="text-align:center;font-weight: bold;">Arquivos</td>
                        <td style="text-align:center;font-weight: bold;">Ação</td>
                    </tr>
                    </thead>
                    <tbody>
                    <?php 
                    if ($questionarioDocumento->questoes) {
                        foreach ($questionarioDocumento->questoes as $questao) : ?>
                        <tr>
                            <td width="20%" <?php if ($questao->qcodNumero == 9) {
                                ?> rowspan="2" <?php
                                            } ?>>
                                <span><?= $questao->qcodNumero ?></span>)<?= $questao->qcodDsc; ?>
                                <a title="<?php echo $arrEsclarecimentoObra[$questao->qcodNumero]; ?>"><img
                                            src="/imagens/question.png"
                                            title="<?php echo $arrEsclarecimentoObra[$questao->qcodNumero]; ?>"></a>
                            </td>
                            <td width="4%" <?php if ($questao->qcodNumero == 9) {
                                ?> rowspan="2" <?php
                                           } ?>>
                                <input type="radio" disabled name="qcodresposta[<?= $questao->qstId; ?>]"
                                       value="t" <?= $questao->respostaTrue; ?>> SIM
                            </td>
                            <td width="4%" <?php if ($questao->qcodNumero == 9) {
                                ?> rowspan="2" <?php
                                           } ?>>
                                <input type="radio" disabled name="qcodresposta[<?= $questao->qstId; ?>]"
                                       value="f" <?= $questao->respostaNA && $questao->qcodNumero == 8 ? $questao->respostaNA : $questao->respostaFalse; ?>>
                                NÃO
                            </td>
                            <td width="8%" <?php if ($questao->qcodNumero == 9) {
                                ?> rowspan="2" <?php
                                           } ?>>
                                <input type="radio" disabled name="qcodresposta[<?= $questao->qstId; ?>]"
                                       value="n" <?= $questao->respostaNA; ?>> NÃO SE APLICA
                            </td>
                            <td class="observacao" <?php if ($questao->qcodNumero == 9) {
                                ?> rowspan="2" <?php
                                                   } ?>>
                                <!-- O item 6 possui regra oposta. Quando SIM, apresentar a mensagem. -->
                                <p>
                                    <img title="<?= htmlspecialchars($questao->qstObs); ?>"
                                         src="/imagens/<?= $questao->qcodResposta == 't' && $questao->qcodNumero == 6 ? 'valida5.gif' : ($questao->qcodNumero == 6 ? 'valida1.gif' : $questao->imgValidacao); ?>"/>
                                        <?= $questao->qcodResposta == 'f' && $questao->qcodNumero != 6 ? $questao->qstObs : ($questao->qcodResposta == 't' && $questao->qcodNumero == 6 ? $questao->qstObs : ''); ?>
                                </p>
                            </td>
                            <td <?php if ($questao->qcodNumero == 9) {
                                ?> rowspan="2" <?php
                                } ?>>
                                    <?php
                                    if ($questao->arquivos) {
                                        foreach ($questao->arquivos as $arquivo) :
                                            #Se puder remover arquivo.
                                            if ($arquivo['del']) :
                                                ?>
                                            <a class="link" onclick="excluirArquivo('<?= $arquivo['arqid'] ?>');">
                                                <img src="/imagens/excluir.gif"
                                                     title="Excluir arquivo <?= $arquivo['arqnome'] ?>">
                                            </a>
                                            <?php endif; ?>
                                        <a class="link"
                                           onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?= $arquivo['arqid']; ?>'">
                                            <img src='/imagens/salvar.png' border='0'>
                                            <?= $arquivo['arqnome'] . '.' . $arquivo['arqextensao'] ?>
                                        </a><br>
                                        <?php endforeach;
                                    }
                                    #Excessão à questão número 3.
                                    if ($questao->qcodNumero == 3 && $questao->qcodResposta != 'n' && ($obra->preid || $obra->obrid_par3)) : ?>
                                    <a class="link" id="janela">
                                        <img src="/imagens/IconeAjuda.gif">
                                        <b style="vertical-align: sub;">Planilha Orçamentária</b>
                                    </a>
                                    <?php endif; ?>
                            </td>

                            <td width="15%">
                                <input type="hidden" name="qcodresposta[<?= $questao->qstId ?>]"
                                       value="<?= $questao->qcodResposta; ?>">
                                    <?php
                                    if ($questao->qcodResposta != 'n' && $questao->qcodNumero != '4' && $questao->qcodNumero != '6') : ?>
                                    <input type="file" name="<?= $questao->qstId; ?>">
                                    <?php endif; ?>
                            </td>

                        </tr>
                            <?php if ($questao->qcodNumero == 9) { ?>
                            <tr>
                                <td><b>Valor dos recursos<br/> devolvidos ao FNDE:</b>
                                    R$ <?php echo campo_texto(
                                        'devvalor',
                                        'N',
                                        'false',
                                        '',
                                        10,
                                        15,
                                        '###.###.###,##',
                                        '',
                                        'left',
                                        '',
                                        0,
                                        'id="devvalor"',
                                        '',
                                        number_format($cumprimento['devolvidoGru']['devvalor'], 2, ',', '.'),
                                        ''
                                    ); ?>
                                </td>
                            </tr>

                            <?php } ?>

                        <?php endforeach; ?>

                    </tbody>
                    <tfoot>
                    <tr style="background-color:#C0C0C0;">
                        <td align="center" colspan="8">
                            <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
                        </td>
                    </tr>
                    </tfoot>
                    <?php } ?>
                </table>
            </form>
            <br/>
            <!-- FIM Formulário de Documentos da Obra -->
            <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
                <thead>
                <tr style="background-color:#C0C0C0;">
                    <td style="text-align: center; font-weight: bold; font-size: 12px;">
                        Deseja incluir mais uma construtora? <input type="button" id="addConstrutora"
                                                                    onclick="addConstrutora()" value="Sim"
                                                                    style="cursor: pointer"/>
                    </td>
                </tr>
                </thead>
            </table>
            <br/>
            <!-- Formulário de Construtora -->
        <?php
        if($obra->obrid != ''){ 

            $getContratoObra = $db->pegaUm(getSqlObrasContrato($questionarioConstrutora->obrid));
        }
        if ($getContratoObra != '' ) {?>
            <form method="post" id="formularioativa" name="formularioativa" enctype="multipart/form-data">
                <input type="hidden" name="req" value="salvar">
                <input type="hidden" name="codid" value="<?= $questionarioConstrutora->codid; ?>">
                <input type="hidden" name="coid" value="<?= $coid; ?>">
                <input type="hidden" name="obrid" value="<?= $questionarioConstrutora->obrid; ?>">
                <input type="hidden" name="itcid" value="<?= $questionarioConstrutora->itcid; ?>">
                <table class="tabela documentos" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
                    <thead>
                    <tr>
                        <th colspan="8"><?= $questionarioConstrutora->descricaoItem; ?></th>
                    </tr>
                    <tr>
                        <td style="text-align:center;font-weight: bold;">Questão</td>
                        <td style="text-align:center;font-weight: bold;" colspan="3">Resposta</td> 
                        <td style="text-align:center;font-weight: bold;">Situação do Item</td>
                        <td style="text-align:center;font-weight: bold;">Arquivos</td>
                        <td style="text-align:center;font-weight: bold;">Ação</td>
                    </tr>
                    </thead>
                    <tbody>
                    <?php
                    if ($questionarioConstrutora->questoes) {
                        foreach ($questionarioConstrutora->questoes as $questao) : ?>
                        <tr>
                            <td width="20%">
                                <span><?= $questao->qcodNumero ?></span>) <?= $questao->qcodDsc; ?>
                                <a title="<?php echo $arrEsclarecimentoConstrutora[$questao->qcodNumero]; ?>"><img
                                            src="/imagens/question.png"
                                            title="<?php echo $arrEsclarecimentoConstrutora[$questao->qcodNumero]; ?>"></a>
                            </td>
                            <td width="4%">
                                <input type="radio" disabled name="qcodresposta[<?= $questao->qstId; ?>]"
                                       value="t" <?= $questao->respostaTrue; ?>> SIM
                            </td>
                            <td width="4%">
                                <input type="radio" disabled name="qcodresposta[<?= $questao->qstId; ?>]"
                                       value="f" <?= $questao->respostaFalse; ?>> NÃO
                            </td>
                            <td width="8%">
                                <input type="radio" disabled name="qcodresposta[<?= $questao->qstId; ?>]"
                                       value="n" <?= $questao->respostaNA; ?>> NÃO SE APLICA
                            </td>
                            <td class="observacao">
                                <p><img title='<?= $questao->qstObs; ?>'
                                        src="/imagens/<?= $questao->imgValidacao; ?>"/> <?= $questao->qcodResposta == 'f' ? $questao->qstObs : ''; ?>
                                </p>
                            </td>
                            <td>
                                    <?php
                                    if ($questao->arquivos) {
                                        foreach ($questao->arquivos as $arquivo) :
                                            $caminho = APPRAIZ . "arquivos/obras2/" . floor($arquivo['arqid'] / 1000) . '/' . $arquivo['arqid'];
                                            if (file_exists($caminho)) {
                                                if ($arquivo['del']) : ?>
                                                <a class="link" onclick="excluirArquivo('<?= $arquivo['arqid']; ?>');">
                                                    <img src="/imagens/excluir.gif"
                                                         title="Excluir arquivo <?= $arquivo['arqnome']; ?>">
                                                </a>
                                                <?php endif; ?>
                                            <a class="link"
                                               onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?= $arquivo['arqid']; ?>'">
                                                <img src='/imagens/salvar.png' border='0'>
                                                    <?= $arquivo['arqnome'] . '.' . $arquivo['arqextensao']; ?>
                                            </a><br>
                                            <?php }
                                        endforeach;
                                    }
                                    ?>
                            </td>
                            <td>
                                <input type="hidden" name="qcodresposta[<?= $questao->qstId; ?>]"
                                       value="<?= $questao->qcodResposta; ?>">
                                <?php if ($questao->qcodResposta != 'n') : ?>
                                    <input type="file" name="<?= $questao->qstId; ?>">
                                <?php endif; ?>
                            </td>
                        </tr>
                        <?php endforeach; ?>
                    </tbody>
                    <tfoot>
                    <tr style="background-color:#C0C0C0;">
                        <td align="center" colspan="8">
                            <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
                        </td>
                    </tr>
                    </tfoot>
                    <?php } ?>
                </table>
            </form>
            <?}?> 

            <?php
         foreach ($vinculadas as $vinculada) :
                    $questionarioConstrutoraVinculada = (object)$vinculada['questionarioConstrutora'];

                    if($obra->obridvinculado != '' ){
                        
                        $getContratoObraVinculada = $db->pegaUm(getSqlObrasContrato($questionarioConstrutoraVinculada->obrid));
                    }
            if ($vinculadas && $getContratoObraVinculada != '' ) {
                ?>
                    <!-- Formulário da Construtora Obra Vinculada -->
                    <form method="post" id="formulariovinculada" name="formulariovinculada"
                          enctype="multipart/form-data">
                        <input type="hidden" name="req" value="salvar">
                        <input type="hidden" name="codid" value="<?= $questionarioConstrutoraVinculada->codid; ?>">
                        <input type="hidden" name="coid" value="<?= $coid; ?>">
                        <input type="hidden" name="obridvinculado"
                               value="<?= $questionarioConstrutoraVinculada->obrid; ?>">
                        <input type="hidden" name="obrid" value="<?= $questionarioConstrutoraVinculada->obrid; ?>">
                        <input type="hidden" name="itcid" value="<?= $questionarioConstrutoraVinculada->itcid; ?>">
                        <table class="tabela documentos" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4"
                               align="center">
                            <thead>
                            <tr>
                                <th colspan="7"><?= $questionarioConstrutoraVinculada->descricaoItem ?></th>
                            </tr>
                            <tr>
                                <td style="text-align:center;font-weight: bold;">Questão</td>
                                <td style="text-align:center;font-weight: bold;" colspan="3">Resposta</td> 
                                <td style="text-align:center;font-weight: bold;">Situação do item</td>
                                <td style="text-align:center;font-weight: bold;">Arquivos</td>
                                <td style="text-align:center;font-weight: bold;">Ação</td>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            if ($questionarioConstrutoraVinculada->questoes) {
                                foreach ($questionarioConstrutoraVinculada->questoes as $questaoVinculada) : ?>
                                    <tr>
                                        <td width="20%">
                                            <span><?= $questaoVinculada->qcodNumero ?></span>) <?= $questaoVinculada->qcodDsc; ?>
                                            <a title="<?php echo $arrEsclarecimentoConstrutora[$questaoVinculada->qcodNumero]; ?>"><img
                                                        src="/imagens/question.png"
                                                        title="<?php echo $arrEsclarecimentoConstrutora[$questaoVinculada->qcodNumero]; ?>"></a>
                                        </td>
                                        <td width="4%">
                                            <input type="radio" disabled
                                                   name="qcodresposta[<?= $questaoVinculada->qstId; ?>]"
                                                   value="t" <?= $questaoVinculada->respostaTrue ?>> SIM
                                        </td>
                                        <td width="4%">
                                            <input type="radio" disabled
                                                   name="qcodresposta[<?= $questaoVinculada->qstId; ?>]"
                                                   value="f" <?= $questaoVinculada->respostaFalse ?>> NÃO
                                        </td>
                                        <td width="8%">
                                            <input type="radio" disabled
                                                   name="qcodresposta[<?= $questaoVinculada->qstId; ?>]"
                                                   value="n" <?= $questaoVinculada->respostaNA ?>> NÃO SE APLICA
                                        <td class="observacao">
                                            <p><img title="<?= $questaoVinculada->qstObs ?>"
                                                    src="/imagens/<?= $questaoVinculada->imgValidacao; ?>"/> <?= $questaoVinculada->qcodResposta == 'f' ? $questaoVinculada->qstObs : ''; ?>
                                            </p>
                                        </td>
                                        <td>
                                            <?php
                                            if ($questaoVinculada->arquivos) {
                                                foreach ($questaoVinculada->arquivos as $arquivo) :
                                                    if ($arquivo['del']) :
                                                        ?>
                                                        <a class="link"
                                                           onclick="excluirArquivo('<?= $arquivo['arqid'] ?>');">
                                                            <img src="/imagens/excluir.gif"
                                                                 title="Excluir arquivo <?= $arquivo['arqnome']; ?>">
                                                        </a>
                                                        <?php
                                                    endif;
                                                    ?>
                                                    <a class="link"
                                                       onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?= $arquivo['arqid']; ?>'">
                                                        <img src='/imagens/salvar.png' border='0'>
                                                        <?= $arquivo['arqnome'] . '.' . $arquivo['arqextensao']; ?>
                                                    </a><br>
                                                <?php endforeach;
                                            }
                                            ?>
                                        </td>
                                        <td>
                                            <input type="hidden" name="qcodresposta[<?= $questaoVinculada->qstId; ?>]"
                                                   value="<?= $questaoVinculada->qcodResposta; ?>">
                                            <?php if ($questaoVinculada->qcodResposta != 'n') : ?>
                                                <input type="file" name="<?= $questaoVinculada->qstId; ?>">
                                            <?php endif; ?>
                                        </td>
                                    </tr>
                                <?php endforeach;
                            } ?>
                            </tbody>
                            <tfoot>
                            <tr style="background-color:#C0C0C0;">
                                <td align="center" colspan="7">
                                    <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
                                </td>
                            </tr>
                            </tfoot>
                        </table>
                    </form>
                    <!-- Fim Formulário Construtora Obra Vinculada -->
                <?php }
                endforeach;
             ?>

            <!-- Construtoras extras -->
            <?php
            if ($construtorasextras) {
                foreach ($construtorasextras as $construtora) :
                    $questionarioConstrutoraExtra = (object)$construtora['questionarioConstrutoraExtra']; ?>
                    <!-- Formulário da Construtora Obra Vinculada -->
                    <form method="post" id="formularioextra" name="formularioextra" enctype="multipart/form-data">
                        <input type="hidden" name="req" value="salvar">
                        <input type="hidden" name="codid" value="<?= $questionarioConstrutoraExtra->codid; ?>">
                        <input type="hidden" name="coid" value="<?= $coid; ?>">
                        <input type="hidden" name="obridvinculado" value="<?= $questionarioConstrutoraExtra->obrid; ?>">
                        <input type="hidden" name="obrid" value="<?= $questionarioConstrutoraExtra->obrid; ?>">
                        <input type="hidden" name="itcid" value="<?= $questionarioConstrutoraExtra->itcid; ?>">
                        <input type="hidden" name="cexid" value="<?= $questionarioConstrutoraExtra->cexid; ?>">
                        <table class="tabela documentos" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4"
                               align="center">
                            <thead>
                            <tr>
                                <th colspan="7"><?= $questionarioConstrutoraExtra->descricaoItem; ?></th>
                            </tr>
                            <tr>
                                <td style="text-align:center;font-weight: bold;">Questão</td>
                                <td style="text-align:center;font-weight: bold;" colspan="2">Resposta</td> 
                                <td style="text-align:center;font-weight: bold;">Situação do item</td>
                                <td style="text-align:center;font-weight: bold;">Arquivos</td>
                                <td style="text-align:center;font-weight: bold;">Ação</td>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            if ($questionarioConstrutoraExtra->questoes) {
                                foreach ($questionarioConstrutoraExtra->questoes as $questaoExtra) :
                                    if (!empty($questaoExtra->arquivos)) {
                                        $checkNao = '';
                                        $checkSim = "checked";
                                    } else {
                                        $checkSim = '';
                                        $checkNao = "checked";
                                    }
                                    ?>
                                    <tr>
                                        <td width="20%">
                                            <span><?= $questaoExtra->qcodNumero ?></span>) <?= $questaoExtra->qcodDsc; ?>
                                            <a title="<?php echo $arrEsclarecimentoConstrutora[$questaoExtra->qcodNumero]; ?>"><img
                                                        src="/imagens/question.png"
                                                        title="<?php echo $arrEsclarecimentoConstrutora[$questaoExtra->qcodNumero]; ?>"></a>
                                        </td>
                                        <td width="4%">
                                            <input type="radio" disabled
                                                   name="qcodresposta[<?= $questaoExtra->qstId; ?>]"
                                                   value="t" <?= $checkSim; ?>> SIM
                                        </td>
                                        <td width="4%">
                                            <input type="radio" disabled
                                                   name="qcodresposta[<?= $questaoExtra->qstId; ?>]"
                                                   value="f" <?= $checkNao; ?>> NÃO
                                        </td>

                                        <td class="observacao">
                                            <?php if (!empty($questaoExtra->arquivos)) { ?>
                                                <p><img title="<?= $questaoExtra->qstObs; ?>"
                                                        src="/imagens/<?= $questaoExtra->imgValidacao; ?>"/> <?= $questaoExtra->qcodResposta == 'f' ? $questaoExtra->qstObs : ''; ?>
                                                </p>
                                            <?php } else { ?>
                                                <p>
                                                    <img title="<?php echo $arrSituacaoDoItem[$questaoExtra->qcodNumero]; ?>"
                                                         src="/imagens/valida5.gif"/> <?php echo $arrSituacaoDoItem[$questaoExtra->qcodNumero]; ?>
                                                </p>
                                            <?php } ?>
                                        </td>
                                        <td>
                                            <?php
                                            if ($questaoExtra->arquivos) {
                                                foreach ($questaoExtra->arquivos as $arquivo) :
                                                    if ($arquivo['del']) :
                                                        ?>
                                                        <a class="link"
                                                           onclick="excluirArquivo('<?= $arquivo['arqid']; ?>');">
                                                            <img src="/imagens/excluir.gif"
                                                                 title="Excluir arquivo <?= $arquivo['arqnome']; ?>">
                                                        </a>
                                                        <?php
                                                    endif;
                                                    ?>
                                                    <a class="link"
                                                       onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?= $arquivo['arqid']; ?>'">
                                                        <img src='/imagens/salvar.png' border='0'>
                                                        <?= $arquivo['arqnome'] . '.' . $arquivo['arqextensao']; ?>
                                                    </a><br>
                                                <?php endforeach;
                                            }
                                            ?>
                                        </td>
                                        <td>
                                            <input type="hidden" name="qcodresposta[<?= $questaoExtra->qstId; ?>]"
                                                   value="<?= $questaoExtra->qcodResposta; ?>">
                                            <input type="file" name="<?= $questaoExtra->qstId; ?>">
                                        </td>
                                    </tr>
                                <?php endforeach;
                            } ?>
                            </tbody>
                            <tfoot>
                            <tr style="background-color:#C0C0C0;">
                                <td align="center" colspan="7">
                                    <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
                                </td>
                            </tr>
                            </tfoot>
                        </table>
                    </form>
                    <!-- Fim Formulário Construtora Extra -->
                <?php endforeach;
            } ?>
        </div>

        <div class="col-md-1 pull-right" style="position:inherit;">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td>
                        <?php
                        wf_desenhaBarraNavegacao($cumprimentoObjeto->docid, array(
                            'coid' => $cumprimentoObjeto->coid,
                            'docid' => $cumprimentoObjeto->docid,
                            'obrid' => $cumprimentoObjeto->obrid
                        )); ?>
                    </td>
                </tr>
            </table>
        </div>
    </div>

    <!-- Formulário para adicionar nova construtora. -->
    <div id="dialog" title="Adicionar construtora" style="display:none;">
        <div class="container">
            <form id="formNovaConstrutora">
                <div class="row">
                    <div class="col-md-3">Razão social:</div>
                    <div class="col-md-6"><?= campo_texto(
                        'razaosocialconstrutora',
                        'S',
                        'S',
                        '',
                        25,
                        80,
                        '',
                        '',
                        'left',
                        '',
                        0,
                        'id="razaosocial"'
                    ); ?></div>
                </div>
                <br/>
                <div class="row">
                    <div class="col-md-3">CNPJ:</div>
                    <div class="col-md-6"><?= campo_texto(
                        'cnpjconstrutora',
                        'S',
                        'S',
                        '',
                        25,
                        80,
                        '##.###.###/####-##',
                        '',
                        'left',
                        '',
                        0,
                        'id="cnpj"'
                    ); ?></div>
                </div>

                <div class="row">
                    <div class="col-md-9" id="msgNovaConstrutora"
                         style="padding: 5px; width: 90% ;background-color: #f2dede; border: 1px solid #eed3d7;text-align: justify; font-weight: bold; font-size: 12px; color: #b94a48; display: none"></div>
                </div>
            </form>
        </div>
    </div>
    <!-- Fim do formulário da nova construtora. -->

    
<?php endif; ?>

<?php 
    require APPRAIZ . 'obras2/includes/principal/cadCumprimentoObjeto/js.php';
?>