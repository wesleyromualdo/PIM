<?php
/**
 * Classe de mapeamento da entidade seguranca.perfilusuario.
 *
 * @version $Id$
 * @since 2016.08.31
 */

/**
 * Seguranca_Model_Perfilusuario: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Perfilusuario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Perfilusuario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $usucpf 
 * @property string $usucpf 
 * @property int $pflcod 
 * @property int $pflcod 
 */
class Seguranca_Model_Perfilusuario extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.perfilusuario';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'pflcod',
        'usucpf',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pflcod' => array('tabela' => 'seguranca.perfil', 'pk' => 'pflcod'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'usucpf' => null,
        'pflcod' => null,
        'pflcod' => null,
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
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'pflcod' => array('Digits'),
            'pflcod' => array('Digits'),
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
