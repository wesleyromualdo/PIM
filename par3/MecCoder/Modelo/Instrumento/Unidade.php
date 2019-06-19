<?php
namespace Simec\Par3\Modelo\Instrumento;

use Simec\Par3\Dado\Instrumento\Unidade as dadosInstrumentoUnidade;
use \Par3_Model_InstrumentoUnidade as modeloLegado;

class Unidade extends \Simec\AbstractModelo
{
    /**
     * @var dadosInstrumentoUnidade
     */
    public $dados;

    protected $modeloLegado;

    const ESDID_ETAPA_PREPARATORIA      = 1638;
    const ESDID_DIAGNOSTICO             = 1637;
    const ESDID_DIAGNOSTICO_FINALIZADO  = 1683;
    const ESDID_PLANEJAMENTO_N_INICIADO = 1874;
    const ESDID_ELABORACAO              = 1999;
    const ESDID_ENVIADO_ANALISE         = 2000;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        $this->modeloLegado = new modeloLegado();
        $this->dados        = new dadosInstrumentoUnidade();

        $this->carregarPorId($id, $tempocache);
    }

    public function carregarPorId($id, $tempocache = null)
    {
        $this->modeloLegado->carregarPorId($id, $tempocache);
        $this->dados->carregar($this->modeloLegado->getDados());

        return $this;
    }

    /**
     * - Monta as pendencias da Prestacao Contas.
     * 
     * ESTE MÉTODO EXISTE TAMBÉM EM: /SIMEC/par3/classes/controller/InstrumentoUnidade.class.inc
     * 
     * @param int $inuid
     * @return array Lista de pendencias.
     */
    public function pegarPendenciasPrestacaoContas($inuid)
    {
        $sql = "WITH wf_municipio_PAR3 AS
                (
                   SELECT
                      i.inuid as inuid_par,
                      iu.inuid as inuid_par3
                   FROM
                      par.instrumentounidade i
                      JOIN par3.instrumentounidade iu ON ((i.muncod = iu.muncod) AND (i.mun_estuf IS NOT NULL))
                         OR (((i.estuf = iu.estuf) AND (i.muncod IS NULL)) AND (iu.muncod IS NULL ))
                    where
    				   	iu.inuid = {$inuid}
                )
                select
                   *
                from
                   (
                      SELECT DISTINCT
                        prp.prpnumeroprocesso as processo,
                        dp.dopnumerodocumento::text as documento,
                        CASE WHEN to_date(dp.dopdatafimvigencia, 'MM/YYYY') > to_date('02/07/2018', 'DD/MM/YYYY')
        				          THEN TO_CHAR(date_trunc('month', to_date(dp.dopdatafimvigencia, 'MM/YYYY'))+ '1 month' + '59 day':: INTERVAL , 'DD/MM/YYYY')
        					        ELSE TO_CHAR(to_date('31/08/2018', 'DD/MM/YYYY') , 'DD/MM/YYYY')
        				        END AS data_vigencia,
                        'PAR' AS tipo,
                        'Omissão no envio de prestação de contas' AS situacao
              FROM par.instrumentounidade inu
               INNER JOIN par.processopar prp ON inu.inuid = prp.inuid
                                                   AND prp.prpstatus = 'A'
               INNER JOIN par.documentopar dp ON prp.prpid = dp.prpid
                                                   AND dp.dopstatus = 'A'
               INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
               INNER JOIN par.situacaoopc opc ON prp.prpid = opc.prpid
               INNER JOIN workflow.documento d ON opc.docid = d.docid
                                                    AND d.esdid = 2298
               INNER JOIN wf_municipio_PAR3 mp3 ON inu.inuid = mp3.inuid_par
                WHERE
                 mp3.inuid_par3 = {$inuid}
                 AND prp.prpid NOT IN
                 (
                    SELECT
                       efd.prpid
                    FROM
                       par.efeitosuspensivodocumento efd
                    WHERE
                       efd.prpid = prp.prpid
                       AND efd.efdstatus = 'A'
                 )
                 UNION ALL
                 SELECT DISTINCT
                            pro.pronumeroprocesso as processo,
                            dp.dopnumerodocumento::text as documento,
                            CASE WHEN to_date(dp.dopdatafimvigencia, 'MM/YYYY') > to_date('02/07/2018', 'DD/MM/YYYY')
            				          THEN TO_CHAR(date_trunc('month', to_date(dp.dopdatafimvigencia, 'MM/YYYY'))+ '1 month' + '59 day':: INTERVAL , 'DD/MM/YYYY')
            					        ELSE TO_CHAR(to_date('31/08/2018', 'DD/MM/YYYY') , 'DD/MM/YYYY')
            				        END AS data_vigencia,
                            'Obras PAR' AS tipo,
                            'Omissão no envio de prestação de contas' AS situacao
                FROM par.instrumentounidade inu
                       INNER JOIN par.processoobraspar pro ON inu.inuid = pro.inuid
                                                                AND pro.prostatus = 'A'
                       INNER JOIN par.documentopar dp ON pro.proid = dp.proid
                                                           AND dp.dopstatus = 'A'
                       INNER JOIN par.termocomposicao pto ON pto.dopid = dp.dopid
                       LEFT JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
                       INNER JOIN obras2.situacaoopcobras opc ON pro.proid = opc.proid_par
                       INNER JOIN workflow.documento d ON opc.docid = d.docid
                                                            AND d.esdid = 2278
                       INNER JOIN wf_municipio_PAR3 mp3 ON inu.inuid = mp3.inuid_par
                WHERE mp3.inuid_par3 = {$inuid}
                  AND pro.proid NOT IN
                      (SELECT efd.proid
                       FROM par.efeitosuspensivodocumento efd
                       WHERE efd.proid = pro.proid
                         AND efd.efdstatus = 'A')
                UNION ALL
                SELECT DISTINCT
                                pro.pronumeroprocesso as processo,
                                par.retornanumerotermopac(tc.proid)::text as documento,
                                CASE WHEN to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1), 'DD/MM/YYYY') > to_date('02/07/2018', 'DD/MM/YYYY')
                                  THEN TO_CHAR(to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY') + INTERVAL '60 day', 'DD/MM/YYYY')
                                  ELSE TO_CHAR(to_date('31/08/2018', 'DD/MM/YYYY'), 'DD/MM/YYYY')
                                END AS data_vigencia,
                                'PAC' AS tipo,
                                'Omissão no envio de prestação de contas' AS situacao
                FROM par.termocompromissopac tc
                       INNER JOIN par.processoobra pro ON pro.proid = tc.proid
                                                            AND pro.prostatus = 'A'
                       INNER JOIN par.termoobra ter ON tc.terid = ter.terid
                       INNER JOIN obras.preobra po ON ter.preid = po.preid
                       INNER JOIN par.instrumentounidade inu ON (tc.muncod = inu.muncod
                                                                   OR tc.estuf = inu.estuf)
                       INNER JOIN obras2.situacaoopcobras opc ON pro.proid = opc.proid_pac
                       INNER JOIN workflow.documento d ON opc.docid = d.docid
                                                            AND d.esdid = 2278
                       INNER JOIN wf_municipio_PAR3 mp3 ON inu.inuid = mp3.inuid_par
                WHERE mp3.inuid_par3 = {$inuid}
                    AND tc.terstatus = 'A'
                    AND pro.proid NOT IN
                    (
                    SELECT
                    efd.proid_pac
                    FROM
                    par.efeitosuspensivodocumento efd
                    WHERE
                    efd.proid_pac = pro.proid
                    AND efd.efdstatus = 'A'
                    )
                   ) as a";
        
        return $this->carregar($sql);
    }
    
    public function retornaEstadosPreDiagnostico()
    {
        $arrEsdPreDIag= array(
            self::ESDID_ETAPA_PREPARATORIA
        );
        return $arrEsdPreDIag;
    }

    public function retornaEstadosPrePlanejamento()
    {
        $arrEsdPreDIag = $this->retornaEstadosPreDiagnostico();
        $arrEsdPreDIag[] = self::ESDID_DIAGNOSTICO;
        $arrEsdPreDIag[] = self::ESDID_DIAGNOSTICO_FINALIZADO;
        return $arrEsdPreDIag;
    }

    public function buscaInuidUF()
    {
        return $this->modeloLegado->buscaInuidUF();
    }

    public function testaPermissaoUnidade($inuid)
    {
        if ( is_null($this->modeloLegado) ) $this->modeloLegado = new modeloLegado($inuid);
        return $this->modeloLegado->testaPermissaoUnidade($inuid);
    }

    public function getDadosUnidade($inuid)
    {
        return $this->modeloLegado->getDadosUnidade($inuid);
    }

    public function prepararFiltro($arrPost)
    {
        return $this->modeloLegado->prepararFiltro($arrPost);
    }

    public function pegarInuidAcessivel($arrPost)
    {
        return $this->modeloLegado->pegarInuidAcessivel($arrPost);
    }

    public function pegarSQLLista($arrPost)
    {
        return $this->modeloLegado->pegarSQLLista($arrPost);
    }

    public function recuperarInuidPar($inuid)
    {
        return $this->modeloLegado->recuperarInuidPar($inuid);
    }

    public function verificaInuidMunicipio($muncod)
    {
        return $this->modeloLegado->verificaInuidMunicipio($muncod);
    }

    public function verificaInuidEstado($estuf)
    {
        return $this->modeloLegado->verificaInuidEstado($estuf);
    }

    public function retornarQrpidUnidade($queid)
    {
        return $this->modeloLegado->retornarQrpidUnidade($queid);
    }

    public function retornarDocidQuestionario($queid)
    {
        return $this->modeloLegado->retornarDocidQuestionario($queid);
    }

    public function retornaPreenchimentoQuestoes($inuid)
    {
        return $this->modeloLegado->retornaPreenchimentoQuestoes($inuid);
    }

    public function retornaQuestoesParaPreenchimento($inuid)
    {
        return $this->modeloLegado->retornaQuestoesParaPreenchimento($inuid);
    }

    public function retornaQuestoesParaPreenchimentoPNE($inuid)
    {
        return $this->modeloLegado->retornaQuestoesParaPreenchimentoPNE($inuid);
    }

    public function retornaPreenchimentoQuestoesPNE($inuid)
    {
        return $this->modeloLegado->retornaPreenchimentoQuestoesPNE($inuid);
    }

    public function recuperarEmailEsponsaveis($inuid)
    {
        return $this->modeloLegado->recuperarEmailEsponsaveis($inuid);
    }

    public function salvarJustificativaTreinamento($inuid)
    {
        return $this->modeloLegado->salvarJustificativaTreinamento($inuid);
    }

    public function verificaMunicipioSIOP($inuid)
    {
        return $this->modeloLegado->verificaMunicipioSIOP($inuid);
    }

    public function carregaOrcamentoUnidade($inuid)
    {
        return $this->modeloLegado->carregaOrcamentoUnidade($inuid);
    }

    public function retornaBooPendencias()
    {
        return $this->modeloLegado->retornaBooPendencias();
    }

    public function getInstrumentoUnidadeById($inuid)
    {
        return $this->modeloLegado->getInstrumentoUnidadeById($inuid);
    }

    public function getInstrumentoUnidadeByIdMunic($inuid)
    {
        return $this->modeloLegado->getInstrumentoUnidadeByIdMunic($inuid);
    }

    public function pegarNomeEstado($esdid)
    {
        return $this->modeloLegado->pegarNomeEstado($esdid);
    }

    public function create($inuid)
    {
        return $this->modeloLegado->create($inuid);
    }

    public function atualizarPendencia($inuid){
        return $this->modeloLegado->atualizarPendencia($inuid);
    }

    public function pegarEscopoPendencias(){
        return $this->modeloLegado->pegarEscopoPendencias();
    }

    public function pegarEscopoAlertas(){
        return $this->modeloLegado->pegarEscopoAlertas();
    }

}
