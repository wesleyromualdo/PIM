<?php
/**
 * Classe de mapeamento da entidade par.itemparecercacs.
 *
 * @version $Id$
 * @since 2016.10.17
 */

/**
 * Par_Model_Itemparecercacs: sem descricao
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par_Model_Itemparecercacs($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par_Model_Itemparecercacs($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ipcacsid  - default: nextval('par.itemparecercacs_ipcacsid_seq'::regclass)
 * @property int $icoid 
 * @property int $pcacsid 
 * @property bool $ipcacsvalidado 
 */
class Par_Model_Itemparecercacs extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.itemparecercacs';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipcacsid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pcacsid' => array('tabela' => 'par.parecercacs', 'pk' => 'pcacsid'),
        'icoid' => array('tabela' => 'par.subacaoitenscomposicao', 'pk' => 'icoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ipcacsid' => null,
        'icoid' => null,
        'pcacsid' => null,
        'ipcacsvalidado' => null,
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
            'ipcacsid' => array('Digits'),
            'icoid' => array('allowEmpty' => true,'Digits'),
            'pcacsid' => array('allowEmpty' => true,'Digits'),
            'ipcacsvalidado' => array('allowEmpty' => true),
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
