<?php

class Validador extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.validador";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("vdrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(

        'vdrid' => 'null',
        'vdrfuncao' => 'null',
        'vdrdescricao' => 'null',
        'vdrstatus' => 'null',
        'vdrdatainclusao' => 'null',
        'vdrobrigatorio' => 'null'
    );

    protected $_message = '';

    public function getMessage()
    {
        return $this->vdrdescricao;
    }

    public function check($obra, $vdrid = null, $extra = false, $iDConstrutoraExtra = null)
    {
        if ($vdrid) {
            $this->carregarPorId($vdrid);
        }
        
        if( $extra )
        {
        	if($vdrid == '29')
        	{
        		return $this->questaoMedicaoExtra($obra, $iDConstrutoraExtra);
        	}
        	else if($vdrid == '31')
        	{
        		return $this->questaoNotasFiscaisExtra($obra, $iDConstrutoraExtra);
        	} 
        }
        else 
        {
        	return $this->{$this->vdrfuncao}($obra);
        }
    }

    public function notasFiscais($obra)
    {
        $sql = "
            SELECT
              med.obrid
            FROM
              obras2.medicoes med
            INNER JOIN
              obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
            INNER JOIN
              obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
            INNER JOIN
              obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid AND ntf.ntfstatus = 'A'
            WHERE med.obrid = $obra->obrid AND med.medstatus = 'A'
            GROUP BY med.obrid
        ";
        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    public function comprovantesPagamentos($obra)
    {
        $sql = "
            SELECT
              med.obrid
            FROM
              obras2.medicoes med
            INNER JOIN
              obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
            INNER JOIN
              obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
            INNER JOIN
              obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid AND ntf.ntfstatus = 'A'
            INNER JOIN
              obras2.documentotransacao dot ON ntf.ntfid = dot.ntfid AND dot.dotstatus = 'A'
            INNER JOIN
              obras2.pagamentotransacao pgt ON dot.pgtid = pgt.pgtid AND pgt.pgtstatus = 'A'
            WHERE med.obrid = $obra->obrid AND med.medstatus = 'A'
            GROUP BY med.obrid
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * Homologação da licitação deve estar anexada na aba licitação ou na aba documentos com o tipo de arquivo "Homologação da Licitação"?
     * @param $obra->obrid
     */
    public function homologacaoLicitacaoAnexo($obra)
    {
        $sql = "
            SELECT
                o.obrid
            FROM obras2.obras o
            JOIN obras2.obralicitacao ol ON ol.obrid = o.obrid AND oblstatus = 'A'
            JOIN obras2.faselicitacao fl ON fl.licid = ol.licid AND fl.flcstatus = 'A' AND tflid = 9
            JOIN obras2.arquivolicitacao al ON al.flcid = fl.flcid
            WHERE o.obrid = $obra->obrid
        ";
        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * O contrato assinado deve estar anexado na aba contratação?
     * @param $obra->obrid
     */
    public function contratoAssinadoAnexo($obra)
    {
        if(obraMi($obra->obrid))
            return true;

        $sql = "
            SELECT
                c.arqid
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obra->obrid AND c.arqid IS NOT NULL
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * A ordem de serviços deve estar anexada na aba contratação?
     * @param $obra->obrid
     */
    public function ordemDeServicoAnexo($obra)
    {
        if(obraMi($obra->obrid))
            return true;

        $sql = "
            SELECT
                oc.arqidos
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obra->obrid
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * A planilha vencedora da licitação deve estar anexada na aba contratação?
     * @param $obra->obrid
     */
    public function planilhaVencedoraAnexo($obra)
    {
        if(obraMi($obra->obrid))
            return true;

        $sql = "
            SELECT
                oc.arqidcusto
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obra->obrid AND oc.arqidcusto IS NOT NULL ORDER BY oc.ocrid DESC LIMIT 1
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * A rescisão contratual deve estar anexada na vistoria de paralisação que informa a rescisão contratual?
     * @param $obra->obrid
     */
    public function rescisaoContraturalAnexo($obra)
    {
        $sql = "
                SELECT
                  h.arqid
                FROM obras2.supervisao s
                INNER JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                INNER JOIN obras2.historicoparalisacao h ON h.supidparalisacao = s.supid
                WHERE
                    s.obrid = $obra->obrid
                    AND s.emsid IS NULL
                    AND s.smiid IS NULL
                    AND s.supstatus = 'A'::bpchar
                    AND s.validadapelosupervisorunidade = 'S'::bpchar
                    AND s.usucpf IS NOT NULL AND s.rsuid = 1
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY
                supdata DESC LIMIT 1
            ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * O boletim de medição acumulada deve ser anexo na vistoria de paralisação que informa a rescisão contratual?
     * @param $obra->obrid
     */
    public function boletimMedicaoAcumuladaAnexo($obra)
    {

        $sql = "
                SELECT
                  h.arqidboletim
                FROM obras2.supervisao s
                INNER JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                INNER JOIN obras2.historicoparalisacao h ON h.supidparalisacao = s.supid
                WHERE
                    s.obrid = $obra->obrid
                    AND s.emsid IS NULL
                    AND s.smiid IS NULL
                    AND s.supstatus = 'A'::bpchar
                    AND s.validadapelosupervisorunidade = 'S'::bpchar
                    AND s.usucpf IS NOT NULL AND s.rsuid = 1
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY
                supdata DESC LIMIT 1
            ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * ART de execução deve ser anexada na aba vistoria?
     * @param $obra->obrid
     */
    public function ARTExecucaoAnexo($obra)
    {
        $sql = "
            SELECT obrid FROM obras2.obras_arquivos  WHERE obrid = $obra->obrid AND tpaid = 25;
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * ART de fiscalização deve ser anexada na aba documentos?
     * @param $obra->obrid
     */
    public function ARTFiscalizacaoAnexo($obra)
    {
        $sql = "
            SELECT obrid FROM obras2.obras_arquivos  WHERE obrid = $obra->obrid AND tpaid = 26;
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * o contrato com a construtora está vigente?
     * @param $obra->obrid
     * @return bool
     */
    public function contratoVigente($obra)
    {
        if(obraMi($obra->obrid))
            return true;

        $crtid    = $obra->pegaContratoPorObra( $obra->obrid );
        $contrato = new Contrato( $crtid );
        $dados    = $contrato->getDados();

        $dateT = new DateTime($dados['crtdttermino']);
        $date = new DateTime();
        return ($dateT > $date) ? true : false;
    }

    /**
     * Os campos da Ordem de Serviço estão preenchidos?
     * @param $obra->obrid
     * @return bool
     */
    public function dadosOrdemDeServico($obra)
    {
        if(obraMi($obra->obrid))
            return true;

        $sql = "
            SELECT
                oc.*
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obra->obrid ORDER BY oc.ocrid DESC LIMIT 1
        ";

        $result = $this->pegaLinha($sql);

        if(empty($result['ocrdtordemservico'])) return false;
        if(empty($result['ocrdtinicioexecucao'])) return false;
        if(empty($result['ocrcustounitario'])) return false;
        if(empty($result['ocrdtterminoexecucao'])) return false;
        if(empty($result['arqidos'])) return false;
        if(empty($result['ocrvalorexecucao'])) return false;
//        if(empty($result['umdid'])) return false;
        if(empty($result['ocrcustounitario'])) return false;
        if(empty($result['ocrpercentualdbi'])) return false;
        if(empty($result['arqidcusto'])) return false;

        return true;
    }

    /**
     * O campo "Valor do contrato e Valor contratado", estão preenchidos com o mesmo valor?
     * @param $obra->obrid
     * @return bool
     */
    public function valorDoContratoValorContratado($obra)
    {
        if(obraMi($obra->obrid))
            return true;

        $sql = "
            SELECT
                c.crtvalorexecucao,
                oc.ocrvalorexecucao
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obra->obrid
        ";

        $result = $this->pegaLinha($sql);

        return ($result['crtvalorexecucao'] = $result['ocrvalorexecucao']) ? true : false;
    }

    /**
     * O campo "Valor Restante" esta zerado?
     * Se a obra não tiver nenhum desembolso (situação efetivado ou solicitado) permitir que o município encaminhe a solicitação, pois se trata da 1ª parcela que deverá ocorrer por ocasião da emissão da OS.
     *
     * @param $obra->obrid
     * @return bool
     */
    public function valorContratadoEValorCronograma($obra)
    {
        if(obraMi($obra->obrid))
            return true;

        $sql = "
            SELECT
                SUM(ico.icovlritem)
            FROM obras2.cronograma c
            JOIN obras2.itenscomposicaoobra ico ON ico.obrid = c.obrid AND ico.icostatus = 'A' AND c.croid = ico.croid AND ico.relativoedificacao = 'D'
            JOIN  obras2.itenscomposicao i ON i.itcid = ico.itcid AND i.itcstatus = 'A'
            WHERE c.obrid = $obra->obrid AND c.crostatus = 'A'
        ";

        $icovlritem = $this->pegaUm($sql);

        /*
        Se a obra não tiver nenhum desembolso (situação efetivado ou solicitado) permitir que o município encaminhe a solicitação,
        pois se trata da 1ª parcela que deverá ocorrer por ocasião da emissão da OS.
        */

        $repasse = $obra->pegaUm("
            SELECT
                o.obrid
            FROM obras2.obras o
            WHERE o.obridpai IS NULL AND tooid != 2 AND o.obrstatus = 'A' AND o.obrid NOT IN (

                SELECT DISTINCT o.obrid FROM obras2.obras o
                JOIN par.pagamentoobra po ON po.preid = o.preid
                JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) <> 'CANCELADO'::text
                WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
                UNION
                SELECT DISTINCT o.obrid FROM obras2.obras o
                JOIN par.pagamentoobrapar po ON po.preid = o.preid
                JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) <> 'CANCELADO'::text
                WHERE o.obridpai IS NULL AND o.obrstatus = 'A'

            ) AND o.obrid = $obra->obrid
        ");

        if($repasse){
            return true;
        }

        $obraContrato = new ObrasContrato();
        $ocrvalorexecucao = $obraContrato->getValorContrato($obra->obrid);

        return ($ocrvalorexecucao == $icovlritem) ? true : false;
    }

    /**
     * A última vistoria possui fotos em quantidade adequada?
     * Quando a tipologia da obra for quadra e cobertura => igual ou maior que 6 fotos
     * Quando for as demais tipologias de obras => igual ou maior do que 12 fotos
     *
     * @param $obra->obrid
     * @return bool
     */
    public function qtdFotosUltVistoria($obra)
    {
        $supervisao = new Supervisao();
        $supData = $supervisao->pegaUltimaVistoriaUnidade($obra->obrid);
        $total = 0;

        if($supData['supid']) {
            $sql = "SELECT
                			fot.*, arq.arqdescricao , c.itcdesc
                		FROM
                			obras2.fotos AS fot
                        LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
                        LEFT JOIN obras2.itenscomposicaoobra i ON i.icoid = fot.icoid
                        LEFT JOIN obras2.itenscomposicao c ON c.itcid = i.itcid
                        WHERE
                        	fot.obrid =" . $obra->obrid . " AND
                        	supid=" . $supData['supid'] . "
                        ORDER BY fotordem;";

            $fotos = $this->carregar($sql);
            $total = count($fotos);
        }

        $empreendimento = new Empreendimento($obra->empid);
        $arrTpoQuadraCobertura = array(18, 21, 17, 22, 20, 19);

        if(in_array($empreendimento->tpoid, $arrTpoQuadraCobertura)){
            $this->vdrdescricao = 'Inserir vistoria com 6 fotos, no mínimo, em ângulos que permitam a verificação pelo FNDE dos itens executados citados na vistoria, além de uma foto geral da obra. Também nessa vistoria, no "relatório técnico de acompanhamento" comentários sobre cada item executado e a situação geral da obra.';
            return ($total >= 6) ? true : false;
        } else {
            $this->vdrdescricao = 'Inserir vistoria com 12 fotos, no mínimo, em ângulos que permitam a verificação pelo FNDE dos itens executados citados na vistoria, além de uma foto geral da obra. Também nessa vistoria, no "relatório técnico de acompanhamento" comentários sobre cada item executado e a situação geral da obra.';
            return ($total >= 12) ? true : false;
        }

    }

    /**
     * o percentual de execução da obra, informado na última vistoria, está maior ou igual ao valor desembolsado pelo FNDE?
     *
     * b.	Considerar como desembolso FNDE somente a situação 2-EFETIVADO;
     * c.	A obra tem que estar com % de execução maior do que 0%
     * d.	OU caso a obra esteja com 0% de execução, não deve existir valor desembolsado pelo FNDE na situação 2-EFETIVADO
     *
     * @param $obra->obrid
     * @return bool
     */
    public function percUltVistoriaEDesembolso($obra)
    {
        $sql = "
                SELECT
                  totalpago, vlrobra, (totalpago*100) / vlrobra as percentual
                FROM (
                    SELECT DISTINCT pre.preid,
                        obra.obrid,
                        pre.preesfera AS esfera,
                        pre.estuf AS uf,
                        pre.muncod,
                        pre.predescricao AS nomeobra,
                        pre.prevalorobra AS vlrobra,
                        sum(pop.popvalorpagamento) AS totalpago,
                        'PAR'::text AS tipopagamento
                       FROM obras.preobra pre
                         JOIN par.pagamentoobrapar pop ON pop.preid = pre.preid
                         JOIN par.pagamento pag ON pop.pagid = pag.pagid AND pag.pagstatus = 'A'::bpchar AND pag.pagsituacaopagamento::text = '2 - EFETIVADO'::text
                         LEFT JOIN obras2.obras obra ON pre.preid = obra.preid
                      WHERE pre.prestatus = 'A'::bpchar
                      GROUP BY pre.preid, obra.obrid, pre.preesfera, pre.estuf, pre.muncod, pre.predescricao, pre.prevalorobra
                    UNION ALL
                     SELECT DISTINCT pre.preid,
                        obra.obrid,
                        pre.preesfera AS esfera,
                        pre.estuf AS uf,
                        pre.muncod,
                        pre.predescricao AS nomeobra,
                        pre.prevalorobra AS vlrobra,
                        sum(pop.pobvalorpagamento) AS totalpago,
                        'PAC'::text AS tipopagamento
                       FROM obras.preobra pre
                         JOIN par.pagamentoobra pop ON pop.preid = pre.preid
                         JOIN par.pagamento pag ON pop.pagid = pag.pagid AND pag.pagstatus = 'A'::bpchar AND pag.pagsituacaopagamento::text = '2 - EFETIVADO'::text
                         LEFT JOIN obras2.obras obra ON pre.preid = obra.preid
                      WHERE pre.prestatus = 'A'::bpchar
                      GROUP BY pre.preid, obra.obrid, pre.preesfera, pre.estuf, pre.muncod, pre.predescricao, pre.prevalorobra
                ) as pg
                WHERE pg.obrid = $obra->obrid
        ";

        $pagamento = $this->pegaLinha($sql);
        $perc = $obra->pegaPercentualExecucao($obra->obrid);

        if ($perc > 0 || $pagamento['percentual'] <> 0) {
            if ($perc >= $pagamento['percentual']) {
                return true;
            }
        }

        return false;
    }

    /**
     * As restrições já foram superadas, ou encaminhadas para análise do FNDE?
     * a.	Mesmo que a restrição não esteja superada, desde que o município tenha encaminhado a solicitação ao FNDE (que não analisou), deve ser permitido a remessa do pedido de desembolso.
     * @param $obra->obrid
     * @return bool
     */
    public function restricoesNaoSuperadas($obra)
    {
        $sql = "
            SELECT
                r.obrid
            FROM obras2.restricao r
            JOIN workflow.documento d ON d.docid = r.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid NOT IN (1142, 1143, 1497, 1141)
            WHERE r.obrid = $obra->obrid AND rstitem IN ('R') AND rststatus = 'A'

        ";

        $result = $this->pegaUm($sql);
        return (!$result) ? true : false;
    }

    public function vigenciaTermo($obra)
    {

        $sql = "
          SELECT
              o.dt_fim_vigencia_termo - '30 days'::interval >= now()
            FROM obras2.v_vigencia_obra_2016 o
            WHERE
              o.obrid = $obra->obrid"
              ;

        $result = $this->pegaUm($sql);

        if ($result == 'f') {
            return $this->validaPedidoProrrogacao($obra);
        } else {
            return true;
        }

    }


    public function validaPedidoProrrogacao($obra)
    {

        //\www\par\_constantes.php
        //Códigos do arquivo de _constantes do PAR
        //1674 define('ESDID_PRORROGACAO_PRAZO_REPROVADA', 1674);
        //1677 define('ESDID_PRORROGACAO_PRAZO_FINALIZADA', 1677);

             $sql = "SELECT
                          spp.sppid
                        FROM
                          obras.preobra pre
                        INNER JOIN
                          par.processoobrasparcomposicao poc ON pre.preid = poc.preid AND poc.pocstatus = 'A'
                        INNER JOIN
                          par.processoobraspar pop ON poc.proid = pop.proid AND pop.prostatus = 'A'
                        INNER JOIN
                          par.documentopar dp ON pop.proid = dp.proid AND dp.dopstatus = 'A'
                        INNER JOIN
                          par.solicitacaoprorrogacaoprazoobra spp ON dp.dopid = spp.dopid AND spp.proid NOTNULL
                        INNER JOIN
                          workflow.documento doc ON spp.docid = doc.docid
                        WHERE pre.preid = $obra->preid AND doc.esdid NOT IN (1674,1677)
                        
                        UNION ALL
                        
                        SELECT
                          spp.sppid
                        FROM
                          obras.preobra pre
                        INNER JOIN
                          par.processoobraspaccomposicao poc ON pre.preid = poc.preid AND poc.pocstatus = 'A'
                        INNER JOIN
                          par.solicitacaoprorrogacaoprazoobra spp ON poc.proid = spp.proid AND spp.terid NOTNULL
                        INNER JOIN
                          workflow.documento doc ON spp.docid = doc.docid
                        WHERE pre.preid = $obra->preid AND doc.esdid NOT IN (1674,1677)
                        " ;

        $result = $this->pegaUm($sql);

        if(!$result){
            return false;
        }else{
            return true;
        }
    }


    /**
     * O contrato com a construtora está vigente?
     * @param $obra->obrid
     */
    public function contratoAnexo($obra)
    {
        $sql = "
            SELECT
                c.arqidcontrato
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obra->obrid AND c.arqidcontrato IS NOT NULL
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    public function verificacaoGeralParPac($obra, $podidPARPP = array(), $podidPARAR = array(), $podidPACPP = array())
    {
        $sql = "SELECT preid, tpoid, tooid FROM obras2.obras WHERE obrid = $obra->obrid AND obrstatus = 'A'";
        $res = $this->pegaLinha($sql);
        if(!$res['preid']) {
            return 'n';
        }

        $preObra = $this->buscaPtoid($res['preid']);
        $ptoid = $preObra['ptoid'];
        if ($preObra['ptoprojetofnde'] == 't') {
            return 'n';
        }

        if ($ptoid == '16' || $ptoid == '17' || $ptoid == '18' || $ptoid == '56' || $ptoid == '64' || $ptoid == '65'|| $ptoid == '66') {
            //Verificação Projeto Proprio PAR
            if($ptoid == '16' || $ptoid == '56') {
                if(count($podidPARPP) == 0) {
                    return 'n';
                }
                $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARPP);
                $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARPP);
                $resposta = $result == count($podidPARPP) ? 't' : 'f';
                return array('resposta' => $resposta, 'arquivos' => $arquivos);
            }
            if(count($podidPARAR) == 0) {
                return 'n';
            }
            //Ampliação e Reforma
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARAR);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARAR);
            $resposta = $result == count($podidPARAR) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);

        } else if ($ptoid == '1' || $ptoid == '21' || $ptoid == '9') {
            //Projeto Proprio PAC
            if(count($podidPACPP) == 0) {
                return 'n';
            }
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPACPP);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPACPP);
            $resposta = $result == count($podidPACPP) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);
        } else return 'n';
    }

    public function buscaPtoid($preid)
    {
        $sql = <<<DML
            SELECT
                po.ptoid,
                pto.ptoprojetofnde,
                po.tooid
            FROM obras.preobra po
            JOIN obras.pretipoobra pto ON (po.ptoid = pto.ptoid)
            WHERE po.preid = $preid
DML;
        return $this->pegaLinha($sql);
    }

    public function buscaPreObraAnexo($preid,$podid)
    {
        $in = implode(', ', $podid);
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras.preobraanexo anx
            INNER JOIN public.arquivo arq ON arq.arqid = anx.arqid AND anx.poasituacao = 'A'
            WHERE anx.preid = $preid
                AND anx.podid IN ($in)
                AND anx.poasituacao = 'A'
DML;
        $resp = $this->carregar($sql);
        return $resp ? $resp : array();
    }

    public function verificaQuantidadePreObra($preid,$podid)
    {
        $in = implode(', ', $podid);
        $sql = <<<DML
            SELECT
                COUNT(DISTINCT anx.podid)
            FROM obras.preobraanexo anx
            INNER JOIN public.arquivo arq ON arq.arqid = anx.arqid AND anx.poasituacao = 'A'
            WHERE anx.preid = $preid
                AND anx.podid IN ($in)
                AND anx.poasituacao = 'A'
DML;
        return $this->pegaUm($sql);
    }

    public function verificaEstadoPreenchimentoSupervisao($obrid)
    {
        $sql = <<<DML
            SELECT distinct
                s.staid,
                ( SELECT
                    CASE
                    WHEN SUM(icovlritem) > 0 THEN
                        ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                    ELSE  0
                    END AS total
                FROM obras2.cronograma cro
                JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid
                LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid
                    AND sic.supid = s.supid
                    AND sic.icoid IS NOT NULL
                    AND sic.ditid IS NULL
                JOIN obras2.supervisao su ON su.supid = sic.supid
                WHERE i.icostatus = 'A'
                    AND i.relativoedificacao = 'D'
                    AND i.obrid = s.obrid
                    AND i.croid = s.croid
                    AND cro.crostatus IN ('A', 'H')
                    AND su.croid = cro.croid
                ) as percentual,
                s.supdata,
                s.supdtinclusao
            FROM obras2.supervisao s
            LEFT JOIN obras2.cronograma cro ON s.croid = cro.croid
            WHERE s.obrid = '$obrid'
                AND s.emsid IS NULL
                AND s.smiid IS NULL
                AND s.supstatus = 'A'
                AND validadaPeloSupervisorUnidade = 'S'
            ORDER BY s.supdata DESC, s.supdtinclusao DESC
            LIMIT 1
DML;
        $linha = $this->pegaLinha($sql);
        if(is_array($linha)) {
            if($linha['staid'] != 3 && $linha['percentual'] != 100) {
                return true;
            }
        }
        if(!$linha) {
            return true;
        }
        return false;
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 1) O Projeto Pactuado está inserido?
     * Possui arquivos.
     */
    public function questaoProjetoPactuado($obra)
    {
        #Exceção para Obras Acabadas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        return $this->verificacaoGeralParPac($obra,array(13,14,15,16),array(13),array(13,14,15,16));
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 2) O Memorial Descritivo e Especificações Técnicas estão inseridas?
     */
    public function questaoMDET($obra)
    {
        #Exceção para Obras Acabadas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        return $this->verificacaoGeralParPac($obra,array(17),array(17),array(17));
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 3) A Planilha pactuada está inserida?
     */
    public function questaoPlanilhaPactuada($obra)
    {
        #Exceção para Obras Acabadas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'n';
        }
        #Fim Exceção.
        $saida = $this->verificacaoGeralParPac($obra,array(10),array(10),array(10));
        if($saida == 'n') {
            $sql = "SELECT preid FROM obras2.obras WHERE obrid = $obra->obrid AND obrstatus = 'A'";
            $res = $this->pegaUm($sql);
            if(!$res) {
                return 'f';
            }
        }
        return $saida;
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 4) A Vistória Final da obra concluída está inserida?
     * Não necessita devolver arquivos.
     */
    public function questaoVistoriaFObraC($obra)
    {
        #Exceção para Obras Acabadas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';

        $sql = <<<DML
            SELECT
                s.staid
            FROM obras2.supervisao s
            WHERE s.obrid = '{$obra->obrid}'
                AND s.emsid IS NULL
                AND s.smiid IS NULL
                AND s.supstatus = 'A'
                AND validadaPeloSupervisorUnidade = 'S'
                AND s.usucpf IS NOT NULL
            ORDER BY s.supdata DESC, s.supid DESC
            LIMIT 1
DML;
        return ($this->pegaUm($sql) == '3') ? 't' : ($esdid == ESDID_OBJ_INACABADA ? 'n' : 'f');
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 5) O Termo de Recebimento Definitivo da Obra está inserido?
     * Possui Arquivos.
     */
    public function questaoTermoRecebimentoDO($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'n';
        }
        #Fim Exceção.
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.supervisao s
            INNER JOIN public.arquivo arq ON (s.arqidtermo = arq.arqid)
            WHERE s.obrid = '{$obra->obrid}'
                AND s.emsid IS NULL
                AND s.smiid IS NULL
                AND s.supstatus = 'A'
            ORDER BY s.supdata DESC, s.supdtinclusao DESC
            LIMIT 1
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 6) Existem restrições e inconformidades da obra?
     * Não necessita devolver arquivos.
     */
    public function questaoRestricoesInconformidades($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = {$obra->obrid})");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) return 'n';
        #Fim Exceção.
        $sql = <<<DML
            SELECT count(*) AS res
            FROM obras2.restricao r
            INNER JOIN workflow.documento doc ON doc.docid = r.docid
            INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE r.rststatus = 'A'
                AND r.obrid IN ({$obra->obrid})
                AND esd.esdid NOT IN (1141,1497,1142,1143)
                --Exceção
                --AND NOT (r.fsrid = 7 AND r.tprid = 16 AND r.rstdsc = 'Aguardando preenchimento do Cumprimento do Objeto.' AND r.usucpf = '00000000191')
