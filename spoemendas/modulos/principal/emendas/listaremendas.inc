<?php
/**
 * Sistema de Publicação.
 * werteralmeida@
 */
include_once APPRAIZ . 'includes/workflow.php';
$perfis = is_array(pegaPerfilGeral($_SESSION['usucpf'])) ? pegaPerfilGeral($_SESSION['usucpf']) : array();
$fm = new Simec_Helper_FlashMessage('spoemendas/principal/emendas/listaremendas');

$dados = $_REQUEST['emendas'];
$where = " WHERE emdstatus = 'A' ";
//Tramita automatico limite liberado para preenchimento
(new Spoemendas_Model_Emenda())->tramitaAutomaticoEmendalimitLiberado();
/* Filtros por Perfil */
$perfisLinha = implode(',', $perfis);

if (in_array(PFL_PARLAMENTAR, $perfis)) {
    $sql = "SELECT autid FROM spoemendas.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND rpustatus = 'A'";
    $pfl = $db->carregar($sql);
    if (is_array($pfl)) {
        foreach ($pfl as $key => $value) {
            $usuresp[$key] = $value['autid'];
        }
    }
    if (is_array($usuresp)) {
        $usurespIn = implode(',', $usuresp);
        $whereParlamentar = " AND aut.autid IN ($usurespIn) ";
        if ($_GET['aguardando'] == '1') {
            $whereParlamentar .= ' AND doc.docid IS NULL ';
        }
        $where .= $whereParlamentar;
    } else {
        $whereParlamentar = " AND false ";
        $where .= $whereParlamentar;
        $fm->addMensagem('Seu perfil de Parlamentar não está vinculado a nenhum autor. <br/><br/> Solicite a vinculação a ASPAR do MEC (2022-7899/7896/7894)', Simec_Helper_FlashMessage::ERRO);
    }
    if (date('Y-m-d') == '2017-03-20') {
        $fm->addMensagem("Prorrogação do prazo para informações adicionais pelos Parlamentares até 20 de março, numa segunda-feira.", Simec_Helper_FlashMessage::ERRO);
    }
}
if (in_array(PFL_REITOR, $perfis) ||
    in_array(PFL_UO_EQUIPE_TECNICA, $perfis) ||
    in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis) ||
    in_array(PFL_TED, $perfis)
) {
//    $sql = "SELECT unicod FROM spoemendas.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND rpustatus = 'A'";
    $sql = <<<SQL
        WITH temp_unidade AS (
            SELECT 
                unicod
            FROM spoemendas.usuarioresponsabilidade
            WHERE
                usucpf = '%s'
                AND rpustatus = 'A'
                AND unicod IS NOT NULL 
            
            UNION 
            
            SELECT 
                ung.unicod
            FROM public.unidadegestora ung
            INNER JOIN spoemendas.usuarioresponsabilidade rpu USING(ungcod)
            WHERE 
                usucpf = '%s'
                AND rpustatus = 'A'
        )
        
        SELECT DISTINCT * FROM temp_unidade;
SQL;
    $sql = sprintf($sql,
        $_SESSION['usucpf'],
        $_SESSION['usucpf']
    );

    $pfl = $db->carregar($sql);
    if (is_array($pfl)) {
        foreach ($pfl as $key => $value) {
            $usuresp[$key] = $value['unicod'];
        }
    }
    if (is_array($usuresp)) {
        array_walk($usuresp, function (&$item) {
            $item = "'{$item}'";
        });

        $usurespIn = implode(',', $usuresp);
        $whereUnidade = " AND uni.unicod::integer IN ($usurespIn)";
        $where .= $whereUnidade;
    } else {
        $whereUnidade = " AND false ";
        $where .= $whereUnidade;
        $fm->addMensagem('Seu perfil de Reitor / UO Equipe Técnica não está vinculado a nenhuma Unidade. <br/><br/> Solicite a vinculação a CGO no MEC (2022-8846)', Simec_Helper_FlashMessage::ERRO);
    }
}

