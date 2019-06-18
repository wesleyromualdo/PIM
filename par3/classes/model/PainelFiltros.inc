<?php
/**
 * Classe de mapeamento da entidade par3.painel_filtros.
 *
 * @version $Id$
 * @since 2018.09.25
 */

/**
 * Par3_Model_Painel_filtros: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Painel_filtros($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Painel_filtros($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $codigo  - default: nextval('par3.painel_filtros_codigo_seq'::regclass)
 * @property string $usucpf
 * @property string $descricao
 * @property string $filtros
 */
class Par3_Model_PainelFiltros extends Modelo
{
    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct($id, $tempocache);
    }

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.painel_filtros';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'codigo',
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
        'codigo' => null,
        'usucpf' => null,
        'descricao' => null,
        'filtros' => null,
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
            'codigo' => array('Digits'),
            'usucpf' => array(),
            'descricao' => array(),
            'filtros' => array(),
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

    public function salvarFiltros($arParams = null)
    {
//        ver($arParams,d);
        $usucpf    = $_SESSION['usucpf'];
        $descricao = ' ';
        $filtros   = $arParams;
        if($this->carregarConfiguracoes($usucpf)) {
            $sql = "UPDATE {$this->stNomeTabela} SET filtros = '{$filtros}' WHERE usucpf = '{$usucpf}'";
            $this->executar($sql);
            $this->commit();
            return 1;
        }
        $sql = "INSERT INTO {$this->stNomeTabela} (usucpf,descricao,filtros) values('{$usucpf}','{$descricao}','{$filtros}')";
        $this->executar($sql);
        $this->commit();
        return 1;
    }

    public function carregarConfiguracoes($usucpf)
    {
        $sql = "SELECT * FROM {$this->stNomeTabela} where usucpf = '{$usucpf}'";
        return $this->carregar($sql)[0];
    }

    public function resetConfiguracoes($usucpf)
    {
        $sql = "DELETE FROM {$this->stNomeTabela} WHERE usucpf = '{$usucpf}'";
        $this->executar($sql);
        $this->commit();
        return 1;
    }
}
