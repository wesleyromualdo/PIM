<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_tipos_objeto.
 *
 * @version $Id$
 * @since 2017.06.09
 */

/**
 * Model_Itenscomposicao_tipos_objeto: sem descricao
 *
 * @package Model\Itenscomposicao
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Itenscomposicao_tipos_objeto($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Itenscomposicao_tipos_objeto($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $itoid Chave Primária - default: nextval('par3.itenscomposicao_tipos_objeto_itoid_seq'::regclass)
 * @property int $itcid Chave estrangeira par3.itenscomposicao - default: nextval('par3.itenscomposicao_tipos_objeto_itcid_seq'::regclass)
 * @property int $intoid Chave estrangeira par3.iniciativa_tipos_objeto - default: nextval('par3.itenscomposicao_tipos_objeto_intoid_seq'::regclass)
 */
class Par3_Model_ItensComposicaoTiposObjeto extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_tipos_objeto';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'itoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'intoid' => array('tabela' => 'par3.iniciativa_tipos_objeto', 'pk' => 'intoid'),
        'itcid' => array('tabela' => 'par3.itenscomposicao',          'pk' => 'itcid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'itoid' => null,
        'itcid' => null,
        'intoid' => null,
    );


    /**
     * @param $itcid int
     * @param $intoid array
     */
    public function salvarItensTiposObjeto($itcid, $arrInto)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where itcid = {$itcid}";
        $this->executar($query_delete);
        foreach ($arrInto as $into){
            $sql = "INSERT INTO {$this->stNomeTabela}(itcid, intoid)VALUES ($itcid,'{$into}')";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function recuperarPorItem($itcid = null)
    {
        $arrItc = array();
        if($itcid){
            $sql = "
            SELECT 
                ito.intoid as codigo
            FROM {$this->stNomeTabela} as ito
            WHERE ito.itcid = $itcid
          ";
            $rsItc = $this->carregar($sql);
            if(is_array($rsItc)){
                foreach ($rsItc as $ito){
                    $arrItc[] = $ito['codigo'];
                }
            }
        }
        return $arrItc;
    }
}