if (in_array(PFL_SECRETARIAS, $perfis) ||
    in_array(PFL_FNDE, $perfis)
) {
    $sql = "SELECT DISTINCT secid FROM spoemendas.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND rpustatus = 'A'";
    $pfl = $db->carregar($sql);
    $usuresp = [];
    if (is_array($pfl)) {
        foreach ($pfl as $key => $value) {

            if ($value['secid'] != null) {
                $usuresp[$key] = $value['secid'];
            }
        }
    }

    if (is_array($usuresp) && array_filter($usuresp)) {
        array_walk($usuresp, function (&$item) {
            $item = "'{$item}'";
        });
        $usurespIn = implode(',', $usuresp);
        $whereSecretaria = " AND sec.secid::integer IN ({$usurespIn}) ";
        $where .= $whereSecretaria;
    } else {
        $whereSecretaria = " AND FALSE ";
        $where .= $whereSecretaria;
        $fm->addMensagem('Seu perfil Secretaria não está vinculado a nenhuma secretaria. <br/><br/> Solicite a vinculação a CGO no MEC (2022-8846)', Simec_Helper_FlashMessage::ERRO);
    }
}

$listaExercicios = "SELECT
                        prsano AS codigo,
                        prsano AS descricao
                    FROM
                        spoemendas.programacaoexercicio
                    WHERE
                        prsexercicioaberto
                    ORDER BY
                        1 DESC";

if (!isset($dados['exercicios'])) {
    $lista[0]['codigo'] = $_SESSION['exercicio'];
    $dados['exercicios'] = [];

    if (is_array($lista)) {
        foreach ($lista as $chave => $valor) {
            array_push($dados['exercicios'], $valor['codigo']);
        }
    }

    $exercicios = implode("','", $dados['exercicios']);

    if (isset($exercicios)) {
        $whereAno = " WHERE eme.emeano IN ('{$exercicios}')";
    }

    /* Forçando o filtro por exercício */
    if (isset($dados['exercicios'])) {
        if (is_array($dados['exercicios']) && count($dados['exercicios']) < 2) {
            $dados['exercicios'] = explode(',', $dados['exercicios'][0]);
        }
        $exercicios = implode("','", $dados['exercicios']);
        $where .= " AND e.emeano in ('{$exercicios}')";
    }
}

