<?php
/**
 * Classe de mapeamento da entidade recorc.usuarioresponsabilidade.
 *
 * @version $Id$
 * @since 2018.04.23
 */

/**
 * Model_Model_Usuarioresponsabilidade: Tabela contendo a rela��o dos usu�rios com acesso ao sistema por perfil e unidade.
 *
 * @package Model\Model
 * @uses Simec\Db\Modelo
 * @author Eudson Alves Macedo <eudson.macedo@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Model_Usuarioresponsabilidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Model_Usuarioresponsabilidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $rpuid Identificador �nico da tabela usuarioresponsabilidade. - default: nextval('recorc.usuarioresponsabilidade_rpuid_seq'::regclass)
 * @property int $pflcod 
 * @property string $usucpf CPF do usu�rio.
 * @property string $rpustatus Status do registro (A - Ativo, I - Inativo). - default: 'A'::bpchar
 * @property string $unicod Codigo da Unidade para delegacao de responsabilidades
 * @property \Datetime(Y-m-d H:i:s) $rpudata_inc Data de inclus�o do registro. - default: '2008-09-02 17:46:42.244463'::timestamp without time zone
 */
class Model_Usuarioresponsabilidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'recorc.usuarioresponsabilidade';

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
        'pflcod' => array('tabela' => 'seguranca.perfil', 'pk' => 'pflcod'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'rpuid' => null,
        'pflcod' => null,
        'usucpf' => null,
        'rpustatus' => null,
        'unicod' => null,
        'rpudata_inc' => null,
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
            'rpuid' => array('Digits'),
            'pflcod' => array('allowEmpty' => true,'Digits'),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'rpustatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'unicod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'rpudata_inc' => array('allowEmpty' => true),
        );
    }

    /**
     * M�todo de transforma��o de valores e valida��es adicionais de dados.
     *
     * Este m�todo tem as seguintes finalidades:
     * a) Transforma��o de dados, ou seja, alterar formatos, remover m�scaras e etc
     * b) A segunda, � a valida��o adicional de dados. Se a valida��o falhar, retorne false, se n�o falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transforma��es de dados aqui

        // -- Por padr�o, o m�todo sempre retorna true
        return parent::antesSalvar();
    }

}
