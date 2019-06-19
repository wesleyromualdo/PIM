<?php
/**
 * Classe de mapeamento da entidade emenda.entidadebeneficiada
 *
 * @version $Id: Entidadebeneficiada.class.inc 129414 2017-08-23 01:12:40Z maykelbraz $
 */

/**
 * Emenda_Model_Entidadebeneficiada
 *
 * @package  Spoemendas\model
 * @author   WERTER DIAS ALMEIDA <werter.almeida@mec.gov.br>
 */
class Spoemendas_Model_Entidadebeneficiada extends Modelo
{
    /**
     * @var string Nome da tabela especificada.
     */
    protected $stNomeTabela = 'emenda.entidadebeneficiada';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'enbid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'entid' => null,
        'enbstatus' => null,
        'enbano' => null,
        'enbid' => null,
        'enbsituacaohabilita' => null,
        'enbdataalteracao' => null,
        'enbcnpj' => null,
        'muncod' => null,
        'estuf' => null,
        'enblog' => null,
        'enbnum' => null,
        'enbbai' => null,
        'enbcep' => null,
        'enbnumdddcomercial' => null,
        'enbnumcomercial' => null,
        'enbnumfax' => null,
        'enbemail' => null,
        'enborgcod' => null,
        'enbnome' => null,
        'enbnumprocesso' => null,
        'enbvaldataini' => null,
        'enbvaldatafim' => null,
        'enbdatacertificado' => null,
        'enbcertificado' => null,
        'enbcertificadovalido' => null,
        'enborgaocertificado' => null,
    );

    /**
     * Validators.
     * @param type $dados
     * @return type
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'entid' => array( 'allowEmpty' => true, 'Digits'  ),
            'enbstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'enbano' => array( 'allowEmpty' => true, 'Digits'  ),
            'enbid' => array(  'Digits'  ),
            'enbsituacaohabilita' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 3000))  ),
            'enbdataalteracao' => array( 'allowEmpty' => true ),
            'enbcnpj' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 20))  ),
            'muncod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))  ),
            'estuf' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 8))  ),
            'enblog' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
            'enbnum' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'enbbai' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))  ),
            'enbcep' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 8))  ),
            'enbnumdddcomercial' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 3))  ),
            'enbnumcomercial' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'enbnumfax' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'enbemail' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))  ),
            'enborgcod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 5))  ),
            'enbnome' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))  ),
            'enbnumprocesso' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 17))  ),
            'enbvaldataini' => array( 'allowEmpty' => true ),
            'enbvaldatafim' => array( 'allowEmpty' => true ),
            'enbdatacertificado' => array( 'allowEmpty' => true ),
            'enbcertificado' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'enbcertificadovalido' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'enborgaocertificado' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 5))  ),
        );
    }

    public function salvarEntidadeBeneficiada(){
        $sql = "insert into emenda.entidadebeneficiada (enbcnpj, enbnome)
                values ('".$this->enbcnpj."', '".$this->enbnome."') returning enbid";
        $enbid = $this->executar($sql);
        $this->commit();
        return $enbid;
    }

    public function antesSalvar()
    {
        $this->enbcnpj = $this->limparCNPJ($this->enbcnpj);
        return parent::antesSalvar();
    }

    public function findCNPF($cnpj)
    {
        $cnpj = $this->limparCNPJ($cnpj);
        $strSQL = "
            select * from emenda.entidadebeneficiada where enbcnpj = '{$cnpj}'
        ";

        return $this->pegaLinha($strSQL);
    }

    protected function limparCNPJ($cnpj)
    {
        return str_replace(['.', '/', '-'], '', $cnpj);
    }

    public static function queryBeneficiarioByEmeid($emeid, $adm = false)
    {
        $where = "AND emdstatus = 'A'";
        if ($adm) {
            $select = ', emd.emdstatus';
            $where = '';
        }

        $sql = <<<DML
SELECT ede.edeid,
       enb.enbcnpj,
       enb.enbnome,
       ede.edeprioridade,
       emd.gndcod,
       emd.mapcod,
       ede.edevalor,
       enb.enbid,
       ede.edelimiteempenhobeneficiario{$select}
  FROM emenda.entidadebeneficiada enb
    INNER JOIN emenda.emendadetalheentidade ede USING(enbid)
    INNER JOIN emenda.emendadetalhe emd USING(emdid)
  WHERE emd.emeid = %d
    AND enbstatus = 'A'
    {$where}
    AND edestatus = 'A'
    AND ede.edevalor > 0
    AND enb.enbcnpj <> ''
    ORDER BY ede.edeprioridade, 2
DML;
        $sql = sprintf($sql, $emeid);
        return $sql;
    }

    public function beneficiariosByEmeid($emeid)
    {
        return $this->carregar(self::queryBeneficiarioByEmeid($emeid));
    }

    public function carregarDetalhesBeneficiarios($emeid, $cnpj)
    {
        $sql = <<<DML
SELECT emd.*,
       ede.*,
       enb.*
  FROM emenda.entidadebeneficiada enb
    LEFT JOIN emenda.emendadetalheentidade ede ON (enb.enbid = ede.enbid /*AND ede.edestatus = 'A'*/)
    LEFT JOIN emenda.emendadetalhe emd ON (emd.emdid = ede.emdid AND emd.emdstatus = 'A' AND emd.emeid = {$emeid})
  WHERE enb.enbcnpj = '{$cnpj}'
    AND enb.enbstatus = 'A'
    AND emd.emdid IS NOT NULL
DML;
        $beneficiarios = $this->carregar($sql);
        return $beneficiarios?$beneficiarios:[];
    }
}