if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    switch ($requisicao) {
        case 'filtrar':

            if (isset($dados['unicod']) && $dados['unicod'] != '') {
                $where .= " AND uni.unicod = '{$dados['unicod']}' ";
            }
            if (isset($dados['autid']) && $dados['autid'] != '') {
                //$where .= " AND aut.autid = {$dados['autid']} ";
                $autid = explode(",", $dados['autid']);
                $where .= " AND aut.autid IN (" . implode(',', $autid) . ") ";


            }
            if (isset($dados['emecod']) && $dados['emecod'] != '') {
                $where .= " AND e.emecod = '{$dados['emecod']}' ";
            }
            if (isset($dados['esdid']) && $dados['esdid'] != '') {
                $where .= " AND doc.esdid = '{$dados['esdid']}' ";
            }
            if (isset($dados['enbcnpj']) && $dados['enbcnpj'] != '') {
                $where .= " AND enbcnpj = '{$dados['enbcnpj']}' ";
            }
            if (isset($dados['etoid']) && !empty($dados['etoid'])) {
                if (is_array($dados['etoid'])) {
                    $etoids = implode('\',\'', $dados['etoid']);
                    $where .= " AND etoid IN ('{$etoids}') ";
                } else {
                    $where .= " AND etoid = {$dados['etoid']}";
                }
            }
            if (isset($dados['secid']) && $dados['secid'] != '') {
                $where .= " AND secid = '{$dados['secid']}' ";
            }
            if (isset($dados['acacod']) && $dados['acacod'] != '') {
                $where .= " AND vfp.acacod = '{$dados['acacod']}' ";
            }
            if (isset($dados['exercicios'])) {
                if (is_array($dados['exercicios']) && count($dados['exercicios']) < 2) {
                    $dados['exercicios'] = explode(',', $dados['exercicios'][0]);
                }
                if (is_array($dados['exercicios'])) {
                    $dados['exercicios'] = array_filter($dados['exercicios'], 'strlen');
                    $exercicios = implode("','", $dados['exercicios']);
                } else {
                    $exercicios = $dados['exercicios'];
                }
                $where .= " AND e.emeano in ('{$exercicios}')";
                $whereAno = " WHERE eme.emeano in ('{$exercicios}')";
            }
            break;
        case 'ver-emenda':
            echo retornaModalDetalheEmenda($_REQUEST);
            die();
            break;
        case 'detalharIniciativa':
            echo retornarDetalharIniciativa($_REQUEST);
            die();
            break;
        case 'drawWorkflow':
            list(, $docid, $solid) = $_POST['params'];
            wf_desenhaBarraNavegacaoBootstrap($docid, ['docid' => $docid, 'solid' => $solid]);
            die();
        case 'historicoBootstrapComentario':
            echo retornaComentarioHistorico($_REQUEST['dados']);
            die;
        case 'imprimirModalEmenda':
            $_REQUEST['imprimir'] = true;

            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.css" rel="stylesheet" media="screen">';
            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">';

            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.css" rel="stylesheet" media="print">';
            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="print">';

            echo "<style>
                .btn{display:none;}
                .navbar{display:none;}
                .tabela-listagem{ text-align: center;}
                </style>
                ";
            echo '
            <br/>
            <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen" >
            <tr bgcolor="#ffffff">
                <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
                <td height="20" nowrap>
                    SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação<br>
                    Ministério da Educação / SE - Secretaria Executiva<br>
                    DTI - Diretoria de Tecnologia da Informação<br>
                </td>
                <td height="20" align="right" style="font-size:9px;">
                    Impresso por: <strong>' . $_SESSION['usunome'] . '</strong><br>
                    Hora da Impressão:' . date("d/m/Y - H:i:s") . '
                </td>
            </tr>
        </table>
        <div style="height: 60px;" class="espacador-simec" ></div>';
            echo retornaModalDetalheEmenda($_REQUEST);
            echo '<script>window.print();</script>';
            die;
        case 'imprimirResumoParlamentar':
            $_REQUEST['imprimir'] = true;

            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.css" rel="stylesheet" media="screen">';
            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">';

            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.css" rel="stylesheet" media="print">';
            echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="print">';

            echo "<style>
                .btn{display:none;}
                .navbar{display:none;}
                .tabela-listagem{ text-align: center;}
                .table{font-size:11px}
                .table tr td:first-child,.table tr th:first-child{display:none}
                </style>
                ";
            echo '
            <br/>
            <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen" >
            <tr bgcolor="#ffffff">
                <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
                <td height="20" nowrap>
                    <b>SIMEC</b> - Sistema Integrado de Monitoramento do Ministério da Educação<br>
                    Ministério da Educação / SE - Secretaria Executiva<br>
                    DTI - Diretoria de Tecnologia da Informação<br>
                </td>
                <td height="20" align="right" style="font-size:9px;">
                    Impresso por: <strong>' . $_SESSION['usunome'] . '</strong><br>
                    Hora da Impressão:' . date("d/m/Y - H:i:s") . '
                </td>
            </tr>
        </table>
        <div style="height: 60px;" class="espacador-simec" ></div>';
            include_once 'painelacompanhamento/parlamentar/resumo.inc';
            die;
    }
}

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

$bc = new Simec_View_Breadcrumb('C');
$bc->add('Emendas', 'spoemendas.php?modulo=principal/emendas/listaremendas&acao=A')
    ->add('Lista de Emendas')
    ->render();

$ememomentosiop = $db->pegaUm("select max(ememomentosiop) from emenda.emenda where emeano = '{$_SESSION['exercicio']}'");

