<?php
/**
 * Classe de mapeamento da entidade par3.usuarioresponsabilidade.
 *
 * @version $Id$
 * @since 2016.09.05
 */

/**
 * Par3_Model_Usuarioresponsabilidade: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Usuarioresponsabilidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Usuarioresponsabilidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $prgid 
 * @property string $estuf 
 * @property int $entid 
 * @property string $muncod 
 * @property \Datetime(Y-m-d H:i:s) $rpudata_inc  - default: '2008-09-02 17:46:42.244463'::timestamp without time zone
 * @property string $rpustatus  - default: 'A'::bpchar
 * @property string $usucpf 
 * @property int $pflcod 
 * @property int $rpuid  - default: nextval('par3.usuarioresponsabilidade_rpuid_seq'::regclass)
 */
class Par3_Model_Usuarioresponsabilidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.usuarioresponsabilidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'rpuid',
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
        'prgid' => null,
        'estuf' => null,
        'entid' => null,
        'muncod' => null,
        'rpudata_inc' => null,
        'rpustatus' => null,
        'usucpf' => null,
        'pflcod' => null,
        'rpuid' => null,
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
            'prgid' => array('allowEmpty' => true,'Digits'),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'entid' => array('allowEmpty' => true,'Digits'),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'rpudata_inc' => array('allowEmpty' => true),
            'rpustatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'pflcod' => array('allowEmpty' => true,'Digits'),
            'rpuid' => array('Digits'),
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
