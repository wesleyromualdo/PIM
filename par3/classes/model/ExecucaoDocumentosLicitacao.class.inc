<?php

class Par3_Model_ExecucaoDocumentosLicitacao extends Modelo
{

    protected $stNomeTabela = 'par3.execucao_documentolicitacao';

    protected $arChavePrimaria = array(
        'edlid',
    );

    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'proid' => array('tabela' => 'par3.processo', 'pk' => 'proid'),
        'doaid' => array('tabela' => 'par3.tipo_documento_anexo', 'pk' => 'doaid'),
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'usucpfinclusao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    protected $arAtributos = array(
        'edlid' => null,
        'arqid' => null,
        'proid' => null,
        'doaid' => null,
        'docid' => null,
        'edldsc' => null,
        'edlcnpjfornecedor' => null,
        'edlvalordocumento' => null,
        'edldtainclusao' => null,
        'edlstatus' => null,
        'usucpfinclusao' => null,
    );

    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        if ($this->edlcnpjfornecedor) {
            $this->edlcnpjfornecedor = pega_numero($this->edlcnpjfornecedor);
        }

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function recuperarPorArqid($arqid)
    {
        $sql = "SELECT edlid, arqid, edldsc FROM {$this->stNomeTabela} WHERE arqid = {$arqid}";
        return $this->pegaLinha($sql);
    }

    public function recuperarArqid($edlid)
    {
        $sql = "SELECT arqid FROM par3.execucao_documentolicitacao WHERE edlid = {$edlid}";
        return $this->pegaUm($sql);
    }

    public function inserirTabelaAssociativa($edlid, $arValorItens)
    {

        $sql = "DELETE FROM par3.execucao_documentolicitacao_itens WHERE edlid = {$edlid}";
        $this->executar($sql);
        $this->commit();

        $sql = "INSERT INTO par3.execucao_documentolicitacao_itens (aicid, edlid, edivalor) VALUES ";

        foreach ($arValorItens as $item) {
            $sql .= "({$item['aicid']}, {$edlid}, {$item['valoritem']}),";
        }
        $sql = rtrim($sql, ',');
        $this->executar($sql);
        if ($this->commit()) {
            return true;
        } else {
            return false;
        }
    }

    /*public function removerItensDocumento($edlid, $arIpiid){
        $sql = "DELETE FROM par3.execucao_documentolicitacao_itens WHERE edlid = {$edlid} AND ipiid IN({$arIpiid})";
        $this->executar($sql);
        $this->commit();
    }*/

    public function buscaValorItens($itens)
    {
        $sql = "SELECT aicid,
                       aicvaloraprovado as valorItem
                FROM par3.analise_itemcomposicao 
                WHERE aicid IN ({$itens})
                GROUP BY aicid, aicvaloraprovado
                ";

        return $this->carregar($sql);
    }

    public function updateSituacaoItem($edlid, $eadsituacao)
    {
        $sql = "UPDATE par3.execucao_documentolicitacao_itens SET etisituacaoitem = '{$eadsituacao}'
                WHERE ediid in (SELECT ediid FROM par3.execucao_documentolicitacao_itens eti
                                 INNER JOIN par3.execucao_documentolicitacao edl ON edl.edlid = edi.edlid
                                 WHERE edl.edlid = {$edlid})";

        $this->executar($sql);
        return $this->commit();
    }


    public function recuperarDadosAnalise($edlid)
    {
        $sql = "SELECT 
                      eadid,
                      eadparecer, 
                      eadproponente,
                      eadproposta,
                      eadprojeto,
                      eadobjetivo,
                      eadjustificativa,
                      eadvalores,
                      eadcabiveis                      
                FROM par3.execucao_analise_documentolicitacao 
                WHERE edlid = {$edlid} AND eadstatus = 'A'";
        return $this->pegaLinha($sql);
    }

    public function recuperarIniciativasPorDocumentoLicitacao($edlid)
    {
        $sql = "SELECT 
                    (inp.inpid||' - '||ind.inddsc || ' - ' || eta.etadsc)  as descricao, 
                    inp.inpid as id,
                    count(aic.ipiid) 
                     as qtditens,
                    'Itens selecionados: ' as descitem
                FROM par3.execucao_documentolicitacao edl
                INNER JOIN par3.execucao_documentolicitacao_itens edi ON edi.edlid = edl.edlid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.aicid = edi.aicid
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = aic.ipiid
                INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = ipi.inpid
                INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid
                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
                WHERE edl.edlid = {$edlid}
                GROUP BY inp.inpid, inddsc, eta.etadsc
                ORDER BY inp.inpid
         ";

        return $this->carregar($sql);
    }



