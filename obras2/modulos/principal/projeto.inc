<?php
$_SESSION['sissnalertaajuda']  = 'N';
verificaSessao( 'obra' );

$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];
$projetoExecutivo = new ProjetoExecutivo();
$projetoExecutivo->populaPorObrid($obrid);

$obra = new Obras();
$obra->carregarPorIdCache($obrid);

if( $_GET['acao'] != 'V' && $_GET['acao'] != 'X'){

    //Chamada de programa
    include  APPRAIZ."includes/cabecalho.inc";
    
    echo "<br>";
    if( !$_SESSION['obras2']['obrid'] && !$_SESSION['obras2']['empid'] ){
        $db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP,$url,$parametros);
    }elseif( $_SESSION['obras2']['obrid'] ){
        if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
        }else{
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
        }
    }else{
        $db->cria_aba(ID_ABA_CADASTRA_OBRA,$url,$parametros);
    }

}
else if($_GET['acao'] == 'X') {
    /*echo '<script language="JavaScript" src="../includes/funcoes.js"></script>
             <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
             <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
             <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
             */
    $db->cria_aba('57688',$url,$parametros);
}
else{
    /*
        
     echo'<script language="JavaScript" src="../includes/funcoes.js"></script>
         <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
         <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
    */
        
    $db->cria_aba($abacod_tela,$url,$parametros);
}

if($_POST['requisicao'] == 'download'){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqid'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    die();
}

if($_POST){
    
    $arCamposNulo = array();
    $projetoExecutivo->obrid = $obrid;
    $projetoExecutivo->proversao1 = $_POST['proversao1'];
    $projetoExecutivo->proversao2 = $_POST['proversao2'];
    $projetoExecutivo->proversao3 = $_POST['proversao3'];
    $projetoExecutivo->proimplantacao_alterado = ($_POST['proimplantacao_alterado']) ? 't' : 'f';
    $projetoExecutivo->profundacao_alterado = ($_POST['profundacao_alterado']) ? 't' : 'f';
    $projetoExecutivo->proestrutural_alterado = ($_POST['proestrutural_alterado']) ? 't' : 'f';
    $projetoExecutivo->proeletrica_alterado = ($_POST['proeletrica_alterado']) ? 't' : 'f';
    $projetoExecutivo->prohidraulica_alterado = ($_POST['prohidraulica_alterado']) ? 't' : 'f';
    $projetoExecutivo->proarquitetura_alterado = ($_POST['proarquitetura_alterado']) ? 't' : 'f';
    $projetoExecutivo->proartrrt_alterado = ($_POST['proartrrt_alterado']) ? 't' : 'f';
    $projetoExecutivo->usucpfinclusao = $_SESSION['usucpf'];

    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    
    foreach($_FILES as $key => $file){
        if (!empty($file['name'])) {
            $file = new FilesSimec($key, null, 'obras2');
            $file->setPasta('obras2');
            $file->setUpload('', $key, false);
            $arqid = $file->getIdArquivo();
            $projetoExecutivo->{$key} = $arqid;
        }

    }
    
    $projetoExecutivo->salvar(true, true, $arCamposNulo);
    $projetoExecutivo->commit();
}

if( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE)) ){
    $habilitado = true;
    $habilita = 'S';
}else{
    $habilitado = false;
    $habilita = 'N';
}

echo cabecalhoObra($obrid);
echo "<br>";
//monta_titulo( 'Projeto Executivo', '' );
monta_titulo_obras( 'Projeto Executivo', '' );

