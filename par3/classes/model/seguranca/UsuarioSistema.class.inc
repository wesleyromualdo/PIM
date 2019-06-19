<?php
/**
 * Classe de mapeamento da entidade seguranca.usuario_sistema.
 *
 * @version $Id$
 * @since 2016.08.31
 */

/**
 * Seguranca_Model_Usuario_sistema: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Usuario_sistema($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Usuario_sistema($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $usucpf
 * @property int $sisid
 * @property string $susstatus  - default: 'A'::bpchar
 * @property int $pflcod
 * @property \Datetime(Y-m-d H:i:s) $susdataultacesso  - default: now()
 * @property string $suscod  - default: 'A'::bpchar
 */
class Seguranca_Model_Seguranca_UsuarioSistema extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.usuario_sistema';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'usucpf',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'sisid' => array('tabela' => 'seguranca.sistema', 'pk' => 'sisid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'sisid' => null,
        'susstatus' => null,
        'pflcod' => null,
        'susdataultacesso' => null,
        'suscod' => null,
    );

    public function getCamposNulos()
    {
        return array('susstatus' => null, 'pflcod' => null, 'susdataultacesso' => null, 'suscod' => null);
    }

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
            'sisid' => array('Digits'),
            'sisid' => array('Digits'),
            'susstatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'pflcod' => array('allowEmpty' => true, 'Digits'),
            'susdataultacesso' => array('allowEmpty' => true),
            'suscod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
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

    function salvarUsuarioSistema($cpf, $sisid)
    {
        $this->usucpf = $cpf;
        $this->sisid = $sisid;
        $this->suscod = 'A';

        $sql = " INSERT INTO seguranca.usuario_sistema( usucpf, sisid, suscod) VALUES ('{$this->usucpf}', '{$this->sisid}', '{$this->suscod}') returning usucpf; ";
        return $this->executar($sql, false);
    }
}
