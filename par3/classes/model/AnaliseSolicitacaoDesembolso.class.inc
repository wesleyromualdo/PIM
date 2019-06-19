<?php

/**
 * Created by PhpStorm.
 * User: danielmarconi
 * Date: 10/05/2018
 * Time: 15:51
 */
class Par3_Model_AnaliseSolicitacaoDesembolso extends Modelo
{

    protected $stNomeTabela = 'par3.analise_solicitacao_desembolso';

    protected $arChavePrimaria = array(
        'asdid',
    );

    protected $arChaveEstrangeira = array(
        'sdpid' => array('tabela' => 'par3.solicitacao_desembolso_par', 'pk' => 'sdpid'),
        'sdo' => array('tabela' => 'par3.solicitacao_desembolso_obra', 'pk' => 'sdo'),
        'proid' => array('tabela' => 'par3.documentotermo', 'pk' => 'proid'),
    );

    protected $arAtributos = array(
        'asdid' => null,
        'sdpid' => null,
        'asdparecer' => null,
        'usucpf' => null,
        'asddtinclusao' => null,
        'asdstatus' => null,
        'asdvaloraprovado' => null,
    );

    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function montaSqlListaSolicitacoes($filtros){
        $where  = '';

        if($filtros['pronumeroprocesso']){
            $filtros['pronumeroprocesso'] = str_replace(".","", $filtros['pronumeroprocesso']);
            $filtros['pronumeroprocesso'] = str_replace("/","", $filtros['pronumeroprocesso']);
            $filtros['pronumeroprocesso'] = str_replace("-","", $filtros['pronumeroprocesso']);
            $where = " AND pronumeroprocesso ilike '%".$filtros['pronumeroprocesso']."%'";
        }

        if(strlen( $filtros['dotnumero']) > 9){
            $filtros['dotnumero'] = substr($filtros['dotnumero'], 0,9);
        }

        if ($filtros['muncod'] && count($filtros['muncod']) != 0) {
            $arrMuncod = array_filter($filtros['muncod'], function ($val) {
                return !empty($val);
            });
            if (count($arrMuncod) != 0) {
                $where  .= " AND inu.muncod::integer in (".implode(',', $arrMuncod).")";
            }
        }

        $where .= $filtros['dotnumero'] ? " AND dotnumero::TEXT like '%{$filtros['dotnumero']}%'" : '';
        $where .= $filtros['estuf'] ? " AND inu.estuf like '%{$filtros['estuf']}%'" : '';
        $where .= $filtros['esfera'] == 'E' ? " AND inu.muncod IS NULL AND inu.estuf IS NOT NULL " : '';
        $where .= $filtros['esfera'] == 'M' ? " AND inu.muncod IS NOT NULL " : '';
        $where .= $filtros['esdid_analise'] ? " AND esd.esdid = {$filtros['esdid_analise']} " : '';
        $where .= $filtros['econumero'] ? " AND econumero::TEXT like '%{$filtros['econumero']}%' " : '';
        $where .= $filtros['sdpid'] ? " AND sdpid = {$filtros['sdpid']} " : '';
        $where .= $filtros['anoprocesso'] ? " AND substring(pronumeroprocesso,12,4)::int = {$filtros['anoprocesso']} " : '';

        /*if($filtros['pronumeroprocesso'] == '' && $filtros['dotnumero'] == '' && $filtros['sdpid']  == ''){
            $where .= $filtros['tipo'] == 'P' ? " AND dot.intoid <> 1 " : '';
            $where .= $filtros['tipo'] == 'O' ? " AND dot.intoid = 1 " : '';
        }*/

        if($filtros['sdpdtinicio'] && !$filtros['sdpdtfim']){
            $where .= " AND sdpdtinclusao >= '" . formata_data_sql($filtros['sdpdtinicio']) . "'";
        }else if(!$filtros['sdpdtinicio'] && $filtros['sdpdtfim']){
            $where .= " AND sdpdtinclusao <= '" . formata_data_sql($filtros['sdpdtfim']) . "'";
        }else if($filtros['sdpdtinicio'] && $filtros['sdpdtfim']){
            $where .= " AND sdpdtinclusao BETWEEN '" . formata_data_sql($filtros['sdpdtinicio']) . "' AND '" . formata_data_sql($filtros['sdpdtfim']) . "'";
        }

        if($filtros['ecodtinicio'] && !$filtros['ecodtfim']){
            $where .= " AND ecodtainclusao >= '" . formata_data_sql($filtros['ecodtinicio']) . "'";
        }else if(!$filtros['ecodtinicio'] && $filtros['ecodtfim']){
            $where .= " AND ecodtainclusao <= '" . formata_data_sql($filtros['ecodtfim']) . "'";
        }else if($filtros['ecodtinicio'] && $filtros['ecodtfim']){
            $where .= " AND ecodtainclusao BETWEEN '" . formata_data_sql($filtros['ecodtinicio']) . "' AND '" . formata_data_sql($filtros['ecodtfim']) . "'";
        }

        if($filtros['dotdatainiciovigencia'] && !$filtros['dotdatafimvigencia']){
            $where .= " AND dotdatafimvigencia >= '" . formata_data_sql($filtros['dotdatainiciovigencia']) . "'";
        }else if(!$filtros['dotdatainiciovigencia'] && $filtros['dotdatafimvigencia']){
            $where .= " AND dotdatafimvigencia <= '" . formata_data_sql($filtros['dotdatafimvigencia']) . "'";
        }else if($filtros['dotdatainiciovigencia'] && $filtros['dotdatafimvigencia']){
            $where .= " AND dotdatafimvigencia BETWEEN '" . formata_data_sql($filtros['dotdatainiciovigencia']) . "' AND '" . formata_data_sql($filtros['dotdatafimvigencia']) . "'";
        }

        if($filtros['contratosemsolicitacao'] == 'S'){
            $where .= ' AND sdpid IS NULL ';
            $joinContrato = ' eco.proid = pro.proid ';
        }else {
            $where .= ' AND sdpid IS NOT NULL ';
            $joinContrato = ' eco.ecoid = sdp.ecoid ';
        }

        $sql = "SELECT 
                  sdpid as solicitacao,
                  est.estuf,
                  coalesce(mun.mundescricao, ' - ') as mundescricao,
                  sdpid as codigoSolicitacao,
                  pronumeroprocesso,
                  dotnumero || '-' || dot.intoid as termoCompromisso,
                  to_char(dotdatainclusao, 'YYYY') as ano,
                  to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' - (' || (dot.dotdatafimvigencia::date - CURRENT_DATE) || ' dias)' as dotdatafimvigencia, -- Vigência,                  
                  CASE WHEN eco.ecoid IS NOT NULL THEN 'Contrato' ELSE doadescricao END as documento,
                  CASE WHEN eco.ecoid IS NOT NULL THEN ecocnpj ELSE edlcnpjfornecedor END as cnpjfornecedor,                
                  CASE WHEN eco.ecoid IS NOT NULL 
                    THEN ecovalor
                    ELSE edlvalordocumento
                  END as valordocumento,
                  sdpvalor,
                  vsaldo.vlrpago as pagoprocesso,
                  to_char(sdpdtinclusao, 'dd/mm/YYYY') as datasolicitacao,
                  esddsc,
                  CASE WHEN eco.ecoid IS NOT NULL 
                    THEN eco.arqid
                    ELSE edl.arqid
                  END as arqid,
                  esd.esdid,
                  pro.proid,
                  'par' AS tipo
                FROM par3.processo pro
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid
                INNER JOIN par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
                LEFT JOIN par3.solicitacao_desembolso_par sdp ON sdp.proid = pro.proid and sdpstatus = 'A'
                INNER JOIN par3.execucao_contrato eco ON {$joinContrato} AND ecostatus = 'A'
                LEFT JOIN par3.execucao_documentolicitacao edl ON edl.edlid = sdp.edlid AND edlstatus = 'A'
                LEFT JOIN par3.tipo_documento_anexo tda ON tda.doaid = edl.doaid
                LEFT JOIN workflow.documento doc ON doc.docid = sdp.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN seguranca.usuario usu ON usu.usucpf = sdp.usucpf
                LEFT JOIN par3.instrumentounidade inu ON inu.inuid = pro.inuid
                LEFT JOIN territorios.municipio mun ON mun.muncod = inu.muncod
                LEFT JOIN territorios.estado est ON est.estuf = inu.estuf              
                WHERE dotstatus = 'A'
                {$where}
                ORDER BY sdpid DESC              
                ";
//ver($sql);
        return $sql;
    }

    public function recuperarAnaliseSolicitacao($sdpid){
        $sql = "SELECT 
                        asdid,
                        doc.docid,
                        esdid,
                        asdparecer,
                        sdp.ecoid,
                        sdp.edlid,
                        sdp.sdpid,
                        asd.asdvaloraprovado
                FROM par3.solicitacao_desembolso_par sdp
                LEFT JOIN par3.analise_solicitacao_desembolso asd ON asd.sdpid = sdp.sdpid
                LEFT JOIN workflow.documento doc ON doc.docid = sdp.docid
                WHERE sdp.sdpid = {$sdpid}
                AND sdpstatus = 'A'";
        return $this->pegaLinha($sql);
    }

}