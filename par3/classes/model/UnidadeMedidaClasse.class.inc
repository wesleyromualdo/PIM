<?php
/**
 * Classe de mapeamento da entidade par3.unidade_medida_classe.
 *
 * @version $Id$
 * @since 2017.05.30
 */

/**
 * Model_Unidade_medida_classe: sem descricao
 *
 * @package Model\Unidade
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Unidade_medida_classe($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Unidade_medida_classe($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $uncid Chave Primária - default: nextval('par3.unidade_medida_classe_uncid_seq'::regclass)
 * @property int $uniid Unidade de Medida relacionada - default: nextval('par3.unidade_medida_classe_uniid_seq'::regclass)
 * @property int $cniid Classe de Unidade Da Medida - default: nextval('par3.unidade_medida_classe_cniid_seq'::regclass)
 * @property "char" $uncstatus status de unidade medida classe A/I
 */
class Par3_Model_UnidadeMedidaClasse extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.unidade_medida_classe';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'uncid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'cniid' => array('tabela' => 'par3.classe_unidade_medida', 'pk' => 'cniid'),
        'uniid' => array('tabela' => 'par3.unidade_medida', 'pk' => 'uniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'uncid' => null,
        'uniid' => null,
        'cniid' => null,
        'uncstatus' => null,
    );

    public function recuperarTodos($stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array())
    {
        return parent::recuperarTodos($stCampos, $arClausulas, $stOrdem, $opcoes);
    }

    public function marcarTodosToggle($uniid,$marcar = false)
    {
        $mClasse = new Par3_Model_ClasseUnidadeMedida();
        $arrClasse = $mClasse->listarClassesArray();

        foreach($arrClasse as $classe){
            $rsUnc = array();
            $mUnidadeClase = new $this;

            //Verifica se há Unidade de Medida com Status Ativo utilizando a classe
            $rsUnc = $mUnidadeClase->recuperar("SELECT * FROM par3.unidade_medida_classe WHERE cniid  = {$classe['cniid']} AND uniid = {$uniid}");
            if ($rsUnc == null) {
                $rsUnc = array();
                $rsUnc['uniid'] = $uniid;
                $rsUnc['cniid'] = $classe['cniid'];
            }
            $rsUnc['uncstatus'] = ($marcar == true?'A':'I');
            $mUnidadeClase->popularDadosObjeto($rsUnc);
            $novoUnidadeMedida =  $mUnidadeClase->salvar();//grava Unidade de Medida Classe
        }
        $retorno = $this->commit();
    }
}
