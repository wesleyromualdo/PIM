<?php
/**
 * Classe de mapeamento da entidade emenda.emendaimpedimentobeneficiario.
 *
 * @version $Id: Emendaimpedimentobeneficiario.class.inc 139367 2018-04-18 12:54:14Z eudsonmacedo $
 * @since 2016.03.02
 */

require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Spoemendas_Model_Emendaimpedimentobeneficiario: Relacionamento entre emendaimpositivo, emendadetalheimpositivo e iniciativaresponsavel
 *
 * @package Emenda\Model
 * @uses Simec\Db\Modelo
 * @author Maykel Santos Braz <maykel.braz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Emenda_Model_Emendaimpedimentobeneficiario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Emenda_Model_Emendaimpedimentobeneficiario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $eibdscobj Texto descritivo de emenda.impositivoresponsavel
 * @property int $ireid referencia para emenda.iniciativaresponsavel
 * @property int $ediid referencia para emenda.emendadetalheimpositivo
 * @property int $emiid referencia para emenda.emendaimpositivo
 * @property int $eibid chave primaria - default: nextval('emenda.emendaimpedimentobeneficiario_eibid_seq'::regclass)
 */
class Spoemendas_Model_Emendaimpedimentobeneficiario extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'emenda.emendaimpedimentobeneficiario';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'eibid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ireid' => array('tabela' => 'emenda.iniciativaresponsavel', 'pk' => 'ireid'),
        'emiid' => array('tabela' => 'emenda.emendaimpositivo', 'pk' => 'emiid'),
        'ediid' => array('tabela' => 'emenda.emendadetalheimpositivo', 'pk' => 'ediid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'eibdscobj' => null,
        'ireid' => null,
        'ediid' => null,
        'emiid' => null,
        'eibid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'eibdscobj' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'ireid' => array('Digits'),
            'ediid' => array('Digits'),
            'emiid' => array('Digits'),
            'eibid' => array('Digits'),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function pegaImpedimentos(array $criteria, $retornaQuery = false)
    {
        
    	$criteria = array_filter($criteria, 'strlen');
        
        if(is_array($criteria)){ 
	        foreach ($criteria as $campo => $valor) {
	        	$aryWhere[] = "{$campo} = '{$valor}'";
	        }
        }
        
    $aryWhere[] = "emdstatus = 'A'";

    $sql = "SELECT emd.emdid,
			       edi.ediid,
			       eib.eibid,
			       eib.emiid,
			       ede.edeid,
			       enb.enbcnpj,
			       enb.enbnome,
			       ede.edeprioridade,
			       emd.gndcod,
			       emd.mapcod,
			       emd.foncod,
			       edi.ediimpositivo,
			       edi.edidescricao,
			       edi.edivalor,
			       eib.ireid,
			       eib.eibdscobj,
			       ede.edevalor AS valor_entidade,
			       ede.edelimiteempenhobeneficiario
		  	FROM emenda.emendadetalhe emd
		    INNER JOIN emenda.emendadetalheentidade ede ON emd.emdid = ede.emdid AND edestatus = 'A' 
		    INNER JOIN emenda.entidadebeneficiada enb ON ede.enbid = enb.enbid AND enbstatus = 'A'
		    LEFT JOIN  emenda.emendadetalheimpositivo edi ON edi.emdid = emd.emdid AND edi.edeid = ede.edeid AND edistatus = 'A' 
		    LEFT JOIN  emenda.emendaimpedimentobeneficiario eib ON eib.ediid = edi.ediid AND edistatus = 'A'
    					".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."
  			ORDER BY edeprioridade, enbnome";

        if ($retornaQuery) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado?$resultado:[];
    }

    public function pegaTodosImpedimentos(array $criteria){
        $criteria = array_filter($criteria, 'strlen');

        if(is_array($criteria)){
            foreach ($criteria as $campo => $valor) {
                $aryWhere[] = "{$campo} = '{$valor}'";
            }
        }

        $aryWhere[] = "emdstatus = 'A'";

        $sql = "SELECT emd.emdid,
			       edi.ediid,
			       eib.eibid,
			       eib.emiid,
			       ede.edeid,
			       enb.enbcnpj,
			       enb.enbnome,
			       ede.edeprioridade,
			       emd.gndcod,
			       emd.mapcod,
			       emd.foncod,
			       edi.ediimpositivo,
			       edi.edidescricao,
			       edi.edivalor,
			       eib.ireid,
			       eib.eibdscobj,
			       ede.edevalor AS valor_entidade,
			       ede.edelimiteempenhobeneficiario
		  	FROM emenda.emendadetalhe emd
		    INNER JOIN emenda.emendadetalheentidade ede ON emd.emdid = ede.emdid 
		    INNER JOIN emenda.entidadebeneficiada enb ON ede.enbid = enb.enbid 
		    LEFT JOIN  emenda.emendadetalheimpositivo edi ON edi.emdid = emd.emdid AND edi.edeid = ede.edeid
		    LEFT JOIN  emenda.emendaimpedimentobeneficiario eib ON eib.ediid = edi.ediid
    					".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."
  			ORDER BY edeprioridade, enbnome";


        $resultado = $this->carregar($sql);
        return $resultado?$resultado:[];
    }

    public function pegaUmImpedimento(array $criteria)
    {
        return current($this->pegaImpedimentos($criteria));
    }

}
