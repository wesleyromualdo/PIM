<?php
$_SESSION['obras2']['semMarcadagua'] = true;

// empreendimento || obra || orgao
//verificaSessao( 'empreendimento' );


//$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);
$sfndeid = ($_REQUEST['sfndeid'] != '' ? $_REQUEST['sfndeid'] : $_SESSION['obras2']['sfndeid']);
$_SESSION['obras2']['sfndeid'] = ($_REQUEST['sfndeid'] != '' ? $_REQUEST['sfndeid'] : $_SESSION['obras2']['sfndeid']);
if (empty($sfndeid)) {
    die("<script>
                alert('Faltam parametros para acessar a tela!'); 
                window.location = '?modulo=principal/listVistoriaEmp&acao=A';
         </script>");
}

$supervisaoEmpresa = new SupervisaoFNDE($sfndeid);
extract($supervisaoEmpresa->getDados());

//$docid = pegaDocidSupervisaoEmpresa($supervisaoEmpresa->sueid);
//$esd = wf_pegarEstadoAtual($docid);
//$suesituacao = ($esd['esdid'] == 1188) ? '<b style="color:red">' . $esd['esddsc'] . '</b>': $esd['esddsc'];

$sosidHabil = ($sosid ? 'N' : 'S');
// o EXTRACT limpa o empid, assim o recarrega

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$somenteLeitura = 'N';
$habilitado = false;
$habil = 'N';


$sfndedtsupervisao = formata_data($sfndedtsupervisao);

$entnomeResp = $supervisaoEmpresa->pegaNomeResponsavelPorSueid($sfndeid);
$sfndfuncionamento = ($sfndfuncionamento == 's' ? 'Sim' : 'Não');

$empreendimento = new Empreendimento();
extract($empreendimento->dadosResumo($empid));

$endereco = new Endereco($endid ? $endid : $empreendimento->endid );

function retornaDadosSupervisaoEmpresa() {
    $sfndeid = $_SESSION['obras2']['sfndeid'];
    $supEmpresa = new SupervisaoFNDE($sfndeid);
    $arrDadosSupervisao = $supEmpresa->getDados();
    return $arrDadosSupervisao;
}

function pesquisaEndereco($tipo_endereco, $empid) {

    require_once APPRAIZ . "adodb/adodb.inc.php";
    require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
    require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
    require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

    $str_tr_endereco = '';

    switch ($tipo_endereco) {
        case 'simec':
            $empreendimento = new Empreendimento($empid);
            //Endereço SIMEC
            $obras = new Obras();
            $arr = $obras->pegaObraPorEmpid($empid);
            $endereco2 = new Endereco($arr['endid']);
            $str_tr_endereco = ' 
                                        <tr>
                                                <th bgcolor="#DCDCDC" colspan="2">Local da obra - Cadastro da Obra</th>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">CEP</td>
                                                <td bgcolor="">
                                                        ' . $endereco2->endcep . '
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Logradouro</td>
                                                <td bgcolor="">
                                                        ' . $endereco2->endlog . '
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Número</td>
                                                <td bgcolor="">
                                                        ' . $endereco2->endnum . '			
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Complemento</td>
                                                <td bgcolor="">
                                                        ' . $endereco2->endcom . '			
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Bairro</td>
                                                <td bgcolor="">
                                                        ' . $endereco2->endbai . '
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Município/UF</td>
                                                <td bgcolor="">
                                                        ' . $endereco2->getMunDescricao() . '			
                                                </td>
                                        </tr>
                                     ';

            break;
        case 'supervisao':
            $arr = retornaDadosSupervisaoEmpresa();
            //Endereço supervisao
            $endereco1 = new Endereco($arr['endid']);
            $str_tr_endereco = ' 
                                        <tr>
                                                <th bgcolor="#DCDCDC" colspan="2">Local da obra - Supervisão</th>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">CEP</td>
                                                <td bgcolor="">
                                                        ' . $endereco1->endcep . '
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Logradouro</td>
                                                <td bgcolor="">
                                                        ' . $endereco1->endlog . '
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Número</td>
                                                <td bgcolor="">
                                                        ' . $endereco1->endnum . '			
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Complemento</td>
                                                <td bgcolor="">
                                                        ' . $endereco1->endcom . '			
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Bairro</td>
                                                <td bgcolor="">
                                                        ' . $endereco1->endbai . '
                                                </td>
                                        </tr>
                                        <tr>
                                                <td bgcolor="#E9E9E9" align="right">Município/UF</td>
                                                <td bgcolor="">
                                                        ' . $endereco1->getMunDescricao() . '			
                                                </td>
                                        </tr>
                                     ';
            break;

        default:
            break;
    }

    return $str_tr_endereco;
}

$arr = retornaDadosSupervisaoEmpresa();
$str_pergunta = ($arr['sfndeendcorreto'] === 'n' ) ? 'Não' : 'Sim';
$str = '';

if ($arr['sfndeendcorreto'] == 'n') {
    $str .= pesquisaEndereco('simec', $arr['empid']);
    $str .= pesquisaEndereco('supervisao', 0);
} else {
    $str .= pesquisaEndereco('simec', $arr['empid']);
}

$str_tr_pergunta = '<tr>
                        <th bgcolor="#DCDCDC" colspan="2">Local da obra</th>
                    </tr>
                    <tr>
                            <td bgcolor="#E9E9E9" align="right">No cadastro da obra o endereço está correto?</td>
                            <td bgcolor="">
                                    ' . $str_pergunta . '
                            </td>
                    </tr>';

$str_local_obra = $str_tr_pergunta . $str;


$situacaoObra = new SituacaoObra();
$situacoes = $situacaoObra->listaCombo();

$tipoParalizacao = new TipoParalisacao($arr[tplid]);

foreach ($situacoes as $s) {
    if ($s['codigo'] == $sobid)
        $esddsc = $s['descricao'];
}
$obras = new Obras();
$arObrid = $obras->pegaIdObraPorEmpid($empid, array('not(obridpai)'));
$obrid = $arObrid[0];

$sql_quest = "SELECT  CASE WHEN qoprespostaum ilike 'N' THEN 'Não'
                             WHEN qoprespostaum ilike 'S' THEN 'Sim' END as qoprespostaum,
                       
                        CASE WHEN qoprespostaquatro = 'N' THEN 'Não'
                             WHEN qoprespostaquatro = 'S' THEN 'Sim' END as qoprespostaquatro,
                        CASE WHEN qoprespostacinco = 'N' THEN 'Não'
                             WHEN qoprespostacinco = 'S' THEN 'Sim' END as qoprespostacinco,
                             qoprespostadois,
                             qoprespostatres
                             


                      FROM obras2.questionarioobraparalisada
                      WHERE obrid = " . $obrid;
$respostas = $db->pegaLinha($sql_quest);

$conteudo = '
        <table width="100%" style="font-family: arial;">

                <tr>

                        <td bgcolor="">
                                ' . $entnome . '			
                        </td>
                </tr>

                <tr>
                        <th bgcolor="#DCDCDC" colspan="2">Obra</th>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">ID Obra</td>
                        <td bgcolor="">
                                ' . $obrid . '
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">Nome</td>
                        <td bgcolor="">
                                ' . $empdsc . '			
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">UF/Município</td>
                        <td bgcolor="">
                                ' . $estuf . '/' . $mundescricao . '			
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right" valign="top">Endereço</td>
                        <td bgcolor="">
                                ' . $endlog . '
                                <br>
                                ' . $endcom . '			
                        </td>
                </tr>
                <tr>
                        <th bgcolor="#DCDCDC" colspan="2">Supervisão</th>
                </tr>

                <tr>
                        <td bgcolor="#E9E9E9" align="right">Data de realização da supervisão</td>
                        <td bgcolor="">
                                ' . $sfndedtsupervisao . '
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">Nome do responsável</td>
                        <td bgcolor="">
                                ' . $entnomeResp . '
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">Cargo</td>
                        <td bgcolor="">
                                ' . $sfndecargorepresentante . '
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">Situação da obra</td>
                        <td bgcolor="">
                                ' . $esddsc . '
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">Em conformidade com o Projeto Básico aprovado e contrato?</td>
                        <td bgcolor="">
                                ' . $sfndeacordo . '
                        </td>
                </tr>
                <tr>
                        <td bgcolor="#E9E9E9" align="right">Unidade em funcionamento</td>
                        <td bgcolor="">
                                ' . $sfndefuncionamento . '
                        </td>
                </tr>

                ';

    //ver(print_r(get_defined_vars()),d);

    $conteudo .= $str_local_obra . '                
                
        </table>
';

    ?>

    <table class="tabela" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
        <tr>
            <td colspan="2">
    <?php echo monta_cabecalho_relatorio(100); ?>
            </td>
        </tr>
        <tr>
            <td colspan="2">
    <?php echo $conteudo ?>
            </td>
        </tr>
        <tr>
            <td colspan="2">

                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
    <?php
        $param					= array();
        $param['not(obridpai)'] = true;
        $obras   				= new Obras();
        $arObrid 				= $obras->pegaIdObraPorEmpid( $empid, $param );

        foreach ( $arObrid as $obrid ):
            $obra = new Obras( $obrid );

            $supervisao 		= new Supervisao();
            $supid				= $supervisao->pegaSupidByObraAndSfndeid( $obrid, $sfndeid );
            // ver($supid,d);

            $param				= array();
            $param['empid'] 	= $empid;
            $param['is(sfndeid)'] = true;
            $ultimoSupid 		= $supervisao->pegaUltSupidByObra( $obrid, $param );

            if(empty($supid))
                continue;

            unset($total);
        ?>
                        <tr id="tr9" >
                            <td colspan="3">
                                <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                                    <thead>
                                        <tr style="background-color: #CDCDCD;">
                                            <td colspan="15" valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                <input type="hidden" name="obrid[]" id="obrid_<?php echo $obrid ?>" value="<?php echo $obrid ?>">
                                                <input type="hidden" name="supid[<?php echo $obrid ?>]" id="supid_<?php echo $obrid ?>" value="<?php echo $supid ?>">
                                                <b><?php echo "($obrid) {$obra->obrnome}" ?></b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Descrição</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre o Objeto</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
                                            <td colspan="4" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
                                        </tr>
                                        <tr>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
                                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado<br></b></td>
                                            <td colspan="1" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre o Objeto após Supervisão</b></td>
                                        </tr>
                                    </thead>

        <?php
        $supervisaoItem = new SupervisaoItem();

        $total['vlrProjeto'] = 0;
        $total['vlrObra'] = 0;
        $total['vlrExecSobreObra'] = 0;
        $total['vlrExecSobreObraUlt'] = 0;
        $total['percExecSobreObraUlt'] = 0;

        // Busca filhos da Edificação
        $dadoEtapa = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid));
