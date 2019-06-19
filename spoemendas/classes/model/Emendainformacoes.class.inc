<?php
/**
 * Classe de mapeamento da entidade spoemendas.emendainformacoes
 *
 * @category Class
 * @package  A1
 * @author   MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-02-2016
 * @link     no link
 */

/**
 * Spoemendas__ModelEmendainformacoes
 *
 * @category Class
 * @package  A1
 * @author   MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-02-2016
 * @link     no link
 */
class Spoemendas_Model_Emendainformacoes extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'spoemendas.emendainformacoes';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'einid'
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'einid' => null,
        'emeid' => null,
        'einimpositiva' => null,
        'docid' => null,
    );

    protected $esdid;

    public function getEsdid()
    {
        if (is_null($this->esdid)) {
            $this->esdid = $this->consultarEstadoDocumento()['esdid'];
        }
        return $this->esdid;
    }

    protected function consultarEstadoDocumento()
    {
        if (empty($this->docid)) {
            throw new Exception('Este conjunto de informações da emenda não possui um docid associado a ele.');
        }

        $query = <<<DML
SELECT esd.esdid,
       esd.esddsc
  FROM workflow.documento doc
    INNER JOIN workflow.estadodocumento esd USING(esdid)
  WHERE doc.docid = {$this->docid}
DML;
        return $this->pegaLinha($query);
    }



    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'einid' => array(  'Digits'  ),
            'emeid' => array( 'allowEmpty' => true ),
            'einimpositiva' => array( 'allowEmpty' => true ),
            'docid' => array( 'allowEmpty' => true ),
        );
    }//end getCamposValidacao($dados)

    public function findByEmeid($emeid)
    {
        $sql = <<<DML
SELECT *
  FROM {$this->stNomeTabela} ein
  WHERE ein.emeid = {$emeid}
DML;
        return $this->pegaLinha($sql);
    }

    public function emPreenchimento()
    {
        return ESD_EM_PREENCHIMENTO == $this->getEsdid();
    }

    public function aguardandoAjustes()
    {
        return ESD_AGUARDANDO_AJUSTES_PARLAMENTAR == $this->getEsdid();
    }

    public function limiteLiberado()
    {
        return ESD_LIMITE_LIBERADO == $this->getEsdid();
    }
    public function aguardandoAnaliseSecre()
    {
        return ESD_ANALISA_SECRETARIA == $this->getEsdid();
    }
    public function aguardandoAprovacaoReitor()
    {
        return ESD_AGUARDANDO_REITOR == $this->getEsdid();
    }
    public function aguardandoAjustesUo()
    {
        return ESD_AGUARD_AJUST_UO == $this->getEsdid();
    }
    public function analiseAspar()
    {
        return ESD_AGUARD_ANALISE_FNDE == $this->getEsdid();
    }
    public function ajustesParlamentar()
    {
        return ESD_AGUARDANDO_AJUSTES_PARLAMENTAR == $this->getEsdid();
    }
    public function analiseFnde()
    {
        return ESD_AGUARD_ANALISE_FNDE == $this->getEsdid();
    }
}
