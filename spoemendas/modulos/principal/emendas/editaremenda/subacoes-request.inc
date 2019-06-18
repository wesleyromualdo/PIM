<?php
switch ($requisicao) {
    case 'salvar-subacoes-par':
        global $db;
        /*
         * Salvar iniciativas do Beneficiário
         */
        $dados = $_REQUEST['subacoes-par'];
        /*
         * Apaga todas, exceto as subações que não podem ser apagadas (Já vinculadas no PAR)
         */
        $sqlDelete = "
        DELETE
        FROM
            emenda.emendapariniciativa
        WHERE
            edeid = {$dados['edeid']}
        AND ppsid NOT IN
                          (
                          SELECT DISTINCT
                              s.ppsid
                          FROM
                              par.subacao s
                          INNER JOIN
                              par.subacaodetalhe sd
                          ON
                              sd.sbaid = s.sbaid
                          INNER JOIN
                              par.subacaoemendapta sep
                          ON
                              sep.sbdid = sd.sbdid
                          AND sep.sepstatus = 'A'
                          INNER JOIN
                              emenda.v_emendadetalheentidade vede
                          ON
                              vede.emdid = sep.emdid
                          AND vede.edestatus = 'A'
                          WHERE
                              sd.sbdano = '{$_SESSION['exercicio']}'
                          AND edeid = {$dados['edeid']} )";
        $db->executar($sqlDelete);
        $db->commit();

        /*
         * Gravar a seleção oriunda da combo
         */
        $dados['ppsid'] = is_array($dados['ppsid'])
            ?array_filter($dados['ppsid'], 'strlen')
            :[];

        foreach ($dados['ppsid'] as $valor) {
            $retornaIniid = "SELECT
                            iniid
                        FROM
                            par.propostasubacao ps
                        INNER JOIN
                            par.propostasubacaoiniciativaemenda pse
                        ON
                            pse.ppsid = ps.ppsid
                        WHERE
                            ps.ppsid = 1136 limit 1";
            $iniid = $db->pegaUm($retornaIniid);
            $sqlInsert = " INSERT
                    INTO
                        emenda.emendapariniciativa
                        (
                            ppsid,
                            edeid,
                            iniid
                        )
                    SELECT
                        {$valor},
                        {$dados['edeid']},
                        {$iniid}
                    FROM
                        emenda.emendapariniciativa
                    WHERE
                        NOT EXISTS
                        (
                            SELECT
                                epiid
                            FROM
                                emenda.emendapariniciativa
                            WHERE
                                ppsid = {$valor}
                            AND edeid = {$dados['edeid']}
                            AND iniid={$iniid} ) limit 1";

            $db->executar($sqlInsert);
            $db->commit();
        }
        if ($db->commit()) {
            $fm->addMensagem('Requisição realizada com sucesso.');
        } else {
            $fm->addMensagem('Erro ao cadastrar', Simec_Helper_FlashMessage::ERRO);
        }
        cadastrarDocid($informacoes, $infoemenda, $_REQUEST['emeid']);
        header("Location: {$_SERVER['REQUEST_URI']}");
        die();
        break;
    case 'retornar-subacoes-par-ajax':
        global $db;
        $edeid = $_REQUEST['edeid'];
        $mapcod = $_REQUEST['mapcod'];
        switch ($mapcod) {
            case '40':
                $itrid = '2';
                break;
            case '30':
                $itrid = '1,3';
                break;
            default:
                $itrid = '0';
                break;
        }
        /*
         * Formulário de Subações do PAR
         */
        $listaSubacoesPar = "SELECT distinct
                                ps.ppsid as codigo,
                                par.retornacodigopropostasubacao(ps.ppsid)||' - '||ps.ppsdsc as descricao
                        FROM par.propostasubacao ps
                        inner join par.propostasubacaoiniciativaemenda pse on pse.ppsid = ps.ppsid
                        inner join emenda.iniciativadetalheentidade ide on ide.iniid = pse.iniid and ide.idestatus = 'A'
                        inner join par.indicador i on i.indid = ps.indid and i.indstatus = 'A'
                        inner join par.area a on a.areid = i.areid and a.arestatus = 'A'
                        inner join par.dimensao dim on dim.dimid = a.dimid
                        INNER JOIN par.criterio c on c.indid = i.indid
                        INNER JOIN par.propostaacao pa ON pa.crtid = c.crtid
                        WHERE ps.frmid in (14, 15)
                        and dim.itrid in ($itrid)
                        and ide.iniid IN (SELECT
                                            iniid
                                        FROM
                                            emenda.iniciativadetalheentidade
                                        WHERE
                                            edeid ={$edeid})";
        $SqlSubacoesJaCadastradas = "SELECT
                                        ppsid
                                    FROM
                                        emenda.emendapariniciativa
                                    WHERE
                                        edeid = {$edeid}";
        $subacoesJaCadastradas = $db->carregar($SqlSubacoesJaCadastradas);
        if (is_array($subacoesJaCadastradas)) {
            foreach ($subacoesJaCadastradas as $key => $value) {
                $cadastradas[$key] = $value['ppsid'];
            }
        }
        $form = new Simec_View_Form('subacoes-par');
        $htmlForm = $form->setRequisicao('salvar-subacoes-par')
                ->addHidden('edeid', $edeid)
                ->addInputCombo('ppsid', $listaSubacoesPar, $cadastradas, 'ppsid', array('flabel' => 'Subações do PAR', 'multiple' => 'true'))
                ->render();
        echo $htmlForm;
        die;
        break;
}