?>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
        $(function () {
            $('[data-toggle="tooltip"]').tooltip()
        });

        $('.tabela-listagem th').eq(0).html('Momento: ' + $('[name="ememomentosiop"]').val());
    });

    /**
     * Detalhar a Emenda
     */
    function verEmenda(emeid) {
        $('#modal-ver-emenda .modal-body').html('');
        $.ajax({
            url: "spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=ver-emenda&emeid=" + emeid,
        }).done(function (html) {
            conteudo = html;
            $('#modal-ver-emenda .modal-body').html(conteudo);
        });

        $('#modal-ver-emenda').modal();

    }

    /**
     * Editar a Emenda
     */
    function alterarEmenda(emeid) {
        location.href = "spoemendas.php?modulo=principal/emendas/editaremenda/editaremenda&acao=A&emeid=" + emeid;

    }

    // Função responsável por imprimir emenda
    function imprimirModalEmenda(emeid) {
        window.open('spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=imprimirModalEmenda&emeid=' + emeid);
    }
</script>
<style>
    * {
        white-space: normal !important;
    }
</style>
<div class="row col-lg-12">
    <input type="hidden" name="ememomentosiop" id="ememomentosiop" value="<?php echo $ememomentosiop; ?>">
    <?php
    /*
     * Formulário dos Filtros
     */
    $listaUnidades = "SELECT
                        uni.unicod                      AS codigo,
                        uni.unicod ||' - ' ||uni.unidsc AS descricao
                    FROM
                        public.unidade uni
                    WHERE
                        unistatus = 'A'
                    AND orgcod = '26000'
                    $whereUnidade
                    ORDER BY
                        1";
    $listaParlamentares = "SELECT
                            aut.autid                                  AS codigo,
                            aut.autnome || ' (' || par.parsigla || ')' AS descricao
                        FROM
                            emenda.autor aut
                        JOIN
                            emenda.partido par
                        ON
                            aut.parid = par.parid
                        WHERE
                            aut.autstatus = 'A'
                        AND aut.autid IN
                            (
                                SELECT
                                    autid
                                FROM
                                    emenda.emenda eme
                                    $whereAno)
                        {$whereParlamentar}
                        ORDER BY
                            2";
    $listaBeneficiarios = "SELECT DISTINCT
                            enb.enbcnpj AS codigo,
                            SUBSTR(enb.enbcnpj, 1, 2) || '.' || SUBSTR(enb.enbcnpj, 3, 3) || '.' || SUBSTR(enb.enbcnpj, 6,
                            3) || '/' || SUBSTR(enb.enbcnpj, 9, 4) || '-' || SUBSTR(enb.enbcnpj, 13) || ' - ' ||
                            enb.enbnome AS descricao,
                            enb.enbnome
                        FROM
                            emenda.emenda eme
                        JOIN
                            emenda.emendadetalhe emd
                        USING
                            (emeid)
                        JOIN
                            emenda.emendadetalheentidade ede
                        USING
                            (emdid)
                        JOIN
                            emenda.entidadebeneficiada enb
                        USING
                            (enbid)
                        $whereAno
                        	and emdstatus = 'A' and edestatus = 'A'
                            AND enb.enbcnpj <>''
                        ORDER BY
                            enb.enbnome";
    $tpdid = WF_TPDID_SPOEMENDAS;
    $listaEstadosDocumento = "SELECT
                            esdid  AS codigo,
                            esddsc AS descricao
                        FROM
                            workflow.estadodocumento where tpdid = {$tpdid}
                        ORDER BY 2";

    $listaAcoes = "SELECT DISTINCT
                    aca.acacod AS codigo,
                    aca.acacod || ' - ' || mon.acadsc AS descricao
                    FROM spoemendas.acao aca
                    INNER JOIN public.secretaria sec USING (secid)
                    INNER JOIN monitora.acao mon USING (acacod)
                    WHERE TRUE {$whereSecretaria} AND mon.prgano='{$_SESSION['exercicio']}'
                    ORDER BY 2";

    $form = new Simec_View_Form('emendas', $method = Simec_View_Form::POST, $action = '');
    $form->setTitulo('Filtro');
    $form->carregarDados($dados);
    $form->addInputCombo('unicod', $listaUnidades, $dados['unicod'], 'unicod', ['flabel' => 'Unidade Orçamentária (UO)'])
        ->addInputCombo('acacod', $listaAcoes, $dados['acacod'], 'acacod', ['flabel' => 'Ação']);
    if (in_array(PFL_PARLAMENTAR, $perfis)) {
        $form->addHidden('autid', $usurespIn, 'autid', ['flabel' => 'Autor']);
    } else {
        $form->addInputCombo('autid', $listaParlamentares, $dados['autid'], 'autid', ['flabel' => 'Autor']);
    }
    $form->addInputCombo('esdid', $listaEstadosDocumento, $dados['esdid'], 'esdid', ['flabel' => 'Situação'])
        ->addInputTexto("emecod", $dados['emecod'], "emecod", 20, false, ['flabel' => 'Emenda'])
        ->addInputCombo('enbcnpj', $listaBeneficiarios, $dados['enbcnpj'], 'enbcnpj', ['flabel' => 'Beneficiário'])
        ->addInputCombo('exercicios', $listaExercicios, $dados['exercicios'], 'exercicios', ['flabel' => 'Exercício(s)', 'multiple' => 'true'])
        ->addChoices('Forma de Execução', 'etoid', ['Todas' => '', 'PAR' => '1', 'Convênios' => '3', 'TED' => '2', 'Apl. Direta' => '4', 'PAR/CONV' => '5'])
        ->setRequisicao('filtrar')
        ->addBotoes(['limpar', 'buscar']);

    if ($_REQUEST['filtro'] != 'none') {
        $form->render();
    }

    echo $fm->getMensagens();

    /*
     * Listagem
     */

    /**
     * @todo
     * refataora a view v_emendadetalheentidade
     */

    $listagem = new Simec_Listagem();
    $arrColunas = [
        'Ano',
        'Código da Emenda',
        'RP Atual',
        'Forma de Execução',
        'Unidade Orçamentária (UO)',
        'Açao',
        'Autor',
        'Situação',
        'Valor (Dot. Atualizada) (a)',
        'Indicado (b)',
        'Impedido (c)',
        'Limite de Empenho (d)',
        'Empenhado (e)',
        'Pago (f)',
        '% do Empenho (Emp./Lim.Emp.) (g)=(e/d)'];
    //Verificacao das permissoes de edicao de acordo com o periodo forma de execucao e resultado primario
    $periodoPreenchimento = new Spoemendas_Controller_Periodopreenchimentoemenda();
    $permissao = $periodoPreenchimento->getPermissaoPreenchimento();
    $todos = EDIT_TODAS_FORMA_EXEC;
    $sql = <<<DML
