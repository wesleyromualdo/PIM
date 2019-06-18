<?php
/**
 * Classe de mapeamento da entidade sase.agendacomissaocoordenadora.
 *
 * @version $Id$
 * @since 2016.08.15
 */

/**
 * Sase_Model_Agendacomissaocoordenadora: sem descricao
 *
 * @package Sase\Model
 * @uses Simec\Db\Modelo
 * @author Luciano Fernandes Ribeiro <luciano.ribeiro@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sase_Model_Agendacomissaocoordenadora($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sase_Model_Agendacomissaocoordenadora($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $accid  - default: nextval('sase.agendacomissaocoordenadora_accid_seq'::regclass)
 * @property int $eacid 
 * @property string $muncod 
 * @property string $accacao 
 * @property string $accresponsavel 
 * @property \Datetime(Y-m-d H:i:s) $accprazo 
 * @property string $accobservacao 
 * @property string $accstatus  - default: 'A'::bpchar
 * @property string $usucpf 
 * @property \Datetime(Y-m-d H:i:s) $accdatainclusao 
 * @property \Datetime(Y-m-d H:i:s) $accdataalteracao 
 */
class Sase_Model_Agendacomissaocoordenadora extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.agendacomissaocoordenadora';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'accid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'eacid' => array('tabela' => 'sase.etapaagendacomcoord', 'pk' => 'eacid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'accid' => null,
        'eacid' => null,
        'muncod' => null,
        'accacao' => null,
        'accresponsavel' => null,
        'accprazo' => null,
        'accobservacao' => null,
        'accstatus' => null,
        'usucpf' => null,
        'accdatainclusao' => null,
        'accdataalteracao' => null,
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
            'accid' => array('Digits'),
            'eacid' => array('allowEmpty' => true,'Digits'),
            'muncod' => array(new Zend_Validate_StringLength(array('max' => 7))),
            'accacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'accresponsavel' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 300))),
            'accprazo' => array('allowEmpty' => true),
            'accobservacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'accstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'accdatainclusao' => array('allowEmpty' => true),
            'accdataalteracao' => array('allowEmpty' => true),
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

}
