<?php

class Par3_Model_ExecucaoNotaFiscal extends Modelo
{
    protected $stNomeTabela = 'par3.execucao_notafiscal';

    protected $arChavePrimaria = [
        'ntfid'
    ];

    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'proid' => array('tabela' => 'par3.processo', 'pk' => 'proid'),
        'ntfcpfalteracao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'ntfcpfinativacao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    protected $arAtributos = [
        'ntfid' => null,
        'proid' => null,
        'ntfstatus' => null,
        'ntfdescricao' => null,
        'ntfdata' => null,
        'ntfnumeronotafiscal' => null,
        'usucpf' => null,
        'arqid' => null,
        'ntfvlrnota' => null,
        'ntfqtditem' => null,
        'ntfcnpjfornecedor' => null,
        'ntfrazaosocialfornecedor' => null,
        'ntfuffornecedor' => null,
        'ntfdtalteracao' => null,
        'ntfcpfalteracao' => null,
        'ntfdtinativacao' => null,
        'ntfcpfinativacao' => null,
    ];

    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        if ($this->ntfcnpjfornecedor) {
            $this->ntfcnpjfornecedor = trim($this->ntfcnpjfornecedor);
        }

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function sqlListaNotasFiscais($proid)
    {
        $sql = "
            SELECT 
              ntf.ntfid,
              ntf.arqid,
              ntfnumeronotafiscal,
              ntf.ntfcnpjfornecedor,
              ntf.ntfrazaosocialfornecedor,
              TO_CHAR(ntfdata, 'DD/MM/YYYY') AS ntfdata,
              ntf.ntfqtditem,
              ntf.ntfvlrnota,
              CASE WHEN ntfstatus = 'A'
                THEN '<span class=\"success\"><b>Ativo</b></span>'
                ELSE '<span class=\"danger\"><b>Inativo</b></span>'
              END AS status,
              ntfstatus
            FROM
              par3.execucao_notafiscal ntf
              INNER JOIn par3.processo pro ON pro.proid = ntf.proid
            WHERE
              ntf.proid = '{$proid}'
            ORDER BY ntfstatus, ntf.ntfid
        ";

        return $sql;
    }

    public function sqlEditNotaFiscal($ntfid)
    {
        $sql = "
            SELECT 
                ntfid,
                ntfnumeronotafiscal,
                ntfvlrnota,
                ntfqtditem,
                to_char(ntfdata, 'dd/mm/YYYY') as ntfdata,
                ntfcnpjfornecedor,
                ntfrazaosocialfornecedor,
                ntfuffornecedor,
                arqextensao,
                arqnome,
                arq.arqid,
                (SELECT coalesce(SUM(encvaloritens),0) FROM par3.execucao_notafiscal_contratoitens WHERE ntfid = {$ntfid}) as valortotalitens,
                (SELECT coalesce(COUNT(encqtditens),0) FROM par3.execucao_notafiscal_contratoitens WHERE ntfid = {$ntfid}) as qtdtotalitens,
                ntfstatus,
                proid
            FROM 
              par3.execucao_notafiscal ntf
              INNER JOIN arquivo arq ON ntf.arqid = arq.arqid
            WHERE 
              ntfid = {$ntfid}
        ";
        return $sql;
    }

