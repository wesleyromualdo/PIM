<?php
/**
 * Classe de mapeamento da entidade par3.obra_estudo_demanda_x_ampliacao.
 *
 * @version $Id$
 * @since 2017.08.17
 */

/**
 * Model_Obra_estudo_demanda_x_ampliacao: sem descricao
 *
 * @package Model\Obra
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Obra_estudo_demanda_x_ampliacao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Obra_estudo_demanda_x_ampliacao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $oedid
 * @property int $edaid
 */
class Par3_Model_ObraEstudoDemandaXampliacao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_estudo_demanda_x_ampliacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'edaid' => array('tabela' => 'par3.obra_estudo_demanda_ampliacao', 'pk' => 'edaid'),
        'oedid' => array('tabela' => 'par3.obra_estudo_demanda', 'pk' => 'oedid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'oedid' => null,
        'edaid' => null,
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
            'oedid' => array('Digits'),
            'edaid' => array('Digits'),
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

    public function salvarAmpliacoes($oedid, $arrAmpliacoes)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where oedid = {$oedid}";
        $this->executar($query_delete);

        if(in_array('TODOS',$arrAmpliacoes)){
            $mEda  = new Par3_Model_ObraEstudoDemandaAmpliacao();
            $arrEda = $mEda->recuperarTodos('edaid');
            foreach ($arrEda as $eda){
                $sql = "INSERT INTO {$this->stNomeTabela}(oedid, estuf)VALUES ({$oedid},{$eda['edaid']})";
                $this->executar($sql);
                $this->commit();
            }
            return true;
        }
        foreach ($arrAmpliacoes as $edaid){
            $sql = "INSERT INTO {$this->stNomeTabela}(oedid, edaid)VALUES ({$oedid},{$edaid})";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function recuperarAmpliacoes($oedid = null)
    {
        $arrEda = array();
        if($oedid){
            $sql = "
            SELECT 
                eda.edaid as codigo
            FROM {$this->stNomeTabela} as foo
            INNER JOIN par3.obra_estudo_demanda_ampliacao as eda ON eda.edaid = foo.edaid
            WHERE foo.oedid = $oedid
          ";
            $rsEda = $this->carregar($sql);
            if(is_array($rsEda)){
                foreach ($rsEda as $eda){
                    $arrEda[] = $eda['codigo'];
                }
            }
        }
        return $arrEda;
    }
}
