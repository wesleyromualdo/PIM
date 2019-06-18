<?php

include_once APPRAIZ . "www/par/_funcoes_prestacaocontas.php";

function enviaEmailChiavicatti($file)
{

    ob_start();
    print_r($_REQUEST);
    $v_requ = ob_get_contents();
    ob_clean();

    ob_start();
    print_r($_POST);
    $v_post = ob_get_contents();
    ob_clean();

    ob_start();
    $session = $_SESSION;
    unset($session['desenvolvedores_']);
    print_r($session);
    $v_session = ob_get_contents();
    ob_clean();

    ob_start();
    print_r($_SERVER);
    $v_server = ob_get_contents();
    ob_clean();

    ob_start();
    print_r($_FILES);
    $v_file = ob_get_contents();
    ob_clean();


    $conteudo = "
       <fieldset>
                <legend><b>Variaveis de _REQUEST</b></legend>
                <pre>" . $v_requ . "</pre>
       </fieldset>
       <fieldset>
                <legend><b>Variaveis de _POST</b></legend>
                <pre>" . $v_post . "</pre>
       </fieldset>
       <fieldset>
                <legend><b>Variaveis de _SESSION</b></legend>
                <pre>" . $v_session . "</pre>
       </fieldset>
       <fieldset>
                <legend><b>Variaveis de _SERVER</b></legend>
                <pre>" . $v_server . "</pre>
       </fieldset>
       <fieldset>
                <legend><b>Variaveis de _FILES</b></legend>
                <pre>" . $v_file . "</pre>
       </fieldset>        ";

//  $conteudo = "[pagina]   => {$_SERVER["REQUEST_URI"]}
//               [name]     => {$file['name']}
//                 [type]   => {$file['type']}
//                 [tmp_name] => {$file['tmp_name']}
//                 [error]  => {$file['error']}
//                 [size]   => {$file['size']}";
    // remetente
    $remetente = array("nome" => "Arquivo Falha de Envio", "email" => "monitoramento.obras@mec.gov.br");
    $assunto = "Falha no envio de arquivo";

    $destinatario = array();
    $destinatario[] = array(
        'usunome' => 'Felipe Chiavicatti',
        'usuemail' => 'felipe.chiavicatti@mec.gov.br'
    );
    $destinatario[] = array(
        'usunome' => 'Adonias Malosso',
        'usuemail' => 'malosso@gmail.com'
    );

    $enviado = enviar_email($remetente, $destinatario, $assunto, $conteudo);
}

/*
 * USADA EM:
 *
 * /simec/obras2/modulos/principal/cadOs.inc
 * /simec/obras2/modulos/principal/tecnicoEmpresa.inc
 * /simec/obras2/modulos/principal/cadTecnicoEmpresa.inc
 * /simec/obras2/modulos/principal/pagamento.inc
 * /simec/obras2/modulos/principal/listaPagamento.inc
 * OUTROS
 */

function carregaEmpresaAndListaObra(array $param = array())
{
    global $db, $sgeid;

    $somenteLeitura = ($param['obrigatorio'] ? $param['obrigatorio'] : 'S');

    $sgeid = $_POST['sgeid'];
    $sgrid = $_POST['sgrid'];
    $orgid = $_POST['orgid'];
    $sosid = $_POST['sosid'];

    if ($_POST['not(comboGrupoEmpresa)'] != true) {
        echo "<comboGrupoEmpresa>";

        $habil = 'S';
        $grupoEmpresa = new Supervisao_Grupo_Empresa();
        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC))) {
            $usuarioResp = new UsuarioResponsabilidade();
            $arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida($_SESSION['usucpf']);
            if (count($arEntidEmpresa)) {
                $param = array(
                    'sgrid' => $sgrid,
                    'entid' => $arEntidEmpresa
                );
                $arSgeid = $grupoEmpresa->pegaSgeid($param);
                $sgeid = (count($arSgeid) == 1 ? current($arSgeid) : null);
//              $habil   = 'N';
            }
        }
        $dados = $grupoEmpresa->listaCombo(array('sgrid' => $sgrid), $_POST['pagamento']);

        $db->monta_combo(
            "sgeid",
            $dados,
            $habil,
            "Selecione...",
            "carregaDependenciaEmpresa",
            '',
            '',
            '',
            $somenteLeitura,
            'sgeid'
        );
        echo "</comboGrupoEmpresa>";
    }

    if ($_POST['not(listaObras)'] != true) {
        echo "<listaObras>";
        if ($sgeid) {
            $grupoEmpresa = new Supervisao_Grupo_Empresa();
            $valorUnitario = $grupoEmpresa->pegaValorUnitarioPorSgeid($sgeid);
            $valorUnitariob = $grupoEmpresa->pegaValorUnitariobPorSgeid($sgeid);
            ?>
            <input type="hidden" rel="<?= number_format($valorUnitario, 2, ',', '.'); ?>" name="sgevalorunitario"
                   id="sgevalorunitario" value="<?php echo $valorUnitario; ?>">
            <input type="hidden" rel="<?= number_format($valorUnitariob, 2, ',', '.'); ?>" name="sgevalorunitariob"
                   id="sgevalorunitariob" value="<?php echo $valorUnitariob; ?>">
            <?php
        }
        ?>
        <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_obra">
            <?php
            if (!empty($sgrid)) {
                $supMesoregiao = new Supervisao_Grupo_Mesoregiao();
                $arMescod = $supMesoregiao->pegaMescodPorSgrid($sgrid);
                if ($arMescod) {
                    echo "<script>$(\"#hdn_mescod\").val('" . implode(",", $arMescod) . "');</script>";
                }
                $empreendimento = new Empreendimento();
                $param = array(
                    'mescod' => $arMescod,
                    'orgid' => $orgid
                );
                $arDadoEmp = $empreendimento->listaDados($param);

                if ($sosid) {
                    $osObra = new Supervisao_Os_Obra();
                    $arEmpid = $osObra->listaEmpidPorOs($sosid);
                }
                $arEmpid = $arEmpid ? $arEmpid : array();

                if (count($arDadoEmp)) {
                    ?>
                    <thead>
                    <tr style="background-color: #CDCDCD;">
                        <th valign="middle" align="center"
                            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            &nbsp;
                        </th>
                        <th valign="middle" align="center"
                            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            ID Obra
                        </th>
                        <th valign="middle" align="center"
                            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            Nome da Obra
                        </th>
                        <th valign="middle" align="center"
                            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            UF
                        </th>
                        <th valign="middle" align="center"
                            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            Mesorregio
                        </th>
                        <th valign="middle" align="center"
                            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            Município
                        </th>
                        <th valign="middle" align="center"
                            style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            Valor Laudo
                            <br>
                            (R$)
                        </th>
                    </tr>
                    </thead>
                    <?php
                    $i = 0;
                    foreach ($arDadoEmp as $dadoEmp) {
                        $obra = new Obras();
                        $obra = $obra->pegaObraPorEmpid($dadoEmp['empid']);
                        $color = ($i % 2 ? '#FFFFFF' : '#FFFFFF');
                        $i++;
                        ?>

                        <tr style="background-color: <?= $color ?>;display:<?php echo in_array(
                            $dadoEmp['empid'],
                            $arEmpid
                                                                           ) ? "" : "none" ?>" id="tr_empid_<?php echo $dadoEmp['empid'] ?>">
                            <td valign="middle" align="center"
                                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                <input type="checkbox" onclick="calculaTotalServico();" name="empid[]"
                                       id="empid_<?= $dadoEmp['empid'] ?>" value="<?= $dadoEmp['empid'] ?>">
                            </td>
                            <td valign="middle" align="left"
                                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                <?= $obra['obrid'] ?>
                            </td>

                            <td valign="middle" align="left"
                                style="padding-left: 5px; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                <?= '<label for="empid_' . $dadoEmp['empid'] . '">' . $dadoEmp['empdsc'] . '</label>' ?>
                            </td>
                            <td valign="middle" align="center"
                                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                <?= $dadoEmp['estuf'] ?>
                            </td>
                            <td valign="middle" align="left"
                                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                <?= $dadoEmp['mesdsc'] ?>
                            </td>
                            <td valign="middle" align="left"
                                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                <?= $dadoEmp['mundescricao'] ?>
                            </td>
                            <td class="valorLaudo" valign="middle" align="right"
                                style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                <?php echo($valorUnitario ? number_format($valorUnitario, 2, ',', '.') : '-'); ?>
                            </td>
                        </tr>

                        <?php
                    }
                } else {
                    ?>
                    <tr style="color: red;">
                        <td>
                            Nenhuma obra encontrada.
                        </td>
                    </tr>
                    <?php
                }
            }
            ?>
        </table>
        <?php
        echo "</listaObras>";
    }
}

function cabecalhoCronograma($obrid, $icoid = null, $ditid = null)
{
    global $db;

    $obra = new Obras($obrid);
    $flag_obrtravaedicaocronograma = $obra->getTravaCronograma($obrid);

    $htm .= "<table class='Tabela' align='center' bgcolor='#E0EEEE'>\n" . PHP_EOL;

    $cliqueAqui = "<a style='text-decoration: underline;' href=\"javascript:janela('/obras2/obras2.php?modulo=principal/popupPlanilhaOrcamentaria&acao=A&preid={$obra->preid}', 900, 600, 'planilha')\">CLIQUE AQUI</a>";

    $obraPar3 = new Obras();
    $dadosPar3 = $obraPar3->retornarDadosParaAcessoPar3($obrid);

    if (!empty($dadosPar3)) {
        $cliqueAqui = "<a style='text-decoration: underline;' href=\"/par3/par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=planilha_orcamentaria&inuid={$dadosPar3['inuid']}&inpid={$dadosPar3['inpid']}&obrid={$dadosPar3['obrid']}\" target=\"_blank\">CLIQUE AQUI</a>";
    }

    $htm .= "   <tr>" . PHP_EOL;
    $htm .= "       <td width='40%' class='divLabelForm' style='color:black; font-size: 16px; text-align: center' colspan='4' >" . PHP_EOL;
    $htm .= "       O cronograma a seguir deve ser preenchido com todas as etapas que compem a planilha orçamentária contratada.<br />" . PHP_EOL;
    $htm .= "       Todos os serviços pactuados com o FNDE devem ter sido contratados. {$cliqueAqui} para visualizar a planilha dos serviços pactuados com o FNDE.<br />" . PHP_EOL;
    $htm .= "       Caso haja serviços contratados que no fazem parte dos serviços pactuados com o FNDE, devem ser agrupados na Etapa \"SERVIÇOS NO PACTUADOS COM O FNDE\"<br />" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "   </tr>" . PHP_EOL;

    $htm .= "   <tr>" . PHP_EOL;
    $htm .= "       <td width='40%' class='divLabelForm' style='text-align: center'>" . PHP_EOL;
    $htm .= "       Descrição" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "       <td width='10%' class='divLabelForm' style='text-align: center'>" . PHP_EOL;
    $htm .= "       Início da Execução" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "       <td width='10%' class='divLabelForm' style='text-align: center'>" . PHP_EOL;
    $htm .= "       Término da Execução" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "       <td width='10%' class='divLabelForm' style='text-align: center'>" . PHP_EOL;
    $htm .= "       Valor (R$)" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
//    $htm .= "       <td width='10%' class='SubTituloEsquerda' >" . PHP_EOL;
//    $htm .= "       (%) Referente a Obra" . PHP_EOL;
//    $htm .= "       </td>" . PHP_EOL;
//    $htm .= "       <td width='10%' class='SubTituloEsquerda' >" . PHP_EOL;
//    $htm .= "       (%) Referente a Estrutura" . PHP_EOL;
//    $htm .= "       </td>" . PHP_EOL;
    $htm .= "   </tr>" . PHP_EOL;

    $contrato = new ObrasContrato();
    $arContrato = $contrato->getDadosCabecalhoByObra($obrid);

    $htm .= "   <tr class='divInputForm' style='text-align: center'>" . PHP_EOL;
    $htm .= "       <td width='40%' >" . PHP_EOL;
    $htm .= "           <b>Contrato:</b> " . PHP_EOL;
    $htm .= "           {$arContrato['entnome']}</a>" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "       <td width='10%' align='center' >" . PHP_EOL;
    $htm .= "           <label id='crtdatainicioLabel'>" . PHP_EOL;
    $htm .= "           {$arContrato['ocrdtinicioexecucao']}" . PHP_EOL;
    $htm .= "           </label>" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "       <td width='10%' align='center' >" . PHP_EOL;
    $htm .= "           <label id='crtdatafimLabel'>" . PHP_EOL;
    $htm .= "           {$arContrato['ocrdtterminoexecucao']}" . PHP_EOL;
    $htm .= "           </label>" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "       <td width='10%' style='color:#4488D5;' >" . PHP_EOL;
    $htm .= "           <label id='crtvalorLabel'>" . PHP_EOL;
    $htm .= "           " . number_format($arContrato['ocrvalorexecucao'], 2, ',', '.') . PHP_EOL;
    $htm .= "           </label>" . PHP_EOL;
    $htm .= "       </td>" . PHP_EOL;
    $htm .= "   </tr>" . PHP_EOL;

    if ($icoid) {
        $itemComposicaoObra = new ItensComposicaoObras();
        $arItemComposicao = $itemComposicaoObra->getDadosCabecalho($icoid);

        $htm .= "   <tr bgcolor='#F5F5F5'>" . PHP_EOL;
        $htm .= "       <td width='40%' >" . PHP_EOL;
        $htm .= "           <b>Etapa:</b> " . PHP_EOL;
        $htm .= "           {$arItemComposicao['itcdesc']}</a>" . PHP_EOL;
        $htm .= "       </td>" . PHP_EOL;
        $htm .= "       <td width='10%' align='center' >" . PHP_EOL;
        $htm .= "           <label id='icodtinicioitemLabel'>" . PHP_EOL;
        $htm .= "           {$arItemComposicao['icodtinicioitem']}" . PHP_EOL;
        $htm .= "           </label>" . PHP_EOL;
        $htm .= "       </td>" . PHP_EOL;
        $htm .= "       <td width='10%' align='center' >" . PHP_EOL;
        $htm .= "           <label id='icodterminoitemLabel'>" . PHP_EOL;
        $htm .= "           {$arItemComposicao['icodterminoitem']}" . PHP_EOL;
        $htm .= "           </label>" . PHP_EOL;
        $htm .= "       </td>" . PHP_EOL;
        $htm .= "       <td width='10%' align='right' style='color:#4488D5;' >" . PHP_EOL;
        $htm .= "           <label id='icovaloritemLabel'>" . PHP_EOL;
        $htm .= "           " . number_format($arItemComposicao['icovlritem'], 2, ',', '.') . PHP_EOL;
        $htm .= "           </label>" . PHP_EOL;
        $htm .= "       </td>" . PHP_EOL;
        $htm .= "   </tr>" . PHP_EOL;
    }

    if ($ditid) {
        $detalheItem = new DetalheItem();
        $arDetalheItem = $detalheItem->getDadosCabecalho($ditid);

        for ($i = 0; $i < count($arDetalheItem); $i++) {
            $arDetalheItem[$i]['label'] = ($i == 0 ? 'Item' : 'Sub Item');

            $htm .= "   <tr bgcolor='#F5F5F5'>" . PHP_EOL;
            $htm .= "       <td width='40%' >" . PHP_EOL;
            $htm .= "           <b>{$arDetalheItem[$i]['label']}:</b> " . PHP_EOL;
            $htm .= "           {$arDetalheItem[$i]['ditdsc']}</a>" . PHP_EOL;
            $htm .= "       </td>" . PHP_EOL;
            $htm .= "       <td width='10%' align='center' >" . PHP_EOL;
            $htm .= "           <label id='ditdtinicioitemLabel'>" . PHP_EOL;
            $htm .= "           {$arDetalheItem[$i]['ditdtinicioitem']}" . PHP_EOL;
            $htm .= "           </label>" . PHP_EOL;
            $htm .= "       </td>" . PHP_EOL;
            $htm .= "       <td width='10%' align='center' >" . PHP_EOL;
            $htm .= "           <label id='ditdtterminoitemLabel'>" . PHP_EOL;
            $htm .= "           {$arDetalheItem[$i]['ditdtterminoitem']}" . PHP_EOL;
            $htm .= "           </label>" . PHP_EOL;
            $htm .= "       </td>" . PHP_EOL;
            $htm .= "       <td width='10%' align='right' style='color:#4488D5;' >" . PHP_EOL;
            $htm .= "           <label id='ditvaloritemLabel'>" . PHP_EOL;
            $htm .= "           " . number_format($arDetalheItem[$i]['ditvalor'], 2, ',', '.') . PHP_EOL;
            $htm .= "           </label>" . PHP_EOL;
            $htm .= "       </td>" . PHP_EOL;
            $htm .= "   </tr>" . PHP_EOL;
        }
    }

    $htm .= "</table>" . PHP_EOL;

    return $htm;
}

function cabecalhoObra($obrid, $tipo = 'padrao' /* simples */)
{
    global $db;

    /* Caso usario (FISCAL, CUNSULTA UNIDADE ou GESTOR) tente acessar uma obra atualizada via url, verifica se o mesmo possui obras desatualizadas
         em sua lista, caso possua redirecioan o mesmo para a lista de obras */
    if (possui_perfil(array(
            PFLCOD_GESTOR_UNIDADE,
            PFLCOD_SUPERVISOR_UNIDADE,
            PFLCOD_CONSULTA_UNIDADE
        )) && !possui_perfil(PFLCOD_SUPER_USUARIO)) {
        $flgFiscal = possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE)) ? true : false;

        $flgDataUltimaVistoria = verificaDataUltimaVistoriaObra($_SESSION['usucpf'], $flgFiscal);

        if (!$flgDataUltimaVistoria) {
            if (isset($_GET["obrid"]) && $_GET["obrid"] != "") {
                $obrid = $_GET["obrid"];
            }

            $sqlPegaObra = "SELECT o.obrdtultvistoria, o.docid, strid FROM obras2.obras o WHERE o.obrid = {$obrid};";

            $obra = $db->pegaLinha($sqlPegaObra);

            if ($obra["strid"] == 4 || $obra["strid"] == 5) {
                if ($obra["obrdtultvistoria"] == "") {
                    $sql2 = "SELECT
                              min(hd.htddata)
                            FROM
                              workflow.historicodocumento hd
                            WHERE
                              hd.docid = {$obra["docid"]}
                            AND
                              hd.aedid IN (
                                SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690
                              );";

                    $dtUltVistoria = $db->pegaUm($sql2);
                } else {
                    $dtUltVistoria = $obra["obrdtultvistoria"];
                }

                $objDtUltVistoria = new DateTime($dtUltVistoria);
                $objDtAtual = new DateTime("now");
                $diff = $objDtUltVistoria->diff($objDtAtual);
                $diasRestantes = $diff->format("%R%a");

//                    if($diasRestantes < 30){
//                        header("Location:obras2.php?modulo=principal/listaObras&acao=A&flgObraAtualizada=true");
//                    }
            }
        }
    }


    $obra = new Obras();
    $dados = $obra->getDadosCabecalho($obrid);
    if($obrid) {
        $dados['obrid'] = buscaObridSec($obrid);
    }

    if (empty($tipo)) {
        $tipo = 'padrao';
    }

    if ($tipo == 'padrao') {
        $htmlBarra = montaBarraFerramentasTooltip($obrid);

//            $avisoPendencia = '';
//            if ($dados['empesfera'] == 'M') {
//                $obrasPendentes = getObrasPendentesPAR($dados['muncod']);
//                $cod = 'muncod="' . $dados['muncod'] . '"';
//            } else {
//                $obrasPendentes = getObrasPendentesPAR(null, $dados['estuf'], $dados['empesfera']);
//                $cod = 'estuf="' . $dados['estuf'] . '"';
//            }
//
//            if ($obrasPendentes) {
        $avisoPendencia = '';
        if ($dados['empesfera'] == 'M') {
            $obrasPendentes = getObrasPendentesPAR($dados['muncod']);
            $cod = 'muncod="' . $dados['muncod'] . '"';
        } else {
            $obrasPendentes = getObrasPendentesPAR(null, $dados['estuf'], $dados['empesfera']);
            $cod = 'estuf="' . $dados['estuf'] . '"';
        }

        if ($obrasPendentes) {
            $avisoPendencia = '
                        <tr>
                            <td colspan="2" style="background: #f00; color: #fff">
                                Senhor Prefeito/Secretrio,
                                o seu município j recebeu recursos para as obras <a class="detalhar_pendencias_obras" href="#" style="color: #fff; text-decoration: underline;" ' . $cod . '">AQUI LISTADAS</a> e estas apresentam pendências em sua execução.
                                Tais pendências podero impactar na análise e aprovao de novas demandas de obras, por parte do FNDE. Caso a Situação tenha sido resolvida, favor atualizar o mdulo Obras 2.0 (monitoramento de obras) - Equipe PAR MEC/FNDE. Clique <a class="detalhar_pendencias_obras" href="#" style="color: #fff; text-decoration: underline;" ' . $cod . '">AQUI</a> para ver detalhes.
                            </td>
                        </tr>';
        }

        $htm = "<link rel=\"stylesheet\" type=\"text/css\" media=\"screen, print\" href=\"../includes/layoutNovoObras2.css\">
                <table align=\"center\" style='background-color: #FFFFff' border=\"0\" class=\"Tabela \" cellpadding=\"3\" cellspacing=\"1\">
                    {$htmlBarra}
                    {$avisoPendencia}
                    <tr>
                            <td class=\"divLabelForm\" width=\"20%\"><b>Tipo de ensino:</b></td>
                            <td>
                                    {$dados['orgdesc']}
                            </td>
                    </tr>";

        if ($dados['demid']) {
            $htm .= "<tr>
                        <td class=\"divLabelForm\" width=\"20%\"><b>Pré-obra:</b></td>
                        <td>
                            <a target=\"preobra\" href=\"?modulo=principal/preobra/cadDemanda&acao=A&demid={$dados['demid']}\">
                            ({$dados['demid']}) {$dados['demnome']}
                            </a>
                        </td>
                     </tr>";
        }

        if (!empty($dados['preid'])) {
            $sql = "SELECT
                        so.sbaid, so.sobano
                    FROM
                        par.subacaoobra  so
                    JOIN par.subacao s on s.sbaid = so.sbaid AND s.sbastatus = 'A'
                    WHERE so.preid = {$dados['preid']}";

            $dadoPreObra = $db->pegaLinha($sql);


//                 $href = ( $dadoPreObra ? "/par/par.php?modulo=principal/subacaoObras&acao=A&preid={$dados['preid']}&sbaid={$dadoPreObra['sbaid']}&ano={$dadoPreObra['sobano']}&vizualiar=true" :
//                                 "/par/par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid={$dados['preid']}&muncod={$dados['premuncod']}&vizualiar=true"
//                         );
//                $href = "/par/par.php?modulo=principal/programas/proinfancia/visualizarPreObra&acao=A&preid={$dados['preid']}";
            $href = "/obras2/obras2.php?modulo=principal/programas/proinfancia/visualizarPreObra&acao=A&preid={$dados['preid']}";
            //sugesto do Raniery, uma forma de liberar visualizao do questionario na tela visualizarPreObra.inc, visto que l a condio  por perfil do PAR e precisamos liberar para o Obras2
            $_SESSION['obras2']['par']['preid'] = $dados['preid'];

            $htm .= "<tr>
                        <td class=\"divLabelForm\" width=\"20%\"><b>Pré-obra:</b></td>
                        <td>
                            <a href=\"javascript:janela('{$href}',800,600,'preobra')\">
                            ({$dados['preid']}) {$dados['predescricao']}
                            </a>
                        </td>
                     </tr>";
        }

        if (!empty($dados['obrid_par3'])) {
            $htm .= "<tr>
                        <td class=\"divLabelForm\" width=\"20%\"><b>Pré-obra:</b></td>
                        <td>
                            <a href=\"/par3/par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid={$dados['inuid']}&inpid={$dados['inpid']}&obrid={$dados['obrid_par3']}\" target=\"_blank\",>
                            ({$dados['obrid_par3']}) {$dados['obrdsc']}
                            </a>
                        </td>
                     </tr>";
        }

        $htm .= "<tr>
                    <td class=\"divLabelForm\" width=\"20%\"><b>Município - UF:</b></td>
                    <td>
                        {$dados['mundescricao']} - {$dados['estuf']}
                    </td>
                </tr>
                <tr>
                    <td class=\"divLabelForm\" width=\"20%\"><b>Obra:</b></td>
                    <td>
                        ({$dados['obrid']}) {$dados['obrnome']}
                    </td>
                </tr>";

        $htm3 = $obra->getLinhaTabelaPercentExecutado($obrid);
        $htm .= $htm3;

        if ($dados['obrid_1']) {
            $htm .= "<tr>
                        <td class=\"divLabelForm\" width=\"20%\"><b>&nbsp;</b></td>
                        <td>
                            Para acessar o obras 1 e ver o histórico desta obra
                            <a href=\"/obras/obras.php?modulo=principal/cadastro&acao=A&obrid={$dados['obrid_1']}\">
                            clique aqui
                            </a>.
                        </td>
                     </tr>";
        }

        $htm .= "</table>";
    } else {
        $htm = "<table align=\"center\" bgcolor=\"#f5f5f5\" border=\"0\" class=\"Tabela\" cellpadding=\"3\" cellspacing=\"1\">
                    <tr>
                            <td class=\"divLabelForm\" width=\"20%\"><b>Tipo de ensino:</b></td>
                            <td>
                                    {$dados['orgdesc']}
                            </td>
                    </tr>
                    ";

        $htm .= "<tr>
                    <td class=\"divLabelForm\" width=\"20%\"><b>Município - UF:</b></td>
                    <td>
                        {$dados['mundescricao']} - {$dados['estuf']}
                    </td>
                </tr>
                <tr>
                    <td class=\"divLabelForm\" width=\"20%\"><b>Obras:</b></td>
                    <td>
                        ({$dados['obrid']}) {$dados['obrnome']}
                    </td>
                </tr>";

        $htm .= "</table>";
    }


    return $htm;
}

function buscaObridSec($obrid){
    global $db;

    $sql = "SELECT coalesce(lpad(cast(entidsecretaria as varchar),2,'0'), '')  || '' || lpad(cast(obrid as varchar),6,'0') as obrid
            from obras2.obras where obrid = {$obrid}";

    return $db->pegaUm($sql);
}

/**
 * @param $alias passar o alias da tabela de obras utilizado na query
 */
function montaCampoObridSec($alias = null){

    if(!empty($alias)){
        $alias .= ".";
    }
    $string = "coalesce(lpad(cast(".$alias."entidsecretaria as varchar),2,'0'), '')  || '' || lpad(cast(".$alias."obrid as varchar),6,'0')";

    return $string;
}

function limpaObridSec($obridSec){
    $tamanho = strlen ( $obridSec );
    if($tamanho == 8){
        $retorno =  substr($obridSec, 2);
        $retorno =  ltrim($retorno, "0");
        return $retorno;
    } else {
        $retorno =  ltrim($obridSec, "0");
        return $retorno;
    }

}

function updateLevelCronograma($vlr, $atiid, $id, $tipo)
{
    $atividade = new Atividade($atiid);
    $valorProjeto = $atividade->ativalor;

    $vlr = (is_numeric($vlr) ? $vlr : MoedaToBd($vlr));

    switch ($tipo) {
        /*
             * Quando est na ETAPA atualiza a estrutura e a atividade(obra)
             */
        case 'etapa':
            $estrutura = new EstruturaObra($id);
            $estrutura->eobvalor = $vlr;
            $estrutura->salvar();

            $vlrSoma = $estrutura->getSomaEstruturaByAtividade($atiid, $id);

            if (($vlrSoma + $vlr) > $valorProjeto) {
                $atividade->ativalor = ($vlrSoma + $vlr);
                $atividade->salvar();
            }
            break;
        /*
             * Quando est no DETALHAMENTO atualiza a etapa e chama recursivamente com o tipo = ETAPA
             */
        case 'detalhamento':
            $etapa = new ItensComposicaoEstrutura($id);
            $etapa->icovlritem = $vlr;
            $etapa->salvar();


            $vlrSoma = $etapa->getSomaEtapaByEstrutura($etapa->eobid);
            $estrutura = new EstruturaObra($etapa->eobid);

            if (($vlrSoma) > $estrutura->eobvalor) {
                updateLevelCronograma($vlrSoma, $atiid, $etapa->eobid, 'etapa');
            }
            break;
    }
}

/*
     * empreendimento
     * obra
     * orgao
     */

function verificaSessao($tela)
{
    $return = true;
    switch ($tela) {
        case 'empreendimento':
            if (empty($_SESSION['obras2']['orgid']) || empty($_SESSION['obras2']['empid'])) {
                $return = false;
            }
            break;
        case 'obra':
            if (empty($_SESSION['obras2']['orgid']) || empty($_SESSION['obras2']['empid']) || empty($_SESSION['obras2']['obrid'])) {
                $return = false;
            }
            break;
        case 'idobra':
            if (empty($_SESSION['obras2']['obrid'])) {
                $return = false;
            }
            break;
        case 'orgao':
            if (empty($_SESSION['obras2']['orgid'])) {
                $return = false;
            }
            break;
    }

    if ($return == false) {
        die("<script>
                alert('Faltam parmetros para acessar esta tela!');
                location.href = '?modulo=inicio&acao=C';
             </script>");
    }
}

function possui_perfil($pflcods)
{

    global $db;

    if (!isset($_SESSION[$_SESSION['usucpf']]['perfilusuario'])) {
        $sql = "
                SELECT pflcod
                FROM seguranca.perfilusuario
                WHERE usucpf = '" . $_SESSION['usucpf'] . "'
            ";
        $_SESSION[$_SESSION['usucpf']]['perfilusuario'] = $db->carregarColuna($sql);
    }

    if (is_array($pflcods)) {
        $pflcods = array_map("intval", $pflcods);
        $pflcods = array_unique($pflcods);
    } else {
        $pflcods = array((integer)$pflcods);
    }
    if (count($pflcods) == 0) {
        return false;
    }

    foreach ($pflcods as $pflcod) {
        if (in_array($pflcod, $_SESSION[$_SESSION['usucpf']]['perfilusuario'])) {
            return true;
        }
    }
    return false;
}

function listaObras()
{

    global $db;

    $where = array('obr.obrstatus = \'A\'', 'org.orgid = ' . $_REQUEST['orgid']);

    if ($_REQUEST['obrbuscatexto'] != '') {
        $where[] = " ( upper(obr.obrdesc) ILIKE upper('%" . $_REQUEST['obrbuscatexto'] . "%')
                       OR obr.obrid::character varying ILIKE upper('%" . $_REQUEST['obrbuscatexto'] . "%') ) ";
    }

    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $innerResp = '';
        $arrEst = array(
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_ESTADUAL,
            PFLCOD_EMPRESA_CONTRATADA
        );
        $arrObr = array(
            PFLCOD_EMPRESA_CONTRATADA,
            PFLCOD_SUPERVISOR_UNIDADE
        );
        $arrOrg = array(
            PFLCOD_ADMINISTRADOR,
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_ESTADUAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC
        );
        $arrUni = array(
            PFLCOD_AUDITOR_INTERNO,
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_UNIDADE,
            PFLCOD_GESTOR_UNIDADE,
            PFLCOD_SUPERVISOR_UNIDADE
        );
        $resp = array();
        if (possui_perfil($arrEst)) {
            $resp[] = " urs.estuf = end.estuf ";
        }
        if (possui_perfil($arrObr)) {
            $resp[] = " urs.obrid = obr.obrid ";
        }
        if (possui_perfil($arrOrg)) {
            $resp[] = " urs.orgid = obr.orgid ";
        }
        if (possui_perfil($arrUni)) {
            $resp[] = " urs.entid = obr.entid ";
        }
        if ($resp[0] != '') {
            $innerResp = "INNER JOIN obras2.usuarioresponsabilidade urs ON " . implode(' AND ', $resp);
        }
    }

    $sql = "SELECT
                obrid,
                obrdesc
            FROM
                obras2.obrainfraestrutura obr
            INNER JOIN obras2.tipoobra          tob ON tob.tobid = obr.tobid
            INNER JOIN obras2.orgao             org ON org.orgid = obr.orgid
            INNER JOIN obras2.classificacaoobra clo ON clo.cloid = obr.cloid
            INNER JOIN entidade.endereco        ede ON ede.endid = obr.endid
            $innerResp
            WHERE
                " . implode(' AND ', $where) . "
            ORDER BY
                2";
    $cabecalho = array("ID", "Descrição");
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '');
}

function monta_titulo_listaObras()
{

    global $db;

    $sql = "SELECT
                    orgdesc
            FROM
                    obras2.orgao
            WHERE
                    orgid = " . $_REQUEST['orgid'];
    $org = $db->pegaUm($sql);

    monta_titulo('Lista de Empreendimentos - ' . $org, '');
}

function obras_possuiPerfilSemVinculo()
{

    global $db;

    $sql = "SELECT
                    count(*)
            FROM
                    seguranca.perfil p
            INNER JOIN seguranca.perfilusuario       u ON u.pflcod = p.pflcod
            LEFT  JOIN obras2.tprperfil             tp ON tp.pflcod = p.pflcod
            LEFT  JOIN obras2.tiporesponsabilidade  tr ON tr.tprcod = tp.tprcod
            WHERE
                    p.pflstatus = 'A' AND
                    p.sisid = '15' AND
                    u.usucpf = '" . $_SESSION['usucpf'] . "' AND
                    tr.tprcod IS NULL ";
    return $db->pegaUm($sql) > 0;
}

function obras_pegarUnidadesPermitidas()
{

    global $db;
    static $unidades = null;

    if ($unidades === null) {
        if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()) {
            return false;
        } else {
            // pega as unidades do perfil do usurio
            $sql = "SELECT
                            ur.entid
                    FROM
                            obras2.usuarioresponsabilidade ur
                    INNER JOIN entidade.entidade        et ON et.entid = ur.entid
                    INNER JOIN seguranca.perfil          p ON p.pflcod = ur.pflcod
                    INNER JOIN seguranca.perfilusuario  pu ON pu.pflcod = ur.pflcod AND pu.usucpf = ur.usucpf
                    WHERE
                            ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
                            ur.rpustatus = 'A' AND
                            p.sisid = 15
                    UNION ALL
                    SELECT DISTINCT
                            oi.entidunidade
                    FROM
                            obras2.usuarioresponsabilidade ur
                    INNER JOIN obras2.obrainfraestrutura oi USING (obrid)
                    WHERE
                            ur.usucpf = '" . $_SESSION['usucpf'] . "'
                            AND ur.rpustatus = 'A'
                            AND ur.obrid IS NOT NULL";
        }


        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();
        $unidades = array();

        foreach ($dados as $linha) {
            array_push($unidades, $linha['entid']);
        }
    }
    return $unidades;
}

function cria_abas_Obras($abacod_tela, $url, $parametros, Array $arMnuid = array())
{

    global $db;

    $where = "";
    if ($_SESSION['sisid']) {
        $where = " AND menu.sisid = {$_SESSION['sisid']} ";
    }

    if ($arMnuid) {
        $filtro = "AND menu.mnuid NOT IN (" . implode(',', $arMnuid) . ")";
    }

    //Funo cria aba que monta as abas visualmente
    if (trim($abacod_tela) <> '') {
        $sql = "SELECT
                        menu.mnuid,
                        menu.mnudsc,
                        menu.mnulink,
                        menu.mnutransacao
                FROM
                        menu, aba_menu
                WHERE
                        menu.mnuid=aba_menu.mnuid
                        AND aba_menu.abacod=" . $abacod_tela . "
                        $where
                        AND menu.mnuid in   (
                                                                SELECT DISTINCT m2.mnuid
                                                                FROM perfilmenu m2, perfilusuario p
                                                                WHERE m2.pflcod=p.pflcod AND p.usucpf='" . $_SESSION['usucpf'] . "'
                                                                )
                        $filtro
                ORDER BY menu.mnucod";
        $RS = $db->carregar($sql);
        if (is_array($RS)) {
            print '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint"><tr><td><table cellpadding="0" cellspacing="0" align="left"><tr>';
            $nlinhas = count($RS) - 1;
            for ($j = 0; $j <= $nlinhas; $j++) {
                extract($RS[$j]);
                if ($url <> $mnulink && $j == 0) {
                    $gifaba = "aba_nosel_ini.gif";
                } elseif ($url == $mnulink && $j == 0) {
                    $gifaba = "aba_esq_sel_ini.gif";
                } elseif ($gifaba == 'aba_esq_sel_ini.gif' or $gifaba == 'aba_esq_sel.gif') {
                    $gifaba = "aba_dir_sel.gif";
                } elseif ($url <> $mnulink) {
                    $gifaba = "aba_nosel.gif";
                } elseif ($url == $mnulink) {
                    $gifaba = "aba_esq_sel.gif";
                }
                $parametro = is_array($parametros) ? $parametros[$j] : $parametros;
                if ($url == $mnulink) {
                    $giffundo_aba = "aba_fundo_sel.gif";
                    $cor_fonteaba = "#000055";
                } else {
                    $giffundo_aba = "aba_fundo_nosel.gif";
                    $cor_fonteaba = "#4488cc";
                }
                print '<td height="20" valign="top"><img src="../imagens/' . $gifaba . '" width="11" height="20" alt="" border="0"></td>';
                print '<td height="20" align="center" valign="middle" background="../imagens/' . $giffundo_aba . '" style="color:' . $cor_fonteaba . '; padding-left: 10px; padding-right: 10px;">';
                if ($mnulink <> $url) {
                    print '<a  href="' . $mnulink . $parametro . '" style="color:' . $cor_fonteaba . ';" title="' . $mnutransacao . '">' . $mnutransacao . '</a>';
                } else {
                    print $mnutransacao . '</td>';
                }
            }
            if ($gifaba == 'aba_esq_sel_ini.gif' or $gifaba == 'aba_esq_sel.gif') {
                $gifaba = "aba_dir_sel_fim.gif";
            } else {
                $gifaba = "aba_nosel_fim.gif";
            }
            print '<td height="20" valign="top"><img src="../imagens/' . $gifaba . '" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';
        }
    }
}

function verificaAcessoEmOrgid()
{
//        $userResp = new UsuarioResponsabilidade();
//        $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
//
//        if (empty($arOrgid) /* && $_SESSION['obras2']['acesso'] !== false */) {
//            $_SESSION['obras2']['acesso'] = false;
//            $_SESSION['obras2']['acessocpf'] = $_SESSION['usucpf'];
//            header('Location: obras2.php?modulo=inicio&acao=C');
//            die;
//        } else/* if ( !empty($arOrgid) ) */ {
//            $_SESSION['obras2']['acesso'] = true;
//            $_SESSION['obras2']['acessocpf'] = $_SESSION['usucpf'];
//        }

    return array(3);
}

function redirecionaTelaInicial()
{
    global $db;

    $arPerfilEmpresaMI = array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_ADMINISTRATIVO);
    $arPerfilEmpresa = array(
        PFLCOD_EMPRESA_CONTRATADA,
        PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
        PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
        PFLCOD_EMPRESA_VISTORIADORA_COORDENADOR
    );

    if (!possui_perfil(PFLCOD_SUPER_USUARIO) && possui_perfil($arPerfilEmpresaMI)) {
        header('Location: obras2.php?modulo=principal/listaObrasMI&acao=A');
    } elseif (!possui_perfil(PFLCOD_SUPER_USUARIO) && possui_perfil($arPerfilEmpresa)) {
        header('Location: obras2.php?modulo=principal/listaEmpreendimentoEmpresa&acao=A');
    } elseif (!possui_perfil(PFLCOD_SUPER_USUARIO) && (possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_CONSULTA_TIPO_DE_ENSINO) || possui_perfil(PFLCOD_CONSULTA_ESTADUAL))) {
        header('Location: obras2.php?modulo=principal/inicioLista&acao=A');
    } else {
        header('Location: obras2.php?modulo=principal/listaObras&acao=A');
    }

    exit;
}

function possuiPerfil($pflcods)
{
    global $db;

    if ($db->testa_superuser()) {
        return true;
    } else {
        if (is_array($pflcods)) {
            $pflcods = array_map("intval", $pflcods);
            $pflcods = array_unique($pflcods);
        } else {
            $pflcods = array((integer)$pflcods);
        }
        if (count($pflcods) == 0) {
            return false;
        }
        $sql = "
        select
        count(*)
        from seguranca.perfilusuario
        where
        usucpf = '" . $_SESSION['usucpf'] . "' and
        pflcod in ( " . implode(",", $pflcods) . " ) ";
        return $db->pegaUm($sql) > 0;
    }
}

function getArAba($tipoAba)
{
    $arAba = array();

    switch ($tipoAba) {
        case 'listacontrato':
            /* /* $arAba[] = array("id" => 1,
                  //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
                  //     "descricao" => "Educao Superior",
                  //     "link" => "?modulo=principal/listaContrato&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);

                  // $arAba[] = array("id" => 2,
                  //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
                  //     "descricao" => "Educao Profissional",
                  //     "link" => "?modulo=principal/listaContrato&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);

                  // $arAba[] = array("id" => 3,
                  //     "orgid" => ORGID_EDUCACAO_BASICA,
                  //     "descricao" => "Educao Bsica",
                  //     "link" => "?modulo=principal/listaContrato&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);

                  // $arAba[] = array("id" => 4,
                  //     "orgid" => ORGID_ADMINISTRATIVO,
                  //     "descricao" => "Administrativo",
                  //     "link" => "?modulo=principal/listaContrato&acao=A&orgid=" . ORGID_ADMINISTRATIVO);

                  // $arAba[] = array("id" => 5,
                  //     "orgid" => ORGID_HOSPITAIS,
                  //     "descricao" => "Hospitais",
                  //     "link" => "?modulo=principal/listaContrato&acao=A&orgid=" . ORGID_HOSPITAIS);

                  //            $arAbaLoop = $arAba;
                  //            $arAba     = array();
                  //            $userResp  = new UsuarioResponsabilidade();
                  //            $arOrgid   = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
                  //            for ($i=0; $i < count( $arAbaLoop ); $i++){
                  //                if ( in_array( $arAbaLoop[$i]['orgid'], $arOrgid ) ){
                  //                    array_push($arAba, $arAbaLoop[$i]);
                  //                }
                  //            } */
            break;
        case 'listalicitacao':
            /* /* $arAba[] = array("id" => 1,
                  //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
                  //     "descricao" => "Educao Superior",
                  //     "link" => "?modulo=principal/listaLicitacao&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);

                  // $arAba[] = array("id" => 2,
                  //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
                  //     "descricao" => "Educao Profissional",
                  //     "link" => "?modulo=principal/listaLicitacao&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);

                  // $arAba[] = array("id" => 3,
                  //     "orgid" => ORGID_EDUCACAO_BASICA,
                  //     "descricao" => "Educao Bsica",
                  //     "link" => "?modulo=principal/listaLicitacao&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);

                  // $arAba[] = array("id" => 4,
                  //     "orgid" => ORGID_ADMINISTRATIVO,
                  //     "descricao" => "Administrativo",
                  //     "link" => "?modulo=principal/listaLicitacao&acao=A&orgid=" . ORGID_ADMINISTRATIVO);

                  // $arAba[] = array("id" => 5,
                  //     "orgid" => ORGID_HOSPITAIS,
                  //     "descricao" => "Hospitais",
                  //     "link" => "?modulo=principal/listaLicitacao&acao=A&orgid=" . ORGID_HOSPITAIS);

                  //            $arAbaLoop = $arAba;
                  //            $arAba     = array();
                  //            $userResp  = new UsuarioResponsabilidade();
                  //            $arOrgid   = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
                  //            for ($i=0; $i < count( $arAbaLoop ); $i++){
                  //                if ( in_array( $arAbaLoop[$i]['orgid'], $arOrgid ) ){
                  //                    array_push($arAba, $arAbaLoop[$i]);
                  //                }
                  //            } */
            break;
        case 'listaorgaoemprendimento':
            /* $arAba[] = array("id" => 1,
                  //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
                  //     "descricao" => "Educao Superior",
                  //     "link" => "?modulo=principal/listaEmpreendimentos&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);

                  // $arAba[] = array("id" => 2,
                  //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
                  //     "descricao" => "Educao Profissional",
                  //     "link" => "?modulo=principal/listaEmpreendimentos&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);

                  // $arAba[] = array("id" => 3,
                  //     "orgid" => ORGID_EDUCACAO_BASICA,
                  //     "descricao" => "Educao Bsica",
                  //     "link" => "?modulo=principal/listaEmpreendimentos&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);

                  // $arAba[] = array("id" => 4,
                  //     "orgid" => ORGID_ADMINISTRATIVO,
                  //     "descricao" => "Administrativo",
                  //     "link" => "?modulo=principal/listaEmpreendimentos&acao=A&orgid=" . ORGID_ADMINISTRATIVO);

                  // $arAba[] = array("id" => 5,
                  //     "orgid" => ORGID_HOSPITAIS,
                  //     "descricao" => "Hospitais",
                  //     "link" => "?modulo=principal/listaEmpreendimentos&acao=A&orgid=" . ORGID_HOSPITAIS);

                  // $arAbaLoop = $arAba;
                  // $arAba = array();
                  // $userResp = new UsuarioResponsabilidade();
                  // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
                  // for ($i = 0; $i < count($arAbaLoop); $i++) {
                  //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
                  //         array_push($arAba, $arAbaLoop[$i]);
                  //     }
                  // } */
            break;
        case 'listaContato':
            /* /* $arAba[] = array("id" => 1,
                  //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
                  //     "descricao" => "Educao Superior",
                  //     "link" => "?modulo=principal/cadContato&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);

                  // $arAba[] = array("id" => 2,
                  //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
                  //     "descricao" => "Educao Profissional",
                  //     "link" => "?modulo=principal/cadContato&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);

                  // $arAba[] = array("id" => 3,
                  //     "orgid" => ORGID_EDUCACAO_BASICA,
                  //     "descricao" => "Educao Bsica",
                  //     "link" => "?modulo=principal/cadContato&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);

                  // $arAba[] = array("id" => 4,
                  //     "orgid" => ORGID_ADMINISTRATIVO,
                  //     "descricao" => "Administrativo",
                  //     "link" => "?modulo=principal/cadContato&acao=A&orgid=" . ORGID_ADMINISTRATIVO);

                  // $arAba[] = array("id" => 5,
                  //     "orgid" => ORGID_HOSPITAIS,
                  //     "descricao" => "Hospitais",
                  //     "link" => "?modulo=principal/cadContato&acao=A&orgid=" . ORGID_HOSPITAIS);

                  // $arAbaLoop = $arAba;
                  // $arAba = array();
                  // $userResp = new UsuarioResponsabilidade();
                  // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
                  // for ($i = 0; $i < count($arAbaLoop); $i++) {
                  //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
                  //         array_push($arAba, $arAbaLoop[$i]);
                  //     }
                  // } */
            break;
        case 'listaorgaoemprendimentoempresa':
            /* /* $arAba[] = array("id" => 1,
                  //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
                  //     "descricao" => "Educao Superior",
                  //     "link" => "?modulo=principal/listaEmpreendimentoEmpresa&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);

                  // $arAba[] = array("id" => 2,
                  //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
                  //     "descricao" => "Educao Profissional",
                  //     "link" => "?modulo=principal/listaEmpreendimentoEmpresa&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);

                  // $arAba[] = array("id" => 3,
                  //     "orgid" => ORGID_EDUCACAO_BASICA,
                  //     "descricao" => "Educao Bsica",
                  //     "link" => "?modulo=principal/listaEmpreendimentoEmpresa&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);

                  // $arAba[] = array("id" => 4,
                  //     "orgid" => ORGID_ADMINISTRATIVO,
                  //     "descricao" => "Administrativo",
                  //     "link" => "?modulo=principal/listaEmpreendimentoEmpresa&acao=A&orgid=" . ORGID_ADMINISTRATIVO);

                  // $arAba[] = array("id" => 5,
                  //     "orgid" => ORGID_HOSPITAIS,
                  //     "descricao" => "Hospitais",
                  //     "link" => "?modulo=principal/listaEmpreendimentoEmpresa&acao=A&orgid=" . ORGID_HOSPITAIS);

                  // $arAbaLoop = $arAba;
                  // $arAba = array();
                  // $userResp = new UsuarioResponsabilidade();
                  // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
                  // for ($i = 0; $i < count($arAbaLoop); $i++) {
                  //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
                  //         array_push($arAba, $arAbaLoop[$i]);
                  //     }
                  // } */
            break;
        case 'listaorgao':
            /* /* $arAba[] = array("id" => 1,
                  //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
                  //     "descricao" => "Educao Superior",
                  //     "link" => "?modulo=principal/listaObras&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);

                  // $arAba[] = array("id" => 2,
                  //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
                  //     "descricao" => "Educao Profissional",
                  //     "link" => "?modulo=principal/listaObras&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);

                  // $arAba[] = array("id" => 3,
                  //     "orgid" => ORGID_EDUCACAO_BASICA,
                  //     "descricao" => "Educao Bsica",
                  //     "link" => "?modulo=principal/listaObras&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);

                  // $arAba[] = array("id" => 4,
                  //     "orgid" => ORGID_ADMINISTRATIVO,
                  //     "descricao" => "Administrativo",
                  //     "link" => "?modulo=principal/listaObras&acao=A&orgid=" . ORGID_ADMINISTRATIVO);

                  // $arAba[] = array("id" => 5,
                  //     "orgid" => ORGID_HOSPITAIS,
                  //     "descricao" => "Hospitais",
                  //     "link" => "?modulo=principal/listaObras&acao=A&orgid=" . ORGID_HOSPITAIS);

                  // $arAbaLoop = $arAba;
                  // $arAba = array();
                  // $userResp = new UsuarioResponsabilidade();
                  // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
                  // for ($i = 0; $i < count($arAbaLoop); $i++) {
                  //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
                  //         array_push($arAba, $arAbaLoop[$i]);
                  //     }
                  // } */
            break;
        case 'listaorgaodesbloqueio':
            /* /* $arAba[] = array("id" => 1,
                  //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
                  //     "descricao" => "Educao Superior",
                  //     "link" => "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);

                  // $arAba[] = array("id" => 2,
                  //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
                  //     "descricao" => "Educao Profissional",
                  //     "link" => "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);

                  // $arAba[] = array("id" => 3,
                  //     "orgid" => ORGID_EDUCACAO_BASICA,
                  //     "descricao" => "Educao Bsica",
                  //     "link" => "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);

                  // $arAba[] = array("id" => 4,
                  //     "orgid" => ORGID_ADMINISTRATIVO,
                  //     "descricao" => "Administrativo",
                  //     "link" => "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . ORGID_ADMINISTRATIVO);

                  // $arAba[] = array("id" => 5,
                  //     "orgid" => ORGID_HOSPITAIS,
                  //     "descricao" => "Hospitais",
                  //     "link" => "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . ORGID_HOSPITAIS);

                  // $arAbaLoop = $arAba;
                  // $arAba = array();
                  // $userResp = new UsuarioResponsabilidade();
                  // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
                  // for ($i = 0; $i < count($arAbaLoop); $i++) {
                  //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
                  //         array_push($arAba, $arAbaLoop[$i]);
                  //     }
                  // } */
            break;
        case 'listamiorgao':
            // $arAba[] = array("id" => 1,
            //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
            //     "descricao" => "Educao Superior",
            //     "link" => "?modulo=principal/listaObrasMI&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);
            // $arAba[] = array("id" => 2,
            //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
            //     "descricao" => "Educao Profissional",
            //     "link" => "?modulo=principal/listaObrasMI&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);
            // $arAba[] = array("id" => 3,
            //     "orgid" => ORGID_EDUCACAO_BASICA,
            //     "descricao" => "Educao Bsica",
            //     "link" => "?modulo=principal/listaObrasMI&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);
            // $arAba[] = array("id" => 4,
            //     "orgid" => ORGID_ADMINISTRATIVO,
            //     "descricao" => "Administrativo",
            //     "link" => "?modulo=principal/listaObrasMI&acao=A&orgid=" . ORGID_ADMINISTRATIVO);
            // $arAba[] = array("id" => 5,
            //     "orgid" => ORGID_HOSPITAIS,
            //     "descricao" => "Hospitais",
            //     "link" => "?modulo=principal/listaObrasMI&acao=A&orgid=" . ORGID_HOSPITAIS);
            // $arAbaLoop = $arAba;
            // $arAba = array();
            // $userResp = new UsuarioResponsabilidade();
            // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
            // for ($i = 0; $i < count($arAbaLoop); $i++) {
            //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
            //         array_push($arAba, $arAbaLoop[$i]);
            //     }
            // }

            break;
        case 'listamiorgaoemissao':
            // $arAba[] = array("id" => 1,
            //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
            //     "descricao" => "Educao Superior",
            //     "link" => "?modulo=principal/listaObrasMI&acao=O&orgid=" . ORGID_EDUCACAO_SUPERIOR);
            // $arAba[] = array("id" => 2,
            //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
            //     "descricao" => "Educao Profissional",
            //     "link" => "?modulo=principal/listaObrasMI&acao=O&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);
            // $arAba[] = array("id" => 3,
            //     "orgid" => ORGID_EDUCACAO_BASICA,
            //     "descricao" => "Educao Bsica",
            //     "link" => "?modulo=principal/listaObrasMI&acao=O&orgid=" . ORGID_EDUCACAO_BASICA);
            // $arAba[] = array("id" => 4,
            //     "orgid" => ORGID_ADMINISTRATIVO,
            //     "descricao" => "Administrativo",
            //     "link" => "?modulo=principal/listaObrasMI&acao=O&orgid=" . ORGID_ADMINISTRATIVO);
            // $arAba[] = array("id" => 5,
            //     "orgid" => ORGID_HOSPITAIS,
            //     "descricao" => "Hospitais",
            //     "link" => "?modulo=principal/listaObrasMI&acao=O&orgid=" . ORGID_HOSPITAIS);
            // $arAbaLoop = $arAba;
            // $arAba = array();
            // $userResp = new UsuarioResponsabilidade();
            // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
            // for ($i = 0; $i < count($arAbaLoop); $i++) {
            //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
            //         array_push($arAba, $arAbaLoop[$i]);
            //     }
            // }

            break;
        case 'listamiorgaoaceite':
            // $arAba[] = array("id" => 1,
            //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
            //     "descricao" => "Educao Superior",
            //     "link" => "?modulo=principal/listaObrasMI&acao=E&orgid=" . ORGID_EDUCACAO_SUPERIOR);
            // $arAba[] = array("id" => 2,
            //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
            //     "descricao" => "Educao Profissional",
            //     "link" => "?modulo=principal/listaObrasMI&acao=E&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);
            // $arAba[] = array("id" => 3,
            //     "orgid" => ORGID_EDUCACAO_BASICA,
            //     "descricao" => "Educao Bsica",
            //     "link" => "?modulo=principal/listaObrasMI&acao=E&orgid=" . ORGID_EDUCACAO_BASICA);
            // $arAba[] = array("id" => 4,
            //     "orgid" => ORGID_ADMINISTRATIVO,
            //     "descricao" => "Administrativo",
            //     "link" => "?modulo=principal/listaObrasMI&acao=E&orgid=" . ORGID_ADMINISTRATIVO);
            // $arAba[] = array("id" => 5,
            //     "orgid" => ORGID_HOSPITAIS,
            //     "descricao" => "Hospitais",
            //     "link" => "?modulo=principal/listaObrasMI&acao=E&orgid=" . ORGID_HOSPITAIS);
            // $arAbaLoop = $arAba;
            // $arAba = array();
            // $userResp = new UsuarioResponsabilidade();
            // $arOrgid = $userResp->pegaOrgidPermitido($_SESSION['usucpf']);
            // for ($i = 0; $i < count($arAbaLoop); $i++) {
            //     if (in_array($arAbaLoop[$i]['orgid'], $arOrgid)) {
            //         array_push($arAba, $arAbaLoop[$i]);
            //     }
            // }

            break;
        case 'cadastrocontrato':
            $arAba[] = array(
                "id" => 1,
                "descricao" => "Lista de Contratos",
                "link" => "?modulo=principal/listaContrato&acao=A"
            );

            $arAba[] = array(
                "id" => 2,
                "descricao" => "Cadastro de Contrato",
                "link" => "?modulo=principal/cadContrato&acao=A"
            );
            break;
        case 'cadastrocontratoedicao':
            $arAba[] = array(
                "id" => 1,
                "descricao" => "Lista de Contratos",
                "link" => "?modulo=principal/listaContrato&acao=A"
            );

            $arAba[] = array(
                "id" => 2,
                "descricao" => "Cadastro de Contrato",
                "link" => "?modulo=principal/cadContrato&acao=E"
            );

            $arAba[] = array(
                "id" => 3,
                "descricao" => "Histórico de Aditivos",
                "link" => "?modulo=principal/historicoAditivo&acao=A"
            );
            break;
        case 'listademandas':
            // $arAba[] = array("id" => 1,
            //     "orgid" => ORGID_EDUCACAO_SUPERIOR,
            //     "descricao" => "Educao Superior",
            //     "link" => "?modulo=principal/preobra/listaDemandas&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR);
            // $arAba[] = array("id" => 2,
            //     "orgid" => ORGID_EDUCACAO_PROFISSIONAL,
            //     "descricao" => "Educao Profissional",
            //     "link" => "?modulo=principal/preobra/listaDemandas&acao=A&orgid=" . ORGID_EDUCACAO_PROFISSIONAL);
            // $arAba[] = array("id" => 3,
            //     "orgid" => ORGID_EDUCACAO_BASICA,
            //     "descricao" => "Educao Bsica",
            //     "link" => "?modulo=principal/preobra/listaDemandas&acao=A&orgid=" . ORGID_EDUCACAO_BASICA);
            // $arAba[] = array("id" => 4,
            //     "orgid" => ORGID_ADMINISTRATIVO,
            //     "descricao" => "Administrativo",
            //     "link" => "?modulo=principal/preobra/listaDemandas&acao=A&orgid=" . ORGID_ADMINISTRATIVO);
            // $arAba[] = array("id" => 5,
            //     "orgid" => ORGID_HOSPITAIS,
            //     "descricao" => "Hospitais",
            //     "link" => "?modulo=principal/preobra/listaDemandas&acao=A&orgid=" . ORGID_HOSPITAIS);
//          $arAbaLoop = $arAba;
//          $arAba     = array();
//          $userResp  = new UsuarioResponsabilidade();
//          $arOrgid   = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
//          for ($i=0; $i < count( $arAbaLoop ); $i++){
//              if ( in_array( $arAbaLoop[$i]['orgid'], $arOrgid ) ){
//                  array_push($arAba, $arAbaLoop[$i]);
//              }
//          }

            break;
        case 'caddemandas':
            $arAba[] = array(
                "id" => 1,
                "descricao" => "Lista de Demandas",
                "link" => "?modulo=principal/preobra/listaDemandas&acao=A&orgid=" . ORGID_EDUCACAO_SUPERIOR
            );

            $arAba[] = array(
                "id" => 2,
                "descricao" => "Dados da Demanda",
                "link" => "?modulo=principal/preobra/cadDemanda&acao=A&demid=" . $_SESSION['obras2']['demid']
            );
            if ($_SESSION['obras2']['demid']) {
                $arAba[] = array(
                    "id" => 3,
                    "descricao" => "Arquivos da Demanda",
                    "link" => "?modulo=principal/preobra/cadDemandaArquivos&acao=A&demid=" . $_SESSION['obras2']['demid']
                );
            }

            break;
    }

    return $arAba;
}

function atualizarFotosVistoria($supid = null)
{
    global $db;
    if ($_POST['hdn_fotos_galeria'] || $_POST['hdn_fotos_supervisao']) {
        $_POST['hdn_fotos_galeria'] = str_replace(array("s_foto_", "[]="), array("", "_"), $_POST['hdn_fotos_galeria']);
        $_POST['hdn_fotos_supervisao'] = str_replace(
            array("foto_", "[]="),
            array("", "_"),
            $_POST['hdn_fotos_supervisao']
        );
        $_REQUEST['fotosGaleria'] = explode("&", $_POST['hdn_fotos_galeria']);
        $_REQUEST['fotosSupervisao'] = explode("&", $_POST['hdn_fotos_supervisao']);
    }

    $obrid = $_SESSION['obras2']["obrid"];
    $supid = !$supid ? $_REQUEST['supid'] : $supid;

    if ($_REQUEST['fotosSupervisao'][0] && $obrid && $supid) {
        $n = 0;
        foreach ($_REQUEST['fotosSupervisao'] as $fotoSupervisao) {
            $fotoSupervisao = trim($fotoSupervisao);

            if ($fotoSupervisao == '[object Object]') {
                return false;
            }

            if (!is_numeric($fotoSupervisao)) {
                if (file_exists("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $fotoSupervisao)) {
                    $imagem = $fotoSupervisao;
                    $imagem = str_replace("___", "/", $imagem);
                    $part1file = explode("__temp__", $imagem);
                    $part2file = explode("__extension__", $part1file[0]);
                    $part2file[0] = md5_decrypt($part2file[0]);
                    $part2file[1] = md5_decrypt($part2file[1]);
                    $nomearquivo = explode(".", $part2file[0]);

                    if (is_readable("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem . ".d")) {
                        $descricao = file_get_contents("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem . ".d");
                    }

                    $icoid = 'null';
                    if (is_readable("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem . ".e")) {
                        $icoid = file_get_contents("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem . ".e");
                    }

                    //Insere o registro da imagem na tabela public.arquivo
                    $nomearquivo[0] = addslashes($nomearquivo[0]);
                    $sql = "INSERT INTO public.arquivo(arqnome,arqdescricao,arqextensao,arqtipo,arqdata,arqhora,usucpf,sisid)
                     values('" . substr($nomearquivo[0], 0, 255) . "','" . simec_addslashes(substr(
                        $descricao,
                        0,
                        255
                    )) . "','" . addslashes($nomearquivo[(count($nomearquivo) - 1)]) . "','" . $part2file[1] . "','" . date('Y-m-d') . "','" . date('H:i:s') . "','" . $_SESSION["usucpf"] . "',147) RETURNING arqid;";
                    $arqid = $db->pegaUm($sql);

                    if (!is_dir('../../arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000))) {
                        mkdir(APPRAIZ . '/arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000), 0777);
                    }

                    if (file_exists("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem)) {
                        if (@copy(
                            "../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem,
                            "../../arquivos/" . $_SESSION['sisarquivo'] . "/" . floor($arqid / 1000) . "/" . $arqid
                        )) {
                            unlink("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem);
                            $_sql = "INSERT INTO obras2.fotos(arqid,obrid,supid,fotdsc,fotbox,fotordem, icoid)
                         values({$arqid},{$_SESSION['obras2']['obrid']},{$supid},'{$imagem}','imageBox{$n}',{$n}, {$icoid});";
                            $db->executar($_sql);
                        }
                    }

                    $fotoSupervisao = $arqid;
                    $sqlFotos .= "update obras2.fotos set fotordem = $n where arqid = $fotoSupervisao and obrid = $obrid and supid = $supid;";
                    $n++;
                } elseif ($_SESSION['obras2']['copy_sup']) {
                    $arqid = str_replace('foto_', '', $fotoSupervisao);

                    $file = new FilesSimec();
                    $caminho_original = $file->getCaminhoFisicoArquivo($arqid);

                    $arr_dados_arquivo = $db->pegaLinha('SELECT * FROM public.arquivo WHERE arqid = ' . $arqid);

                    $arqnome = $arr_dados_arquivo['arqnome'];
                    $arqextensao = $arr_dados_arquivo['arqextensao'];
                    $arqtipo = $arr_dados_arquivo['arqtipo'];

                    if (is_readable($caminho_original)) {
                        $descricao = file_get_contents($caminho_original);
                    }

                    //Insere o registro da imagem na tabela public.arquivo
                    $sql = "INSERT INTO public.arquivo( arqnome,
                                                        arqdescricao,
                                                        arqextensao,
                                                        arqtipo,
                                                        arqdata,
                                                        arqhora,
                                                        usucpf,
                                                        sisid)
                                                values('" . simec_addslashes(substr($arqnome, 0, 255)) . "',
                                                       '',
                                                       '" . $arqextensao . "',
                                                       '" . $arqtipo . "',
                                                       '" . date('Y-m-d') . "',
                                                       '" . date('H:i:s') . "',
                                                       '" . $_SESSION["usucpf"] . "',
                                                       15) RETURNING arqid;";
                    $arqid = $db->pegaUm($sql);

                    if (!is_dir('../../arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000))) {
                        mkdir(APPRAIZ . '/arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000), 0777);
                    }

                    $caminho_destino = "../../arquivos/" . $_SESSION['sisarquivo'] . "/" . floor($arqid / 1000) . "/" . $arqid;

                    if (@copy($caminho_original, $caminho_destino)) {
                        $sql2 = "INSERT INTO obras2.fotos(arqid,obrid,supid,fotdsc,fotbox,fotordem)
                         values({$arqid},{$_SESSION['obras2']['obrid']},{$supid},'{$fotoSupervisao}','imageBox{$n}',{$n});";
                        $db->executar($sql2);
                    }

                    $fotoSupervisao = $arqid;
                    $sqlFotos .= "update obras2.fotos set fotordem = $n where arqid = $fotoSupervisao and obrid = $obrid and supid = $supid;";
                    $n++;
                }
            } else {
                $sqlFotos .= "update obras2.fotos set fotordem = $n where arqid = $fotoSupervisao and obrid = $obrid and supid = $supid;";
                $n++;
            }
            $arrFotoid[] = $fotoSupervisao;
        }
    }

    if ($arrFotoid) {
        foreach ($arrFotoid as $key => $value) {
            if (!is_numeric($value)) {
                unset($arrFotoid[$key]);
            }
        }
        if (count($arrFotoid) > 0) {
            $sqlFotos .= "update obras2.arquivosobra set aqostatus = 'A'
                         where arqid in (" . implode(",", str_replace(
                "foto_",
                "",
                $arrFotoid
            )) . ") and obrid = $obrid and tpaid = " . TIPO_ARQUIVO_FOTO_VISTORIA . ";";
        }
    }

    if ($_REQUEST['fotosGaleria'][0] && $obrid && $supid) {
//        ver('IF - 4');
        foreach ($_REQUEST['fotosGaleria'] as $fotoGaleria) {
            $fotoGaleria = str_replace("s_foto_", "", $fotoGaleria);

            if (is_numeric($fotoGaleria)) {
                $sqlFotos .= "INSERT INTO obras2.obras_arquivos(
                                obrid,
                                tpaid,
                                arqid,
                                oardata,
                                oardtinclusao
                            ) VALUES (
                                                            $obrid,
                                                            " . TIPO_ARQUIVO_FOTO_VISTORIA . ",
                                                            $fotoGaleria,
                                                            now(),
                                                            now()
                                                            );";

//                 $sqlFotos.= "INSERT INTO
//                         obras2.arquivosobra (
//                                      obrid,
//                                      tpaid,
//                                      arqid,
//                                      usucpf,
//                                      aqodtinclusao,
//                                      aqostatus )
//                             VALUES (
//                                     $obrid,
//                                      ".TIPO_ARQUIVO_FOTO_VISTORIA.",
//                                      $fotoGaleria,
//                                      '{$_SESSION["usucpf"]}',
//                                      'now',
//                                      'A' );";
            }
        }
    }
    if ($sqlFotos) {
        $arr_sqls = explode(';', $sqlFotos);
        foreach ($arr_sqls as $value) {
            if (trim($value) != '') {
                $value = $value . ';';
                $db->executar($value);
            }
        }
    }

    if ($db->commit()) {
        return true;
    } else {
        return false;
    }
}

function atualizarFotosEvolucaoMI($emiid = null)
{
    global $db;
    if ($_POST['hdn_fotos_galeria'] || $_POST['hdn_fotos_supervisao']) {
        $_POST['hdn_fotos_galeria'] = str_replace(array("s_foto_", "[]="), array("", "_"), $_POST['hdn_fotos_galeria']);
        $_POST['hdn_fotos_supervisao'] = str_replace(
            array("foto_", "[]="),
            array("", "_"),
            $_POST['hdn_fotos_supervisao']
        );
        $_REQUEST['fotosGaleria'] = explode("&", $_POST['hdn_fotos_galeria']);
        $_REQUEST['fotosSupervisao'] = explode("&", $_POST['hdn_fotos_supervisao']);
    }

    $obrid = $_SESSION['obras2']["obrid"];
    $emiid = !$emiid ? $_REQUEST['emiid'] : $emiid;

    if (!empty($_REQUEST['arrFotosSupervisao'])) {
        $arrFotosSupervisao = explode('&', $_REQUEST['arrFotosSupervisao']);
        foreach ($arrFotosSupervisao as &$value) {
            $value = str_replace('foto[]=', '', $value);
        }
        $_REQUEST['fotosSupervisao'] = $arrFotosSupervisao;
    }

    if (!empty($_REQUEST['arrFotosGaleria'])) {
        $arrFotosGaleria = explode('&', $_REQUEST['arrFotosGaleria']);
        foreach ($arrFotosGaleria as &$value) {
            $value = str_replace('s_foto[]=', '', $value);
        }
        $_REQUEST['fotosGaleria'] = $arrFotosGaleria;
    }

    if ($_REQUEST['fotosSupervisao'][0] && $obrid && $emiid) {
        $n = 0;
        foreach ($_REQUEST['fotosSupervisao'] as $fotoSupervisao) {
            $fotoSupervisao = trim($fotoSupervisao);
            if (!is_numeric($fotoSupervisao)) {
                if (file_exists("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $fotoSupervisao)) {
                    $imagem = $fotoSupervisao;
                    $imagem = str_replace("___", "/", $imagem);
                    $part1file = explode("__temp__", $imagem);
                    $part2file = explode("__extension__", $part1file[0]);
                    $part2file[0] = md5_decrypt($part2file[0]);
                    $part2file[1] = md5_decrypt($part2file[1]);
                    $nomearquivo = explode(".", $part2file[0]);

                    if (is_readable("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem . ".d")) {
                        $descricao = file_get_contents("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem . ".d");
                    }

                    //Insere o registro da imagem na tabela public.arquivo
                    $nomearquivo[0] = addslashes($nomearquivo[0]);
                    $sql = "INSERT INTO public.arquivo(arqnome,arqdescricao,arqextensao,arqtipo,arqdata,arqhora,usucpf,sisid)
                     values('" . substr($nomearquivo[0], 0, 255) . "','" . substr(
                        $descricao,
                        0,
                        255
                    ) . "','" . $nomearquivo[(count($nomearquivo) - 1)] . "','" . $part2file[1] . "','" . date('Y-m-d') . "','" . date('H:i:s') . "','" . $_SESSION["usucpf"] . "',15) RETURNING arqid;";
                    $arqid = $db->pegaUm($sql);

                    if (!is_dir('../../arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000))) {
                        mkdir(APPRAIZ . '/arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000), 0777);
                    }
                    if (@copy(
                        "../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem,
                        "../../arquivos/" . $_SESSION['sisarquivo'] . "/" . floor($arqid / 1000) . "/" . $arqid
                    )) {
                        unlink("../../arquivos/" . $_SESSION['sisarquivo'] . "/imgs_tmp/" . $imagem);
                        $_sql = "INSERT INTO obras2.fotos(arqid,obrid,emiid,fotdsc,fotbox,fotordem)
                                                   values({$arqid},{$_SESSION['obras2']['obrid']},{$emiid},'{$imagem}','imageBox{$n}',{$n});";
                        $db->executar($_sql);
                    }
                    $fotoSupervisao = $arqid;
                    $sqlFotos .= "update obras2.fotos set fotordem = $n where arqid = $fotoSupervisao and obrid = $obrid and emiid = $emiid;";
                    $n++;
                } elseif ($_SESSION['obras2']['copy_sup']) {
                    $arqid = str_replace('foto_', '', $fotoSupervisao);

                    $file = new FilesSimec();
                    $caminho_original = $file->getCaminhoFisicoArquivo($arqid);

                    $arr_dados_arquivo = $db->pegaLinha('SELECT * FROM public.arquivo WHERE arqid = ' . $arqid);

                    $arqnome = $arr_dados_arquivo['arqnome'];
                    $arqextensao = $arr_dados_arquivo['arqextensao'];
                    $arqtipo = $arr_dados_arquivo['arqtipo'];

                    if (is_readable($caminho_original)) {
                        $descricao = file_get_contents($caminho_original);
                    }

                    //Insere o registro da imagem na tabela public.arquivo
                    $sql = "INSERT INTO public.arquivo( arqnome,
                                                        arqdescricao,
                                                        arqextensao,
                                                        arqtipo,
                                                        arqdata,
                                                        arqhora,
                                                        usucpf,
                                                        sisid)
                                                values('" . substr($arqnome, 0, 255) . "',
                                                       '',
                                                       '" . $arqextensao . "',
                                                       '" . $arqtipo . "',
                                                       '" . date('Y-m-d') . "',
                                                       '" . date('H:i:s') . "',
                                                       '" . $_SESSION["usucpf"] . "',
                                                       15) RETURNING arqid;";
                    $arqid = $db->pegaUm($sql);

                    if (!is_dir('../../arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000))) {
                        mkdir(APPRAIZ . '/arquivos/' . $_SESSION['sisarquivo'] . '/' . floor($arqid / 1000), 0777);
                    }

                    $caminho_destino = "../../arquivos/" . $_SESSION['sisarquivo'] . "/" . floor($arqid / 1000) . "/" . $arqid;

                    if (@copy($caminho_original, $caminho_destino)) {
                        $sql2 = "INSERT INTO obras2.fotos(arqid,obrid,emiid,fotdsc,fotbox,fotordem)
                         values({$arqid},{$_SESSION['obras2']['obrid']},{$emiid},'{$fotoSupervisao}','imageBox{$n}',{$n});";
                        $db->executar($sql2);
                    }

                    $fotoSupervisao = $arqid;
                    $sqlFotos .= "update obras2.fotos set fotordem = $n where arqid = $fotoSupervisao and obrid = $obrid and emiid = $emiid;";
                    $n++;
                }
            } else {
                $sqlFotos .= "update obras2.fotos set fotordem = $n
                             where arqid = $fotoSupervisao
                               and obrid = $obrid
                               and emiid = $emiid;";

                $n++;
            }
            $arrFotoid[] = $fotoSupervisao;
        }
    }

    if ($arrFotoid) {
        $sqlFotos .= " update obras2.arquivosobra
                      set aqostatus = 'A'
                      where arqid in (" . implode(",", str_replace("foto_", "", $arrFotoid)) . ")
                      and obrid = $obrid and tpaid = " . TIPO_ARQUIVO_FOTO_VISTORIA . ";";
    }

    if ($_REQUEST['fotosGaleria'][0] && $obrid && $emiid) {
        foreach ($_REQUEST['fotosGaleria'] as $fotoGaleria) {
            $fotoGaleria = str_replace("s_foto_", "", $fotoGaleria);
            if (is_numeric($fotoGaleria)) {
//                $sqlFotos.= "INSERT INTO obras2.obras_arquivos( obrid, tpaid, arqid, oardata, oardtinclusao)
//                             VALUES ( $obrid, " . TIPO_ARQUIVO_FOTO_VISTORIA . ", $fotoGaleria, now(), now() );";
                $veriArqObr = $db->pegaUm('SELECT * FROM obras2.arquivosobra WHERE arqid = ' . $fotoSupervisao . ' AND obrid = ' . $obrid);

                if (!$veriArqObr) {
                    $sqlFotos .= "INSERT INTO
                             obras2.arquivosobra (
                                          obrid,
                                          tpaid,
                                          arqid,
                                          usucpf,
                                          aqodtinclusao,
                                          aqostatus )
                                 VALUES (
                                         $obrid,
                                          " . TIPO_ARQUIVO_FOTO_VISTORIA . ",
                                          $fotoSupervisao,
                                          '{$_SESSION["usucpf"]}',
                                          'now',
                                          'A' );";
                }
            }
        }
    }

    $msgErro = '';
    if ($sqlFotos) {
        $arr_sqls = explode(';', $sqlFotos);
        foreach ($arr_sqls as $value) {
            if (trim($value) != '') {
                $value = $value . ';';
                try {
                    $db->executar($value);
                } catch (Exception $e) {
                    $msgErro .= ' - ' . $e->getMessage() . '<br>';
                }
            }
        }
    }

    if ($db->commit()) {
        return true;
    } else {
        return false;
    }
}

function bloqueiaMenuObjetoPorSituacao($obrid)
{
    $obra = new Obras($obrid);
    $esdid = pegaEstadoObra($obra->docid);

    $arMenuBlock = array();
    switch ($esdid) {
        case ESDID_OBJ_REPASSE:
            $arMenuBlock = array(
                ID_MENU_LICITACAO,
                ID_MENU_CONTRATACAO,
                ID_MENU_VISTORIA,
                ID_MENU_CRONOGRAMA
            );
            break;
        case ESDID_OBJ_PLANEJAMENTO_PROPONENTE:
            $arMenuBlock = array(
                ID_MENU_LICITACAO,
                ID_MENU_CONTRATACAO,
                ID_MENU_VISTORIA,
                ID_MENU_CRONOGRAMA
            );
            break;
        case ESDID_OBJ_LICITACAO:
            $arMenuBlock = array(
                ID_MENU_CONTRATACAO,
                ID_MENU_VISTORIA,
                ID_MENU_CRONOGRAMA
            );
            break;
        case ESDID_OBJ_CONTRATACAO:
            $contrato = new Contrato();
            $crtid = $contrato->pegaCrtidPorObrid($obrid);

            if ($crtid) {
                $arMenuBlock = array(ID_MENU_VISTORIA);
            } else {
                $arMenuBlock = array(
                    ID_MENU_CRONOGRAMA,
                    ID_MENU_VISTORIA
                );
            }

            break;
    }

    return $arMenuBlock;
}

/*
     * FUNES DO WORKFLOW
     *
     */
/*
     * function pegarDocidSupervisao( $supid )

      //{
      //    global $db;
      //
      //    if (!$supid) return false;
      //
      //    $sql = "SELECT
      //                docid
      //            FROM
      //                obras2.supervisao
      //            WHERE
      //                supid = '" . $supid . "'";
      //    $docid = $db->pegaUm( $sql );
      //
      //    if(!$docid){
      //        /*
      //         * Criei essa constante "_constantes.php", corresponde ao workflow "workflow.tipodocumento" cadastrado pelo Vitor no sistema.
      //         *
      //        $tpdid = TPDID_VISTORIA;
      //
      //        // MONTA NOME DO DOC
      //        $docdsc = sprintf( "Supervisão (%s)", $supid);
      //        // cria documento
      //        $docid = wf_cadastrarDocumento( $tpdid, $docdsc );
      //        $sql = "UPDATE obras2.supervisao SET docid=" . $docid . " WHERE supid = " . $supid;
      //        $db->executar( $sql );
      //        $db->commit();
      //    }
      //    return ($docid);
      //}

     */
/*
     * FIM - FUNES DO WORKFLOW
     *
     */

/**
 * WORKFLOW (Empreendimento - OBRA) - início
 */
function criarDocidEmpreendimento($empid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de empreendimento do mdulo Obras II - empid " . $empid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_EMPREENDIMENTO, $docdsc);

    // atualiza o DOCID no EMPREENDIMENTO
    $empreendimento = new Empreendimento($empid);
    $empreendimento->docid = $docid;
    $empreendimento->salvar();

    $db->commit();

    return $docid;
}

function pegaDocidEmpreendimento($empid)
{
    global $db;

    if (!$empid) {
        return false;
    }

    $empreendimento = new Empreendimento($empid);
    $docid = $empreendimento->docid;
    if (!$empreendimento->docid) {
        $docid = criarDocidEmpreendimento($empid);
    }

    return $docid;
}

function pegaEstadoEmpreendimento($docid)
{
    global $db;

    $docid = ($docid ? $docid : 0);

    $sql = "SELECT
                esdid
            FROM
                workflow.documento d
            WHERE
                docid = {$docid}";

    $esdid = $db->pegaUm($sql);

    return $esdid;
}

/*
     * Estado: Cadastramento
     * Ao:   Enviar para execução
     *
     * ISSO DEVE SER VALIDADO EM TODOS OS OBJETOS DA OBRA
     * 1 - os 3 anexos que falei estiverem cadastrados (pra educao bsica)
     * 2 - o cronograma tiver o valor batendo com o do contrato x objeto (pra qqer orgid)
     */
/*
      //function validaEmpreendimentoEmCadastramentoEnviarParaExecucao( $empid ){
      //
      //    $empreendimento     = new Empreendimento();
      //    $obra               = new Obras();
      //    $obraContrato       = new ObrasContrato();
      //
      //    $msg = '';
      //    if ( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
      //        $arObrid = $obra->pegaIdObraPorEmpid( $empid );
      //        foreach ( $arObrid as $obrid ){
      //            $dados = $obraContrato->listaTodosArqidPorObra( $obrid );
      //            if ( empty($dados['arqidcontrato']) || empty($dados['arqidos']) || empty($dados['arqidcusto']) ){
      //                $msg = 'O contrato em todos objetos da obra devem conter os anexos de: contrato digitalizado, ordem de serviço e a planilha de custo.\n';
      //                break;
      //            }
      //        }
      //    }
      //
      //    if ( $empreendimento->verificaEquivalenciaContratoCronogramaDoObjetoPorEmpid( $empid ) == false ){
      //        $msg .= 'O valor do contrato em todos os objetos da obra devem ser igual ao seus respectivos cronogramas.\n';
      //    }
      //
      //    return ($msg ? $msg : true);
      //}
     */

/**
 * WORKFLOW (Obra - Restrição/Inconformidade)
 */
function condicaoAcaoAceitarJustificativa($rstid)
{
    $restricao = new Restricao($rstid);

    if (($restricao->rstitem == 'I' || $restricao->rstitem == 'R') && $restricao->tprid == 13) {
        return true;
    } else {
        return 'Ao disponivel somente para inconformidades do tipo executiva.';
    }
}


/**
 * WORKFLOW (Empreendimento - OBRA) - FIM
 */

/**
 * WORKFLOW (Obra - OBJETO) - início
 */
function criarDocidObra($obrid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de obra do mdulo Obras II - obrid " . $obrid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_OBJETO, $docdsc);

    // atualiza o DOCID na OBRA
    $obra = new Obras($obrid);
    $obra->docid = $docid;
    $obra->salvar();

    $db->commit();

    return $docid;
}

function pegaDocidObra($obrid)
{
    global $db;

    if (!$obrid) {
        return false;
    }

    $obra = new Obras($obrid);
    $docid = $obra->docid;

    // Bloqueia o cadastro de docid POR ESTA VIA para as obras MI
    if (!$obra->docid) {
        $docid = criarDocidObra($obrid);
    }

    return $docid;
}

function pegaEstadoObra($docid)
{
    global $db;

    $docid = ($docid ? $docid : 0);

    $sql = "SELECT
                esdid
            FROM
                workflow.documento d
            WHERE
                docid = {$docid}";

    $esdid = $db->pegaUm($sql);

    return $esdid;
}

/*
     * Estados: Planejamento pelo proponente, Aguardando anuncia do fornecedor e Aguardando autorizao FNDE
     * Aes:   Enviar para anuncia do fornecedor, Registrar anuncia e enviar para autorizao FNDE e Enviar para contratao
     *
     * 1 - Ser obra do tipo MI.
     */

function wf_testa_obra_mi()
{

    global $db, $docid;

    $arrTpoidMI = array(TPOID_MI_TIPO_B, TPOID_MI_TIPO_C);

    $sql = "SELECT
                        true
                FROM
                        obras2.obras
                WHERE
                        docid = $docid
                        AND tpoid in (" . (implode(', ', $arrTpoidMI)) . ")";

    $teste = $db->pegaUm($sql);

    return $teste == 't';
}

/*
     * Estado: Execução
     * Ao:   Enviar para concluso
     *
     * 1 - A obra deve estar com percentual de concluso acima de 80%
     */

function validaObraDeExecucaoParaConclusao($obrid)
{
    $obra = new Obras($obrid);

    $staid = $obra->pegaUm("SELECT s.staid
                                FROM obras2.supervisao s
                                WHERE
                                    s.obrid = $obrid
                                    AND s.emsid IS NULL
                                    AND s.smiid IS NULL
                                    AND s.supstatus = 'A'::bpchar
                                    AND s.validadapelosupervisorunidade = 'S'::bpchar
                                    AND s.rsuid = 1
                                ORDER BY s.supdata DESC, s.supid DESC LIMIT 1");

    return ($obra->obrpercentultvistoria >= 80 && $staid == 3 ? true : 'Para concluir deve ser adicionada uma vistoria de concluso com percentual superior a 80%');
}

/*
     * Estado: Licitao
     * Ao:   Enviar para contratao
     *
     * 1 - Ter licitao cadastrada e ter cadastro da fase (da licitao) HOMOLOGAO com anexo.
     * 2 - Ter solicitação de desbloqueio deferida e aprovada OU ser obra vinculada
     */

function validaObraDeLicitacaoParaContratacao($obrid)
{

    $faseLicitacao = new FaseLicitacao();
    $faseOk = $faseLicitacao->verificaHomologacaoPorObra($obrid);

//    if (!$faseOk) {
//        return false;
//
//        }
//  return ($faseOk && (verificaSolicitacaoDesbloqueio($obrid) || obraVinculada($obrid)) ? true : 'Para enviar para contratao  necessrio solicitação de desbloqueio de Obra deferida na aba solicitaes');
    
    return $faseOk  ? true : ' necessrio preencher as informações obrigatórias.(Edital, Homologao e declaração de conformidade)';
    
}

/*
     * Estado: Contratao
     * Ao:   Enviar para execução
     *
     * 1 - os 2 anexos que falei estiverem cadastrados (pra educao bsica)
     * 2 - o cronograma tiver o valor batendo com o do contrato x objeto (pra qqer orgid)
     * 3 - Verifica se h obra vinculada
     * 4 - (SS 3885) Verifica se h solicitação de desbloqueio ou se a obra j passou pelo estao de execução
     */

function validaObraDeContratacaoParaExecucao($obrid)
{

    //se houver obra vinculada, libera a obra sem checar as outras regras
    /*if(obraVinculada($obrid)){
            return true;
        }*/

    $obraContrato = new ObrasContrato();
    $itemComposicaoObra = new ItensComposicaoObras();
    $msg = '';
    if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
        $dados = $obraContrato->listaTodosArqidPorObra($obrid);
        if (/* empty($dados['arqidcontrato']) || */
            empty($dados['arqidos']) || empty($dados['arqidcusto'])) {
            $msg .= 'O contrato da obra deve conter os anexos de: ordem de serviço e planilha de custo.\n';
        }
    }

    $ocrvalorexecucao = $obraContrato->getValorContrato($obrid);
    $somaVlr = $itemComposicaoObra->getSomaEtapaByObra($obrid);

    if ($ocrvalorexecucao != $somaVlr || $ocrvalorexecucao <= 0 || $somaVlr <= 0) {
        $msg .= 'O valor do contrato deve ser igual ao do cronograma.\n';
    }

    $obra = new Obras($obrid);
    if ($obra->obrcronogramaservicocontratado != 'S') {
        $msg .= 'A pergunta existente na aba de cronograma deve ser respondida com SIM.\n';
    }

    //verificando se existe uma solicitação de desbloqueio e se a obra j esteve em execução
    /*if ((!verificaSolicitacaoDesbloqueio($obrid) && !obraVinculada($obrid)) && !obraJaEsteveEmExecucao($obrid)) {
        $msg .= 'No h solicitação de desbloqueio aprovada para esta obra.\n';
    }*/

    //verificando qual ser o retorno
    if (!empty($msg)) {
        //se houver alguma mensagem(ou seja alguma regra no foi cumprida) no libera a obra
        return $msg;
    } else {
        //se no houver nenhuma msg libera o workflow
        return true;
    }
}

/*
     * Estado: Celebrao de aditivo
     * Ao:   Enviar para execução
     *
     * 1 - o 1 anexo que falei estiverem cadastrados (pra educao bsica)
     * 2 - o cronograma tiver o valor batendo com o do contrato x objeto (pra qqer orgid)
     */

function validaObraDeAditivoParaExecucao($obrid)
{
    $contrato = new Contrato();
    $crtid = $contrato->pegaCrtidPorObrid($obrid);
    $contrato = new Contrato($crtid);
    $obraContrato = new ObrasContrato();
    $itemComposicaoObra = new ItensComposicaoObras();
    $msg = '';

    if (!$contrato->arqid) {
        $msg .= 'Anexo no enviado.\n';
    }

    $ocrvalorexecucao = $obraContrato->getValorContrato($obrid);
    $somaVlr = $itemComposicaoObra->getSomaEtapaByObra($obrid);

    if ($ocrvalorexecucao != $somaVlr || $ocrvalorexecucao <= 0 || $somaVlr <= 0) {
        $msg .= 'O valor do contrato deve ser igual ao do cronograma.\n';
    }

    return ($msg ? $msg : true);
}

/*
     * Ps Ao
     * Estado: Celebrao de aditivo
     * Ao  : Cancelar solicitação de aditivo
     *
     * Cancela o aditivo da obra
     */

function posCancelaAditivoParaExecucao($obrid)
{
    global $db;

    $obras = new Obras();
    $crtid = $obras->pegaContratoPorObra($obrid);

    if ($crtid) {
        $contrato = new Contrato();
        $contrato->retrocedeContratoInteiro($crtid, $obrid);
    }

    $db->commit();

    return true;
}

/**
 * Estado: Aguardando aceitao de OS
 * @param integer $osmid
 */
function condicaoAcaoAceitaParaExecucao($osmid)
{
    global $db;

    $OrdemServicoMI = new OrdemServicoMI();
    $os = $OrdemServicoMI->carregarPorOsmid($osmid);

    if ($OrdemServicoMI->tomid == '1') {
        // Verifica se a OS possui os anexos necessrios

        $anexoOs = new AnexoOsMi();
        $anexoOs = $anexoOs->getAnexoExecucao($osmid);

        $msg = '';
        if (!$anexoOs['arqid']) {
            $msg .= ' necessrio anexar a Ordem de Serviço assinada.\n';
        }

        $osI = new OrdemServicoMI();
        $osI->carregarPorObridETomid($OrdemServicoMI->obrid, 3);

        if (!$osI->osmid) {
            $msg .= ' necessrio ter uma OS de Implantação concluda.';
        }

        $esd = wf_pegarEstadoAtual($osI->docid);
        if ($esd['esdid'] != ESDID_OS_MI_CONCLUIDA) {
            $msg .= ' necessrio ter uma OS de Implantação concluda.';
        }

        if ($msg) {
            return $msg;
        }
    }
    return true;
}

function posAcaoDeEmCadastramentoParaCancelada($sueid)
{
    $supervisao = new SupervisaoEmpresa($sueid);
    $os = new Supervisao_Os($supervisao->sosid);
    $os->recalculaValorOs();
    return true;
}

/*
     *
     * Estado: Aguardando aceitao de OS
     * Ao  : Enviar para aceite da OS pelo fornecedor
     *
     */

function posAcaoOSDeAceitaParaExecucao($osmid)
{
    global $db;

    $OrdemServicoMI = new OrdemServicoMI();
    $os = $OrdemServicoMI->carregarPorOsmid($osmid);
    $obra = new Obras($OrdemServicoMI->obrid);
    if ($OrdemServicoMI->tomid == '1') {
        // Gera o cronograma para obra mi
        $obra->exportarCronogramaPadraoParaObra();
        // Tramita a obra

        if (wf_acaoPossivel($obra->docid, ESDID_OBJ_EXECUCAO, array('obrid' => $os->obrid))) {
            $estadoOrigem = wf_pegarEstadoAtual($obra->docid);
            $acao = wf_pegarAcao($estadoOrigem['esdid'], ESDID_OBJ_EXECUCAO);
            wf_alterarEstado($obra->docid, $acao['aedid'], '', array('obrid' => $os->obrid));
        } else {
            $sql = "UPDATE workflow.documento SET esdid = " . ESDID_OBJ_EXECUCAO . " WHERE docid = {$obra->docid}";
            $db->executar($sql);
        }
        // Alerta de Mudana de estado p/ o Responsvel pelo prximo estado
        enviaAlertaMudancaEstadoOsMi($osmid);
    }
    return true;
}

/*
     *
     * Estado: Aguardando emisso de OS
     * Ao  : Enviar para aceite da OS pelo fornecedor
     *
     */

function posAcaoOsDeEmissaoParaAceite($osmid)
{
    global $db;
    $OrdemServicoMI = new OrdemServicoMI();
    $os = $OrdemServicoMI->carregarPorOsmid($osmid);
    $obra = new Obras($OrdemServicoMI->obrid);
    if ($OrdemServicoMI->tomid == 1) {
        // Tramita a obra
        tramitaObraOsExecucaoParaAguardandoAceite($obra->obrid);

        // Alerta de Mudana de estado p/ o Responsvel pelo prximo estado
        enviaAlertaMudancaEstadoOsMi($osmid);
    }
    return true;
}

/**
 * Tramita obra quando OS recusada
 * @param type $osmid
 * @return boolean
 */
function tramitaObraQuandoOsRecusada($osmid)
{

    $ordemServico = new OrdemServicoMI();
    $dados = $ordemServico->carregarPorOsmid($osmid);

    $obrid = $dados['obrid'];

    $obra = new Obras($obrid);
    $esdid = pegaEstadoObra($obra->docid);

    if ($esdid == ESDID_OBJ_AGUARDANDO_ACEITE_OS && $dados['tomid'] == 1) {
        wf_alterarEstado($obra->docid, AEDID_OBJ_ACEITE_RECUSADA, '', array());
    }

    enviaAlertaMudancaEstadoOsMi($osmid);
    return true;
}

/**
 * Tramita obra quando criado OS de execução
 * de "Aguardando emisso de OS"
 * para "Aguardando aceite da OS pelo fornecedor"
 */
function tramitaObraOsExecucaoParaAguardandoAceite($obrid)
{
    //$obrid = $_SESSION['obras2']['obrid'];
    if (empty($obrid)) {
        return false;
    }

    $obra = new Obras($obrid);
    $esdid = pegaEstadoObra($obra->docid);
//    ver($esdid);
//    ver(ESDID_OBJ_OS_RECUSADA);
//    die;

    if ($esdid == ESDID_OBJ_AGUARDANDO_EMISSAO_OS) {
        wf_alterarEstado($obra->docid, AEDID_OBJ_AGUARDANDO_ACEITE, '', array());
        return true;
    }

    return false;
}

function posAcaoOSDeExecucaoparaValidacao($osmid)
{
    global $db;
    $OrdemServicoMI = new OrdemServicoMI();
    $os = $OrdemServicoMI->carregarPorOsmid($osmid);

    // Alerta de Mudana de estado p/ o Responsvel pelo prximo estado
    enviaAlertaMudancaEstadoOsMi($osmid);

    /*
          //    if ($os['tomid'] == 3) {
          //        $sql = "UPDATE workflow.documento SET esdid = 908 WHERE docid = {$os['docid']};";
          //        if($db->executar($sql))
          //            return true;
          //        else
          //            return false;
          //    }
         */

    return true;
}

function acaoOsCorrecaoParaConclusao($osmid)
{
    global $db;
    $OrdemServicoMI = new OrdemServicoMI();
    $os = $OrdemServicoMI->carregarPorOsmid($osmid);
    if ($os['tomid'] == 1) {
        $obra = new Obras($os['obrid']);
        $esdid = pegaEstadoObra($obra->docid);
        if ($esdid != ESDID_OBJ_CONCLUIDO) {
            return "A obra deve estar como concluda para concluir a OS.";
        }
    }
    return true;
}

function acaoOsConclusaoParaExecucao($osmid)
{
    global $db;
    $OrdemServicoMI = new OrdemServicoMI();
    $os = $OrdemServicoMI->carregarPorOsmid($osmid);
    if ($os['tomid'] == 1) {
        $obra = new Obras($os['obrid']);
        $esdid = pegaEstadoObra($obra->docid);
        if ($esdid != ESDID_OBJ_CONCLUIDO) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

function validaOsEnviarParaValidacao($osmid)
{
    global $db;
    $OrdemServicoMI = new OrdemServicoMI();
    $os = $OrdemServicoMI->carregarPorOsmid($osmid);

    //Execução
    //Vericar se a obra est concluda e se foi anexado o termo de recebimento e a planilha de medio acumulada.
    if ($os['tomid'] == 1) {
        $anexoOs = new AnexoOsMi();
        $anexoTR = $anexoOs->getAnexoExecucaoTermoRecebimento($os['osmid']);
        $anexoPMA = $anexoOs->getAnexoExecucaoPlanilhaMedicaoAcumulada($os['osmid']);

        $obra = new Obras($os['obrid']);
        $esdid = pegaEstadoObra($obra->docid);
        if ($esdid != ESDID_OBJ_CONCLUIDO) {
            return "A obra deve estar como concluda para enviar a OS para validao.";
        }
        if (empty($anexoTR) && empty($anexoPMA)) {
            return " necessrio anexar o Termo de Recebimento e a Planilha de Medio Acumulada.";
        }
        if (empty($anexoTR)) {
            return " necessrio anexar o Termo de Recebimento.";
        }
        if (empty($anexoPMA)) {
            return " necessrio anexar a Planilha de Medio Acumulada.";
        }
    }
    //Sondagem
    //Verifica o arquivo do laudo de sondagem
    if ($os['tomid'] == 2) {
        $anexoOs = new AnexoOsMi();
        $anexoOs = $anexoOs->getAnexoExecucaoSondagem($os['osmid']);
        if (empty($anexoOs)) {
            return " necessrio anexar o laudo de sondagem";
        }
    }
    //Projeto de Implantação
    //Verifica se foi preenchido e gravado o cronograma(serviços externos) e se tem ao menos um arquivo
    if ($os['tomid'] == 3) {
        $anexoOs = new AnexoOsMi();
        $anexoOs = $anexoOs->getAnexoExecucaoImplantacao($os['osmid']);
        if (empty($anexoOs)) {
            return " necessrio anexar ao menos um anexo do Projeto de Implantação.";
        }
        $obrid = $os['obrid'];
        $sei = $OrdemServicoMI->getArrayServicosExternosImplantacao($obrid);
        if (count($sei) == 0) {
            return " necessrio o preechimento dos Serviços Externos da OS do Projeto de Implantação.";
        }
    }

    return true;
}

function posAcaoMedicaoDeCadastramentoParaHomologacao($smiid)
{
    global $db;

    $supervisaoMi = new SupervisaoMi($smiid);
    $contato = new Contato();
    $obra = new Obras();

    $sql = $contato->ResponsavelObra($supervisaoMi->empid);
    $resp = $db->carregar($sql);
    $obra = $obra->pegaObraPorEmpid($supervisaoMi->empid);

    if (!$resp) {
        return true;
    }

    $dadosRemetentes = array();
    foreach ($resp as $r) {
        $dadosRemetentes[] = $r['usuemail'];
    }

    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'Braslia, DD de mesTextual de YYYY.');


    $dados = array(
        'emlconteudo' => '
                                    <html>
                                        <head>
                                            <title></title>
                                            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                                            <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                                        </head>
                                        <body>
                                            <table style="width: 100%;">
                                                <thead>
                                                    <tr>
                                                        <td style="text-align: center;">
                                                            <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                            <b>MINISTRIO DA EDUCAO</b><br/>
                                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAO - FNDE<br/>
                                                            DIRETORIA DE GESTO, ARTICULAO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                            COORDENAO GERAL DE IMPLEMENTAO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                            SBS Q.2 Bloco F Edifcio FNDE - 70.070-929 - Braslia, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: right; padding: 40px 0 0 0;">
                                                            ' . $data . '
                                                        </td>
                                                    </tr>
                                                </thead>
                                                <tbody>
                                                    <tr>
                                                        <td style="line-height: 15px;">

                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="padding:20px 0 20px 0;">
                                                          Assunto: <b>Medio da obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . '</b>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="line-height: 15px; text-align:justify">
                                                            <p>Prezados Gestor Institucional e  Fiscal de Obra,</p>
                                                            <p>Informamos que foi realizada nesta data a medio da obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . '.  necessrio que o fiscal da obra insira a vistoria relativa a esta medio no SIMEC, no prazo de at 3 dias.</p>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="padding: 10px 0 0 0;">
                                                                Atenciosamente,
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: center; padding: 10px 0 0 0;">
                                                                <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                                                <br />
                                                                <b>Fbio Lcio de Almeida Cardoso<b>
                                                                <br />
                                                                Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                                <br />
                                                                CGIMP/DIRPE/FNDE/MEC
                                                        </td>
                                                    </tr>
                                                </tbody>
                                                <tfoot>

                                                </tfoot>
                                            </table>
                                        </body>
                                    </html>
                                                ',
        'emlassunto' => 'Medio da obra (' . $obra['obrid'] . ') ' . $obra['obrnome'],
        'temid' => 5,
        'emlregistroatividade' => false,
        'obrid' => $obra['obrid']
    );

    $email = new Email();
    $email->popularDadosObjeto($dados);
    $email->salvar($dadosRemetentes);
    $email->enviar();

    return true;
}

/**
 * Pos ao envir obra para execução
 */
function posAcaoContratacaoParaExecucao($obrid)
{
    $email = new Email();
    $email->enviaEmailContratacaoParaExecucao($obrid);

    return true;
}

/**
 * Pos ao enviar obra para paralisao
 * @param $obrid
 * @return bool
 */
function posacaoEnviaParaParallisacao($obrid)
{
    $email = new Email();
    $email->enviaEmalParalisacaoObra($obrid);
    return true;
}

/**
 * WORKFLOW (Obra - OBJETO) - FIM
 */

/**
 * WORKFLOW Obra MI - início
 */
function criarDocidObraMI($obrid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de obra MI do mdulo Obras II - obrid " . $obrid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_OBRAMI, $docdsc);

    // atualiza o DOCID na OBRA
    $obra = new Obras($obrid);
    $obra->docid = $docid;
    $obra->salvar();

    $db->commit();

    return $docid;
}

function pegaDocidObraMI($obrid)
{
    global $db;

    if (!$obrid) {
        return false;
    }

    $obra = new Obras($obrid);
    $docid = $obra->docid;
    if (!$obra->docid) {
        $docid = criarDocidObraMI($obrid);
    }

    return $docid;
}

function pegaEstadoObraMI($docid)
{
    global $db;

    $docid = ($docid ? $docid : 0);

    $sql = "SELECT
                    esdid
            FROM
                    workflow.documento d
            WHERE
                    docid = {$docid}";

    $esdid = $db->pegaUm($sql);

    return $esdid;
}

/**
 * WORKFLOW Obra MI - FIM
 */

/**
 * WORKFLOW (OS) - início
 */
function criarDocidOs($sosid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de OS do mdulo Obras II - sosid " . $sosid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_OS, $docdsc);

    // atualiza o DOCID na OS
    $os = new Supervisao_Os($sosid);
    $os->docid = $docid;
    $os->salvar();

    $db->commit();

    return $docid;
}

function pegaDocidOs($sosid)
{
    global $db;

    if (!$sosid) {
        return false;
    }

    $os = new Supervisao_Os($sosid);
    $docid = $os->docid;

    if (!$docid) {
        $docid = criarDocidOs($sosid);
    }

    return $docid;
}

function pegaEstadoOs($docid)
{
    global $db;

    $docid = ($docid ? $docid : 0);

    $sql = "SELECT
                    esdid
            FROM
                    workflow.documento d
            WHERE
                    docid = {$docid}";

    $esdid = $db->pegaUm($sql);

    return $esdid;
}

/**
 * @param int $docid
 * @return int
 */
function pegaEstadoOsMi($docid)
{
    global $db;

    $docid = ($docid ? $docid : 0);

    $sql = "SELECT
                    esdid
            FROM
                    workflow.documento d
            WHERE
                    docid = {$docid}";

    $esdid = $db->pegaUm($sql);

    return $esdid;
}

function posCadastramentoEmpresa($sosid)
{
    $supGrupoEmpresa = new Supervisao_Grupo_Empresa();
    $entid = $supGrupoEmpresa->pegaEmpresaPorSosid($sosid);

    $usuResp = new UsuarioResponsabilidade();
    $arUsuario = $usuResp->pegaUsuarioPorEntidAndPflcod($entid, PFLCOD_EMPRESA_VISTORIADORA_GESTOR);

    $destinatario = array();
    foreach ($arUsuario as $usuario) {
        $destinatario[] = array(
            'usunome' => $usuario['usunome'],
            'usuemail' => $usuario['usuemail']
        );
    }

    $os = new Supervisao_Os($sosid);
    extract($os->getDados());
    $sgrid = $supGrupoEmpresa->pegaGrupoIdPorSgeid($sgeid);

    $empenho2 = new Supervisao_Empenho($semid);
    $nomeEmpresa = $supGrupoEmpresa->pegaEmpresaPorSgeid($sgeid);

    $html = "<link rel='stylesheet' type='text/css' href='http://simec.mec.gov.br/includes/Estilo.css'/>
             <link rel='stylesheet' type='text/css' href='http://simec.mec.gov.br/includes/listagem.css'/>

             <table class=\"Tabela\" align=\"center\" cellPadding=\"3\">
                <tr>
                    <td class=\"SubTituloDireita\">OS N</td>
                    <td>
                        <b>" . $sosnum . "</b>
                    </td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\">Data Emisso</td>
                    <td>
                        " . formata_data($sosdtemissao) . "
                    </td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\">Nota de Empenho</td>
                    <td id=\"tdEmpenho\">
                        " . $empenho2->semnumempenho . "
                    </td>
                </tr>
                <tr bgcolor=\"#C0C0C0\">
                    <td colspan=\"2\" align=\"center\" >
                        <b>Bloco 1 - Dados do Contrato</b>
                    </td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\">1.1 - Empresa</td>
                    <td id=\"tdComboEmpresa\">
                        " . $nomeEmpresa . "
                    </td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\" valign=\"top\">1.2 - Perodo de Execução do Serviço</td>
                    <td colspan=\"1\">
                        <div>
                            " . formata_data($sosdtinicio) . "
                            <b>at</b>
                            " . formata_data($sosdttermino) . "
                            -
                            " . ($sosemergencial == 'f' ? '30 DIAS' : 'EMERGENCIAL ' . $sosdiasexecucao . ' DIAS') . "
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\">
                        1.3 - Valor Total dos Serviços (R$)
                    </td>
                    <td id=\"tdTotalServico\" colspan=\"1\">
                        " . ($sosvalortotal ? number_format($sosvalortotal, 2, ',', '.') : '-') . "
                    </td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\">
                        1.4 - Serviço
                    </td>
                    <td id=\"tdTotalServico\" colspan=\"3\">
                        <p>Supervisão de aes de infraestrutura , tais como construo, reforma, ampliao e instalao,
                        financiadas com recursos federais,  com emisso de Relatório de Supervisão e Laudo Técnico de Supervisão da
                        Obra, conforme especificaes e condies previstas no Contrato.</p>
                    </td>
                </tr>
                <tr bgcolor=\"#C0C0C0\">
                    <td colspan=\"3\" align=\"center\" >
                        <b>Bloco 2 - Especificao dos Serviços</b>
                    </td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\" colspan=\"3\">
                        <center>2.1 - Obras</center>
                    </td>
                </tr>
                <tr>
                    <td colspan=\"3\">
                        <table class=\"listagem\" width=\"100%\" bgcolor=\"#FFFFFF\" id=\"lista_obra\">";

    $vlrTotalServico = 0;
    if (!empty($sgrid)) {
        $supMesoregiao = new Supervisao_Grupo_Mesoregiao();
        $arMescod = $supMesoregiao->pegaMescodPorSgrid($sgrid);

        $empreendimento = new Empreendimento();
        $param = array(
            'mescod' => $arMescod,
            'orgid' => $orgid
        );
        $arDadoEmp = $empreendimento->listaDados($param);

        if (count($arDadoEmp)) {
            $html .= "
                            <thead>
                                <tr style=\"background-color: #CDCDCD;\">
                                    <th  valign=\"middle\" align=\"center\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        Nome da Obra
                                    </th>
                                    <th  valign=\"middle\" align=\"center\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        UF
                                    </th>
                                    <th  valign=\"middle\" align=\"center\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        Mesorregio
                                    </th>
                                    <th  valign=\"middle\" align=\"center\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        Microrregio
                                    </th>
                                    <th  valign=\"middle\" align=\"center\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        Município
                                    </th>
                                    <th  valign=\"middle\" align=\"center\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        Valor Laudo
                                        <br>
                                        (R$)
                                    </th>
                                </tr>
                            </thead>";

            if ($sgeid) {
                $grupoEmpresa = new Supervisao_Grupo_Empresa();
                $valorUnitario = $grupoEmpresa->pegaValorUnitarioPorSgeid($sgeid);
            }
            $osObra = new Supervisao_Os_Obra();
            $arEmpid = $osObra->listaEmpidPorOs($sosid);
            $i = 0;
            foreach ($arDadoEmp as $dadoEmp) {
                if (in_array($dadoEmp['empid'], $arEmpid)) {
                    $color = ($i % 2 ? '#FFFFFF' : '#E9E9E9');
                    $i++;
                    if (in_array($dadoEmp['empid'], $arEmpid)) {
                        $vlrTotalServico += $valorUnitario;
                    }
                    $html .= "
                            <tbody>
                                <tr style=\"background-color: <?=$color ?>;\">
                                    <td valign=\"middle\" align=\"left\" style=\"padding-left: 5px; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        <label for=\"empid_" . $dadoEmp['empid'] . "\">(" . $dadoEmp['empid'] . ") " . $dadoEmp['empdsc'] . "</label>
                                    </td>
                                    <td  valign=\"middle\" align=\"center\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        " . $dadoEmp['estuf'] . "
                                    </td>
                                    <td  valign=\"middle\" align=\"left\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        " . $dadoEmp['mesdsc'] . "
                                    </td>
                                    <td  valign=\"middle\" align=\"left\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        " . $dadoEmp['micdsc'] . "
                                    </td>
                                    <td  valign=\"middle\" align=\"left\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        " . $dadoEmp['mundescricao'] . "
                                    </td>
                                    <td  valign=\"middle\" align=\"right\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\">
                                        " . ($valorUnitario ? number_format($valorUnitario, 2, ',', '.') : '-') . "
                                    </td>
                                </tr>
                            </tbody>";
                }
            }
        } else {
            $html .= "
                            <tr style=\"color: red;\">
                                <td>
                                    Nenhuma obra encontrada.
                                </td>
                            </tr>";
        }
    }
    $html .= "
                        </table>
                    </td>
                </tr>
            </table>";

    // remetente
    $remetente = array("nome" => "SIMEC - Monitoramento de Obras", "email" => "monitoramento.obras@mec.gov.br");
    $assunto = "SIMEC - ORDEM DE SERVICO - N {$sosnum} - ENVIADA";
    $conteudo = $html;

    $enviado = enviar_email($remetente, $destinatario, $assunto, $conteudo);

//    if ($enviado) {
    return true;
//    } else {
//        return false;
//    }
}

function cadastrarSupervisaoPorOS($sosid)
{
    global $db;
    require_once APPRAIZ . "obras2/classes/modelo/Supervisao_Os.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/Supervisao_Os_Obra.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/SupervisaoEmpresa.class.inc";
    $osObra = new Supervisao_Os_Obra();
    $arrEmpid = $osObra->listaEmpidPorOs($sosid);
    if ($arrEmpid) {
        foreach ($arrEmpid as $empid) {
            $supervisaoEmpresa = new SupervisaoEmpresa();
            // descrição do documento
            $docdsc = "Fluxo de Supervisão Empresa do mdulo Obras II - sueid {$sosid}";
            // cria documento do WORKFLOW
            $docid = wf_cadastrarDocumento(WF_TPDID_LAUDO_SUPERVISAO_EMPRESA, $docdsc);
            $arDado['usucpf'] = $_SESSION['usucpf'];
            $arDado['empid'] = $empid;
            $arDado['docid'] = $docid;
            $arDado['sosid'] = $sosid;
            $arDado['suedtatualizacao'] = date('Y-m-d');
            $supervisaoEmpresa->popularDadosObjeto($arDado);
            $supervisaoEmpresa->salvar();
            $supervisaoEmpresa->commit();
        }

        $email = new Email();
        $email->enviaEmalOsAceita($sosid);

        return true;
    } else {
        return false;
    }
}

/**
 * No deve ser possvel tramitar para homologao as Supervises de Ordens de Serviço que:
 *
 * 1. Preenchimento de todos os campos obrigatórios da aba Dados da Supervisão
 * 2. Tem que ter sido feita o preenchimento do questionrio
 * 3. Nao estiverem com todo o questionrio preenchido (sim, nao, nao se aplica -> quando o nao tiver subquestao no minimo uma tem que ser preenchida)
 * 4. Toda caixa de foto da aba "fotos" tem que ter ao menos uma foto associada
 * 5. Todas as respostas da aba Restrições e Infonformidades tem que estar preenchidas.
 * @return boolean
 */
function verificaPreenchimentoLaudo($sueid)
{
    global $db;

    $msg = array();
    $sue = new SupervisaoEmpresa($sueid);

    $obra = new Obras();
    $obra = $obra->pegaObraPorEmpid($sue->empid);
    $os = new Supervisao_Os($sue->sosid);

    // 1. Preenchimento de todos os campos obrigatórios da aba Dados da Supervisão
    if (!$sue->sosid) {
        return false;
    }
    if (!$sue->entidvistoriador) {
        $msg[] = 'Nome do Responsvel no preenchido.';
    }
    if (!$sue->sueproblema) {
        $msg[] = 'Deve ser informado na aba tramitao se existe problema grave nesta obra.';
    }
    if ($sue->sueproblema == 't' && trim($sue->sueobsproblema) == '') {
        $msg[] = 'Deve ser informado na aba tramitao a observaao do problema grave nesta obra.';
    }
    if (!$sue->suecargovistoriador) {
        $msg[] = 'Cargo do Vistoriador no preenchido.';
    }
    if (!$sue->suedtsupervisao) {
        $msg[] = 'Data da Supervisão no preenchida.';
    }
    if ($os->sosterreno != 't') {
        if (!$sue->suefuncionamento) {
            $msg[] = 'Unidade em Funcionamento no preenchido.';
        }
    }
    if (!$sue->sobid) {
        $msg[] = 'Situação da Obra no preenchida.';
    }
    if (!obraMi($obra['obrid'])) {
        if (!$sue->sueacordo && $sue->sobid == 1) {
            $msg[] = 'Conformidade com o Projeto Bsico aprovado e contrato no preenchido.';
        }
    }

    // 2. Tem que ter sido feita o preenchimento do questionrio
    $sql = "select count(*) as resposta from obras2.questaosupervisao where sueid = $sueid";
    if ($db->pegaUm($sql) < 1) {
        $msg[] = 'O questionrio deve ser preenchido.';
    } else {
        // 3. Nao estiverem com todo o questionrio preenchido (sim, nao, nao se aplica -> quando o nao tiver subquestao no minimo uma tem que ser preenchida)
        $questao = new Questao();
        $subQuestao = new SubQuestao();
        $arquivoRespostaSubQuestao = new ArquivoRespostaSubQuestao();
        $arquivoQuestaoSupervisao = new ArquivoQuestaoSupervisao();
        $obra = new Obras();
        $obra->popularDadosObjeto($obra->pegaObraPorEmpid($_SESSION['obras2']['empid']));
        $obrid = $obra->obrid; //$obra->pegaIdObraPorEmpid($sue->empid);
        $dadosMi = pegaDadosTecnologiaMi($obrid, $sueid);
        $qstescopo = ($dadosMi) ? $dadosMi['qstescopo'] : "SE";


        if ($os->sosterreno == 't') {
            $qstescopo = "QSTER";
        }

        $arFiltro = array(
            'qstescopo' => $qstescopo,
            'orgid' => $_SESSION['obras2']['orgid'],
            'sueid' => ($sueid ? $sueid : 0)
        );

        $arDados = $questao->pegaTodaEstrutura($arFiltro);
        foreach ($arDados as $k => $questao) {
            $arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid($questao['qstid']);
            if (!$questao['qtsresposta']) {
                $msg['qtsresposta'] = 'Todas as questes devem ser respondidas.';
            }

            // Verifica se ao menos uma subquestao esta marcada
            if ($questao['qtsresposta'] == 'f' && count($arDadosSubQuestao) > 0) {
                $checked = false;
                foreach ($arDadosSubQuestao as $dadosSubQuestao) {
                    $arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid(
                        $dadosSubQuestao['sqtid'],
                        ($questao['qtsid'] ? $questao['qtsid'] : 0)
                    );
                    if ($arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A') {
                        $checked = true;
                        break;
                    }
                }
                if (!$checked) {
                    $msg['rsq'] = 'Deve ser selecionado no mnimo uma resposta para as sub questes.';
                }
            }

            // 4. Toda caixa de foto da aba "fotos" tem que ter ao menos uma foto associada
            // Verifica se cada questao possui ao menos uma imagem
            $qstctrlimg = json_decode($questao['qstctrlimagem']);
            switch ($questao['qtsresposta']) {
                case 't':
                    $temImg = $qstctrlimg->S;
                    break;
                case 'f':
                    $temImg = $qstctrlimg->N;
                    break;
                case 'n':
                    $temImg = false;
                    break;
            }

            if ($temImg && $questao['qtsresposta'] != '') {
                $arrArquivosQuesto = $arquivoQuestaoSupervisao->listaPorRespQuestao($questao['qtsid']);

                if (is_array($arrArquivosQuesto) && empty($arrArquivosQuesto)) {
                    $temImgSub = false;
                    foreach ($arDadosSubQuestao as $dadosSubQuestao) {
                        $temImgSub = (($temImgSub || $dadosSubQuestao['sqtimg'] == 't') && $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A') ? true : false;
                    }

                    if (!$temImgSub) {
                        $msg['fotoquestao'] = 'Toda caixa de foto da aba fotos tem que ter ao menos uma foto associada.';
                        break;
                    }
                }
            }

            // Verifica se as subquestoes possuem ao menos uma imagem
            foreach ($arDadosSubQuestao as $dadosSubQuestao) {
                $arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid(
                    $dadosSubQuestao['sqtid'],
                    ($questao['qtsid'] ? $questao['qtsid'] : 0)
                );
                if ($arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A') {
                    $arrArquivosSubQuesto = $arquivoRespostaSubQuestao->listaPorRespQuestao($arResultadosDadosSubQuestao[0]['rsqid']);
                    if (empty($arrArquivosSubQuesto)) {
                        $msg['fotosubquestao'] = 'Toda caixa de foto sub-questo da aba fotos tem que ter ao menos uma foto associada.';
                        break 2;
                    }
                }
            }
        }
    }

    //regra dos 10% e das restriesm "salvarSupervisaoEmpresa"
    if ($sue->empid && strlen($sue->suejustificativa) < 1) {
        $valorp = percentualSupEmpresa($sue->empid);
        $diff = $valorp['percental']['empresa'] - $valorp['percental']['unidade'];
        if ($diff > '10.0' || $diff < '-10.0') {
            $msg['tramitacao'] = 'A justificativa da defasagem deve ser preenchida.';
        }
    }

    /**
     * 1. Para toda e cada questo ou subquesto que vai gerar uma restrição deve ser respondida a pergunta:
     * "Os serviços que foram executados geram algum tipo de risco aos usurios?" com sim ou no
     * 2. Se respondido sim deve ser preenchido o campo texto "Tipo de risco observado"
     * Regra para verificar se as perguntas foram respondidas
     */
    $collectionRestricao = pegaColecaoRestricao($sueid);
    $riscoQuestionarioSupervisao = new RiscoQuestionarioSupervisao();
    $risco = $riscoQuestionarioSupervisao->getBySueid($sueid);

    if (count($collectionRestricao) == count($risco)) {
        $resultado = array();
        foreach ($collectionRestricao as $restricao) {
            foreach ($risco as $resposta) {
                if ($restricao['qstid'] == $resposta['qstid']) {
                    array_push($resultado, array(
                        'qstid' => $restricao['qstid'],
                        'sqtid' => $restricao['sqtid'],
                        'sueriscousuario' => $resposta['sueriscousuario'],
                        'suetiporiscoobs' => $resposta['suetiporiscoobs'],
                        'rstitem' => $restricao['rstitem']
                    ));
                }
            }
        }

        if (count($resultado)) {
            foreach ($resultado as $result) {
                if ($result['sueriscousuario'] == 'S' && empty($result['suetiporiscoobs']) && ($result['rstitem'] == 'R')) {
                    $msg['risco'] = 'A descrição da restrição deve ser preenchida para cada questo na aba tramitao.';
                }
            }
        }
    } else {
        $msg['risco'] = 'A descrição da restrição deve ser preenchida para cada questo e subquesto na aba tramitao.';
    }


    /*
          Regras para obra com vinculao
          Quando a obra  vinculada o campo memoria de calculo  obrigatpiro
          Quando a obra no  vinculada, o percentual total de execução deve ser igual ao valor do cronograma
         */

    $valorp = percentualSupEmpresa($sue->empid);
    $obra = new Obras();
    $obrid = $obra->pegaIdObraPorEmpid($sue->empid);
    $obra->carregarPorId($obrid[0]);
    if (!$obra->obridvinculado) {
        if ($sue->suepercentualexe != $valorp['percental']['empresa']) {
            if ($os->sosterreno != 't' && $obra->obrid != "1014700") {
                $msg['percentual'] = 'O percentual total de execução da obra deve ser igual ao percentual da supervisão (' . number_format(
                    $valorp['percental']['empresa'],
                    2,
                    ',',
                    '.'
                ) . '%).';
            }
        }
    } else {
        if (!$sue->suearqmemcalc) {
//            A pedido do Fbio deixar esta regra opcional em todos os casos
//            $msg['arquivopercet'] = 'A Memria de Clculo do percentual total deve ser preenchido.';
        }
        if (!$sue->suepercentualexe) {
            $msg['percentual'] = 'Total atual do percentual de execução da obra deve ser preenchido.';
        }
    }

    /*
            Verifica preenchimento das repostas acerca das Restrições e Inconformidades.
         */
    #$supervisaoEmpresaRestricao = new SupervisaoEmpresaRestricao();
    #if(!$supervisaoEmpresaRestricao->verificaPreenchimento($obra->obrid, $sueid)){
    #$msg[] = " necessrio responder toda a aba de Restrições e Inconformidades.";
    #}

    return (!empty($msg)) ? implode('\n', $msg) : true;
}

/**
 * Toda mudana de workflow de uma OS MI ser enviado um alerta para o responsvel da prxima ao
 * @param int $osmid
 * @return true
 */
function enviaAlertaMudancaEstadoOsMi($osmid)
{
    $email = new Email();
    $email->enviaEmailTramitacaoOsMi($osmid);
    return true;
}

/**
 * WORKFLOW (OS) - FIM
 */

/**
 * WORKFLOW (SUPERVISO MI) - início
 */
function criarDocidSupervisaoMI($smiid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de Supervisão MI do mdulo Obras II - smiid " . $smiid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_SUPERVISAO_MI, $docdsc);

    // atualiza o DOCID na Supervisao Mi
    $supervisaoMi = new SupervisaoMi($smiid);
    $supervisaoMi->docid = $docid;
    $supervisaoMi->salvar();

    $db->commit();

    return $docid;
}

function pegaDocidSupervisaoMI($smiid)
{
    global $db;

    if (!$smiid) {
        return false;
    }

    $supervisaoMi = new SupervisaoMi($smiid);
    $docid = $supervisaoMi->docid;
    if (!$docid) {
        $docid = criarDocidSupervisaoMI($smiid);
    }

    return $docid;
}

function pegaEstadoSupervisaoMI($docid)
{
    global $db;
    $docid = ($docid ? $docid : 0);
    $sql = "SELECT
                    esdid
            FROM
                    workflow.documento d
            WHERE
                    docid = {$docid}";
    $esdid = $db->pegaUm($sql);
    return $esdid;
}

/**
 *
 * @param type $smiid
 * @return boolean
 * @deprecated desde a mudana para a Evoluo MI
 */
function tramiteSupervisaoMI($smiid)
{
    $supMI = new SupervisaoMi();
    $supMI->posAcaoTramiteSupervisaoMI($smiid);
    return true;
}

function condicaoTramiteValidarSupervisaoMI($smiid)
{
    global $db;
    $supMi = new SupervisaoMi();
    $arr_sup = $supMi->getDadosSupervisaoMi($smiid, array('o.obrid', 'sup.supid', 'sup.supobs'));
    $sql = "SELECT
                    fot.*
            FROM
                    obras2.fotos AS fot
            LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
            WHERE
                    obrid =" . $arr_sup['obrid'] . " AND
                    supid=" . $arr_sup['supid'] . "
            ORDER BY fotordem;";
    $fotos = $db->carregar($sql);
    if (empty($fotos)) {
        return ' necessrio cadastrar as Fotos da Supervisão!';
    }
    if (trim($arr_sup['supobs']) == '') {
        return ' necessrio cadastrar o Relatório Técnico do Acompanhamento!';
    }
    return true;
}

/**
 * WORKFLOW (SUPERVISAO MI) - FIM
 */

/**
 * WORKFLOW (Evoluo MI) - Início
 */
function condicaoTramiteValidarEvolucaoMI($emiid)
{
    require_once APPRAIZ . "obras2/classes/modelo/EvolucaoMi.class.inc";
    $evoMI = new EvolucaoMi();
    $resp = $evoMI->verificaValidacaoEvolucaoMi($emiid);
    if (empty($resp)) {
        return ' necessrio cadastrar os dados da Validao !';
    }
    return true;
}

function tramiteEvolucaoMI($emiid)
{
    require_once APPRAIZ . "obras2/classes/modelo/EvolucaoMi.class.inc";
    $evoMI = new EvolucaoMi();
    $evoMI->posAcaoTramiteEvolucaoMI($emiid);
    return true;
}

/**
 * WORKFLOW (Evoluo MI) - FIM
 */
function posAcaoWfRestricoesInconformidades($rstid)
{
    require_once APPRAIZ . "obras2/classes/modelo/Restricao.class.inc";
    $rI = new Restricao();
    $rI->posAcoesWfRestricoesInconformidades($rstid);
    return true;
}

function condicaoSuperacaoRestricoesInconformidades($rstid)
{
    require_once APPRAIZ . "obras2/classes/modelo/Restricao.class.inc";
    $rI = new Restricao();
    $resp = $rI->condicaoSuperacaoRestricoesInconformidades($rstid);
    ;
    if (!$resp) {
        return ' necessrio cadastrar os dados de Ressalva!';
    }
    return true;
}

function posAcaoWfDiligencia($dlgid)
{

    require_once APPRAIZ . "obras2/classes/modelo/Diligencia.class.inc";
    $dI = new Diligencia();
    $dI->posAcoesWfDiligencia($dlgid);
    return true;
}

function condicaoSuperacaoDiligencia($dlgid)
{
    require_once APPRAIZ . "obras2/classes/modelo/Diligencia.class.inc";
    $dI = new Diligencia();

    $resp = $dI->condicaoSuperacaoDiligencia($dlgid);

    if (!$resp) {
        return ' necessrio cadastrar os dados de Ressalva!';
    }
    return true;
}

function pegaDocidSupervisaoFNDE($sfndeid)
{
    global $db;

    if (!$sfndeid) {
        return false;
    }

    $supervisaoFNDE = new SupervisaoFNDE($sfndeid);
    $docid = $supervisaoFNDE->docid;
    if (!$docid) {
        $docid = criarDocidSupervisaoFNDE($sfndeid);
    }

    return $docid;
}

function criarDocidSupervisaoFNDE($sfndeid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de Supervisão FNDE do mdulo Obras II - smiid " . $sfndeid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_SUPERVISAO_MI, $docdsc);

    // atualiza o DOCID na Supervisao Mi
    $supervisaoMi = new SupervisaoMi($smiid);
    $supervisaoMi->docid = $docid;
    $supervisaoMi->salvar();

    $db->commit();

    return $docid;
}

/**
 * WORKFLOW (ChecklistFNDE) - Início
 */
function posAcaoChecklistFnde($ckfid)
{
    require_once APPRAIZ . "obras2/classes/modelo/Obras.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/Validacao.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/ChecklistFnde.class.inc";

    $chkFnde = new ChecklistFnde($ckfid);
    $tipoChk = (int)$chkFnde->getTipoChecklistFnde($ckfid);

    $dados_estado = $chkFnde->getEstadoChecklist($ckfid);
    $esdid = $dados_estado['esdid'];

    switch ($esdid) {
        case ESDID_CHKLST_CADASTRAMENTO:
            //Preparao para necessidade futura
            break;
        case ESDID_CHKLST_CONCLUIDO:

            // Verifica se o tipo  "Checklist da 2 Parcela"
            if ($tipoChk == QUEID_QUEST_CHKLST_2P) {

                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklist2P($ckfid);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklist2P($ckfid);
                } else {
                    $chkFnde->liberaSegundaParcelaChecklist2pConcluido($ckfid);
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_ADM_SP) {
                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklistAdmSp($ckfid);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklistAdmSp($ckfid);
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_ADM_2015) {
                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklistAdm2015($ckfid, null, null, true);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklistAdm2015($ckfid);

                    foreach ($arrDadosPendencia as $key => $p) {
                        if ($p['tipo_pendencia'] == 'E') {
                            $chkFnde->mandaEmailContratoAVencer($ckfid);
                            unset($arrDadosPendencia[$key]);
                            break;
                        }
                    }
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_ADM) {
                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklistAdm($ckfid);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklistAdm($ckfid);
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_TEC) {
                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklistTec($ckfid);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklistTec($ckfid);
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_TEC_2015) {
                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklistTec2015($ckfid);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklistTec($ckfid);
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_OBR_VINC) {
                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklistObraVinculada($ckfid);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklistObraVinculada($ckfid);
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_OBR_MI) {
                $arrDadosPendencia = $chkFnde->montaArrayDadosPendenciaChecklistObraMi($ckfid);
                if (count($arrDadosPendencia) > 0) {
                    $chkFnde->cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia, $tipoChk);
                    $chkFnde->mandaEmailInconformidadeChecklistObraMI($ckfid);
                }
            } elseif ($tipoChk == QUEID_QUEST_CHKLST_SOLICITACOES) {
                $solicitacao = new Solicitacao();
                $solicitacao->acaoChecklistSolicitacao($ckfid);
            }


            //Faz o registro de atividade quando o checklist for concludo
            $chkFnde->gravaRegistroAtividadeChecklistConcluido($ckfid);
            break;
        case ESDID_CHKLST_CORRECAO:
            //Preparao para necessidade futura
            break;
    }

    $s = new SolicitacaoDesembolso();
    $s->verificaTramiteSolicitacoes($_SESSION['obras2']['obrid']);

    return true;
}

/**
 * WORKFLOW (ChecklistFNDE) - Fim
 */

/**
 * WORKFLOW (Monitoramento Especial) - Início
 */
function posAcaoTarefaMonitoramentoEspecial($itmid)
{
    require_once APPRAIZ . "obras2/classes/modelo/Obras.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/MonitoramentoEspecial.class.inc";

    $objMonitora = new MonitoramentoEspecial();
    $dadosItem = $objMonitora->getDadosTarefas(array('itmid' => $itmid), 'array', 'itm.*, atm.atmid, doc.*, esd.*');
    $dados_estado = $objMonitora->getEstadoTarefa($itmid);
    $esdid = $dados_estado['esdid'];
    $atmid = $dadosItem[0]['atmid'];

    switch ($esdid) {
        case ESDID_ME_ITEM_CADASTRAMENTO:
            //Preparao para necessidade futura
            break;
        case ESDID_ME_ITEM_ANALISE:
            //Preparao para necessidade futura
            break;
        case ESDID_ME_ITEM_CORRECAO:
            //Preparao para necessidade futura
            break;
        case ESDID_ME_ITEM_CONCLUIDO:
            //Preparao para necessidade futura
            $objMonitora->posAcoesTarefaConcluida($itmid, $atmid);
            break;
    }
    return true;
}

function posAcaoAtividadeMonitoramentoEspecial($atmid)
{
    require_once APPRAIZ . "obras2/classes/modelo/Obras.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/MonitoramentoEspecial.class.inc";

    $objMonitora = new MonitoramentoEspecial();
    $dadosAtividade = $objMonitora->getDadosAtividade(array('atmid' => $atmid), 'array', 'atm.*, doc.*, esd.*');

    $dados_estado = $objMonitora->getEstadoAtividade($atmid);
    $esdid = $dados_estado['esdid'];

    switch ($esdid) {
        case ESDID_ME_ATIVIDADE_CADASTRAMENTO:
            //Preparao para necessidade futura
            break;
        case ESDID_ME_ATIVIDADE_ANALISE:
            //Preparao para necessidade futura
            break;
        case ESDID_ME_ATIVIDADE_CORRECAO:
            //Preparao para necessidade futura
            break;
        case ESDID_ME_ATIVIDADE_CONCLUIDO:
            //Preparao para necessidade futura
            $objMonitora->posAcoesAtividadeConcluida($atmid);
            break;
    }
    return true;
}

/**
 * WORKFLOW (Monitoramento Especial) - Fim
 */

/**
 * WORKFLOW (VISTORIA EMPRESA) - início
 */
function criarDocidSupervisaoEmpresa($sueid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de Supervisão Empresa do mdulo Obras II - sueid " . $sueid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(WF_TPDID_LAUDO_SUPERVISAO_EMPRESA, $docdsc);

    // atualiza o DOCID na Supervisao Empresa
    $supervisaoEmpresa = new SupervisaoEmpresa($sueid);
    $supervisaoEmpresa->docid = $docid;
    $supervisaoEmpresa->salvar();

    $db->commit();

    return $docid;
}

function pegaDocidSupervisaoEmpresa($sueid)
{
    global $db;

    if (!$sueid) {
        return false;
    }

    $supervisaoEmpresa = new SupervisaoEmpresa($sueid);
    $docid = $supervisaoEmpresa->docid;
    if (!$docid) {
        $docid = criarDocidSupervisaoEmpresa($sueid);
    }

    return $docid;
}

function pegaEstadoSupervisaoEmpresa($docid)
{
    global $db;

    $docid = ($docid ? $docid : 0);

    $sql = "SELECT
                    esdid
            FROM
                    workflow.documento d
            WHERE
                    docid = {$docid}";

    $esdid = $db->pegaUm($sql);

    return $esdid;
}

function validaRespostaContundenteAndCampoObrigatorio($sueid)
{
    $supervisaoEmpresa = new SupervisaoEmpresa($sueid);

    if (!$supervisaoEmpresa->sosid ||
        !$supervisaoEmpresa->suedtsupervisao ||
        !$supervisaoEmpresa->entidvistoriador ||
        !$supervisaoEmpresa->sobid ||
        !$supervisaoEmpresa->suecargovistoriador ||
        !$supervisaoEmpresa->sueacordo ||
        !$supervisaoEmpresa->suefuncionamento ||
        !$supervisaoEmpresa->sueendcorreto ||
        !$supervisaoEmpresa->endid
    ) {
        $msg = 'Os campos obrigatórios devem ser preenchidos.';
    }

    $msg .= ($msg ? '\n' : '');

    if ($supervisaoEmpresa->sueacordo == 's') {
        $questaoSupervisao = new QuestaoSupervisao();
        $qtdRespContundente = $questaoSupervisao->pegaOcorrenciaRespostaContundentePorSueid($sueid);

        $msg .= ($qtdRespContundente ? 'As respostas do questionrio no permitem marcar a opo SIM na pergunta: Em conformidade com o Projeto Bsico aprovado e contrato?' : '');
    }


//  if
    return ($msg ? $msg : true);
}

function posHomologar($sueid)
{
    global $db;
    $empid = $_SESSION['obras2']['empid'];

    $obra = new Obras();
    $arObrid = $obra->pegaIdObraPorEmpid($empid, array('not(obridpai)' => true));

    $modeloRestricaoQuestionario = new ModeloRestricaoQuestionario();
    $arModeloRestricao = $modeloRestricaoQuestionario->carregaIdRelacaoPorSueid($sueid);
    $restricao = new Restricao();
    $filaRestricao = new FilaRestricao();
    tramitarRestricoesSupervisao($sueid);

    /**
     * Faz um merge entre as questes que foram consideradas
     * risco para usurio e o modelo de restrição do questionrio
     */
    $riscoQuestionarioSupervisao = new RiscoQuestionarioSupervisao();
    $risco = $riscoQuestionarioSupervisao->getBySueid($sueid);
    foreach ($risco as $v) {
        foreach ($arModeloRestricao as $k => $r) {
            if ($v['mrqid'] == $r['mrqid']) {
                $arModeloRestricao[$k] = array_merge($arModeloRestricao[$k], $v);
            }
        }
    }
    //Array resultante
    $arrayRestricao = $arrayFilaRestricao = array();
    foreach ($arObrid as $obrid) {
        foreach ($arModeloRestricao as $modeloRestricao) {
            $modeloRestricaoQuestionario->carregarPorId($modeloRestricao['mrqid']);

            $date = new DateTime(date('Y-m-d'));
            $date->modify('+30 day');

            $tipoRisco = '';
            if (array_key_exists('sueriscousuario', $modeloRestricao)) {
                switch ($modeloRestricao['sueriscousuario']) {
                    case 'S':
                        $tipoRisco = '. Tipo de risco: ' . $modeloRestricao['suetiporiscoobs'];
                        $rstitem = 'R';
                        break;
                    case 'N':
                        $rstitem = 'I';
                        break;
                }
            } else {
                $rstitem = $modeloRestricaoQuestionario->rstitem;
            }

            if ($modeloRestricao['sqtbloqrestricao'] != 't') {
                $arDado = array(
                    'rsqid' => $modeloRestricao['rsqid'],
                    'qtsid' => $modeloRestricao['qtsid'],
                    'tprid' => $modeloRestricaoQuestionario->tprid,
                    'fsrid' => 1,
                    'empid' => null,
                    'obrid' => $obrid,
                    'usucpf' => $_SESSION['usucpf'],
                    'rstdsc' => str_replace(
                        '{observacao}',
                        $modeloRestricao['obs'] . $tipoRisco,
                        $modeloRestricaoQuestionario->mrqtitulo
                    ),
                    'rstdtprevisaoregularizacao' => $date->format('Y-m-d'),
                    'rstdscprovidencia' => $modeloRestricaoQuestionario->mrqdsc,
                    //'rstitem'                    => $modeloRestricaoQuestionario->rstitem
                    'rstitem' => $rstitem
                );

                array_push($arrayRestricao, $arDado);
                if (($restricao->rstid = $restricao->verificaExistenciaRestricao(
                    $arDado['rsqid'],
                    $arDado['qtsid'],
                    $obrid
                ))) {
                    $historico = new HistoricoRestricao();
                    $historico->popularDadosObjeto(
                        array(
                            'rstid' => $restricao->rstid,
                            'fsrid' => 1,
                            'usucpf' => $_SESSION['usucpf'],
                            'rstdsc' => str_replace(
                                '{observacao}',
                                $modeloRestricao['obs'] . $tipoRisco,
                                $modeloRestricaoQuestionario->mrqtitulo
                            ),
                            'rstdtprevisaoregularizacao' => $date->format('Y-m-d'),
                            'rstdscprovidencia' => $modeloRestricaoQuestionario->mrqdsc,
                            'obrid' => $obrid,
                            'rstitem' => $rstitem
                        )
                    )->salvar();
                } else {
                    $restricao->popularDadosObjeto($arDado)
                        ->salvar(true, true, $arCamposNulo);
                }

                $restricao->atualizaDocidNullRetricao($restricao->rstid);

                $restricao->clearDados();
            } else {
                $arDado = array(
                    'rsqid' => $modeloRestricao['rsqid'],
                    'qtsid' => $modeloRestricao['qtsid'],
                    'tprid' => $modeloRestricaoQuestionario->tprid,
                    'fsrid' => 1,
                    'empid' => null,
                    'obrid' => $obrid,
                    'usucpf' => $_SESSION['usucpf'],
                    'frtdsc' => str_replace(
                        '{observacao}',
                        $modeloRestricao['obs'],
                        $modeloRestricaoQuestionario->mrqtitulo
                    ),
                    'frtdtprevisaoregularizacao' => $date->format('Y-m-d'),
                    'frtdscprovidencia' => $modeloRestricaoQuestionario->mrqdsc,
                    'frtitem' => $modeloRestricaoQuestionario->rstitem
                );

                array_push($arrayFilaRestricao, $arDado);
                $filaRestricao->popularDadosObjeto($arDado)
                    ->salvar(true, true, $arCamposNulo);
                $filaRestricao->clearDados();
            }
        }

        if (count($arrayRestricao)) {

            $email = new Email();

            $objObras = new Obras($obrid);
            $situacao = $objObras->getEstadoObraWf();

            if ($situacao['esdid'] != ESDID_OBJ_CANCELADO) {
                $email->enviaEmalRestricaoObra($obrid);
                $email->enviaEmailRestricaoSupervisao($obrid);
            }

            foreach ($arrayRestricao as $restricao) {
                if ($restricao['rstitem'] == 'R') {
                    $email = new Email();
                    $email->enviaEmailRestricoesSupervisao($obrid);
                    break;
                }
            }
        }

        //Registra email para envio ao FNDE: "indicao de mudana de endereo"
        $supervisaoEmpresa = new SupervisaoEmpresa($sueid);

        if ($supervisaoEmpresa->sueendcorreto == 'n') {
            $email = new Email();
            $email->enviaEmailAlteracaoEnderecoFNDE($obrid, $sueid);
        }

        // Atualiza o percentual da obra
        $supervisaoEmpresa = new SupervisaoEmpresa($sueid);
        $empreendimento = new Empreendimento($empid);
        $supervisao = new Supervisao();

        $empreendimento->empdtultvistoriaempresa = $supervisaoEmpresa->suedtsupervisao;
        $supid = $supervisao->pegaSupidByObraAndSueid($obrid, $sueid);
        $empreendimento->emppercentultvistoriaempresa = ($supid) ? $supervisao->pegaPercentSupervisao($supid) : 0;

        if ($supervisaoEmpresa->sueproblema == 't') {
            $email = new Email();
            $email->enviaEmailSupervisaoObraComProblema($obrid, $supervisaoEmpresa->sosid, $supervisaoEmpresa->sueid);
        }
        $empreendimento->salvar();
    }

    $db->commit();

    $obra = $obra->pegaObraPorEmpid($empid, array('not(obridpai)' => true));

    // Dispara um e-mail quando quando a Supervisão tiver a defazagem de 10%
    $valorp = percentualSupEmpresa($empid);
    $diff = $valorp['percental']['empresa'] - $valorp['percental']['unidade'];
    if ($diff > '10.0' || $diff < '-10.0') {
        $dados['fsrid'] = 1;
        //fsrid = 1 --  execução;

        $dados['rstitem'] = "R";
        $dados['rstdtprevisaoregularizacao'] = date("d/m/Y", mktime(0, 0, 0, date("m"), date("d") + 15, date("Y")));
        $dados['rstdsc'] = "Em decorrncia do monitoramento realizado por meio do Sistema Integrado de Monitoramento,
                                            Execução e Controle do Ministrio da Educao (Simec) e de Supervisão realizada por empresa contratada
                                            pelo FNDE, verificamos que existe uma grande divergncia entre o percentual de execução da obra
                                            informado em vistoria pelo fiscal do estado responsvel pela obra (" . number_format(
            $valorp['percental']['unidade'],
            2,
            ',',
            '.'
        ) . "%) e o da empresa de Supervisão
                                            (" . number_format($valorp['percental']['empresa'], 2, ',', '.') . "%).";

        $dados['rstdscprovidencia'] = "1. Solicitamos o fiscal reveja os percentuais informados dos serviços executados,
                                            corrigindo-os se for o caso, ou que insira nova vistoria, com fotos atualizadas da obra, que comprovem o
                                            percentual de execução informado.

                                            2. Solicitamos, no prazo mximo de 15 dias, que o cumprimento das providências requeridas seja
                                            informado a esta Autarquia, por meio do Sistema Integrado de Monitoramento, Execução e Controle do
                                            Ministrio da Educao (Simec). O no atendimento das providências solicitadas causar a suspenso do
                                            repasse de recursos dessa obra at a sua resoluo.";

        $dados['tprid'] = 17;
        $_GET['acao'] = 'X';

        $restricao = new Restricao();
        $restricao->salvaRestricaoInconformidade($dados, $obra['obrid']);
        $restricao->atualizaDocidNullRetricao();
        $db->commit();

        $email = new Email();
        $email->enviaEmailDefasagemSuperviso($obra['obrid']);
    }

    $supervisaoEmpresa->verificaDivergenciaSituacao($obra['obrid']);

    extract($supervisaoEmpresa->getDados());

    tramitarOsParaConcluida($sosid);

    return true;
}

/**
 * @demand #317655
 *
 * @description
 * Ps-homologar. Envia todas as restrições respondidas com S ou SR para superadas.
 */
function tramitarRestricoesSupervisao($sueid)
{
    $supervisaoER = new SupervisaoEmpresaRestricao();
    $documentos = $supervisaoER->buscarDocumentosRestricoesPosHomologar($sueid);
    $empresa = $supervisaoER->buscarDadosPosHomologacaoEmpresa($sueid);
    if (is_array($documentos)) {
        foreach ($documentos as $documento) {
            $aedid = wf_pegarAcao($documento['esdid'], ESDID_JUSTIFICADA);
            wf_alterarEstado(
                $documento['docid'],
                $aedid['aedid'],
                'Justificada atravás da supervisão feita pela empresa ' . $empresa . ' e homologado por ' . $_SESSION['usunome'] . '.',
                array(),
                array()
            );
        }
    }
}

/**
 * @demand #224921
 *
 * @description
 * Quando todas as supervises de uma Ordem de serviço forem homologadas o sistema precisa tramitar a Ordem de Serviço
 * para a Situação de concluda.
 * Fazer isso como parte da ps-ao de homologao da supervisão
 */
function tramitarOsParaConcluida($sosid)
{

    global $db;
    // Busca os quantitativos de supervises homologadas
    $sql = "
        SELECT sh.sosid_s,
               sh.qtd_supervisoes_homologadas_por_os,
               nh.qtd_supervisoes_nao_homologadas_por_os
        FROM (
                SELECT se.sosid as sosid_s, count(se.sosid) as qtd_supervisoes_homologadas_por_os
                FROM obras2.supervisaoempresa se
                JOIN workflow.documento       wdc ON wdc.docid = se.docid
                JOIN workflow.estadodocumento wed ON wed.esdid = wdc.esdid
                JOIN obras2.supervisao_os     os  ON os.sosid  = se.sosid AND os.sosstatus = 'A'
                WHERE
                    se.suestatus = 'A'
                 AND wed.esdid = 734 -- Homologado
                group by se.sosid
              ) AS sh
        INNER JOIN (
                SELECT se.sosid as sosid_n, count(se.sosid) as qtd_supervisoes_nao_homologadas_por_os
                FROM obras2.supervisaoempresa se
                JOIN workflow.documento       wdc ON wdc.docid = se.docid
                JOIN workflow.estadodocumento wed ON wed.esdid = wdc.esdid
                JOIN obras2.supervisao_os     os  ON os.sosid  = se.sosid AND os.sosstatus = 'A'
                WHERE
                    se.suestatus = 'A'
                 AND wed.esdid != 734 -- !Homologado
                group by se.sosid
             ) AS nh ON sh.sosid_s = nh.sosid_n
        WHERE sh.sosid_s = " . $sosid . " AND nh.sosid_n = " . $sosid . " ";

    $dados_supervisao_os_obra = $db->pegaLinha($sql);

    // Verifica se a qtd de supervises no homologadas  zero e se a qtd de supervises homologadas  maior que zero
    // Se atender a condio, executa a tramitao da OS, seno, no faz nada.
    if ($dados_supervisao_os_obra != false &&
        $dados_supervisao_os_obra['qtd_supervisoes_nao_homologadas_por_os'] == 0 &&
        $dados_supervisao_os_obra['qtd_supervisoes_homologadas_por_os'] > 0) {
        //Recupera o docid da OS para fazer a tramitao
        $sql_os_docid = " select docid
                          from obras2.supervisao_os os
                          where sosid = " . $sosid . " ";

        $docid = $db->pegaUm($sql_os_docid);
        $aedid = AEDID_OS_CONCLUIDA;
        $cmddsc = 'Atualizao para OS concluida, feita de forma automtica aps verificar que todas as supervises foram homologadas.';

        include_once 'workflow.php';

        try {
            wf_alterarEstado($docid, $aedid, $cmddsc, array('sosid' => $sosid));
        } catch (Exception $ex) {
            die("<script>
                        alert('Houve um erro ao Tramitar, de forma automtica, a OS para concluda! Entre em contato com a equipe do SIMEC.');
                 </script>");
        }
    }
}

/**
 * WORKFLOW (VISTORIA EMPRESA) - FIM
 */
/*
     * SIGARP - Início
     */
function validaSituacaoSigarp($preid, $esdid)
{
}

/*
     * SIGARP - Fim
     */

function pegaUsuarioWorkflowExecutor($docid)
{
    global $db;

    $sql = "select
                    usu.usucpf,
                    usu.usunome,
                    to_char(htddata,'DD/MM/YYYY') as data
            from
                    workflow.historicodocumento his
            inner join
                    seguranca.usuario usu ON usu.usucpf = his.usucpf
            where
                    docid = $docid
            and
                    aedid = " . AEDID_WF_EXECUCAO;
    return $db->pegaLinha($sql);
}

function pegaUsuarioWorkflowExecutorAcao($docid, $acao)
{
    global $db;

    $sql = "select
                    usu.usucpf,
                    usu.usunome,
                    to_char(htddata,'DD/MM/YYYY') as data
            from
                    workflow.historicodocumento his
            inner join
                    seguranca.usuario usu ON usu.usucpf = his.usucpf
            where
                    docid = $docid
            and
                    aedid = $acao";
    return $db->pegaLinha($sql);
}

function mascaraglobal($value, $mask)
{
    $casasdec = explode(",", $mask);
    // Se possui casas decimais
    if ($casasdec[1]) {
        $value = sprintf("%01." . strlen($casasdec[1]) . "f", $value);
    }

    $value = str_replace(array("."), array(""), $value);
    if (strlen($mask) > 0) {
        $masklen = -1;
        $valuelen = -1;
        while ($masklen >= -strlen($mask)) {
            if (-strlen($value) <= $valuelen) {
                if (substr($mask, $masklen, 1) == "#") {
                    $valueformatado = trim(substr($value, $valuelen, 1)) . $valueformatado;
                    $valuelen--;
                } else {
                    if (trim(substr($value, $valuelen, 1)) != "") {
                        $valueformatado = trim(substr($mask, $masklen, 1)) . $valueformatado;
                    }
                }
            }
            $masklen--;
        }
    }
    return $valueformatado;
}

function validaCPFProfissionalEmrpesa()
{
    global $db;

    $tecnico = new Tecnico_Empresa();
    $cpf = $_POST['temcpf'];
    $existe = $tecnico->pegaPorCpf($cpf);

    if ($existe) {
        echo "ok";
    } else {
        echo "naoexiste";
    }
}

function criaAbaVisualizacaoObra()
{
    $menu = array(
        0 => array(
            "id" => 1,
            "descricao" => "Dados da Obra",
            "link" => "/obras2/obras2.php?modulo=principal/cadObra&acao=A&visualizar=1&obrid=" . $_GET['obrid']
        ),
        1 => array(
            "id" => 2,
            "descricao" => "Licitao",
            "link" => "/obras2/obras2.php?modulo=principal/exibeLicitacao&acao=A&visualizar=1"
        ),
        2 => array(
            "id" => 3,
            "descricao" => "Contratao",
            "link" => "/obras2/obras2.php?modulo=principal/exibeContrato&acao=A&visualizar=1"
        ),
        3 => array(
            "id" => 4,
            "descricao" => "Cronograma",
            "link" => "/obras2/obras2.php?modulo=principal/etapas_da_obra&acao=A&visualizar=1"
        ),
        4 => array(
            "id" => 5,
            "descricao" => "Vistorias",
            "link" => "/obras2/obras2.php?modulo=principal/vistoria&acao=A&visualizar=1"
        ),
        5 => array(
            "id" => 6,
            "descricao" => "Recursos",
            "link" => "/obras2/obras2.php?modulo=principal/cadObraRecursos&acao=A&visualizar=1"
        ),
        6 => array(
            "id" => 7,
            "descricao" => "Documentos",
            "link" => "/obras2/obras2.php?modulo=principal/cadObraDocumentos&acao=A&visualizar=1"
        ),
        7 => array(
            "id" => 8,
            "descricao" => "Restrições e Providências",
            "link" => "/obras2/obras2.php?modulo=principal/listaRestricao&acao=A&visualizar=1"
        ),
        8 => array(
            "id" => 9,
            "descricao" => "Execução Orçamentária",
            "link" => "/obras2/obras2.php?modulo=principal/listaExecOrcamentaria&acao=A&visualizar=1"
        ),
        9 => array(
            "id" => 10,
            "descricao" => "Documentos Pr-Obras",
            "link" => "/obras2/obras2.php?modulo=principal/listaExecOrcamentaria&acao=A&visualizar=1"
        ),
    );
    $abaAtiva = $_SERVER['REQUEST_URI'];
    echo montarAbasArray($menu, $abaAtiva);
}

function criaComboWorkflow($tpdid, $arrParametros = array(), $multiplo = false)
{
    global $db;
    $arrParametros['nome'] = $arrParametros['nome'] ? $arrParametros['nome'] : "esdid";
    $arrParametros['habilitado'] = $arrParametros['habilitado'] ? $arrParametros['habilitado'] : "S";
    $arrParametros['obrigatorio'] = $arrParametros['obrigatorio'] ? $arrParametros['habilitado'] : "N";
    $arrParametros['id'] = $arrParametros['id'] ? $arrParametros['id'] : "esdid";
    $arrParametros['valor'] = $arrParametros['valor'] ? $arrParametros['valor'] : $_REQUEST['tpdid'];
    $arrParametros['onchange'] = $arrParametros['onchange'] ? $arrParametros['onchange'] : "";

    $sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = $tpdid order by esdordem";
    $arrDados = $db->carregar($sql);
    $arrDados = $arrDados ? $arrDados : array();
    if (is_array($arrParametros['option'])) {
        foreach ($arrParametros['option'] as $chave => $valor) {
            $arrDados[] = array("codigo" => $chave, "descricao" => $valor);
        }
    }
    if ($multiplo) {
        $db->monta_combo_multiplo(
            $arrParametros['nome'],
            $arrDados,
            $arrParametros['habilitado'],
            "",
            $arrParametros['onchange'],
            '',
            '',
            '',
            $arrParametros['obrigatorio'],
            $arrParametros['id'],
            '',
            $arrParametros['valor']
        );
    } else {
        $db->monta_combo(
            $arrParametros['nome'],
            $arrDados,
            $arrParametros['habilitado'],
            "Selecione...",
            $arrParametros['onchange'],
            '',
            '',
            '',
            $arrParametros['obrigatorio'],
            $arrParametros['id'],
            '',
            $arrParametros['valor']
        );
    }
}

function criaAbaOS()
{
    $menu = array(
        0 => array(
            "id" => 1,
            "descricao" => "Lista de OS",
            "link" => "/obras2/obras2.php?modulo=principal/listOs&acao=A"
        ),
        1 => array(
            "id" => 2,
            "descricao" => "Cadastro de OS",
            "link" => "/obras2/obras2.php?modulo=principal/cadOs&acao={$_GET['acao']}" . ($_GET['sosid'] ? "&sosid=" . $_GET['sosid'] : "")
        ),
    );
    $abaAtiva = $_SERVER['REQUEST_URI'];
    echo montarAbasArray($menu, $abaAtiva);
}

function salvarDadosSupervisao()
{

    global $db;

    $empid = $_SESSION['obras2']['empid'];
    $sueid = $_SESSION['obras2']['sueid'];

    $supervisaoEmpresa = new SupervisaoEmpresa($sueid);

    $arDado = $_POST;
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['empid'] = $empid;
    $arDado['suedtatualizacao'] = date('Y/m/d');
    $arDado['suedtsupervisao'] = formata_data_sql($arDado['suedtsupervisao']);


    $arquivo = $_FILES["arquivo"];
    if ($_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file = new FilesSimec(null, null, "obras2");
        $file->setPasta('obras2');
        $file->setUpload(null, 'arquivo', false);
        $arqid = $file->getIdArquivo();
        $arDado['suearqmemcalc'] = $arqid;
    }

    $arDado['suepercentualexe'] = str_replace(',', '.', $arDado['suepercentualexe']);

    $arCamposNulo = array();
    if (empty($arDado['sobid'])) {
        $arDado['sobid'] = null;
        $arCamposNulo[] = 'sobid';
    }
    if (empty($arDado['sueacordo'])) {
        $arDado['sueacordo'] = null;
        $arCamposNulo[] = 'sueacordo';
    }
    if (empty($arDado['sosid'])) {
        $arDado['sosid'] = null;
        $arCamposNulo[] = 'sosid';
    }
    if (empty($arDado['entidvistoriador'])) {
        $arDado['entidvistoriador'] = null;
        $arCamposNulo[] = 'entidvistoriador';
    }
    if (empty($arDado['suedtsupervisao'])) {
        $arDado['suedtsupervisao'] = null;
        $arCamposNulo[] = 'suedtsupervisao';
    }
    if (empty($arDado['tplid'])) {
        $arDado['tplid'] = null;
        $arCamposNulo[] = 'tplid';
    }

    if (!isset($_POST['sueobsretificacaoos']) || empty($arDado['sueobsretificacaoos'])) {
        $arDado['sueobsretificacaoos'] = null;
        $arCamposNulo[] = 'sueobsretificacaoos';
    }

    foreach ($arDado as $key => $value) {
        if (empty($value) || $value == 'null' || $value == null) {
            $arDado[$key] = null;
            array_push($arCamposNulo, $key);
        }
    }

//ver($arDado,d);
    $sueid = $supervisaoEmpresa->popularDadosObjeto($arDado)
        ->salvar(true, true, $arCamposNulo);
    $_SESSION['obras2']['sueid'] = $sueid;

    /* /*   $supervisao = new Supervisao();
          //    $percentExec = 0;
          //    $_POST['obrid'] = $_POST['obrid'] ? $_POST['obrid'] : array();
          //    foreach ( $_POST['obrid'] as $obrid ){
          //        $numObr++;
          //        if ( !empty( $_POST['supid'][$obrid] ) ){
          //            $supervisao->carregarPorId( $_POST['supid'][$obrid] );
          //        }
          //        $supervisao->sueid = $sueid;
          //        $supervisao->obrid = $obrid;
          //        $supid = $supervisao->salvar();
          //        $supervisao->clearDados();
          //
          //        $percentExec += $supervisao->percentExec;
          //    } */
}

function salvarLocalObra()
{

    global $db;

    $empid = $_SESSION['obras2']['empid'];
    $sueid = $_SESSION['obras2']['sueid'];

    if ($_POST['sueendcorreto'] == 'n') {
        $endid = ($_POST['endid'] != $_POST['emp_endid'] ? $_POST['endid'] : '');
        $endereco = new Endereco($endid);
        $dadosEnd = $_POST['endereco'];
        $dadosEnd['endcep'] = str_replace(array('.', '-'), '', $dadosEnd['endcep']);
        $endid = $endereco->popularDadosObjeto($dadosEnd)
            ->salvar();
    } else {
        $endid = $_POST['emp_endid'];
    }
    $supervisaoEmpresa = new SupervisaoEmpresa($sueid);

    $arDado['endid'] = $endid;
    $arDado['sueendcorreto'] = $_POST['sueendcorreto'];

    $sueid = $supervisaoEmpresa->popularDadosObjeto($arDado)
        ->salvar(true, true);
    $_SESSION['obras2']['sueid'] = $sueid;
//
//  $supervisao = new Supervisao();
//  $percentExec = 0;
//  $_POST['obrid'] = $_POST['obrid'] ? $_POST['obrid'] : array();
//  foreach ( $_POST['obrid'] as $obrid ){
//      $numObr++;
//      if ( !empty( $_POST['supid'][$obrid] ) ){
//          $supervisao->carregarPorId( $_POST['supid'][$obrid] );
//      }
//      $supervisao->sueid = $sueid;
//      $supervisao->obrid = $obrid;
//      $supid = $supervisao->salvar();
//      $supervisao->clearDados();
//
//      $percentExec += $supervisao->percentExec;
//  }
//
//  $empreendimento = new Empreendimento( $empid );
//  $empreendimento->empdtultvistoriaempresa = date('Y-m-d');
//
//  $ultSueid = $supervisaoEmpresa->pegaUltSueidByEmpreendimento( $empid );
//  if ( $sueid == $ultSueid ){
//      $empreendimento->emppercentultvistoriaempresa = ($percentExec > 0 ? $percentExec / $numObr : 0);
//  }
//  $empreendimento->salvar();
}

function salvarCronograma()
{
    $empid = $_SESSION['obras2']['empid'];
    $sueid = $_SESSION['obras2']['sueid'];
    $cronograma = new Cronograma();

    $supervisaoEmpresa = new SupervisaoEmpresa($sueid);

    $supervisao = new Supervisao();
    $percentExec = 0;
    $_POST['obrid'] = $_POST['obrid'] ? $_POST['obrid'] : array();
    foreach ($_POST['obrid'] as $obrid) {
        $numObr++;
        if (!empty($_POST['supid'][$obrid])) {
            $supervisao->carregarPorId($_POST['supid'][$obrid]);
        }
        $supervisao->sueid = $sueid;
        $supervisao->obrid = $obrid;
        $supervisao->croid = $cronograma->getIdCronogramaObra($obrid);
        $supid = $supervisao->salvar();
        $supervisao->clearDados();

        $percentExec += $supervisao->percentExec;
    }

//    $empreendimento = new Empreendimento($empid);
//    $empreendimento->empdtultvistoriaempresa = date('Y-m-d');
//    $ultSueid = $supervisaoEmpresa->pegaUltSueidByEmpreendimento($empid);
//    if ($sueid == $ultSueid) {
//        $empreendimento->emppercentultvistoriaempresa = ($percentExec > 0 ? $percentExec / $numObr : 0);
//    }
//    $empreendimento->salvar();
}

/**
 * Tira a diferenca entre entre as subquestoes marcadas pelo usuario e as questoes
 * do banco com o proposito de initivar as respontas das subquestoes desmarcadas
 *
 * @param array $arrSqtid Array contendo todas as subquestoes marcadas pelo usuario
 * @param type $arrRsqid Array contendo todas as subquestoes do banco
 */
/*
      //function clearSubPerguntas($arrSqtid, $arrRsqid, $qtsid) {
      //    $respostaSubquestao = new RespostaSubquestao();
      //    foreach ($arrRsqid as $qstid => $sqt) {
      //        foreach ($sqt as $sqtid => $rsqid) {
      //            if (!empty($rsqid)) {
      //                if(!is_array($arrSqtid[$qstid])) {
      //                        $respostaSubquestao->apagaRespostaSubquestao($qtsid, $sqtid);
      //                } else {
      //                    if (array_search($sqtid, $arrSqtid[$qstid]) === false)
      //                        $respostaSubquestao->apagaRespostaSubquestao($qtsid, $sqtid);
      //                }
      //            }
      //        }
      //    }
      //}
     */
function salvarQuestionario()
{

    global $db;

    $empid = $_SESSION['obras2']['empid'];
    $obrid = $_SESSION['obras2']['obrid'];
    $sueid = $_SESSION['obras2']['sueid'];
//    $rsqidSalvos = array();
    // QUESTIONRIO
    if (is_array($_POST['qstid'])) {
        $questaoSupervisao = new QuestaoSupervisao();
        $respostaSubquestao = new RespostaSubquestao();
        $modeloRestricaoQuestionario = new ModeloRestricaoQuestionario();
        $restricao = new Restricao();
        $arqQuestaoSupervisao = new ArquivoQuestaoSupervisao();

        // resgata textarea
        $_POST = conversaoArrayPhpParaSupervisao($_POST);

        foreach ($_POST['qstid'] as $qstid => $resp) {
//            $questaoSupervisao->apagaRespostaPorQuestao( $qstid );
            if ($_POST["qtsid_{$qstid}"]) {
                $questaoSupervisao->carregarPorId($_POST["qtsid_{$qstid}"]);
                if ($questaoSupervisao->qtsid && trim($questaoSupervisao->qtsresposta) != trim($resp)) {
                    $arqQuestaoSupervisao->apagaPorQtsid($questaoSupervisao->qtsid);
                }
            }

            // Tratamento para no criar um outro registro antes de verificar se ja existe um ativo no banco
            if (!$arqQuestaoSupervisao->qtsid) {
                $arDados = $questaoSupervisao->pegaRespostaPorTipoSueid($qstid, $sueid);
                $questaoSupervisao->popularDadosObjeto($arDados);
                $_POST["qtsid_{$qstid}"] = $questaoSupervisao->qtsid;
            }

            $arDados = array(
                'qstid' => $qstid,
                'sueid' => $sueid,
                'qtsresposta' => $resp,
                'qtsobs' => $_POST['qtsobs'][$qstid]
            );
            $qtsid = $questaoSupervisao->popularDadosObjeto($arDados)->salvar();

            $questaoSupervisao->clearDados();

            $arRsqidAtivo = array();
            if (is_array($_POST['sqtid'][$qstid]) && $resp == 'f') {
                foreach ($_POST['sqtid'][$qstid] as $sqtid) {
//            if ($rsqobs != '') {
                    if ($_POST['rsqid'][$qstid][$sqtid] != '') {
                        $respostaSubquestao->carregarPorId($_POST['rsqid'][$qstid][$sqtid]);
//                        $rsqidSalvos[] = $_POST['rsqid'][$qstid][$sqtid];
                    }
                    $rsqobs = $_POST['rsqobs'][$qstid][$sqtid];
                    $arDados = array(
                        'qtsid' => $qtsid,
                        'sqtid' => $sqtid,
                        'rsqobs' => $rsqobs
                    );

                    $rsqid = $respostaSubquestao->popularDadosObjeto($arDados)->salvar();
                    $respostaSubquestao->clearDados();
                    $arRsqidAtivo[] = $rsqid;
//                    }
                }
            } elseif (is_array($_POST['sqtid'][$qstid])) {
                // Apaga as respostas das subquestes que a questo mudou a resposta de 'f' para outro
                foreach ($_POST['sqtid'][$qstid] as $sqtid) {
                    if ($_POST['rsqid'][$qstid][$sqtid] != '') {
                        $respostaSubquestao->carregarPorId($_POST['rsqid'][$qstid][$sqtid]);
                        $respostaSubquestao->rsqstatus = 'I';
                        $respostaSubquestao->salvar();
                        $respostaSubquestao->clearDados();
                    }
                }
            }

            // Apaga as subquestes que tenham sido desmarcadas
            $arRsqidExistente = $respostaSubquestao->pegaRsqid(array('qtsid' => $qtsid));
            $arRsqidApagar = array_diff($arRsqidExistente, $arRsqidAtivo);
            $respostaSubquestao->apagaPorRsqid($arRsqidApagar);
            $db->commit();
//            clearSubPerguntas($_POST['sqtid'], $_POST['rsqid'], $qtsid);
        }
    }
}

function salvarQuestionarioObraParalisadaSupervisao()
{
    global $db;

    $sueid = $_SESSION['obras2']['sueid'];

    $sql_veri = 'SELECT qopid
                      FROM obras2.questionarioobraparalisada
                      WHERE sueid = ' . $sueid;
    $qopid = $db->pegaUm($sql_veri);

    $supEmp = new SupervisaoEmpresa($_POST['sueid']);
    $dataOs = $supEmp->pegaDataOsSupEmpresa($_POST['sueid']);
    $dataLim = '2014-06-09';

    if ($dataOs > $dataLim) {
        if (empty($_POST['qoprespostaum'])) {
            echo '<script type="text/javascript">
                    alert(\'O formulrio de Obras Paralisadas deve ser preenchido.\');
                  </script>';
            return false;
        }
    } else {
        if (empty($_POST['qoprespostaum'])) {
            return false;
        }
    }

    $arrDado['qoprespostaum'] = "'" . $_POST['qoprespostaum'] . "'";

    $arrDado['qoprespostadois'] = (empty($_POST['qoprespostadois']) || trim($_POST['qoprespostadois']) == '') ? 'NULL' : $_POST['qoprespostadois'];
    $arrDado['qoprespostatres'] = (empty($_POST['qoprespostatres']) || trim($_POST['qoprespostatres']) == '') ? 'NULL' : $_POST['qoprespostatres'];
    $arrDado['qoprespostaquatro'] = empty($_POST['qoprespostaquatro']) ? 'NULL' : "'" . $_POST['qoprespostaquatro'] . "'";
    $arrDado['qoprespostacinco'] = empty($_POST['qoprespostacinco']) ? 'NULL' : "'" . $_POST['qoprespostacinco'] . "'";

    $arrDado['qoprespostadois'] = is_numeric($arrDado['qoprespostadois']) ? $arrDado['qoprespostadois'] : 0;
    $arrDado['qoprespostatres'] = is_numeric($arrDado['qoprespostatres']) ? $arrDado['qoprespostatres'] : 0;

    if (!empty($qopid)) {
        $sql = "UPDATE obras2.questionarioobraparalisada SET qoprespostaum     = " . $arrDado['qoprespostaum'] . ",
                                                             qoprespostadois   = " . $arrDado['qoprespostadois'] . ",
                                                             qoprespostatres   = " . $arrDado['qoprespostatres'] . ",
                                                             qoprespostaquatro = " . $arrDado['qoprespostaquatro'] . ",
                                                             qoprespostacinco  = " . $arrDado['qoprespostacinco'] . "
                 WHERE qopid = " . $qopid;
    } else {
        $sqlObrid = "SELECT obrid FROM obras2.obras WHERE empid = " . $_SESSION['obras2']['empid'];
        $obrid = $db->pegaUm($sqlObrid);
        $obrid = (empty($obrid)) ? 'NULL' : $obrid;
        $sql = "INSERT INTO obras2.questionarioobraparalisada (obrid, docid, sueid, usucpf, qopdatainclusao, qopstatus, qoprespostaum, qoprespostadois, qoprespostatres, qoprespostaquatro, qoprespostacinco)
                                                       VALUES (" . $obrid . ",
                                                               NULL,
                                                               " . $sueid . ",
                                                               '" . $_SESSION['usucpf'] . "',
                                                               NOW(),
                                                               'A',
                                                               " . $arrDado['qoprespostaum'] . ",
                                                               " . $arrDado['qoprespostadois'] . ",
                                                               " . $arrDado['qoprespostatres'] . ",
                                                               " . $arrDado['qoprespostaquatro'] . ",
                                                               " . $arrDado['qoprespostacinco'] . " )";
    }

    try {
        $db->executar($sql);
    } catch (Exception $ex) {
        echo '<script type="text/javascript">
                alert(\'Erro ao cadastrar os dados do formulrio de Obras Paralisadas.\');
              </script>';
    }
}

function salvarSupervisaoEmpresa()
{

    global $db;
    switch ($_SESSION['obras2']['abaAjax']) {
        case 'dadosSupervisao':
            if (empty($_POST['sosid'])) {
                die("<script>
           alert('No  possvel cadastrar uma supervisão sem a vinculao de uma Ordem de serviço.');
            window.location = '?modulo=principal/cadVistoriaEmpresa&acao=E';
        </script>");
            } else {
                salvarDadosSupervisao();
            }
            if ($_POST['sobid'] === '4' || $_POST['sobid'] === 4) {
                salvarQuestionarioObraParalisadaSupervisao();
            }
            break;
        case 'localObra':
            salvarLocalObra();
            break;
        case 'cronograma':
            salvarCronograma();
            break;
        case 'questionario':
            salvarQuestionario();
            break;
        case 'fotos':
            break;
        case 'tramitacao':
            salvarTramitacao();
            break;
        case 'restricao':
            $supervisaoEmpresaRestricao = new SupervisaoEmpresaRestricao();
            $supervisaoEmpresaRestricao->salvarSupervisaoEmpresaRestricao($_POST['resposta'], $_POST['sueid']);
            break;
    }

    $db->commit();
    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = '?modulo=principal/cadVistoriaEmpresa&acao=E';
        </script>");
}

function salvarTramitacao()
{

    //$empid = $_SESSION['obras2']['empid'];
    $sueid = $_SESSION['obras2']['sueid'];
    $risco = new RiscoQuestionarioSupervisao();
    $supervisaoEmpresa = new SupervisaoEmpresa($sueid);

    if (isset($_POST['suejustificativa'])) {
        $supervisaoEmpresa->suejustificativa = strip_tags($_POST['suejustificativa']);
        $supervisaoEmpresa->salvar();
        unset($supervisaoEmpresa);
    }

    if (isset($_POST['sueproblema'])) {
        $supervisaoEmpresa = new SupervisaoEmpresa($sueid);
        $supervisaoEmpresa->sueproblema = $_POST['sueproblema'];
        $supervisaoEmpresa->sueobsproblema = ($_POST['sueproblema'] == 't') ? $_POST['sueobsproblema'] : ' ';
        $supervisaoEmpresa->salvar();
        unset($supervisaoEmpresa);
    }

    $risco->deletaPorSueId($sueid);
    if (!empty($_POST['sqtid']) && count($_POST['sqtid'], COUNT_RECURSIVE) - count($_POST['sqtid']) == 0) {
        foreach ($_POST['sqtid'] as $k => $sqtid) {
            $value = explode('-', $sqtid);
            $arrDado = array(
                'qstid' => (int)$value[1],
                'sqtid' => (int)$value[0],
                'sueid' => (int)$_POST['sueid'],
                'mrqid' => (int)$_POST['mrqid'][$k],
                'sueriscousuario' => strip_tags($_POST["sueriscousuario{$value[0]}"]),
                'suetiporiscoobs' => ($_POST["sueriscousuario{$value[0]}"] == 'S') ? strip_tags($_POST["suetiporiscoobs{$value[0]}"]) : '',
                'rsqstatus' => 'A',
            );

            $risco->popularDadosObjeto($arrDado)->salvar(true, true, null);
            $risco->clearDados();
        }
    }

//    if (count($_POST['mrqid'])) {
//        foreach ($_POST['mrqid'] as $mrqid) {
//            $value = explode('-', $mrqid);
//            $arrDado = array(
//                'qstid' => (int) $value[1],
//                'mrqid' => (int) $value[0],
//                'sueid' => (int) $_POST['sueid'],
//                'sueriscousuario' => strip_tags($_POST["sueriscousuario{$value[0]}"]),
//                'suetiporiscoobs' => ($_POST["sueriscousuario{$value[0]}"] == 'S') ? strip_tags($_POST["suetiporiscoobs{$value[0]}"]) : '',
//                'rsqstatus' => 'A',
//            );
//            $risco->popularDadosObjeto($arrDado)->salvar(true, true, null);
//            $risco->clearDados();
//        }
//    }
    return true;
}

function salvarDadosSupervisaoMI()
{

    global $db;

    $empid = $_SESSION['obras2']['empid'];
    $sueid = $_SESSION['obras2']['smiid'];

    $supervisaoMi = new SupervisaoMi($smiid);

    $arDado = $_POST;
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['empid'] = $empid;
    $arDado['smidtatualizacao'] = date('Y/m/d');
    $arDado['smidtsupervisao'] = formata_data_sql($arDado['smidtsupervisao']);
    $arCamposNulo = array();
    if (empty($arDado['sobid'])) {
        $arDado['sobid'] = null;
        $arCamposNulo[] = 'sobid';
    }
    if (empty($arDado['smiacordo'])) {
        $arDado['smiacordo'] = null;
        $arCamposNulo[] = 'smiacordo';
    }
    if (empty($arDado['entidvistoriador'])) {
        $arDado['entidvistoriador'] = null;
        $arCamposNulo[] = 'entidvistoriador';
    }
    if (empty($arDado['smidtsupervisao'])) {
        $arDado['smidtsupervisao'] = null;
        $arCamposNulo[] = 'smidtsupervisao';
    }
    if (empty($arDado['smicargorepresentante'])) {
        $arDado['smicargorepresentante'] = null;
        $arCamposNulo[] = 'smicargorepresentante';
    }

    $smiid = $supervisaoMi->popularDadosObjeto($arDado)
        ->salvar(true, true, $arCamposNulo);
    $_SESSION['obras2']['smiid'] = $smiid;
}

function salvarDadosSupervisaoFNDE()
{

    global $db;

    $empid = $_SESSION['obras2']['empid'];
    $sueid = $_SESSION['obras2']['smiid'];

    $supervisaoFnde = new SupervisaoFNDE($smiid);

    $arDado = $_POST;
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['empid'] = $empid;
    $arDado['sfndedtatualizacao'] = date('Y/m/d');
    $arDado['sfndedtsupervisao'] = formata_data_sql($arDado['sfndedtsupervisao']);

    $arCamposNulo = array();
    if (empty($arDado['sobid'])) {
        $arDado['sobid'] = null;
        $arCamposNulo[] = 'sobid';
    }
    if (empty($arDado['sfndeacordo'])) {
        $arDado['sfndeacordo'] = null;
        $arCamposNulo[] = 'sfndeacordo';
    }
    if (empty($arDado['entidvistoriador'])) {
        $arDado['entidvistoriador'] = null;
        $arCamposNulo[] = 'entidvistoriador';
    }
    if (empty($arDado['sfndedtsupervisao'])) {
        $arDado['sfndedtsupervisao'] = null;
        $arCamposNulo[] = 'sfndedtsupervisao';
    }
    if (empty($arDado['sfndecargorepresentante'])) {
        $arDado['sfndecargorepresentante'] = null;
        $arCamposNulo[] = 'sfndecargorepresentante';
    }

    $smiid = $supervisaoFnde->popularDadosObjeto($arDado)
        ->salvar(true, true, $arCamposNulo);
    $_SESSION['obras2']['sfndeid'] = $smiid;

//  $supervisao = new Supervisao();
//  $percentExec = 0;
//  $_POST['obrid'] = $_POST['obrid'] ? $_POST['obrid'] : array();
//  foreach ( $_POST['obrid'] as $obrid ){
//      $numObr++;
//      if ( !empty( $_POST['supid'][$obrid] ) ){
//          $supervisao->carregarPorId( $_POST['supid'][$obrid] );
//      }
//      $supervisao->smiid = $smiid;
//      $supervisao->obrid = $obrid;
//      $supid = $supervisao->salvar();
//      $supervisao->clearDados();
//
//      $percentExec += $supervisao->percentExec;
//  }
}

function salvarLocalObraMI()
{
    if (empty($_SESSION['obras2']['smiid'])) {
        salvarDadosSupervisaoMI();
    }

    $empid = $_SESSION['obras2']['empid'];
    $smiid = $_SESSION['obras2']['smiid'];

    if ($_POST['smiendcorreto'] == 'n') {
        $endid = ($_POST['endid'] != $_POST['emp_endid'] ? $_POST['endid'] : '');
        $endereco = new Endereco($endid);
        $dadosEnd = $_POST['endereco'];
        $dadosEnd['endcep'] = str_replace(array('.', '-'), '', $dadosEnd['endcep']);
        $endid = $endereco->popularDadosObjeto($dadosEnd)
            ->salvar();
    } else {
        $endid = $_POST['emp_endid'];
    }
    $supervisaoMi = new SupervisaoMi($smiid);

    $arDado['empid'] = $empid;
    $arDado['endid'] = $endid;
    $arDado['smiendcorreto'] = $_POST['smiendcorreto'];

    $smiid = $supervisaoMi->popularDadosObjeto($arDado)
        ->salvar(true, true);
    $_SESSION['obras2']['smiid'] = $smiid;

    $supervisao = new Supervisao();
    $percentExec = 0;
    $_POST['obrid'] = $_POST['obrid'] ? $_POST['obrid'] : array();
    foreach ($_POST['obrid'] as $obrid) {
        $numObr++;
        if (!empty($_POST['supid'][$obrid])) {
            $supervisao->carregarPorId($_POST['supid'][$obrid]);
        }
        $supervisao->smiid = $smiid;
        $supervisao->obrid = $obrid;
        $supid = $supervisao->salvar();
        $supervisao->clearDados();

        $percentExec += $supervisao->percentExec;
    }

    $empreendimento = new Empreendimento($empid);
    $empreendimento->empdtultvistoriami = date('Y-m-d');

    $ultSmiid = $supervisaoMi->pegaUltSmiidByEmpid($empid);
    if ($smiid == $ultSmiid) {
        $empreendimento->emppercentultvistoriami = ($percentExec > 0 ? $percentExec / $numObr : 0);
    }
    $empreendimento->salvar();
}

function salvarLocalObraFNDE()
{
    if (empty($_SESSION['obras2']['sfndeid'])) {
        salvarDadosSupervisaoFNDE();
    }

    $empid = $_SESSION['obras2']['empid'];
    $sfndeid = $_SESSION['obras2']['sfndeid'];

    if ($_POST['sfndeendcorreto'] == 'n') {
        $endid = ($_POST['endid'] != $_POST['emp_endid'] ? $_POST['endid'] : '');
        $endereco = new Endereco($endid);
        $dadosEnd = $_POST['endereco'];
        $dadosEnd['endcep'] = str_replace(array('.', '-'), '', $dadosEnd['endcep']);
        $endid = $endereco->popularDadosObjeto($dadosEnd)
            ->salvar();
    } else {
        $endid = $_POST['emp_endid'];
    }
    $supervisaoFnde = new SupervisaoFNDE($sfndeid);

    $arDado['empid'] = $empid;
    $arDado['endid'] = $endid;
    $arDado['sfndeendcorreto'] = $_POST['sfndeendcorreto'];

    $sfndeid = $supervisaoFnde->popularDadosObjeto($arDado)
        ->salvar(true, true);
    $_SESSION['obras2']['sfndeid'] = $sfndeid;

    $supervisao = new Supervisao();
    $percentExec = 0;
    $_POST['obrid'] = $_POST['obrid'] ? $_POST['obrid'] : array();
    foreach ($_POST['obrid'] as $obrid) {
        $numObr++;
        if (!empty($_POST['supid'][$obrid])) {
            $supervisao->carregarPorId($_POST['supid'][$obrid]);
        }
        $supervisao->sfndeid = $sfndeid;
        $supervisao->obrid = $obrid;
        $supid = $supervisao->salvar();
        $supervisao->clearDados();

        $percentExec += $supervisao->percentExec;
    }

    $empreendimento = new Empreendimento($empid);
    $empreendimento->empdtultvistoriafnde = date('Y-m-d');

    $ultSfndeid = $supervisaoFnde->pegaUltSfndeidByEmpid($empid);
    if ($sfndeid == $ultSfndeid) {
        $empreendimento->emppercentultvistoriafnde = ($percentExec > 0 ? $percentExec / $numObr : 0);
    }
    $empreendimento->salvar();
}

function salvarFotosMI()
{
    $dados = $_POST;
    $obrid = $_SESSION['obras2']['obrid'];
    $smiid = ($_REQUEST['smiid'] != '' ? $_REQUEST['smiid'] : $_SESSION['obras2']['smiid']);
    $supervisaoMi = new SupervisaoMi($smiid);
    $supervisao = new Supervisao();
    $supid = $supervisao->pegaSupidByObraAndSmiid($obrid, $supervisaoMi->smiid);
    if (!$supid) {
        return;
    }
    $supervisao->carregarPorId($supid);

    $supervisao->supobs = (($dados["supobs"] > 5000) ? substr($dados["supobs"], 0, 5000) : $dados["supobs"]);
    $supervisao->salvar();
}

function salvarQuestionarioMI()
{
    if (empty($_SESSION['obras2']['smiid'])) {
        salvarDadosSupervisaoMI();
    }

    $empid = $_SESSION['obras2']['empid'];
    $obrid = $_SESSION['obras2']['obrid'];
    $smiid = $_SESSION['obras2']['smiid'];

    // QUESTIONRIO
    if (is_array($_POST['qstid'])) {
        $questaoSupervisao = new QuestaoSupervisao();
        $respostaSubquestao = new RespostaSubquestao();
        $modeloRestricaoQuestionario = new ModeloRestricaoQuestionario();
        $restricao = new Restricao();
        $arqQuestaoSupervisao = new ArquivoQuestaoSupervisao();

        $_POST = conversaoArrayPhpParaSupervisao($_POST);

        foreach ($_POST['qstid'] as $qstid => $resp) {
            //$questaoSupervisao->apagaRespostaPorQuestao( $qstid );
            if ($_POST["qtsid_{$qstid}"]) {
                $questaoSupervisao->carregarPorId($_POST["qtsid_{$qstid}"]);
                if ($questaoSupervisao->qtsid && trim($questaoSupervisao->qtsresposta) != trim($resp)) {
                    $arqQuestaoSupervisao->apagaPorQtsid($questaoSupervisao->qtsid);
                }
            }
            $arDados = array(
                'qstid' => $qstid,
                'smiid' => $smiid,
                'qtsresposta' => $resp,
                'qtsobs' => $_POST['qtsobs'][$qstid]
            );
            $qtsid = $questaoSupervisao->popularDadosObjeto($arDados)
                ->salvar();

            $questaoSupervisao->clearDados();

            $arRsqidAtivo = array();
            if (is_array($_POST['sqtid'][$qstid]) && $resp == 'f') {
                foreach ($_POST['sqtid'][$qstid] as $sqtid) {
//                  if( $rsqobs != '' ){
                    if ($_POST['rsqid'][$qstid][$sqtid] != '') {
                        $respostaSubquestao->carregarPorId($_POST['rsqid'][$qstid][$sqtid]);
//                        $rsqidSalvos[] = $_POST['rsqid'][$qstid][$sqtid];
                    }
                    $rsqobs = $_POST['rsqobs'][$qstid][$sqtid];
                    $arDados = array(
                        'qtsid' => $qtsid,
                        'sqtid' => $sqtid,
                        'rsqobs' => $rsqobs
                    );
                    $rsqid = $respostaSubquestao->popularDadosObjeto($arDados)
                        ->salvar();
                    $respostaSubquestao->clearDados();
//                  }
                    $arRsqidAtivo[] = $rsqid;
                }
            } elseif (is_array($_POST['sqtid'][$qstid])) {
                // Apaga as respostas das subquestes que a questo mudou a resposta de 'f' para outro
                foreach ($_POST['sqtid'][$qstid] as $sqtid) {
                    if ($_POST['rsqid'][$qstid][$sqtid] != '') {
                        $respostaSubquestao->carregarPorId($_POST['rsqid'][$qstid][$sqtid]);
                        $respostaSubquestao->rsqstatus = 'I';
                        $respostaSubquestao->salvar();

                        $respostaSubquestao->clearDados();
                    }
                }
            }

            // Apaga as subquestes que tenham sido desmarcadas
            $arRsqidExistente = $respostaSubquestao->pegaRsqid(array('qtsid' => $qtsid));
            $arRsqidApagar = array_diff($arRsqidExistente, $arRsqidAtivo);
            $respostaSubquestao->apagaPorRsqid($arRsqidApagar);
//            clearSubPerguntas($_POST['sqtid'], $_POST['rsqid'], $qtsid);
        }
    }
}

function salvarQuestionarioFNDE()
{
    if (empty($_SESSION['obras2']['sfndeid'])) {
        salvarDadosSupervisaoFNDE();
    }

    $empid = $_SESSION['obras2']['empid'];
    $obrid = $_SESSION['obras2']['obrid'];
    $sfndeid = $_SESSION['obras2']['sfndeid'];

    // QUESTIONRIO
    if (is_array($_POST['qstid'])) {
        $questaoSupervisao = new QuestaoSupervisao();
        $respostaSubquestao = new RespostaSubquestao();
        $modeloRestricaoQuestionario = new ModeloRestricaoQuestionario();
        $restricao = new Restricao();
        $arqQuestaoSupervisao = new ArquivoQuestaoSupervisao();

        // resgata textarea
        $_POST = conversaoArrayPhpParaSupervisao($_POST);


        foreach ($_POST['qstid'] as $qstid => $resp) {
            //          $questaoSupervisao->apagaRespostaPorQuestao( $qstid );
            if ($_POST["qtsid_{$qstid}"]) {
                $questaoSupervisao->carregarPorId($_POST["qtsid_{$qstid}"]);
                if ($questaoSupervisao->qtsid && trim($questaoSupervisao->qtsresposta) != trim($resp)) {
                    $arqQuestaoSupervisao->apagaPorQtsid($questaoSupervisao->qtsid);
                }
            }
            $arDados = array(
                'qstid' => $qstid,
                'sfndeid' => $sfndeid,
                'qtsresposta' => $resp,
                'qtsobs' => $_POST['qtsobs'][$qstid]
            );
            $qtsid = $questaoSupervisao->popularDadosObjeto($arDados)
                ->salvar();

            $questaoSupervisao->clearDados();

            $arRsqidAtivo = array();
            if (is_array($_POST['sqtid'][$qstid]) && $resp == 'f') {
                foreach ($_POST['sqtid'][$qstid] as $sqtid) {
//                  if( $rsqobs != '' ){
                    if ($_POST['rsqid'][$qstid][$sqtid] != '') {
                        $respostaSubquestao->carregarPorId($_POST['rsqid'][$qstid][$sqtid]);
//                        $rsqidSalvos[] = $_POST['rsqid'][$qstid][$sqtid];
                    }
                    $rsqobs = $_POST['rsqobs'][$qstid][$sqtid];
                    $arDados = array(
                        'qtsid' => $qtsid,
                        'sqtid' => $sqtid,
                        'rsqobs' => $rsqobs
                    );
                    $rsqid = $respostaSubquestao->popularDadosObjeto($arDados)
                        ->salvar();
                    $respostaSubquestao->clearDados();
//                  }
                    $arRsqidAtivo[] = $rsqid;
                }
            } elseif (is_array($_POST['sqtid'][$qstid])) {
                // Apaga as respostas das subquestes que a questo mudou a resposta de 'f' para outro
                foreach ($_POST['sqtid'][$qstid] as $sqtid) {
                    if ($_POST['rsqid'][$qstid][$sqtid] != '') {
                        $respostaSubquestao->carregarPorId($_POST['rsqid'][$qstid][$sqtid]);
                        $respostaSubquestao->rsqstatus = 'I';
                        $respostaSubquestao->salvar();

                        $respostaSubquestao->clearDados();
                    }
                }
            }

            // Apaga as subquestes que tenham sido desmarcadas
            $arRsqidExistente = $respostaSubquestao->pegaRsqid(array('qtsid' => $qtsid));
            $arRsqidApagar = array_diff($arRsqidExistente, $arRsqidAtivo);
            $respostaSubquestao->apagaPorRsqid($arRsqidApagar);
//            clearSubPerguntas($_POST['sqtid'], $_POST['rsqid'], $qtsid);
        }
    }
}

function salvarJustificativasCronogramaMI()
{
    global $db;
    $sql = '';
    if (is_array($_POST['spiid_just'])) {
        foreach ($_POST['spiid_just'] as $key => $value) {
            $sql .= "UPDATE obras2.supervisaoitem SET spitextojustificativatrocavalores = '" . $value[0] . "' WHERE spiid = " . $key . ";   ";
        }
        $db->executar($sql);
    }
}

function salvarCronogramaMI()
{
    if (empty($_SESSION['obras2']['smiid'])) {
        salvarDadosSupervisaoMI();
    }

    if (is_array($_POST['spiid_just'])) {
        salvarJustificativasCronogramaMI();
    }

    $smiid = $_SESSION['obras2']['smiid'];
    $supervisao = new Supervisao();
    $percentExec = 0;
    $_POST['obrid'] = ($_POST['obrid'] ? $_POST['obrid'] : array());

    foreach ($_POST['obrid'] as $obrid) {
        $numObr++;
        if (!empty($_POST['supid'][$obrid])) {
            $supervisao->carregarPorId($_POST['supid'][$obrid]);
        }
        $supervisao->smiid = $smiid;
        $supervisao->obrid = $obrid;
        $supid = $supervisao->salvar();
        $supervisao->clearDados();
        $percentExec += $supervisao->percentExec;
    }
}

function salvarCronogramaFNDE()
{
    if (empty($_SESSION['obras2']['sfndeid'])) {
        salvarDadosSupervisaoFNDE();
    }

    $sfndeid = $_SESSION['obras2']['sfndeid'];

    $supervisao = new Supervisao();
    $percentExec = 0;
    $_POST['obrid'] = ($_POST['obrid'] ? $_POST['obrid'] : array());
    foreach ($_POST['obrid'] as $obrid) {
        $numObr++;
        // dbg($_POST['supid'][$obrid]);
        // dbg($_POST['supid'],d);
        if (!empty($_POST['supid'][$obrid])) {
            $supervisao->carregarPorId($_POST['supid'][$obrid]);
        }
        $supervisao->sfndeid = $sfndeid;
        $supervisao->obrid = $obrid;
        // dbg($supervisao,d);
        $supid = $supervisao->salvar();
        $supervisao->clearDados();

        $percentExec += $supervisao->percentExec;
    }
}

function salvarSupervisaoMI()
{

    global $db;

    switch ($_SESSION['obras2']['abaAjax']) {
        case 'dadosSupervisao':
            salvarDadosSupervisaoMI();
        case 'localObra':
            salvarLocalObraMI();
        case 'cronograma':
            salvarCronogramaMI();
        case 'questionario':
            salvarQuestionarioMI();
        case 'fotos':
            salvarFotosMI();
        case 'tramitacao':
    }

    $db->commit();
    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = '?modulo=principal/cadSupervisaoMI&acao=E';
        </script>");
}

function salvarSupervisaoFNDE()
{

    global $db;

    switch ($_SESSION['obras2']['abaAjax']) {
        case 'dadosSupervisao':
            salvarDadosSupervisaoFNDE();
        case 'localObra':
            salvarLocalObraFNDE();
        case 'cronograma':
            salvarCronogramaFNDE();
        case 'questionario':
            salvarQuestionarioFNDE();
        case 'fotos':
        case 'tramitacao':
    }

    $db->commit();
    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = '?modulo=principal/cadSupervisaoFNDE&acao=E';
        </script>");
}

function criaAbaPagamento()
{
    $menu = array(
        0 => array(
            "id" => 1,
            "descricao" => "Lista de Pagamentos",
            "link" => "/obras2/obras2.php?modulo=principal/listaPagamento&acao=A"
        ),
        1 => array(
            "id" => 2,
            "descricao" => "Cadastro Pagamento",
            "link" => "/obras2/obras2.php?modulo=principal/pagamento&acao={$_GET['acao']}" . ($_GET['pagid'] ? "&pagid=" . $_GET['pagid'] : "")
        ),
        2 => array(
            "id" => 2,
            "descricao" => "Realizar Pagamento",
            "link" => "/obras2/obras2.php?modulo=principal/realizarPagamento&acao={$_GET['acao']}" . ($_GET['pagid'] ? "&pagid=" . $_GET['pagid'] : "")
        ),
    );
    $abaAtiva = $_SERVER['REQUEST_URI'];
    echo montarAbasArray($menu, $abaAtiva);
}

function recuperaArquivo($arqid)
{
    global $db;
    $sql = "select * from public.arquivo where arqid = $arqid";
    return $db->pegaLinha($sql);
}

function verificaPreenchimentoPagamento($pagid)
{
    require_once APPRAIZ . "obras2/classes/modelo/Pagamento.class.inc";

    $pag = new Pagamento($pagid);

    if (!$pag->arqid) {
        return "Favor inserir a nota fiscal.";
    }
    if (!$pag->verificaSupervisao()) {
        return "Favor informar as superises.";
    }

    return true;
}

function tramitarLaudos($pagid, $aedid)
{
    global $db;

    //Executa a mudana de estado de wk em lote.
    $sql = "select
                    sue.docid
            from
                    obras2.supervisaoempresa sue
            inner join
                    obras2.pagamento_supervisao_empresa pse ON pse.sueid = sue.sueid AND pse.psestatus = 'A'
            where
                    pagid = $pagid";
    $arrDocid = $db->carregarColuna($sql);
    if ($arrDocid) {
        require_once APPRAIZ . 'includes/workflow.php';
        foreach ($arrDocid as $docid) {
            wf_alterarEstado($docid, $aedid, '', array());
        }
        return true;
    } else {
        return false;
    }
}

function verificaPreenchimentoBancario($pagid)
{
    global $db;

    if ($_SESSION['usucpf'] == '') {
        $sql = "select count(pagid) from obras2.pagamento
                    where pagid = $pagid
                        and pagnrnotafiscal in ('1122014','1042014','1052014','1172014','1012014','992014','1022014','1182014','1162014','2852014')";
        $boTotal = $db->pegaUm($sql);
        if ((int)$boTotal > 0) {
            return true;
        }
    }
    //verifica se todas as supervises do pagamento foram verificadas
    $sql = "select count(se.sueid) from obras2.pagamento_supervisao_empresa se JOIN obras2.supervisaoempresa s ON s.sueid = se.sueid AND s.suestatus = 'A' where se.pagid = $pagid AND se.psestatus = 'A'";
    $total_pagamento = $db->pegaUm($sql);
    if (!$total_pagamento) {
        return "No existem supervises vinculadas a este pagamento.";
    }
    //verifica se todas as supervises do pagamento foram verificadas
    $sql = "
            select count(*)
            from obras2.pagamento_supervisao_empresa pse
            inner join obras2.supervisaoempresa sue on pse.sueid = sue.sueid
            inner join obras2.supervisao_os so on sue.sosid = so.sosid
            where pse.pagid = $pagid AND pse.psestatus = 'A'
                AND so.sosvalortotal/ (SELECT COUNT(*) FROM obras2.supervisao_os_obra soo
                JOIN obras2.supervisaoempresa s ON s.empid = soo.empid AND s.suestatus = 'A' AND s.sosid = soo.sosid
                JOIN workflow.documento d ON d.docid = s.docid AND d.esdid != 1188
                WHERE soo.sosid = so.sosid AND soostatus = 'A'
                ) = 0";
    $total_pagamento_retificado = $db->pegaUm($sql);
    $sql = "select count(sueid) from obras2.pagamento_glosa_supervisao where pagid = $pagid";
    $total_glosado = $db->pegaUm($sql);
    if ($total_pagamento != $total_glosado) {
        $total = $total_pagamento - $total_glosado - $total_pagamento_retificado;
        if ($total > 1) {
            return "Existem $total supervises no verificadas no pagamento.";
        } elseif ($total == 1) {
            return "Existe $total supervisão no verificada no pagamento.";
        }
    }
    return true;
}

function verificaSupervisaoOSConcluida($sosid)
{
    global $db;
    //Pegar todas as supervises da OS e verificar se foram pagas
    $sql = "select count(sueid) from obras2.supervisaoempresa where sosid = $sosid and suestatus = 'A'";
    $total_supervisao = $db->pegaUm($sql);
    if (!$total_supervisao) {
        return "No existem supervises nesta OS.";
    }
    $sql = "select
                    count(sueid)
            from
                    obras2.supervisaoempresa sue
            inner join
                    workflow.documento doc ON doc.docid = sue.docid
            where
                    sue.sosid = $sosid and suestatus = 'A'
            and
                    esdid = " . WF_ESDID_LAUDO_SUPERVISAO_PAGO;
    $total_pagos = $db->pegaUm($sql);
    if ($total_supervisao != $total_pagos) {
        $total = $total_supervisao - $total_pagos;
        if ($total > 1) {
            return "Existem $total supervises vinculadas  OS no pagas.";
        } else {
            return "Existe $total supervisão vinculada  OS no paga.";
        }
    }
    return true;

    //return "verificar se as supervises foram pagas...";
}

function verificaDataAcaoWorkFlow($docid, $aedid)
{
    global $db;
    $sql = "select *,to_char(htddata,'YYYY-MM-DD') as data from workflow.historicodocumento where docid = $docid and aedid = $aedid";
    return $db->pegaLinha($sql);
}

function retornaDiasEntreDatas($data1, $data2)
{
    // Usa a funo strtotime() e pega o timestamp das duas datas:
    $time_inicial = strtotime($data1);
    $time_final = strtotime($data2);

    // Calcula a diferena de segundos entre as duas datas:
    $diferenca = $time_final - $time_inicial;

    // Calcula a diferena de dias
    $dias = (int)floor($diferenca / (60 * 60 * 24));

    return ($dias < 0 ? 0 : $dias);
}

function exibeGLosaPorSueid($sueid, $pagid = null, $dias_fora_prazo = null, $valor = null)
{
    global $db;

    if ($pagid) {
        $sql = "select * from obras2.pagamento_glosa_supervisao where sueid = $sueid and pagid = $pagid";
        $arrDados = $db->pegaLinha($sql);
        if ($arrDados) {
            extract($arrDados);
        }
    }

    $tipo = new Tipo_Glosa_Pagamento();
    $dados = $tipo->listaCombo();
    if (!empty($tgpid)) {
        $tgpid = 1;
    }
    $comboTipo = $db->monta_combo("tgpid_$sueid", $dados, "S", "", "", '', '', '', 'S', 'tgpid', true, $tgpid);
    $campoObservacao = campo_textarea("pgsobs_$sueid", "N", "S", "", 60, 5, 255, "", '', '', '', '', $pgsobs);

    if (empty($pgsvalor)) {
        $pgsglosavalor = (0.03 * $dias_fora_prazo) * $valor / 100;
        $pgsvalor = ($valor - $pgsglosavalor);
    }

    $campoValor = campo_texto(
        "pgsvalor_$sueid",
        "S",
        "S",
        "",
        20,
        255,
        "[.###],##",
        '',
        '',
        '',
        '',
        '',
        '',
        number_format($pgsvalor, 2, ',', '.')
    );
    $campoValor2 = campo_texto(
        "pgsglosavalor_$sueid",
        "N",
        "S",
        "",
        20,
        255,
        "[.###],##",
        '',
        '',
        '',
        '',
        '',
        '',
        number_format($pgsglosavalor, 2, ',', '.')
    );

    $html = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <tr>
            <td class="SubTituloDireita" width="25%">Tipo</td>
                    <td>
                            ' . $comboTipo . '
                    </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Observaes</td>
                    <td>
                            ' . $campoObservacao . '
                    </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Valor Final</td>
                    <td>
                            ' . $campoValor . '
                    </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Valor da Glosa</td>
                    <td>
                              ' . $campoValor2 . '
                    </td>
        </tr>


            </table>';

    return $html;
}

function carregaAbaDadosUnidade($stPaginaAtual = null, $obrid, $esfera = 'M')
{

    $abas = array();
    if ($esfera == 'M') {
        array_push($abas, array(
            "id" => 0,
            "descricao" => "Prefeitura",
            "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&dutid=6&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 1,
            "descricao" => "Prefeito(a)",
            "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&dutid=7&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 2,
            "descricao" => "Secretaria Municipal de Educao",
            "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&dutid=8&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 3,
            "descricao" => "Dirigente Municipal de   Educao",
            "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&dutid=2&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 4,
            "descricao" => "Equipe Local",
            "link" => "/obras2/obras2.php?modulo=principal/equipeLocal&acao=A&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 5,
            "descricao" => "Comit Local",
            "link" => "/obras2/obras2.php?modulo=principal/comiteLocal&acao=A&obrid=" . $obrid
        ));
    } else {
        array_push($abas, array(
            "id" => 1,
            "descricao" => "Secretaria Estadual de Educao",
            "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&dutid=9&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 2,
            "descricao" => "Secretrio(a) Estadual de Educao",
            "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&dutid=10&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 3,
            "descricao" => "Equipe Local",
            "link" => "/obras2/obras2.php?modulo=principal/equipeLocal&acao=A&obrid=" . $obrid
        ));
        array_push($abas, array(
            "id" => 4,
            "descricao" => "Comit Local",
            "link" => "/obras2/obras2.php?modulo=principal/comiteLocal&acao=A&obrid=" . $obrid
        ));
    }

//    if ($esfera == 'M') {
//        array_push($abas, array("id" => 0, "descricao" => "Prefeitura", "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=1&obrid=" . $obrid));
//        array_push($abas, array("id" => 1, "descricao" => "Prefeito(a)", "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=2&obrid=" . $obrid));
//        array_push($abas, array("id" => 2, "descricao" => "Secretaria Municipal de Educao", "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=7&obrid=" . $obrid));
//        array_push($abas, array("id" => 3, "descricao" => "Dirigente Municipal de Educao", "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=15&obrid=" . $obrid));
//        array_push($abas, array("id" => 4, "descricao" => "Equipe Local", "link" => "/obras2/obras2.php?modulo=principal/equipeLocal&acao=A&obrid=" . $obrid));
//        array_push($abas, array("id" => 5, "descricao" => "Comit Local", "link" => "/obras2/obras2.php?modulo=principal/comiteLocal&acao=A&obrid=" . $obrid));
//    } else {
//        array_push($abas, array("id" => 1, "descricao" => "Secretaria Estadual de Educao", "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=6&obrid=" . $obrid));
//        array_push($abas, array("id" => 2, "descricao" => "Secretrio(a) Estadual de Educao", "link" => "/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=25&obrid=" . $obrid));
//        array_push($abas, array("id" => 3, "descricao" => "Equipe Local", "link" => "/obras2/obras2.php?modulo=principal/equipeLocal&acao=A&obrid=" . $obrid));
//        array_push($abas, array("id" => 4, "descricao" => "Comit Local", "link" => "/obras2/obras2.php?modulo=principal/comiteLocal&acao=A&obrid=" . $obrid));
//    }

    return montarAbasArray($abas, $stPaginaAtual);
}

function carregaAbaDesbloqueio($stPaginaAtual = null, $obrid, $pdoid)
{

    $abas = array();

    if ($pdoid) {
        array_push($abas, array(
            "id" => 0,
            "descricao" => "Pedido Desbloqueio",
            "link" => "/obras2/obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid=" . $obrid . '&pdoid=' . $pdoid
        ));
        array_push($abas, array(
            "id" => 1,
            "descricao" => "Desbloqueio",
            "link" => "/obras2/obras2.php?modulo=principal/dadosDesbloqueio&acao=A&obrid=" . $obrid . '&pdoid=' . $pdoid
        ));
    } else {
        array_push($abas, array(
            "id" => 0,
            "descricao" => "Pedido Desbloqueio",
            "link" => "/obras2/obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid=" . $obrid
        ));
    }

    return montarAbasArray($abas, $stPaginaAtual);
}

function carregaAbaSolicitacao($stPaginaAtual = null, $obrid, $slcid)
{

    $abas = array();

    $tslid = retornaTslid(2);
    if ($slcid) {
        $solicitacao = new Solicitacao($slcid);
        $estado = wf_pegarEstadoAtual($solicitacao->docid);

        array_push($abas, array(
            "id" => 0,
            "descricao" => "Solicitacao",
            "link" => "/obras2/obras2.php?modulo=principal/solicitacao&acao=A&obrid=" . $obrid . '&slcid=' . $slcid . $tslid
        ));
        /* PFLCOD_SUPERVISOR_UNIDADE foi includo para poder imprimir esta pgina SIMEC #358990*/
        if (possui_perfil(array(
                PFLCOD_GESTOR_MEC,
                PFLCOD_SUPER_USUARIO,
                PFLCOD_GESTOR_UNIDADE,
                PFLCOD_SUPERVISOR_UNIDADE
            )) && retornaTslid() != 6 && retornaTslid() != 7 && ($estado['esdid'] != ESDID_SOLICITACOES_CADASTRAMENTO)) {
            array_push($abas, array(
                "id" => 1,
                "descricao" => "Análise",
                "link" => "/obras2/obras2.php?modulo=principal/solicitacaoAnalise&acao=A&obrid=" . $obrid . '&slcid=' . $slcid . $tslid
            ));
        }
    } else {
        array_push($abas, array(
            "id" => 0,
            "descricao" => "Solicitacao",
            "link" => "/obras2/obras2.php?modulo=principal/solicitacao&acao=A&obrid=" . $obrid . $tslid
        ));
    }
    return montarAbasArray($abas, $stPaginaAtual);
}

function retornaTslid($type = 1)
{
    if ($_GET['tslid']) {
        return $type === 1 ? $_GET['tslid'][0] : ('&tslid[]=' . $_GET['tslid'][0]);
    }
    return '';
}

function modificaEstadoObra()
{

    $arIdObra = array(
        14130,
        14131,
        14132,
        14134,
        14135,
        14138,
        14139,
        14140,
        14141,
        14142,
        14143,
        14144,
        14145,
        14146,
        14147,
        14148,
        14149,
        14150,
        14151,
        14152,
        14153,
        14154,
        14155,
        14156,
        14157,
        14158,
        14159,
        14160,
        14162,
        14163,
        14165,
        14166,
        14167,
        14168,
        14169,
        14170,
        14171,
        14172,
        14173,
        14174,
        14175,
        14176,
        14177,
        14178,
        14179,
        14180,
        14181,
        14182,
        14183,
        14184,
        14185,
        14189,
        14190,
        14191
    );

    require_once APPRAIZ . 'includes/workflow.php';

    $comentario = "Obra cancelada pois o recurso foi devolvido pelo Estado do Rio de Janeiro relacionado  Resoluo 19";
    foreach ($arIdObra as $obrid) {
        $docid = pegaDocidObra($obrid);
        $esdid = pegaEstadoObra($docid);
        if ($esdid == ESDID_OBJ_PLANEJAMENTO_PROPONENTE) {
            $i++;
//          dbg($docid);
//          dbg(AEDID_OBJ_PLANEJAMENTO_CANCELADO, d);
            wf_alterarEstado($docid, AEDID_OBJ_PLANEJAMENTO_CANCELADO, $comentario, array());
        } else {
//          echo "{$docid},<br>";
            echo "Obrid: {$obrid}<br>";
            echo "docid: {$docid}<br>";
            echo "Esdid: {$esdid}<br><br>";
        }
    }
    echo "Qtd Obras Migradas: {$i}";
}

function pegaDadosSolicitacao($slcid)
{
    global $db;
    $sql = "
        SELECT
            CASE WHEN emp.empesfera = 'M' THEN 'Municipal'
                 WHEN emp.empesfera = 'E' THEN 'Estadual'
                 WHEN emp.empesfera = 'F' THEN 'Federal'
            END as empesfera,
            o.obrnome,
            o.obrid,
            '(' || o.obrid || ') ' || o.obrnome || '' as descricao,
            ende.estuf,
            mun.mundescricao,
            p.slcjustificativa,
            p.usunome AS usunomepedido,
            p.usucpf AS usucpfpedido,
            p.tipo,
            TO_CHAR(p.slcdatainclusao, 'dd/mm/YYYY HH24:MM:SS') AS slcdatainclusao,

            d.deferimento,
            d.usunome AS usunomedesbloqueio,
            d.usucpf AS usucpfdesbloqueio,
            TO_CHAR(d.dsldata, 'dd/mm/YYYY HH24:MM:SS') AS dsldata,
            TO_CHAR(d.dsldatainicio, 'dd/mm/YYYY') AS dsldatainicio,
            d.dsldias,
            TO_CHAR(d.dsltermino, 'dd/mm/YYYY') AS dsltermino,
            d.dslcomentario,
            d.dsltipodesbloqueio
        FROM
            obras2.obras o
            inner join obras2.empreendimento emp on emp.empid = o.empid
        LEFT JOIN entidade.endereco     ende ON ende.endid = o.endid AND
                                                ende.endstatus = 'A' AND
                                                ende.tpeid = 4
        LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
        JOIN (
            SELECT
                slcid,
                obrid,
                slcjustificativa,
                u.usunome,
                u.usucpf,
                slcdatainclusao,
                (SELECT array_to_string(array_agg(tslnome), ',') FROM obras2.tiposolicitacao_solicitacao ts
                JOIN obras2.tiposolicitacao t ON t.tslid = ts.tslid
                WHERE ts.slcid = p.slcid) as tipo
            FROM
                obras2.solicitacao p
            JOIN seguranca.usuario u ON u.usucpf = p.usucpf
            WHERE
                slcstatus = 'A'
             ) p ON p.obrid = o.obrid
        LEFT JOIN (
            SELECT
                slcid,
                dslid,
                CASE
                    WHEN d.dsltipodesbloqueio IS NULL OR d.dsltipodesbloqueio = '' THEN 'No analisado'
                    WHEN d.dsltipodesbloqueio = 'D' THEN 'Deferido'
                    WHEN d.dsltipodesbloqueio = 'I' THEN 'Indeferido'
                END AS deferimento,
                u.usunome,
                u.usucpf,
                dsldata,
                dsltermino,
                dsldias,
                dsldatainicio,
                d.dslcomentario,
                d.dsltipodesbloqueio
            FROM
                obras2.dados_solicitacao d
            JOIN seguranca.usuario u ON u.usucpf = d.usucpf
            WHERE
                dslid IN (SELECT
                            MAX(dslid)
                          FROM
                            obras2.dados_solicitacao where slcid = d.slcid)
             ) d ON d.slcid = p.slcid
        WHERE obrstatus = 'A' AND p.slcid = $slcid";
    return $db->pegaLinha($sql);
}

function pegaDadosDesbloqueio($pdoid)
{
    global $db;
    $sql = "
            SELECT
                CASE WHEN emp.empesfera = 'M' THEN 'Municipal'
                     WHEN emp.empesfera = 'E' THEN 'Estadual'
                     WHEN emp.empesfera = 'F' THEN 'Federal'
                END as empesfera,
                o.obrnome,
                o.obrid,
                '(' || o.obrid || ') ' || o.obrnome || '' as descricao,
                ende.estuf,
                mun.mundescricao,
                p.pdojustificativa,
                p.usunome AS usunomepedido,
                p.usucpf AS usucpfpedido,
                TO_CHAR(p.pdodatainclusao, 'dd/mm/YYYY HH24:MM:SS') AS pdodatainclusao,

                d.deferimento,
                d.usunome AS usunomedesbloqueio,
                d.usucpf AS usucpfdesbloqueio,
                TO_CHAR(d.desdata, 'dd/mm/YYYY HH24:MM:SS') AS desdata,
                TO_CHAR(d.desdatainicio, 'dd/mm/YYYY') AS desdatainicio,
                d.desdias,
                TO_CHAR(d.destermino, 'dd/mm/YYYY') AS destermino,
                d.descomentario,
                d.destipodesbloqueio
            FROM
                obras2.obras o
                inner join obras2.empreendimento emp on emp.empid = o.empid
            LEFT JOIN entidade.endereco     ende ON ende.endid = o.endid AND
                                                    ende.endstatus = 'A' AND
                                                    ende.tpeid = 4
            LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
            JOIN (
                SELECT
                    pdoid,
                    obrid,
                    pdojustificativa,
                    u.usunome,
                    u.usucpf,
                    pdodatainclusao
                FROM
                    obras2.pedidodesbloqueioobra p
                JOIN seguranca.usuario u ON u.usucpf = p.usucpf
                WHERE
                    pdostatus = 'A' and
                    pdoid IN (SELECT
                                                    MAX(pdoid)
                                              FROM
                                                    obras2.pedidodesbloqueioobra a
                                              WHERE
                                                    pdostatus = 'A' and a.obrid = p.obrid )
                 ) p ON p.obrid = o.obrid
            LEFT JOIN (
                SELECT
                    pdoid,
                    desid,
                    CASE
                        WHEN d.destipodesbloqueio IS NULL OR d.destipodesbloqueio = '' THEN 'No analisado'
                        WHEN d.destipodesbloqueio = 'D' THEN 'Deferido'
                        WHEN d.destipodesbloqueio = 'I' THEN 'Indeferido'
                    END AS deferimento,
                    u.usunome,
                    u.usucpf,
                    desdata,
                    destermino,
                    desdias,
                    desdatainicio,
                    d.descomentario,
                    d.destipodesbloqueio
                FROM
                    obras2.desbloqueioobra d
                JOIN seguranca.usuario u ON u.usucpf = d.usucpf
                WHERE
                    desid IN (SELECT
                                MAX(desid)
                              FROM
                                obras2.desbloqueioobra where pdoid = d.pdoid)
                 ) d ON d.pdoid = p.pdoid
            WHERE obrstatus = 'A' AND p.pdoid = $pdoid";
    return $db->pegaLinha($sql);
}

function enviarEmailAnaliseDesbloqueio($pdoid)
{
    $usuarios = array();
    extract(pegaDadosDesbloqueio($pdoid));

    $assunto = "SIMEC.OBRAS2 - Pedido de desbloqueio ($deferimento) - Obra $descricao";
    $conteudo = "
        <table>
            <tr><td><b>Pedido de desbloqueio - ($deferimento)</b></td></tr>
            <tr><td>ID obra: $obrid</td></tr>
            <tr><td>Nome da obra: $obrnome</td></tr>
            <tr><td>Esfera: $empesfera</td></tr>
            <tr><td>UF: $estuf</td></tr>
            <tr><td>Municipio: $mundescricao</td></tr>
            <tr><td>$pdojustificativa</td></tr>
            <tr><td>&nbsp;  </td></tr>
            <tr><td>Realizado em $pdodatainclusao por $usunomepedido</td></tr>
        </table>
    ";


    $conteudo .= "
        <table>
        <tr><td>Análise:</td></tr>

        <tr><td>Situação: $deferimento</td></tr>
        ";
    if ($destipodesbloqueio == 'D') {
        $conteudo .= "
        <tr><td>Data de início: $desdatainicio</td></tr>
        <tr><td>Durao: $desdias dias</td></tr>
        <tr><td>Data de término: $destermino</td></tr>
        ";
    }
    $conteudo .= "
        <tr><td>Comentrios: $descomentario</td></tr>
        <tr><td>Realizado em $desdata por $usunomedesbloqueio</td></tr>
        </table>
    ";
    $remetente = array("nome" => "SIMEC - Monitoramento de Obras", "email" => "monitoramento.obras@mec.gov.br");
    $destinatarios = array();

    $usuarios[] = $usucpfpedido;
    $usuarios = pegaUsuarios($usuarios);

    foreach ($usuarios as $usuario) {
        $destinatarios[] = array("usunome" => $usuario['usunome'], "usuemail" => $usuario['usuemail']);
    }
    echo $assunto;
    echo "<br />";
    echo $conteudo;
    //return enviar_email($remetente, $destinatarios, $assunto, $conteudo);
}

function enviarEmailAnaliseSolicitacao($slcid)
{
    $usuarios = array();
    extract(pegaDadosSolicitacao($slcid));

    $assunto = "SIMEC.OBRAS2 - Pedido de desbloqueio ($deferimento) - Obra $descricao";
    $conteudo = "
            <table>
                <tr><td><b>Pedido de desbloqueio - ($deferimento)</b></td></tr>
                <tr><td>ID obra: $obrid</td></tr>
                <tr><td>Solicitação: $tipo</td></tr>
                <tr><td>Nome da obra: $obrnome</td></tr>
                <tr><td>Esfera: $empesfera</td></tr>
                <tr><td>UF: $estuf</td></tr>
                <tr><td>Municipio: $mundescricao</td></tr>
                <tr><td>$slcjustificativa</td></tr>
                <tr><td>&nbsp;  </td></tr>
                <tr><td>Realizado em $slcdatainclusao por $usunomepedido</td></tr>
            </table>
        ";


    $conteudo .= "
            <table>
            <tr><td>Análise:</td></tr>

            <tr><td>Situação: $deferimento</td></tr>
            ";
    if ($dsltipodesbloqueio == 'D') {
        $conteudo .= "
            <tr><td>Data de início: $dsldatainicio</td></tr>
            <tr><td>Durao: $dsldias dias</td></tr>
            <tr><td>Data de término: $dsltermino</td></tr>
            ";
    }
    $conteudo .= "
            <tr><td>Comentrios: $dslcomentario</td></tr>
            <tr><td>Realizado em $dsldata por $usunomedesbloqueio</td></tr>
            </table>
        ";
    $remetente = array("nome" => "SIMEC - Monitoramento de Obras", "email" => "monitoramento.obras@mec.gov.br");
    $destinatarios = array();

    $usuarios[] = $usucpfpedido;
    $usuarios = pegaUsuarios($usuarios);

    foreach ($usuarios as $usuario) {
        $destinatarios[] = array("usunome" => $usuario['usunome'], "usuemail" => $usuario['usuemail']);
    }
    return enviar_email($remetente, $destinatarios, $assunto, $conteudo);
}

function enviarEmailPedidoDesbloqueio($pdoid)
{
    $usuarios = array('98143760430', '22973613191');
    extract(pegaDadosDesbloqueio($pdoid));

    $assunto = "SIMEC.OBRAS2 - Pedido de desbloqueio - Obra $descricao";
    $conteudo = "
        <table>
            <tr><td><b>Pedido de desbloqueio</b></td></tr>
            <tr><td>ID obra: $obrid</td></tr>
            <tr><td>Nome da obra: $obrnome</td></tr>
            <tr><td>Esfera: $empesfera</td></tr>
            <tr><td>UF: $estuf</td></tr>
            <tr><td>Municipio: $mundescricao</td></tr>
            <tr><td>Justificativa: $pdojustificativa</td></tr>
            <tr><td>&nbsp;  </td></tr>
            <tr><td>Realizado em $pdodatainclusao por $usunomepedido</td></tr>
        </table>
    ";

    $remetente = array("nome" => "SIMEC - Monitoramento de Obras", "email" => "monitoramento.obras@mec.gov.br");
    $destinatarios = array();
    $usuarios = pegaUsuarios($usuarios);

    foreach ($usuarios as $usuario) {
        $destinatarios[] = array("usunome" => $usuario['usunome'], "usuemail" => $usuario['usuemail']);
    }
    return enviar_email($remetente, $destinatarios, $assunto, $conteudo);
}

/**
 * Retorna os dados dos usuarios
 *
 * @param string | array $cpf
 */
function pegaUsuarios($cpf)
{
    global $db;
    if (is_array($cpf)) {
        $where = 'usucpf IN (\'' . implode('\',\'', $cpf) . '\')';
    } else {
        $where = 'usucpf = ' . $cpf;
    }

    $sql = "select * from seguranca.usuario where $where";
    return $db->carregar($sql);
}

function montaPainelSituacaoEstado($post = array())
{
    global $db;
    $where = array();

    extract($post);
    // Programa
    if ($prfid[0] && $prfid_campo_flag) {
        if (!$prfid_campo_excludente) {
            array_push($where, " e.prfid  IN (" . implode(',', $prfid) . ") ");
        } else {
            array_push($where, " ( e.prfid  NOT IN (" . implode(',', $prfid) . ") OR e.prfid is null ) ");
        }
    }
    // Fonte
    if ($tooid[0] && $tooid_campo_flag) {
        if (!$tooid_campo_excludente) {
            array_push($where, " o.tooid  IN (" . implode(',', $tooid) . ") ");
        } else {
            array_push($where, " ( o.tooid  NOT IN (" . implode(',', $tooid) . ") OR o.tooid IS NULL ) ");
        }
    }

    if (!empty($esfera)) {
        array_push($where, " e.empesfera = '{$esfera}' ");
    }

    $colunas = array();
    $colunasSub = array();
    $colunasSub2 = array();

    $estado = new Estado();
    $estados = $estado->recuperarTodos('*', null, 'estuf ASC');

    foreach ($estados as $est) {
        $colunasSub[] = "CASE WHEN pre.estuf IN ('{$est['estuf']}') THEN COUNT(0) END as \"{$est['estuf']}\"";
        $colunas[] = "COALESCE(SUM(\"{$est['estuf']}\"),0) AS \"{$est['estuf']}\"";
    }
    $colunas = implode(', ', $colunas);
    $colunasSub = implode(', ', $colunasSub);
    $colunasSub2 = str_replace('pre.', 'est.', $colunasSub);

    $where_tipologia = '';
    if (isset($post['tpoid'])) {
        $tpoid = $post['tpoid'];
        $mi = false;
        $con = false;

        foreach ($tpoid as $key => $value) {
            if ($value == 'obra_mi') {
                $mi = true;
            }
            if ($value == 'obra_con') {
                $con = true;
            }
        }

        if ($mi == true && $con == false) {
            $where_tipologia = '  (o.tpoid IN (' . TPOID_MI_TIPO_B . ', ' . TPOID_MI_TIPO_C . ') )';
        }
        if ($mi == false && $con == true) {
            $where_tipologia = '  (o.tpoid NOT IN (' . TPOID_MI_TIPO_B . ', ' . TPOID_MI_TIPO_C . ') OR (o.tpoid IS NULL) )';
        }
        if ($mi == true && $con == true) {
            $where_tipologia = '  (o.tpoid IS NOT NULL OR o.tpoid IS NULL )';
        }
        array_push($where, $where_tipologia);
    }

    $sqlUnion = "";

    if (empty($where)) {
        $sqlUnion = "UNION ALL
                        SELECT '683' as esdid, '00-Aguardando empenho' AS situacaoobra, COUNT(0) AS coluna1, 0 as esdordem,
                        {$colunasSub}
                        FROM obras.preobra pre
                        INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
                        INNER JOIN workflow.documento doc ON doc.docid = pre.docid
                        WHERE pre.prestatus = 'A'
                        AND pto.ptoclassificacaoobra = 'P'
                        AND doc.esdid IN (228,360,365,366,367,683,754,755)
                        AND pre.obrid IS NULL
                        GROUP BY situacaoobra, pre.estuf, pre.preid, doc.esdid";
    }


    $sql = "
            SELECT
                esdid,
                situacaoobra,
                {$colunas},
                COALESCE(SUM(coluna1),0) AS coluna1

            FROM (
                SELECT  esd.esdid as esdid, CASE esd.esdid
                        WHEN 693 THEN
                            CASE
                                WHEN DATE_PART('year', o.obrdtvistoria) <= 2010 THEN '01-'||esd.esddsc||'s at 2010'
                                WHEN DATE_PART('year', o.obrdtvistoria) > 2010 THEN '02-'||esd.esddsc||'s aps 2010'
                                WHEN o.obrdtvistoria is null THEN '02-Sem data de concluso'
                            END
                        WHEN 690 THEN --Execução
                            CASE
                                --WHEN COALESCE(o.obrpercentultvistoria,0) > 80 THEN '03-'||esd.esddsc||' (> 80%)'
                                --WHEN COALESCE(o.obrpercentultvistoria,0) <= 80 THEN '04-'||esd.esddsc||' (0% a 80%)'
                                --Modificado para atender a solicitação abaixo
                                --Execução at 25%
                                --Execução de 25% a 50%
                                --Execução de 50% a 75%
                                --Execução acima de 75%
                                WHEN (o.obrpercentultvistoria <= 25 AND o.obrpercentultvistoria >= 0) or  o.obrpercentultvistoria IS NULL     THEN '03-'||esd.esddsc||' (at 25%)'
                                WHEN o.obrpercentultvistoria between 25 and 50                          THEN '04-'||esd.esddsc||' (de 25% a 50%)'
                                WHEN o.obrpercentultvistoria between 50 and 75                          THEN '05-'||esd.esddsc||' (de 50% a 75%)'
                                WHEN o.obrpercentultvistoria > 75                                       THEN '06-'||esd.esddsc||' (acima de 75%)'

                            END
                        WHEN 768 THEN '07-'||esd.esddsc --Em Reformulao
                        WHEN 763 THEN '08-'||esd.esddsc --Licitao
                        WHEN 691 THEN '09-'||esd.esddsc --Paralisada
                        WHEN 769 THEN '10-'||esd.esddsc --Obra Cancelada
                        WHEN 689 THEN '11-'||esd.esddsc --Planejamento pelo proponente
                        WHEN 771 THEN '12-'||esd.esddsc --Aguardando registro de preos
                        ELSE esd.esddsc
                    END AS situacaoobra,
                    COUNT(0) AS coluna1,
                    {$colunasSub2},
                    esd.esdordem


                from obras2.obras o
                inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                inner join workflow.documento d ON d.docid = o.docid
                inner join workflow.estadodocumento esd ON esd.esdid = d.esdid
                inner join entidade.endereco ed ON ed.endid = o.endid AND ed.endstatus = 'A' AND ed.tpeid = 4
                INNER JOIN territorios.municipio   mun ON mun.muncod = ed.muncod
                inner join territorios.estado est ON est.estuf = mun.estuf
                LEFT JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid
                LEFT JOIN obras2.programafonte pf ON pf.prfid = e.prfid

                where o.obrstatus = 'A'
                and e.orgid=3
                -- and d.esdid NOT IN (770) --Etapa Concluda
                and o.obridpai is null
                and o.obrid not in (7828,7829,7840,1000015,1000046,1000049) --Obras de teste
                " . (!empty($where) ? ' AND ' . implode(' AND ', $where) : '') . "


                group by est.estuf, situacaoobra, esd.esdid, esd.esdordem, e.prfid, o.PREID, o.TOOID

                " . $sqlUnion . "

            ) AS FOO
            group by esdordem, situacaoobra, esdid
            order by situacaoobra, esdordem";
//    dbg(simec_htmlentities($sql),d);
    return $db->carregar($sql);
}

/**
 * Funcao para converter "array" de textarea dos questionarios de supervisao mi, empresa e fnde, em "true array" de php.
 */
function conversaoArrayPhpParaSupervisao($post)
{
    $chaves = array_keys($post);
    foreach ($chaves as $key => $value) {
        // qtsobs
        if (strpos($value, 'qtsobs') !== false) {
            $nomeDividido = explode('_', $value);
            $post[$nomeDividido[0]][$nomeDividido[1]] = $post[$value];
            unset($post[$value]);
        }

        // rsqobs
        if (strpos($value, 'rsqobs') !== false) {
            $nomeDividido = explode('_', $value);
            $post[$nomeDividido[0]][$nomeDividido[1]][$nomeDividido[2]] = $post[$value];
            unset($post[$value]);
        }
    }
    return $post;
}

function pegaBotaoOrgaoControleInfo($obrid)
{

    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return '';
    }

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    if ($obra->obrorgcontrole == 't') {
        return "<span style='color:#f00;font-weight:bold;'> Órgão de controle </span>";
    }
}

function pegaBotaoContaBloqueadaInfo($obrid)
{

    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return '';
    }

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    if ($obra->obrcontabloqueada == 't') {
        return "<span style='color:#9A2323;font-weight:bold;'> Conta Bloqueada </span>";
    }
}

function pegaBotaoProcessoAnteriorInfo($obrid)
{

    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return '';
    }

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    if ($obra->obrprocessoanterior == 't') {
        return "<span style='color:#F07C06;font-weight:bold;'> Obra de processo anterior  implantação do sistema </span>";
    }
}


function pegaBotaoSituacao($obrid)
{
    global $db;

    $sql = "SELECT
                        CASE
                            WHEN ed.esdid = " . ESDID_OBJ_CONCLUIDO . "
                            THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
                            WHEN ed.esdid = " . ESDID_OBJ_CANCELADO . "
                            THEN '<font COLOR=\"#000000\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
                            ELSE ed.esddsc
                        END as situacao
                    FROM
                        obras2.obras o
                    JOIN workflow.documento d ON d.docid = o.docid
                    JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                    WHERE o.obrid = {$obrid}
        ";

    $dados = $db->pegaLinha($sql);

    return 'Situação atual: <b>' . $dados["situacao"] . '</b>';
}

function pegaBotaoAtualizacao($obrid)
{
    global $db;

    $sql = "SELECT
                        '<font ' ||
                        CASE
                            WHEN obrdtultvistoria IS NOT NULL AND ed.esdid NOT IN (" . ESDID_OBJ_REGISTRO_PRECO . ") THEN
                                CASE
                                    WHEN staid NOT IN (" . STAID_CONCLUIDO . ", " . STAID_OBRA_CANCELADA . ") THEN
                                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                                    'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada em at 25 dias\">' ||
                                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                                                ELSE
                                                    'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em at 25 dias\">' ||
                                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                                END
                                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                                    'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada entre 25 e 30 dias\">' ||
                                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                                ELSE
                                                    'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 25 e 30 dias\">' ||
                                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                                END
                                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 30 THEN
                                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                                    'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada a mais de 30 dias\">' ||
                                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                                ELSE
                                                    'COLOR=\"#DD0000\" TITLE=\"Esta obra est desatualizada\">'||
                                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                                END
                                        END
                                    WHEN staid IN (" . STAID_CONCLUIDO . ", " . STAID_OBRA_CANCELADA . ") THEN
                                        'COLOR=\"#000000\" >' ||  to_char(obrdtultvistoria, 'DD/MM/YYYY')
                                END
                            ELSE
                                'COLOR=\"#000000\" TITLE=\" \">' ||
                                to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - o.obrdtultvistoria)||' dia(s) )'
                        END
                        || '</FONT>' as ultima_atualizacao
                    FROM
                        obras2.obras o
                    JOIN workflow.documento d ON d.docid = o.docid
                    JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                    WHERE o.obrid = {$obrid}
        ";

    $dados = $db->pegaLinha($sql);

    return 'Última atualizao: <b>' . $dados["ultima_atualizacao"] . '</b>';
}

function pegaBotaoFiscalUnidade($obrid)
{
    global $db;
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    if($obra->empid){
        $sql = "
                    SELECT
                        u.usunome
                    FROM obras2.usuarioresponsabilidade ur
                    INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
                    INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf AND sisid = 147 AND us.susstatus = 'A' AND us.suscod = 'A'
                    INNER JOIN obras2.empreendimento e ON e.empid = {$obra->empid}
                    WHERE
                        ur.rpustatus = 'A' AND
                        u.suscod = 'A' AND
                        ur.pflcod = 948 AND
                        --u.usustatus = 'A' AND
                        ( (ur.empid = {$obra->empid}) )
                ";
        $dado = $db->pegaUm($sql);
    } else {
        $dado = '';
    }
    return 'Fiscal da Unidade: <b>' . $dado . '</b>';
}

function pegaBotaoSupervisao($obrid)
{
    if (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPER_USUARIO)) {
        return $supervisao = '<img src="/imagens/edit_on.gif" title="Supervisão FNDE" /> <a href="obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A" > Supervisão FNDE </a>';
    }
}

function pegaBotaoControleFinanceiro($obrid)
{
    return '
                        <script type="text/javascript">
                            function abreDadoRelCtrlFinanc(){
                                var obrid = ' . $obrid . ';
                                var url = "/obras2/obras2.php?modulo=relatorio/relatorioControleFinanceiro&acao=A" +
                                    "&form=true" +
                                    "&obrid=" + obrid ;
                                popup1 = window.open(
                                    url,
                                    "Relatório de Controle Financeiro",
                                    "width=1200,height=750,scrollbars=yes,scrolling=no,resizebled=no"
                                );

                                return false;
                            }
                        </script>
                        <img src="/imagens/money_g.gif"> <a title="Relatório de Controle Financeiro da Obra (' . $obrid . ')"
                             id="rel_ctrl_financ" href="#" onclick="abreDadoRelCtrlFinanc();">Controle Financeiro</a>
               ';
}

function pegaBotaoSupervisaoEmpresa($obrid)
{
    if (!obraMi($obrid) and (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPER_USUARIO))) {
        return '<a href="obras2.php?modulo=principal/cadVistoriaEmpresa&acao=E"><img src="/imagens/edit_on.gif" title="Supervisão Empresa" /> Supervisão Empresa</a>';
    }
}

function pegaBotaoVinculada($obrid)
{
    global $db;

    $html = '';

    $strSQL = "SELECT COUNT(*) FROM obras2.obras WHERE obrid={$obrid} AND (obridpai IS NOT NULL OR obridvinculado IS NOT NULL)";
    if ((int)$db->pegaUm($strSQL)) {
        $html = '<img src="/imagens/0_ativo.png" border="0" title="Obra vinculada" /> <a href="#" id="link_obr_vincuado" >Obra vinculada</a>';

        $html .= '
                <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
                <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
                <script type="text/javascript">
                  //Forma antiga
                  function abrePopUp(url) {
                    janela = window.open(
                        url,
                        "ObrasVinculadas",
                        "width=1200, height=265, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1"
                    );
                    janela.focus();
                 }

                 jQuery.noConflict();

                 jQuery(document).ready(function() {

                    jQuery("#link_obr_vincuado").click(function(){

                       var url = "obras2.php?modulo=principal/popupObraVinculada&acao=A&show=' . $_GET['acao'] . '";
                       var div_close = \'<div id="span_dialog_lista_vinculado_close" \'+
                                       \'   style=" position: static; \'+
                                       \'           margin-right: 2; \'+
                                       \'           width: 100%; \'+
                                       \'           height: 25px; \'+
                                       \'           text-align: right; \'+
                                       \'           font-color: #fff; \'+
                                       \'           -webkit-border-radius: 7px; \'+
                                       \'           -moz-border-radius: 7px;    \'+
                                       \'            border-radius: 7px;        \'+
                                       \'           background-color:#fff" > \'+
                                         \'   <table class="" bgcolor="#fff" cellSpacing="3" cellPadding="5" align="center" width="100%"> \'+
                                         \'       <tr>                                                                                          \'+
                                         \'           <td class="SubtituloTabela" align="center" style="width:5%;">                             \'+
                                         \'               &nbsp;                                                                                \'+
                                         \'           </td>                                                                                     \'+
                                         \'           <td class="" align="center" style="width:90%;">                            \'+
                                         \'               <b>Obras Vinculadas</b>                                                               \'+
                                         \'           </td>                                                                                     \'+
                                         \'           <td class="SubtituloTabela" align="right" style="width:5%; font-size:15px; color:#ccc; padding-right: 15px;"> \'+
                                         \'               <b><span class="close_div_dialog_lista_vinculado" style="cursor:hand" title="Fechar">X</b> \'+
                                         \'           </td>                                                                                          \'+
                                         \'       </tr>                                                                                              \'+
                                         \'   </table>                                                                                               \'+
                                         \'   <br /> \'+
                                       \'</div>\';

                       var div = \'<div id="div_dialog_lista_vinculado"     \'+
                                 \'     style="display:  none;              \'+
                                 \'            background-color: #fff;   \'+
                                 \'            width:    1300;              \'+
                                 \'            position: absolute;          \'+
                                 \'            box-shadow: 7px 7px 5px #888888; \'+
                                 \'            height:   300;               \'+
                                 \'            padding-top: 10px;              \'+
                                 \'            margin-top: 10px;              \'+
                                 \'            -webkit-border-radius: 7px;  \'+
                                 \'            -moz-border-radius: 7px;     \'+
                                 \'             border-radius: 7px;         \'+
                                 \'            overflow: auto;              \'+
                                 \'            z-index:  900; ">&nbsp;</div>\';

                       var content = "";

                       jQuery( document.body ).append( div );
                       jQuery("#div_dialog_lista_vinculado").append( div_close );

                        if (window.XMLHttpRequest){// code for IE7+, Firefox, Chrome, Opera, Safari
                            xmlhttp=new XMLHttpRequest();
                        }
                        else{// code for IE6, IE5
                            xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
                        }

                        xmlhttp.open("GET",url,false);
                        xmlhttp.send();
                        content=xmlhttp.responseText;

                        content = \'<div id="div_dialog_lista_vinculado_conteudo" style="background-color: #fff; height: 100%; width: 100%; padding-top: 10px; margin-top: 10px;">\'+content+\'</div>\';

                        jQuery("#div_dialog_lista_vinculado").append(content)

                        jQuery("#div_dialog_lista_vinculado").show();

                        var divHeight = 300;
                        var divWidth  = 1300;
                        x = (window.innerWidth)/2 - (divWidth)/2;
                        y = (window.innerHeight)/2 - (divHeight)/2;

                        x = (x<0) ? 0 : x;

                        floatDiv("div_dialog_lista_vinculado", x,y).floatIt();

                        jQuery(".close_div_dialog_lista_vinculado").click(function(){
                            jQuery("#div_dialog_lista_vinculado").remove();
                        });
                    });
                 });

                 var ns = (navigator.appName.indexOf("Netscape") != -1);
                 var d = document;

                 function floatDiv(id, sx, sy){
                        var el=d.getElementById?d.getElementById(id):d.all?d.all[id]:d.layers[id];
                        var px = document.layers ? "" : "px";
                        window[id + "_obj"] = el;
                        if(d.layers)el.style=el;
                        el.cx = el.sx = sx;el.cy = el.sy = sy;
                        el.sP=function(x,y){this.style.left=x+px;this.style.top=y+px;};

                        el.floatIt=function(){
                                var pX, pY;
                                pX = (this.sx >= 0) ? 0 : ns ? innerWidth :
                                document.documentElement && document.documentElement.clientWidth ?
                                document.documentElement.clientWidth : document.body.clientWidth;
                                pY = ns ? pageYOffset : document.documentElement && document.documentElement.scrollTop ?
                                document.documentElement.scrollTop : document.body.scrollTop;
                                if(this.sy<0)
                                pY += ns ? innerHeight : document.documentElement && document.documentElement.clientHeight ?
                                document.documentElement.clientHeight : document.body.clientHeight;
                                this.cx += (pX + this.sx - this.cx)/8;this.cy += (pY + this.sy - this.cy)/8;
                                this.sP(this.cx, this.cy);
                                setTimeout(this.id + "_obj.floatIt()", 40);
                        }
                        return el;
                 }
                </script>
            ';
    }

    return $html;
}

function pegaBotaoSolicitcoes($obrid)
{
    global $db;
    if (!possui_perfil(array(
        PFLCOD_SUPER_USUARIO,
        PFLCOD_ADMINISTRADOR,
        PFLCOD_GESTOR_MEC,
        PFLCOD_GESTOR_UNIDADE,
        PFLCOD_SUPERVISOR_UNIDADE,
        PFLCOD_CALL_CENTER
    ))) {
        return '';
    }

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    if ($obra->obrstatus != 'A') {
        return '';
    }

    $sql = "
                SELECT
                  o.*,
                  e.prfid,
                  ed.muncod,
                  ed.estuf
                FROM obras2.obras o
                JOIN obras2.empreendimento e ON e.empid = o.empid
                JOIN entidade.endereco ed ON ed.endid = e.endid
                WHERE
                  e.prfid IN (41, 54)
                  AND o.obrstatus = 'A'
                  AND o.obridpai IS NULL
                  AND o.obrid IN ($obrid)";
    $dadosObra = $db->pegaLinha($sql);

    $mobiliario = '';
    $desbloqueio = '';
    if ($dadosObra) {
        if (possui_perfil(array(
                PFLCOD_GESTOR_UNIDADE,
                PFLCOD_SUPER_USUARIO,
                PFLCOD_ADMINISTRADOR,
                PFLCOD_GESTOR_MEC
            )) && $dadosObra['obrpercentultvistoria'] >= 20) {

            $sql = "SELECT inuid FROM par3.instrumentounidade WHERE muncod = '{$dadosObra['muncod']}' AND estuf = '{$dadosObra['estuf']}'";
            $inuid_par3 = $db->pegaUm($sql);
            $obrid_par3 = $dadosObra['obrid_par3'];

//            $mobiliario = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return solicitacaoPar3(\'' . $obrid . '\', \'' . $inuid_par3 . '\', \'' . $obrid_par3 . '\');">Solicitar Mobiliário</a></p><p>';
            $mobiliario = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return solicitacaoPar3(\'' . $obrid . '\', ' . $inuid_par3 . ', ' . $obrid_par3 . ');">Solicitar Mobiliário</a></p><p>';

        } else {
            $mobiliario = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município, quando a obra ultrapassar 90% de execução fsica." style="color: #919191; cursor: pointer">Solicitar Mobiliário</span></p><p>';
        }
    }

    if (possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_GESTOR_MEC))) {
        $saldo = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 2);">Solicitar Uso de Saldo</a></p><p>';
        $projeto = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 3);">Solicitar Alteração de Projeto/Serviço</a></p><p>';
        $terreno = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 4);">Solicitar Troca de Terreno</a></p><p>';
        $locacao = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 5);">Solicitar Alteração de Locação</a></p><p>';

        $sql = "select * FROM obras2.controle_cumprimento_objeto cco WHERE ccostatus = 'A' AND obrid = $obrid";
        $data = $db->pegaLinha($sql);
        if ($data) {
            $prorrogacao = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 8);">Solicitar Prorrogação de prazo de Cumprimento de Objeto</a></p><p>';
        }

        $cancelamento = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 9);">Solicitar Cancelamento de Obra</a></p><p>';
        $contacorrente = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 10);">Solicitar Desbloqueio de Conta Corrente</a></p><p>';
        $alteracaofiscal = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 11);">Solicitar ou Alterar Fiscal de Obra, Gestor Unidade, Consulta Unidade</a></p><p>';
    } else {
        $saldo = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar Uso de Saldo</span></p><p>';
        $projeto = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar Alteração de Projeto/Serviço</span></p><p>';
        $terreno = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar Troca de Terreno</span></p><p>';
        $locacao = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar Alteração de Locação</span></p><p>';

        $prorrogacao = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar Prorrogação de prazo de Cumprimento de Objeto</span></p><p>';
        $cancelamento = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar Cancelamento de Obra</span></p><p>';
        $contacorrente = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar Desbloqueio de Conta Corrente</span></p><p>';
        $alteracaofiscal = '<img src="/imagens/editar_conteudo_caixa.png" /> <span title="Solicitação feita atravs do gestor do município." style="color: #919191; cursor: pointer">Solicitar ou Alterar Fiscal de Obra, Gestor Unidade, Consulta Unidade</span></p><p>';
    }


    $obras = new Obras($obrid);
    $doc = wf_pegarDocumento($obras->docid);
    if ($doc['esdid'] == '690' || $doc['esdid'] == '691') {
        $desbloqueio = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 6);">Solicitar Desbloqueio do Cronograma</a></p><p>';
    }


    $crtid = $obras->pegaContratoPorObra($obrid);
    $contrato = new Contrato($crtid);
    $dados = $contrato->getDados();

    //   ver($dados,d);

    if (possui_perfil(array(
            PFLCOD_GESTOR_UNIDADE,
            PFLCOD_SUPER_USUARIO,
            PFLCOD_ADMINISTRADOR,
            PFLCOD_GESTOR_MEC,
            PFLCOD_SUPERVISOR_UNIDADE
        )) and $dados['ttaid']) {
        $cancelar_aditivo = '<img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitacoes(\'' . $obrid . '\', 7);">Solicitar Excluso de Aditivo</a></p><p>';
    }

    return '
                <script type="text/javascript">
                    function abreSolicitacoes(obrid, tipo){
                        var url = "/obras2/obras2.php?modulo=principal/solicitacao&acao=A" +
                            "&obrid=" + obrid +
                            "&tslid[]=" + tipo;
                        popup1 = window.open(
                            url,
                            "solicitarVinculada",
                            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                        );

                        return false;
                    }
                    
                    function solicitacaoPar3(obrid, inuid, obrid_par3 = ""){
                        var url = "/par3/par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=" + inuid +
                        "&aba=cadastro&obrid=" + obrid +
                        "&obrid_par3=" + obrid_par3;
                        window.location.href = url;

                        return false;
                    }
                </script>
                ' . $mobiliario . '
                ' . $saldo . '
                ' . $projeto . '
                ' . $terreno . '
                ' . $locacao . '
                ' . $prorrogacao . '
                ' . $cancelamento . '
                ' . $contacorrente . '
                ' . $desbloqueio . '
                ' . $cancelar_aditivo . '
                ' . $alteracaofiscal . '
                ';


}

function pegaBotaoSolicitarDesembolso($obrid)
{
//        if(!possui_perfil(array(PFLCOD_SUPER_USUARIO)))
//            return '';

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

//caso a obra esteja entre as selecionadas na portria 348 no pode solicitar desembolso
    $obraEstaNaPortaria = obraPortaria($obrid);
    if ($obraEstaNaPortaria) {
        return '<span style="color: #919191; cursor: pointer" title="A obra deve ter o no mnimo uma vistoria." ><img src="/imagens/editar_conteudo_caixa.png" /> Solicitar Desembolso</span>';
    }


    if ($obra->tooid == 2 && possui_perfil(array(
            PFLCOD_SUPER_USUARIO,
            PFLCOD_GESTOR_MEC,
            PFLCOD_GESTOR_UNIDADE,
            PFLCOD_SUPERVISOR_UNIDADE,
            PFLCOD_CALL_CENTER
        ))) {
        $solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio();
        $solicitacaoDesembolso = $solicitacaoDesembolso->pegaProcessoConvPorObra($obrid);

        return '
                    <script type="text/javascript">
                        function abreSolicitarDesembolso(processo, convenio, obrid){
                            var url = "/obras2/obras2.php?modulo=principal/popupSolicitarDesembolsoConvenio&acao=A" + "&processo=" + processo + "&convenio=" + convenio + "&obrid=" + obrid;
                            popup1 = window.open(
                                url,
                                "solicitarVinculada",
                                "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                            );

                            return false;
                        }
                    </script>
                    <img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitarDesembolso(\'' . $solicitacaoDesembolso['pronumeroprocesso'] . '\', \'' . $solicitacaoDesembolso['termo_convenio'] . '\', \'' . $obrid . '\');">Solicitar 6890 Desembolso</a>';
    }


    $supervisao = new Supervisao();
    $supid = ($supid) ? $supid : $supervisao->pegaUltSupidByObra($obrid, array('validadoSupervisor' => true));

    $sql = "SELECT COUNT(*) repasse FROM (
                    SELECT DISTINCT o.obrid FROM obras2.obras o
                    JOIN par.pagamentoobra po ON po.preid = o.preid
                    JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                    WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
                    UNION
                    SELECT DISTINCT o.obrid FROM obras2.obras o
                    JOIN par.pagamentoobrapar po ON po.preid = o.preid
                    JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                    WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
                    UNION
                    SELECT DISTINCT o.obrid FROM obras2.obras o
                    JOIN par3.v_saldo_empenho_composicao_obra emp ON emp.obrid = o.obrid_par3
                    WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
                ) as f WHERE f.obrid = $obrid";

    $repasse = $obra->pegaUm($sql);

    if ($repasse > 0 && $obra->obrpercentultvistoria > 0 && $supid) {
        return '
                    <script type="text/javascript">
                        function abreSolicitarDesembolso(obrid){
                            var url = "/obras2/obras2.php?modulo=principal/popupSolicitarDesembolso&acao=A" +
                                "&obrid=" + obrid;
                            popup1 = window.open(
                                url,
                                "solicitarVinculada",
                                "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                            );

                            return false;
                        }
                    </script>
                    <img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitarDesembolso(\'' . $obrid . '\');">Solicitar Desembolso</a>';
    }

    if (!$supid) {
        return '<span style="color: #919191; cursor: pointer" title="A obra deve ter o no mnimo uma vistoria." ><img src="/imagens/editar_conteudo_caixa.png" /> Solicitar Desembolso</span>';
    }

    return '<span style="color: #919191; cursor: pointer" title="A obra deve ter o percentual acima de 0% e ter recebido o 1 repasse." ><img src="/imagens/editar_conteudo_caixa.png" /> Solicitar Desembolso</span>';
}

function pegaBotaoSolicitarVinculada($obrid)
{
    if (!possui_perfil(array(
        PFLCOD_SUPER_USUARIO,
        PFLCOD_ADMINISTRADOR,
        PFLCOD_GESTOR_MEC,
        PFLCOD_GESTOR_UNIDADE,
        PFLCOD_SUPERVISOR_UNIDADE,
        PFLCOD_CALL_CENTER
    ))) {
        return '';
    }

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    if ($obra->obrstatus != 'A') {
        return '';
    }

    if (verificaObraMiContratoRescindido($obrid)) {
        return '
                <script type="text/javascript">
                    function abreSolicitarVinculada(obrid){
                        var url = "/obras2/obras2.php?modulo=principal/popupSolicitarVinculada&acao=A" +
                            "&obrid=" + obrid;
                        popup1 = window.open(
                            url,
                            "solicitarVinculada",
                            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                        );

                        return false;
                    }
                </script>
                <img src="/imagens/editar_conteudo_caixa.png" /> <a href="#" onclick="javascript: return abreSolicitarVinculada(\'' . $obrid . '\');">Solicitar Vinculada</a>';
    }
    return '<span style="color: #919191; cursor: pointer" title="A obra deve estar paralisada por contrato rescindido para solicitar a criao da vinculada." ><img src="/imagens/editar_conteudo_caixa.png" /> Solicitar Vinculada</span>';
}

function pegaBotaoExtratoObra($obrid)
{
    if (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO)) {
        return '<img src="/imagens/historico.png" /> <a href="obras2.php?modulo=principal/extratoObra&acao=A">Extrato da Obra</a> ';
    }
}

function pegaBotaoPendenciasObra($obrid)
{
    if (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO)) {
        return '<img src="/imagens/editar_nome_vermelho.gif" /> <a href="obras2.php?modulo=relatorio/pendencias&acao=A">Pendências</a>';
    }
}

function pegaBotaoGraficoEvolucao($obrid)
{
    return '
                <script type="text/javascript">
                    function abreEvolucaoFinan( obrid ){
                        janela(\'?modulo=principal/grafico_evolucao_financeira&acao=A&obrid=\'+' . $obrid . ' ,800,650);
                        return false;
                    }
                </script>
                <img src="/imagens/seriehistorica_ativa.gif" title="Evolução Financeira" />
                <a href="" onclick="javascript: return abreEvolucaoFinan(\'' . $obrid . '\');"> Evolução Financeira</a>';
}

function pegaBotaoHistoricoHorkflow($obrid)
{
    $obra = new Obras($obrid);
    return '
                <script type="text/javascript">
                    function wf_exibeHistorico(docid){
                        var url = "http://' . $_SERVER['HTTP_HOST'] . '/geral/workflow/historico.php" +
                            "?modulo=principal/tramitacao" +
                            "&acao=C" +
                            "&docid=" + docid;
                        window.open(
                            url,
                            "alterarEstado",
                            "width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                        );
                        return false;
                    }
                </script>
                <img title="Histórico workflow" src="/imagens/fluxodocm.gif"/> <a href="" onclick="javascript: return wf_exibeHistorico(\'' . $obra->docid . '\');" > Histórico workflow </a>';
}

function pegaBotaoAcessoObra1($obrid)
{
    return '<img src="/imagens/icone_historico.PNG"/> <a href="/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' . $obrid . '">Acessar Obras 1</a>';
}

function pegaBotaoLicitacao($obrid)
{
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    $dados = wf_pegarDocumento($obra->docid);

    if (!possui_perfil(PFLCOD_GESTOR_MEC) && !possui_perfil(PFLCOD_SUPER_USUARIO) && !possui_perfil(PFLCOD_GESTOR_UNIDADE) && !possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) {
        return;
    }

    if ($dados['esdid'] == ESDID_OBJ_PARALISADO || $dados['esdid'] == ESDID_OBJ_CONCLUIDO || $dados['esdid'] == ESDID_OBJ_EXECUCAO || $dados['esdid'] == ESDID_OBJ_CONTRATACAO) {
        return '
            <script type="text/javascript">
                function abreEditarLicitacao(obrid){
                    var url = "/obras2/obras2.php?modulo=principal/popupEditarLicitacao&acao=E" +
                        "&obrid=" + obrid;
                    popup1 = window.open(
                        url,
                        "editarLicitacao",
                        "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }
            </script>
            <img src="/imagens/principal.gif"> <a href="#" onclick="javascript: return abreEditarLicitacao(\'' . $obrid . '\');">Editar Licitao</a>';
    }
}

function pegaBotaoContrato($obrid)
{
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $dados = wf_pegarDocumento($obra->docid);
    $crtid = $obra->pegaContratoPorObra($obrid);

    if (!possui_perfil(PFLCOD_GESTOR_MEC) && !possui_perfil(PFLCOD_SUPER_USUARIO) && !possui_perfil(PFLCOD_GESTOR_UNIDADE) && !possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) {
        return;
    }

    if ($dados['esdid'] == ESDID_OBJ_PARALISADO || $dados['esdid'] == ESDID_OBJ_CONCLUIDO || $dados['esdid'] == ESDID_OBJ_EXECUCAO) {
        if (!empty($crtid)) {
            return '
                        <script type="text/javascript">
                            function abreEditarContrato(obrid){

                                var url = "/obras2/obras2.php?modulo=principal/popupEditarContrato&acao=A" +
                                    "&obrid=" + obrid + "&crtid=" + ' . $crtid . ';
                                popup = window.open(
                                    url,
                                    "editarContrato",
                                    "width=1000,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                                );

                                return false;
                            }
                        </script>
                        <img src="/imagens/principal.gif"> <a href="#" onclick="javascript: return abreEditarContrato(\'' . $obrid . '\');">Editar Contrato</a>';
        } else {
            return '
                        <script type="text/javascript">
                            function abreInserirContrato(obrid){
                                var url = "/obras2/obras2.php?modulo=principal/cadContrato&acao=A" +
                                    "&obrid=" + obrid;
                                window.location = url;
                                return false;
                            }
                        </script>
                        <img src="/imagens/principal.gif"> <a href="#" onclick="javascript: return abreInserirContrato(\'' . $obrid . '\');">Inserir Contrato</a>';
        }

        if (isset($dadosContrato['ttaid'])) {
            return '<img src="/imagens/principal.gif"> <span title="O contrato possui aditivos e no pode ser editado">Editar Contrato</span>';
        }
    }
}

function pegaBotaoAditivo($obrid)
{
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $dados = wf_pegarDocumento($obra->docid);
    $crtid = $obra->pegaContratoPorObra($obrid);

    if (!possui_perfil(PFLCOD_GESTOR_MEC) && !possui_perfil(PFLCOD_SUPER_USUARIO) && !possui_perfil(PFLCOD_GESTOR_UNIDADE) && !possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) {
        return;
    }

    if ($dados['esdid'] == ESDID_OBJ_PARALISADO || $dados['esdid'] == ESDID_OBJ_EXECUCAO && !empty($crtid)) {
        return '
            <script type="text/javascript">
                function abreAditivoContrato(obrid){
                    var url = "/obras2/obras2.php?modulo=principal/popUpInserirAditivoCronograma&acao=A" +
                        "&obrid=" + obrid + "&crtid=" + ' . $crtid . ';
                    popup1 = window.open(
                        url,
                        "editarAditivo",
                        "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }
            </script>
           <img src="/imagens/principal.gif"> <a href="#" onclick="javascript: return abreAditivoContrato(\'' . $obrid . '\');">Inserir aditivo</a>';
    } else {
        return '<img src="/imagens/principal.gif"> <span title="A obra deve estar em Execução ou Paralisada para registrar aditivos ao contrato">Inserir aditivo</span>';
    }
}

function pegaBotaoEditarHistoricoAditivo($obrid)
{
    $obra = new Obras($obrid);
    $crtid = $obra->pegaContratoPorObra($obrid);

    $ctr = new Contrato($crtid);
    $dadosContrato = $ctr->getDados();

    if (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO)) {
        if ($dadosContrato['ttaid']) {
            return "<img src='/imagens/historico.gif'/> <a href=\"javascript:janela('?modulo=principal/historicoAditivo&acao=E&crtid=" . $crtid . "',600,800,'historicoAditivo')\">Histórico do contrato</a>";
        } else {
            return "<img src='/imagens/historico.gif'/> <span title=\"O contrato no possui aditivos\">Histórico do contrato</span>";
        }
    }
}

function pegaBotaoEditarCronograma($obrid)
{
    if (!possui_perfil(array(
        PFLCOD_SUPER_USUARIO,
        PFLCOD_ADMINISTRADOR,
        PFLCOD_GESTOR_MEC,
        PFLCOD_GESTOR_UNIDADE,
        PFLCOD_SUPERVISOR_UNIDADE
    ))) {
        return '';
    }

    return '
        <script type="text/javascript">
            function abrePopupEditaCronograma(obrid){
                var url = "/obras2/obras2.php?modulo=principal/etapas_da_obra&acao=E" + "&obrid=" + obrid;
                popup1 = window.open(
                    url,
                    "editarCronograma",
                    "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                );

                return false;
            }
        </script>
        <img src="/imagens/principal.gif"> <a id="btn_editar_cronograma" onclick="abrePopupEditaCronograma(' . $obrid . ')">Editar Prazos do Cronograma </a> ';
}

function pegaBotaoEvolucaoMi($obrid)
{
    if (obraMi($obrid)) {
        return '<img src="/imagens/edit_on.gif" title="Evolução MI" id="icone_solicitacao_pg"> <a href="obras2.php?modulo=principal/listaEvolucaoMi&acao=A" >Evolução MI</a>';
    }
}

function pegaBotaoOrgaoControle($obrid)
{
    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return '';
    }
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    if ($obra->obrorgcontrole == 't') {
        return '<img src="/imagens/checked.gif" /> <a href="obras2.php?modulo=principal/cadObra&acao=A&requisicao=retirarmarcacaoorgcontrole" >Órgão de controle</a>';
    } else {
        return '<img src="/imagens/check.gif" /> <a href="obras2.php?modulo=principal/cadObra&acao=A&requisicao=inserirrmarcacaoorgcontrole" >Órgão de controle</a>';
    }
}

function pegaBotaoProcessoAnterior($obrid)
{
    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return '';
    }
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    if ($obra->obrprocessoanterior == 't') {
        return ' <img src="/imagens/checked.gif" /> <a href="obras2.php?modulo=principal/cadObra&acao=A&requisicao=retirarmarcacaoprocessoaterior" >Obra de processo anterior  implantação do sistema</a>';
    } else {
        return ' <img src="/imagens/check.gif" /> <a href="obras2.php?modulo=principal/cadObra&acao=A&requisicao=inserirrmarcacaoprocessoanterior" >Obra de processo anterior  implantação do sistema</a>';
    }
}

function pegaBotaoContaBloqueada($obrid)
{
    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return '';
    }
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    if ($obra->obrcontabloqueada == 't') {
        return ' <img src="/imagens/checked.gif" /> <a href="obras2.php?modulo=principal/cadObra&acao=A&requisicao=retirarmarcacaocontabloqueada" >Conta bloqueada</a>';
    } else {
        return ' <img src="/imagens/check.gif" /> <a href="obras2.php?modulo=principal/cadObra&acao=A&requisicao=inserirrmarcacaocontabloqueada" >Conta bloqueada</a>';
    }
}

function pegaBotaoAcompanhamentoMi($obrid)
{
    if (obraMi($obrid)) {
        return '<img src="/imagens/aceite_os_2.png" /> <a href="/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&obrid=' . $obrid . '" >Acompanhamento MI</a>';
    }
}

function pegaBotaoMobiliario($obrid)
{
    if (!possui_perfil(PFLCOD_GESTOR_MEC) && !possui_perfil(PFLCOD_SUPER_USUARIO)) {
        return;
    }

    return '
            <img src="/imagens/icone_mobiliario.gif" title="Mobiliário" align="absmiddle" id="icone_mobiliario"> <a href="#" onclick="abreDadoMobiliario();" >Mobiliário</a>

            <script type="text/javascript">
                function abreDadoMobiliario(){
                    var obrid = ' . $obrid . '
                    var url = "/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A" +
                        "&obrid=" + obrid ;
                    popup1 = window.open(
                        url,
                        "InserirDadoMobiliario",
                        "width=1200,height=750,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }
            </script>';
}

function pegaBotaoOsMi($obrid)
{
    if (obraMi($obrid)) {
        $osMi = new OrdemServicoMI();
        $imgs = $osMi->getBtnSinalizacaoOsMi($obrid);
        return $imgs['imgs_juntas'];
    }
}

function pegaBotaoInconsistenciaSistema($obrid)
{
    if (!possui_perfil(array(
        PFLCOD_SUPER_USUARIO,
        PFLCOD_GESTOR_MEC,
        PFLCOD_GESTOR_UNIDADE,
        PFLCOD_SUPERVISOR_UNIDADE
    ))) {
        return;
    }

    return <<<HTML
<img src="/imagens/editar_conteudo_caixa.png" title="Solicitação para Correção de Inconsistência no sistema"
     id="inconsistencia_sistema" />
    <a href="#" onclick="abreSolicitacoes({$obrid}, 12)">Solicitação para Correção de Inconsistência no sistema</a>
HTML;
}

function pegaBotaoSolicitacaoDesbloqueio($obrid)
{
    $html = '<img src="/imagens/editar_conteudo_caixa.png" title="Solicitação de Desbloqueio de Obra"
     id="inconsistencia_sistema" />
    <a href="#" onclick="abreSolicitacoes(' . $obrid . ', 13)">Solicitação de Desbloqueio de Obra</a>';

    return $html;
}

function pegaBotaoSolicitarNovaPactuacao($obrid)
{

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $dados = wf_pegarDocumento($obra->docid);

    if ($dados['esdid'] == ESDID_OBJ_INACABADA) {
        $html = '<img src="/imagens/editar_conteudo_caixa.png" title="Solicitar Nova Pactuao"
         id="inconsistencia_sistema" />
        <a href="#" onclick="abreSolicitacoes(' . $obrid . ', 14)">Solicitar Nova Pactuao</a>';
    }

    return $html;
}

function pegaBotaoAgrupador($obrid)
{
    global $db;
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $butons1 = array();
    $butons2 = array();
    $butons1[] = pegaBotaoProcessoAnterior($obrid);
    $butons1[] = pegaBotaoContaBloqueada($obrid);
    $butons1[] = pegaBotaoOrgaoControle($obrid);
    $butons1[] = pegaBotaoSupervisao($obrid);
    $butons1[] = pegaBotaoSupervisaoEmpresa($obrid);
    $butons1[] = pegaBotaoVinculada($obrid);
    $butons1[] = pegaBotaoControleFinanceiro($obrid);
    $butons1[] = pegaBotaoExtratoObra($obrid);
    $butons1[] = pegaBotaoPendenciasObra($obrid);
    $butons1[] = pegaBotaoGraficoEvolucao($obrid);
    $butons1[] = pegaBotaoHistoricoHorkflow($obrid);
    $butons1[] = pegaBotaoAcessoObra1($obrid);
    $butons1[] = pegaBotaoLicitacao($obrid);
    $butons1[] = pegaBotaoContrato($obrid);
    $butons1[] = pegaBotaoEditarCronograma($obrid);
    $butons1[] = pegaBotaoAditivo($obrid);
    $butons1[] = pegaBotaoEditarHistoricoAditivo($obrid);
    $butons1[] = pegaBotaoEvolucaoMi($obrid);
    $butons1[] = pegaBotaoAcompanhamentoMi($obrid);
    $butons1[] = pegaBotaoMobiliario($obrid);
    $butons2[] = pegaBotaoSolicitarDesembolso($obrid);
    $butons2[] = pegaBotaoSolicitarVinculada($obrid);
    $butons2[] = pegaBotaoSolicitcoes($obrid);
    $butons2[] = pegaBotaoOsMi($obrid);
    $butons2[] = pegaBotaoInconsistenciaSistema($obrid);
    //$butons2[] = pegaBotaoSolicitacaoDesbloqueio($obrid);
    $butons2[] = pegaBotaoSolicitarNovaPactuacao($obrid);

    $butons1 = array_filter($butons1);
    $butons2 = array_filter($butons2);

    $butons1 = implode(' </p><p> ', $butons1);
    $butons2 = implode(' </p><p> ', $butons2);

    $butons = '<div class="coluna-lista-opcoes"><p><b>Opções:</b></p><p>' . $butons1 . '</div><div class="coluna-lista-opcoes"><p><b>Solicitações:</b></p><p>' . $butons2 . '</div>';

    $seloMi = (obraMi($obrid)) ? '<div style="position: absolute; top: 58px; right: 0px; z-index:1;">
                    <img border="0" title="Obra de Metodologia Inovadora." src="../imagens/carimbo-mi.png">
                </div>' : '';

    $file = new FilesSimec();
    $arqidPortaria = obraPortaria($obrid);
    $seloPortaria = ($arqidPortaria != '') ? '<div style="position: absolute; top: 58px; right: 0px; z-index:1;">
                  <img border="0" width="100px" height="100px" title="Portaria 348/2016 Planejamento."  src="data:image/png;base64,' . $file->getImgArquivo($arqidPortaria) . '">
                </div>' : '';


    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    $aprovacaoCond = ($obra->verificaAprovacaoCondicional($obra->obrid)) ?
        '
                <div style="width: 200px; color: green; font-weight: bold; font-size: 14px; opacity: 0.4; position: absolute; top: 58px; right: 0px; z-index:1;">
                    [<img border="0" title="Obra de Metodologia Inovadora." src="../imagens/check_p.gif"> Aprovao Condicional ]
                </div>' : '';


    $html = '
            <style>
                #opcao{
                    position: relative;
                }
                #lista-opcoes{
                    position: absolute;
                    width: 380px;
                    z-index:100;
                    background-color: #FFFFCC;
                    border: 1px solid #DCA;
                    box-shadow: 5px 5px 8px #CCC;
                    border-radius: 4px;
                    text-align: left;
                    right: -4px;
                    top: 19px;
                    display:none;
                    margin-bottom: 16px;
                }
                .coluna-lista-opcoes{
                    width: 50%;
                    float: left;
                }
                #lista-opcoes p {
                    margin: 5px 5px 10px 5px;
                }
            </style>
            <script type="text/javascript">
                jQuery(function(){
                    jQuery("body").click(function(event) {
                        var target = jQuery(event.target);
                        if (target.parents("#lista-opcoes").length == 0 && target.attr("id") !== "lista-opcoes" ) {
                          jQuery( "#lista-opcoes" ).hide(100);
                        }
                    });
                    jQuery("#opcao span").click(function(event){
                        jQuery( "#lista-opcoes" ).toggle(100);
                        event.stopPropagation();
                    });
                });
            </script>
            <span id="opcao">
                <span style="cursor: pointer"><img title="Lista de Opções" style="width: 17px;" src="/imagens/lista-opcoes.png" /> <b>Lista de Opções </b></span>
                <div id="lista-opcoes">
                    <p>' . $butons . '</p>
                </div>
                ' . $seloMi . '
                ' . $seloPortaria . '
                ' . $aprovacaoCond . '
            </span>
        ';

    return $html;
}

function montaBarraFerramentasTooltip($obrid)
{
    global $db;

    if (empty($obrid)) {
        return '';
    }

    $butons = array();
    $butonsLeft[] = pegaBotaoOrgaoControleInfo($obrid);
    $butonsLeft[] = pegaBotaoContaBloqueadaInfo($obrid);
    $butonsLeft[] = pegaBotaoProcessoAnteriorInfo($obrid);

    $butons[] = pegaBotaoSituacao($obrid);
    $butons[] = pegaBotaoAtualizacao($obrid);
    $butons[] = pegaBotaoFiscalUnidade($obrid);
    $butons[] = pegaBotaoAgrupador($obrid);

    $butons = array_filter($butons);
    $butons = implode(' | ', $butons);

    $butonsLeft = array_filter($butonsLeft);
    $butonsLeft = implode(' | ', $butonsLeft);
    $butonsLeft = '<div style="float:left">' . $butonsLeft . '</div>';

    $html = '
            <tr id="barra-ferramenta-obra">
                <td style="text-align:right; font-size:12px; background-color:#E0EEEE" colspan="2">
                    ' . $butonsLeft . '' . $butons . '
                </td>
            </tr>

        ';


    return $html;
}

function montaBarraFerramentas($obrid)
{
    global $db;

    if (empty($obrid)) {
        return '';
    }

    $docid = pegaDocidObra($obrid);
    $obra = new Obras($obrid);
    $crtid = $obra->pegaContratoPorObra($obrid);
    $contrato = '';
    $aditivo = '';


    $ctr = new Contrato($crtid);
    $dadosContrato = $ctr->getDados();

    $select = "
        CASE
            WHEN ed.esdid = " . ESDID_OBJ_CONCLUIDO . "
            THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
            WHEN ed.esdid = " . ESDID_OBJ_CANCELADO . "
            THEN '<font COLOR=\"#000000\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
            ELSE ed.esddsc
        END as situacao,
        '<font ' ||
        CASE
            WHEN obrdtultvistoria IS NOT NULL AND ed.esdid NOT IN (" . ESDID_OBJ_REGISTRO_PRECO . ") THEN
                CASE
                    WHEN staid NOT IN (" . STAID_CONCLUIDO . ", " . STAID_OBRA_CANCELADA . ") THEN
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                    'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada em at 25 dias\">' ||
                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                                ELSE
                                    'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em at 25 dias\">' ||
                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                END
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                    'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada entre 25 e 30 dias\">' ||
                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                ELSE
                                    'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 25 e 30 dias\">' ||
                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                END
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 30 THEN
                                CASE WHEN obrpercentultvistoria >= 100.00 THEN
                                    'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada a mais de 30 dias\">' ||
                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                ELSE
                                    'COLOR=\"#DD0000\" TITLE=\"Esta obra est desatualizada\">'||
                                    COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'),
                                    to_char(obrdtultvistoria, 'DD/MM/YYYY'))||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s)) '
                                END
                        END
                    WHEN staid IN (" . STAID_CONCLUIDO . ", " . STAID_OBRA_CANCELADA . ") THEN
                        'COLOR=\"#000000\" >'
                END
            ELSE
                'COLOR=\"#000000\" TITLE=\" \">' ||
                to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - o.obrdtultvistoria)||' dia(s) )'
        END
        || '</FONT>' as ultima_atualizacao,
        ed.esdid
        ";

    $sql = "SELECT
        DISTINCT
            {$select}
        FROM
            obras2.obras o
        LEFT JOIN entidade.endereco      ende ON ende.endid     = o.endid AND
                                                 ende.endstatus = 'A' AND
                                                 ende.tpeid     = " . TIPO_ENDERECO_OBJETO . "
        LEFT JOIN territorios.municipio   mun ON mun.muncod     = ende.muncod
        LEFT JOIN workflow.documento        d ON d.docid        = o.docid AND tpdid = " . TPDID_OBJETO . "
        LEFT JOIN workflow.estadodocumento ed ON ed.esdid       = d.esdid
        LEFT JOIN obras2.empreendimento     e ON e.empid        = o.empid AND e.empstatus = 'A'

        WHERE
            o.obrstatus = 'A' AND o.obridpai IS NULL AND e.orgid IN(3) AND o.obrid = {$obrid}
        ORDER BY
            2";

    $dados = $db->pegaLinha($sql);

    $mi = '';
    if ($obra->tpoid == 104 || $obra->tpoid == 105) {
        $mi = '<div style="color: red; position: absolute; top: 290px; right: 60px; float: right; z-index:1;">
                    <img border="0" title="Obra de Metodologia Inovadora." src="../imagens/carimbo-mi.png">
                </div>';
    }

    if ($obra->verificaAprovacaoCondicional($obra->obrid)) {
        $mi .= '<div style="color: green; position: absolute; top: 408px; right: 23px; float: right; z-index:1; font-weight: bold; font-size: 14px; opacity: 0.4">
                    <!--<img border="0" title="Obra de Metodologia Inovadora." src="../imagens/carimbo-mi.png">-->
                    [<img border="0" title="Obra de Metodologia Inovadora." src="../imagens/check_p.gif"> Aprovao Condicional ]
                </div>';
    }

    // supervisao fnde
    $supervisao = '<a href="obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A" >Supervisão FNDE <img src="/imagens/edit_on.gif"
                                                title="Supervisão FNDE" /></a> | ';
    // supervisao empresa
    if (false === strpos($_SERVER['REQUEST_URI'], 'fotos_terrenos')) {
        // No mostrar a Supervisão Empresa para obras MI. Mostrar apenas a Evolução MI
        if ($obra->tpoid != 104 && $obra->tpoid != 105) {
            if (empty($_SESSION['obras2']['empid'])) {
                $objObr = new Obras($obrid);
                $empid = $objObr->empid;
            } else {
                $empid = $_SESSION['obras2']['empid'];
            }

            if (empty($empid)) {
                $sueid = false;
            } else {
                $sql = "
                    select sueid
                    from obras2.supervisaoempresa
                    where suestatus = 'A' AND empid = {$empid}
                    order by sueid desc
                     ";
                $sueid = $db->pegaUm($sql);
            }

            if ($sueid) {
                $supervisao .= '<a href="obras2.php?modulo=principal/cadVistoriaEmpresa&acao=E&sueid=' . $sueid . '" >Supervisão Empresa <img src="/imagens/edit_on.gif"
                                                        title="Supervisão Empresa" /></a> | ';
            }
        }
    } else { // -- link de supervisão mostrado em obras2/geta/fotos_terrenos.php
        // No mostrar a Supervisão Empresa para obras MI. Mostrar apenas a Evolução MI
        if ($obra->tpoid != 104 && $obra->tpoid != 105) {
            // supervisao empresa
            $supervisao .= '<a href="obras2.php?modulo=principal/cadVistoriaEmpresa&acao=E" >Supervisão Empresa <img src="/imagens/edit_on.gif"
                                                        title="Supervisão Empresa" /></a> | ';
        }
    }


    if (verificaObraMiContratoRescindido($obrid)) {
        $vinculada = ' <a href="#" onclick="javascript: return abreSolicitarVinculada(\'' . $obrid . '\');">Solicitar Vinculada</a> | ';
    }

    if (obraMi($obrid)) {
        $acompanhamento = ' | <a href="/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&obrid=' . $obrid . '" >Acompanhamento MI</a>';
    }

    if ($dados['esdid'] == ESDID_OBJ_PARALISADO || $dados['esdid'] == ESDID_OBJ_CONCLUIDO || $dados['esdid'] == ESDID_OBJ_EXECUCAO || $dados['esdid'] == ESDID_OBJ_CONTRATACAO) {
        $licitacao = ' | <a href="#" onclick="javascript: return abreEditarLicitacao(\'' . $obrid . '\');">Editar Licitao</a>';
    }

    if ($dados['esdid'] == ESDID_OBJ_PARALISADO || $dados['esdid'] == ESDID_OBJ_CONCLUIDO || $dados['esdid'] == ESDID_OBJ_EXECUCAO) {
        if (!empty($crtid)) {
            $contrato = ' | <a href="#" onclick="javascript: return abreEditarContrato(\'' . $obrid . '\');">Editar Contrato</a>';
        } else {
            $contrato = ' | <a href="#" onclick="javascript: return abreInserirContrato(\'' . $obrid . '\');">Inerir Contrato</a>';
        }

        if (isset($dadosContrato['ttaid'])) {
            $contrato = ' | <span title="O contrato possui aditivos e no pode ser editado">Editar Contrato</span>';
        }
    }

    if ($dados['esdid'] == ESDID_OBJ_PARALISADO || $dados['esdid'] == ESDID_OBJ_EXECUCAO && !empty($crtid)) {
        $aditivo = ' | <a href="#" onclick="javascript: return abreAditivoContrato(\'' . $obrid . '\');">Inserir aditivo</a>';
    } else {
        $aditivo = ' | <span title="A obra deve estar em Execução ou Paralisada para registrar aditivos ao contrato">Inserir aditivo</a>';
    }

    if (!possui_perfil(PFLCOD_GESTOR_MEC) && !possui_perfil(PFLCOD_SUPER_USUARIO) && !possui_perfil(PFLCOD_GESTOR_UNIDADE) && !possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) {
        $licitacao = '';
        $contrato = '';
        $aditivo = '';
    }

    $extrato_obra = '';
    if (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO)) {
        $extrato_obra = '<a href="obras2.php?modulo=principal/extratoObra&acao=A">Extrato da Obra</a> | ';
        $extrato_obra .= '<a href="obras2.php?modulo=relatorio/pendencias&acao=A">Pendências</a> | ';
    }

    $historico_aditivo = '';
    if (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO)) {
        if ($dadosContrato['ttaid']) {
            $historico_aditivo = "| <a href=\"javascript:janela('?modulo=principal/historicoAditivo&acao=E&crtid=" . $crtid . "',600,800,'historicoAditivo')\">Histórico do contrato</a>";
        } else {
            $historico_aditivo = "| <span title=\"O contrato no possui aditivos\">Histórico do contrato</span>";
        }
    }
    $mobiliario = '
                    |
            <img src="/imagens/icone_mobiliario.gif"
                 title="Mobiliário"
                 align="absmiddle"
                 id="icone_mobiliario"
                 onclick="abreDadoMobiliario();"
                 style="cursor: pointer;">
            <script type="text/javascript">
                function abreDadoMobiliario(){
                    var obrid = ' . $obrid . '
                    var url = "/obras2/obras2.php?modulo=principal/popUpInserirDadoMobiliario&acao=A" +
                        "&obrid=" + obrid ;
                    popup1 = window.open(
                        url,
                        "InserirDadoMobiliario",
                        "width=1200,height=750,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }
            </script>
               ';

    $mi_visu_os = '';
    $btn_emi = '';

    if ($obra->tpoid == 104 || $obra->tpoid == 105) {
        $osMi = new OrdemServicoMI();
        $imgs = $osMi->getBtnSinalizacaoOsMi($obrid);
        $mi_visu_os = ' | ' . $imgs['imgs_juntas'];

        $btn_emi = '
                    |&nbsp;
                    <a href="obras2.php?modulo=principal/listaEvolucaoMi&acao=A" > Evolução MI
                     <img src="/imagens/edit_on.gif"
                          title="Evolução MI"
                          align="absmiddle"
                          id="icone_solicitacao_pg"
                          style="cursor: pointer;"></a>&nbsp;';
    }


    if (!possui_perfil(PFLCOD_GESTOR_MEC) && !possui_perfil(PFLCOD_SUPER_USUARIO)) {
        $supervisao = '';
        $mobiliario = '';
    }

    $crtid = (empty($crtid)) ? 0 : $crtid;

    $strSQL = "SELECT COUNT(*) FROM obras2.obras WHERE obrid={$obrid} AND (obridpai IS NOT NULL OR obridvinculado IS NOT NULL)";
    if ((int)$db->pegaUm($strSQL)) {
        $isObridVinculado = ' &nbsp;&nbsp;
                              <img src="/imagens/0_ativo.png" border="0" title="Obra vinculada" /> &nbsp;
                              <!--<a href=\'javascript: abrePopUp("obras2.php?modulo=principal/popupObraVinculada&acao=A");\' style=\'color:#DD0000;font-weight:bold;\'>Obra vinculada</a>&nbsp;|&nbsp;-->
                              <a href="#" style="color:#DD0000;font-weight:bold;" id="link_obr_vincuado" >Obra vinculada</a> &nbsp;|&nbsp;
                              ';

        echo '
            <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
            <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
            <script type="text/javascript">
              //Forma antiga
              function abrePopUp(url) {
                janela = window.open(
                    url,
                    "ObrasVinculadas",
                    "width=1200, height=265, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1"
                );
                janela.focus();
             }

             jQuery.noConflict();

             jQuery(document).ready(function() {

                jQuery("#link_obr_vincuado").click(function(){

                   var url = "obras2.php?modulo=principal/popupObraVinculada&acao=A&show=' . $_GET['acao'] . '";
                   var div_close = \'<div id="span_dialog_lista_vinculado_close" \'+
                                   \'   style=" position: static; \'+
                                   \'           margin-right: 2; \'+
                                   \'           width: 100%; \'+
                                   \'           height: 25px; \'+
                                   \'           text-align: right; \'+
                                   \'           font-color: #fff; \'+
                                   \'           -webkit-border-radius: 7px; \'+
                                   \'           -moz-border-radius: 7px;    \'+
                                   \'            border-radius: 7px;        \'+
                                   \'           background-color:#fff" > \'+
                                     \'   <table class="" bgcolor="#fff" cellSpacing="3" cellPadding="5" align="center" width="100%"> \'+
                                     \'       <tr>                                                                                          \'+
                                     \'           <td class="SubtituloTabela" align="center" style="width:5%;">                             \'+
                                     \'               &nbsp;                                                                                \'+
                                     \'           </td>                                                                                     \'+
                                     \'           <td class="" align="center" style="width:90%;">                            \'+
                                     \'               <b>Obras Vinculadas</b>                                                               \'+
                                     \'           </td>                                                                                     \'+
                                     \'           <td class="SubtituloTabela" align="right" style="width:5%; font-size:15px; color:#ccc; padding-right: 15px;"> \'+
                                     \'               <b><span class="close_div_dialog_lista_vinculado" style="cursor:hand" title="Fechar">X</b> \'+
                                     \'           </td>                                                                                          \'+
                                     \'       </tr>                                                                                              \'+
                                     \'   </table>                                                                                               \'+
                                     \'   <br /> \'+
                                   \'</div>\';

                   var div = \'<div id="div_dialog_lista_vinculado"     \'+
                             \'     style="display:  none;              \'+
                             \'            background-color: #fff;   \'+
                             \'            width:    1300;              \'+
                             \'            position: absolute;          \'+
                             \'            box-shadow: 7px 7px 5px #888888; \'+
                             \'            height:   300;               \'+
                             \'            padding-top: 10px;              \'+
                             \'            margin-top: 10px;              \'+
                             \'            -webkit-border-radius: 7px;  \'+
                             \'            -moz-border-radius: 7px;     \'+
                             \'             border-radius: 7px;         \'+
                             \'            overflow: auto;              \'+
                             \'            z-index:  900; ">&nbsp;</div>\';

                   var content = "";

                   jQuery( document.body ).append( div );
                   jQuery("#div_dialog_lista_vinculado").append( div_close );

                    if (window.XMLHttpRequest){// code for IE7+, Firefox, Chrome, Opera, Safari
                        xmlhttp=new XMLHttpRequest();
                    }
                    else{// code for IE6, IE5
                        xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
                    }

                    xmlhttp.open("GET",url,false);
                    xmlhttp.send();
                    content=xmlhttp.responseText;

                    content = \'<div id="div_dialog_lista_vinculado_conteudo" style="background-color: #fff; height: 100%; width: 100%; padding-top: 10px; margin-top: 10px;">\'+content+\'</div>\';

                    jQuery("#div_dialog_lista_vinculado").append(content)

                    jQuery("#div_dialog_lista_vinculado").show();

                    var divHeight = 300;
                    var divWidth  = 1300;
                    x = (window.innerWidth)/2 - (divWidth)/2;
                    y = (window.innerHeight)/2 - (divHeight)/2;

                    x = (x<0) ? 0 : x;

                    floatDiv("div_dialog_lista_vinculado", x,y).floatIt();

                    jQuery(".close_div_dialog_lista_vinculado").click(function(){
                        jQuery("#div_dialog_lista_vinculado").remove();
                    });
                });
             });

             var ns = (navigator.appName.indexOf("Netscape") != -1);
             var d = document;

             function floatDiv(id, sx, sy){
                    var el=d.getElementById?d.getElementById(id):d.all?d.all[id]:d.layers[id];
                    var px = document.layers ? "" : "px";
                    window[id + "_obj"] = el;
                    if(d.layers)el.style=el;
                    el.cx = el.sx = sx;el.cy = el.sy = sy;
                    el.sP=function(x,y){this.style.left=x+px;this.style.top=y+px;};

                    el.floatIt=function(){
                            var pX, pY;
                            pX = (this.sx >= 0) ? 0 : ns ? innerWidth :
                            document.documentElement && document.documentElement.clientWidth ?
                            document.documentElement.clientWidth : document.body.clientWidth;
                            pY = ns ? pageYOffset : document.documentElement && document.documentElement.scrollTop ?
                            document.documentElement.scrollTop : document.body.scrollTop;
                            if(this.sy<0)
                            pY += ns ? innerHeight : document.documentElement && document.documentElement.clientHeight ?
                            document.documentElement.clientHeight : document.body.clientHeight;
                            this.cx += (pX + this.sx - this.cx)/8;this.cy += (pY + this.sy - this.cy)/8;
                            this.sP(this.cx, this.cy);
                            setTimeout(this.id + "_obj.floatIt()", 40);
                    }
                    return el;
             }
            </script>

            ';
    } else {
        $isObridVinculado = '';
    }

    $rel_ctrl_financ = '
                        <script type="text/javascript">
                            function abreDadoRelCtrlFinanc(){
                                var obrid = ' . $obrid . ';
                                var url = "/obras2/obras2.php?modulo=relatorio/relatorioControleFinanceiro&acao=A" +
                                    "&form=true" +
                                    "&obrid=" + obrid ;
                                popup1 = window.open(
                                    url,
                                    "Relatório de Controle Financeiro",
                                    "width=1200,height=750,scrollbars=yes,scrolling=no,resizebled=no"
                                );

                                return false;
                            }
                        </script>
                        <img src="/imagens/money_g.gif"
                             title="Relatório de Controle Financeiro da Obra (' . $obrid . ')"
                             align="absmiddle"
                             id="rel_ctrl_financ"
                             onclick="abreDadoRelCtrlFinanc();"
                             style="cursor: pointer;"> |
               ';

    $editar_cronograma = ' | <span id="btn_editar_cronograma" onclick="abrePopupEditaCronograma(' . $obrid . ')" style="cursor: pointer; color: #133368"> Editar Prazos do Cronograma </span> ';


    $html = '
        <tr id="barra-ferramenta-obra">
            <script type="text/javascript">

                function abreEvolucaoFinan( obrid ){
                    janela(\'?modulo=principal/grafico_evolucao_financeira&acao=A&obrid=\'+' . $obrid . ' ,800,650);
                    return false;
                }
                function wf_exibeHistorico(docid){
                    var url = "http://' . $_SERVER['HTTP_HOST'] . '/geral/workflow/historico.php" +
                        "?modulo=principal/tramitacao" +
                        "&acao=C" +
                        "&docid=" + docid;
                    window.open(
                        url,
                        "alterarEstado",
                        "width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );
                    return false;
                }

                function abreInserirContrato(obrid){
                    var url = "/obras2/obras2.php?modulo=principal/cadContrato&acao=A" +
                        "&obrid=" + obrid;
                    window.location = url;
                    return false;
                }


                function abreEditarContrato(obrid){

                    var url = "/obras2/obras2.php?modulo=principal/popupEditarContrato&acao=A" +
                        "&obrid=" + obrid + "&crtid=" + ' . $crtid . ';
                    popup = window.open(
                        url,
                        "editarContrato",
                        "width=1000,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }
                function abreAditivoContrato(obrid){
                    var url = "/obras2/obras2.php?modulo=principal/popUpInserirAditivoCronograma&acao=A" +
                        "&obrid=" + obrid + "&crtid=" + ' . $crtid . ';
                    popup1 = window.open(
                        url,
                        "editarAditivo",
                        "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }

                function abreEditarLicitacao(obrid){
                    var url = "/obras2/obras2.php?modulo=principal/popupEditarLicitacao&acao=E" +
                        "&obrid=" + obrid;
                    popup1 = window.open(
                        url,
                        "editarLicitacao",
                        "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }

                function abreSolicitarVinculada(obrid){
                    var url = "/obras2/obras2.php?modulo=principal/popupSolicitarVinculada&acao=A" +
                        "&obrid=" + obrid;
                    popup1 = window.open(
                        url,
                        "solicitarVinculada",
                        "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }

                function abrePopupEditaCronograma(obrid){
                    var url = "/obras2/obras2.php?modulo=principal/etapas_da_obra&acao=E" + "&obrid=" + obrid;
                    popup1 = window.open(
                        url,
                        "editarCronograma",
                        "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
                    );

                    return false;
                }

//                function checaPopup(){
//                    if(window.opener){
//                        document.getElementById(\'barra-ferramenta-obra\').style.display = "none";
//                    }
//                };


               // checaPopup();
            </script>


            <td style="text-align:right; font-size:10px; background-color:#E0DFDF" colspan="2">
                ' . $orgcontrole . ' ' . $mi . '
                Situação atual: <b>' . $dados["situacao"] . '</b> |
                Última atualizao: <b>' . $dados["ultima_atualizacao"] . '</b> |
                ' . $supervisao . '
                ' . $isObridVinculado . '
                ' . $vinculada . '
                ' . $rel_ctrl_financ . '
                ' . $extrato_obra . '
                <a href="" onclick="javascript: return abreEvolucaoFinan(\'' . $obrid . '\');"> <!--Grfico evolução--> <img
                                                src="/imagens/seriehistorica_ativa.gif"
                                                title="Evolução Financeira" /></a> |
                <a href="" onclick="javascript: return wf_exibeHistorico(\'' . $docid . '\');" ><!--Histórico workflow-->
                    <img title="Histórico workflow" style="position: relative; top: 3px;" src="/imagens/fluxodocm.gif"/></a> |
                <a href="/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' . $obrid . '">Acessar Obras 1</a>
                ' . $licitacao . '
                ' . $contrato . '
                ' . $editar_cronograma . '
                ' . $aditivo . '
                ' . $historico_aditivo . '
                ' . $btn_emi . '
                ' . $acompanhamento . '
                ' . $mobiliario . '
                ' . $mi_visu_os . '
            </td>
        </tr>
    ';
    return $html;
}

/** Alteração feita para atender o Relatório geral * */

/**
 * Pega o rgo que o usurio possui responsabilidade
 *
 * @author Fernando Arajo Bagno da Silva
 * @return mixed
 *
 */
function obras_pegarOrgaoPermitido()
{

    global $db;
    static $orgao = null;

    if ($orgao === null) {
        if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()) {
            // pega todos os orgos
            $sql = " SELECT
                        o.orgdesc as descricao,
                    o.orgid as id,
'/obras/obras.php?modulo=inicio&acao=A&org=' || o.orgid as link
             FROM
            obras.orgao o
                     ORDER BY
            o.orgid";
        } else {
            $sql = "SELECT DISTINCT
                            coalesce(o.orgdesc, coalesce(o3.orgdesc,o2.orgdesc))                        as descricao,
                            coalesce(o.orgid, coalesce(o3.orgid,o2.orgid))                              as id,
'/obras/obras.php?modulo=inicio&acao=A&org=' || coalesce(o.orgid, o2.orgid) as link,
ur.pflcod as perfil
                    FROM
                            obras.usuarioresponsabilidade ur

                    -- Por obra
                    LEFT JOIN
                            obras.obrainfraestrutura oi ON oi.obrid = ur.obrid
                    LEFT JOIN
                            obras.orgao               o ON ur.orgid = o.orgid OR
                                                            o.orgid = oi.orgid
                    --Por Entidade
                    LEFT JOIN
                            entidade.entidade       en ON ur.entid = en.entid
                    LEFT JOIN
                            entidade.funcaoentidade ef ON ef.entid = en.entid
                    LEFT JOIN
                            obras.orgaofuncao       of ON ef.funid = of.funid
                    LEFT JOIN
                            obras.orgao             o2 ON of.orgid = o2.orgid

                    --Por Orgid
                    LEFT JOIN
                            obras.orgao o3 ON ur.orgid = o3.orgid

                    --Trata seguranca
                    LEFT JOIN
                            seguranca.perfil p ON ur.pflcod = p.pflcod
                    LEFT JOIN
                            seguranca.perfilusuario pu ON pu.pflcod = ur.pflcod AND pu.usucpf = ur.usucpf
                    WHERE
                            ur.usucpf    = '{$_SESSION["usucpf"]}' AND
                            ur.rpustatus = 'A' AND
                            p.sisid      = 15";
        }

        $orgao = $db->carregar($sql);
    }
    return $orgao;
}

/**
 * Funao que monta o sql para trazer o relatório geral de obras - Funo no usada
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return string
 */
function obras_monta_sql_relatio()
{

    $where = array();

    extract($_REQUEST);

    $selectTerritorios = "territorios.municipio ";

    if (in_array("tipomun", $agrupador)) {
        if ($selectTerritorios == "territorios.municipio ") {
            $selectTerritorios = "( SELECT
                                            tm.muncod, tm.mundescricao, gt.gtmid, gt.gtmdsc, tpm.tpmdsc
                                        FROM
                                            territorios.municipio tm
                                        INNER JOIN
                                            territorios.muntipomunicipio mtm ON mtm.muncod = tm.muncod
                                        INNER JOIN
                                            territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid
                                        INNER JOIN
                                            territorios.grupotipomunicipio gt ON gt.gtmid = tpm.gtmid
                                        WHERE
                                            tpm.gtmid = 5 AND gt.gtmid = 5 )";
        }

        $selectTipoMun = "CASE WHEN tm.gtmid  = 5 THEN tm.tpmdsc ELSE 'Outros' END as tipomun, ";
        $dadosTipoMun = "tipomun,";
        $groupByTipoMun = "tm.tpmdsc,";
        if (!$groupByGtmid) {
            $groupByGtmid = "tm.gtmid, ";
        }
    }

    // Obras
    if ($_SESSION['obras']['obrid_mapa']) {
        array_push($where, " oi.obrid in (" . implode(',', $_SESSION['obras']['obrid_mapa']) . ") ");
    }

    // tipo de ensino
    if ($orgid) {
        array_push($where, " oi.orgid in (" . implode(',', $orgid) . ") ");
    }

    if ($painel) {
        if ($prfid) {
            array_push($where, " oi.prfid IN (" . $prfid . ") ");
        }

        if ($painel == 2) {
            $percentualinicial = 0;
            $percentualfinal = 80;
        }
        if ($painel == 3) {
            $percentualinicial = 81;
            $percentualfinal = 100;
        }
        if ($tooid == 'pac') {
            array_push($where, "  oi.tooid IN (1) ");
        }
        if ($tooid == 'prepac') {
            array_push($where, "  oi.tooid IN (2,4) ");
        }
        if ($tooid == 1) {
            array_push($where, "  oi.tooid IN (1,2,4) ");
        }
    }

    // regio
    if ($regiao[0] && $regiao_campo_flag) {
        array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode(
            "','",
            $regiao
        ) . "') ");
    }

    // mesoregio
    if ($mesoregiao[0] && $mesoregiao_campo_flag) {
        array_push(
            $where,
            " me.mescod " . (!$mesoregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode(
                "','",
                $mesoregiao
            ) . "') "
        );
    }

    // microregio
    if ($microregiao[0] && $microregiao_campo_flag) {
        array_push(
            $where,
            " mi.miccod " . (!$microregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode(
                "','",
                $microregiao
            ) . "') "
        );
    }

    // UF
    if ($uf[0] && $uf_campo_flag) {
        array_push(
            $where,
            " ed.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $uf) . "') "
        );
    }

    // grupo municipio
    if ($grupomun[0] && $grupomun_campo_flag) {
        $selectTerritorios = "( SELECT
                                    tm.muncod, tm.mundescricao, gt.gtmid, gt.gtmdsc, tpm.tpmdsc
                                FROM
                                    territorios.municipio tm
                                INNER JOIN
                                    territorios.muntipomunicipio mtm ON mtm.muncod = tm.muncod
                                INNER JOIN
                                    territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid
                                INNER JOIN
                                    territorios.grupotipomunicipio gt ON gt.gtmid = tpm.gtmid
                                WHERE
                                    tpm.gtmid = 5 AND gt.gtmid = 5 )";

        $selectGrupoMun = "CASE WHEN tm.gtmid is not null THEN tm.gtmdsc ELSE 'Outros' END as grupomun, ";
        $dadosGrupoMun = "grupomun, ";
        $groupByGrupoMun = "tm.gtmdsc, ";
        $groupByGtmid = "tm.gtmid, ";
        array_push($where, " gt.gtmid " . (!$grupomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode(
            "','",
            $grupomun
        ) . "') ");
    }

    // tipo municipio
    if ($tipomun[0] && $tipomun_campo_flag) {
        array_push($where, " tpm.tpmid " . (!$tipomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode(
            "','",
            $tipomun
        ) . "') ");
    }

    // municipio
    if ($municipio[0] && $municipio_campo_flag) {
        array_push($where, " ed.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode(
            "','",
            $municipio
        ) . "') ");
    }

    // unidade
    if ($unidade[0] && $unidade_campo_flag) {
        array_push(
            $where,
            " oi.entidunidade " . (!$unidade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(
                ',',
                $unidade
            ) . ") "
        );
    }

    // entidcampus
    if ($entidcampus[0] && $entidcampus_campo_flag) {
        array_push(
            $where,
            " oi.entidcampus " . (!$entidcampus_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(
                ',',
                $entidcampus
            ) . ") "
        );
    }

    // programa
    if (!$painel) {
        if ($prfid[0] && $prfid_campo_flag) {
            if (!$prfid_campo_excludente) {
                array_push($where, " oi.prfid  IN (" . implode(',', $prfid) . ") ");
            } else {
                array_push($where, " ( oi.prfid  NOT IN (" . implode(',', $prfid) . ") OR oi.prfid is null ) ");
            }
        }

        // Fonte
        if ($tooid[0] && $tooid_campo_flag) {
            if (!$tooid_campo_excludente) {
                array_push($where, " oi.tooid  IN (" . implode(',', $tooid) . ") ");
            } else {
                array_push($where, " ( oi.tooid  NOT IN (" . implode(',', $tooid) . ") OR oi.tooid IS NULL ) ");
            }
        }
    }

    // tipologia da obra
    if ($tpoid[0] && $tpoid_campo_flag) {
        array_push(
            $where,
            " oi.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $tpoid) . ") "
        );
    }

    // classificao da obra
    if ($cloid[0] && $cloid_campo_flag) {
        array_push(
            $where,
            " oi.cloid " . (!$cloid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $cloid) . ") "
        );
    }

    // Situação da obra
    array_push($where, " oi.stoid not in (11) ");
    if ($stoid[0] && $stoid_campo_flag) {
        array_push(
            $where,
            " oi.stoid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $stoid) . ") "
        );
    }

    // tipo da obra
    if ($tobaid[0] && $tobaid_campo_flag) {
        array_push(
            $where,
            " oi.tobraid " . (!$tobaid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode(',', $tobaid) . ") "
        );
    }

    if ((int)$percentualinicial > 0) {
        $perc = (int)$percentualfinal == 100 ? 110 : $percentualfinal;
        array_push($where, " ((oi.obrpercexec BETWEEN {$percentualinicial} AND {$perc}))");
    } elseif ((int)$percentualinicial == '0') {
        if ((int)$percentualfinal > 0) {
            if (!((int)$percentualfinal == 100)) {
                $perc = (int)$percentualfinal == 100 ? 110 : $percentualfinal;

                array_push(
                    $where,
                    " ((oi.obrpercexec IS NULL OR oi.obrpercexec BETWEEN {$percentualinicial} AND {$perc}))"
                );
            }
        } elseif ((int)$percentualfinal == 0) {
            array_push($where, " ((oi.obrpercexec = 0 OR oi.obrpercexec IS NULL))");
        }
    }

    // repositorio
    if ($flag_repositorio) {
        array_push($where, " oi.obrid IN ( select distinct obrid from obras.repositorio where repstatus = 'A') ");
    }

    // percentual da obra
    if ($latitudeElongitude) {
        array_push($where, " (TRIM(ed.medlatitude)<>'' AND TRIM(ed.medlongitude)<>'')");
    }

    // metragem da obra
    if ($metragem_inicio) {
        if ($metragem_operador == 'entre') {
            array_push(
                $where,
                " oi.obrqtdconstruida >= '$metragem_inicio' AND oi.obrqtdconstruida <= '$metragem_fim' "
            );
        } else {
            array_push($where, " oi.obrqtdconstruida $metragem_operador '$metragem_inicio' ");
        }
    }

    // possui foto
    switch ($foto) {
        case 'sim':
            $stFiltro .= " and (ao.obrid is not null and ao.aqostatus = 'A') ";
            break;
        case 'nao':
            $stFiltro .= " and ao.obrid is null  ";
            break;
    }

    if (($vlrmenor) != '' && !empty($vlrmaior) != '') {
        $vlrmenor = str_replace(array(".", ","), array("", "."), $vlrmenor);
        $vlrmaior = str_replace(array(".", ","), array("", "."), $vlrmaior);
        $stFiltro .= " AND (oi.obrvlrrealobra BETWEEN {$vlrmenor} AND {$vlrmaior}) ";
    }

    // Filtro de vistoria
    switch ($_REQUEST["vistoria"]) {
        case 'sim':
            $stFiltro .= " and oi.obrdtvistoria is not null ";
            break;
        case 'nao':
            $stFiltro .= " and oi.obrdtvistoria is null ";
            break;
    }

    // Possui Supervisão por Empresas
    switch ($_REQUEST["supervisao_empresa"]) {
        case 'sim':
            $stFiltro .= " and oi.obrsupemp is true ";
            break;
        case 'nao':
            $stFiltro .= " and (oi.obrsupemp is false or oi.obrsupemp is null )";
            break;
    }

    // Est em funcionamento?
    switch ($_REQUEST["funundfuncionamento"]) {
        case 'sim':
            $stFiltro .= " and funundfuncionamento is true ";
            break;
        case 'nao':
            $stFiltro .= " and (funundfuncionamento is false )";
            break;
    }
    if ($_REQUEST["funundfuncionamento"] == 'sim' || $_REQUEST["funundfuncionamento"] == 'nao') {
        $innerFuncionamento = "INNER JOIN ( SELECT
                                                f1.obrid, funundfuncionamento
                                            FROM
                                                obras.funcunidade f1
                                            INNER JOIN (
                                                            SELECT obrid, max(funid) as funid FROM obras.funcunidade GROUP BY obrid ORDER BY obrid
                                                       ) f2 ON f1.funid = f2.funid ) fun ON oi.obrid = fun.obrid";
    }
    if (!empty($dados["rsuid"]) || $dados["rsuid"] == '0') {
        if ($dados["rsuid"] == '0') {
            $filtro .= " and COALESCE(ov.supervisao,0) = 0 ";
        } else {
            $filtro .= " and sup.rsuid = " . $dados["rsuid"];
        }
    }
    // Filtro de responsvel pela vistoria
    switch ($_REQUEST["responsavel"]) {
        case '':
            $stFiltro .= " ";
            break;
        case '0':
            $stFiltro .= " and COALESCE(s.supvid,0) = 0 ";
            break;
        case '1':
            $stFiltro .= " and s.rsuid = 1 ";
            break;
        case '2':
            $stFiltro .= " and s.rsuid = 2 ";
            break;
        case '3':
            $stFiltro .= " and s.rsuid = 3 ";
            break;
        case '4':
            $stFiltro .= " and s.rsuid = 4 ";
            break;
    }

    // Filtro de restricao
    switch ($restricao) {
        case 'sim':
            $stFiltro .= " and (r.obrid is not null and r.rststatus = 'A')";
            break;
        case 'nao':
            $stFiltro .= " and r.obrid is null ";
            break;
    }

    // valor da obra
    if ($_REQUEST["vlrmenor"] && $_REQUEST["vlrmaior"]) {
        $vlrmenor = str_replace(array(".", ","), array("", "."), $_REQUEST["vlrmenor"]);
        $vlrmaior = str_replace(array(".", ","), array("", "."), $_REQUEST["vlrmaior"]);
        $stFiltro .= " AND oi.obrvlrrealobra BETWEEN " . $vlrmenor . " AND " . $vlrmaior . " ";
    }

    //Status da Obra
    if ($_REQUEST["status"] == "inativo") {
        $stFiltro .= " AND oi.obsstatus = 'I' and usucpfexclusao is not null ";
    } elseif ($_REQUEST["status"] == "todas") {
        $stFiltro .= "  ";
    } else {
        $stFiltro .= " AND oi.obsstatus = 'A' ";
    }

    if (!empty($_REQUEST["obrtipoesfera"])) {
        $stFiltro .= " AND oi.obrtipoesfera = '{$_REQUEST["obrtipoesfera"]}' ";
    }

//Regra = verifica se o grupo da obra est finalizado
    switch ($_REQUEST["supervisao"]) {
        case "S":
            if (!$_REQUEST["subfiltro_inicio"]) {
                $stFiltro .= " AND ( (  SELECT
                                                COUNT(ooi.obrid)
                                        FROM
                                                obras.obrainfraestrutura ooi
                                        left JOIN
                                                obras.repositorio r ON r.obrid = ooi.obrid
                                        left JOIN
                                                obras.itemgrupo oig ON oig.repid = r.repid
                                        left JOIN
                                                obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid
                                        left JOIN
                                                workflow.documento wd ON wd.docid = ogd.docid
                                        left JOIN
                                                workflow.estadodocumento we ON we.esdid = wd.esdid
                                        WHERE
                                                ooi.obrid = oi.obrid
                                                AND we.esdid = " . OBRSUPFINALIZADA . "
                                                AND r.repstatus = 'I'
                                                AND ogd.gpdstatus = 'A'
                                                ) > 0  )";
            } else {
                if ($_REQUEST["tiposupervisao"] == "entre") {
                    $stFiltro .= " AND ( (  SELECT
                                                    COUNT(ooi.obrid)
                                            FROM
                                                    obras.obrainfraestrutura ooi
                                            left JOIN
                                                    obras.repositorio r ON r.obrid = ooi.obrid
                                            left JOIN
                                                    obras.itemgrupo oig ON oig.repid = r.repid
                                            left JOIN
                                                    obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid
                                            left JOIN
                                                    workflow.documento wd ON wd.docid = ogd.docid
                                            left JOIN
                                                    workflow.estadodocumento we ON we.esdid = wd.esdid
                                            WHERE
                                                    ooi.obrid = oi.obrid
                                                    AND we.esdid = " . OBRSUPFINALIZADA . "
                                                    AND r.repstatus = 'I'
                                                    AND ogd.gpdstatus = 'A'
                                                    ) between {$_REQUEST["subfiltro_inicio"]} and {$_REQUEST["subfiltro_fim"]} )";
                } else {
                    $stFiltro .= " AND ( (  SELECT
                                                    COUNT(ooi.obrid)
                                            FROM
                                                    obras.obrainfraestrutura ooi
                                            left JOIN
                                                    obras.repositorio r ON r.obrid = ooi.obrid
                                            left JOIN
                                                    obras.itemgrupo oig ON oig.repid = r.repid
                                            left JOIN
                                                    obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid
                                            left JOIN
                                                    workflow.documento wd ON wd.docid = ogd.docid
                                            left JOIN
                                                    workflow.estadodocumento we ON we.esdid = wd.esdid
                                            WHERE
                                                    ooi.obrid = oi.obrid
                                                    AND we.esdid = " . OBRSUPFINALIZADA . "
                                                    AND r.repstatus = 'I'
                                                    AND ogd.gpdstatus = 'A'
                                                    ) {$_REQUEST["tiposupervisao"]} {$_REQUEST["subfiltro_inicio"]}  )";
                }
            }
            break;

        case "N":
            $filtro .= " AND ( (SELECT
                                        COUNT(ooi.obrid)
                                FROM
                                        obras.obrainfraestrutura ooi
                                left JOIN
                                        obras.repositorio r ON r.obrid = ooi.obrid
                                left JOIN
                                        obras.itemgrupo oig ON oig.repid = r.repid
                                left JOIN
                                        obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid
                                left JOIN
                                        workflow.documento wd ON wd.docid = ogd.docid
                                left JOIN
                                        workflow.estadodocumento we ON we.esdid = wd.esdid
                                WHERE
                                        ooi.obrid = oi.obrid
                                        AND we.esdid = " . OBRSUPFINALIZADA . "
                                        AND r.repstatus = 'I'
                                        AND ogd.gpdstatus = 'A'
                                        ) = 0  )";
            break;
    }

    // Filtro de 'Com retorno de (concluda) para (em execução)'
    switch ($concluidaexec) {
        case 'sim':
            $stFiltro .= " and (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT su.supvid FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid order by supvdt desc, supvid desc limit 1)) = 1 and (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT su.supvid FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid AND su.supvid NOT IN (SELECT su.supvid FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid order by supvdt desc, supvid desc limit 1) order by supvdt desc, supvid desc limit 1)) = 3 ";
            break;
        case 'nao':
            $stFiltro .= " and (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT su.supvid FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid order by supvdt desc, supvid desc limit 1)) <> 1 or (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT su.supvid FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid AND su.supvid NOT IN (SELECT su.supvid FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid order by supvdt desc, supvid desc limit 1) order by supvdt desc, supvid desc limit 1)) <> 3 ";
            break;
    }

    // Valor Contratado da Obra (R$):
    if ($obrcustocontrato_inicio != "" && $obrcustocontrato_fim != "") {
        $obrcustocontrato_inicio = str_replace(array(".", ","), array("", "."), $obrcustocontrato_inicio);
        $obrcustocontrato_fim = str_replace(array(".", ","), array("", "."), $obrcustocontrato_fim);
        array_push($where, " oi.obrcustocontrato between $obrcustocontrato_inicio and $obrcustocontrato_fim ");
    }

    /* '<a style=\"cursor:pointer;\" onclick=\"parent.opener.window.location.href=\'/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' || oi.obrid || '\'; parent.opener.window.focus();\"> (' || oi.obrid || ') ' || oi.obrdesc || ' &nbsp;(' || (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' % Executado', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) || ') </a>' as nomedaobra, */

    // monta o sql
    $sql = "SELECT
                CASE WHEN metragem > 0 AND metragem <= 500 THEN '<span id=\"1\">At 500 (m)</span>'
                     WHEN metragem >= 501 AND metragem <= 1500  THEN '<span id=\"2\">501 at 1500 (m)</span>'
                      WHEN metragem >= 1501 AND metragem <= 4500 THEN '<span id=\"3\">1501 at 4500 (m)</span>'
                     WHEN metragem >= 4501 AND metragem <= 10000 THEN '<span id=\"4\">4501 at 10000 (m)</span>'
                     WHEN metragem >= 10001 THEN '<span id=\"5\">Maior que 10001 (m)</span>'
                     WHEN metragem is null THEN '<span id=\"6\">No Informado</span>'
                ELSE '<span id=\"6\">No Informado</span>' END as metragem,
                CASE WHEN metragem > 0 AND metragem <= 500 THEN 'At 500 (m)'
                     WHEN metragem >= 501 AND metragem <= 1500  THEN '501 at 1500 (m)'
                      WHEN metragem >= 1501 AND metragem <= 4500 THEN '1501 at 4500 (m)'
                     WHEN metragem >= 4501 AND metragem <= 10000 THEN '4501 at 10000 (m)'
                     WHEN metragem >= 10001 THEN 'Maior que 10001 (m)'
                     WHEN metragem is null THEN 'No Informado'
                ELSE 'No Informado' END as metragemxls,
                COALESCE(mesoregiao,'No Informado') as mesoregiao,
                COALESCE(microregiao,'No Informado') as microregiao,
                COALESCE(regiao,'No Informado') as regiao,
                COALESCE(pais,'No Informado') as pais,
                COALESCE(unidade,'No Informado') as unidade,
                COALESCE(campus,'No Informado') as campus,
                COALESCE(empresa,'No Informado') as empresa,
                COALESCE(uf,'No Informado') as uf,
                {$dadosTipoMun}
                {$dadosGrupoMun}
                COALESCE(municipio,'No Informado') as municipio,
                CASE WHEN codigo_situacao IN (1, 2) THEN
                    (CASE WHEN DATE_PART('days', NOW() - nivelpreenchimento) <= 25
                             THEN '<span style=\"color: green;\">1 - Verde (Obras atualizadas h menos de 25 dias atrs)</span>'
                         WHEN DATE_PART('days', NOW() - nivelpreenchimento) BETWEEN 25  AND 30
                             THEN '<span style=\"color: #BB9900;\">2 - Amarelo (Obras atualizadas entre 25 e 30 dias)</span>'
                         ELSE '<span style=\"color: red;\">3 - Vermelho (Obras atualizadas h mais de 30 dias)</span>'
                    END)
                    WHEN codigo_situacao = 3 THEN '<span style=\"color: blue;\">4 - Azul (Obras concludas)</span>'
                ELSE
                    '5 - No se aplica' END as nivelpreenchimento,
                CASE WHEN codigo_situacao IN (1, 2) THEN
                    (CASE WHEN DATE_PART('days', NOW() - nivelpreenchimento) <= 25
                             THEN '1 - Verde (Obras atualizadas h menos de 25 dias atrs)'
                         WHEN DATE_PART('days', NOW() - nivelpreenchimento) BETWEEN 25  AND 30
                             THEN '2 - Amarelo (Obras atualizadas entre 25 e 30 dias)'
                         ELSE '3 - Vermelho (Obras atualizadas h mais de 30 dias)'
                    END)
                    WHEN codigo_situacao = 3 THEN '4 - Azul (Obras concludas)'
                ELSE
                    '5 - No se aplica' END as nivelpreenchimentoxls,
                classificacao,
                situacao,
                tipologia,
                programa,
                CASE WHEN fonte IS NOT NULL THEN fonte ELSE 'No Iformado' END AS fonte,
                COALESCE(nomedaobra,'No Informado') as nomedaobra,
                nomedaobra2,
                nomedaobraxls,
                coalesce(sum(superior),0) as superior,
                coalesce(sum(tecnico),0) as tecnico,
                coalesce(sum(basico),0) as basico,
                coalesce(sum(administrativa),0) as administrativa,
                coalesce(sum(hospital),0) as hospital,
                coalesce(sum(total),0) as total,
                traid,
                traseq,
                obrid,
                COALESCE(to_char(obrdtexclusao,'DD/MM/YYYY'),'N/A') as obrdtexclusao,
                COALESCE(obrobsexclusao,'N/A') as obrobsexclusao,
                COALESCE((select usunome from seguranca.usuario where usucpf = usucpfexclusao),'N/A') as usucpfexclusao,
                COALESCE(tobadesc,'No Informado') as tipodaobra,
                obrnumprocesso
            FROM
                (SELECT
                    obrobsexclusao,
                    obrdtexclusao,
                    usucpfexclusao,
                    oi.obrqtdconstruida as metragem,
                    CASE WHEN oi.entidcampus is not null THEN ee2.entnome ELSE 'No informado' END as campus,
                    CASE WHEN ee3.entnome is not null THEN ee3.entnome ELSE 'No informado' END as empresa,
                    me.mesdsc as mesoregiao,
                    mi.micdsc as microregiao,
                    re.regdescricao as regiao,
                    pa.paidescricao as pais,
                    ee.entnome as unidade,
                    CASE WHEN ed.estuf <> '' THEN ed.estuf ELSE 'No Informado' END as uf,
                    {$selectTipoMun}
                    {$selectGrupoMun}
                    tm.mundescricao as municipio,
                    CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END as nivelpreenchimento,
                    CASE WHEN oi.cloid is not null THEN cl.clodsc ELSE 'No informado' END as classificacao,
                    CASE WHEN oi.stoid is not null THEN st.stodesc ELSE 'No Informado' END as situacao,
                    oi.stoid as codigo_situacao,
                    CASE WHEN oi.tpoid is not null THEN tp.tpodsc ELSE 'No informado' END as tipologia,
                    CASE WHEN oi.prfid is not null THEN pf.prfdesc ELSE 'No informado' END as programa,
                    tobr.toodescricao AS fonte,
                    oi.obrdesc as nomedaobraxls,
                    oi.obrnumprocesso,

                    '<a style=\"cursor:pointer;\" onclick=\"parent.opener.window.location.href=\'/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' || oi.obrid || '\'; parent.opener.window.focus();\"> (' || oi.obrid || ') ' || oi.obrdesc || ' &nbsp;(' ||
                    COALESCE((SELECT
                        replace(
                            ( SELECT
                                trunc(coalesce( sum(( icopercsobreobra * supvlrinfsupervisor ) / 100) ,0 )::numeric, 2)
                              FROM
                                obras.itenscomposicaoobra i
                            INNER JOIN obras.supervisaoitenscomposicao si ON i.icoid = si.icoid
                            WHERE si.supvid = s.supvid
                              AND obrid = oi.obrid
                              AND i.icovigente = 'A' )
                         || ' % Executado', '.', ',') as percentual
                        FROM
                        obras.supervisao s
                        INNER JOIN
                        obras.situacaoobra si ON si.stoid = s.stoid
                        INNER JOIN
                        seguranca.usuario u ON u.usucpf = s.usucpf
                        LEFT JOIN
                        entidade.entidade e ON e.entid = s.supvistoriador
                        LEFT JOIN
                        obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid
                        WHERE
                        s.obrid = oi.obrid AND
                        s.supstatus = 'A'
                        ORDER BY
                        s.supdtinclusao DESC LIMIT 1),'0% Executado')
                     || ') </a>' as nomedaobra,

                    '<a style=\"cursor:pointer;\" onclick=\"abrebalao(' || oi.obrid || ');\">' || oi.obrdesc || '</a>' as nomedaobra2,
                    CASE WHEN oi.orgid = 1 THEN count(oi.obrid) END as superior,
                    CASE WHEN oi.orgid = 2 THEN count(oi.obrid) END as tecnico,
                    CASE WHEN oi.orgid = 3 THEN count(oi.obrid) END as basico,
                    CASE WHEN oi.orgid = 4 THEN count(oi.obrid) END as administrativa,
                    CASE WHEN oi.orgid = 5 THEN count(oi.obrid) END as hospital,
                    count(oi.obrid) as total,
                    ta.traid,
                    ta.traseq,
                    oi.obrid,
                    tpo.tobadesc
                FROM
                    obras.obrainfraestrutura oi
                $innerFuncionamento
                INNER JOIN
                    entidade.endereco ed        ON oi.endid = ed.endid
                LEFT JOIN
                    territorios.estado et        ON ed.estuf = et.estuf
                LEFT JOIN
                    territorios.regiao re        ON re.regcod = et.regcod
                LEFT JOIN
                    territorios.municipio tm2  ON tm2.muncod = ed.muncod
                LEFT JOIN
                    territorios.microregiao mi  ON mi.miccod = tm2.miccod
                LEFT JOIN
                    territorios.mesoregiao me  ON me.mescod = mi.mescod
                LEFT JOIN
                    {$selectTerritorios} tm    ON tm.muncod = ed.muncod
                INNER JOIN
                    entidade.entidade ee        ON oi.entidunidade = ee.entid
                LEFT JOIN
                    territorios.pais pa        ON pa.paiid = re.paiid
                LEFT JOIN
                    entidade.entidade ee2        ON oi.entidcampus = ee2.entid
                LEFT JOIN
                    entidade.funcaoentidade ef ON ee2.entid = ef.entid AND ef.funid IN( 17 )
                LEFT JOIN
                    entidade.entidade ee3        ON oi.entidempresaconstrutora = ee3.entid
                LEFT JOIN
                    obras.programafonte pf        ON oi.prfid = pf.prfid
                LEFT JOIN
                    obras.tipoorigemobra tobr  ON oi.tooid = tobr.tooid
                LEFT JOIN
                    obras.classificacaoobra cl ON oi.cloid = cl.cloid
                LEFT JOIN
                    obras.situacaoobra st        ON oi.stoid = st.stoid
                LEFT JOIN
                    obras.tipologiaobra tp        ON oi.tpoid = tp.tpoid
                LEFT JOIN
                    obras.tipoobra tpo           ON oi.tobraid = tpo.tobaid
                LEFT JOIN
                    (SELECT
                        rsuid,obrid, supvid
                    FROM
                        obras.supervisao s
                    WHERE
                        supvid = (SELECT ss.supvid FROM obras.supervisao ss WHERE ss.supstatus = 'A' AND ss.obrid = s.obrid order by supvdt desc, supvid desc limit 1) ) AS s ON s.obrid = oi.obrid
                LEFT JOIN
                    ( SELECT DISTINCT obrid, aqostatus FROM obras.arquivosobra WHERE tpaid = 21 AND aqostatus = 'A' ) as ao ON ao.obrid = oi.obrid
                LEFT JOIN
                    ( SELECT DISTINCT obrid, rststatus FROM obras.restricaoobra WHERE rststatus = 'A' ) as r ON r.obrid = oi.obrid

                LEFT JOIN
                    obras.termoaditivo ta ON ta.obrid = oi.obrid AND ta.trastatus = 'A' AND ta.traid = (SELECT traid FROM obras.termoaditivo WHERE obrid = ta.obrid AND trastatus = 'A' ORDER BY traseq DESC LIMIT 1)
                WHERE
                    1=1 " . (is_array($where) ? ' AND' . implode(' AND ', $where) : '')
        . $stFiltro . "
                GROUP BY
                    oi.orgid, ed.estuf, tm.mundescricao, {$groupByGtmid} {$groupByGrupoMun} {$groupByTipoMun}
                    ee.entnome, ee2.entnome, ee3.entnome, me.mesdsc, mi.micdsc,
                    re.regdescricao, pa.paidescricao, cl.clodsc,
                    st.stodesc, oi.stoid, tp.tpodsc, pf.prfdesc, tobr.toodescricao, oi.obrdesc,
                    oi.prfid, oi.tooid , oi.entidcampus, oi.cloid, oi.stoid, oi.obrnumprocesso
                    oi.tpoid, oi.prfid, oi.obrid, oi.obrdtvistoria, oi.obsdtinclusao, oi.obrqtdconstruida, ta.traid, ta.traseq, oi.obrid, obrobsexclusao, obrdtexclusao, usucpfexclusao, tpo.tobadesc ) as foo
            GROUP BY
                unidade, campus, uf, {$dadosTipoMun} {$dadosGrupoMun}
                municipio, nivelpreenchimento, mesoregiao, microregiao, regiao, tipologia, obrnumprocesso
                classificacao, programa, fonte, situacao, codigo_situacao, nomedaobra, nomedaobra2, empresa, nomedaobraxls,
                pais, metragem, traid, traseq, obrid, obrobsexclusao, obrdtexclusao, usucpfexclusao, tipodaobra
            ORDER BY
                " . (is_array($agrupador) ? implode(",", $agrupador) : "pais");
    //dbg($sql,d);
    return $sql;
}

/**
 * Funao que monta o agrupador do relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return array
 */
function obras_monta_agp_relatorio()
{

    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];

    $agp = array(
        "agrupador" => array(),
        "agrupadoColuna" => array(
            "superior",
            "tecnico",
            "basico",
            "administrativa",
            "hospital",
            "total",
            "obrobsexclusao",
            "obrdtexclusao",
            "usucpfexclusao"
        ),
        "agrupadorDetalhamento" => array(
            array(
                "campo" => "mesoregiao",
                "label" => "Mesorregio"
            ),
            array(
                "campo" => "microregiao",
                "label" => "Microrregio"
            ),
            array(
                "campo" => "campus",
                "label" => "Campus"
            ),
            array(
                "campo" => "municipio",
                "label" => "Município"
            ),
            array(
                "campo" => "pais",
                "label" => "Pas"
            ),
            array(
                "campo" => "regiao",
                "label" => "Regio"
            ),
            array(
                "campo" => "nomedaobra",
                "label" => "Nome da Obra"
            )
        )
    );

    foreach ($agrupador as $val) {
        switch ($val) {
            case "campus":
                array_push($agp['agrupador'], array(
                    "campo" => "campus",
                    "label" => "Campus"
                ));
                break;
            case "mesoregiao":
                array_push($agp['agrupador'], array(
                    "campo" => "mesoregiao",
                    "label" => "Mesorregio"
                ));
                break;
            case "microregiao":
                array_push($agp['agrupador'], array(
                    "campo" => "microregiao",
                    "label" => "Microrregio"
                ));
                break;
            case "municipio":
                array_push($agp['agrupador'], array(
                    "campo" => "municipio",
                    "label" => "Município"
                ));
                break;
            case "pais":
                array_push($agp['agrupador'], array(
                    "campo" => "pais",
                    "label" => "Pas"
                ));
                break;
            case "regiao":
                array_push($agp['agrupador'], array(
                    "campo" => "regiao",
                    "label" => "Regio"
                ));
                break;
            case "uf":
                array_push($agp['agrupador'], array(
                    "campo" => "uf",
                    "label" => "UF"
                ));
                break;
            case "unidade":
                array_push($agp['agrupador'], array(
                    "campo" => "unidade",
                    "label" => "Unidade"
                ));
                break;
            case "programa":
                array_push($agp['agrupador'], array(
                    "campo" => "programa",
                    "label" => "Programa"
                ));
                break;
            case "fonte":
                array_push($agp['agrupador'], array(
                    "campo" => "fonte",
                    "label" => "Fonte"
                ));
                break;
            case "situacao":
                array_push($agp['agrupador'], array(
                    "campo" => "situacao",
                    "label" => "Situação da Obra"
                ));
                break;
            case "tipologia":
                array_push($agp['agrupador'], array(
                    "campo" => "tipologia",
                    "label" => "Tipologia da Obra"
                ));
                break;
            case "classificacao":
                array_push($agp['agrupador'], array(
                    "campo" => "classificacao",
                    "label" => "Classificao da Obra"
                ));
                break;
            case "nomedaobra":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedaobra",
                    "label" => "Nome da Obra"
                ));
                break;
            case "nomedaobra2":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedaobra2",
                    "label" => "Nome da Obra"
                ));
                break;
            case "nomedaobraxls":
                array_push($agp['agrupador'], array(
                    "campo" => "nomedaobraxls",
                    "label" => "Nome da Obra"
                ));
                break;
            case "nivelpreenchimento":
                array_push($agp['agrupador'], array(
                    "campo" => "nivelpreenchimento",
                    "label" => "Nvel de Preenchimento"
                ));
                break;
            case "nivelpreenchimentoxls":
                array_push($agp['agrupador'], array(
                    "campo" => "nivelpreenchimentoxls",
                    "label" => "Nvel de Preenchimento"
                ));
                break;

            case "empresa":
                array_push($agp['agrupador'], array(
                    "campo" => "empresa",
                    "label" => "Empresa Contratada"
                ));
                break;
            case "metragem":
                array_push($agp['agrupador'], array(
                    "campo" => "metragem",
                    "label" => "Metragem da Obra"
                ));
                break;
            case "metragemxls":
                array_push($agp['agrupador'], array(
                    "campo" => "metragemxls",
                    "label" => "Metragem da Obra"
                ));
                break;

            /* case "grupomun":
                  array_push($agp['agrupador'], array(
                  "campo" => "grupomun",
                  "label" => "Grupo de Município")
                  );
                  break; */
            case "tipomun":
                array_push($agp['agrupador'], array(
                    "campo" => "tipomun",
                    "label" => "Territrio da Cidadania"
                ));
                break;

            case "tipodaobra":
                array_push($agp['agrupador'], array(
                    "campo" => "tipodaobra",
                    "label" => "Tipo da Obra"
                ));
                break;
        }
    }

    return $agp;
}

/**
 * Funao que monta as colunas do relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return array
 */
function obras_monta_coluna_relatorio()
{

    $coluna = array();

    foreach ($_REQUEST['orgid'] as $valor) {
        switch ($valor) {
            case '1':
                array_push($coluna, array(
                    "campo" => "superior",
                    "label" => "Ensino Superior",
                    "blockAgp" => "nomedaobra",
                    "type" => "numeric"
                ));
                break;
            case '2':
                array_push($coluna, array(
                    "campo" => "tecnico",
                    "label" => "Ensino Profissional",
                    "blockAgp" => "nomedaobra",
                    "type" => "numeric"
                ));
                break;
            case '3':
                array_push($coluna, array(
                    "campo" => "basico",
                    "label" => "Ensino Bsico",
                    "blockAgp" => "nomedaobra",
                    "type" => "numeric"
                ));
                break;
            case '4':
                array_push($coluna, array(
                    "campo" => "administrativa",
                    "label" => "Administrativas",
                    "blockAgp" => "",
                    "type" => "numeric"
                ));
                break;
            case '5':
                array_push($coluna, array(
                    "campo" => "hospital",
                    "label" => "Hospitais",
                    "blockAgp" => "",
                    "type" => "numeric"
                ));
                break;
        }
    }

    if (in_array("nomedaobra", $_REQUEST['agrupador'])) {
        $_REQUEST['colunas'] = $_REQUEST['colunas'] ? $_REQUEST['colunas'] : array();
        foreach ($_REQUEST['colunas'] as $valor) {
            switch ($valor) {
                case 'obrdtexclusao':
                    array_push($coluna, array(
                        "campo" => "obrdtexclusao",
                        "label" => "Data da Excluso",
                        "blockAgp" => "",
                        "type" => "string"
                    ));
                    break;
                case 'usucpfexclusao':
                    array_push($coluna, array(
                        "campo" => "usucpfexclusao",
                        "label" => "Resposansvel pela Excluso",
                        "blockAgp" => "",
                        "type" => "string"
                    ));
                    break;
                case 'obrobsexclusao':
                    array_push($coluna, array(
                        "campo" => "obrobsexclusao",
                        "label" => "Observao da Excluso",
                        "blockAgp" => "",
                        "type" => "string"
                    ));
                    break;
            }
        }
    }

    array_push($coluna, array(
        "campo" => "total",
        "label" => "Total de Obras",
        "blockAgp" => "nomedaobra",
        "type" => "numeric"
    ));

    return $coluna;
}

// informa de aceite para execucao de obra mi
function informaAceiteDeObraMI($obrid)
{
    global $db;

    require_once APPRAIZ . "obras2/classes/modelo/Orgao.class.inc";
    require_once APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc";

    if ($_SESSION['usucpf'] != '') {
        return false;
    }

    $sql = "
        select usuemail
        from seguranca.usuario u
        inner join seguranca.perfilusuario pu on u.usucpf = pu.usucpf and pu.pflcod = " . PFLCOD_ALERTA_MI_INTERNO . "
        where u.usustatus = 'A' or u.usustatus is null
    ";
    $listaEmails = $db->carregar($sql);
    $novaLista = array();
    foreach ($listaEmails as $key => $value) {
        $novaLista[] = $value['usuemail'];
    }
    $listaEmails = $novaLista;

    // dados da obra

    $obra = new Obras($obrid);
    $sql = "
        select
            e.estuf,
            m.mundescricao
        from obras2.obras o
        inner join entidade.endereco e on e.endid = o.endid
        inner join territorios.municipio m on e.muncod = m.muncod
        where obrid = {$obrid}
    ";
    $obrInfo = $db->pegaUm($sql);

    $remetente = array("nome" => "SIMEC - Monitoramento de Obras", "email" => "monitoramento.obras@mec.gov.br");
    $assunto = "SIMEC - Ordem de serviço aceita - Obra {$obrid} - {$obra->obrnome} - {$obrInfo['mundescricao']}/{$obrInfo['estuf']}";

    $sql = " SELECT em.orgid FROM obras2.empreendimento em INNER JOIN obras2.obras o ON o.obrid = {$obrid} AND o.empid = em.empid ";
    $orgid = $db->pegaUm($sql);
    $orgao = new Orgao($orgid);
    $tipoDeEnsino = $orgao->orgdesc;

    $entidade = new Entidade($obra->entid);
    $unidadeImplantadora = $entidade->entnome;

    $sql = " select tpodsc from obras2.tipologiaobra where tpoid = {$obra->tpoid} ";
    $tpdsc = $db->pegaUm($sql);
    $tipologiaDaObra = $tpdsc;

    $sql = " select tobdesc from obras2.tipoobra where tobid = {$obra->tobid} ";
    $tobdesc = $db->pegaUm($sql);
    $tipoDeObra = $tobdesc;

    $sql = " select clodsc from obras2.classificacaoobra where cloid = {$obra->cloid}";
    $classificacaoObra = $db->pegaUm($sql);

    $conteudo = "
        <table>
            <tr><td colspan='2'><h2>Dados da Obra</h2></td></tr>
            <tr><td>Tipo de Ensino:</td><td>{$tipoDeEnsino}</td></tr>
            <tr><td>Unidade Implantadora:</td><td>{$unidadeImplantadora}</td></tr>
            <tr><td>Nome da Obra:</td><td>{$obra->obrnome}</td></tr>
            <tr><td>Tipologia da Obra:</td><td>{$tipologiaDaObra}</td></tr>
            <tr><td>Tipo de Obra:</td><td>{$tipoDeObra}</td></tr>
            <tr><td>Classificao da Obra:</td><td>{$classificacaoObra}</td></tr>
            <tr><td>Descrição/Composio da Obra:</td><td>{$obra->obrdsc}</td></tr>
            <tr><td>Valor Previsto:</td><td>{$obra->obrvalorprevisto}</td></tr>
            <tr><td colspan='2'>&nbsp;</td></tr>
    ";

    // dados da os
    $ordemServico = new OrdemServicoMI();
    $dados = $ordemServico->carregarPorObrid($obrid);

    $tipoOsMi = new TipoOsMi();
    $tipoOS = $tipoOsMi->resgataTipoOSMI($dados['tomid']);

    $dataHoraEmissao = date('d/m/Y H:i:s');

    $sql = " select usunome from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}' ";
    $usuarioEmissor = $db->pegaUm($sql);

    $osmdtinicio = formata_data($dados['osmdtinicio']);

    $dataDaOS = date("d/m/Y H:m:d", strtotime($dados['osmdtcadastro']));

    $prazoexecucao = $dados['osmprazo'];

    $datatermino = formata_data($dados['osmdttermino']);

    // $arquivo = new Arquivo( $dados['arqid'] );
    // if ( $arquivo->arqid ){
    //     $ordemdeservico = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
    // }
    // // <tr><td>Ordem de Serviço:</td><td>{$ordemdeservico}</td></tr>

    $conteudo .= "
            <tr><td><h2>Dados da OS</h2></td></tr>
            <tr><td>Tipo:</td><td>{$tipoOS}</td></tr>
            <tr><td>Data da OS:</td><td>{$dados['osmdtcadastro']}</td></tr>
            <tr><td>Data de início da execução:</td><td>{$osmdtinicio}</td></tr>
            <tr><td>Prazo de execução (dias):</td><td>{$prazoexecucao}</td></tr>
            <tr><td>Data término da execução:</td><td>{$datatermino}</td></tr>
            <tr><td colspan='2'>&nbsp;</td></tr>
            <tr><td>Emitido por:</td><td>{$usuarioEmissor}</td></tr>
            <tr><td>Data/Hora emisso:</td><td>{$dataHoraEmissao}</td></tr>
        </table>
    ";

    $destinatario = $listaEmails;
    // $destinatario = array("lotharthesavior@gmail.com");

    $enviado = enviar_email($remetente, $destinatario, $assunto, $conteudo);
}

//Funo de correo das vinculaes provenientes do obrid pai e vinculado
//Funo executada no dia 04-12-2013
function corrigeNumeracaoIdsComVinculos()
{

    echo '<p> Dentro da funo: </p>' . __FILE__ . "::" . __FUNCTION__ . "()";

    global $db;

    // Segundo o Adonias, as obras que tem algum tipo de vnculo tem o seu obrid (no momento da criao) a numerao acima de 1.000.000
    // Por isso,  usado o filtro obrid > 1000000
    //Coleo das obras com ids vinculados - 52 rows
    $sql_obras_vinculadas = "
                                SELECT
                                   obrid , obridvinculado
                                FROM  obras2.obras
                                WHERE obrstatus = 'A'
                                  AND obridvinculado is not null
                                  AND obrid > 1000000
                                ORDER BY obridvinculado
                            ";

    $obras_a_renumerar = $db->carregar($sql_obras_vinculadas);

    $resultado_final = array();

    if ($obras_a_renumerar) {
        foreach ($obras_a_renumerar as $key => $value) {
            $res = array();

            $idObraNova = $value['obrid'];
            $idObraOriginal = $value['obridvinculado'];

            //Para executar novamente, descomente a linha abaixo
            //$novo_id_obra_original = posAcaoCadastroObraVinculada($idObraNova, $idObraOriginal);

            $res['o_que_era'] ['obra_original'] = 'obrid: ' . $value['obridvinculado'] . ' => ' . 'obridvinculado: ' . ' => NULL';
            $res['o_que_era'] ['obra_nova'] = 'obrid: ' . $value['obrid'] . ' => ' . 'obridvinculado: ' . ' => ' . $value['obridvinculado'];
            $res['como_ficou']['obra_original'] = 'obrid: ' . $novo_id_obra_original . ' => ' . 'obridvinculado: ' . ' => ' . $value['obridvinculado'];
            $res['como_ficou']['obra_nova'] = 'obrid: ' . $value['obridvinculado'] . ' => ' . 'obridvinculado: ' . ' => ' . $novo_id_obra_original;
            $resultado_final[$key] = $res;
        }
    } else {
        echo '<p>No existem obras a corrigir a numerao.</p>';
    }

    echo '<pre>';
    print_r($resultado_final);
    echo '</pre>';

    die('<script>
            alert(\'Os dados foram atualizados com sucesso !\');
         </script>');

    return true;
}

/**
 *
 * @param int $idObraNova - Obra nova
 * @param int $idObraOriginal - Obra Paralizada
 * @description
 * Atendendo a demanda #224255
 * Ajusta a numerao das obras vinculadas.
 *  - A obra que foi criada, que ser a continuao, deve ser numerada com o ID que a obra tinha no sistema.
 *  - A obra que existia e passou a ser histórico, deve ter seu id alterado para 90XXYYYYY
 *      Aonde XX  o numero da vinculao 01 pra primeira, 02 pra segunda etc, e YYYYY sao 5 digitos pro ID que a obra tinha no sistema.
 *      Se tinha 4 digitos, completar com 0 a esquerda.
 */
function posAcaoCadastroObraVinculada($idObraNova, $idObraOriginal)
{

    global $db;
//
//        //Guarda o obrid da nova obra para o UPDATE
//        $idObraNova_hist = $idObraNova;
//        //Seta o obrid da nova obra para o id da obra antiga, o objetivo  manter a referencia.
//        $idObraNova = $idObraOriginal;
//
//        //Contar quantas vezes a obra foi vinculada
//        $sql_count = 'SELECT count(*) as qtd FROM obras2.obras WHERE obridvinculado = ' . $idObraNova;
//        $qtds_vinculos = $db->pegaUm($sql_count);
//        $qtds_vinculos = ($qtds_vinculos == false) ? 0 : $qtds_vinculos;
//        $qtds_vinculos_veri = $qtds_vinculos;
//
//        //Medir o tamanho da string do obrid da obra original e colocar zeros a esquerda at que o 'tamanho' do ID seja de 5 digitos
//        while (strlen($idObraOriginal) < 5) {
//            $idObraOriginal = '0' . $idObraOriginal;
//        }
//
//        //Fazer a verificao se j existe um id j cadastrado com o 90xxyyyyy
//        $qtd_vinculo_cad = '01';
//        $qtd_vinculos_ja_cad = 0;
//        while ($qtds_vinculos_veri != 0) {
//
//            $idObraOriginal_veri = '90' . $qtd_vinculo_cad . $idObraOriginal;
//            $sql_verificacao = "SELECT obrid FROM obras2.obras WHERE obrid = " . $idObraOriginal_veri;
//            $veri_existencia_vinc = $db->pegaUm($sql_verificacao);
//
//            if ($veri_existencia_vinc === false) {
//                $qtds_vinculos_veri = 0;
//            } else {
//                $qtd_vinculos_ja_cad++;
//                $qtd_vinculo_cad = ($qtd_vinculos_ja_cad < 10) ? '0' . $qtd_vinculos_ja_cad : $qtd_vinculos_ja_cad;
//            }
//        }
//
//        //Completa o novo OBRID da obra antiga com o '90' na frente e a quantidade de vinculaes conforme a regra definida
    $idObraTmp = '9' . $idObraNova;

    $sql_update_id = "
            -- Troca os IDs
            UPDATE obras2.obras SET obrid = $idObraTmp, obrstatus = 'P' WHERE obrid =  $idObraOriginal;
            UPDATE obras2.obras SET obrid = $idObraOriginal, obrstatus  = 'A' WHERE obrid = $idObraNova;
            UPDATE obras2.obras SET obrid = $idObraNova WHERE obrid = $idObraTmp;

            UPDATE obras2.obras SET obridvinculado = $idObraOriginal WHERE obrid =  $idObraNova;
        ";

    $db->executar($sql_update_id);
    $db->commit();

    migraDadosObraVinculada($idObraOriginal, $idObraNova);
    return $idObraOriginal;
}

/**
 * Funo responsvel por migrar alguns dados da obra original para a nova obra
 * - Migra os registros de atividades
 * - Migra as Restrições e inconformidades
 * - Migra os Documentos
 *
 * @param $idObraNova
 * @param $idObraOriginal
 */
function migraDadosObraVinculada($idObra, $idObraVinculada)
{

    global $db;

    // Migra os registros de validao
    $sql = "SELECT * FROM obras2.validacao WHERE obrid = {$idObraVinculada}";
    $val = $db->pegaLinha($sql);
    if ($val) {
        $val['vldid'] = null;
        $val['obrid'] = $idObra;
        $validao = new Validacao();
        $validao->popularDadosObjeto($val);
        $validao->salvar();
    }

    // Migra os registros de atividades
    $sql = "SELECT * FROM obras2.registroatividade WHERE obrid = {$idObraVinculada}";
    $ras = $db->carregar($sql);

    if ($ras) {
        foreach ($ras as $ra) {
            $registro = new RegistroAtividade();
            $ra['rgaid'] = null;
            $ra['obrid'] = $idObra;
            $registro->popularDadosObjeto($ra);
            $registro->salvar();
        }
    }

    // Migra os registros de restrições e inconformidades
    $sql = "SELECT * FROM obras2.restricao WHERE obrid = {$idObraVinculada}";
    $res = $db->carregar($sql);

    if ($res) {
        foreach ($res as $re) {
            $restricao = new Restricao();
            $re['rstid'] = null;
            $re['obrid'] = $idObra;
            $restricao->popularDadosObjeto($re);
            $restricao->salvar();
        }
    }

    $sql = "SELECT * FROM obras2.filarestricao WHERE obrid = {$idObraVinculada}";
    $fres = $db->carregar($sql);

    if ($fres) {
        foreach ($fres as $fre) {
            $filaRestricao = new FilaRestricao();
            $fre['frtid'] = null;
            $fre['obrid'] = $idObra;
            $filaRestricao->popularDadosObjeto($fre);
            $filaRestricao->salvar();
        }
    }

    $sql = "SELECT * FROM obras2.solicitacao_vinculada WHERE obrid = {$idObraVinculada}";
    $slvs = $db->carregar($sql);

    if ($slvs) {
        foreach ($slvs as $slv) {
            $s = new SolicitacaoVinculada($slv['slvid']);
            $s->obrid = $idObra;
            $s->salvar();
        }
    }

    $db->commit();

    // Migra os Documentos
    $sql = " SELECT oa.*, a.*
             FROM
                    obras2.obras_arquivos oa
             JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
             JOIN public.arquivo      a ON a.arqid = oa.arqid
             WHERE oa.oarstatus = 'A'
               AND oa.obrid = {$idObraVinculada}
               AND (arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')";

    $docs_originais = $db->carregar($sql);

    if (!empty($docs_originais)) {
        $erro = false;
        foreach ($docs_originais as $key => $value) {
            //obras2.obras_arquivos
            $oardata = !empty($value['oardata']) ? "'" . $value['oardata'] . "'" : 'NULL';
            $oardtinclusao = !empty($value['oardtinclusao']) ? "'" . $value['oardtinclusao'] . "'" : 'NULL';
            $sql_ioa = "INSERT INTO obras2.obras_arquivos( obrid, tpaid, arqid, oardesc, oardata, oardtinclusao, oarstatus)
                        VALUES ( $idObra, " . $value['tpaid'] . ", {$value['arqid']}, '" . addslashes($value['oardesc']) . "', $oardata, $oardtinclusao, '" . $value['oarstatus'] . "' )RETURNING oarid;";
            try {
                $oarid = $db->pegaUm($sql_ioa);
            } catch (Exception $ex) {
                $erro = true;
                break;
            }
        }
        if (!$erro) {
            $db->commit();
        } else {
            $db->rollback();
        }
    }
}


function pegaEstadoAtualDocumento($docid)
{
    global $db;
    if ($docid) {
        $docid = (integer)$docid;
        $sql = "SELECT esdid
                        FROM   workflow.documento
                        WHERE  docid = {$docid}";
        $estado = $db->pegaUm($sql);
        return $estado;
    } else {
        return false;
    }
}

function pegaPerfil($usucpf)
{
    global $db;

    $sql = "SELECT
                    pu.pflcod
            FROM seguranca.perfil AS p
            LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
            WHERE
                    p.sisid = '{$_SESSION['sisid']}'
                    AND pu.usucpf = '$usucpf'";


    $pflcod = $db->pegaUm($sql);
    return $pflcod;
}

function percentualSupEmpresa($empid)
{

    $sueid = $_SESSION['obras2']['sueid'];
    $param = array();
    $param['not(obridpai)'] = true;
    $obras = new Obras();
    $arObrid = $obras->pegaIdObraPorEmpid($empid, $param);

    $total = array();
    $total['vlrExecSobreObra'] = 0;
    $total['percExecSobreObra'] = 0;
    $total['obrid'] = 0;

    foreach ($arObrid as $obrid) {
        $obra = new Obras($obrid);
        $obraMi = ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) ? true : false;
        $param = array();
        $param['empid'] = $empid;
        $param['is(sueid)'] = true;
        $supervisao = new Supervisao();
        $supid = $supervisao->pegaSupidByObraAndSueid($obrid, $sueid);
        $ultimoSupid = $supervisao->pegaUltSupidByObra($obrid, $param);
        $vistoriaUnidade = $supervisao->pegaUltimaVistoriaUnidade($obrid);

        $supervisaoItem = new SupervisaoItem();
        $dadosEtapa = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid));
        $total['obrid'] = $obrid;

        foreach ($dadosEtapa as $etapa) {
            $total['vlrExecSobreObra'] += $etapa['spivlrfinanceiroinfsupervisor'];
            $total['percExecSobreObra'] = ($etapa['ocrvalorexecucao'] > 0 ? ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100 : 0);
        }
    }

    return array(
        'percental' => array(
            'empresa' => (float)number_format($total['percExecSobreObra'], 2, '.', ''),
            'unidade' => (float)$vistoriaUnidade['percentual'],
            'obrid' => $total['obrid']
        )
    );
}

/**
 * Pega resposta da questo do questionario
 * respondido na aba questionario
 * @param string $jsonStringfy
 * @param string $resposta
 * return String|null
 */
function pegaRespostaQuestao($jsonStringfy, $resposta)
{

    $qstctrlobs = json_decode($jsonStringfy);
    switch ($resposta) {
        case 't':
            $resp = ($qstctrlobs->S ? 'Sim' : 'No');
            break;
        case 'f':
            $resp = ($qstctrlobs->N ? 'No' : 'Sim');
            break;
        case 'n':
            $resp = ($qstctrlobs->NA ? 'No se aplica' : '');
            break;
        default:
    }

    return $resp;
}

/**
 * Retorna a tupla com a resposta de risco da subquestao
 * @param array $linhaQ
 * @return array|boolean
 */
function getRespostaRisco(array $linhaQ)
{
    $rqSup = new RiscoQuestionarioSupervisao();
    if (isset($linhaQ['sqtid'])) {
        $linha = $rqSup->pegaRespostaPorSqtid($_SESSION['obras2']['sueid'], $linhaQ['sqtid']);
    } else {
        $linha = $rqSup->pegaRespostaPorSueidMrqid($_SESSION['obras2']['sueid'], $linhaQ['mrqid']);
    }

    unset($rqSup);
    if (count($linha)) {
        return (object)$linha;
    }
    return false;
}

/**
 * printa html com subquestao e com auxilio da funco "getRespostaRisco"
 * pega a resposta e ja marca no subquesto
 *
 * @param array $linhaQ
 * $return void(0)
 */
function subQuestao(array $linhaQ)
{

    //ver($linhaQ); die;
    if ($linhaQ['rstitem'] == 'R') {
        $marcadorN = (isset($linhaQ['sqtid'])) ? $linhaQ['sqtid'] : '';

        $resposta = getRespostaRisco($linhaQ);
        $style = ($resposta->sueriscousuario == 'S') ? 'style="display:;"' : 'style="display:none;"';
        $checkedT = ($resposta->sueriscousuario == 'S') ? 'checked="checked"' : '';
        $checkedF = ($resposta->sueriscousuario == 'N') ? 'checked="checked"' : '';

        echo '<tr><td>&nbsp;&nbsp;&nbsp;O serviço executado em desconformidade com o projeto pode oferecer risco  segurana do usurio?';
        echo '<input type="hidden" name="sqtid[]" value="' . $marcadorN . '-' . $linhaQ['qstid'] . '" />';
        echo '<input type="hidden" name="mrqid[]" value="' . $linhaQ['mrqid'] . '" />';
        echo '&nbsp;&nbsp;&nbsp;Sim <input type="radio" ' . $checkedT . ' class="input-check" rel="' . $marcadorN . '" name="sueriscousuario' . $marcadorN . '" value="S"/>';
        echo '&nbsp;&nbsp;&nbsp;No <input type="radio" ' . $checkedF . ' class="input-check" rel="' . $marcadorN . '" name="sueriscousuario' . $marcadorN . '" value="N"/>';
        echo '</td></tr>';
        echo '<tr ' . $style . ' id="div-suetiporiscoobs' . $marcadorN . '"><td>
                <table class="tabela" style="width:95%">
                <tr><td class="SubTituloEsquerda">Tipo de risco observado:</td></tr>
                <tr><td>' . campo_textarea(
            'suetiporiscoobs' . $marcadorN,
            'S',
            'S',
            '',
            '70',
            '5',
            '5000',
            '',
            0,
            '',
            '',
            false,
            $resposta->suetiporiscoobs
        ) . '</td></tr>
                </table>
              </td></tr>';
    }
}

/**
 * Pega o modelo de restricao  e monta um array de restricoes
 * ou inconformidades por qtsid ou rsqid
 * para questao que tem subquesto ou no
 *
 * @param integer $sueid
 * @return array $collection
 */
function pegaColecaoRestricao($sueid)
{
    $modeloRestricaoQuestionario = new ModeloRestricaoQuestionario();
    $collection = array();
    $arModeloRestricao = $modeloRestricaoQuestionario->carregaIdRelacaoPorSueid($sueid);
    //ver($arModeloRestricao);
    foreach ($arModeloRestricao as $modeloRestricao) {
        if (!empty($modeloRestricao['qtsid'])) {
            $arr = $modeloRestricaoQuestionario->pegaQuestaoPorQtsid($modeloRestricao['qtsid']);
            $arr['mrqid'] = $modeloRestricao['mrqid'];
            $mrq = new ModeloRestricaoQuestionario();
            $mrq->carregarPorId($modeloRestricao['mrqid']);
            $arr['rstitem'] = $mrq->rstitem;

            $collection[] = $arr;
        } elseif (!empty($modeloRestricao['rsqid'])) {
            $arr = $modeloRestricaoQuestionario->pegaQuestaoPorRsqid($modeloRestricao['rsqid']);
            $arr['mrqid'] = $modeloRestricao['mrqid'];
            $mrq = new ModeloRestricaoQuestionario();
            $mrq->carregarPorId($modeloRestricao['mrqid']);
            $arr['rstitem'] = $mrq->rstitem;

            $collection[] = $arr;
        }
        unset($mrq);
    }

    foreach ($collection as $k => $c) {
        if ($c['rstitem'] != 'R') {
            unset($collection[$k]);
        }
    }

    return $collection;
}

/**
 * Retorna TRUE se a obra for MI ou FALSE se no for MI
 * @param int $obrid
 * @return boolean
 */
function obraMi($obrid)
{
    $obra = new Obras($obrid);

    if ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) {
        return true;
    } else {
        return false;
    }
}



/**
 * Retorna TRUE se a obra estiver incluida na portaria 348/2016
 * @param int $obrid
 * @return boolean
 */
function obraPortaria($obrid)
{
    global $db;
    $sql = ("SELECT arqid from obras2.obrasPortariaBloqueio opb
                 INNER JOIN obras2.portaria prt ON prt.prtid = opb.prtid 
             WHERE obrid = ".$obrid);
    $result = $db->pegaUm($sql);

    if ($result) {
        return $result;
    } else {
        return false;
    }
}

/**
 * Retorna os dados da empresa MI e da tecnologia
 * @param $obrid
 */
function pegaDadosMi($obrid)
{
    global $db;

    $sql = "SELECT o.obrid, e.emicnpj, e.emidsc, t.tminome, t.tmiid  FROM obras2.obras o
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN obras2.empresami_uf euf ON euf.estuf = ed.estuf AND euf.eufstatus = 'A'
            JOIN obras2.tecnologiami t ON t.tmiid = euf.tmiid AND t.tmistatus = 'A'
            JOIN obras2.empresami e ON e.emiid = euf.emiid AND e.emistatus = 'A'
            WHERE /*o.obridpai is null and*/ o.obrstatus = 'A' AND o.tpoid IN (104, 105) AND o.obrid = {$obrid}";

    return $db->pegaLinha($sql);
}

/**
 * Boto de tramitar o aditivo para excluso
 * @param integer $obrid
 * @param integer $tpdid
 */
function wf_botao_excluir_aditivo($obrid, $tpdid = 169)
{
    //Funo descontinuada: agora  feita na lista de opes
    return false;

    $tramita = new ObraAditivoExclusao();
    if ($result = $tramita->pegaLinhaPorObrid($obrid)) {
        $dados_wf = wf_pegarEstadoAtual($result['docid']);

        $obra = new Obras($obrid);
        $crtid = $obra->pegaContratoPorObra($obrid);
        $contrato = new Contrato($crtid);
        $dados = $contrato->getDados();

        if ($dados_wf['esdid'] == 1054 && $dados['ttaid']) {
            $estadoInicial = true;
            $dados_wf = wf_pegarEstadoInicial($tpdid);
        } else {
            $estadoInicial = false;
        }
    } else {
        $dados_wf = wf_pegarEstadoInicial($tpdid);
        $estadoInicial = true;
    }
    ?>
    <table border="0" cellpadding="3" cellspacing="0"
           style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
        <tbody>
        <tr style="background-color: #c9c9c9; text-align:center;">
            <td style="font-size:7pt; text-align:center;">
                    <span title="excluir aditivos da obra">
                        <b>Excluir Aditivos da Obra</b>
                    </span>
            </td>
        </tr>
        <tr style="text-align:center;">
            <td style="font-size:7pt; text-align:center;">
                    <span title="estado atual">
    <?php
    if ($dados_wf['esdid'] == 1054) {
        echo $dados_wf['esddsc'];
    } else {
        switch ($dados_wf['esdid']) {
            case 1059:
                if (possuiPerfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
                    echo '<a class="tramitaDeletaAditivo" href="#">' . $dados_wf['esddsc'] . '</a>';
                } else {
                    echo 'Aguardando pedido de excluso FNDE';
                }
                break;
            case 1052:
                if (possuiPerfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE))) {
                    echo '<a class="tramitaDeletaAditivo" href="#">' . $dados_wf['esddsc'] . '</a>';
                } else {
                    echo 'Aguardando confirmao do município';
                }
                break;
            case 1053:
                if ($dados_wf['esdid'] == 1053 && possuiPerfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
                    echo '<a class="tramitaDeletaAditivo" href="#">' . $dados_wf['esddsc'] . '</a>';
                } else {
                    echo 'Aguardando confirmao do FNDE';
                }
                break;
        }
    }
    ?>
                    </span>
            </td>
        </tr>
        </tbody>
    </table>
    <script type="text/javascript">
        jQuery(function () {
            var urlAction = "?modulo=principal/excluirAditivoObra&acao=A&obrid=<?php echo $obrid; ?>&esdid=<?= $dados_wf['esdid'] ?>";
            jQuery(".tramitaDeletaAditivo").click(function (e) {
                e.preventDefault();
                janela(urlAction, 850, 600, 'TramitaDeletaAditivo');
            });
        });
    </script>
    <?php
}

/**
 * Transforma matriz em array, para aplicar o implode
 * @param array $params
 * @param type $chave
 * @return array
 */
function formataArray(array $params, $chave)
{
    $output = array();
    foreach ($params as $linha) {
        array_push($output, $linha[$chave]);
    }
    return $output;
}

/**
 * Excluir as obras filhas pela obridpai e inativa os aditivos de uma obra
 * @global type $db
 * @param type $obrid
 * @param type $oaeid
 * @return boolean
 */
function apagarAditivoObra($obrid, $slcid)
{
    global $db;

    $obra = new Obras($obrid);
    $obraEmp = $obra->pegaUltimaObraPorEmpId($obra->empid);

    // Tratamento de exceo Para casos semelhantes aos da demanda 357264, quando o ultimo id, no corresponde a ultima obra e sim a original
    if (!$obraEmp['obridpai']) {
        if ($obra->empid == $obrid) {
            $obraEmp['obridpai'] = $obrid;
        }
    }

    $contrato_id = $obra->pegaContratoPorObra($obrid);

    $commit = false;

    $sqlEmpID = "SELECT obrid FROM obras2.obras WHERE empid = {$obra->empid} AND obrstatus = 'A'";
    $rsEmpID = $db->carregar($sqlEmpID);
    if (count($rsEmpID)) {
        $rsEmpID = formataArray($rsEmpID, 'obrid');
        $sqlCrtID = "SELECT min(crtid) as crtidoriginal, crtidpai as crtidatual
                     FROM obras2.obrascontrato
                     JOIN obras2.contrato using (crtid)
                     WHERE crtidpai = $contrato_id
                       AND ocrstatus = 'A'
                       AND crtidpai IS NOT NULL
                     GROUP BY contrato.crtidpai";
        //dbg(simec_htmlentities($sqlCrtID));
        $rsCrtID = $db->pegaLinha($sqlCrtID);


        if (count($rsCrtID)) {
            $updateContrato = " UPDATE obras2.contrato SET
                                crtdtassinatura   = t2.crtdtassinatura,
                                crtdttermino      = t2.crtdttermino,
                                crtprazovigencia  = t2.crtprazovigencia,
                                crtvalorexecucao  = t2.crtvalorexecucao,
                                crtpercentualdbi  = t2.crtpercentualdbi,
                                crtstatus         = t2.crtstatus,
                                entidempresa      = t2.entidempresa,
                                licid             = t2.licid,
                                orgid             = t2.orgid,
                                crtidpai          = null, -- setar nulo
                                crtdenominacao    = t2.crtdenominacao,
                                crtjustificativa  = t2.crtjustificativa,
                                ttaid             = t2.ttaid,
                                crtsupressao      = t2.crtsupressao,
                                crtdtassinaturaaditivo = t2.crtdtassinaturaaditivo,
                                arqid             = t2.arqid,
                                obrid_1           = t2.obrid_1,
                                arqidcontrato     = t2.arqidcontrato,
                                usucpf            = t2.usucpf,
                                dt_cadastro       = t2.dt_cadastro,
                                crtnumero         = t2.crtnumero
                                FROM
                                ( SELECT *
                                  FROM obras2.contrato
                                  WHERE crtid in ({$rsCrtID['crtidoriginal']})
                                ) t2
                                WHERE obras2.contrato.crtid = {$rsCrtID['crtidatual']}";
            //dbg(simec_htmlentities($updateContrato));
            $db->executar($updateContrato);

            $updateObraContrato = "UPDATE obras2.obrascontrato SET
                                    umdid                = t2.umdid,
                                    ocrqtdconstrucao     = t2.ocrqtdconstrucao,
                                    ocrdtordemservico    = t2.ocrdtordemservico,
                                    ocrdtinicioexecucao  = t2.ocrdtinicioexecucao,
                                    ocrprazoexecucao     = t2.ocrprazoexecucao,
                                    ocrdtterminoexecucao = t2.ocrdtterminoexecucao,
                                    ocrvalorexecucao     = t2.ocrvalorexecucao,
                                    ocrcustounitario     = t2.ocrcustounitario,
                                    ocrpercentualdbi     = t2.ocrpercentualdbi,
                                    ocrstatus            = t2.ocrstatus,
                                    ocraditivado         = t2.ocraditivado,
                                    arqidos              = t2.arqidos,
                                    arqidcusto           = t2.arqidcusto
                                    FROM
                                    ( SELECT *
                                      FROM obras2.obrascontrato
                                      -- talvez tenha que adicionar o obrid pra garantir unicidade
                                      WHERE crtid = {$rsCrtID['crtidoriginal']}
                                      AND obrid = {$obrid}
                                    ) t2
                                    WHERE obras2.obrascontrato.crtid = {$rsCrtID['crtidatual']}";
            //dbg(simec_htmlentities($updateObraContrato));
            $db->executar($updateObraContrato);

            $subQueryCrtid = "SELECT crtid FROM obras2.contrato WHERE crtstatus = 'A' AND crtidpai = {$rsCrtID['crtidatual']}";
            $subQueryObras = "SELECT obrid FROM obras2.obras    WHERE obridpai = {$obraEmp['obridpai']} AND obrstatus = 'A'";
            $rsQCrtid = $db->carregar($subQueryCrtid);
            $rsQObras = $db->carregar($subQueryObras);


            $rsQCrtid = formataArray($rsQCrtid, 'crtid');
            $rsQObras = formataArray($rsQObras, 'obrid');

            if (count($rsQCrtid) && count($rsQObras)) {
                $updateInativar = "UPDATE obras2.obrascontrato SET ocrstatus = 'I'
                                    WHERE crtid in (" . implode(',', $rsQCrtid) . ")
                                    AND ocrstatus = 'A'
                                    AND obrid in (" . implode(',', $rsQObras) . ")";
                //dbg(simec_htmlentities($updateInativar));
                $db->executar($updateInativar);

                $inativaContrato = "UPDATE obras2.contrato SET crtstatus = 'I' WHERE crtidpai = {$rsCrtID['crtidatual']} AND crtstatus = 'A'";
                //dbg(simec_htmlentities($inativaContrato));
                $db->executar($inativaContrato);

                $updateObras = "UPDATE obras2.obras SET obrstatus = 'I' WHERE obridpai = {$obraEmp['obridpai']} AND obrstatus = 'A'";
                //dbg(simec_htmlentities($updateObras));
                $db->executar($updateObras);

                registroAtividadeAditivoExclusao($obraEmp, $slcid);
                $commit = true;
            }
        }
    }

    if ($commit) {
        $db->commit();
        // $db->rollback();
        return true;
    } else {
        $db->rollback();
        return false;
    }
}

/**
 * Registra atividade para excluso do aditivo da obra
 * @param array $obraEmp
 * @param integer $oaeid
 * @return type
 */
function registroAtividadeAditivoExclusao(array $obraEmp, $slcid)
{

    global $db;
    include_once APPRAIZ . 'obras2/classes/modelo/RegistroAtividade.class.inc';
    $sql = "select docid from obras2.solicitacao where slcid = $slcid";
    $docid = $db->pegaUm($sql);


    $regAtividade = new RegistroAtividade();

    $history = wf_pegarHistorico($docid);
    $registroStr = '';
    foreach ($history as $view) {
        $registroStr .= str_replace(
            'Enviar para ',
            '',
            $view['esddsc']
        ) . " por: {$view['usunome']} em: {$view['htddata']} \n\r";
    }
    $registroStr = substr($registroStr, 0, -1);

    $arDado = array(
        'obrid' => $obraEmp['obridpai'],
        'rgaautomatica' => true,
        'rgadscsimplificada' => 'Excluso dos aditivos da obra (' . $obraEmp['obrid'] . ') - ' . $obraEmp['obrnome'],
        'rgadsccompleta' => "Aditivo de contrato excluido."
    );
    $arCamposNulo = array();
    return $regAtividade->popularDadosObjeto($arDado)->salvar(true, true, $arCamposNulo);
}

/*
                     * Monta um arquivo Excel a partir de uma Query
                     *
                     * @param $sql String Query ou um Array a ser executada
                     * @param $arquivo String prefixo do arquivo a ser gerado
                     * @param $cabecalho Array Opcional, nome dos campos no cabecalho do arquivo
                     * @param $formatocoluna Array pode ser n (Numero) ou s (String)
                     * @return void
                     */

function _sql_to_excel($sql, $arquivo, $cabecalho = "", $formatocoluna = "", $type = 'D')
{
    // este método transforma uma query em excel
    global $nomeDoArquivoXls;
    $nomeDoArquivoXls = "SIMEC_" . date("His") . "_" . $arquivo;
    include_once APPRAIZ . 'obras2/classes/modelo/GerarExcelObras.class.inc';

    $xls = new GerarExcelObras();

    $RS = !is_array($sql) ? $this->carregar($sql) : $sql;
    $nlinhas = $RS ? count($RS) : 0;
    if (!$RS) {
        $nl = 0;
    } else {
        $nl = $nlinhas;
    }
    if ($nlinhas > 0) {
        //Monta Cabealho
        if (is_array($cabecalho)) {
            for ($i = 0; $i < count($cabecalho); $i++) {
                $xls->MontaConteudoString(0, $i, $cabecalho[$i]);
            }
        } else {
            $col = 0;
            $lin = 0;
            foreach ($RS[0] as $k => $v) {
                $xls->MontaConteudoString($lin, $col, $k);
                $col++;
            }
        }
        //Monta Listagem
        for ($i = 0; $i < $nlinhas; $i++) {
            $lin = $i + 1;
            $col = 0;
            foreach ($RS[$i] as $k => $v) {
                if (!empty($formatocoluna) && $formatocoluna[$col] == 'n') {
                    $xls->MontaConteudoNumero($lin, $col, $v);
                } else {
                    $xls->MontaConteudoString($lin, $col, $v);
                }
                $col++;
            }
        }
        return $xls->GeraArquivo($type);
    }
}

/**
 * Transforma array em csv
 * @global cls_banco $db
 * @param array $arr
 * @return string|false
 */
function _sql_to_csv(array $arr)
{
    include_once APPRAIZ . 'obras2/classes/modelo/ArrayToCSV.class.inc';

    $csv = new ArrayToCSV();
    $csvFile = $csv->convert($arr);

    return (strlen($csvFile)) ? $csvFile : false;
}

function formata_data_sql_null($data)
{
    if ($data) {
        return substr($data, 6, 4) . '-' . substr($data, 3, 2) . '-' . substr($data, 0, 2);
    } else {
        return null;
    }
}

function pegaAvancoObra($obrid)
{
    global $db;

    $sql = "SELECT ((SELECT sup.percentual FROM obras2.supervisao s
                    JOIN (
                      SELECT distinct
                        s.*,
                        ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total FROM obras2.itenscomposicaoobra i INNER JOIN obras2.cronograma cro ON cro.croid = i.croid LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL WHERE i.icostatus = 'A' AND i.relativoedificacao = 'D' AND cro.obrid = {$obrid} AND cro.crostatus IN ('A','H') AND cro.croid = s.croid AND i.obrid = {$obrid}) as percentual
                      FROM
                          obras2.supervisao s
                      WHERE
                        s.obrid = {$obrid} AND
                        s.emsid IS NULL AND s.smiid IS NULL AND
                        s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S'
                      ORDER BY
                      s.supdata DESC LIMIT 3
                      ) as sup ON sup.supid = s.supid
                    ORDER BY s.supdata DESC LIMIT 1)
                    -

                    CASE WHEN (
                        SELECT COUNT(*) FROM obras2.supervisao s
                        WHERE
                          s.obrid = {$obrid} AND
                          s.emsid IS NULL AND s.smiid IS NULL AND
                          s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S' AND
                          s.usucpf IS NOT NULL AND s.rsuid = 1
                      ) = 1 THEN 0
                    ELSE
                        (SELECT sup.percentual FROM obras2.supervisao s
                        JOIN (
                        SELECT distinct
                        s.*,
                        ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total FROM obras2.itenscomposicaoobra i INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL WHERE i.icostatus = 'A' AND i.relativoedificacao = 'D' AND cro.obrid = {$obrid} AND i.obrid = {$obrid}) as percentual
                        FROM
                        obras2.supervisao s
                        WHERE
                        s.obrid = {$obrid} AND
                        s.emsid IS NULL AND s.smiid IS NULL AND
                        s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S' AND
                        s.usucpf IS NOT NULL AND s.rsuid = 1
                        ORDER BY
                        s.supdata DESC LIMIT 3
                        ) as sup ON sup.supid = s.supid
                        ORDER BY s.supdata ASC LIMIT 1)
                        END
                    ) as avanco";

    $percent = $db->pegaUm($sql);
    return $percent;
}

function alertaObraMi($obrid)
{
    if (obraMi($obrid)) {
        $ordemServico = new OrdemServicoMI();
        $dados = $ordemServico->carregarPorObridETomid($obrid, 1);
        if ($dados) {
            $estado = pegaEstadoOsMi($dados['docid']);
        } else {
            $estado = null;
        }
        // So exibe quando a OS estiver aguardando aceite quando ainda no foi emitida
        if ($estado == 904 || $estado === null) {
            return '
                <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
                    <tbody>
                        <tr>
                            <td width="100%" align="center">
                                <label class="TituloTela" style="color:#FA0000;"><img src="/imagens/atencaoVermelho.png" /> Essa funcionalidade depende do aceite da O.S de Execução</label>
                            </td>
                        </tr>
                    </tbody>
                </table>
            ';
        }
    }
    return '';
}

function pegaDadosTecnologiaMi($obrid, $sueid, $sosid = null)
{
    global $db;
    if (obraMi($obrid)) {
        $dadosMI = pegaDadosMi($obrid);

        $supLegado = 0;
        if ($sueid) {
            $sql = "SELECT COUNT(*) FROM obras2.questaosupervisao qs
                JOIN obras2.questao q ON q.qstid = qs.qstid AND q.qstescopo = 'SE'
                WHERE qs.qtsstatus = 'A' AND qs.sueid = {$sueid}";
            $supLegado = $db->pegaUm($sql);
        }

        $sueid = (!$sueid) ? 0 : $sueid;
        $sosid = (!$sosid) ? 0 : $sosid;

        $sql = "SELECT COUNT(os.sosdtinicio) FROM obras2.supervisao_os os
                LEFT JOIN obras2.supervisaoempresa sue ON os.sosid = sue.sosid
                WHERE (sue.sueid = {$sueid} OR os.sosid = {$sosid}) AND os.sosdtinicio >= '2014-06-30'";
        $supVigor = $db->pegaUm($sql);

        if (obraMi($obrid)) {
            $dadosMI = pegaDadosMi($obrid);
            $dadosMI['qstescopo'] = 'SE';
            // Regra enta em vigor
            if ($supLegado < 1 && $supVigor > 0) {
                switch ($dadosMI['tmiid']) {
                    case 1:
                        $dadosMI['qstescopo'] = 'QSPVC';
                        break;
                    case 2:
                        $dadosMI['qstescopo'] = 'QSSF';
                        break;
                    case 3:
                        $dadosMI['qstescopo'] = 'QSMVC';
                        break;
                }
            }
        }
        return $dadosMI;
    }
    return false;
}

/**
 * Convert a PHP array into CSV
 * https://github.com/jonseg/array-to-csv
 *
 */
class arrayToCsv
{

    protected $delimiter;
    protected $text_separator;
    protected $replace_text_separator;
    protected $line_delimiter;

    public function __construct(
        $delimiter = ";",
        $text_separator = '"',
        $replace_text_separator = "'",
        $line_delimiter = "\n"
    ) {
        $this->delimiter = $delimiter;
        $this->text_separator = $text_separator;
        $this->replace_text_separator = $replace_text_separator;
        $this->line_delimiter = $line_delimiter;
    }

    public function convert($input)
    {
        $lines = array();
        foreach ($input as $v) {
            $lines[] = $this->convertLine($v);
        }
        return implode($this->line_delimiter, $lines);
    }

    private function convertLine($line)
    {
        $csv_line = array();
        foreach ($line as $v) {
            $csv_line[] = is_array($v) ?
                $this->convertLine($v) :
                $this->text_separator . str_replace(
                    $this->text_separator,
                    $this->replace_text_separator,
                    $v
                ) . $this->text_separator;
        }
        return implode($this->delimiter, $csv_line);
    }
}

function enviaEmailCumprimentoObjeto()
{
    global $db;
    return true;


    $obrid = $_SESSION['obras2']['obrid'];

    $sql = "select * from workflow.acaoestadodoc acao join workflow.estadodocumento est on est.esdid = acao.esdiddestino where acao.aedid = " . $_REQUEST['aedid'];
    $dados = $db->pegaLinha($sql);
    $conteudo_interno = "<p>O cumprimento do objeto da obra {$obrid}, foi tramitado para a Situação \"{$dados[esddsc]}\", por {$_SESSION['usunome']} na data " . date("d/m/Y") . "</p>";


    $conteudo = '<html>
                                        <head>
                                            <title></title>
                                            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                                            <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                                        </head>
                                        <body>
                                            <table style="width: 100%;">
                                                <thead>
                                                    <tr>
                                                        <td style="text-align: center;">
                                                            <p><br/>
                                                            <b>MINISTRIO DA EDUCAO</b><br/>
                                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAO - FNDE<br/>
                                                            DIRETORIA DE GESTO, ARTICULAO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                            COORDENAO GERAL DE IMPLEMENTAO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                            SBS Q.2 Bloco F Edifcio FNDE - 70.070-929 - Braslia, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: right; padding: 40px 0 0 0;">

                                                        </td>
                                                    </tr>
                                                </thead>
                                                <tbody>
                                                    <tr>
                                                        <td style="line-height: 15px;">

                                                        </td>
                                                    </tr>
                                                    <tr>
                                                     <td style="text-align:center">
                                                          ' . $conteudo_interno . '
                                                     </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="padding: 10px 0 0 0;">
                                                                Atenciosamente,
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: center; padding: 10px 0 0 0;">
                                                                                                                                <br />
                                                                <b>Fbio Lcio de Almeida Cardoso<b>
                                                                <br />
                                                                Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                                <br />
                                                                CGIMP/DIRPE/FNDE/MEC
                                                        </td>
                                                    </tr>
                                                </tbody>
                                                <tfoot>

                                                </tfoot>
                                            </table>
                                        </body>
                                    </html>';


    $remetente = array(
        "nome" => "Alteração de estado do Cumprimento do objeto",
        "email" => "monitoramento.obras@mec.gov.br"
    );
    $assunto = "Alteração de estado do Cumprimento do objeto";

    $destinatario = array();
    $destinatario[] = array(
        'usunome' => 'Marcus Rocha',
        'usuemail' => 'marcus.rocha@mec.gov.br'
    );


    //return enviar_email($remetente, $destinatario, $assunto, $conteudo);
    return true;
}


function getUltimaEdicaoCronograma($obrid)
{
    global $db;
    $sql = "SELECT
                    MAX (r.rgadtinclusao) rgadtinclusao
                FROM obras2.registroatividade  r
                WHERE
                    r.rgadscsimplificada = 'Atualizao dos dados do cronograma.'
                    AND r.rgadtinclusao < COALESCE( (SELECT MIN(rgadtinclusao) rgadtinclusao FROM obras2.registroatividade WHERE  rgadscsimplificada = 'Edio do cronograma.' AND obrid = r.obrid), NOW())
                    AND obrid = $obrid
                GROUP BY obrid
                ";
    $data = $db->pegaUm($sql, 0, 3600);

    return ($data) ? $data : false;
}

function verificaValorCronogramaSupervisao($obrid, $supid)
{
    global $db;
    $sql = "SELECT

                    (SELECT array_to_string(array_agg(icovlritem), ',')
                      FROM ( SELECT ic.icovlritem
                        FROM obras2.supervisao s
                        JOIN obras2.supervisaoitem  si ON si.supid = s.supid
                        JOIN obras2.itenscomposicaoobra  ic ON ic.icoid = si.icoid
                        WHERE s.supid = $supid ORDER BY ic.itcid) as f)

                    <>

                    (SELECT array_to_string(array_agg(icovlritem), ',')
                      FROM (SELECT ic.icovlritem FROM obras2.obras o
                        JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
                        JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A'
                        WHERE o.obrid = $obrid ORDER BY ic.itcid) as f)
                ";
    $data = $db->pegaUm($sql, 0, 600);

    return ($data == 't') ? true : false;
}

function posAcaoCriaObraFluxoVinculada($obridVinculada, $slvid)
{
    include_once APPRAIZ . "obras2/classes/modelo/Validacao.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/RegistroAtividade.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/Restricao.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/FilaRestricao.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/SolicitacaoVinculada.class.inc";

    global $db;

    $obraVinculada = new Obras($obridVinculada);

    $slv = new SolicitacaoVinculada($slvid);

    $percentual = $obraVinculada->pegaPercentualExecucao($obraVinculada->obrid); //se ja ouver mais de 1 obra vinculada, precisa calcular a porcentagem total
    if (empty($percentual)) {
        $percentual = $slv->slvpercnovocontrato; // se for a primeira vinculao, o percentual  o que foi executado na vistoria.
    }

    $insertObra = "
            INSERT INTO obras2.obras
            SELECT

              NEXTVAL('obras2.obras_obrid_seq'), -- obrid
              endid,
              empid,
              iexid,
              entid,
              tobid,
              tpoid,
              cloid,
              tooid,
              obrnome,
              obrstatusinauguracao,
              obrdsc,
              obrdtinicio,
              obrdtfim,
              obrvalorprevisto,
              obrstatus,
              arqid,
              obrdtvistoria,
              obrnumprocessoconv,
              obranoconvenio,
              numconvenio,
              frpid,
              null, -- staid
              null, -- obridpai
              preid,
              null, -- obrdtultvistoria
              null, -- obrpercentultvistoria
              obrid_1,
              null, -- docid
              {$percentual},-- obrperccontratoanterior
              $obridVinculada, -- obridvinculado
              NOW(), -- obrdtinclusao
              null, -- obrcronogramaservicocontratado
              null, -- obrcronogramaservicocontratadojustificativa
              obrdtcarga,
              obrsndocumentos,
              obrsnfotos,
              true, -- obrtravaedicaocronograma
              '{$_SESSION['usucpf']}', -- usucpfinclusao
              'N', -- obrconcluida
              null, -- stiid
              null, -- medidasexcecao
              null -- strid
            FROM obras2.obras WHERE obrid = $obridVinculada RETURNING obrid;
        ";

    $obrid = $db->pegaUm($insertObra);
    $docid = criarDocidObra($obrid);

    $obraVinculada->obrstatus = 'P';
    $obraVinculada->salvar();
//    $obraVinculada->commit();

    if ($obraVinculada->preid) {
        $sql = "UPDATE obras.preobra SET obrid = {$obrid} WHERE preid = {$obraVinculada->preid} AND prestatus = 'A'";
        $db->executar($sql);
    }

    $db->executar("UPDATE workflow.documento SET esdid = " . ESDID_OBJ_LICITACAO . "WHERE docid = $docid");

    $idObraNova = $obrid;
    $idObraOriginal = $obridVinculada;

    posAcaoCadastroObraVinculada($idObraNova, $idObraOriginal);

    wf_alterarEstado($docid, AEDID_OBJ_LICITACAO_LICITACAO, '', array());

    return true;
}


function verificaObraMiContratoRescindido($obrid)
{
    global $db;
    $sql = "SELECT
                    *
                FROM obras2.obras o
                JOIN workflow.documento d ON d.docid = o.docid
                WHERE o.obrid = $obrid AND d.esdid = 691 AND 3 IN (

                    SELECT
                        h.tplid
                    FROM obras2.supervisao s
                    INNER JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                    INNER JOIN obras2.historicoparalisacao  h ON h.supidparalisacao = s.supid
                    WHERE
                        s.obrid = o.obrid
                        AND s.emsid IS NULL
                        AND s.smiid IS NULL
                        AND s.supstatus = 'A'::bpchar
                        AND s.validadapelosupervisorunidade = 'S'::bpchar
                        AND s.usucpf IS NOT NULL
                        AND s.rsuid = 1
                    ORDER BY supdata DESC LIMIT 1

                )";
    return ($db->pegaUm($sql)) ? true : false;
}


function verificaQstSolicitacaoDeferido($perid, $qrpid)
{
    global $db;

    // Verifica a questao de concluir análise
    $perguntaAnalise = 4100;

    $sql = "
                    SELECT
                        UPPER(i.itptitulo)
                    FROM questionario.resposta r
                    JOIN questionario.itempergunta i ON i.itpid = r.itpid
                    WHERE r.qrpid = $qrpid AND r.perid = $perguntaAnalise
        ";

    $resposta = $db->pegaUm($sql);
    if ($resposta != 'CONTINUAR CHECKLIST') {
        return false;
    }


    // Verifica a questo 1.1
    $perguntaDiligencia = 3934;

    if ($perid == $perguntaDiligencia) {
        return true;
    }

    $sql = "
                    SELECT
                        UPPER(i.itptitulo)
                    FROM questionario.resposta r
                    JOIN questionario.itempergunta i ON i.itpid = r.itpid
                    WHERE r.qrpid = $qrpid AND r.perid = $perguntaDiligencia
        ";

    $resposta = $db->pegaUm($sql);

    if ($resposta == 'NO') {
        return false;
    }
    return true;
}

function condicaoAcaoAnaliseFnde($slcid, $aedid)
{
    global $db;

    $sql = "SELECT s.*, t.tslid FROM obras2.solicitacao s JOIN obras2.tiposolicitacao_solicitacao t ON s.slcid = t.slcid WHERE s.slcid = $slcid";
    $solicitacao = $db->pegaLinha($sql);

    if ($solicitacao['tslid'] == '6') {
        return true;
    }

    return 'Trmite automtico feito aps a concluso do checklist.';
}


function condicaoAcaoDesembolsoAnaliseTecnica($sldid = null, $obrid = null, $processo = null, $sdcid = null)
{
//        if(possui_perfil( array(PFLCOD_SUPER_USUARIO) )){
//            return true;
//        }
    global $db;
    return true;
    //break;
    include_once APPRAIZ . "obras2/classes/modelo/SolicitacaoDesembolsoConvenio.class.inc";

    if ($obrid) {
        $sql = "SELECT
                    COUNT(*)
                FROM obras2.restricao r
                LEFT JOIN workflow.documento        d ON d.docid  = r.docid
                LEFT JOIN workflow.estadodocumento  e ON e.esdid  = d.esdid
                WHERE r.obrid = $obrid AND d.esdid IN (1140, 1144, 1141) AND r.rststatus = 'A' AND rstitem IN ('R')
            ";

        $result = $db->pegaUm($sql);
        if ($result > 0) {
            return 'A obra possui restrições no superadas.';
        }

        $solicitacaoDesembolso = new SolicitacaoDesembolso($sldid);
        if ($solicitacaoDesembolso->sldpercobra <= 0 || $solicitacaoDesembolso->sldpercpagamento <= 0) {
            return 'Os campos Percentual Obra e Percentual Pagamento devem ser maiores que zero.';
        }
        return true;
    } else {
        $solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio();
        $obras = $solicitacaoDesembolso->pegaObrasPorProcesso($processo);
        $obrid = array();

        foreach ($obras as $obra) {
            $obrid[] = $obra['obrid'];
        }

        $sql = "SELECT
                    COUNT(*)
                FROM obras2.restricao r
                LEFT JOIN workflow.documento        d ON d.docid  = r.docid
                LEFT JOIN workflow.estadodocumento  e ON e.esdid  = d.esdid
                WHERE r.obrid IN (" . implode(',', $obrid) . ") AND d.esdid IN (1140, 1144, 1141) AND r.rststatus = 'A' AND rstitem IN ('R')
            ";

        $result = $db->pegaUm($sql);
        if ($result > 0) {
            return 'O convênio possui obras com restrições no superadas.';
        }

        $solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio($sdcid);
        if ($solicitacaoDesembolso->sdcpercpagamento <= 0) {
            return 'O campo Percentual Pagamento deve ser maior que zero.';
        }
        return true;
    }
}

function retirar_flag_reprovado($slcid)
{

    global $db;
    $sql = "UPDATE obras2.solicitacao s SET reprovado = 'N' WHERE s.slcid = $slcid";
    $db->executar($sql);
    $db->commit();
    return true;
}

function posAcaoDeAguardandoAnaliseParaDeferido($obrid, $slcid)
{
    global $db;


    $sql = "select tslid from obras2.solicitacao s
                JOIN  obras2.tiposolicitacao_solicitacao tss ON(tss.slcid = s.slcid)
                WHERE s.slcid = $slcid";
    $tslid = $db->pegaLinha($sql);
    $tslid = $tslid['tslid'];

    $sql = "UPDATE obras2.solicitacao s SET reprovado = 'N' WHERE s.slcid = $slcid";
    $db->executar($sql);
    $db->commit();

    //  select * from obras2.tiposolicitacao_solicitacao
    //select * from obras2.solicitacao
    if ($tslid == 6) {
        $sql = "UPDATE obras2.obras SET obrtravaedicaocronograma = 'f' WHERE obrid = $obrid;";
        $db->executar($sql);
        $db->commit();
    } elseif ($tslid == 7) {
        apagarAditivoObra($obrid, $slcid);
    } elseif ($tslid == 8) {
        /*
             * @todo  implementar sqls e a soma dos dias
             * */
        // VERIFICA SE EXISTE PRAZO CADASTRADO
        $sql = "SELECT pcoid FROM obras2.prazocumprimentoobjeto where obrid = {$obrid} and pcostatus = 'A'";
        $pocid = $db->pegaUm($sql);

        if ($pocid) {
            $objDtAtual = new Datetime('now');
            $novoprazo = $objDtAtual->add(new DateInterval('P30D'));
            $novoprazo = $novoprazo->format('Y-m-d');

            $cpfAlteracao = $_SESSION['usucpf'];

            $sqlUpdate = "
                UPDATE
                    obras2.prazocumprimentoobjeto
                SET
                    pcodatavigencia = '{$novoprazo}',
                    usucpf = '{$cpfAlteracao}',
                    pcodataultimaalteracao = 'now()',
                    pcodataprorrogada = TRUE
                WHERE
                    pcoid = {$pocid}; ";

            $db->executar($sqlUpdate);
            $db->commit();
        }
    }
    return true;
}

/**
 * Captura todas as solicitaes criadas para uma determinada obra.
 * @global type $db
 * @param int $obrid
 * @return array(slcid,slcobservacao,slcjustificativa,slcdatainclusao,docid,qrpid,aprovado,tslid,tsldescricao,esdid,criador,queid)
 */
function capturaSolicitacoesObra($obrid)
{
    global $db;

    $sql = "
            SELECT
                sol.slcid,
                sol.slcobservacao,
                sol.slcjustificativa,
                TO_CHAR(sol.slcdatainclusao,'DD/MM/YYYY') as slcdatainclusao,
                sol.docid,
                sol.qrpid,
                sol.aprovado,
                sol.slccpfaprovado,
                ts.tslid,
                ts.tsldescricao,
                esd.esdid,
                esd.esddsc,
                usu.usunome AS criador,
                qr.queid,
                (SELECT usunome FROM seguranca.usuario WHERE usucpf = sol.slccpfaprovado) as usuaprovado
            FROM obras2.solicitacao sol
            INNER JOIN obras2.tiposolicitacao_solicitacao tss ON(sol.slcid = tss.slcid)
            INNER JOIN obras2.tiposolicitacao ts ON(tss.tslid = ts.tslid)
            INNER JOIN workflow.documento doc ON(sol.docid = doc.docid)
            INNER JOIN workflow.estadodocumento esd ON(doc.esdid = esd.esdid)
            INNER JOIN seguranca.usuario usu ON(sol.usucpf = usu.usucpf)
            LEFT JOIN  seguranca.usuario usu2 ON(sol.slccpfaprovado = usu.usucpf)
            LEFT  JOIN questionario.questionarioresposta qr ON(sol.qrpid = qr.qrpid)
            WHERE obrid = $obrid AND tss.tpsstatus = 'A' AND sol.slcstatus = 'A'
            ORDER BY 1 DESC";
    $retorno = $db->carregar($sql);
    return $retorno ? $retorno : array();
}

/**
 * Retorna ultima análise realizada do documento de solicitação para os destinos: DEFERIDO, INDEFERIDO ou DILIGNCIA.
 * @global type $db
 * @param int $docid
 * @return array(htddata,analista,observacao)
 */
function capturaAnaliseSolicitacao($docid)
{
    global $db;
    $query = <<<DML
            SELECT
                TO_CHAR(htd.htddata,'DD/MM/YYYY') AS htddata,
                usu.usunome AS analista,
                cd.cmddsc AS observacao
            FROM workflow.historicodocumento htd
            INNER JOIN workflow.acaoestadodoc ad ON (htd.aedid = ad.aedid)
            INNER JOIN seguranca.usuario usu on (htd.usucpf = usu.usucpf)
            LEFT  JOIN workflow.comentariodocumento cd on(htd.hstid = cd.hstid)
            WHERE htd.docid = $docid
                AND ad.esdiddestino in (1572,1573,1574)
            ORDER BY htd.hstid DESC
            LIMIT 1;
DML;
    return $db->pegaLinha($query);
}

/**
 * Retorna todos os comentrios relacionados aos tramites do documento de solicitação.
 * @global type $db
 * @param int $docid
 * @return array(htddata,analista,observacao)
 */
function capturaAnaliseSolicitacaoObservacoes($docid)
{
    global $db;
    $cadastramento = ESDID_SOLICITACOES_CADASTRAMENTO;
    $query = <<<DML
            SELECT
                TO_CHAR(htd.htddata,'DD/MM/YYYY') AS htddata,
                usu.usunome,
                cd.cmddsc,
                ad.esdidorigem,
                ad.esdiddestino,
                (select esddsc from workflow.estadodocumento where esdid = ad.esdidorigem) as origem,
                (select esddsc from workflow.estadodocumento where esdid = ad.esdiddestino) as destino,
                ad.aeddscrealizada
            FROM workflow.historicodocumento htd
            INNER JOIN workflow.acaoestadodoc ad ON (htd.aedid = ad.aedid)
            INNER JOIN seguranca.usuario usu on (htd.usucpf = usu.usucpf)
            LEFT  JOIN workflow.comentariodocumento cd on(htd.hstid = cd.hstid)
            WHERE htd.docid = $docid AND ad.esdidorigem != {$cadastramento}
            ORDER BY htd.hstid DESC;
DML;
    $dados = $db->carregar($query);
    if (!$dados) {
        $dados = array();
    }
    $retorno = '';
    $pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
    $mostraTudo = false;
    if (possui_perfil($pflcods)) {
        $mostraTudo = true;
    }
    foreach ($dados as $dado) {
        if (!$mostraTudo && ($dado['esdiddestino'] != ESDID_SOLICITACOES_DILIGENCIA && $dado['esdiddestino'] != ESDID_SOLICITACOES_DEFERIDO && $dado['esdiddestino'] != ESDID_SOLICITACOES_INDEFERIDO)) {
            continue;
        }

        $resposta = 'Resposta Entidade';
        $box = '';
        if (verificaTramiteFNDE($dado['esdidorigem'])) {
            $box = 'box-comentario-fnde';
            $resposta = 'Resposta FNDE';
        }
        $retorno .= <<<HTML
                <div style="" title="{$resposta}" class="box-comentario {$box}">
                    <p style="font-size:10px"><b style="font-size:12px">{$dado['aeddscrealizada']}</b> por {$dado['usunome']} ({$dado['htddata']}):</p>
                    <p><b style="font-size:12px">"{$dado['cmddsc']}"</b></p>
                </div>
HTML;
    }
    return $retorno;
}

function verificaTramiteFNDE($esdidOrigem)
{
    if ($esdidOrigem == ESDID_SOLICITACOES_AGUARDANDO_ANALISE
        || $esdidOrigem == ESDID_SOLICITACOES_RETORNADO
        || $esdidOrigem == ESDID_SOLICITACOES_DEFERIDO
        || $esdidOrigem == ESDID_SOLICITACOES_INDEFERIDO) {
        return true;
    }
}

/**
 * Verifica estado final da solicitação e retorna o texto referente.
 * @param int $esdid
 * @return string
 */
function retornaTextoEstadoSolicitacao($esdid)
{
    switch ($esdid) {
        case ESDID_SOLICITACOES_DEFERIDO:
            return 'Deferido';
        case ESDID_SOLICITACOES_INDEFERIDO:
            return 'Indeferido';
        case ESDID_SOLICITACOES_DILIGENCIA:
            return 'Diligência';
    }
}


function posAcaoObraSolicitacao($obrid, $slcid)
{

    if ($_SERVER['HTTP_HOST'] == 'simec-local') {
        return true;
    }
    require_once APPRAIZ . 'obras2/classes/modelo/Email.class.inc';
    $email = new Email();

    return $email->enviaEmailSolicitacoes($slcid);
}

function posAcaoObraSolicitacaoDesembolso($sldid, $obrid, $processo = null, $sdcid = null)
{
    #ver($obrid,$slcid,d);
    require_once APPRAIZ . 'obras2/classes/modelo/Email.class.inc';
    $email = new Email();
    if ($obrid) {
        return $email->enviaEmailSolicitacoesDesembolso($sldid);
    } else {
        return $email->enviaEmailSolicitacoesDesembolsoConvenio($sdcid);
    }
}

/**
 * Verifica se o tipo de solicitação  Solicitacao::TSLID_CORRECAO_INCONSISTENCIA_NO_SISTEMA.
 *
 * @param int $tslid ID de identificao do tipo de solicitação.
 * @return boolean
 */
function condicaoExibirEnviarParaAnaliseTi($tslid)
{
    return (Solicitacao::TSLID_CORRECAO_INCONSISTENCIA_NO_SISTEMA == $tslid);
}

function condicaoAcaoObrasSolicitacaoDesembolsoAnaliseDocParaTecnica(
    $sldid = null,
    $obrid = null,
    $processo = null,
    $sdcid = null
) {
//        if(possui_perfil( array(PFLCOD_SUPER_USUARIO) )){
//            return true;
//        }
    include_once APPRAIZ . "obras2/classes/modelo/SolicitacaoDesembolsoConvenio.class.inc";
    //nos casos que houver restrições executivas inabilitar o tramite para análise tcnica.
    global $db;
    if ($obrid) {
        $sql = "SELECT
                r.obrid
            FROM obras2.restricao r
            JOIN workflow.documento d ON d.docid = r.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid NOT IN (1142, 1143, 1497)
            WHERE rstitem IN ('R') AND rststatus = 'A' AND tprid = 13 AND r.obrid = $obrid
            ";
        $result = $db->pegaUm($sql);
        return ($result) ? 'Esta obra possui restrições executivas.' : true;
    } else {
        $solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio();
        $obras = $solicitacaoDesembolso->pegaObrasPorProcesso($processo);
        $obrid = array();

        foreach ($obras as $obra) {
            $obrid[] = $obra['obrid'];
        }

        $sql = "SELECT
                r.obrid
            FROM obras2.restricao r
            JOIN workflow.documento d ON d.docid = r.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid NOT IN (1142, 1143, 1497)
            WHERE rstitem IN ('R') AND rststatus = 'A' AND tprid = 13 AND r.obrid IN (" . implode(',', $obrid) . ")
            ";
        $result = $db->pegaUm($sql);
        return ($result) ? 'Existe uma ou mais oubras com restrições executivas.' : true;
    }
}

function condicaoAcaoObrasSolicitacaoDesembolsoAnaliseDocParaReI($sldid, $obrid, $processo = null, $sdcid = null)
{
    include_once APPRAIZ . "obras2/classes/modelo/SolicitacaoDesembolsoConvenio.class.inc";
    global $db;
    if ($obrid) {
        //quando no houver restrições executivas inabilitar o tramite para análise de restrições e inconformidades

        $sql = "SELECT
                r.obrid
            FROM obras2.restricao r
            JOIN workflow.documento d ON d.docid = r.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid NOT IN (1142, 1143, 1497)
            WHERE rstitem IN ('R') AND rststatus = 'A' AND tprid = 13 AND r.obrid = $obrid
            ";

        $result = $db->pegaUm($sql);
        return (!$result) ? 'Esta convênio no possui restrições executivas.' : true;
    } else {
        $solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio();
        $obras = $solicitacaoDesembolso->pegaObrasPorProcesso($processo);
        $obrid = array();

        foreach ($obras as $obra) {
            $obrid[] = $obra['obrid'];
        }

        $sql = "SELECT
                r.obrid
            FROM obras2.restricao r
            JOIN workflow.documento d ON d.docid = r.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid NOT IN (1142, 1143, 1497)
            WHERE rstitem IN ('R') AND rststatus = 'A' AND tprid = 13 AND r.obrid  IN (" . implode(',', $obrid) . ")
            ";

        $result = $db->pegaUm($sql);
        return (!$result) ? 'Esta obra no possui restrições executivas.' : true;
    }
}

function posAcaoDemandaObras2()
{
    $docid = $_REQUEST['docid'];
    require_once APPRAIZ . 'obras2/classes/modelo/Email.class.inc';
    $email = new Email();
    return $email->enviaEmailDemanda($docid, null);
}


function condicaoDocumentacaoObjeto($docid)
{
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return true;
    }

    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoDocumentacao.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoQuestionario.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/FotosQuestionarioCumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/QuestionarioCumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/QuestaoCumprimentoObjeto.class.inc";
    $cumprimentoObjeto = new CumprimentoObjeto();
    $cumprimentoObjeto->pegaPorDocid($docid);

    $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
    if (!$cumprimentoObjetoDocumentacao->validaRespostasDocumentacao($cumprimentoObjeto->obrid)) {
        return 'Ao disponvel somente aps preenchimento do formulrio de documentao.';
    }
    return true;
}

function condicaoQuestionarioCumprimentoObjeto($docid)
{
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return true;
    }

    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoDocumentacao.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoQuestionario.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/FotosQuestionarioCumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/QuestionarioCumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/QuestaoCumprimentoObjeto.class.inc";
    $cumprimentoObjeto = new CumprimentoObjeto();
    $cumprimentoObjeto->pegaPorDocid($docid);

    #Exceo para Obras Inacabadas e Canceladas.
    $esdid = $cumprimentoObjeto->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $cumprimentoObjeto->obrid)");
    #No se aplica para obra cancelada.
    if ($esdid == ESDID_OBJ_CANCELADO) {
        return true;
    }
    #No se aplica para obra inacabada e ultima vistoria incompleta.
    if ($esdid == ESDID_OBJ_INACABADA) {
        $sql = <<<DML
                SELECT
                    s.staid
                FROM obras2.supervisao s
                WHERE s.obrid = '{$cumprimentoObjeto->obrid}'
                    AND s.emsid IS NULL
                    AND s.smiid IS NULL
                    AND s.supstatus = 'A'
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY s.supdata DESC, s.supdtinclusao DESC
                LIMIT 1
DML;
        $staid = $cumprimentoObjeto->pegaUm($sql);
        if ($staid != 3) {
            return true;
        }
    }
    #Fim Exceo.
    $cumprimentoObjetoQuestionario = new CumprimentoObjetoQuestionario();
    $cumprimentoObjetoQuestionario->verificaExistencia($cumprimentoObjeto->coid);
    $fotosQuestionarioCumprimentoObjeto = new FotosQuestionarioCumprimentoObjeto();
    $fotosQuestionarioCumprimentoObjeto->coqid = $cumprimentoObjetoQuestionario->coqid;

    $pendencias = $cumprimentoObjetoQuestionario->verificaPendencias();
    if (!$pendencias) {
        return 'Preencha o Questionrio antes de prosseguir.';
    }
    if ($pendencias['codigo_inep'] === false) {
        return 'O cdigo do INEP inserido no questionrio no  vlido.';
    }
    if ($pendencias['respostas_zeradas'] == 'S' && $pendencias['resposta_fotos'] == 'N') {
        return true;
    } elseif ($pendencias['respostas_zeradas'] == 'N') {
        return 'Existem uma ou mais perguntas no respondidas no questionrio.';
    } elseif ($pendencias['resposta_fotos'] == 'S') {
        if ($fotosQuestionarioCumprimentoObjeto->verificaFotos()) {
            return true;
        } else {
            return 'Inserir fotos externas e internas da edificao e de todos seus ambientes no formulrio Fotos da Concluso da obra.';
        }
    }
    return true;
}

function condicaoCumprimentoObjeto($docid)
{
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        return true;
    }

    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoDocumentacao.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoQuestionario.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/FotosQuestionarioCumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/QuestionarioCumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/QuestaoCumprimentoObjeto.class.inc";
    $cumprimentoObjeto = new CumprimentoObjeto();
    $cumprimentoObjeto->pegaPorDocid($docid);

    $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
    if (!$cumprimentoObjetoDocumentacao->validaRespostasDocumentacao($cumprimentoObjeto->obrid)) {
        return 'Ao disponvel somente aps preenchimento do formulrio de documentao.';
    }
    #Exceo para Obras Inacabadas e Canceladas.
    $esdid = $cumprimentoObjeto->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $cumprimentoObjeto->obrid)");
    #No se aplica para obra cancelada.
    if ($esdid == ESDID_OBJ_CANCELADO) {
        return true;
    }
    #No se aplica para obra inacabada e ultima vistoria incompleta.
    if ($esdid == ESDID_OBJ_INACABADA) {
        $sql = <<<DML
                SELECT
                    s.staid
                FROM obras2.supervisao s
                WHERE s.obrid = '{$cumprimentoObjeto->obrid}'
                    AND s.emsid IS NULL
                    AND s.smiid IS NULL
                    AND s.supstatus = 'A'
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY s.supdata DESC, s.supdtinclusao DESC
                LIMIT 1
DML;
        $staid = $cumprimentoObjeto->pegaUm($sql);
        if ($staid != 3) {
            return true;
        }
    }
    #Fim Exceo.
    $cumprimentoObjetoQuestionario = new CumprimentoObjetoQuestionario();
    $cumprimentoObjetoQuestionario->verificaExistencia($cumprimentoObjeto->coid);
    $fotosQuestionarioCumprimentoObjeto = new FotosQuestionarioCumprimentoObjeto();
    $fotosQuestionarioCumprimentoObjeto->coqid = $cumprimentoObjetoQuestionario->coqid;

    $pendencias = $cumprimentoObjetoQuestionario->verificaPendencias();
    if (!$pendencias) {
        return 'Preencha o Questionrio antes de prosseguir.';
    }

    if ($pendencias['codigo_inep'] === false) {
        return 'O cdigo do INEP inserido no questionrio no  vlido.';
    }

    if ($pendencias['respostas_zeradas'] == 'S' && $pendencias['resposta_fotos'] == 'N') {
        return true;
    } elseif ($pendencias['respostas_zeradas'] == 'N') {
        return 'Existem uma ou mais perguntas no respondidas no questionrio.';
    } elseif ($pendencias['resposta_fotos'] == 'S') {
        if ($fotosQuestionarioCumprimentoObjeto->verificaFotos()) {
            return true;
        } else {
            return 'Inserir fotos externas e internas da edificao e de todos seus ambientes no formulrio Fotos da Concluso da obra.';
        }
    }
    return true;
}

function posAcaoCOOValidacao($coid)
{
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjeto.class.inc";

    $co = new CumprimentoObjeto();
    $co->verificaRestricoesDiligencia($coid);

    disparaEmailCumprimentoObjeto($coid);

    return true;
}

function disparaEmailObraConcluida($obrid)
{
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/alerta/AlertaObra.class.inc";
    $cumprimentoObjeto = new CumprimentoObjeto();
    if (!$cumprimentoObjeto->obrasPermitidas($obrid)) {
        return true;
    }
    $cumprimentoObjeto->verificaExistencia($obrid);
    $alertaObra = new AlertaObra();
    $alertaObra->novo($obrid, 1);
    $email = new Email();
    $email->enviaEmailObraConcluida($obrid);

    $timestamp = strtotime("+45 days");
    $data = date('d/m/Y', $timestamp);
    $dados = array(
        'rstitem' => 'R',
        'fsrid' => 7, #Situação da Obra na Ocorrncia -> Concluida
        'tprid' => 16, #Tipo -> Cumprimento do Objeto
        'rstdsc' => 'Aguardando preenchimento do Cumprimento do Objeto.',
        'rstdtprevisaoregularizacao' => $data,
        'rstdscprovidencia' => 'Preencher as abas do Cumprimento do Objeto (Documentao e Questionrio)e tramitar o workflow para "Aguardando Validao FNDE".'
    );
    $restricao = new Restricao();
    $restricao->salvaRestricaoCO($dados, $obrid);
    return true;
}


function aprovaSolitacao($obrid){
    global $db;

    $arraySlcid =  capturaSolicitacoesObra($obrid);

    foreach($arraySlcid as $dadosSlcid){

        if($dadosSlcid['tslid'] == 14 && $dadosSlcid['esdid'] == ESDID_SOLICITACOES_DEFERIDO ){
            $slcid = $dadosSlcid['slcid'];
        }
    }

    if($slcid){
        try {
            $retorno = posAcaoObraSolicitacao(null,$slcid);
            $mensagem = 'Solicitação atualizada com sucesso!';
            $query = <<<DML
            UPDATE obras2.solicitacao SET aprovado = 'S' , slccpfaprovado = '{$usucpf}'  WHERE slcid = $slcid

DML;
            $db->executar($query);
            $db->commit();
        } catch(Exception $e) {
            $log = $e->getMessage();
            $retorno = false;
            $mensagem = 'Falha no disparo de email.';
        }

    }

}

function reprovaSolitacao($obrid){
    global $db;

    $arraySlcid =  capturaSolicitacoesObra($obrid);

    foreach($arraySlcid as $dadosSlcid){
        if($dadosSlcid['tslid'] == 14 && $dadosSlcid['esdid'] == ESDID_SOLICITACOES_DEFERIDO ){
            $slcid = $dadosSlcid['slcid'];
        }
    }

    if($slcid){
        try {

            $sql = "select * from obras2.solicitacao where slcid = $slcid";
            $dados = $db->pegaLinha($sql);


            $sql = "select * from workflow.documento where docid = $dados[docid]";
            $dados1 = $db->pegaLinha($sql);

            if($dados1['esdid'] == ESDID_SOLICITACOES_DEFERIDO){

                $acao = 3845;
            }
            if($dados1['esdid'] == ESDID_SOLICITACOES_INDEFERIDO){

                $acao = 3846;
            }
            wf_alterarEstado($dados['docid'],$acao,"Pedido reprovado",array());
            $mensagem = 'Solicitação atualizada com sucesso!';
            $query ="UPDATE obras2.solicitacao SET reprovado = 'S' WHERE slcid = $slcid";
            $db->executar($query);
            $db->commit();
        } catch(Exception $e) {
            $log = $e->getMessage();
            $retorno = false;
            $mensagem = 'Falha no disparo de email.';
        }



    }

}
function disparaEmailCumprimentoObjeto($coid)
{

    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoDocumentacao.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoProcesso.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/alerta/AlertaObra.class.inc";
    global $db;

    $cumprimentoObjeto = new CumprimentoObjeto($coid);

     $sql = <<<DML
            SELECT
                ed.esdid,
                ac.aedid,
                ac.esdidorigem,
                ac.esdiddestino
            FROM workflow.documento d
            INNER JOIN workflow.historicodocumento hd ON (d.hstid = hd.hstid)
            INNER JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
            INNER JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdiddestino
            WHERE hd.docid = {$cumprimentoObjeto->docid}
DML;
   
    $estado = $db->pegaLinha($sql);
    $email = new Email();
    if ($estado['esdid'] != ESDID_CUMPRIMENTO_DILIGENCIADO) {
        $email->enviaEmailCumprimentoObjeto($coid);
    }

//    Reviso de Parecer -- 1268 -- ESDID_CUMPRIMENTO_VALIDACAO_FNDE
//    Enviar para Validao Processo -- 1623 -- ESDID_CUMPRIMENTO_AGUARDANDO_VALIDACAO_PROCESSO
//    Enviar para deferimento -- 1624 -- ESDID_CUMPRIMENTO_AGUARDANDO_DEFERIMENTO
//    Enviar para Aprovao -- 1625 --ESDID_CUMPRIMENTO_AGUARDANDO_APROVACAO
//    Aprovar -- 1626 -- ESDID_CUMPRIMENTO_APROVADO
//    Reprovar -- 1268 -- ESDID_CUMPRIMENTO_VALIDACAO_FNDE
    if($estado['esdidorigem'] == ESDID_CUMPRIMENTO_AGUARDANDO_APROVACAO && $estado['esdid'] == ESDID_CUMPRIMENTO_APROVADO){
        //    anterior -- 1625 prximo -- 1626 (aprovado)
        aprovaSolitacao($cumprimentoObjeto->obrid);

    }else if($estado['esdidorigem'] == ESDID_CUMPRIMENTO_AGUARDANDO_APROVACAO && $estado['esdid'] == ESDID_CUMPRIMENTO_VALIDACAO_FNDE){
        //    anterior -- 1625 prximo -- 1268 (reprovado)
        reprovaSolitacao($cumprimentoObjeto->obrid);
    }

    if ($estado['esdid'] == ESDID_CUMPRIMENTO_VALIDACAO_FNDE) {
      $sql = <<<DML
                SELECT rstid
                FROM obras2.restricao r
                WHERE r.fsrid = 7
                    AND r.tprid = 16
                    AND r.rstdsc = 'Aguardando preenchimento do Cumprimento do Objeto.'
                    AND r.usucpf = '00000000191'
                    AND r.obrid = {$cumprimentoObjeto->obrid}
DML;

        $rstid = $db->pegaUm($sql);
        $restricao = new Restricao($rstid);
        if ($restricao->atualizaDocidRetricaoParaSuperado()) {
            $restricao->commit();
        }
    } elseif (in_array($estado['esdidorigem'], array(
        ESDID_CUMPRIMENTO_AGUARDANDO_VALIDACAO_PROCESSO,
        ESDID_CUMPRIMENTO_AGUARDANDO_DEFERIMENTO,
        ESDID_CUMPRIMENTO_AGUARDANDO_APROVACAO,
        ESDID_CUMPRIMENTO_APROVADO
    ))) {
        $processo = $cumprimentoObjeto->retornaNumeroProcesso($coid);
        $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
        $obras = $cumprimentoObjetoProcesso->retornaObrasPorProcesso($processo);
        $update = '';
        foreach ($obras as $obra) {
            if ($obra['coid'] == $coid) {
                continue;
            }
            $sqlHistorico = "insert into workflow.historicodocumento (aedid, docid, usucpf, htddata)
                    values ({$estado['aedid']}, {$obra['docid']}, '{$_SESSION['usucpf']}', now()) returning hstid";
            $hstid = (integer)$db->pegaUm($sqlHistorico);
            if (!$hstid) {
                $db->rollback();
                return false;
            }
            if ($db->commit()) {
                $update .= "update workflow.documento set esdid = {$estado['esdid']}, hstid = $hstid where docid = {$obra['docid']};";
            }
        }
        if ($update != '') {
            $db->executar($update);
            $db->commit();
        }
    } elseif ($estado['esdid'] == ESDID_CUMPRIMENTO_DILIGENCIADO) {
        $timestamp = strtotime("+45 days");
        $data = date('d/m/Y', $timestamp);
        $dados = array(
            'rstitem' => 'R',
            'fsrid' => 7, #Situação da Obra na Ocorrncia -> Concluida
            'tprid' => 16, #Tipo -> Cumprimento do Objeto
            'rstdsc' => 'Cumprimento do Objeto Dilignciado.',
            'rstdtprevisaoregularizacao' => $data,
            'rstdscprovidencia' => 'Requisitamos que, num prazo mximo de 30 dias, contados do recebimento desta notificao, sejam atendidas as pendências descritas na aba "Cumprimento do Objeto", para darmos andamento  Prestao de Contas do instrumento pactuado.

Os documentos solicitados devem ser inseridos na prpria aba "Cumprimento do Objeto", conforme orientao que consta em vermelho do seu cabealho. Aps a insero da documentao, clicar no comando: "Enviar para Análise FNDE".

Salientamos que a omisso por parte do (a) Senhor (a), no prazo estabelecido, poder acarretar na cobrana integral dos valores repassados pelo FNDE.

Colocamo-nos  disposio para quaisquer outros esclarecimentos que se faam necessrios acerca do preenchimento das informações no SIMEC por meio do endereo eletrnico: cumprimentodoobjeto@fnde.gov.br.'
        );
        $restricao = new Restricao();
        $restricao->salvaRestricaoCO($dados, $cumprimentoObjeto->obrid);
        $alertaObra = new AlertaObra();
        $alertaObra->novo($cumprimentoObjeto->obrid, 2);
        $email->enviaEmailCumprimentoObjetoDiligencia($coid);
    }
    //Adicionado para criao de registro na documentao do cumprimento do objeto.
    $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
    $cumprimentoObjetoDocumentacao->verificaExistencia($cumprimentoObjeto->obrid, true);
    return true;
}

function condicaoCumprimentoObjetoConclusao($coid)
{
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoConclusao.class.inc";
    $cumprimentoObjetoProcesso = new CumprimentoObjetoConclusao();
    if (!$cumprimentoObjetoProcesso->verificaExistencia($coid)) {
        return ' necessrio o preenchimento da aba de concluso da obra.';
    }
    return true;
}

function condicaoCumprimentoObjetoProcesso($coid)
{
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjeto.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/CumprimentoObjetoProcesso.class.inc";
    $cumprimentoObjeto = new CumprimentoObjeto($coid);
    $processo = $cumprimentoObjeto->retornaNumeroProcesso($coid);
    $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
    $return = $cumprimentoObjetoProcesso->verificaObrasPorProcesso($processo, false);
    if ($return['result'] == '0') {
        return ' necessrio o preenchimento da aba de concluso da obra para todas as obras do processo.';
    }
    if (!$cumprimentoObjetoProcesso->verificaObrasPorProcessoCondicao($processo, $coid)) {
        return ' necessrio que todas as obras do processo estejam no mesmo estado de tramitao.';
    }
    return true;
}


function posCadastramentoEmpresaV2($sosid)
{
    include_once APPRAIZ . "obras2/classes/modelo/SupervisaoOrdemServico.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/SupervisaoGrupoEmpresa.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/Supervisao2Empresa.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/SupervisaoEmpenhoEmpresa.class.inc";
    include_once APPRAIZ . "obras2/classes/modelo/SupervisaoOrdemServicoObra.class.inc";

    $email = new Email();
    $email->enviaEmailCadastramentoOrdemServico($sosid);
    return true;
}

function cadastrarSupervisaoPorOSV2($sosid)//stop_here
{

    //Capturar obras vinculadas a ordem de serviço e criar supervisao.
    //Enviar email OS aceita ao final. enviaEmalOsAceita
    return true;
    global $db;
    require_once APPRAIZ . "obras2/classes/modelo/Supervisao_Os.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/Supervisao_Os_Obra.class.inc";
    require_once APPRAIZ . "obras2/classes/modelo/SupervisaoEmpresa.class.inc";
    $osObra = new Supervisao_Os_Obra();
    $arrEmpid = $osObra->listaEmpidPorOs($sosid);
    if ($arrEmpid) {
        foreach ($arrEmpid as $empid) {
            $supervisaoEmpresa = new SupervisaoEmpresa();
            // descrição do documento
            $docdsc = "Fluxo de Supervisão Empresa do mdulo Obras II - sueid {$sosid}";
            // cria documento do WORKFLOW
            $docid = wf_cadastrarDocumento(WF_TPDID_LAUDO_SUPERVISAO_EMPRESA, $docdsc);
            $arDado['usucpf'] = $_SESSION['usucpf'];
            $arDado['empid'] = $empid;
            $arDado['docid'] = $docid;
            $arDado['sosid'] = $sosid;
            $arDado['suedtatualizacao'] = date('Y-m-d');
            $supervisaoEmpresa->popularDadosObjeto($arDado);
            $supervisaoEmpresa->salvar();
            $supervisaoEmpresa->commit();
        }

        $email = new Email();
        $email->enviaEmalOsAceita($sosid);

        return true;
    } else {
        return false;
    }
}

function verificaSupervisaoOSConcluidaV2($sosid)
{
    #DESENVOLVER
    return true;
    global $db;
    //Pegar todas as supervises da OS e verificar se foram pagas
    $sql = "select count(sueid) from obras2.supervisaoempresa where sosid = $sosid and suestatus = 'A'";
    $total_supervisao = $db->pegaUm($sql);
    if (!$total_supervisao) {
        return "No existem supervises nesta OS.";
    }
    $sql = "select
                        count(sueid)
                from
                        obras2.supervisaoempresa sue
                inner join
                        workflow.documento doc ON doc.docid = sue.docid
                where
                        sue.sosid = $sosid and suestatus = 'A'
                and
                        esdid = " . WF_ESDID_LAUDO_SUPERVISAO_PAGO;
    $total_pagos = $db->pegaUm($sql);
    if ($total_supervisao != $total_pagos) {
        $total = $total_supervisao - $total_pagos;
        if ($total > 1) {
            return "Existem $total supervises vinculadas  OS no pagas.";
        } else {
            return "Existe $total supervisão vinculada  OS no paga.";
        }
    }
    return true;

    //return "verificar se as supervises foram pagas...";
}


function obraVinculada($obrid)
{
    global $db;
    $sql = "SELECT obridvinculado, obrperccontratoanterior FROM obras2.obras WHERE obrid = $obrid";
    $obra = $db->pegaLinha($sql);

    if ($obra['obridvinculado'] && $obra['obrperccontratoanterior'] > 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * Verifica se existe uma solicitação de desbloqueio de obra, que tenha sido
 * deferida E aprovada. Se sim retorna true. Caso contrrio retorna false.
 *
 * @param int $obrid ID da obra
 * @return boolean True para um solicitação aprovada e false caso contrrio
 */
function verificaSolicitacaoDesbloqueio($obrid)
{

    global $db;

    //verificando se um ID de obra foi recebido
    if (empty($obrid)) {
        return false;
    } else {
        $obrid = (int)$obrid;
    }

    //id do tipo da solicitação de cancelamento
    $tipoSolicitacao = 13;
    $estadoDeferido = 1572;

    $sql = "SELECT sol.slcid
                FROM  obras2.solicitacao AS sol
                JOIN  obras2.tiposolicitacao_solicitacao AS tiposol ON (sol.slcid = tiposol.slcid)
                JOIN  obras2.tiposolicitacao AS tipo ON (tiposol.tslid = tipo.tslid)
                JOIN  workflow.documento AS doc ON (sol.docid = doc.docid)
                WHERE sol.obrid = " . $obrid . "
                    AND doc.esdid IN(" . $estadoDeferido . ")
                    AND tipo.tslid = " . $tipoSolicitacao . "
                    AND sol.aprovado = 'S' ";

    $retorno = $db->carregar($sql);

    if (!empty($retorno)) {
        return true;
    } else {
        return false;
    }
}

/**
 * Verifica se a obra tem ao menos um estado de "Execução" no histórico do seu workflow.
 *
 * @param int $obrid ID da obra
 * @return boolean True se a obra tiver um estado de execução no histrio e false se no
 */
function obraJaEsteveEmExecucao($obrid)
{

    global $db;

    if (empty($obrid)) {
        return false;
    }

    //query para buscar se a obra j passou pelo estado de execução em algum momento.
    $sql = "SELECT obra.obrid
                FROM obras2.obras AS obra
                JOIN workflow.historicodocumento AS historico ON (obra.docid = historico.docid)
                JOIN workflow.acaoestadodoc AS acao ON (acao.aedid = historico.aedid)
                WHERE (acao.esdidorigem = " . ESD_EXECUCAO . " OR acao.esdiddestino = " . ESD_EXECUCAO . ")
                AND obra.obrid = " . (int)$obrid;

    $retorno = $db->carregar($sql);

    //verifica se algum registro foi encontrado
    if (!empty($retorno)) {
        //se algum registro for encontrado a obra j passou por execução
        return true;
    } else {
        //se nenhum registro foi encontrado considera que a obra nunca passou por execução
        return false;
    }
}

/**
 * Realiza a ps ao do deferimento da solicitação.
 *
 * Se for uma demanda de TI, cria automaticamente uma demanda no Solicitação -> Demandas da TI
 *
 * @param int $slcid ID da solicitação
 * @return boolean True ocorreu a importao e false se no
 */
function wf_pos_deferir($slcid)
{

    global $db;

    $sql = "SELECT DISTINCT TRUE FROM obras2.tiposolicitacao_solicitacao WHERe slcid = $slcid AND tslid = 12";

    $testaTI = $db->pegaUm($sql);
    if ($testaTI == 't') {
        require_once APPRAIZ . 'includes/workflow.php';

        // descrição do documento
        $docdsc = "Fluxo de demandas do Obras 2 - dmdid " . $dmdid;

        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento(WF_TPDID_DEMANDA, $docdsc);

        $sql = "INSERT INTO obras2.demandas(
                                dmtid, dmdnome, dmddescricao, docid, dmdcpfcadastro,
                                dmddatacadastro, dmdstatus, dmoid, dmdultimaalteracao,
                                dmdsolicitante,
                                dmdcpfsolicitante,
                                obrid)
                    SELECT
                        1, 'Solicitação n '||slc.slcid, slc.slcjustificativa||' Obs: '||coalesce(slc.slcobservacao, 'N/A'), $docid as docid, slc.usucpf,
                        now(), 'A', 1, now(),
                        usu.usunome,
                        slc.usucpf,
                        obrid
                    FROM obras2.solicitacao slc
                    INNER JOIN seguranca.usuario usu ON usu.usucpf = slc.usucpf
                    WHERE slcid = $slcid
                    RETURNING dmdid";

        $dmdid = $db->pegaUm($sql);
        $db->commit();

        $sql = "INSERT obras2.demandasanexo(dmdid, arqid)
                    SELECT $dmdid as dmdid, arqid
                    FROM obras2.anexosolicitacao
                    WHERE slcid = $slcid;";

        return true;
    } else {
        return true;
    }
}

/**
 * Funo responsvel por verificar se houve o bloqueio/desbloqueio forado da abra 'Cumprimento do objeto'.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool
 */
function verificaBloqueioManual($obrid)
{

    global $db;

    $sql = "SELECT
              rgaid
            FROM
              obras2.registroatividade rga
            WHERE
              rga.obrid = {$obrid}
            AND
              ( rga.rgadscsimplificada ILIKE '%Cumprimento do Objeto bloqueado manualmente%'
                OR
                rga.rgadscsimplificada ILIKE '%Cumprimento do Objeto liberado manualmente%'
              )
            AND
                rgastatus = 'A'

            ORDER BY
              rga.rgaid DESC;
            ";

    $rgaid = $db->pegaUm($sql);
    if ($rgaid) {
        return true;
    }
    return false;
}

/**
 * Funo responsvel por verificar a Situação da obra. (Planejamento pelo preponente, Licitao, Contratao, Execução, etc.)
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid
 * @return bool|mixed|NULL|string
 */
function verificaSituacaoObra($obrid)
{

    global $db;

    $sql = "SELECT
              str.*
            FROM
              obras2.obras o
            INNER JOIN
              obras2.situacao_registro str ON o.strid = str.strid
            WHERE
              o.obrid = {$obrid};
            ";

    $strid = $db->pegaUm($sql);
    return $strid;
}

/**
 * Funo que verifica se o responsvel pela obra leu a notificao de cumprimento do objeto.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool|mixed|NULL|string
 */
function verificaLeituraNotificacaoCumprimentoObjeto($obrid)
{

    global $db;

    $sql = "SELECT
              ra.rgaid
            FROM
              obras2.registroatividade ra
            WHERE
              ra.obrid = {$obrid}
            AND
              ra.rgadscsimplificada ILIKE '%leitura da notificao cumprimento%';
            ";

    $rgaid = $db->pegaUm($sql);

    return $rgaid;
}

/**
 * Funo que verifica se o responsvel pela obra leu o alerta de concluso da obra.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool|mixed|NULL|string
 */
function verificaLeituraNotificacaoObraConcluida($obrid)
{

    global $db;

    $sql = "SELECT
              ra.rgadtinclusao
            FROM
              obras2.registroatividade ra
            WHERE
              ra.obrid = {$obrid}
            AND
              ra.rgadscsimplificada ILIKE '%alerta de concluso de obra%';
            ";

    $resultado = $db->pegaUm($sql);
    return $resultado;
}

/**
 * Funo responsvel por verificar a origem da obra. (PAC2, Convênio, Descentralizao, TD, PRONATEC, etc.)
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool|mixed|NULL|string
 */
function verificaOrigemObra($obrid)
{

    global $db;
    $sql = "SELECT o.tooid FROM obras2.obras o WHERE o.obrid = {$obrid}";
    return $db->pegaUm($sql);
}

/**
 * Funo responsvel por verificar se uma obra est ou no vigente.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool|null
 */
function verificaVigenciaObra($obrid)
{

    global $db;
    $sql = "SELECT dt_fim_vigencia_termo as dtvigencia, CASE WHEN dt_fim_vigencia_termo >= NOW() THEN 1 ELSE 2 END stiid FROM obras2.v_vigencia_obra_2016 WHERE obrid = {$obrid}";
    $dtVigencia = $db->pegaUm($sql);
    $vigencia = null;

    if ($dtVigencia) {
        $objDtVigencia = new DateTime($dtVigencia);
        $objDtAtual = new DateTime("now");
        $diff = $objDtVigencia->diff($objDtAtual);
        $diasRestantes = $diff->format("%R%a");

        if ($diasRestantes > 0) {
            $vigencia = false;
        } else {
            $vigencia = true;
        }
    }

    return $vigencia;
}

/**
 * Funo responsvel por obter a data fim do convênio. (Aba recursos)
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool|mixed|NULL|string
 */
function verificaDataFimConvenio($obrid)
{

    global $db;

    $sql = "SELECT
              b.dcodatafim
            FROM
              obras2.obras a
              LEFT JOIN painel.dadosconvenios b
                ON b.dcoprocesso = Replace(Replace(Replace(a.obrnumprocessoconv, '.', ''), '/', ''), '-', '')
            WHERE
              obrid = {$obrid}
            ";

    return $db->pegaUm($sql);
}

/**
 * Funo responsvel por obter a data fim do convênio. (Aba recursos)
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool|mixed|NULL|string
 */
function verificaNumeroConvenio($obrid)
{

    global $db;

    $sql = "SELECT
              b.dcodatafim
            FROM
              obras2.obras a
              INNER JOIN painel.dadosconvenios b
                ON b.dcoprocesso = Replace(Replace(Replace(a.obrnumprocessoconv, '.', ''), '/', ''), '-', '')
            WHERE
              obrid = {$obrid}
            ";


    if ($db->pegaUm($sql)) {
        return true;
    }
    return false;
}

/**
 * Funo responsvel por obter as situaes das previses de providência da obra.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool
 */
function obterSituacaoPrevisaoProvidencia($obrid)
{

    global $db;

    $sql = "SELECT
              -- r.rstdtprevisaoregularizacao,
              -- r.rstdtprevisaoregularizacao::DATE - NOW()::DATE diff,
              r.rstdsc,
              CASE
                WHEN (r.rstdtprevisaoregularizacao::DATE - NOW()::DATE < 0) THEN 0 ELSE 1
              END situacao
            FROM
              obras2.restricao r
            WHERE
              r.obrid = {$obrid}
            AND
              r.rstdtprevisaoregularizacao IS NOT NULL;
           ";

    $resultado = $db->carregar($sql);
    $previsaoProvidencia = true;
    if ($resultado) {
        foreach ($resultado as $r) {
            if ($r["situacao"] == 1 && $r["rstdsc"] != "Cumprimento do Objeto Dilignciado.") {
                $previsaoProvidencia = false;
                break;
            }
        }
    } else {
        $previsaoProvidencia = true;
    }

    return $previsaoProvidencia;
}

/**
 * Funo que verifica a composio de uma obra concluda.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid ID da obra.
 * @return bool
 */
function verificaComposicaoConcluida($obrid)
{

    global $db;

    /* Pesquisando o 'proid' de acordo com o id da obra. */
    $arrProidOrigemObra = array();

    $sql1 = "SELECT
          foo.proid,
          foo.origem
        FROM (
          SELECT
            obr.obrid,
            pro.proid,
            'par' as origem
          FROM
            par.processoobrasparcomposicao pro
          INNER JOIN
            obras2.obras obr ON obr.preid = pro.preid
          INNER JOIN
            par.processoobraspar pop ON pop.proid = pro.proid AND pop.prostatus = 'A' :: BPCHAR
          WHERE
            pro.pocstatus = 'A' :: BPCHAR
          AND
            obr.obridpai IS NULL
        
          UNION ALL
        
          SELECT
            obr.obrid,
            pro.proid,
            'pac' as origem
          FROM
            par.processoobraspaccomposicao pro
          INNER JOIN
            obras2.obras obr ON obr.preid = pro.preid
          INNER JOIN
            par.processoobra pobr ON pobr.proid = pro.proid AND pobr.prostatus = 'A' :: BPCHAR
          WHERE
            pro.pocstatus = 'A' :: BPCHAR
          AND
            obr.obridpai IS NULL
        ) foo
        WHERE
          foo.obrid = {$obrid};";

    $retorno = $db->pegalinha($sql1);

    if ($retorno !== false) {
        $arrProidOrigemObra = $retorno;
    }

    if (!empty($arrProidOrigemObra)) {
        if ($arrProidOrigemObra["origem"] == 'par') { // Origem da obra: PAR.
            $sqlComposicao = "
            SELECT
              obr.obrid,
              pro.proid,
              obr.strid
            FROM
              par.processoobrasparcomposicao pro
            INNER JOIN
              obras2.obras obr ON obr.preid = pro.preid
            INNER JOIN
              par.processoobraspar pop ON pop.proid = pro.proid AND pop.prostatus = 'A' :: BPCHAR
            WHERE
              pro.pocstatus = 'A' :: BPCHAR
            AND
              obr.obridpai IS NULL
            AND
              pro.proid = {$arrProidOrigemObra["proid"]};";
        } elseif ($arrProidOrigemObra["origem"] == 'pac') { // Origem da obra: PAC.
            $sqlComposicao = "
            SELECT
              obr.obrid,
              pro.proid,
              obr.strid
            FROM
              par.processoobraspaccomposicao pro
            INNER JOIN
              obras2.obras obr ON obr.preid = pro.preid
            INNER JOIN
              par.processoobra pop ON pop.proid = pro.proid AND pop.prostatus = 'A' :: BPCHAR
            WHERE
              pro.pocstatus = 'A' :: BPCHAR
            AND
              obr.obridpai IS NULL
            AND
              pro.proid = {$arrProidOrigemObra["proid"]};";
        }
    } else {
        $sqlComposicao = "
        SELECT
          obr.obrid,
          obr.strid
        FROM
          obras2.obras obr
        WHERE
          Replace(Replace(Replace(obr.obrnumprocessoconv, '.', ''), '/', ''), '-', '') =
          (SELECT Replace(Replace(Replace(o.obrnumprocessoconv, '.', ''), '/', ''), '-', '')
            FROM
              obras2.obras o
            WHERE
              obrid = {$obrid}
          )
        AND
          obr.obrstatus = 'A' :: BPCHAR
        AND
          obr.obridpai IS NULL";
    }

    $composicao = $db->carregar($sqlComposicao);

    if (is_array($composicao)) {
        foreach ($composicao as $c) {
            if ($c["strid"] != 6 && $c["strid"] != 7 && $c["strid"] != 8) {
                return false;
            }
        }
        return true;
    }

    return false;
}

/**
 * Funo que verifica as regras de bloqueio e desbloqueio da abra 'Cumprimento do objeto'.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $obrid Id da obra.
 * @return bool
 */
function verificaCumprimentoObjeto($obrid)
{

    /* Verificando se no houve um bloqueio/desbloqueio forado (manual). */
    if (!verificaBloqueioManual($obrid)) {
        $situacaoObra = verificaSituacaoObra($obrid);
        //$co = new CumprimentoObjeto();
        $obra = new Obras($obrid);

        $pagamentosEfetuados = $obra->getPagamentoPar($obrid);
        $pagamentoEfetivadoExists = false;

        if ($situacaoObra == 8) { // OBRA CANCELADA
            // VERIFICANDO SE A ORIGEM DA OBRA  'CONVNIO'.
            if (verificaOrigemObra($obrid) == 2 || verificaNumeroConvenio($obrid) == true) {
                $pagamentoEfetivadoExists = true;
            } else {
                // VERIFICANDO SE EXISTE PELO MENOS UM PAGAMENTO EFETIVADO.
                if (is_array($pagamentosEfetuados)) {
                    foreach ($pagamentosEfetuados as $pagamento) {
                        if (strpos($pagamento["pagsituacaopagamento"], "EFETIVADO") !== false) {
                            $pagamentoEfetivadoExists = true;
                            break;
                        }
                    }
                }
            }

            if (!$pagamentoEfetivadoExists) { // SE NO HOUVER NENHUM PAGAMENTO EFETIVADO
                return false;
            } else {
                return true;
            }
        } elseif ($situacaoObra == 7) { // OBRA INACABADA
            return true;
        } elseif ($situacaoObra == 6) { // OBRA CONCLUDA
            $vigencia = null;
            $situacaoEfetivada = false; // Varivel que indica se a obra est 100% paga.

            if (verificaOrigemObra($obrid) == 2 || verificaNumeroConvenio($obrid) == true) { // CONVNIO
                // Para a origem 'Convênio'  assumido que a obra est 100% paga.
                $situacaoEfetivada = true;

                // Pesquisando na tabela 'registroatividades'.
                $dtvigencia = verificaLeituraNotificacaoObraConcluida($obrid);

                if ($dtvigencia) { // Se houver data de vigncia na tabela 'registroatividades'...
                    $dtAtual = new DateTime("now");

                    // Obtendo a data da tabela 'registroatividades' e somando mais 45 dias.
                    $dtvigencia = new DateTime($dtvigencia);
                    $dtvigencia->add(new DateInterval("P45D"));

                    // Verificando a vigncia a partir da nova data (45 dias).
                    if ($dtvigencia->diff($dtAtual) > 0) {
                        $vigencia = false;
                    } else {
                        $vigencia = true;
                    }
                } else {
                    $dtfimconvenio = verificaDataFimConvenio($obrid);
                    if ($dtfimconvenio) {
                        $dtAtual = new DateTime("now");
                        $dtfimconvenio = new DateTime($dtfimconvenio);

                        // Verificando se a vigncia a partir da nova data (45 dias).
                        if ($dtfimconvenio->diff($dtAtual) > 0) {
                            $vigencia = false;
                        } else {
                            $vigencia = true;
                        }
                    } else {
                        // Caso no haja data de vigncia campo 'Fim' na aba 'Recursos' (para convênio) e nem na tabela 'registroatividades', bloqueia a aba 'Cumprimnento do objeto'.
                        return false;
                    }
                }
            } else { // PROCESSO
                // Verificando a data de vigncia da aba "Recursos".
                $vigencia = verificaVigenciaObra($obrid);

                if ($vigencia === null) { // Se no existir data de vigncia na aba recursos, pesquisar na tabela 'registroatividades'.
                    // Pesquisando na tabela 'registroatividades'.
                    $dtvigencia = verificaLeituraNotificacaoObraConcluida($obrid);

                    if ($dtvigencia) { // Se houver data de vigncia na tabela 'registroatividades'...
                        $dtAtual = new DateTime("now");

                        // Obtendo a data da tabela 'registroatividades' e somando mais 45 dias.
                        $dtvigencia = new DateTime($dtvigencia);

                        $dtvigencia->add(new DateInterval("P45D"));

                        // Verificando se a vigncia a partir da nova data (45 dias).
                        if ($dtvigencia->diff($dtAtual) > 0) {
                            $vigencia = false;
                        } else {
                            $vigencia = true;
                        }
                    } else {
                        // Caso no haja data de vigncia na aba 'Recursos' e nem na tabela 'registroatividades', bloqueia a aba 'Cumprimnento do objeto'.
                        return false;
                    }
                }
            }

            if ($vigencia === false) { // Obra vencida.
                if ($pagamentosEfetuados) {
                    $calc = 0;
                    array_walk($pagamentosEfetuados, function ($v, $k) use (&$calc) {
                        if (strpos($v["pagsituacaopagamento"], "EFETIVADO") !== false) {
                            $percentual = round($v["percentualpagamento"], 2);
                            $calc += $percentual;
                        }
                    }, $calc);

                    if ($calc < 100) {
                        $situacaoEfetivada = false;
                    } else {
                        $situacaoEfetivada = true;
                    }
                }

                if ($situacaoEfetivada === false) {
                    // Se a obra no for 100% efetivada, bloqueia-se a aba 'Cumprimento do objeto'.
                    return false;
                } else {
                    // Se todas as previses de providência estiverem vencidas, liberar a aba 'Cumprimento do objeto'.
                    if (obterSituacaoPrevisaoProvidencia($obrid)) {
                        return true;
                    } else {
                        // Se pelo menos uma previso de providência no estiver vencida, bloquear a aba 'Cumprimento do objeto'.
                        return false;
                    }
                }
            } elseif ($vigencia === true) { // Obra em vigncia.
                if ($pagamentosEfetuados) {
                    $calc = 0;
                    array_walk($pagamentosEfetuados, function ($v, $k) use (&$calc) {
                        if (strpos($v["pagsituacaopagamento"], "EFETIVADO") !== false) {
                            $percentual = round($v["percentualpagamento"], 2);
                            $calc += $percentual;
                        }
                    }, $calc);

                    if ($calc < 100) {
                        $situacaoEfetivada = false;
                    } else {
                        $situacaoEfetivada = true;
                    }
                }

                if ($situacaoEfetivada === false) {
                    // Se a obra no for 100% efetivada, bloqueia-se a aba 'Cumprimento do objeto'.
                    return false;
                } else {
                    // Se todas as previses de providência estiverem vencidas, liberar a aba 'Cumprimento do objeto'.
                    if (obterSituacaoPrevisaoProvidencia($obrid)) {
                        $solicitacoes = capturaSolicitacoesObra($obrid);
                        $solUsoSaldoDeferida = false;
                        foreach ($solicitacoes as $solicitacao) {
                            if ($solicitacoes["aprovado"] == "S" && $solicitacoes["tslid"] == 2) {
                                $solUsoSaldoDeferida = true;
                                break;
                            }
                        }

                        // Se houver solicitação de uso de saldo deferida, bloqueia a aba 'Cumprimento do objeto'.
                        if ($solUsoSaldoDeferida) {
                            return false;
                        } else {
                            if (verificaComposicaoConcluida($obrid)) {
                                return true;
                            } else {
                                return false;
                            }
                        }
                    } else {
                        // Se pelo menos uma previso de providência no estiver vencida, bloquear a aba 'Cumprimento do objeto'.
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
    }
}

/**
 * Funo responsvel por obter a data da Última vistoria de uma obra.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10733
 * @param $usucpf CPF do usuario.
 * @param $flgFiscal Flag que identifica se o usuario  um fiscal de obra.
 * @return bool
 * @throws Exception
 */
function verificaDataUltimaVistoriaObra($usucpf, $flgFiscal)
{

    $retorno = true;

    if ($usucpf && is_bool($flgFiscal)) {
        global $db;

        $perfilFiscal = PFLCOD_SUPERVISOR_UNIDADE;
        $perfilConsultaUnidade = PFLCOD_CONSULTA_UNIDADE;
        $perfilGestorUnidade = PFLCOD_GESTOR_UNIDADE;

        if ($flgFiscal === true) {
            $sql = "SELECT
                      o.obrid,
                      d.docid,
                      o.obrdtultvistoria,
                      o.strid
                    FROM
                      obras2.obras o
                    LEFT JOIN
                      obras2.empreendimento ep ON ep.empid = o.empid
                    LEFT JOIN
                      entidade.entidade en  ON en.entid = ep.entidunidade
                    LEFT JOIN
                      workflow.documento d ON d.docid = o.docid
                    LEFT JOIN
                      workflow.estadodocumento ed ON ed.esdid = d.esdid
                    WHERE
                      o.obridpai IS NULL
                    AND
                      o.obrstatus = 'A'
                    AND
                      o.strid IN (4,5)
                    AND
                      ep.empid IN (
                        SELECT
                          usr.empid
                        FROM
                          obras2.usuarioresponsabilidade usr
                        WHERE
                          usr.pflcod IN ({$perfilFiscal}, {$perfilConsultaUnidade})
                        AND
                          usr.rpustatus = 'A'
                        AND
                          usr.usucpf = '{$usucpf}'
                        AND
                          usr.empid IN (SELECT ur.empid FROM obras2.usuarioresponsabilidade ur WHERE ur.rpustatus = 'A' AND ur.usucpf = '{$usucpf}' AND ur.empid IS NOT NULL)
                      )
                    ORDER BY o.obrid;";
        } elseif ($flgFiscal === false) {
            $sql = "
                SELECT
                  o.obrid,
                  d.docid,
                  o.obrdtultvistoria,
                  o.strid
                FROM
                  obras2.obras o
                LEFT JOIN
                  obras2.empreendimento ep ON ep.empid = o.empid
                LEFT JOIN
                  entidade.entidade en  ON en.entid = ep.entidunidade
                LEFT JOIN
                  workflow.documento d ON d.docid = o.docid
                LEFT JOIN
                  workflow.estadodocumento ed ON ed.esdid = d.esdid
                WHERE
                  o.obridpai IS NULL
                AND
                  o.obrstatus = 'A'
                AND
                  o.strid IN (4,5)
                AND
                  ep.entidunidade IN (SELECT ur.entid FROM obras2.usuarioresponsabilidade ur WHERE ur.rpustatus = 'A' AND ur.pflcod IN ({$perfilGestorUnidade}) AND ur.usucpf = '{$usucpf}')
                ORDER BY o.obrid;
            ";
        } else {
            throw new Exception("Parmetro invlido.");
        }

        $arrObras = $db->carregar($sql);

        if (!empty($arrObras)) {
            foreach ($arrObras as $obra) {
                if ($obra["obrdtultvistoria"] == "") {
                    $sql2 = "SELECT
                              min(hd.htddata)
                            FROM
                              workflow.historicodocumento hd
                            WHERE
                              hd.docid = {$obra["docid"]}
                            AND
                              hd.aedid IN (
                                SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690
                              );";

                    $dtUltVistoria = $db->pegaUm($sql2);
                } else {
                    $dtUltVistoria = $obra["obrdtultvistoria"];
                }

                $objDtUltVistoria = new DateTime($dtUltVistoria);
                $objDtAtual = new DateTime("now");
                $diff = $objDtUltVistoria->diff($objDtAtual);
                $diasRestantes = $diff->format("%R%a");

                if ($diasRestantes > 30) {
                    $retorno = false;
                    break;
                }
            }
        }
    }

    return $retorno;
}

/**
 * Funo responsvel por verificar se houve leitura de notificao sobre atualizaao de uma obra.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10733
 * @param $obrid
 * @return bool|mixed|NULL|string
 */
function verificaLeituraAvisoObraDesatualizada($obrid)
{

    global $db;

    $sql = "
        SELECT
          rga.rgaid
        FROM
          obras2.registroatividade rga
        WHERE
          rga.obrid = {$obrid}
        AND
          rga.rgadscsimplificada ILIKE 'Confirmao de leitura sobre aviso de atualizao de obra'
        AND
          rga.rgastatus = 'A';";

    return $db->pegaUm($sql);
}

/**
 * Funo responsvel por obter a data da Última vistoria de uma obra MI.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $usucpf CPF do usurio
 * @param $flgFiscal Flag que identifica se o usuario  um fiscal de obra.
 * @return bool
 * @throws Exception
 */
function verificaDataUltimaVistoriaObraMI($usucpf, $flgFiscal)
{

    $retorno = true;

    if ($usucpf && is_bool($flgFiscal)) {
        global $db;

        $perfilFiscal = PFLCOD_SUPERVISOR_UNIDADE;
        $perfilConsultaUnidade = PFLCOD_CONSULTA_UNIDADE;
        $perfilGestorUnidade = PFLCOD_GESTOR_UNIDADE;

        if ($flgFiscal === true) {
            $sql = "SELECT DISTINCT ON (o.obrid)
                            o.obrid,
                            o.docid,
                            o.strid,
                            obrdtultvistoria
                        FROM
                        obras2.obras o
                        INNER JOIN
                            entidade.endereco ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = 4
                        LEFT JOIN
                              workflow.documento d ON d.docid    = o.docid AND d.tpdid = 105
                        LEFT JOIN
                              obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                        JOIN
                            obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A'
                        AND
                            urs.usucpf = '{$usucpf}'
                        AND
                            urs.pflcod IN ({$perfilFiscal}, {$perfilConsultaUnidade})
                        AND
                            ( urs.entid = e.entidunidade AND e.empid IN(SELECT empid FROM obras2.usuarioresponsabilidade WHERE rpustatus = 'A' AND empid IS NOT NULL AND pflcod IN ({$perfilFiscal}, {$perfilConsultaUnidade}) AND usucpf = '{$usucpf}'))
                        WHERE
                           o.obrstatus = 'A'  AND o.obridpai IS NULL AND d.esdid IN(690, 691) AND e.orgid IN(3) AND o.tpoid IN (104, 105) AND COALESCE(obrpercentultvistoria, 0) >= 0
                        ORDER BY o.obrid;";
        } elseif ($flgFiscal === false) {
            $sql = "
                SELECT DISTINCT ON (o.obrid)
                    o.obrid,
                    o.docid,
                    o.strid,
                    obrdtultvistoria
                 FROM
                    obras2.obras o
                INNER JOIN
                    entidade.endereco ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = 4
                LEFT JOIN
                      workflow.documento d ON d.docid    = o.docid AND d.tpdid = 105
                LEFT JOIN
                      obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                JOIN
                    obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A'
                AND
                    urs.usucpf = '{$usucpf}'
                AND
                    urs.pflcod IN ({$perfilGestorUnidade})
                AND
                    (urs.entid = e.entidunidade)
                WHERE
                   o.obrstatus = 'A'  AND o.obridpai IS NULL AND d.esdid IN(690,691) AND e.orgid IN(3) AND o.tpoid IN (104, 105) AND COALESCE(obrpercentultvistoria, 0) >= 0
                ORDER BY o.obrid;
            ";
        } else {
            throw new Exception("Parmetro invlido.");
        }

        $arrObras = $db->carregar($sql);
        if (!empty($arrObras)) {
            foreach ($arrObras as $obra) {
                if ($obra["obrdtultvistoria"] == "") {
                    $sql2 = "SELECT
                                os.osmdtinicio
                            FROM
                                obras2.obras obr
                            JOIN
                                obras2.ordemservicomi os ON os.obrid = obr.obrid
                            WHERE
                                obr.obrid = {$obra["obrid"]}
                            AND
                                os.tomid = 1
                            AND
                                os.osmstatus = 'A'
                            ORDER BY os.osmid DESC LIMIT 1;
                            ";

                    $dtUltVistoria = $db->pegaUm($sql2);
                } else {
                    $dtUltVistoria = $obra["obrdtultvistoria"];
                }

                $objDtUltVistoria = new DateTime($dtUltVistoria);
                $objDtAtual = new DateTime("now");
                $diff = $objDtUltVistoria->diff($objDtAtual);
                $diasRestantes = $diff->format("%R%a");

                if ($diasRestantes > 30) {
                    $retorno = false;
                    break;
                }
            }
        }
    }

    return $retorno;
}


/**
 * Funo que exibe popup na lista de obras quando usario tem obras desatualizadas em sua lista e tenta acessar uma obra atualizada via url.
 */
function avisoRedirecionamentoObraAtualizada()
{

    ?>
    <div id="dialog" style="display: none;z-index:99999 !important;">
        <div>
            <p style="text-align:justify">
                O município possui obra(as) desatualizada(as) h mais de 30 dias.
                Informamos que, para obter acesso s outras abas,
                TODAS as obras com status execução ou PARALISADA devero estar atualizadas.
            </p>
        </div>
    </div>

    <script type='text/javascript'>

        var buttons = [];

        buttons.push(
            {
                text: "OK",
                click: function () {
                    $("#dialog").dialog("close");
                }
            }
        );

        jQuery(function () {
            jQuery('#dialog').dialog(
                {
                    title: 'Aviso',
                    resizable: false,
                    modal: true,
                    draggable: false,
                    buttons: buttons
                }
            );
        });</script>
    <?php
}

function trocaid_par_par3_dadosunidadetipo($idPar)
{
    switch ($idPar) {
        case 1:
            $id = null;
            break;
        case 2:
            $id = 4;
            break;
        case 3:
        case 4:
        case 5:
            $id = 5;
            break;
        case 6:
            $id = 1;
            break;
        case 7:
            $id = 2;
            break;
        case 8:
        case 9:
            $id = 3;
            break;
        case 10:
            $id = 9;
            break;
        case 11:
            $id = 7;
            break;
        case 12:
            $id = 8;
            break;
    }
    return $id;
}

function limparNumeros($string)
{
    return preg_replace('/[^A-Za-z0-9\-]/', '', $string);
}


function criarDocidExecucaoFinanceira($obrid)
{
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Execução Financeira Obras {}";

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento(TPDID_EXECUCAO_FINANCEIRA, $docdsc);

    // atualiza o DOCID na OBRA
    $obra = new Obras($obrid);
    $obra->docid = $docid;
    $obra->salvar();

    $db->commit();

    return $docid;
}

function pegaDocidExecucaoFinanceira($obrid)
{
    global $db;

    if (!$obrid) {
        return false;
    }

    $obra = new Obras($obrid);
    $docid = $obra->docid;

    // Bloqueia o cadastro de docid POR ESTA VIA para as obras MI
    if (!$obra->docid) {
        $docid = criarDocidObra($obrid);
    }

    return $docid;
}

function criarAcaoEstadoDocObras($esdidorigem, $esdiddestino)
{
    global $db;

    $sql = "INSERT INTO workflow.acaoestadodoc
                (esdidorigem,
                esdiddestino,
                aeddscrealizar,
                aedstatus,
                aeddscrealizada,
                esdsncomentario,
                aedvisivel,
                aedcodicaonegativa,
                aedordem)
              VALUES
                ({$esdidorigem},
                {$esdiddestino},
                'Enviar para obra cancelada',
                'A',
                'Enviado para obra cancelada',
                TRUE,
                FALSE,
                TRUE,
                0)
              RETURNING
                aedid";
    $result = $db->pegaUm($sql);
    $db->commit();
    return $result;
}

function posAcaoCancelarObraParEPac($obrid)
{
    global $db;
    $sql = "SELECT
            o.preid,
            ob.obrid,
            o.docid AS docid_par,
            ob.docid AS docid_obras,
            tpdid,
            wd.esdid
            FROM obras2.obras ob
            LEFT JOIN obras.preobra o ON o.preid = ob.preid
            LEFT JOIN workflow.documento wd ON wd.docid = o.docid
            WHERE ob.obrid = {$obrid}
                AND wd.esdid NOT IN (628, 368)";
    $result = $db->pegaLinha($sql);
    
    switch ($result['tpdid']) {
        //fluxo PAR2
        case 45:
            $esdiddestino = 628;
            break;
        //fluxo PAC2
        case 37:
            $esdiddestino = 368;
            break;
    }

    if( !empty($result['esdid']) && !empty($esdiddestino)){
        $sqlAcaoDoc = "SELECT
                        aedid
                    FROM workflow.acaoestadodoc
                    WHERE
                        esdidorigem = {$result['esdid']} AND
                        esdiddestino = {$esdiddestino}";

        $resultAcaoDoc = $db->pegaLinha($sqlAcaoDoc);

        if (!empty($resultAcaoDoc)) {
            wf_alterarEstado(
                $result['docid_par'],
                $resultAcaoDoc['aedid'],
                'Obra cancelada via Obras2',
                array('obrid' => $result['obrid'])
            );
            return true;
        } else {
            $aedid = criarAcaoEstadoDocObras($result['esdid'], $esdiddestino);
            wf_alterarEstado($result['docid_par'], $aedid, 'Obra cancelada via Obras2', array('obrid' => $result['obrid']));
            return true;
        }
    }else{
        return true;
    }
}
