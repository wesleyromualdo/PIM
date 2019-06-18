<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Emenda.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/EmendaDetalhe.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Emendadetalheentidade.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Emendadetalheimpositivo.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Entidadebeneficiada.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Emendadetalheptres.php";
include_once APPRAIZ . "spoemendas/classes/model/Emendaimpedimentobeneficiario.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Autor.class.inc";
include_once APPRAIZ . "monitora/classes/model/Acao.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

if ($_POST['acao'] == 'carregar') {

    $exercicio = $_POST['ano'];

    if (empty($exercicio)) {
        echo <<<HTML
            <script >
                alert('Ano não informado.');
                window.location.href = 'carregaCargaEmenda_v2.php';
            </script>
HTML;
    }


// Carrega os dados da carga
    $sql = <<<SQL
SELECT cesid,
       esfera,
       subfuncao,
       funcao,
       lpad(COALESCE(acao, '0'), 4 , '0') AS acao,
       lpad(localizador, 4 , '0') AS localizador,
       programa,
       uo,
       emenda,
       ano_emenda,
       autcod,
       planoorcamentario,
       rp_atual,
       gnd,
       fonte,
       mapcod,
       dotacaoatualizada,
       cnpj,
       nome_beneficiario,
       uf,
       valor_rcl_apurada,
       valor_disponivel,
       prioridade,
       valorimpedido,
       ptres,
       limiteempenhado
  FROM emenda.carga_emenda_siop
  WHERE ano_emenda = '{$exercicio}'
  ORDER BY emenda, ano_emenda, gnd, mapcod, cnpj DESC
SQL;

    $arrCarga = $db->carregar($sql);
    $arrCarga = $arrCarga ? $arrCarga : [];

    function atualizarEmenda($emeid, $acaid, $ploid)
    {
        // -- Atualizando o acaid da emenda
        $emenda = new Spoemendas_Model_Emenda($emeid);
        $emenda->acaid = $acaid;
        $emenda->ploid = $ploid;

        $emenda->salvar([
            'emeano',
            'emevalor',
            'emecod',
            'autid',
            'acaid',
            'resid',
            'emetipo',
            'emeano',
            'emedescentraliza',
            'emelibera',
            'emevalor',
            'etoid',
            'emerelator',
            'ploid'
        ]);
    }

    function atualizarBeneficiarios($emeid, $beneficiarios)
    {
        $benefGMFCarga = [];

        // -- Não posso tirar isso? É só agrupar nesse formato no objeto primário
        foreach ($beneficiarios as $beneficiario) {
            foreach ($beneficiario as $dadosBeneficiario) {
                $benefGMFCarga[] = $dadosBeneficiario;
            }
        }

        $benefGMFBanco = (new Spoemendas_Model_Emenda())
            ->carregarBeneficiariosPorEmeid($emeid, null);

        $benefCMFSemCorrespondencia = [];
        $modelEntidade = new Spoemendas_Model_Entidadebeneficiada();

        // -- Atualizando todos que estão iguais
        foreach ($benefGMFCarga as &$benefGMF) {
            if (CNPJComfonteModalidadeGNDSemAlteracao($benefGMFBanco, $benefGMF)) {
                atualizarEmendaDetalheEntidade($benefGMF['edeid'], $benefGMF['emdid'], $benefGMF);
            } elseif ((1 == count($benefGMFCarga))
                && (1 == count($benefGMFBanco))
                && ($benefGMFBanco[0]['enbcnpj'] == $benefGMF['cnpj'])
            ) { // -- um beneficiario mudando de um conjunto pra outro
                atualizarEmendaDetalhe($benefGMFBanco[0]['emdid'], $benefGMF);
                atualizarEmendaDetalheEntidade($benefGMFBanco[0]['edeid'], $benefGMFBanco[0]['emdid'], $benefGMF);
            } else { // -- beneficiarios diferentes ou com multiplos beneficiarios sem correspondencia direta
                $enbid = $modelEntidade
                    ->pegaUm("SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj = '{$benefGMF['cnpj']}' AND enbstatus = 'A' ORDER BY enbano DESC");

                if (empty($enbid)) {
                    $enbid = salvarEntidadeBeneficiada($benefGMF);
                }

                $emdid = salvarEmendaDetalhe($emeid, $benefGMF);
                $edeid = salvarEmendaDetalheEntidade($emdid, $enbid, $benefGMF);
            }

            // -- Atualiza o registro que já foi processado
            atualizarCarga($benefGMF['cesid']);
        }
    }

    function preparaEmDetEntidade($emecod = null)
    {
        global $db, $exercicio;

        $whereAnd = '';
        if (!empty($emecod)) {
            $whereAnd = " AND emecod = '{$emecod}'";
        }

        $sql = <<<SQL
UPDATE emenda.emendadetalheentidade SET
      edestatus = 'I'
  WHERE edeid in (SELECT edeid
                    FROM emenda.emendadetalheentidade ede
                      INNER JOIN emenda.emendadetalhe emd USING (emdid)
                      INNER JOIN emenda.emenda eme USING (emeid)
                    WHERE emeano = '{$exercicio}'
                      AND ede.edestatus = 'A'
                      AND emd.emdimpositiva IN (6, 7, 2) {$whereAnd})
SQL;

        $db->executar($sql);
    }

    function CNPJComfonteModalidadeGNDSemAlteracao(&$banco, &$carga)
    {
        foreach ($banco as &$_banco) {
            if (($_banco['enbcnpj'] == $carga['cnpj'])
                && ($_banco['gndcod'] == $carga['gnd'])
                && ($_banco['foncod'] == $carga['fonte'])
                && ($_banco['mapcod'] == $carga['mapcod'])
            ) {
                $_banco['temcorrespondencia'] = true;
                $carga['edeid'] = $_banco['edeid'];
                $carga['emdid'] = $_banco['emdid'];
                return true;
            }
        }
        return false;
    }

    function atualizarEmendaDetalheEntidade($edeid, $emdid, array $dados)
    {
        $modelEmDetEntidade = new Spoemendas_Model_Emendadetalheentidade($edeid);

        $modelEmDetEntidade->edevalor = $dados['valor_rcl_apurada'];
        $modelEmDetEntidade->edeprioridade = $dados['prioridade'];
        $modelEmDetEntidade->edelimiteempenhobeneficiario = $dados['limiteempenhado'];
        $modelEmDetEntidade->edestatus = 'A';
        $modelEmDetEntidade->usucpfalteracao = $_SESSION['usucpf'];
        $modelEmDetEntidade->ededataalteracao = date('Y-m-d H:i:s');
        $edeid = $modelEmDetEntidade->salvar();

        // -- Atualizando impedimentos da emenda
        (new Spoemendas_Model_Emendadetalheimpositivo())
            ->atualizarImpedimentoByEdeid($edeid, $emdid, $dados);

        return $edeid;
    }

    function salvarEmendaDetalhe($emeid, array $dados)
    {
        $modelEmDetalhe = new Spoemendas_Model_EmendaDetalhe();

        $dadosEmDetalhe = $modelEmDetalhe
            ->carregarPorEmeidEComposicao($emeid, $dados['gnd'], $dados['fonte'], $dados['mapcod']);

        if (!empty($dadosEmDetalhe)) {
            return $dadosEmDetalhe['emdid'];
        }

        $modelEmDetalhe->emeid = $emeid;
        $modelEmDetalhe->gndcod = $dados['gnd'];
        $modelEmDetalhe->foncod = $dados['fonte'];
        $modelEmDetalhe->mapcod = $dados['mapcod'];
        $modelEmDetalhe->emdtipo = 'E';
        $modelEmDetalhe->emdimpositiva = $dados['rp_atual'];
        $modelEmDetalhe->emdvalor = 0;
        $modelEmDetalhe->usucpfalteracao = $_SESSION['usucpf'];
        $modelEmDetalhe->emddataalteracao = date('Y-m-d H:i:s');

        return $modelEmDetalhe->salvar();
    }

    function salvarEmendaDetalheEntidade($emdid, $enbid, array $dados)
    {
        $modelEmDetEntidade = new Spoemendas_Model_Emendadetalheentidade();
        $modelEmDetEntidade->emdid = $emdid;
        $modelEmDetEntidade->enbid = $enbid;

        $modelEmDetEntidade->edevalor = $dados['valor_rcl_apurada'];
        $modelEmDetEntidade->edeprioridade = $dados['prioridade'];
        $modelEmDetEntidade->edelimiteempenhobeneficiario = $dados['limiteempenhado'];
        $modelEmDetEntidade->edestatus = 'A';
        $modelEmDetEntidade->usucpfalteracao = $_SESSION['usucpf'];
        $modelEmDetEntidade->ededataalteracao = date('Y-m-d H:i:s');

        $edeid = $modelEmDetEntidade->salvar();

        // -- Atualizando impedimentos da emenda
        (new Spoemendas_Model_Emendadetalheimpositivo())
            ->atualizarImpedimentoByEdeid($edeid, $emdid, $dados);

        return $edeid;
    }

    function salvarEntidadeBeneficiada(array $dados)
    {
        global $exercicio;

        $enb = new Spoemendas_Model_Entidadebeneficiada();
        $enb->enbcnpj = $dados['cnpj'];
        $enb->enbano = $exercicio;
        $enb->enbnome = $dados['nome_beneficiario'];

        return $enb->salvar();
    }

    function atualizarEmendaDetalhe($emdid, array $dados)
    {
        $modelEmDet = new Spoemendas_Model_EmendaDetalhe($emdid);
        $modelEmDet->gndcod = $dados['gnd'];
        $modelEmDet->foncod = $dados['fonte'];
        $modelEmDet->mapcod = $dados['mapcod'];
        $modelEmDet->emdstatus = 'A';

        $modelEmDet->salvar();
    }

    function atualizarCarga($cesid)
    {
        global $db;

        $db->executar("UPDATE emenda.carga_emenda_siop SET cesprocessado = true WHERE cesid = {$cesid}");
//    $db->commit();
    }

    function finalizarEmenda($emeid, array $dados)
    {
        global $db;

        // -- Atualizando valor do detalhe
        foreach ($dados as $conjunto => $drp) {
            list($gndcod, $foncod, $mapcod) = explode('-', $conjunto);

            $emdvalor = empty($drp['dotacaoatualizada']) ? '0' : $drp['dotacaoatualizada'];

            $sql = <<<DML
UPDATE emenda.emendadetalhe
  SET emdvalor = {$emdvalor}
  WHERE emeid = {$emeid}
    AND gndcod = '{$gndcod}'
    AND foncod = '{$foncod}'
    AND mapcod = '{$mapcod}'
  RETURNING emdid
DML;
            if (!$db->pegaUm($sql)) {
                $emendaDet = new Spoemendas_Model_EmendaDetalhe();
                $emendaDet->emeid = $emeid;
                $emendaDet->gndcod = $gndcod;
                $emendaDet->foncod = $foncod;
                $emendaDet->mapcod = $mapcod;
                $emendaDet->emdvalor = $emdvalor;
                $emendaDet->emdimpositiva = $drp['rp_atual'];
                $emendaDet->emdtipo = 'E';
                $emendaDet->salvar();

            }
        }

        // -- Desativando detalhes sem beneficiarios ativos
        $sql = <<<DML
UPDATE emenda.emendadetalhe
  SET emdstatus = CASE WHEN (SELECT COUNT(1)
                               FROM emenda.emendadetalheentidade ede
                               WHERE ede.emdid = emendadetalhe.emdid
                                 AND ede.edestatus = 'A') = 0 THEN 'I' ELSE 'A' END
  WHERE emeid = {$emeid}
    AND NOT EXISTS (SELECT 1
                      FROM emenda.emenda eme
                        INNER JOIN emenda.carga_emenda_siop ces ON eme.emecod = ces.emenda
                      WHERE eme.emeid = {$emeid}
                        AND emendadetalhe.gndcod = ces.gnd
                        AND emendadetalhe.mapcod = ces.mapcod
                        AND emendadetalhe.foncod = ces.fonte)
DML;
        $db->executar($sql);

        // -- Atualizando o valor da dotacao da emenda com base nos detalhamentos
        $sql = <<<DML
UPDATE emenda.emenda
  SET emevalor = (SELECT SUM(emd.emdvalor)
                    FROM emenda.emendadetalhe emd
                    WHERE emenda.emeid = emd.emeid
                      AND emd.emdstatus = 'A')
  WHERE emeid = {$emeid};
DML;
        $db->executar($sql);
    }

    function pegaPlanoOrcamentario($plocodigo, $acaid) {
        global $db;
        $sql = <<<SQL
            select
                ploid
            from monitora.planoorcamentario 
            where acaid = %d 
            and plocodigo = '%s';
SQL;
        return $db->pegaUm(sprintf($sql, $acaid, $plocodigo), 'ploid');
    }

    $listaEmendas = [];
    foreach ($arrCarga as $linha) {

        if (array_key_exists($linha['emenda'], $listaEmendas)) {
            if (!empty($linha['cnpj'])) {
                $listaEmendas[$linha['emenda']]['beneficiarios'][$linha['cnpj']]["{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}"]
                    = [
                    'cesid' => $linha['cesid'],
                    'nome_beneficiario' => str_replace("{$linha['cnpj']} - ", '', $linha['nome_beneficiario']),
                    'gnd' => $linha['gnd'],
                    'fonte' => $linha['fonte'],
                    'mapcod' => $linha['mapcod'],
                    'uf' => $linha['uf'],
                    'valor_rcl_apurada' => $linha['valor_rcl_apurada'],
                    'cnpj' => $linha['cnpj'],
                    'valor_disponivel' => $linha['valor_disponivel'],
                    'prioridade' => $linha['prioridade'],
                    'valorimpedido' => $linha['valorimpedido'],
                    'limiteempenhado' => $linha['limiteempenhado'],
                    'rp_atual' => $linha['rp_atual'],
                ];
            } else {
                if (array_key_exists("{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}", $listaEmendas[$linha['emenda']]['detalhes'])) {
                    $listaEmendas[$linha['emenda']]['detalhes']["{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}"]['dotacaoatualizada'] += $linha['dotacaoatualizada'];
                } else {
                    $listaEmendas[$linha['emenda']]['detalhes']["{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}"]
                        = [
                        'dotacaoatualizada' => $linha['dotacaoatualizada'],
                        'rp_atual' => $linha['rp_atual']
                    ];
                }
            }
        } elseif (empty($linha['cnpj'])) {
            $listaEmendas[$linha['emenda']] = [
                'cesid' => $linha['cesid'],
                'acao' => $linha['acao'],
                'localizador' => $linha['localizador'],
                'programa' => $linha['programa'],
                'uo' => $linha['uo'],
                'ptres' => $linha['ptres'],
                'planoorcamentario' => $linha['planoorcamentario'],
                'beneficiarios' => [],
                'detalhes' => [
                    "{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}" => [
                        'dotacaoatualizada' => $linha['dotacaoatualizada'],
                        'rp_atual' => $linha['rp_atual']
                    ]
                ]
            ];
        }
    }
//ver($listaEmendas);

    $modelEmendas = new Spoemendas_Model_Emenda();
    $modelAcao = new Monitora_Model_Acao();

    foreach ($listaEmendas as $emecod => $emenda) {
        // -- Procurando o ID da emenda
        $emeid = $modelEmendas
            ->carregarPorCodigo($emecod, $exercicio);

        if (empty($emeid)) {
            $autcod = substr($emecod, 0, 4);
            $autid = (new Spoemendas_Model_Autor())->carregarPorAutcod($autcod);
            $eme = new Spoemendas_Model_Emenda();
            $eme->emecod = $emecod;
            $eme->autid = $autid;
            $eme->emeano = $exercicio;
            $emeid = $eme->salvar();
        }

        // -- Procurando o ID da ação
        $acaid = $modelAcao->carregarPorProgramatica(
            $emenda['uo'],
            $emenda['programa'],
            $emenda['acao'],
            $emenda['localizador'],
            $exercicio
        );

        $ploid = pegaPlanoOrcamentario($emenda['planoorcamentario'], $acaid);

        if (empty($acaid)) {
            echo "Ação não encontrada({$emenda['emenda']} - {$emenda['uo']}.{$emenda['programa']}.{$emenda['acao']}.{$emenda['localizador']}'/{$emenda['ano_emenda']});\n";
            ver($emecod, $emenda);
            continue;
        }

        // -- Inativando emendadetalheentidade
        preparaEmDetEntidade($emecod);

        atualizarEmenda($emeid, $acaid, $ploid);

        atualizarBeneficiarios($emeid, $emenda['beneficiarios']);

        finalizarEmenda($emeid, $emenda['detalhes']);
    }
//ver('FIM :D',d);
    $db->commit();
    exit;
}

?>

<form action="" method="POST">
    <input type="hidden" name="acao" value="carregar">
    <input type="text" name="ano" maxlength="4" title="Digite o ano exercício">
    <input type="submit" value="Carregar">
</form>
