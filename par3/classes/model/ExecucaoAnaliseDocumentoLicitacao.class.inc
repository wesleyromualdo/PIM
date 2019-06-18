<?php

/**
 * Class Par3_Model_ExecucaoAnaliseDocumentoLicitacao
 *
 * Plano de trabalho / Execução e Acompanhamento / Acompanhamento
 */
class Par3_Model_ExecucaoAnaliseDocumentoLicitacao extends Modelo
{

    protected $stNomeTabela = 'par3.execucao_analise_documentolicitacao';

    protected $arChavePrimaria = array(
        'eadid',
    );

    protected $arChaveEstrangeira = array(
        'edlid' => array('tabela' => 'par3.execucao_documentoslicitacao', 'pk' => 'etrid'),
    );

    protected $arAtributos = array(
        'eadid' => null,
        'edlid' => null,
        'eadparecer' => null,
        'eadsituacao' => null,
        'eadstatus' => null,
        'eaddtainclusao' => null,
        'usucpfinclusao' => null,
        'eadproponente' => null,
        'eadproposta' => null,
        'eadprojeto' => null,
        'eadobjetivo' => null,
        'eadjustificativa' => null,
        'eadvalores' => null,
        'eadcabiveis' => null,
    );

    public function getCamposValidacao($dados = array())
    {
        return array(
            'eadid' => array('Digits'),
            'edlid' => array('Digits'),
            'eadparecer' => array(new Zend_Validate_StringLength(array('max' => 500))),
            'usucpfinclusao' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'eadstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
        );
    }

    public function antesSalvar()
    {

        $this->usucpfinclusao = $_SESSION['usucpf'];

        return parent::antesSalvar();
    }

    public function recuperarAnaliseDocumento($edlid)
    {
        $sql = "SELECT
                       edl.edlid,
                       edldsc,
                       arq.arqid,
                       arqnome,
                       arqextensao,
                       doaid,
                       esd.esdid,
                       edl.docid,
                       eadid,
                       eadsituacao,
                       eadparecer,
                       eadproponente,
                       eadproposta,
                       eadprojeto,
                       eadobjetivo,
                       eadjustificativa,
                       eadvalores,
                       eadcabiveis
                FROM par3.execucao_documentolicitacao edl
                LEFT JOIN par3.execucao_analise_documentolicitacao ead ON edl.edlid = ead.edlid AND eadstatus = 'A'
                LEFT JOIN workflow.documento doc ON doc.docid = edl.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN arquivo arq ON arq.arqid = edl.arqid
                WHERE
                edl.edlid = {$edlid}
                ";

        return $this->pegaLinha($sql);
    }