//                                            $habil = (count($dadoEtapa) ? 'N' : 'S');

        foreach ($dadoEtapa as $etapa) {

            $total['vlrProjeto'] = $etapa['ocrvalorexecucao'];

            $total['vlrObra'] += $etapa['icovlritem'];
            $total['percObra'] = ($etapa['ocrvalorexecucao'] > 0 ? ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100 : 0);

            $total['vlrExecSobreObra'] += $etapa['spivlrfinanceiroinfsupervisor'];
            $total['percExecSobreObra'] = ($etapa['ocrvalorexecucao'] > 0 ? ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100 : 0);

            $total['vlrExecSobreObraUlt'] += $etapa['spivlrfinanceiroanterior'];
            $total['percExecSobreObraUlt'] = ($etapa['ocrvalorexecucao'] > 0 ? ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100 : 0);
            //                        $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];


            $etapa['icodtinicioitem'] = formata_data($etapa['icodtinicioitem']);
            $etapa['icodterminoitem'] = formata_data($etapa['icodterminoitem']);

            $etapa['spipercsobreobrainfsupervisor'] = ($etapa['ocrvalorexecucao'] > 0 ? ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100 : 0);

            // Busca Filhos da Etapa
            $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
//                                                $habil = (count($dadoDetalhe) ? 'N' : 'S');
            ?>
                                        <tbody>
                                            <tr bgcolor="#FDF8E7" id="<?= "tr_item_etapa_" . $obrid . "_" . $etapa['icoid'] ?>">
                                                <td align="left">
                                                    <input type="hidden" name="icoid[<?php echo $obrid ?>][]" value="<?= $etapa['icoid'] ?>">
                                                    <input type="hidden" name="ico_spiid[<?php echo $obrid ?>][]" value="<?= $etapa['spiid'] ?>">
                                                    &nbsp;<img src="/imagens/seta_filho.gif" border="0"><?= $etapa['itcdesc'] ?>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                        <?= number_format($etapa['icovlritem'], 2, ',', '.') ?>
                                                </td>
                                                <td align="right" style="color:#336EFF">
            <?= number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                                                </td>

                                                <td align="center">
            <?= $etapa['icodtinicioitem'] ?>
                                                </td>
                                                <td align="center">
                                                    <?= $etapa['icodterminoitem'] ?>
                                                </td>
                                                <td align="center">
                                                    <?php echo campo_texto('ico_spivlrinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, ' . $obrid . ' )', number_format($etapa['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                                </td>
                                                <td align="center">
            <?php echo campo_texto('ico_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, ' . $obrid . ', \'numerico\' )', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                                </td>
                                                <td align="right" style="color:#336EFF">
                                                    <input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="<?= number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                                    <span>
                                                    <?= number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                                    </span>
                                                </td>

                                            </tr>
                                        </tbody>
            <?php
            foreach ($dadoDetalhe as $detalhe) {
                $ditid = $detalhe['ditid'];
                $detalhe['ditdatainicio'] = formata_data($detalhe['ditdatainicio']);
                $detalhe['ditdatatermino'] = formata_data($detalhe['ditdatatermino']);
                $detalhe['spipercsobreobrainfsupervisor'] = ($detalhe['spivlrfinanceiroinfsupervisor'] / $detalhe['ocrvalorexecucao']) * 100;

                //adicionando os filhos
                $dadoDetalheFilho = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai ={$ditid}"));
                $habil = (count($dadoDetalheFilho) ? 'N' : 'S');
                ?>
                                            <tbody>
                                                <tr bgcolor="#DBF6D5" id="<?= "tr_item_detalhamento_" . $obrid . "_" . $detalhe['icoid'] . "_" . $detalhe['ditid'] ?>">
                                                    <td align="left">
                                                        <input type="hidden" name="ditid[<?php echo $obrid ?>][]" value="<?= $detalhe['ditid'] ?>">
                                                        <input type="hidden" name="dit_spiid[<?php echo $obrid ?>][]" value="<?= $detalhe['spiid'] ?>">
                                                        &nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?= $detalhe['ditdsc'] ?>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                                            <?= number_format($detalhe['ditvalor'], 2, ',', '.') ?>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                <?= number_format($detalhe['ditpercsobreobra'], 2, ',', '.') ?>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                <?= ($detalhe['ditmetafisica'] ? number_format($detalhe['ditmetafisica'], 2, ',', '.') : '-') ?>
                                                    </td>
                                                    <td align="center">
                                                        <?= ($detalhe['umcdsc'] ? $detalhe['umcdsc'] : '-') ?>
                                                    </td>
                                                    <td align="center">
                                                        <?= $detalhe['ditdatainicio'] ?>
                                                    </td>
                                                    <td align="center">
                                                        <?= $detalhe['ditdatatermino'] ?>
                                                    </td>
                                                    <td align="center">
                                                        <input type="hidden" name="dit_spivlrfinanceiroanterior[]" value="<?= number_format($detalhe['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                                        <?php echo campo_texto('dit_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemexecanterior'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="center">
                                                        <?php echo campo_texto('dit_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="center">
                                                        <?php echo campo_texto('dit_spivlrinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, ' . $obrid . ', ' . $detalhe['icoid'] . ' )', number_format($detalhe['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                                    </td>
                                                    <td align="center">
                <?php echo campo_texto('dit_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, ' . $obrid . ', ' . $detalhe['icoid'] . ', \'numerico\' )', number_format($detalhe['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                                                        <input type="hidden" name="dit_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="<?= number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                                        <span>
                                                        <?= number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                                        </span>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                                                        <?=
                                                        ($detalhe['umcdsc'] ?
                                                                campo_texto('dit_spivlrmetafisicasupervisor[' . $detalhe['ditid'] . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalhe['ditmetafisica'] . ' )', number_format($detalhe['spivlrmetafisicasupervisor'], 2, ',', '.')) :
                                                                '&nbsp;')
                                                        ?>
                                                    </td>

                                                    <td align="right" style="color:#336EFF">
                                                        <input type="hidden" value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dit_spidiferencavlr[]">
                                                        <span> <?php echo number_format($detalhe['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                                                    </td>
                                                    <td align="right" style="color:#336EFF">
                                                        <input type="hidden" value="<?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dit_spidiferencaporcentagem[]">
                                                        <span> <?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                                                    </td>
                                                </tr>
                                            </tbody>
                <?php
                foreach ($dadoDetalheFilho as $detalheFilho) {
                    $detalheFilho['ditdatainicio'] = formata_data($detalheFilho['ditdatainicio']);
                    $detalheFilho['ditdatatermino'] = formata_data($detalheFilho['ditdatatermino']);

                    $detalheFilho['spipercsobreobrainfsupervisor'] = ($detalheFilho['spivlrfinanceiroinfsupervisor'] / $detalheFilho['ocrvalorexecucao']) * 100;
                    ?>
                                                <tbody>
                                                    <tr bgcolor="#D7EAF2" id="<?= "tr_item_detalhamento_filho_" . $obrid . "_" . $detalheFilho['icoid'] . "_" . $detalheFilho['ditidpai'] . "_" . $detalheFilho['ditid'] ?>">
                                                        <td align="left">
                                                            <input type="hidden" name="ditid_filho[<?php echo $obrid ?>][]" value="<?= $detalheFilho['ditid'] ?>">
                                                            <input type="hidden" name="dtf_spiid[<?php echo $obrid ?>][]" value="<?= $detalheFilho['spiid'] ?>">
                                                            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?= $detalheFilho['ditdsc'] ?>
                                                        </td>
                                                        <td align="right" style="color:#336EFF">
                                                <?= number_format($detalheFilho['ditvalor'], 2, ',', '.') ?>
                                                        </td>
                                                        <td align="right" style="color:#336EFF">
                    <?= number_format($detalheFilho['ditpercsobreobra'], 2, ',', '.') ?>
                                                        </td>
                                                        <td align="right" style="color:#336EFF">
                    <?= ($detalheFilho['ditmetafisica'] ? number_format($detalheFilho['ditmetafisica'], 2, ',', '.') : '-') ?>
                                                        </td>
                                                        <td align="center">
                                                            <?= ($detalheFilho['umcdsc'] ? $detalheFilho['umcdsc'] : '-') ?>
                                                        </td>
                                                        <td align="center">
                                                            <?= $detalheFilho['ditdatainicio'] ?>
                                                        </td>
                                                        <td align="center">
                                                            <?= $detalheFilho['ditdatatermino'] ?>
                                                        </td>
                                                        <td align="center">
                                                            <input type="hidden" name="dtf_spivlrfinanceiroanterior[]" value="<?= number_format($detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                                            <?php echo campo_texto('dtf_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemexecanterior'], 2, ',', '.')); ?>
                                                        </td>
                                                        <td align="center">
                                                            <?php echo campo_texto('dtf_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                                        </td>
                                                        <td align="center">
                                                            <?php echo campo_texto('dtf_spivlrinfsupervisor[' . $obrid . '][]', 'N', 'S', '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, ' . $obrid . ', ' . $detalheFilho['icoid'] . ', ' . $detalheFilho['ditidpai'] . ' )', number_format($detalheFilho['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                                        </td>
                                                        <td align="center">
                    <?php echo campo_texto('dtf_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, ' . $obrid . ', ' . $detalheFilho['icoid'] . ', ' . $detalheFilho['ditidpai'] . ', \'numerico\'  )', number_format($detalheFilho['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                                        </td>
                                                        <td align="right" style="color:#336EFF">
                                                            <input type="hidden" name="dtf_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="<?= number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                                            <span>
                                                            <?= number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                                            </span>
                                                        </td>
                                                        <td align="right" style="color:#336EFF">
                                                            <?=
                                                            ($detalheFilho['umcdsc'] ?
                                                                    campo_texto('dtf_spivlrmetafisicasupervisor[' . $obrid . '][' . $detalheFilho['ditid'] . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalheFilho['ditmetafisica'] . ' )', number_format($detalheFilho['spivlrmetafisicasupervisor'], 2, ',', '.')) :
                                                                    '&nbsp;')
                                                            ?>
                                                        </td>

                                                        <td align="right" style="color:#336EFF">
                                                            <input type="hidden" value="<?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencavlr[]">
                                                            <span> <?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                                                        </td>
                                                        <td align="right" style="color:#336EFF">
                                                            <input type="hidden" value="<?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencaporcentagem[]">
                                                            <span> <?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                                                        </td>

                                                    </tr>
                                                </tbody>
                    <?php
                }
            }
        }
        ?>
                                    <tbody>
                                        <tr id="tr_total_supervisao_<?php echo $obrid ?>" bgcolor="#E3E3E3">
                                            <td align="right">
                                                <b>Total</b>
                                                <input type="hidden" name="vlrProjeto_<?php echo $obrid ?>" id="vlrProjeto_<?php echo $obrid ?>" value="<?= $total['vlrProjeto'] ?>">
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                    <?= number_format($total['vlrObra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
        <?= number_format($total['percObra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="center">
                                                &nbsp;
                                            </td>
                                            <td align="center">
                                                &nbsp;
                                            </td>

                                            <td align="right">
                                                <?php // echo campo_texto( 'total_spivlrinfsupervisor', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00'); ?>
                                            </td>
                                            <td align="right">
                                                &nbsp;
                                            </td>
                                            <td align="right" style="color:#336EFF">
        <?= number_format($total['percExecSobreObra'], 2, ',', '.') ?>
                                            </td>

                                        </tr>
                                    </tbody>
                                </table>
                                <br>
                            </td>
                        </tr>    
                                                <?php
                                                endforeach;
                                            ?>
                </table>
            </td>
        </tr>
        <tr>
            <td colspan="2">
    <?php
    $questao  = new Questao();
    $arquivoQuestaoSupervisao = new ArquivoQuestaoSupervisao();
    $arFiltro = array("qstescopo" => "SFNDE",
        "orgid" 	  => $_SESSION['obras2']['orgid'],
        "sfndeid" 	  => ($sfndeid ? $sfndeid : 0));
    $arDados  = $questao->pegaTodaEstrutura( $arFiltro );
    $etqidUlt = null;
    $itcidUlt = null;
    $qstidUlt = null;


    $subQuestao = new SubQuestao();
    $arquivoRespostaSubQuestao = new ArquivoRespostaSubQuestao();

    $imgRadio = '<img src="../imagens/form/radio.jpg">';
    $imgRadioMarcado = '<img src="../imagens/form/radioMarcado.jpg">';
    $imgCheckbox = '<img src="../imagens/form/checkbox.jpg">';
    $imgCheckboxMarcado = '<img src="../imagens/form/checkboxMarcado.jpg">';

//    $riscoSupervisao = new RiscoQuestionarioSupervisao();
//    $riscos = $riscoSupervisao->getBySueid($sueid);
//    $modeloRestricaoQuestionario = new ModeloRestricaoQuestionario();
//    $arModeloRestricao = $modeloRestricaoQuestionario->carregaIdRelacaoPorSueid($sueid);

    foreach ($arDados as $k => $dados) {

        $arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid($dados['qstid']);

        $qstctrlobs = json_decode($dados['qstctrlobs']);
        switch ($dados['qtsresposta']) {
            case 't':
                $obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                break;
            case 'f':
                $obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                break;
            case 'n':
                $obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                break;
            default:
                $obsClass = 'divQuestaoNivel3_none';
                break;
        }

        $qstctrlimg = json_decode($dados['qstctrlimagem']);
        switch ($dados['qtsresposta']) {
            case 't':
                $imgClass = ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                $temImg = $qstctrlimg->S;
                break;
            case 'f':
                $imgClass = ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                $temImg = $qstctrlimg->N;
                break;
            case 'n':
                $imgClass = ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                $temImg = $qstctrlimg->NA;
                break;
            default:
                $imgClass = 'divQuestaoNivel4_none';
                break;
        }


        // ABRE Questionário
        if ($etqidUlt != $dados['etqid']) {
            if (!is_null($etqidUlt)) {
                //$html .= "</div>";	    		
                $html .= "</table>";
            }
            $etqidUlt = $dados['etqid'];
//                                $html .= "<div id='fieldgroup'>
//                                                <div class='legend'>{$dados['etqdsc']}</div>";
            $html .= "<table id='fieldgroup'>
                <tr><td class='legend' colspan='2'>{$dados['etqdsc']}</td></tr>";
        }

        // ETAPA
        if ($itcidUlt != $dados['itcid']) {
            $itcidNum++;
//                                $html .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdsc']}</div>";
            $html .= "<tr><td class='divItemComposicao' colspan='2'>{$itcidNum} - {$dados['itcdsc']}</td></tr>";
            $itcidUlt = $dados['itcid'];
        }

        // DIVISÃO
        if ($dvqidUlt != $dados['dvqid'] && !empty($dados['dvqid'])) {
//                                $html .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";
            $html .= "<tr><td class='divDivisao' colspan='2'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</td></tr>";
            $dvqidUlt = $dados['dvqid'];
        }
        // QUESTÃO
        if ($qstidUlt != $dados['qstid']) {

            // Sub Questão
            $subClass = ($dados['qtsresposta'] == 'f' ? 'divSubQuestao' : 'divSubQuestao_none' );
            $subItemClass = 'divSubItemQuestao';
            $subImgClass = "divSubQuestaoNivel4_none";
            $htmlSubQuestao = "";
            if ($dados['qtsresposta'] == 'f') {
                if ((count($arDadosSubQuestao) > 0) /* && (strpos($subClass, 'none') !== false ) */) {
                    //                          $htmlSubQuestao .= "<div id=\"div_subquestao_{$dados['qstid']}_{$dadosSubQuestao['sqtid']}\" class=\"{$subClass}\">
                    //                                              <div class=\"divSubQuestaoNivel1\">Tipo:</div>";
                    $htmlSubQuestao .= "<tr><td colspan=\"2\" class=\"divSubQuestaoNivel1\">Tipo:</td></tr>";
                }

                $temImgSub = false;
                foreach ($arDadosSubQuestao as $dadosSubQuestao) {

                    $arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid($dadosSubQuestao['sqtid'], ($dados['qtsid'] ? $dados['qtsid'] : 0));

                    $temImgSub = ($dadosSubQuestao['sqtimg'] == 't' && $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? true : false);

                    $htmlImgSubQuestao = '';
                    if ($temImgSub) {
                        if ($temImg && $dados['qtsresposta'] != '') {
                            $arArqSubQuestao = $arquivoRespostaSubQuestao->listaPorRespQuestao($arResultadosDadosSubQuestao[0]['rsqid'], null, $sfndeid);
                            if (count($arArqSubQuestao) > 0) {
                                $htmlImgSubQuestao .="<tr>
                                                    <td colspan='2' style='padding-left: 30px;'>
                                                    <b>Fotos da Subquestão</b> - {$dadosSubQuestao['sqtdsc']}
                                                    </td>
                                              </tr>";
                                $idxFoto = 0;
                                $totArqSubQuestao = count($arArqSubQuestao);
                                foreach ($arArqSubQuestao as $idxFoto => $arquivo) {
                                    $htmlImgSubQuestao .= '<td align="center" width="50%">
                                                        <img width="100" src="../slideshow/slideshow/verimagem.php?arqid=' . $arquivo['arqid'] . '&newwidth=100" class="img_class2">
                                                   </td>';
                                    if ($totArqSubQuestao == 1) {
                                        $htmlImgSubQuestao .= '<td align="center" width="50%">&nbsp;</td>';
                                    }
                                    $htmlImgSubQuestao .= $idxFoto != 0 && $idxFoto % 2 ? '</tr><tr>' : '';
                                }
                                $htmlImgSubQuestao .= $idxFoto != 0 && $idxFoto % 2 ? '</tr>' : '';
                            }
                        }
                    }

                    $subObsClass = $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "divSubQuestaoNivel3" : "divSubQuestaoNivel3_none";
                    //                            $checked 		= $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "checked=\"checked\"" : '';
                    $checked = $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? true : false;


                    $htmlSubQuestao .= "<tr>
                                    <td colspan='2'>
                                        <div id=\"div_subquestao_item_{$dadosSubQuestao['sqtid']}\" class=\"{$subItemClass}\">
                                            <div class=\"divSubQuestaoNivel2\">" .
                            ($checked ? $imgCheckboxMarcado : $imgCheckbox) . "
                                                <label for=\"sqtid_{$dadosSubQuestao['sqtid']}\">
                                                    {$dadosSubQuestao['sqtdsc']}
                                                </label>
                                            </div>
                                            <div class='" . $subObsClass . "' id='div_obs_sub_{$dadosSubQuestao['sqtid']}'>
                                                <div class='divSubQuestaoNivel3_1'>
                                                    <b>Observação:</b>
                                                </div>
                                                " . $arResultadosDadosSubQuestao[0]['rsqobs'] . "
                                            </div>
                                        </div>
                                        {$causaRiscoAoUsuario}
                                    </td>
                                    </tr>" . $htmlImgSubQuestao; // tr e td colocado no htmlimgsubquestao
                }
            }

            $htmlImgQuestao = '';
            if ($temImg && $dados['qtsresposta'] != '') {
                $htmlImgQuestao .="<tr>
                                <td colspan='2' class='divFotoQuestao'>
                                <b>Fotos da Questão</b> - {$dados['qstnumero']} - {$dados['qstdsc']} <br><b>Resp.:</b> " . ($dados['qtsresposta'] == 't' ? "Sim" : ($dados['qtsresposta'] == 'f' ? "Não" : "Não se aplica")) . " 
                                </td>
                           </tr>";
                $arrArquivosQuestao = $arquivoQuestaoSupervisao->listaPorRespQuestao($dados['qtsid']);
                if (is_array($arrArquivosQuestao)) {
                    $idxFoto = 0;
                    $htmlImgQuestao .= "<tr>";
                    $totArqQuestao = count($arrArquivosQuestao);
                    foreach ($arrArquivosQuestao as $idxFoto => $arquivo) {
                        $htmlImgQuestao .= '<td align="center" width="50%">
                                        <img width="100" src="../slideshow/slideshow/verimagem.php?arqid=' . $arquivo['arqid'] . '&newwidth=100" class="img_class2">
                                </td>';

                        if ($totArqQuestao == 1) {
                            $htmlImgQuestao .= '<td align="center" width="50%">&nbsp;</td>';
                        }

                        $htmlImgQuestao .= $idxFoto != 0 && $idxFoto % 2 ? '</tr><tr>' : '';
                    }
                }
                $htmlImgQuestao .= $idxFoto != 0 && $idxFoto % 2 ? '</tr>' : "";
            }

            $htmlObsQuestao = '';
            if (empty($htmlSubQuestao) /* && (strpos($obsClass, 'none') !== false) */) {
                $htmlObsQuestao = "<tr><td colspan='2' class='" . $obsClass . "' id='div_obs_{$dados['qstid']}'>
                            <div class='divQuestaoNivel3_1'>Observação:</div>" . $dados['qtsobs'] . "</td></tr>";
            }

            $html .= "<tr><td colspan='2' class='divQuestao'>
                    <div class='divQuestaoNivel1'>{$dados['qstnumero']} - {$dados['qstdsc']}</div>
                    <div class='divQuestaoNivel2'>
                            <input type='hidden' name='qtsid_{$dados['qstid']}' id='qtsid_{$dados['qstid']}' value='{$dados['qtsid']}'>
                            " . ($dados['qtsresposta'] == 't' ? $imgRadioMarcado : $imgRadio) . "
                        <label for='qstid_t_{$dados['qstid']}'>Sim</label>
                        &nbsp;&nbsp;
                            " . ($dados['qtsresposta'] == 'f' ? $imgRadioMarcado : $imgRadio) . "
                        <label for='qstid_f_{$dados['qstid']}'>Não</label>
                        &nbsp;&nbsp;
                            " . ($dados['qtsresposta'] == 'n' ? $imgRadioMarcado : $imgRadio) . "
                        <label for='qstid_n_{$dados['qstid']}'>Não se aplica</label>
                                                            </div>
                    <table width='100%'>" . $htmlObsQuestao . "</table>\n
                    <table width='100%'>" . $htmlImgQuestao . "</table>\n";
            //						if ( strpos( $subClass, "none" ) !== false )
//						{
            $html .= "<table width='100%'>" . $htmlSubQuestao . "</table>\n";
//						}
            $html .= "</td></tr>\n";
            $qstidUlt = $dados['qstid'];
        }

        echo $html;
        $html = null;
    }
    ?>
        </td>
    <tr bgcolor="#C0C0C0">
        <td colspan="3" align="center" class="notprint">
            <input type="button" value="Imprimir" onclick="window.print();">
        </td>
    </tr>
</tr>
</table>
<link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<style>
    .div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
    .div_fotos li { font-size: 1.2em; height: 255px; height: 190px; padding: 1px}
    .field_fotos{
        padding:1px; 
    }
    .field_fotos_subQuestao{
        padding:10px; 
        padding-top:1px; 
        width: 582px;
        overflow: hidden;	
    }
    .draggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;cursor:move;text-align:center;background-color:#FFFFFF}
    .nodraggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
    .draggable_space{line-height: 1.2em;width:110px;height:90px;margin:3px;float:left;cursor:pointer;background-color:#CCCCCC}
    .f_selected{border: solid 1px lightgreen;background-color: lightgreen;}
    .fechar{position:relative;margin-left:105px;top:-8px;cursor:pointer}
    .img_foto{z-index:2;}
    .img_class{margin-top:5px;}

    .divItemComposicao{
        width:100%;
        font-weight:bold;
    }

    .divDivisao{
        width:100%;
        margin-left:5px;
        font-weight:bold;
        padding:5px;
    }

    .divFotoQuestao{
        width:100%;
        margin-left:0px;
        padding:1px;
        padding-top:10px;
    }

    .divQuestao{
        width:100%;
        margin-left:10px;
        padding:5px;
    }
    .divQuestaoNivel1{
        width:598px;
    }
    .divQuestaoNivel2{
        width:100%;
    }
    .divQuestaoNivel3{
        margin-top:3px;
    }
    .divQuestaoNivel4{
        margin-top:3px;
    }
    .divQuestaoNivel4_none{
        margin-top:3px;
        display:none;
    }
    .divQuestaoNivel4_fieldset{
        width:98%
    }
    .divQuestaoNivel4_1{
        margin-top:3px;
    }
    .divQuestaoNivel4_2{
        margin-top:3px;
    }
    .divQuestaoNivel4_3{
        margin-top:3px;
        padding:2px 2px 2px 2px;
        border: 1px dashed #ccc;
        height: 70px;
    }
    .divQuestaoNivel3{
        margin-top:3px;
    }
    .divQuestaoNivel3_none{
        display:none;
        margin-top:3px;
    }
    .divQuestaoNivel3_1{
        font-weight:bold;
    }

    .divSubQuestao{
        padding-left: 20px;
        padding-top: 10px;
        padding-bottom: 5px;
    }
    .divSubQuestao_none{
        display: none;
    }

    .divSubItemQuestao{
        padding: 5px, 5px, 5px, 5px;
    }
    .divSubItemQuestao_none{
        display: none;
    }

    .divSubQuestaoNivel1{
        font-weight: bold;	
    }
    .divSubQuestaoNivel1_none{}

    .divSubQuestaoNivel2{
        font-weight: bold;
    }
    .divSubQuestaoNivel2_none{}

    .divSubQuestaoNivel3{
        padding-left: 30px;
    }
    .divSubQuestaoNivel3_none{
        display: none;	
    }

    .divSubQuestaoNivel4{
        padding-left: 30px;
        width: 90% 
    }
    .divSubQuestaoNivel4_none{
        display: none;	
    }
    #fieldgroup{
        width: 100%;
    }
    .field_fotos{
        width: 582px;
        overflow: hidden;
    }

    input[type=radio]:disabled {
        background-color:#000000;
        color:red;
    }
    .PerguntaRestricao {
        padding-left: 30px;
        padding-bottom: 10px;
    }
</style>