DML;
        $res = $this->pegaUm($sql);
        return ($res) ? 't' : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 7) O Relatório de Cumprimento do Objeto está inserido?
     * ******** Não necessita de validador ********
     */
    public function questaoRelatorioCumprimento($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'n';
        }
        #Fim Exceção.
        //resposta default
        return 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * #Não pode retornar Não se aplica
     * Item Composição: Documentos da Obra.
     * 8) O Comprovante de dominialidade definitivo está inserido?
     */
    public function questaoComprovanteDominialidade($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'f';
        }
        #Fim Exceção.
        $sql = "
            SELECT obr.preid, obr.tpoid, obr.tooid, emp.empesfera
            FROM obras2.obras obr
            INNER JOIN obras2.empreendimento emp ON (obr.empid = emp.empid)
            WHERE obrid = $obra->obrid
                AND obrstatus = 'A'
            ";
        $res = $this->pegaLinha($sql);
        if(!$res['preid']) {
            return 'f';
        }
        $podidPACFNDE = array(9);
        $podidPARFNDE = array(21);
        $podidPARPP = array(21);
        $podidPARAR = array(21);
        $podidPACPP = array(19);

        $preObra = $this->buscaPtoid($res['preid']);
        $ptoid = $preObra['ptoid'];
        if ($res['tooid'] == '1' && $preObra['ptoprojetofnde'] == 't') {
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPACFNDE);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPACFNDE);
            $resposta = $result == count($podidPACFNDE) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);
        } else if ($preObra['ptoprojetofnde'] == 't'){
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARFNDE);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARFNDE);
            $resposta = $result == count($podidPARFNDE) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);
        } else {
            if ($ptoid == '16' || $ptoid == '17' || $ptoid == '18' || $ptoid == '56' || $ptoid == '64' || $ptoid == '65'|| $ptoid == '66') {
                //Verificação Projeto Proprio PAR
                if($ptoid == '16' || $ptoid == '56') {
                    $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARPP);
                    $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARPP);
                    $resposta = $result == count($podidPARPP) ? 't' : 'f';
                    return array('resposta' => $resposta, 'arquivos' => $arquivos);
                }
                //Ampliação e Reforma
                $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARAR);
                $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARAR);
                $resposta = $result == count($podidPARAR) ? 't' : 'f';
                return array('resposta' => $resposta, 'arquivos' => $arquivos);

            } else if (($ptoid == '1' || $ptoid == '21' || $ptoid == '9') && $res['empesfera'] == 'E') {
                //Projeto Proprio PAC
                $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPACPP);
                $result = $this->verificaQuantidadePreObra($res['preid'], $podidPACPP);
                $resposta = $result == count($podidPACPP) ? 't' : 'f';
                return array('resposta' => $resposta, 'arquivos' => $arquivos);
            }
        }
        return 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * #Não pode retornar Não se aplica
     * Item Composição: Documentos da Obra.
     * 9) Houve devolução de recursos na GRU?
     */
    public function questaoDevolucaoGRU($obra)
    {
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.obras_arquivos oa
            JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
            JOIN public.arquivo    arq ON arq.arqid = oa.arqid
            WHERE oa.oarstatus = 'A'
                AND oa.obrid = '{$obra->obrid}'
                AND (arq.arqtipo != 'image/jpeg'
                    AND arq.arqtipo != 'image/gif'
                    AND arq.arqtipo != 'image/png')
                AND (ta.tpaid = 32 OR oa.oardesc ilike ('%gru%'))
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * #Não pode retornar Não se aplica
     * Item Composição: Documentos da Obra.
     * 10) O Plano de Trabalho está inserido?
     */
    public function questaoPlanoTrabalho($obra)
    {
        #Não se aplica para obra sem termo de comprmisso.
        if($obra->preid) {
            $dopid = $this->pegaUm("SELECT d.dopid FROM par.termocomposicao t INNER JOIN par.documentopar d on d.dopid = t.dopid WHERE t.preid = {$obra->preid} AND d.dopstatus = 'A'");
            if (!$dopid) {
                return 'n';
            }
        }
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'n';
        }
        #Fim Exceção.

        $tooid = $this->pegaUm("SELECT tooid FROM obras2.obras WHERE obrid = {$obra->obrid} AND obrstatus = 'A' AND obridpai IS NULL");

        return $tooid == '2' ? 'f' : 'n';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 1) O Contrato Principal da Obra está inserido?
     * Possui arquivos.
     */
    public function questaoContratoPrincipalObra($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'f';
        }
        #Fim Exceção.
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.contrato cr
            INNER JOIN public.arquivo arq ON (cr.arqidcontrato = arq.arqid)
            WHERE cr.crtid =
                (SELECT DISTINCT crtid
                FROM obras2.obrascontrato
                WHERE ocrstatus = 'A'
                    AND obrid = $obra->obrid LIMIT 1)
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 2) A Planilha de Medições Acumuladas Consolidadas está inserida?
     * Possui arquivos.
     * * //  execucao alterar esta requisição para trazer da nova tabela
     */
    public function questaoPlanilhaMedicaoAcumulada($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'f';
        }
        
        $resultadoIds = $this->retornaIdsContrato($obra->obrid);
        
        #Fim Exceção.
      	if($resultadoIds != '' )
        {
        	$sql = "
        			select 
						arq.arqid,
						arq.arqdescricao as arqnome,
						arq.arqextensao 
						from obras2.medicoes med
						INNER JOIN arquivo arq on arq.arqid = med.arqid
					where crtid in
						(
							SELECT
								crt.crtid
								
							FROM 
								obras2.contrato crt
								
							WHERE 
							(
								crt.crtid  in ( {$resultadoIds} )
								or
								(crt.crtidpai = (select crtidpai from obras2.contrato where crtid in ( {$resultadoIds} ) order by crtid limit 1) AND ttaid in (2,3))
							)
						)
						AND medstatus = 'A'
			";
        	
        	$arquivo = $this->carregar($sql);
        	return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
        }
        else
        {
        	return 'f';
        }
    }
    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 2) A Planilha de Medições Acumuladas Consolidadas está inserida?
     * Possui arquivos.
     * * //  execucao alterar esta requisição para trazer da nova tabela
     * @todo muda carai
     */
    public function questaoMedicaoExtra($obra, $iDConstrutoraExtra = null)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'n';
        }
        
        $resultadoIds = $this->retornaIdsContratoExtra($obra->obrid, $iDConstrutoraExtra);
        
        #Fim Exceção.
      	if($resultadoIds != '' )
        {
        	$sql = "
        			select 
						arq.arqid,
						arq.arqdescricao as arqnome,
						arq.arqextensao 
						from obras2.medicoes med
						INNER JOIN arquivo arq on arq.arqid = med.arqid
					where cceid in
		    		(
		    			{$resultadoIds}
		    		)
		    		AND medstatus = 'A'
			";
        	
        	$arquivo = $this->carregar($sql);
        	return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
        }
        else
        {
        	return 'f';
        }
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 3) A Planilha Contratada está inserida?
     * Possui arquivos. Não pode retornar Não se Aplica
     */
    public function questaoPlanilhaContratada($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'f';
        }
        #Fim Exceção.
        $sql = <<<DML
            SELECT true FROM obras2.obras WHERE obrid = $obra->obrid AND tpoid IN (104,105)
