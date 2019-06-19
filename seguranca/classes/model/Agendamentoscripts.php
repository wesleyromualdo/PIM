<?php
/**
 * Classe de mapeamento da entidade seguranca.agendamentoscripts.
 *
 * @version $Id$
 * @since 2018.05.28
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Seguranca_Model_Agendamentoscripts: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author Felipe Tarchiani Ceravolo Chiavicatti <felipe.chiavicatti@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Agendamentoscripts($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Agendamentoscripts($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property \Datetime(Y-m-d H:i:s) $agsultexecucao  - default: now()
 * @property numeric $agstempoexecucao 
 * @property string $agsdescricao 
 * @property \Datetime(Y-m-d H:i:s) $agsdataexec 
 * @property string $agsstatus  - default: 'A'::bpchar
 * @property string $agsperdetalhes 
 * @property string $agsperiodicidade 
 * @property string $agsfile 
 * @property int $agsid  - default: nextval('agendamentoscripts_agsid_seq'::regclass)
 */
class Seguranca_Model_Agendamentoscripts extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.agendamentoscripts';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'agsid',
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
        'agsultexecucao' => null,
        'agstempoexecucao' => null,
        'agsdescricao' => null,
        'agsdataexec' => null,
        'agsstatus' => null,
        'agsperdetalhes' => null,
        'agsperiodicidade' => null,
        'agsfile' => null,
        'agsid' => null,
        'aemid' => null,
    );
    
//     /**
//      * Validators.
//      *
//      * @param mixed[] $dados
//      * @return mixed[]
//      */
//     public function getCamposValidacao($dados = array())
//     {
//         return array(
//             'agsultexecucao' => array('allowEmpty' => true),
//             'agstempoexecucao' => array('allowEmpty' => true),
//             'agsdescricao' => array('allowEmpty' => true),
//             'agsdataexec' => array('allowEmpty' => true),
//             'agsstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
//             'agsperdetalhes' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
//             'agsperiodicidade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 200))),
//             'agsfile' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 250))),
//             'agsid' => array('Digits'),
//         );
//     }

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
    
    public function carregarPorAemid($aemid){
    	$aemid = (is_numeric($aemid) ? $aemid : 0);
    	$sql = "select * from seguranca.agendamentoscripts where aemid=$aemid";
    	$arDado = $this->pegaLinha($sql);
    	$arDado = ($arDado ? $arDado : array());
    	
    	$this->popularDadosObjeto($arDado);
    }

}
