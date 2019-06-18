<?php
/**
 * Classe de mapeamento da entidade spoemendas.quadroinformacoes.
 *
 * @version $Id$
 * @since 2017.12.07
 */

/**
 * Spoemendas_Model_Quadroinformacoes: sem descricao
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Quadroinformacoes($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Quadroinformacoes($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $quiid  - default: nextval('spoemendas.quadroinformacoes_quiid_seq'::regclass)
 * @property \Datetime(Y-m-d H:i:s) $quidata  - default: now()
 * @property string $quimensagem 
 * @property string $usucpf
 * @property int $pflcod
 */
class Spoemendas_Model_Quadroinformacoes extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.quadroinformacoes';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'quiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'pflcod' => array('tabela' => 'seguranca.perfil', 'pk' => 'pflcod')
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'quiid' => null,
        'quidata' => null,
        'quimensagem' => null,
        'usucpf' => null,
        'pflcod' => null
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
            'quiid' => array('Digits'),
            'quidata' => array('allowEmpty' => true),
            'quimensagem' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'pflcod' => array('allowEmpty' => true)
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

    /**
     * Método que retorna as informações registradas, limitando a quantidade de registros pela variável $qtd
     *
     * @param int $qtd
     * @return array|mixed|NULL
     */
    public function getInformacoes($qtd = 10) {
        $where = ["WHERE 1=1"];

        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

        if (!empty($pflcod) and !in_array(PFL_SUPER_USUARIO, $perfis)) {
            $where[] = "AND (pflcod IS NULL OR pflcod IN ('".implode("', '", $perfis)."'))";
        } else {
            $where[] = "AND pflcod IS NULL";
        }

        $where = implode(', ', $where);
        $sql = <<<SQL
            SELECT
              quiid,
              TO_CHAR(quidata, 'DD/MM/YYYY') AS quidata,
              quimensagem,
              usucpf,
              pflcod
            FROM spoemendas.quadroinformacoes
            ORDER BY quidata DESC
            LIMIT {$qtd}
SQL;
        return $this->carregar($sql);
    }

}
