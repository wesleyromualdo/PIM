<?php

/**
 * Emenda_Model_Emendadetalheentidade
 *
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-02-2016
 * @link     no link
 * @property int $emdid Chave primaria;
 */
class Spoemendas_Model_EmendaDetalhe extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emenda.emendadetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'emdid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emdid' => null,
        'emeid' => null,
        'gndcod' => null,
        'foncod' => null,
        'mapcod' => null,
        'emdvalor' => null,
        'emddataalteracao' => null,
        'usucpfalteracao' => null,
        'emdtipo' => null,
        'emdimpositiva' => null,
        'emdtipodescentralizacao' => null,
        'emdliberacaosri' => null,
        'emddtultimoenviosiop' => null,
        'emdstatus' => null
    );

    /**
     * @param $emeid
     * @return array|string
     */
    public function getCombo($emeid)
    {
        if (!$emeid) {
            return [];
        }

        $strSQL = "
            SELECT
                emdid                                                                 AS codigo,
                'GND: '||gndcod || ' - Fonte: '|| foncod || ' - Modalidade: '||mapcod AS descricao
            FROM emenda.emendadetalhe WHERE emeid = {$emeid} and emdstatus = 'A'
        ";

        return $strSQL;
    }

    public function findByEmeid($emeid)
    {
        $query = <<<DML
SELECT emd.emdid,
       emd.emeid,
       emd.gndcod,
       emd.foncod,
       emd.mapcod,
       emd.emdvalor,
       emd.emdimpositiva
  FROM {$this->stNomeTabela} emd
  WHERE emd.emeid = %d
DML;
        $query = sprintf($query, $emeid);
        return $this->pegaLinha($query);
    }

    public static function getQueryDetalhesDaEmenda($emeid)
    {
        $sql = <<<DML
SELECT emd.emdid,
       emd.gndcod,
       gnd.gnddsc,
       emd.foncod,
       ftr.fondsc,
       emd.mapcod,
       map.mapdsc,
       emd.emdvalor
  FROM emenda.emendadetalhe emd
    INNER JOIN public.gnd USING(gndcod)
    INNER JOIN public.fonterecurso ftr USING(foncod)
    INNER JOIN public.modalidadeaplicacao map USING(mapcod)
  WHERE emd.emeid = %d
        and emdstatus = 'A'
DML;
        $sql = sprintf($sql, $emeid);
        return $sql;
    }

    public function antesSalvar()
    {
        if (strstr($this->emdvalor, ',')) {
            $this->emdvalor = str_replace(['.', ','], ['', '.'], $this->emdvalor);
        }

        return parent::antesSalvar();
    }

    public function carregarPorEmeidEComposicao($emeid, $gndcod, $foncod, $mapcod)
    {
        $sql = <<<DML
SELECT *
  FROM emenda.emendadetalhe emd
  WHERE emd.emeid = {$emeid}
    AND emd.gndcod = '{$gndcod}'
    AND emd.foncod = '{$foncod}'
    AND emd.mapcod = '{$mapcod}'
DML;

        $dados = $this->pegaLinha($sql);
        return $dados?$dados:[];
    }
}
