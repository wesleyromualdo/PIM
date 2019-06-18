<?php

class Par3_Model_ExecucaoContrato extends Modelo
{

    protected $stNomeTabela = 'par3.execucao_contrato';

    protected $arChavePrimaria = array(
        'ecoid',
    );

    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'proid' => array('tabela' => 'par3.processo', 'pk' => 'proid'),
        'usucpfinclusao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    protected $arAtributos = array(
        'ecoid' => null,
        'arqid' => null,
        'proid' => null,
        'econumero' => null,
        'ecocnpj' => null,
        'ecorazaosocialfornecedor' => null,
        'ecocpfresponsavel' => null,
        'ecodta' => null,
        'ecodsc' => null,
        'ecovalor' => null,
        'ecoqtd' => null,
        'ecostatus' => null,
        'usucpfinclusao' => null,
        'ecodtainclusao' => null,
        'ecocpfinativacao' => null,
        'ecodtinativacao' => null,
    );

    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        if ($this->ecocnpj) {
            $this->ecocnpj = trim($this->ecocnpj);
        }

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     * Dados gerais do processo que poderão ser usados em todas as abas (escolas, documento, contrato e nf)
     * ineid = processo de emendas.
     * @param $proid
     * @return array|bool|mixed|NULL
     */
    public function recuperarProcesso($proid)
    {
        $sql = "SELECT pronumeroprocesso, dotnumero||'-'||dot.intoid as dotnumero, ineid, dot.intoid, dot.dotid
                FROM par3.processo pro
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid AND dotstatus = 'A'
                INNER JOIN par3.processoparcomposicao ppc ON ppc.proid = pro.proid
                LEFT JOIN par3.iniciativa_emenda ine ON ine.inpid = ppc.inpid
                WHERE
                prostatus = 'A' AND
                pro.proid = {$proid}";
        return $this->pegaLinha($sql);
    }

    public function sqlListaContratos($proid)
    {
        $sql = "SELECT DISTINCT
                        eco.ecoid,
                        econumero,
                        ecocnpj,
                        ecorazaosocialfornecedor,
                        to_char(ecodta, 'dd/mm/YYYY') as datacontrato,
                        ecoqtd,
                        ecovalor,
                        dotvalortermo - totalSolicitacao - vsaldo.vlrpago as saldo,
                        coalesce(vsaldo.vlrpago,0) as totalpago,
                        ultimaSituacao, 
                        CASE WHEN ecostatus = 'A'
                         THEN '<span class=\"success\"><b>Ativo</b></span>'
                         ELSE '<span class=\"danger\"><b>Inativo</b></span>'
                        END AS status,
                        ecostatus,
                        eco.arqid,
                        to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' - (' || (dot.dotdatafimvigencia::date - CURRENT_DATE) || ' dias)' as dotdatafimvigencia
                FROM par3.execucao_contrato eco
                LEFT JOIN par3.execucao_contratoitens eci ON eco.ecoid = eci.ecoid
                LEFT JOIN par3.analise_itemcomposicao aic ON aic.aicid = eci.aicid
                LEFT JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = aic.ipiid
                LEFT JOIN par3.iniciativa_planejamento inp ON ipi.inpid = inp.inpid
                INNER JOIN par3.processo pro ON pro.proid = eco.proid
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid
                INNER JOIN par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
                LEFT JOIN LATERAL (SELECT sum(sdpvalor) totalSolicitacao FROM par3.solicitacao_desembolso_par sdp
                                     INNER JOIN workflow.documento doc ON sdp.docid = doc.docid
                                     WHERE sdp.ecoid = eco.ecoid 
                                     AND sdpstatus = 'A' AND doc.esdid = ".PAR3_DESEMBOLSO_APROVADO.") v on true
                LEFT JOIN LATERAL (SELECT esddsc as ultimaSituacao FROM par3.solicitacao_desembolso_par sdp
                                     INNER JOIN workflow.documento doc ON sdp.docid = doc.docid
                                     INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                                     WHERE sdp.ecoid = eco.ecoid AND sdpstatus = 'A'
                                     ORDER BY sdpid DESC LIMIT 1) v2 on true                                                         
                WHERE eco.proid = {$proid} AND dotstatus = 'A'
                ORDER BY ecostatus,eco.ecoid
                ";

        return $sql;
    }

    public function dadosEditarContrato($ecoid)
    {
        $sql = "SELECT
                       ecocnpj,
                       ecorazaosocialfornecedor,
                       ecocpfresponsavel,
                       econumero,
                       ecodta,
                       ecodsc,
                       to_char(ecodta, 'dd/mm/YYYY') as ecodta,
                       ecovalor,
                       ecoqtd,
                       arqnome,
                       arqextensao,
                       arq.arqid,
                       ecostatus,
                       (SELECT coalesce(SUM(ecivaloritens),0) FROM par3.execucao_contratoitens WHERE ecoid = {$ecoid}) as valortotalitens,
                       (SELECT coalesce(SUM(eciqtditens),0) FROM par3.execucao_contratoitens WHERE ecoid = {$ecoid}) as qtdtotalitens
                FROM par3.execucao_contrato eco
                INNER JOIN arquivo arq ON eco.arqid = arq.arqid
                WHERE ecoid = {$ecoid}";

        return $this->pegaLinha($sql);
    }

    public function recuperaIniciativas($processo)
    {
        $sql = "SELECT distinct ON(inp.inpid)
                    (inp.inpid||' - '||ind.inddsc || ' - ' || eta.etadsc) as descricao,
                    inp.inpid as id,
                    COUNT(CASE WHEN eci.eciid IS NULL THEN 1 ELSE NULL END) as qtditens,
                    'Itens sem contrato: ' as descitem
                FROM par3.processo pro
                INNER JOIN par3.iniciativa_planejamento inp ON inp.inuid = pro.inuid
                INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid
                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
                INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON inp.inpid = ipi.inpid
                INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                LEFT JOIN par3.execucao_contratoitens eci ON eci.aicid = aic.aicid
                WHERE 
                ppcstatus = 'A'
                AND aicstatus = 'A'
                AND aicqtdaprovado > 0
                AND pronumeroprocesso = '{$processo}'
                GROUP BY inp.inpid, inddsc, eta.etadsc
         ";

        return $this->carregar($sql);
    }

    public static function inputQtd($qtd, $dados){
//        ver($dados);

        $max = $dados['qtdsemcontrato'] + $qtd;

        $aicid = $dados['aicaicid'];


        if($dados['eciaicid'] && $dados['statuscontrato'] == 'A' || $dados['condicaoedicao'] != ''){
            $inputQtd = "<input type=\"text\" value=\"{$qtd}\" class=\"qtditem txtright\" name=\"eciqtditens[]\"
                            data-max=\"{$max}\" data-vref=\"{$dados['aicvaloraprovado']}\" data-eciid=\"{$dados['eciid']}\"
                            data-aicid=\"{$aicid}\" size=\"7\"
                            onkeyup=\"this.value=mascaraglobal('#######',this.value);\">";
        }else if($dados['qtdsemcontrato'] < 1 || $dados['statuscontrato'] == 'I'){
            //se o item estiver com a quantidade sem contrato = 0 e, ao editar, for de um contrato diferente, deve aparecer desabilitado
            $inputQtd = "<input type=\"text\" value=\"{$qtd}\" class=\"qtditem\" name=\"eciqtditens[]\" data-aicid=\"{$aicid}\" size=\"7\" disabled>";
        }else{
            $inputQtd = "<input type=\"text\" class=\"qtditem txtright\" name=\"eciqtditens[]\" data-aicid=\"{$aicid}\"
                            data-max=\"{$dados['qtdsemcontrato']}\" data-vref=\"{$dados['aicvaloraprovado']}\" size=\"7\" data-eciid=\"{$dados['eciid']}\"
                            onkeyup=\"this.value=mascaraglobal('#######',this.value);\">";
        }

        return $inputQtd;
    }

    public function sqlListaItens($inpid, $ecoid)
    {
        //se for edição
        if ($ecoid) {
            $whereJoin = "AND eci.ecoid = {$ecoid}";
            $lateralJoin = 'LEFT JOIN LATERAL (SELECT ecostatus as statuscontrato FROM par3.execucao_contrato eco1
                                     WHERE eco1.ecoid = ' . $ecoid . ') v on true ';
        } else {
            $condicaoEdicao = "AND false"; //apenas para dizer ao sql que não é uma edição
        }

        #input de valor
        //para editar contrato, mostrando o valor já cadastrado
        $inputValorEdit = "'<input type=\"text\" value=\"' || max(eci.ecivaloritens) || '\" class=\"valoritem txtright\"
                            name=\"ecivaloritens[]\" data-aicid=\"' || aic.aicid || '\" size=\"11\" readonly >'";
        //sem o atributo value, para edição (quando este item não estiver no contrato editado), ou para inclusão de novo contrato
        $inputValor = "'<input type=\"text\" class=\"valoritem txtright\" name=\"ecivaloritens[]\" data-aicid=\"' || aic.aicid || '\" size=\"11\" readonly >'";
        //se o item estiver com a quantidade sem contrato = 0 e, ao editar, for de um contrato diferente, deve aparecer desabilitado
        $inputValorDisabled = "'<input type=\"text\" class=\"valoritem\" name=\"ecivaloritens[]\" data-aicid=\"' || aic.aicid || '\" size=\"11\" disabled>'";


        $avisoEscola = '<span class="help-block m-b-none"><i class="fa fa-question-circle" style="color: #1c84c6;"></i> <small style="font: 8px">Clique acima para adicionar Item</small></span>';

        $inputDestinacao = "'<input type=\"text\" class=\"qtddestinada txtright\" name=\"eciqtddestinada[]\"  size=\"7\"
                            data-eciid=\"\"
                            data-aicid=\"' || aic.aicid || '\"
                            onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" >'";
        $inputDestinacaoEdit = "'<input type=\"text\" class=\"qtddestinada txtright\" name=\"eciqtddestinada[]\"  size=\"7\"
                            value=\"' || coalesce(max(eciqtddestinada)::text,'') || '\"
                            data-eciid=\"' || coalesce(max(eci.eciid)::text,'') || '\"
                            data-aicid=\"' || aic.aicid || '\"
                            onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" >'";
        $inputDestinacaoDisabled = "'<input type=\"text\" class=\"qtddestinada txtright\" name=\"eciqtddestinada[]\"  size=\"7\"
                                    data-eciid=\"\"
                                    data-aicid=\"' || aic.aicid || '\"
                                    onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" disabled>'";
        $inputDestinacaoEscola = "'<input type=\"text\" class=\"modalescola txtright\" style=\"cursor:pointer\" readonly name=\"eciqtddestinada[]\" size=\"7\"
                            onclick=\"modalEscolas(this)\"
                            data-eciid=\"\"
                            data-ipiid=\"' || aic.ipiid || '\">{$avisoEscola}'";
        $inputDestinacaoEscolaEdit = "'<input type=\"text\" class=\"modalescola txtright\" style=\"cursor:pointer\" readonly name=\"eciqtddestinada[]\" size=\"7\"
                            onclick=\"modalEscolas(this)\"
                            data-eciid=\"' || coalesce(max(eci.eciid)::text,'') || '\"
                            data-ipiid=\"' || aic.ipiid || '\"
                            value=\"' || coalesce(max(eciqtddestinada)::text,'') || '\">{$avisoEscola}'";
        $inputDestinacaoEscolaDisabled = "'<input type=\"text\" class=\"modalescola txtright\" style=\"cursor:pointer\" readonly name=\"eciqtddestinada[]\" size=\"7\"
                            onclick=\"modalEscolas(this)\"
                            data-eciid=\"\"
                            data-ipiid=\"' || aic.ipiid || '\" disabled>{$avisoEscola}'";

        //intaid -> 2 = por escola; 1 = global;
        $sql = "SELECT
                    aic.aicid as aicaicid,
                    eci.aicid as eciaicid,
                    itcdsc,
                    ppcano,
                    aic.aicid as contratos,
                    aic.aicqtdaprovado,
                    qtdsemcontrato,
                    aic.aicvaloraprovado valorreferencia,
                    aic.aicvaloraprovado * aic.aicqtdaprovado as valortotal,
                    eci.eciqtditens as inputqtd,
                    CASE WHEN eci.aicid IS NOT NULL AND statuscontrato = 'A' {$condicaoEdicao}
                      THEN {$inputValorEdit}
                      ELSE
                        CASE WHEN qtdsemcontrato < 1 OR statuscontrato = 'I'
                          THEN {$inputValorDisabled}
                          ELSE {$inputValor}
                        END
                    END AS inputvalor,
                    CASE WHEN ini.intaid = 1 THEN
                        CASE WHEN eci.aicid IS NOT NULL AND statuscontrato = 'A' {$condicaoEdicao}
                          THEN {$inputDestinacaoEdit}
                          ELSE
                            CASE WHEN qtdsemcontrato < 1 OR statuscontrato = 'I'
                              THEN {$inputDestinacaoDisabled}
                              ELSE {$inputDestinacao}
                            END
                        END
                      ELSE
                        CASE WHEN ini.intaid = 2 then
                          CASE WHEN eci.aicid IS NOT NULL AND statuscontrato = 'A' {$condicaoEdicao}
                            THEN {$inputDestinacaoEscolaEdit}
                            ELSE
                              CASE WHEN qtdsemcontrato < 1
                                THEN {$inputDestinacaoEscolaDisabled}
                                ELSE {$inputDestinacaoEscola}
                              END
                          END
                      END
                    END as qtddestinada,
                    statuscontrato,
                    '{$condicaoEdicao}' as condicaoedicao,
                    aicvaloraprovado,
                    coalesce(max(eci.eciid)::text,'') as eciid
                FROM par3.iniciativa_planejamento inp
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON inp.inpid = ipi.inpid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.unidade_medida uniid ON itc.uniid = uniid.uniid
                INNER JOIN par3.processo pro ON inp.inuid = pro.inuid
                INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                LEFT JOIN par3.execucao_contratoitens eci ON eci.aicid = aic.aicid {$whereJoin}
                LEFT JOIN par3.execucao_contrato eco ON eco.ecoid = eci.ecoid AND ecostatus = 'A'
                INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid
                {$lateralJoin}
                ,LATERAL (SELECT coalesce(aicqtdaprovado -
                            (SELECT sum(eciqtditens)
                              FROM par3.execucao_contratoitens t
                              INNER JOIN par3.execucao_contrato t2 ON t2.ecoid = t.ecoid
                              WHERE t.aicid = aic.aicid AND ecostatus = 'A'), aicqtdaprovado))
                         AS qtd(qtdsemcontrato)
                  
                WHERE
                ipi.ipistatus = 'A'
                AND aicstatus = 'A'
                AND aicqtdaprovado > 0
                AND inp.inpid = {$inpid}
                GROUP BY aic.aicid, eci.aicid, itcdsc , itc.itcespecificacao, unidsc, ppcano, 
                         qtdsemcontrato, eciqtditens, ini.intaid, aic.aicqtdaprovado, 
                         aic.aicvaloraprovado, statuscontrato
                ORDER BY 1
        ";

        return $sql;
    }

    /** Contratos relacionados ao item
     * @param $aicid
     * @return string
     */
    public function sqlContratosItem($aicid)
    {
        $sql = "SELECT DISTINCT
                        eco.ecoid,
                        econumero,
                        eciqtditens,
                        ecivaloritens
                FROM par3.execucao_contrato eco
                INNER JOIN par3.execucao_contratoitens eci ON eco.ecoid = eci.ecoid
                WHERE eci.aicid = {$aicid}
                AND ecostatus = 'A'
                ORDER BY 1
                ";
        return $sql;
    }

    public function recuperarValorTotalItens($ecoid)
    {
        $sql = "SELECT SUM(ecivaloritens) over() as valortotal, 
                       SUM(eciqtditens) over() as qtdtotal, 
                       eciid, eciqtditens, aicid
                FROM par3.execucao_contratoitens               
                WHERE ecoid = {$ecoid} 
                GROUP BY eciid";
        $itens = $this->carregar($sql);

        //Atualiza valor e quantidade na tabela de contrato
        if(!$itens[0]['qtdtotal']) {
            $itens[0]['qtdtotal'] = 0;
            $itens[0]['valortotal'] = 0;
        }
        $sql = "UPDATE par3.execucao_contrato SET ecovalor = {$itens[0]['valortotal']}, ecoqtd = {$itens[0]['qtdtotal']} WHERE ecoid = {$ecoid}";
        $this->executar($sql);
        $this->commit();


        return $itens;
    }

    /**
     * @param $aicid
     * @param $ecoid
     * @param $eciqtditens
     * @param $ecivaloritens
     * @param null $eciid
     * @return |bool|mixed|string
     *      array ('erro' => 'itemexistente', 'qtditem' => $qtditem) em caso de item cadastrado em nota fiscal
     *      id do item caso tenha sido alterado com sucesso
     *      false em caso de falha
     *
     */
    public function salvarItem($aicid, $ecoid, $eciqtditens, $ecivaloritens, $eciid = null)
    {
//        ver($eciid,d);
        if ($eciid) {
            //se valor igual a zero, remove o item do contrato
            if ($eciqtditens == 0) {
                $sql = "SELECT count(enc.eciid) as existe, eciqtditens, ecivaloritens
                        FROM par3.execucao_notafiscal_contratoitens enc
                        INNER JOIN par3.execucao_contratoitens eci ON eci.eciid = enc.eciid
                        WHERE eci.eciid = {$eciid}
                        GROUP BY eci.eciqtditens, eci.ecivaloritens";
                $item = $this->pegaLinha($sql);
                if($item['existe'] > 0){
                    return array('erro' => 'itemnf', 'qtditem' => $item['eciqtditens'], 'vlritem' => $item['ecivaloritens']);
                }
                $sql = "DELETE FROM par3.execucao_contratoitens WHERE eciid = {$eciid}";
            } else {
                $sql = "UPDATE par3.execucao_contratoitens set aicid={$aicid}, ecoid={$ecoid}, eciqtditens={$eciqtditens}, ecivaloritens={$ecivaloritens}
                    WHERE eciid = {$eciid} RETURNING eciid";
            }
        } else {
            $sql = "INSERT INTO par3.execucao_contratoitens (aicid, ecoid, eciqtditens, ecivaloritens)
                VALUES ({$aicid}, {$ecoid}, {$eciqtditens}, {$ecivaloritens}) RETURNING eciid";
        }
//ver($sql,d);
        $id = $this->pegaUm($sql);
        if ($this->commit()) {
            return $id;
        }
        return false;
    }

    /**
     * Incluir todos
     * @param $arrItem
     *          [0] - aicid
     */
    public function inserirArrayItem($arrItem)
    {
        $existeUpdate = false;
        $existeInsert = false;

        $sqlInsert = "INSERT INTO par3.execucao_contratoitens (ecoid, aicid, ecivaloritens, eciqtditens) VALUES ";
        $sqlUpdate = "UPDATE par3.execucao_contratoitens as eci 
                            SET eciqtditens = eci2.eciqtditens, ecivaloritens = eci2.ecivaloritens
                            FROM (values";
        foreach ($arrItem as $item) {
            if ($item['eciid']) {
                $sqlUpdate .= "({$item['eciqtditens']}, {$item['ecivaloritens']}, {$item['eciid']}),";
                $existeUpdate = true;
            } else {
                $sqlInsert .= "({$item['ecoid']}, {$item['aicid']}, {$item['ecivaloritens']}, {$item['eciqtditens']}),";
                $existeInsert = true;
            }
        }
        if ($existeUpdate) {
            $sqlUpdate = rtrim($sqlUpdate, ',');
            $sqlUpdate .= ") as eci2(eciqtditens, ecivaloritens, eciid) WHERE eci2.eciid = eci.eciid";
            $this->executar($sqlUpdate);
        }

        if ($existeInsert) {
            $sqlInsert = rtrim($sqlInsert, ',');
            $this->executar($sqlInsert);
        }

        if ($existeInsert || $existeUpdate) {
            $this->commit();
        }
    }

    /* Funções do monitoramento de itens */

    public function sqlModalEscolas($eciid, $ipiid, $ecoid, $dadosUnidade, $postMuncod = '')
    {
//        $offset = ($paginaAtual -1) * $itensPorPagina;

        $qtdDestinada = "'<input type=\"text\" class=\"qtdescola txtright\" name=\"emeqtd[]\"  size=\"7\"
                            value=\"' || coalesce(emeqtd::text,'') || '\"
                            data-eciid=\"{$eciid}\"
                            data-ipeid=\"' || coalesce(ipe.ipeid::text, '') || '\"
                            data-emeid=\"' || coalesce(eme.emeid::text,'') || '\"
                            data-escid=\"' || esc.escid || '\"
                            onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\">'";

        if($dadosUnidade['itrid'] == 1){
            $where = ' esc.estuf = \'' . $dadosUnidade['estuf'] . '\' AND escco_esfera = \'E\' ';
            if($postMuncod > 0){
                $where .= ' AND esc.muncod = \'' . $postMuncod . '\' ';
            }
        }else{
            $where = ' esc.muncod = \'' . $dadosUnidade['muncod'] . '\' AND escco_esfera = \'M\' ';
        }

        $sql = "SELECT DISTINCT ON (esc.esccodinep)
                    mundescricao, 
                    esccodinep, 
                    escnome, 
                    escno_localizacao, 
                    escno_esfera, 
                    ipequantidade, 
                    {$qtdDestinada} as qtddestinada,
                    escco_situacao_funcoinamento,
                    escqtd_salas,
                    escqtd_alunos as qtdaluno,
                    count(esc.escid) over() as totalescolas
                 FROM par3.escola  esc
                 INNER JOIN territorios.municipio mun ON mun.muncod = esc.muncod                
                 LEFT JOIN par3.execucao_contrato eco ON eco.ecoid = {$ecoid} AND ecostatus = 'A'
                 LEFT JOIN par3.execucao_contratoitens eci ON eci.ecoid = eco.ecoid                
                 LEFT JOIN par3.iniciativa_planejamento_item_composicao_escola ipe ON ipe.escid = esc.escid and ipestatus = 'A' and ipe.ipiid = {$ipiid}
                 LEFT JOIN par3.execucao_monitoramento_escola eme ON eme.escid = esc.escid AND eme.eciid = {$eciid} AND emestatus = 'A'
                 WHERE 
                   --esc.muncod = '2804607'
                   --esc.estuf = 'SE' 
                   --AND escco_esfera = 'E'
                   --and escco_situacao_funcoinamento in (1,2) 
                   {$where} 
                 GROUP BY mun.mundescricao, esccodinep, escnome, escqtd_alunos, esc.escid,
                          escno_localizacao, escno_esfera, escco_situacao_funcoinamento, 
                          emeqtd, ipequantidade, eco.ecoid, ipe.ipeid, eme.emeid, escqtd_salas
                 ORDER BY esc.esccodinep, escco_situacao_funcoinamento , escno_localizacao
                 
                ";

        return $sql;
    }

    public function salvarQuantidadeDestinada($eciid, $qtddestinada)
    {

        $sql = "UPDATE par3.execucao_contratoitens SET eciqtddestinada = {$qtddestinada} WHERE eciid = {$eciid}";
        $this->executar($sql);
        return $this->commit();
    }

    public function salvarQuantidadeEscola($eciid, $escid, $emeqtd, $emeid = null, $ipeid = null)
    {

        $campoIpeid = $ipeid ? ', ipeid' : '';
        $valorIpeid = $ipeid ? ', ' . $ipeid : '';

        if(!$emeid){ // Verificação necessária para o caso do usuário inserir um item estando com duas abas abertas
            if($ipeid){
                $sql = "SELECT emeid FROM par3.execucao_monitoramento_escola WHERE eciid = {$eciid} AND escid = {$escid} AND ipeid = {$ipeid}";
            }else{
                $sql = "SELECT emeid FROM par3.execucao_monitoramento_escola WHERE eciid = {$eciid} AND escid = {$escid}";
            }
            $emeid = $this->pegaUm($sql);
        }

        if ($emeid != null && $emeid != '') {
            if ($emeqtd == 0) {
                $sql = "DELETE FROM par3.execucao_monitoramento_escola WHERE emeid = {$emeid}";
            } else {
                $sql = "UPDATE par3.execucao_monitoramento_escola SET emeqtd={$emeqtd} WHERE emeid = {$emeid} RETURNING emeid";
            }
        } else {
            $sql = "INSERT INTO par3.execucao_monitoramento_escola (eciid $campoIpeid, emeqtd, usucpf, escid) 
                VALUES ({$eciid} {$valorIpeid}, {$emeqtd}, {$_SESSION['usucpf']}, {$escid}) RETURNING emeid";
        }
        $idmonitoramento = $this->pegaUm($sql);


        $sql = "UPDATE par3.execucao_contratoitens eci SET eciqtddestinada = subquery.total
                FROM (SELECT sum(emeqtd) total FROM par3.execucao_monitoramento_escola where eciid = {$eciid}) as subquery
                WHERE eci.eciid = {$eciid} RETURNING subquery.total;";
        $total = $this->pegaUm($sql);

        if($this->commit()){
            $dados['idmonitoramento'] = $idmonitoramento;
            $dados['total'] = $total;
            $dados['erro'] = '';
        }else{
            $dados['erro'] = 'insert_error';
        }
        return simec_json_encode($dados);
    }
    /* Fim funções monitoramento */


    /** Lista contratos - Nota fiscal
     * @param $ecocnpj
     * @param $proid
     * @param null $ntfid
     * @return array|mixed|NULL
     */
    public function buscarContratosPorCnpjEProcesso($ecocnpj, $proid, $ntfid = null)
    {

        if ($ntfid) {
            $_and = " AND ntfid = {$ntfid}";
        }

        $sql = "
            SELECT DISTINCT 
              econumero || ' - ' || ecodsc as descricao,
              ecoid as id,
              (SELECT count(eci.eciid) 
                FROM par3.execucao_contratoitens eci
                INNER JOIN par3.execucao_contrato eco ON eco.ecoid = eci.ecoid
                LEFT JOIN par3.execucao_notafiscal_contratoitens enc ON enc.eciid = eci.eciid
                WHERE eco.ecocnpj = '{$ecocnpj}'
                AND eco.proid = {$proid}
                AND encid IS NULL
                AND eco.ecoid = t.ecoid) as qtditens,
              'Quantidade de itens sem nota fiscal: ' as descitem,
              (SELECT sum(encvaloritens) 
                FROM par3.execucao_notafiscal_contratoitens enc
                INNER JOIN par3.execucao_contratoitens eci ON eci.eciid = enc.eciid
                INNER JOIN par3.execucao_contrato eco ON eco.ecoid = eci.ecoid
                WHERE eco.ecocnpj = '{$ecocnpj}'
                AND eco.proid = {$proid}
                {$_and}) as valortotalitens
            FROM
                par3.execucao_contrato t
            WHERE
                proid = {$proid}
                AND ecocnpj = '{$ecocnpj}'
                AND ecostatus = 'A'
            ORDER BY
                t.ecoid
        ";

        return $this->carregar($sql);
    }

    /** Itens do contrato - Nota fiscal
     * @param $ecoid
     * @return array|mixed|NULL
     */
    public function buscarItensContratoPorId($ecoid)
    {
        //se for edição, tem que colocar condição
        //and ntfid = {$ntfid}
        if ($_SESSION['ntfid']) {
            $_and = "AND enc.ntfid = {$_SESSION['ntfid']}";
        }
        $sql = "
            SELECT DISTINCT
              ipi.ipiid,
              itc.itcdsc,
              eci.eciqtditens AS qtdTotalItens,
              qtdsemnota AS qtdDisponivelItens,
              eci.ecivaloritens as vlrtotalitens,
              aic.aicvaloraprovado,
              CASE
                WHEN enc.encqtditens IS NOT NULL
                  THEN '<input type=\"text\" value=\"'||enc.encqtditens||'\" onkeyup=\"this.value=mascaraglobal(''#######'',this.value);\" class=\"form-control\" name=\"qtditensnota[' || eci.eciid || ']\" data-restantes=\"'|| qtdsemnota + enc.encqtditens ||'\" data-ipivalorreferencia=\"'||aic.aicvaloraprovado||'\" data-encid=\"'|| coalesce(enc.encid::text, '') ||'\" />'
                ELSE 
                  CASE WHEN qtdsemnota < 1
                    THEN '<input type=\"text\" onkeyup=\"this.value=mascaraglobal(''#######'',this.value);\" class=\"form-control\" name=\"qtditensnota[' || eci.eciid || ']\" data-restantes=\"'|| qtdsemnota ||'\" data-ipivalorreferencia=\"'||aic.aicvaloraprovado||'\" data-encid=\"'|| coalesce(enc.encid::text, '') ||'\" disabled />'
                    WHEN (SELECT ntfstatus FROM par3.execucao_notafiscal WHERE ntfid = {$_SESSION['ntfid']}) = 'I'
                      THEN '<input type=\"text\" onkeyup=\"this.value=mascaraglobal(''#######'',this.value);\" class=\"form-control\" name=\"qtditensnota[' || eci.eciid || ']\" data-restantes=\"'|| qtdsemnota ||'\" data-ipivalorreferencia=\"'||aic.aicvaloraprovado||'\" data-encid=\"'|| coalesce(enc.encid::text, '') ||'\" disabled />'
                    ELSE '<input type=\"text\" onkeyup=\"this.value=mascaraglobal(''#######'',this.value);\" class=\"form-control\" name=\"qtditensnota[' || eci.eciid || ']\" data-restantes=\"'|| qtdsemnota ||'\" data-ipivalorreferencia=\"'||aic.aicvaloraprovado||'\" data-encid=\"'|| coalesce(enc.encid::text, '') ||'\" />'
                  END
              END AS qtditensnota,
              '<input type=\"text\" class=\"form-control\" name=\"vlritensnota[' || eci.eciid || ']\" readOnly=\"readonly\" data-encid=\"'|| coalesce(enc.encid::text, '') ||'\" value=\"'|| coalesce(enc.encvaloritens::text,'') ||'\" />' as vlritensnota
              --count(CASE WHEN edvid IS NOT NULL THEN 1 ELSE NULL END) as qtddetalhada,
              --'<button class=\"visualizar_item btn btn-success btn-sm\" type=\"button\" onclick=\"detalharItem('|| enc.encid ||')\"> Detalhar </button>'
            FROM
              par3.execucao_contratoitens eci
              INNER JOIN par3.execucao_contrato eco     ON eco.ecoid = eci.ecoid
              LEFT JOIN par3.analise_itemcomposicao aic ON aic.aicid = eci.aicid
              LEFT JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = aic.ipiid
              LEFT JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
              LEFT JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
              LEFT JOIN par3.execucao_notafiscal_contratoitens enc ON enc.eciid = eci.eciid {$_and}
              --LEFT JOIN par3.execucao_detalhamento_veiculo edv ON edv.encid = enc.encid AND edvstatus = 'A'
              ,LATERAL (SELECT coalesce(eciqtditens - 
                            (SELECT sum(encqtditens) 
                              FROM par3.execucao_notafiscal t
                              INNER JOIN par3.execucao_notafiscal_contratoitens t2 ON t2.ntfid = t.ntfid
                              WHERE t2.eciid = eci.eciid AND ntfstatus = 'A'), eciqtditens)) 
                         AS qtd(qtdsemnota)
            WHERE
              eco.ecoid = {$ecoid}
              and ecostatus = 'A'
            GROUP BY ipi.ipiid, itc.itcid, eci.eciid, qtdsemnota, enc.encid, aic.aicvaloraprovado
            ORDER BY ipi.ipiid DESC
        ";

        return $this->carregar($sql);
    }


    /* Funções Solicitação desembolso */

    /**
     * Caso o contrato já possua solicitações de desembolso,
     * seu valor não pode ser alterado para um valor menor do que a soma das solicitações aprovadas.
     * @param $ecoid
     * @return bool
     */
    public function verificaValorContrato($ecoid, $valorContrato)
    {
        $sql = "SELECT sum(sdpvalor) totalSolicitacao FROM par3.solicitacao_desembolso_par sdp
                                     INNER JOIN workflow.documento doc ON sdp.docid = doc.docid
                                     WHERE sdp.ecoid = {$ecoid} 
                                     AND sdpstatus = 'A' AND doc.esdid = ".PAR3_DESEMBOLSO_APROVADO;

        $valorDesembolso = $this->pegaUM($sql);
        if ($valorContrato < $valorDesembolso) {
            return false;
        }
        return true;
    }

    /**
     * O contrato não pode ser removido caso haja solicitações de desembolso e notas fiscais cadastradas para este contrato.
     * @param $ecoid
     * @return bool
     */
    public function verificaPodeRemoverContrato($ecoid)
    {
        $sql = 'SELECT count(sdpid) 
                FROM par3.solicitacao_desembolso_par sdp
                INNER JOIN workflow.documento doc ON doc.docid = sdp.docid
                WHERE ecoid = ' . $ecoid . '
                AND sdpstatus = \'A\'
                AND doc.esdid in(' . PAR3_DESEMBOLSO_APROVADO . ', ' . PAR3_DESEMBOLSO_AGUARDANDOANALISE .  ', ' . PAR3_DESEMBOLSO_EMANALISE . ', ' . PAR3_DESEMBOLSO_DILIGENCIA . ', ' . PAR3_DESEMBOLSO_VALIDACAO_COORDENADOR .')';

        $solicitacao = $this->pegaUm($sql);
        if ($solicitacao > 0) {
            return false;
        }
        return true;
    }

    /* Funções Lista de Escolas */

    public function sqlEscolasTermo($dados, $offset, $itensPorPagina, $excel = false)
    {

        $proid = $dados['proid'];

        $_and = '';
        if($dados['muncod']){
            $_and .= " AND mun.muncod = '{$dados['muncod']}'";
        }
        if($dados['ipiid']){
            $_and .= ' AND ipi.ipiid = ' . $dados['ipiid'];
        }
        if($dados['escno_localizacao']){
            $_and .= " AND escno_localizacao = '{$dados['escno_localizacao']}'";
        }
        if($dados['esccodinep']){
            $_and .= " AND esccodinep = '{$dados['esccodinep']}'";
        }

        if(!$excel){
            $campomuncod = ' mun.muncod, ';
            $campototal = ' ,count(esc.escid) over() as totalescolas ';
        }

        if(!$dados['mostrarTodas']){

            $limit = ' LIMIT ' . $itensPorPagina;

            if ($itensPorPagina == 0) {
                $limit = '';
            }

            $paginacao = "OFFSET {$offset} {$limit}";
        }

        if($dados['ineid'] > 0){
            $qtdEscola = ' AND iee.ieequantidade > 0 ';
        }else{
            $qtdEscola = ' AND ipe.ipequantidade > 0 ';
        }

        $sql = "SELECT DISTINCT
                    {$campomuncod}
                    mundescricao,
                    esccodinep,
                    inp.inpid::text,
                    escnome,
                    eta.etadsc,
                    escno_localizacao,
                    escno_esfera,
                    itcdsc,
                    CASE WHEN ieequantidade > 0 THEN ieequantidade ELSE ipequantidade END AS quantidade,
                    emeqtd,
                    escqtd_salas,
                    escqtd_alunos as qtdaluno
                    {$campototal}
                FROM par3.iniciativa_planejamento_item_composicao ipi
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = ipi.inpid
                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                INNER JOIN par3.processo pro ON pro.proid = ppc.proid
                INNER JOIN par3.iniciativa_planejamento_item_composicao_escola ipe ON ipe.ipiid = ipi.ipiid AND ipestatus = 'A'
                INNER JOIN par3.escola esc ON esc.escid = ipe.escid
                INNER JOIN territorios.municipio mun ON mun.muncod = esc.muncod
                LEFT JOIN par3.iniciativa_emenda ie ON ie.inpid = inp.inpid and inestatus = 'A'
                LEFT JOIN par3.iniciativa_emenda_item_composicao_escola iee ON iee.ipeid = ipe.ipeid AND iee.ineid = ie.ineid and iee.ieestatus = 'A'
                LEFT JOIN par3.execucao_contratoitens eci ON eci.aicid = aic.aicid
                LEFT JOIN par3.execucao_monitoramento_escola eme ON eme.ipeid = ipe.ipeid AND eme.eciid = eci.eciid 
                WHERE ppcstatus = 'A'
                AND aic.aicstatus = 'A' AND aicaprovado in ('S', 'CS')
                AND pro.proid = {$proid}
                {$_and}
                {$qtdEscola}
                    
                UNION ALL 
                -- escolas extintas ou que não foram planejadas
                SELECT DISTINCT
                    mun.muncod, 
                    mundescricao,
                    esccodinep,
                    '-',
                    escnome,
                    '-',
                    escno_localizacao,
                    escno_esfera,
                    itcdsc,
                    0,
                    emeqtd,
                    escqtd_salas,
                    escqtd_alunos as qtdaluno
                    ,count(esc.escid) over() as totalescolas 
                FROM par3.execucao_monitoramento_escola eme
                INNER JOIN par3.execucao_contratoitens eci ON eci.eciid = eme.eciid
                INNER JOIN par3.execucao_contrato eco ON eco.ecoid = eci.ecoid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.aicid = eci.aicid AND aicstatus = 'A'
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = aic.ipiid
                INNER JOIN par3.escola esc ON esc.escid = eme.escid
                INNER JOIN territorios.municipio mun ON mun.muncod = esc.muncod
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                where eco.proid = {$proid}
                AND eme.ipeid is null
                {$_and}

                ORDER BY mundescricao, escnome
                {$paginacao}
                ";

        return $sql;
    }

    public function retornaTotalEscolas($proid, $ineid){
        $sql = 'SELECT COUNT (*) FROM (';
        $sql .= $this->sqlEscolasTermo(array('proid' => $proid,'mostrartodas' => true, 'ineid' => $ineid), 0, 0);
        $sql .= ') c';
        $total = $this->pegaUm($sql);
        return $total;
    }

    public function retornaSqlMunicipios($proid, $inuid){
        $sql = "SELECT mun.muncod as codigo, 
                       mundescricao as descricao
                FROM par3.iniciativa_planejamento_item_composicao ipi
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = ipi.inpid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                INNER JOIN par3.processo pro ON pro.proid = ppc.proid
                INNER JOIN par3.iniciativa_planejamento_item_composicao_escola ipe ON ipe.ipiid = ipi.ipiid AND ipestatus = 'A' AND ipequantidade > 0
                INNER JOIN par3.escola esc ON esc.escid = ipe.escid
                INNER JOIN territorios.municipio mun ON mun.muncod = esc.muncod
                LEFT JOIN par3.execucao_contratoitens eci ON eci.aicid = aic.aicid
                LEFT JOIN par3.execucao_monitoramento_escola eme ON eme.ipeid = ipe.ipeid AND eme.eciid = eci.eciid 
                WHERE ppcstatus = 'A'
                AND aicqtdaprovado > 0
                AND pro.proid = {$proid}
                    GROUP BY mun.muncod, mundescricao
                    ORDER BY mundescricao";
        return $sql;
    }

    public function retornaSqlItens($proid, $inuid){
        $sql = "SELECT aic.ipiid as codigo,
                       itcdsc as descricao
                   FROM par3.iniciativa_planejamento_item_composicao ipi
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = ipi.inpid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                INNER JOIN par3.processo pro ON pro.proid = ppc.proid
                INNER JOIN par3.iniciativa_planejamento_item_composicao_escola ipe ON ipe.ipiid = ipi.ipiid AND ipestatus = 'A' AND ipequantidade > 0
                INNER JOIN par3.escola esc ON esc.escid = ipe.escid
                INNER JOIN territorios.municipio mun ON mun.muncod = esc.muncod
                LEFT JOIN par3.execucao_contratoitens eci ON eci.aicid = aic.aicid
                LEFT JOIN par3.execucao_monitoramento_escola eme ON eme.ipeid = ipe.ipeid AND eme.eciid = eci.eciid 
                WHERE ppcstatus = 'A'
                  AND aicqtdaprovado > 0
                  AND pro.proid = {$proid}
                GROUP BY aic.ipiid, itcdsc";
        return $sql;
    }

    public function retornaSqlLocalizacao($proid, $inuid){
        $sql = "SELECT escno_localizacao  as codigo, escno_localizacao as descricao
                FROM par3.iniciativa_planejamento_item_composicao ipi
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = ipi.inpid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                INNER JOIN par3.processo pro ON pro.proid = ppc.proid
                INNER JOIN par3.iniciativa_planejamento_item_composicao_escola ipe ON ipe.ipiid = ipi.ipiid AND ipestatus = 'A' AND ipequantidade > 0
                INNER JOIN par3.escola esc ON esc.escid = ipe.escid
                INNER JOIN territorios.municipio mun ON mun.muncod = esc.muncod
                LEFT JOIN par3.execucao_contratoitens eci ON eci.aicid = aic.aicid
                LEFT JOIN par3.execucao_monitoramento_escola eme ON eme.ipeid = ipe.ipeid AND eme.eciid = eci.eciid 
                WHERE ppcstatus = 'A'
                  AND aicqtdaprovado > 0
                  AND pro.proid = {$proid}
                GROUP BY escno_localizacao";
        return $sql;
    }
}