SELECT DISTINCT e.emeid,
		       e.emeano,
		       e.emecod,
		       CASE WHEN e.etoid IN ({$permissao['etoid']})
		              AND e.emeano = {$_SESSION['exercicio']}
		              AND emdimpositiva IN ({$permissao['pperesultprimario']})
		              OR '{$permissao['etoid']}' = '{$todos}' THEN true
		            ELSE false
		         END AS editavel,
		       emdimpositiva,
		       CASE WHEN etoid = 1 THEN 'PAR'
		            WHEN etoid = 2 THEN 'TED'
		            WHEN etoid = 3 THEN 'CONV'
		            WHEN etoid = 4 THEN 'DIR'
		            WHEN etoid = 5 THEN 'PAR/CONV' END AS origem,
		       vf.unicod ||' - ' ||uni.uniabrev ||' - ' ||uni.unidsc AS unidade,
		       vfp.acacod AS acao,
		       vfp.acacod || ' - ' || vfp.fupdsc AS acaodsc,
		       aut.autcod || ' - ' || aut.autnome || ' ('  || par.parsigla || ' / '  || aut.estuf || ') ' AS autor,
		       COALESCE(esd.esddsc, 'Aguardando preenchimento') AS situacao,
       			e.emevalor AS valoremenda,
       
       
        0 AS valordetalhado,
       /* (SELECT SUM(edevalor)
          FROM emenda.v_emendadetalheentidade
          WHERE emeid = e.emeid
            AND edestatus = 'A') AS valordetalhado, /*indicado*/ */
            
            
       (SELECT COALESCE(SUM(edivalor), 0)
          FROM emenda.emendadetalheimpositivo edi
            INNER JOIN emenda.emendadetalheentidade ede_1 USING(edeid, emdid)
            INNER JOIN emenda.emendadetalhe emd_1 using(emdid)
            INNER JOIN emenda.emenda eme_1 using(emeid)
          WHERE eme_1.emeid = e.emeid
            AND emd_1.emdstatus = 'A'
            AND edi.edistatus = 'A'
            AND ede_1.edestatus = 'A') AS impedimento, /*impedido*/
       SUM(DISTINCT sex.vlrlimiteempenho) AS limite,
       SUM(DISTINCT sex.vlrempenhado) AS empenhado,
       0 AS pago,
		CEIL(CASE WHEN SUM(DISTINCT sex.vlrempenhado) > 0 THEN
				CASE WHEN SUM(DISTINCT sex.vlrlimiteempenho) > 0 THEN (SUM(DISTINCT sex.vlrempenhado)*100)/SUM(DISTINCT sex.vlrlimiteempenho) ELSE 0 END
		ELSE 0 END) AS limiteempenhado
  FROM emenda.emenda e
    LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid AND emdstatus = 'A')
    INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
    INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
    LEFT JOIN emenda.partido par on aut.parid = par.parid
    LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
    JOIN public.unidade uni ON vf.unicod = uni.unicod
    LEFT JOIN public.secretariaunidade secuo ON uni.unicod = secuo.unicod
    LEFT JOIN public.secretaria sec ON sec.secid = secuo.secid
    LEFT JOIN spo.siopexecucao sex ON (sex.emecod::text = e.emecod::text AND sex.exercicio::text = e.emeano::text)
    LEFT JOIN spoemendas.emendainformacoes ine ON ine.emeid = e.emeid
    LEFT JOIN emenda.entidadebeneficiada enb USING(enbid)
    LEFT JOIN workflow.documento doc USING(docid)
    LEFT JOIN workflow.estadodocumento esd USING(esdid)
    LEFT JOIN emenda.v_funcionalprogramatica vfp ON vfp.acaid = e.acaid
  {$where}
  GROUP BY e.emeid,
           e.emeano,
           e.emecod,
           editavel,
           emdimpositiva,
           origem,
           unidade,
           acao,
           acaodsc,
           autor,
           esddsc
  ORDER BY e.emeano DESC,
           e.emecod ASC