DML;
        if($this->pegaUm($sql)){
            return 'f';
        }
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.obrascontrato oc
            JOIN obras2.obras o ON o.obrid = oc.obrid AND o.obrstatus = 'A'
            INNER JOIN public.arquivo arq ON (oc.arqidcusto = arq.arqid)
            WHERE ocrstatus = 'A'
                AND o.obrid = $obra->obrid
                AND oc.crtid = (SELECT DISTINCT crtid
                    FROM obras2.obrascontrato
                    WHERE ocrstatus = 'A'
                        AND obrid = $obra->obrid LIMIT 1)
            ORDER BY oc.ocrid
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }
	
    public function retornaIdsContrato( $obrid )
    {
    	if( $obrid )
    	{
	    	$resultadoIds = $this->pegaUm("
	    			SELECT
	    			array_to_string( array(
	    			SELECT
	    			CASE WHEN ttaid IS NULL THEN
	    			crt.crtid
	    			ELSE
	    			(SELECT c2.crtid  FROM obras2.contrato c2 WHERE c2.crtidpai = crt.crtid AND c2.ttaid is NULL AND c2.crtstatus = 'A' LIMIT 1)
	    			END as codigo
	    				
	    			FROM obras2.contrato crt
	    				
	    			WHERE
	    			crt.crtid in  (SELECT DISTINCT
	    			crtid
	    			FROM
	    			obras2.obrascontrato
	    			WHERE
	    			ocrstatus = 'A'
	    			AND obrid in ( {$obrid} ) )
	    			AND crtstatus = 'A'
	    				
	    	), ',' )
	    	");
	    	
	    	return $resultadoIds;
    	}
    	else
    	{
    		return '';
    	}
    }
    
    public function retornaIdsContratoExtra( $obrid, $iDConstrutoraExtra = null )
    {
    	if( $obrid && $iDConstrutoraExtra)
    	{
    		
    		$resultadoIds = $this->pegaUm("
    		SELECT
    				array_to_string( array(
    				SELECT
					DISTINCT cceid
    				FROM obras2.contratoconstrutoraextra cce
					INNER JOIN obras2.construtoraextra cex ON cex.cexid = cce.cexid AND cex.obrid = {$obrid}  AND cexstatus = 'A'
    	    		WHERE ccestatus = 'A' AND cce.cexid = {$iDConstrutoraExtra}
    		), ',' )
    		");
    
    		return $resultadoIds;
    		
    	}
    	else
    	{
    		return '';
    	}
    }
    
    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 4) As Notas Fiscais, relativas as Medições, estão inseridas?
     * Possui arquivos.
     * * //@todo execucao alterar esta requisição para trazer da nova tabela
     */
    public function questaoNotasFiscais($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'f';
        }
        //$obra->obrid
        #Fim Exceção.
       	$resultadoIds = $this->retornaIdsContrato($obra->obrid);
       	
        if($resultadoIds != '' )
        {
        	$sql = "
        			select 
						arq.arqid,
						arq.arqdescricao as arqnome,
						arq.arqextensao 
						from obras2.notafiscal ntf
						INNER JOIN arquivo arq on arq.arqid = ntf.arqid
					where crtid in
						(
							SELECT
								crt.crtid
								
							FROM 
								obras2.contrato crt
								
							WHERE 
							(
								crt.crtid  in ( {$resultadoIds} )
								or
								(crt.crtidpai = (select crtidpai from obras2.contrato where crtid in ( {$resultadoIds} ) order by crtid limit 1) AND ttaid in (2,3))
							)
						)
						AND ntfstatus = 'A'
			";
        	
        	$arquivo = $this->carregar($sql);
        	return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
        }
        else
        {
        	return 'f';
        }
        
        
    }
    
    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 4) As Notas Fiscais, relativas as Medições, estão inseridas?
     * Possui arquivos.
     * * //@todo execucao alterar esta requisição para trazer da nova tabela
     */
    public function questaoNotasFiscaisExtra($obra, $iDConstrutoraExtra = null)
    {
    	#Exceção para Obras Inacabadas e Canceladas.
    	$esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
    	#Não se aplica para obra cancelada.
    	if($esdid == ESDID_OBJ_CANCELADO) return 'n';
    	#Não se aplica para obra inacabada e ultima vistoria incompleta.
    	if($esdid == ESDID_OBJ_INACABADA) {
    		$retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
    		if($retorno) return 'n';
    	}
    	//$obra->obrid
    	#Fim Exceção.
    	$resultadoIds = $this->retornaIdsContratoExtra($obra->obrid, $iDConstrutoraExtra);
    
    	if($resultadoIds != '' )
    	{
    		$sql = "
    		select
    		arq.arqid,
    		arq.arqdescricao as arqnome,
    		arq.arqextensao
    		from obras2.notafiscal ntf
    		INNER JOIN arquivo arq on arq.arqid = ntf.arqid
    		where cceid in
    		(
    			{$resultadoIds}
    		)
    		AND ntfstatus = 'A'
    		";
    		 
    		$arquivo = $this->carregar($sql);
    		return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    	}
    	else
    	{
    		return 'f';
    	}
    
    
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 5) Os aditivos de valores estão inseridos?
     * Possui arquivos.
     */
    public function questaoAditivosValores($obra)
    {
        #Exceção para Obras Inacabadas e Canceladas.
        $esdid = $this->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = (SELECT docid FROM obras2.obras WHERE obrid = $obra->obrid)");
        #Não se aplica para obra cancelada.
        if($esdid == ESDID_OBJ_CANCELADO) return 'n';
        #Não se aplica para obra inacabada e ultima vistoria incompleta.
        if($esdid == ESDID_OBJ_INACABADA) {
            $retorno = $this->verificaEstadoPreenchimentoSupervisao($obra->obrid);
            if($retorno) return 'f';
        }
        #Fim Exceção.
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.contrato cr
            INNER JOIN public.arquivo arq ON (cr.arqid = arq.arqid)
            WHERE (cr.crtid = (SELECT DISTINCT crtid FROM obras2.obrascontrato WHERE ocrstatus = 'A' AND obrid = $obra->obrid LIMIT 1)
                    OR cr.crtidpai = (SELECT DISTINCT crtid FROM obras2.obrascontrato WHERE ocrstatus = 'A' AND obrid = $obra->obrid LIMIT 1))
                AND ttaid = 2;

DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }
}
