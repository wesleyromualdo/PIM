<?php
/**
 * Classe de mapeamento da entidade public.extratodinamicoconsultas.
 *
 * @version $Id: Extratodinamicoconsultas.class.inc 109536 2016-03-29 21:19:57Z maykelbraz $
 * @since 2016.03.29
 */

/**
 * Public_Model_Extratodinamicoconsultas: Armazena as consultas configuradas e salvas pelos usuarios para posterior utilizacao
 *
 * @package Public\Model
 * @uses Simec\Db\Modelo
 * @author Maykel Santos Braz <maykel.braz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Public_Model_Extratodinamicoconsultas($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Public_Model_Extratodinamicoconsultas($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $edcdescricao Texto de descricao do extrato salvo
 * @property \Datetime(Y-m-d H:i:s) $dataultimaalteracao Data em que aconteceu a ultima alteracao na configuracao da consulta - default: now()
 * @property bool $edcpublico Indica se o relatorio salvo deve ser exibido para qualquer usuario - default: false
 * @property string $edcconfig JSON da configuracao do relatorio
 * @property string $usucpf CPF do usuario que salvou o relatorio - referencia para seguranca.usuario
 * @property string $edcnome Nome do relatorio salvo pelo usuario
 * @property int $exdid Referencia para public.extratodinamico
 * @property int $edcid  - default: nextval('extratodinamicoconsultas_edcid_seq'::regclass)
 */
class Public_Model_Extratodinamicoconsultas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'public.extratodinamicoconsultas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'edcid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'exdid' => array('tabela' => 'extratodinamico', 'pk' => 'exdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'edcdescricao' => null,
        'dataultimaalteracao' => null,
        'edcpublico' => null,
        'edcconfig' => null,
        'usucpf' => null,
        'edcnome' => null,
        'exdid' => null,
        'edcid' => null,
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
            'edcdescricao' => array('allowEmpty' => true),
            'dataultimaalteracao' => array(),
            'edcpublico' => array(),
            'edcconfig' => array(),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'edcnome' => array(new Zend_Validate_StringLength(array('max' => 250))),
            'exdid' => array('Digits'),
            'edcid' => array('Digits'),
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
        $this->edcpublico = empty($this->edcpublico)?'f':'t';

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

}