DML;

    echo "<div>RP = Resultado Primário
          <br>
          R$ = 1,00</div>";
    $listagem->setId('principal-emendas-listaremendas');
    $listagem->addAcao('edit', 'alterarEmenda');
    $listagem->addAcao('info', 'verEmenda');
    $listagem->esconderColunas('editavel', 'acao');
    $listagem->setAcaoComoCondicional(
        'edit', [['campo' => 'editavel', 'op' => 'igual', 'valor' => 't']]
    );
    $listagem->addCallbackDeCampo('acaodsc', 'alinhaParaEsquerda');
    $listagem->addCallbackDeCampo(['beneficiario', 'autor'], 'alinhaParaEsquerda');
    $listagem->addCallbackDeCampo('impedimento', 'pegaImpedimentos2');
    $listagem->addCallbackDeCampo(['valoremenda', 'valordetalhado', 'valorpresidencia', 'valordisponivel', 'limite', 'empenhado', 'saldo', 'pago'], 'mascaraMoeda');
    $listagem->addCallbackDeCampo('limiteempenhado', 'callbackProgressBar');
    $listagem->addCallbackDeCampo('impositiva', 'tratarSimNao');
    $listagem->addCallbackDeCampo('unidade', 'formatarUnidadeListagem');
    $listagem->addCallbackDeCampo('acaodsc', 'formatarAcaoListagem');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['valoremenda', 'valordetalhado', 'edevalordisponivel', 'empenhado', 'limite', 'saldo', 'liquidado', 'pago', 'impedimento']);
    $listagem->setFormFiltros('emendas');
    $listagem->turnOnOrdenacao();
    $listagem->setCabecalho($arrColunas)
        ->setQuery($sql);
    $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    ?>
</div>
<div class="modal fade" id="modal-ver-emenda">
    <div class="modal-dialog-large">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhes da Emenda Parlamentar</h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
