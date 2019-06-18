<?php

include_once APPRAIZ . 'spoemendas/classes/model/Carga_emenda_siop.php';

class Spo_Job_Emenda_DadosEmenda extends Simec_Job_Abstract
{
    /**
     * @var array
     */
    private $params;

    /**
     * Retorna o label dada para este Job
     *
     * @return string
     */
    public function getName()
    {
        return 'Processando dados das emendas';
    }

    /**
     * @throws Exception
     */
    protected function init()
    {
        // merge parametros recebidos com parametros padrões
        $this->params = $this->loadParams();
    }

    protected function echoOutput($mensagem){
        echo $mensagem."<br>";
        $this->salvarOutput();
    }

    /**
     * @throws Exception
     */
    protected function execute()
    {
        try {
            $this->echoOutput('Processamento Iniciado');

            $this->echoOutput('Preparando informações');
            $listaEmendas = $this->organizaDadosCarga();
            $exercicio = $this->params['exercicio'];

            $modelEmendas = new Spoemendas_Model_Emenda();
            $modelAcao = new Monitora_Model_Acao();

            $this->echoOutput('Processando informações');
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

                $ploid = $this->pegaPlanoOrcamentario($emenda['planoorcamentario'], $acaid);

                if (empty($acaid)) {
                    $this->echoOutput("Ação não encontrada({$emenda['emenda']} - {$emenda['uo']}.{$emenda['programa']}.{$emenda['acao']}.{$emenda['localizador']}'/{$emenda['ano_emenda']});");
                    continue;
                }

                // -- Inativando emendadetalheentidade
                $this->preparaEmDetEntidade($emecod);

                $this->atualizarEmenda($emeid, $acaid, $ploid);

                $this->atualizarBeneficiarios($emeid, $emenda['beneficiarios']);

                $this->finalizarEmenda($emeid, $emenda['detalhes'], $emecod);

                $this->atualizaDadosPar($emenda['ano_emenda']);
            }

            $this->commit();
        } catch (Exception $e) {
            throw $e;
        }
    }

    private function atualizarEmenda($emeid, $acaid, $ploid)
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

    private function atualizarBeneficiarios($emeid, $beneficiarios)
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
            if ($this->CNPJComfonteModalidadeGNDSemAlteracao($benefGMFBanco, $benefGMF)) {
                $this->atualizarEmendaDetalheEntidade($benefGMF['edeid'], $benefGMF['emdid'], $benefGMF);
            } elseif ((1 == count($benefGMFCarga))
                && (1 == count($benefGMFBanco))
                && ($benefGMFBanco[0]['enbcnpj'] == $benefGMF['cnpj'])
            ) { // -- um beneficiario mudando de um conjunto pra outro
                $this->atualizarEmendaDetalhe($benefGMFBanco[0]['emdid'], $benefGMF);
                $this->atualizarEmendaDetalheEntidade($benefGMFBanco[0]['edeid'], $benefGMFBanco[0]['emdid'], $benefGMF);
                // -- Atualiza o registro que já foi processado
                $this->atualizarCarga($benefGMF['cesid']);
            } else { // -- beneficiarios diferentes ou com multiplos beneficiarios sem correspondencia direta
                $enbid = $modelEntidade
                    ->pegaUm("SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj = '{$benefGMF['cnpj']}' AND enbstatus = 'A' ORDER BY enbano DESC");

                if (empty($enbid)) {
                    $enbid = $this->salvarEntidadeBeneficiada($benefGMF);
                }

                $emdid = $this->salvarEmendaDetalhe($emeid, $benefGMF);
                $edeid = $this->salvarEmendaDetalheEntidade($emdid, $enbid, $benefGMF);

                // -- Atualiza o registro que já foi processado
                $this->atualizarCarga($benefGMF['cesid']);
            }

        }
    }

    private function preparaEmDetEntidade($emecod = null)
    {
        $exercicio = $this->params['exercicio'];

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

        $this->executar($sql);
    }

    private function CNPJComfonteModalidadeGNDSemAlteracao(&$banco, &$carga)
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

    private function atualizarEmendaDetalheEntidade($edeid, $emdid, array $dados)
    {
        $modelEmDetEntidade = new Spoemendas_Model_Emendadetalheentidade($edeid);

        if ($dados['valor_rcl_apurada'] != $modelEmDetEntidade->edevalor
            || $modelEmDetEntidade->edeprioridade != $dados['prioridade']
            || $modelEmDetEntidade->edelimiteempenhobeneficiario != $dados['limiteempenhado']
            || $modelEmDetEntidade->usucpfalteracao != $_SESSION['usucpf']) {
            // -- Atualiza o registro que já foi processado
            $this->atualizarCarga($dados['cesid']);
        }

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

    private function salvarEmendaDetalhe($emeid, array $dados)
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

    private function salvarEmendaDetalheEntidade($emdid, $enbid, array $dados)
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

    private function salvarEntidadeBeneficiada(array $dados)
    {
        $exercicio = $this->params['exercicio'];

        $enb = new Spoemendas_Model_Entidadebeneficiada();
        $enb->enbcnpj = $dados['cnpj'];
        $enb->enbano = $exercicio;
        $enb->enbnome = $dados['nome_beneficiario'];

        return $enb->salvar();
    }

    private function atualizarEmendaDetalhe($emdid, array $dados)
    {
        $modelEmDet = new Spoemendas_Model_EmendaDetalhe($emdid);
        $modelEmDet->gndcod = $dados['gnd'];
        $modelEmDet->foncod = $dados['fonte'];
        $modelEmDet->mapcod = $dados['mapcod'];
        $modelEmDet->emdstatus = 'A';

        $modelEmDet->salvar();
    }

    private function atualizarCarga($cesid)
    {
        $this->executar("UPDATE emenda.carga_emenda_siop SET cesprocessado = true WHERE cesid = {$cesid}");
    }

    private function finalizarEmenda($emeid, array $dados, $emecod)
    {
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
            if (!$this->pegaUm($sql)) {
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
        $this->executar($sql);
        $this->commit();

        // -- Atualizando o valor da dotacao da emenda com base nos detalhamentos
        $sql = <<<DML
UPDATE emenda.emenda
  SET emevalor = (SELECT SUM(emd.emdvalor)
                    FROM emenda.emendadetalhe emd
                    WHERE emenda.emeid = emd.emeid
                      AND emd.emdstatus = 'A')
  WHERE emeid = {$emeid};
DML;
        $this->executar($sql);
        $this->echoOutput("Dados da emenda ".$emecod." carregados no sistema");
    }

    private function pegaPlanoOrcamentario($plocodigo, $acaid) {
        $sql = <<<SQL
            select
                ploid
            from monitora.planoorcamentario 
            where acaid = %d 
            and plocodigo = '%s';
SQL;
        return $this->pegaUm(sprintf($sql, $acaid, $plocodigo), 'ploid');
    }

    private function organizaDadosCarga()
    {
        $arrCarga = $this->carregaDadosCarga();
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
                        'ano_emenda' => $linha['ano_emenda']
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
                    'ano_emenda' => $linha['ano_emenda'],
                    'detalhes' => [
                        "{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}" => [
                            'dotacaoatualizada' => $linha['dotacaoatualizada'],
                            'rp_atual' => $linha['rp_atual']
                        ]
                    ]
                ];
            }
        }
        return $listaEmendas;
    }

    private function carregaDadosCarga()
    {
        try {
            return (new Emenda_Model_Carga_emenda_siop())
                ->recuperarTodos(
                    'cesid,
                    esfera,
                    subfuncao,
                    funcao,
                    lpad(COALESCE(acao, \'0\'), 4 , \'0\') AS acao,
                    lpad(localizador, 4 , \'0\') AS localizador,
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
                    limiteempenhado', ['ano_emenda = \''.$this->params['exercicio'].'\'']);
        } catch (Exception $e) {
            throw $e;
        }
    }

    // -- Atualização das informaçoes no PAR

    private function atualizaDadosPar($emeano){
        $iniciativas = $this->pegarDadosIniciativaEmenda($emeano);

        foreach ($iniciativas as $iniciativa) {
            $edeidnovo = $this->pegarEdeidNovo($iniciativa['emeid'], $iniciativa['enbid'], $iniciativa['inevalor']);

            if ($edeidnovo != $iniciativa['edeid'] && !empty($edeidnovo)) {
                if (!$this->atualizaIniciativaEmendaPar($iniciativa['emeid'], $iniciativa['edeid'], $edeidnovo)) {
                    throw new Exception('Erro ao alterar a iniciativa da emenda no PAR');
                }

                $analises = $this->pegarDadosAnalisePar($iniciativa['edeid']);
                foreach ($analises as $analise) {
                    if (!$this->atualizaAnalisePar($edeidnovo, $analise['anaid'])) {
                        throw new Exception('Erro ao alterar a análise do PAR');
                    }
                }
            }
        }
    }

    private function atualizaAnalisePar($edeidnovo, $anaid){
        $sql = <<<SQL
			UPDATE par3.analise SET
				edeid = %d
			WHERE
				anaid = %d;
SQL;
        $sql = sprintf($sql, $edeidnovo, $anaid);
        return $this->executar($sql);
    }

    private function pegarDadosAnalisePar($edeid){
        $sql = <<<SQL
            SELECT
                anaid
            FROM
                par3.analise
            WHERE
                edeid = %d
                AND anastatus = 'A';
SQL;
        $sql = sprintf($sql, $edeid);
        return $this->carregar($sql);
    }

    private function atualizaIniciativaEmendaPar($emeid, $edeid, $edeidnovo) {
        $sql = <<<SQL
            UPDATE par3.iniciativa_emenda SET
                edeid = %d
            WHERE
                emeid = %d
                AND edeid = %d
                AND inestatus = 'A'
                and 0 = (SELECT count(edeid) FROM emenda.emendadetalheentidade WHERE edeid = %d AND edestatus = 'A');
SQL;
        $sql = sprintf($sql, $edeidnovo, $edeid, $emeid, $edeid, $edeid);

        return $this->executar($sql);
    }

    private function pegarEdeidNovo($emeid, $enbid, $inevalor){
        $sql = <<<SQL
            SELECT
                ede.edeid
            FROM emenda.emenda eme
            INNER JOIN emenda.emendadetalhe emd USING(emeid)
            INNER JOIN emenda.emendadetalheentidade ede USING(emdid)
            WHERE
                eme.emeid = %d
                AND ede.enbid = %d
                AND ede.edestatus = 'A'
SQL;
        $sql = sprintf($sql, $emeid, $enbid);

        return $this->pegaUm($sql);
    }

    private function pegarDadosIniciativaEmenda($emeano){
        $sql = <<<SQL
            SELECT
                ine.ineid,
                ine.inevalor,
                eme.emeid,
                ede.edeid,
                ede.enbid
            FROM par3.iniciativa_emenda ine
            INNER JOIN emenda.emenda eme USING(emeid)
            INNER JOIN emenda.emendadetalheentidade ede USING(edeid)
            WHERE
                ine.inestatus = 'A'
                AND eme.emeano = '%s'
                AND EXISTS(
                    SELECT
                        1
                    FROM
                        emenda.carga_emenda_siop ces
                    WHERE
                        ces.emenda = eme.emecod
                        AND ces.ano_emenda::TEXT = eme.emeano::TEXT
                )
            ORDER BY
                ine.emeid;
SQL;
        $sql = sprintf($sql, $emeano);

        return $this->carregar($sql);
    }

    // -- FIM Atualização das informaçoes no PAR

    protected function shutdown()
    {
    }
}