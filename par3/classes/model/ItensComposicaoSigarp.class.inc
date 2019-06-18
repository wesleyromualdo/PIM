<?php
header("Content-Type: text/html; charset=ISO-8859-1", true);

/**
 * Classe de mapeamento da entidade par3.itenscomposicao_categoria_sigarp.
 *
 * @version $Id$
 * @since 2017.06.16
 */

/**
 * Par3_Controller_ItensComposicaoPregaoSicarp: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais<hemersonmorais@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Controller_ItensComposicaoPregaoSicarp($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Controller_ItensComposicaoPregaoSicarp($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_ItensComposicaoSigarp extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.sigarpitem';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'sprid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array();

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'sitid' => null,
        'sitdsc' => null,
        'sprnupregao' => null,
        'sctcodigo' => null,
        'sctdsc' => null,
        'sctdtinclusao' => null,

    );

    public function sqlDataGrid($arrPost)
    {


        if ($arrPost['sitcodigo'] != "" || $arrPost['sitdsc'] != "" || $arrPost['sctdtinclusao'] != "" || $arrPost['sctcodigo'] != "" ) {

            $where = $arrPost['sitcodigo'] ? " AND sitcodigo = '{$arrPost['sitcodigo']}'" : '';
            $where .= $arrPost['sitdsc'] ? " AND upper(sitdsc) LIKE upper('%{$arrPost['sitdsc']}%')" : '';
            $where .= $arrPost['sctdtinclusao'] ? " AND  TO_CHAR(sit.sitdtinclusao, 'dd/mm/YYYY') = '{$arrPost['sctdtinclusao']}'" : '';
            $where .= $arrPost['sctcodigo'] ? " AND sctcodigo = '{$arrPost['sctcodigo']}'" : '';

        }

        $sql = "
              SELECT
				sit.sitid as id,
				sit.sitcodigo as codigo,
				sit.sitdsc as descricao,
				TO_CHAR(sit.sitdtinclusao, 'dd/mm/YYYY') as data,
				sct.sctcodigo as codigo_categoria,
				sct.sctdsc as descricao_categoria,
				TO_CHAR(sct.sctdtinclusao, 'dd/mm/YYYY') as data_categoria
			FROM
				par.sigarpitem sit
			INNER JOIN par.sigarpcategoria sct ON sct.sctid = sit.sctid
		    WHERE 1 = 1 $where
			
			ORDER BY
				sit.sitcodigo, sit.sitdsc
	
		
        ";
        $sql = ($sql);
        return $sql;
    }


}
