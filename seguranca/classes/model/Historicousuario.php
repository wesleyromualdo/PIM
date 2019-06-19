<?php 
/**
 * Classe de mapeamento da entidade seguranca.historicousuario.
 *
 * @version $Id$
 * @since 2017.02.03
 */

/**
 * Seguranca_Model_Historicousuario: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Historicousuario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Historicousuario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $usucpfadm 
 * @property string $suscod 
 * @property int $sisid 
 * @property string $usucpf 
 * @property \Datetime(Y-m-d H:i:s) $htudata  - default: now()
 * @property string $htudsc 
 * @property int $htuid  - default: nextval('seguranca.historicousuario_htuid_seq'::regclass)
 */
class Seguranca_Model_Historicousuario extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.historicousuario';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'htuid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'sisid' => array('tabela' => 'seguranca.sistema', 'pk' => 'sisid'),
        'suscod' => array('tabela' => 'seguranca.statususuario', 'pk' => 'suscod'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'usucpfadm' => null,
        'suscod' => null,
        'sisid' => null,
        'usucpf' => null,
        'htudata' => null,
        'htudsc' => null,
        'htuid' => null,
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
            'usucpfadm' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'suscod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'sisid' => array('allowEmpty' => true,'Digits'),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'htudata' => array(),
            'htudsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'htuid' => array('Digits'),
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
