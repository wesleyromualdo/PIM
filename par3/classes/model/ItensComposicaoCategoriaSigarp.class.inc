<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);

/**
 * Classe de mapeamento da entidade par3.itenscomposicao_categoria_sigarp.
 *
 * @version $Id$
 * @since 2017.06.16
 */

/**
 * Par3_Model_Itenscomposicao_categoria_sigarp: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais<hemersonmorais@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Itenscomposicao_categoria_sigarp($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Itenscomposicao_categoria_sigarp($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_ItensComposicaoCategoriaSigarp extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.sigarpcategoria';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'sctid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array();

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'sctid' => null,
        'sctcodigo' => null,
        'sctdesc' => null,
        'sctdtinclusao' => null,

    );



    public function sqlDataGrid($arrPost)
    {


        if ($arrPost['sctcodigo'] != "" || $arrPost['sctdsc'] != "") {

            $where = $arrPost['sctcodigo'] ? " AND sct.sctcodigo = '{$arrPost['sctcodigo']}'" : '';
            $where .= $arrPost['sctdsc'] ? " AND upper(sct.sctdsc) LIKE upper('%{$arrPost['sctdsc']}%')" : '';

        }

        $sql = "
            SELECT  
               
				sct.sctcodigo,
				sct.sctdsc,
				TO_CHAR(sct.sctdtinclusao, 'dd/mm/YYYY') as sctdtinclusao
			FROM 
				par.sigarpcategoria sct
				
			  WHERE 1 = 1 $where
			ORDER BY
				sct.sctcodigo, sct.sctdsc
        ";

        $sql = ($sql);

       //print_r($sql);exit;
        return $sql;
    }
}
