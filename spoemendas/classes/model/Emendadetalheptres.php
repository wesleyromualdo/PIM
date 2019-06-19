<?php
/**
 * Classe de mapeamento da entidade spoemendas.emendadetalheptres.
 *
 * @version $Id$
 * @since 2017.08.11
 */

/**
 * Spoemendas_Model_Emendadetalheptres: Tabela que armazena os valores da emenda por PTRES e Plano Orçamentário
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Emendadetalheptres($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Emendadetalheptres($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $edpid  - default: nextval('spoemendas.emendadetalheptres_edpid_seq'::regclass)
 * @property int $emdid Identificador do detalhe da emenda
 * @property numeric $emdvalor Valor do detalhe no PTRES
 * @property string $ptres Código do Plano de Trabalho Resumido (PTRES)
 * @property string $plocod Código do Plano Orçamentário
 */
class Spoemendas_Model_Emendadetalheptres extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.emendadetalheptres';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'edpid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'emdid' => array('tabela' => 'emenda.emendadetalhe', 'pk' => 'emdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'edpid' => null,
        'emdid' => null,
        'emdvalor' => null,
        'ptres' => null,
        'plocod' => null,
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
            'edpid' => array('Digits'),
            'emdid' => array('Digits'),
            'emdvalor' => array('allowEmpty' => true),
            'ptres' => array(new Zend_Validate_StringLength(array('max' => 6))),
            'plocod' => array(new Zend_Validate_StringLength(array('max' => 4))),
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

    public function getTotalValor($emdid = null) {
        if(!empty($emdid)){
            $sql = <<<SQL
              SELECT sum(emdvalor) FROM spoemendas.emendadetalheptres WHERE emdid = {$emdid}
SQL;
            $total = $this->pegaUm($sql);
            return $total;
        } else {
            throw new Exception('Emenda não informada.');
        }
    }

}