$projetos = array(
    'proimplantacao' => array('nome' => 'Implantação', 'checked' => true, 'upload' => true, 'disabled' => true),
    'profundacao' => array('nome' => 'Fundação', 'checked' => false, 'upload' => true, 'disabled' => false),
    'proestrutural' => array('nome' => 'Estrutural', 'checked' => false, 'upload' => false, 'disabled' => false),
    'proeletrica' => array('nome' => 'Elétrica', 'checked' => false, 'upload' => false, 'disabled' => false),
    'prohidraulica' => array('nome' => 'Hidráulica', 'checked' => false, 'upload' => false, 'disabled' => false),
    'proarquitetura' => array('nome' => 'Arquitetura', 'checked' => false, 'upload' => false, 'disabled' => false),
    'proartrrt' => array('nome' => 'ART/RRT dos projetos alterados', 'checked' => true, 'upload' => true, 'disabled' => true)
);

$projetos_proprios = array(
    'proimplantacao' => array('nome' => 'Implantação', 'checked' => true, 'upload' => true, 'disabled' => true),
    'profundacao' => array('nome' => 'Fundação', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proestrutural' => array('nome' => 'Estrutural', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proeletrica' => array('nome' => 'Elétrica', 'checked' => true, 'upload' => true, 'disabled' => true),
    'prohidraulica' => array('nome' => 'Hidráulica', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proarquitetura' => array('nome' => 'Arquitetura', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proartrrt' => array('nome' => 'ART/RRT dos projetos alterados', 'checked' => true, 'upload' => true, 'disabled' => true)
);

if($projetoExecutivo->proid) {
    $projetos['proimplantacao']['checked'] = ($projetoExecutivo->proimplantacao_alterado == 't') ? true : false;
    $projetos['profundacao']['checked']    = ($projetoExecutivo->profundacao_alterado == 't') ? true : false;
    $projetos['proestrutural']['checked']  = ($projetoExecutivo->proestrutural_alterado == 't') ? true : false;
    $projetos['proeletrica']['checked']    = ($projetoExecutivo->proeletrica_alterado == 't') ? true : false;
    $projetos['prohidraulica']['checked']  = ($projetoExecutivo->prohidraulica_alterado == 't') ? true : false;
    $projetos['proarquitetura']['checked'] = ($projetoExecutivo->proarquitetura_alterado == 't') ? true : false;
    $projetos['proartrrt']['checked']      = ($projetoExecutivo->proartrrt_alterado == 't') ? true : false;
}
?>
  

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloDireita" >Tipologia</td>
        <td>
            <?php
            if($obra->tpoid){
                $tipologia = $obra->pegaUm("SELECT tpodsc FROM obras2.tipologiaobra WHERE tpoid = {$obra->tpoid}");
                echo $tipologia;
            }
            ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita" >Tipo do Projeto</td>
        <td><?=(projetoFNDE($obrid)) ? 'Projeto elaborado pelo FNDE' : 'Projeto elaborado pelo ente federado'; ?></td>
    </tr>
</table>

<form method="post" name="form-projeto" id="form-projeto" enctype="multipart/form-data">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloCentro" colspan="2">Conjunto de documentos do projeto utilizado no canteiro de obras</td>
    </tr>
    <?php if(!projetoFNDE($obrid)): ?>
        <tr>
            <td class="SubTituloDireita" >Inserir Projetos<button data-container="body" data-toggle="popover" data-placement="top" data-content="* Caso haja mais de uma prancha para a disciplina de projeto, inserir os  arquivos  em formato compactado (zip, rar, etc). <br />* A prancha pode estar em formato DWG ou PDF. <br />* Clique aqui para acessar manual de preenchimento" type="button" class="btn btn-default btn-xs">&nbsp;<span class="glyphicon glyphicon-question-sign" aria-hidden="true"></span>&nbsp;</button></td>
            <td>
                <style>
                    table.check {
                        border-collapse: collapse;
                        width: 100%;
                    }

                    table.check, table.check th, table.check td {
                        border-bottom: 1px solid #dcdcdc;
                    }
                </style>
                <table class="check">
                    <?php if(is_array($projetos_proprios)){ ?>
                    <?php foreach($projetos_proprios as $key => $projeto): ?>
                        <tr>
                            <td style="width: 200px;"><input <?if($projeto['disabled']):?>onclick="return false"<?endif;?> <?if($projeto['checked']):?>checked="checked"<?endif;?> type="checkbox" name="<?=$key?>_alterado" > <?=$projeto['nome']?></td>
                            <td>
                                <input style="display: <?if($projeto['checked']):?>inline<?else:?>none<?endif;?>" type="file" name="<?=$key?>_arquivo" id="<?=$key?>_arquivo"><?if($projeto['upload'] && $projeto['checked']):?><img class="obrig" border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."><?endif;?>
                                <?php
                                if ( $projetoExecutivo->{$key . '_arquivo'} ){
                                    $arquivo = new Arquivo($projetoExecutivo->{$key . '_arquivo'});
                                    echo "<a href='javascript:DownloadArquivo(" . $arquivo->arqid . ");'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                                }
                                ?>
                            </td>
                        </tr>
                    <?php endforeach;?>
                    <?php } ?>
                </table>
            </td>
        </tr>
    <?php else: ?>
    <tr>
        <td class="SubTituloDireita">Versão utilizada (ano) 
            <?php if($_GET['acao'] != 'V' && $_GET['acao'] != 'X') : ?>
            <button data-container="body" data-toggle="popover" data-placement="top" data-content="* Preencher conforme revisões de projeto utilizadas.<br />* Caso tenha sido usado detalhamento ou projeto complementar de versão mais atualizada para complementar versão anterior, indicar os anos correspondentes.  " type="button" class="btn btn-default btn-xs">&nbsp;<span class="glyphicon glyphicon-question-sign" aria-hidden="true"></span>&nbsp;</button></td>
            <?php endif; ?>
        <td style="    width: 75%;">
            <p><input placeholder="Ex.: 2016" type="text" id="" name="proversao1" value="<?=trim($projetoExecutivo->proversao1)?>" size="5" maxlength="4" > <img class="obrig" border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."></p>
            <p><input type="text" id="" name="proversao2" value="<?=trim($projetoExecutivo->proversao2)?>" size="5" maxlength="4" ></p>
            <p><input type="text" id="" name="proversao3" value="<?=trim($projetoExecutivo->proversao3)?>" size="5" maxlength="4" ></p>
        </td>
    </tr>
    <tr>
        <? /* adicionando link para o manual de preenchimento */ ?>
        <td class="SubTituloDireita" >Quais projetos foram alterados? 
            <?php if($_GET['acao'] != 'V' && $_GET['acao'] != 'X') : ?>
            <button data-container="body" data-toggle="popover" data-placement="top" data-content="* Caso haja mais de uma prancha para a disciplina de projeto, inserir os  arquivos  em formato compactado (zip, rar, etc). <br />* A prancha pode estar em formato DWG ou PDF. <br />* <a target='_blank' href='/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882771'>Clique aqui</a> para acessar manual de preenchimento" type="button" class="btn btn-default btn-xs">&nbsp;<span class="glyphicon glyphicon-question-sign" aria-hidden="true"></span>&nbsp;</button></td>
            <?php endif;?>
        <td>
            <style>
                table.check {
                    border-collapse: collapse;
                    width: 100%;
                }

                table.check, table.check th, table.check td {
                    border-bottom: 1px solid #dcdcdc;
                }
            </style>
            <table class="check">
                <?php if(is_array($projetos)){ ?>
                <? foreach($projetos as $key => $projeto): ?>
                    <tr>
                        <td style="width: 200px;"><input <?if($projeto['disabled']):?>onclick="return false"<?else:?>onclick="alteraProjeto($(this))"<?endif;?> <?if($projeto['checked']):?>checked="checked"<?endif;?> type="checkbox" name="<?=$key?>_alterado" > <?=$projeto['nome']?></td>
                        <td>
                            <input style="display: <?if($projeto['checked']):?>inline<?else:?>none<?endif;?>" type="file" name="<?=$key?>_arquivo" id="<?=$key?>_arquivo"><?if($projeto['upload'] && $projeto['checked']):?><img class="obrig" border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."><?endif;?>
                            <?php
                            if ( $projetoExecutivo->{$key . '_arquivo'} != '' && $projeto['checked']){
                                $arquivo = new Arquivo($projetoExecutivo->{$key . '_arquivo'});
                                echo "<a  class='btn btn-default btn-xs' href='javascript:DownloadArquivo(" . $arquivo->arqid . ");'><span class='glyphicon glyphicon-download-alt' aria-hidden='true'></span> " . $arquivo->arqnome . "." . $arquivo->arqextensao . "</a>";
                            }

                            ?>
                        </td>
                    </tr>
                <? endforeach;?>
                <?php } ?>
            </table>
        </td>
    </tr>
    <td class="SubTituloDireita" > Inserir os projetos que no sofreram alteração
        <?php if($_GET['acao'] != 'V' && $_GET['acao'] != 'X') : ?>
        <button data-container="body" data-toggle="popover" data-placement="top" data-content="Inserir pacote de projetos, conforme versões utilizadas dos projetos elaborados FNDE" type="button" class="btn btn-default btn-xs">&nbsp;<span class="glyphicon glyphicon-question-sign" aria-hidden="true"></span>&nbsp;</button>
        <?php endif;?>
    <td>
        <table class="check">
            <?php if(is_array($projetos)){ ?>
            <?php foreach($projetos as $key => $projeto): ?>
                    <tr id="<?=$key?>_alterado" style="display:<?if($projeto['checked']):?>none<?endif?>">
                        <td style="width: 200px;"><input onclick="return false" checked="checked" type="checkbox" name="" value=""> <?=$projeto['nome']?></td>
                        <td>
                            <input style="display:inline" <?if($projeto['checked']):?>disabled<?endif?> type="file" name="<?=$key?>_arquivo" id="<?=$key?>_arquivo">
                            <?php
                            if ( $projetoExecutivo->{$key . '_arquivo'} != '' && !$projeto['checked']){
                                $arquivo = new Arquivo($projetoExecutivo->{$key . '_arquivo'});
                                echo "<a  class='btn btn-default btn-xs' href='javascript:DownloadArquivo(" . $arquivo->arqid . ");'><span class='glyphicon glyphicon-download-alt' aria-hidden='true'></span> " . $arquivo->arqnome . "." . $arquivo->arqextensao . "</a>";
                            }

                            ?>
                        </td>
                    </tr>
            <?php endforeach; ?>
            <?php } ?>
        </table>
    </td>
    <?php endif;?>
    <tr class="divTituloPadrao">
        <td colspan="3" align="center">
            <?php if ($habilitado): ?>
            <input type="submit" name="salvar" class="salvar" value="Salvar"/>
            <?php endif; ?>
        </td>
    </tr>
</table>
    <input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
    <input type="hidden" name="arqid" id="arqid" value="salvar"/>
</form>


<?php 
    require APPRAIZ . 'obras2/includes/principal/projeto/js.php';
?>


<?php
monta_titulo('Solicitações de Uso de Saldo/ Alteração de Projeto/Serviço/ Alteração de Terreno)', $linha2);
$solicitacoes = pegaSolicitacoesObra($obrid);
if(is_array($solicitacoes)):
foreach($solicitacoes as $solicitacao):?>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="3" align="center">
                <b>Solicitao N <?=$solicitacao['slcid']; ?></b>
            </td>
        </tr>
        <tr>
            <td colspan='2'></td>
            <td align="right">
                <?php if($habilitado_super && ($solicitacao['esdid'] == ESDID_SOLICITACOES_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO) && $solicitacao['aprovado'] == 'N'):?>
                    <button type="button" class='' onclick='aprovaSolicitacao(<?=$solicitacao['slcid']; ?>);'>APROVAR</button>
                <?php endif;?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Solicitao</td>
            <td><span onclick="abreSolicitacao(<?=$obrid; ?>,<?=$solicitacao['tslid']; ?>,<?=$solicitacao['slcid']; ?>);" style="cursor:pointer;" title="Acessar Solicitao"><img src="../imagens/alterar.gif" style="border:0;"> <?=$solicitacao['tsldescricao']?></span></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Situação</td>
            <?php if($habilitado): ?>
                <td><?=$solicitacao['esddsc']; ?></td>
            <?php else: ?>
                <td>
                    <?
                    if($solicitacao['aprovado'] == 'N'){
                        if($solicitacao['esdid'] == ESDID_SOLICITACOES_CADASTRAMENTO){
                            echo '<b>Em Cadastramento</b>';
                        } else if($solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA) {
                            echo '<b>Diligência</b>';
                        } else if($solicitacao['esdid'] == ESDID_SOLICITACOES_CANCELADO) {
                            echo'<b>Cancelado</b>';
                        } else {
                            echo'<b>Aguardando Análise</b>';
                        }
                    }else{
                        echo '<b style="color:green;">' . retornaTextoEstadoSolicitacao($solicitacao['esdid']) . '</b>';
                    }
                    ?>
                </td>
            <?php endif; ?>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Justificativa</td>
            <td><?=$solicitacao['slcjustificativa']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Inserido por</td>
            <td><?=$solicitacao['criador']; ?></td>
        </tr>
        <?php if(($solicitacao['esdid'] != ESDID_SOLICITACOES_CADASTRAMENTO && $solicitacao ['esdid'] != ESDID_SOLICITACOES_AGUARDANDO_ANALISE && $solicitacao['esdid'] != ESDID_SOLICITACOES_RETORNADO && $solicitacao['aprovado'] == 'S') || ($solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA)): ?>
        <tr>
            <td class="SubTituloDireita" width="20%">Análise FNDE</td>
            <td>
                <?php
                $analise = capturaAnaliseSolicitacao($solicitacao['docid']);
                ?>
                <table style="width: 100%">
                    <tr>
                        <td class="SubTituloDireita" style="width: 100px">Data da Análise</td>
                        <td><?=$analise['htddata']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita"  style="width: 100px">Analisado por</td>
                        <td><?=$analise['analista']; ?></td>
                    </tr>
                    <?php if($solicitacao['esdid'] == ESDID_SOLICITACOES_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA || $solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO): ?>
                        <tr>
                            <td class="SubTituloDireita"  style="width: 100px">Observaes</td>
                            <td>
                                <style>
                                    .box-comentario{
                                        padding: 0 0 0 2px;
                                        border: 1px solid #ccc;
                                        color: #7B7B7B;
                                        margin: 1px;
                                        width: 478px;
                                        -webkit-border-radius: 3px;
                                        -moz-border-radius: 3px;
                                        border-radius: 3px;
                                    }
                                    .box-comentario-fnde{
                                        border: 2px solid #97BAF9;
                                        color:  #003DAB;
                                        width: 476px;
                                    }
                                </style>
                                <?=capturaAnaliseSolicitacaoObservacoes($solicitacao['docid']); ?>
                            </td>
                        </tr>
                    <? endif; ?>
                    <? if(($solicitacao['aprovado'] == 'S' || $solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA) and ($solicitacao['tslid'] == 1 || $solicitacao['tslid'] == 2 || $solicitacao['tslid'] == 3 || $solicitacao['tslid'] == 4  ||$solicitacao['tslid'] == 5 )): ?>
                        <tr>
                            <td class="SubTituloDireita"  style="width: 100px">Ver Análise</td>
                            <td> <a onclick='printQuestionario(<?=$obrid?>,<?=simec_json_encode($solicitacao['qrpid'])?>,<?=$solicitacao['slcid']?>,<?=simec_json_encode($solicitacao['queid'])?>);'> <img src='/imagens/print.png' > </a></td>
                        </tr>
                    <? endif; ?>
                </table>
            </td>
            <? endif;?>
        </tr>

    </table>
<?php
endforeach;

else:
    ?>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <td colspan="3" align="center">
            <b>Nenhuma solicitao cadastrada</b>
        </td>
    </tr>
    </table>
<?php
endif;

function pegaSolicitacoesObra($obrid)
{
    global $db;

    $sql = "SELECT      sol.slcid,
                        sol.slcobservacao,
                        sol.slcjustificativa,
                        TO_CHAR(sol.slcdatainclusao,'DD/MM/YYYY') as slcdatainclusao,
                        sol.docid,
                        sol.qrpid,
                        sol.aprovado,
                        ts.tslid,
                        ts.tsldescricao,
                        esd.esdid,
                        esd.esddsc,
                        usu.usunome AS criador,
                        qr.queid
            FROM        obras2.solicitacao sol
            INNER JOIN  obras2.tiposolicitacao_solicitacao tss ON(sol.slcid = tss.slcid)
            INNER JOIN  obras2.tiposolicitacao ts ON(tss.tslid = ts.tslid)
            INNER JOIN  workflow.documento doc ON(sol.docid = doc.docid)
            INNER JOIN  workflow.estadodocumento esd ON(doc.esdid = esd.esdid)
            INNER JOIN  seguranca.usuario usu ON(sol.usucpf = usu.usucpf)
            LEFT  JOIN  questionario.questionarioresposta qr ON(sol.qrpid = qr.qrpid)
            WHERE       obrid = {$obrid} AND tss.tpsstatus = 'A' AND sol.slcstatus = 'A' AND ts.tslid IN (2,3,4) AND sol.aprovado = 'S'
            ORDER BY    1 DESC";
    
    $retorno = $db->carregar($sql);
    
    return $retorno ? $retorno : array();
}

function projetoFNDE($obrid){
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $tpoProjetoFnde = array(
        1, // Espao Educativo - 01 Sala
        2, // Espao Educativo - 02 Salas
        3, // Espao Educativo - 04 Salas
        4, // Espao Educativo - 06 Salas
        5, // Espao Educativo - 08 Salas
        6, // Espao Educativo - 10 Salas
        7, // Espao Educativo - 12 Salas
        8, // Espao Educativo Ensino Mdio Profissionalizante
        9, // Escola de Educao Infantil Tipo B
        10, // Escola de Educao Infantil Tipo C
//        13, // Escola com projeto elaborado pelo concedente
//        14, // Escola com projeto elaborado pelo concedente
//        15, // Escola com projeto elaborado pelo concedente
//        16, // Escola de Educao Infantil Tipo A
        17, // QUADRA ESCOLAR COBERTA COM PALCO- PROJETO FNDE
//        18, // QUADRA ESCOLAR COBERTA - PROJETO PRPRIO
        19, // COBERTURA DE QUADRA ESCOLAR PEQUENA - PROJETO FNDE
        20, // COBERTURA DE QUADRA ESCOLAR GRANDE - PROJETO FNDE
//        21, // COBERTURA DE QUADRA ESCOLAR - PROJETO PRPRIO
        22, // QUADRA ESCOLAR COBERTA COM VESTIRIO- PROJETO FNDE
//        102, // Reforma
//        103, // Ampliao
        104, // MI - Escola de Educao Infantil Tipo B
        105, // MI - Escola de Educao Infantil Tipo C
        106, // Escola com Projeto elaborado pelo proponente
        107, // Escola 12 salas - Projeto FNDE 2014
        108, // Projeto 1 Convencional
        109, // Projeto 2 Convencional
        110, // Projeto 1 Convencional - Emenda
        111, // Projeto 2 Convencional - Emenda
//        112, // Construo
        113, // Projeto Tipo B - Bloco Estrutural
        114, // Projeto Tipo C - Bloco Estrutural

    );

    if(in_array($obra->tpoid, $tpoProjetoFnde)){
        return true;
    } else {
        return false;
    }
}
?>