/*,
                    (SELECT COUNT(*) FROM ( SELECT
                            ipi.ipiid
                        FROM par3.iniciativa_planejamento inp2
                        INNER JOIN par3.v_analise_planejamento_item_composicao ipi ON inp2.inpid = ipi.inpid
                        LEFT JOIN par3.execucao_documentolicitacao_itens edi ON ipi.ipiid = edi.ipiid
                        INNER JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp2.inpid AND ipi.ipiano = ppc.ppcano
                        WHERE
                        ipi.ipistatus = 'A'
                        AND inp2.inpid = inp.inpid
                        AND ipiquantidadeaprovada > 0
                        AND (NOT EXISTS(SELECT t1.ipiid
                                FROM par3.execucao_documentolicitacao_itens t1
                                WHERE t1.ipiid = ipi.ipiid )
                             OR etisituacaoitem = 'R')
                        AND NOT EXISTS (SELECT t1.ipiid
                                FROM par3.execucao_documentolicitacao_itens t1
                                WHERE t1.ipiid = ipi.ipiid AND (etisituacaoitem IS NULL OR etisituacaoitem = 'A'))
                        GROUP BY ipi.ipiid ) a
                                )
                     as qtditens,
                    'Quantidade de itens sem termo de referência: ' as descitem*/
    public function recuperarIniciativas($proid)
    {
        $sql = "SELECT distinct ON(inp.inpid)
                    (inp.inpid||' - '||ind.inddsc || ' - ' || eta.etadsc) as descricao, 
                    inp.inpid as id
                FROM par3.processo pro
                INNER JOIN par3.iniciativa_planejamento inp ON inp.inuid = pro.inuid
                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
                INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON inp.inpid = ipi.inpid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                LEFT JOIN par3.execucao_documentolicitacao_itens edi ON edi.aicid = aic.aicid
                LEFT JOIN par3.execucao_documentolicitacao edl ON edl.edlid = edi.edlid
                WHERE 
                ppcstatus = 'A'
                AND aicqtdaprovado > 0
                AND pro.proid = '{$proid}'
                GROUP BY inp.inpid, inddsc, eta.etadsc
                ORDER BY inp.inpid
         ";

        return $this->carregar($sql);
    }

    public function recuperaDocumentoLicitacao($edlid)
    {
        $sql = "SELECT 
                       edldsc,
                       arq.arqid,
                       arqnome,
                       arqextensao,
                       ediid,
                       doaid,
                       ead.edlid as analise,
                       esd.esdid,
                       edl.docid,
                       edlcnpjfornecedor,
                       edlvalordocumento
                FROM par3.execucao_documentolicitacao edl
                LEFT JOIN par3.execucao_documentolicitacao_itens edi ON edl.edlid = edi.edlid
                LEFT JOIN par3.execucao_analise_documentolicitacao ead ON edl.edlid = ead.edlid
                LEFT JOIN workflow.documento doc ON doc.docid = edl.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN arquivo arq ON arq.arqid = edl.arqid
                WHERE edl.edlid = {$edlid}";

        return $this->pegaLinha($sql);
    }
}
