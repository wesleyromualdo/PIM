<?php
/**
 * Classe de mapeamento da entidade par3.ensino_nivel.
 *
 * @version $Id$
 * @since 2017.05.08
 */

/**
 * Model_Ensino_nivel: sem descricao
 *
 * @package Model\Ensino
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Ensino_nivel($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Ensino_nivel($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $nivid Chave Primária - default: nextval('par3.ensino_nivel_nivid_seq'::regclass)
 * @property string $nivdsc Descrição do  Nivel de Ensino
 */
class Par3_Model_EnsinoNivel extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.ensino_nivel';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'nivid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array();

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'nivid' => null,
        'nivdsc' => null,
    );

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

    public function pegarSQLSelectCombo($nivid = null)
    {
        $where = $nivid ? "WHERE nivid = $nivid" : '';
        return "SELECT  nivid as codigo,nivdsc as descricao FROM {$this->stNomeTabela} $where";
    }


    /**
     * Lista Nível de Ensino.
     *
     * @return array|mixed|NULL
     */
    public function listaEnsinoNivel()
    {
        return $this->carregar("SELECT nivid as codigo,  nivdsc as descricao FROM {$this->stNomeTabela}");

    }


}