    public function salvarNotaFiscalContratoItens($ntfid, array $contratoItens)
    {
        if (!empty($contratoItens)) {
            foreach ($contratoItens as $contratoItem) {
                $sql = sprintf("
                    INSERT INTO par3.execucao_notafiscal_contratoitens (eciid, ntfid, encqtditens, encvaloritens) 
                    VALUES (%d, %d, %d, %d);
                ", $contratoItem['eciid'], $ntfid, $contratoItem['encqtditens'], $contratoItem['encvaloritens']);
                $this->executar($sql);
            }
        }

        if ($this->commit()) {
            return true;
        } else {
            return false;
        }
    }

    public function salvarItemNf($ntfid, $eciid, $encqtditens, $encvaloritens, $encid = null)
    {
        if ($encid != null && $encid != '') {
            //se valor igual a zero, remove o item do contrato
            if ($encqtditens == 0) {
                $sql = "DELETE FROM par3.execucao_notafiscal_contratoitens WHERE encid = {$encid}";
            } else {
                $sql = "UPDATE par3.execucao_notafiscal_contratoitens set eciid={$eciid}, ntfid={$ntfid}, encqtditens={$encqtditens}, encvaloritens={$encvaloritens}
                    WHERE encid = {$encid} RETURNING encid";
            }
        } else {
            $sql = "INSERT INTO par3.execucao_notafiscal_contratoitens (eciid, ntfid, encqtditens, encvaloritens)
                VALUES ({$eciid}, {$ntfid}, {$encqtditens}, {$encvaloritens}) RETURNING encid";
        }

        $id = $this->pegaUm($sql);
        if ($this->commit()) {
            return $id;
        }
        return false;
    }

    public function salvarQuantidadeTotal($ntfid, $total)
    {
        $sql = "UPDATE par3.execucao_notafiscal SET ntfqtditem = {$total} WHERE ntfid = {$ntfid}";
        $this->executar($sql);
        $this->commit();
    }

    public function inserirArrayItem($arrItem)
    {
        $existeUpdate = false;
        $existeInsert = false;

        $sqlInsert = "INSERT INTO par3.execucao_notafiscal_contratoitens (eciid, ntfid, encvaloritens, encqtditens) VALUES ";
        $sqlUpdate = "UPDATE par3.execucao_notafiscal_contratoitens as enc 
                            SET encqtditens = enc2.encqtditens, encvaloritens = enc2.encvaloritens
                            FROM (values";
        foreach ($arrItem as $item) {
            if ($item['encid']) {
                $sqlUpdate .= "({$item['encqtditens']}, {$item['encvaloritens']}, {$item['encid']}),";
                $existeUpdate = true;
            } else {
                $sqlInsert .= "({$item['eciid']}, {$item['ntfid']}, {$item['encvaloritens']}, {$item['encqtditens']}),";
                $existeInsert = true;
            }
        }
        if ($existeUpdate) {
            $sqlUpdate = rtrim($sqlUpdate, ',');
            $sqlUpdate .= ") as enc2(encqtditens, encvaloritens, encid) WHERE enc2.encid = enc.encid";
            $this->executar($sqlUpdate);
        }

        if ($existeInsert) {
            $sqlInsert = rtrim($sqlInsert, ',');
            $this->executar($sqlInsert);
        }

        if ($existeInsert || $existeUpdate) {
            if ($this->commit()) {
                return true;
            }
        }
        return false;
    }

    /** busca valor total da nota fiscal, e o tipo de item vem junto para economizar query. */
    public function calcularValorTotalNf($ntfid)
    {
        $sql = "SELECT 
                  sum(encvaloritens) as valortotal,
                  ito.intoid as tipoitem,
                  intodsc
                FROM par3.execucao_notafiscal_contratoitens enc
                INNER JOIN par3.execucao_contratoitens eci ON eci.eciid = enc.eciid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.aicid = eci.aicid
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = aic.ipiid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.iniciativa ini ON ini.iniid = iig.iniid
                INNER JOIN par3.iniciativa_tipos_objeto ito ON ito.intoid = ini.intoid
                WHERE ntfid = {$ntfid}
                GROUP BY ito.intoid";
        return $this->pegaLinha($sql);
    }

    public function recuperarEstadoMunicipio($inuid)
    {
        $sql = "SELECT est.estuf, mun.muncod, mundescricao
                FROM par3.instrumentounidade inu
                LEFT JOIN territorios.municipio mun ON mun.muncod = inu.muncod
                LEFT JOIN territorios.estado est ON est.estuf = inu.estuf
                WHERE inuid = {$inuid}
                ";

        return $this->pegaLinha($sql);
    }

    public function buscaItensDetalhados($encid, $offset = 0)
    {
        $sql = "SELECT edvid,
                       edvchassi,
                       edvrenavam,
                       edvplaca,
                       CASE WHEN edvvistoriainmetro = 'f' THEN 'Não' ELSE 'Sim' END as edvvistoriainmetro,
                       mundescricao,
                       arqnome,
                       arqextensao,
                       arq.arqid,
                       mun.muncod,
                       encqtditens
                FROM par3.execucao_notafiscal_contratoitens enc
                LEFT JOIN par3.execucao_detalhamento_veiculo edv ON edv.encid = enc.encid
                LEFT JOIN territorios.municipio mun ON mun.muncod = edv.muncod
                LEFT JOIN public.arquivo arq ON arq.arqid = edv.arqid
                WHERE enc.encid = {$encid}
                AND edvstatus = 'A'
                ORDER BY edvid 
                OFFSET {$offset}
                LIMIT 5
                ";

        return $this->carregar($sql);
    }

    public function contagemItensDetalhados($encid)
    {
        $sql = "SELECT coalesce(count(edvid),0)
                FROM par3.execucao_detalhamento_veiculo edv
                WHERE encid = {$encid}
                AND edvstatus = 'A'";

        return $this->pegaUm($sql);
    }

    /**
     * Recupera item a ser detalhado,
     * Deve trazer a quantidade total e o tipo de item (veículo/embarcação)
     * @param $encid
     */
    public function recuperarItem($encid)
    {
        $sql = "SELECT 
                      encqtditens as qtdtotal,
                      count(CASE WHEN edvid IS NOT NULL THEN 1 ELSE NULL END) as qtddetalhada
                FROM par3.execucao_notafiscal_contratoitens enc
                LEFT JOIN par3.execucao_detalhamento_veiculo edv ON edv.encid = enc.encid AND edvstatus = 'A'
                WHERE enc.encid = {$encid}
                GROUP BY encqtditens";

        return $this->pegaLinha($sql);
    }
}