    public function montarSQLSimples($arrPost)
    {

        if ($arrPost['numeroprocesso']) {
            $processo = str_replace(array(".","/","-"), '', $arrPost['numeroprocesso']);
        }

        $where = '';
        $where .= $arrPost['numeroprocesso'] ? " AND pronumeroprocesso = '{$processo}'":'';
        $where .= $arrPost['dotnumero'] ? " AND numero_termo ilike '%{$arrPost['dotnumero']}%'":'';
        $where .= $arrPost['doaid'] ? " AND doa.doaid = {$arrPost['doaid']}":'';
        $where  .= $arrPost['estuf'] ? " AND iu.estuf = '{$arrPost['estuf']}'":'';
        $where  .= $arrPost['esdid_analise'] ? " AND esd.esdid = {$arrPost['esdid_analise']}":'';
        $where .= $arrPost['iniid'] ? ' AND ini.iniid = ' . $arrPost['iniid'] : '';
        $where .= $arrPost['intoid'] ? ' AND tpo.intoid = ' . $arrPost['intoid'] : '';

        if ($arrPost['muncod'] && count($arrPost['muncod']) != 0) {
            $arrMuncod = array_filter($arrPost['muncod'], function ($val) {
                return !empty($val);
            });
            if (count($arrMuncod) != 0) {
                $where  .= " AND iu.muncod::integer in (".implode(',', $arrMuncod).")";
            }
        }

        if ($arrPost['itrid'] == 1) {
            $where .= " AND iu.muncod IS NULL AND iu.estuf IS NOT NULL ";
        }
        if ($arrPost['itrid'] == 2) {
            $where .= " AND iu.muncod IS NOT NULL ";
        }

        if ($arrPost['analisado'] == 'S') {
            $where .= " AND eadid IS NOT NULL ";
        }
        if ($arrPost['analisado'] == 'N') {
            $where .= " AND eadid IS NULL ";
        }

        $sql = "SELECT DISTINCT ON(edl.edlid)
                    arq.arqid,
                    CASE WHEN doaanalise = TRUE
                      THEN '<span style=\'display:block; cursor:pointer;\' onclick=\'modalAnaliseDocumento('||edl.edlid||')\'><i class=\'fa fa-clipboard btn btn-lg\'></i></span>'
                      ELSE '-'
                    END as analise,
                    pro.pronumeroprocesso,
                    dot.dotnumero||'-'||dot.intoid,
                    est.estuf,
                    CASE WHEN mun.muncod is null THEN est.estdescricao ELSE mundescricao END as entidade,
                    doadescricao,
                    edldsc as descricao,
                    ini.iniid || ' - ' || de.inddsc as iniciativa,
                    to_char(edl.edldtainclusao, 'dd/mm/YYYY') as data,
                    COALESCE(qtd::text, '-') as qtditens,
                    COALESCE(valordoc::text, '-') as valor,
                    CASE WHEN doaanalise = TRUE
                      THEN esddsc
                      ELSE '-'
                    END AS situacao,
                    esd.esdid
                FROM par3.execucao_documentolicitacao edl
                INNER JOIN par3.processo pro ON pro.proid = edl.proid
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid
                INNER JOIN arquivo arq ON arq.arqid = edl.arqid
                INNER JOIN par3.tipo_documento_anexo doa ON doa.doaid = edl.doaid
                LEFT JOIN par3.execucao_documentolicitacao_itens edi ON edi.edlid = edl.edlid
                LEFT JOIN par3.analise_itemcomposicao aic ON aic.aicid = edi.aicid
                LEFT JOIN par3.execucao_analise_documentolicitacao ead ON ead.edlid = edl.edlid AND eadstatus = 'A'
                LEFT JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppcstatus = 'A'
                LEFT JOIN par3.iniciativa_planejamento ip ON ip.inpid = ppc.inpid and ip.inpstatus = 'A'
                LEFT JOIN par3.iniciativa ini ON ini.iniid = ip.iniid
                LEFT JOIN par3.iniciativa_descricao de ON de.indid = ini.indid
                LEFT JOIN par3.iniciativa_tipos_objeto tpo ON tpo.intoid = ppc.intoid
				INNER JOIN par3.instrumentounidade iu on iu.inuid = pro.inuid and iu.inustatus = 'A'
				LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
				LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                LEFT JOIN workflow.documento doc on doc.docid = edl.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN LATERAL (SELECT sum(t.aicqtdaprovado * t.aicvaloraprovado) as valordoc, sum(aicqtdaprovado) as qtd
                          FROM par3.analise_itemcomposicao t
                          INNER JOIN par3.execucao_documentolicitacao_itens t2 ON t2.aicid = t.aicid
                          WHERE t2.edlid = edl.edlid
                          ) v on true
                LEFT JOIN LATERAL (SELECT dot.dotnumero || '-'::text || dot.intoid AS numero_termo) numero on true
                WHERE
                edlstatus = 'A'
                AND doaanalise is true
                {$where}
                ORDER BY edl.edlid desc
         ";

        return $sql;
    }

    public function carregaItensComposicao($edlid)
    {
        $sql = "SELECT
                    itcdsc,
                    ppcano,
                    unidsc,
                    ipiquantidadeaprovada,
                    ipi.ipivalorreferencia,
                    ipi.ipivalorreferencia * ipiquantidadeaprovada as valortotal
                FROM par3.iniciativa_planejamento inp
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON inp.inpid = ipi.inpid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid 
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.unidade_medida uniid ON itc.uniid = uniid.uniid
                INNER JOIN par3.processo pro ON inp.inuid = pro.inuid
                INNER JOIN par3.execucao_documentolicitacao_itens edi ON edi.aicid = aic.aicid
                INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                LEFT JOIN par3.itemadesaosigarp ics ON itc.itcidsigarp = ics.item AND ics.proid = pro.proid and ics.adestatus = 'A'
                WHERE
                ipi.ipistatus = 'A'
                AND aicstatus = 'A'
                AND aicqtdaprovado > 0
                AND edi.edlid = {$edlid}
                GROUP BY aic.aicid, itcdsc , itc.itcespecificacao, unidsc, ppcano, ics.iasid, ediid, ipi.ipiquantidadeaprovada, ipi.ipivalorreferencia
                ORDER BY itcdsc";

        return $this->carregar($sql);
    }

    public function sqlFormListaIniciativas(){
        $sql = 'SELECT ini.iniid as codigo, ini.iniid || \' - \' || ind.inddsc as descricao
                FROM par3.iniciativa ini
                INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid
                WHERE indstatus = \'A\' AND inistatus = \'A\'
                ORDER BY ini.iniid
                ';
        return $sql;
    }

    public function sqlFormListaTipoObjeto(){
        $sql = 'SELECT intoid as codigo, intoid || \' - \' || intodsc as descricao
                FROM par3.iniciativa_tipos_objeto ini
                WHERE intostatus = \'A\'
                ORDER BY intoid
                ';
        return $sql;
    }
}
