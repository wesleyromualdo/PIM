<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_grupos_tipos_objeto.
 *
 * @version $Id$
 * @since 2017.06.16
 */

/**
 * Par3_Model_Itenscomposicao_grupos_tipos_objeto: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Itenscomposicao_grupos_tipos_objeto($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Itenscomposicao_grupos_tipos_objeto($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $igoid Chave Primária - default: nextval('par3.itenscomposicao_grupos_tipos_objeto_igoid_seq'::regclass)
 * @property int $igrid Chave estrangeira par3.itenscomposicao_grupos - default: nextval('par3.itenscomposicao_grupos_tipos_objeto_igrid_seq'::regclass)
 * @property int $intoid Chave estrangeira par3.iniciativa_tipos_objeto - default: nextval('par3.itenscomposicao_grupos_tipos_objeto_intoid_seq'::regclass)
 */
class Par3_Model_ItensComposicaoGruposTiposObjeto extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_grupos_tipos_objeto';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'igoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'intoid' => array('tabela' => 'par3.iniciativa_tipos_objeto', 'pk' => 'intoid'),
        'igrid' => array('tabela'  => 'par3.itenscomposicao_grupos',  'pk' => 'igrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'igoid' => null,
        'igrid' => null,
        'intoid' => null,
    );

    /**
     * @param $$igrid int
     * @param $intoid array
     */
    public function salvarItensGruposTiposObjeto($igrid, $arrInto)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where igrid = {$igrid}";
        $this->executar($query_delete);
        foreach ($arrInto as $into){
            $sql = "INSERT INTO {$this->stNomeTabela}(igrid, intoid)VALUES ($igrid,'{$into}')";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function recuperarPorGrupo($igrid = null)
    {
        $arrIgo = array();
        if($igrid){
            $sql = "
            SELECT 
                igo.intoid as codigo
            FROM {$this->stNomeTabela} as igo
            WHERE igo.igrid = $igrid
          ";
            $rsIgo = $this->carregar($sql);
            if(is_array($rsIgo)){
                foreach ($rsIgo as $igo){
                    $arrIgo[] = $igo['codigo'];
                }
            }
        }
        return $arrIgo;
    }
}
