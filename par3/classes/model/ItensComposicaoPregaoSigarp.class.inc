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
class Par3_Model_ItensComposicaoPregaoSigarp extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.sigarppregao';

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
        'sprid' => null,
        'sprnuseqpregao' => null,
        'sprnupregao' => null,
        'sprdtinicio' => null,
        'sprdtfim' => null,
        'sprdtinclusao' => null,

    );


    public function sqlDataGrid($arrPost)
    {


        if ($arrPost['sprid'] != "" || $arrPost['sprnupregao'] != "") {

            $where = $arrPost['sprid'] ? " AND sprid = '{$arrPost['sprid']}'" : '';
            $where .= $arrPost['sprnupregao'] ? " AND sprnupregao LIKE '%{$arrPost['sprnupregao']}%'" : '';

        }

        $sql = "
              
				SELECT
				sprid as acao,
				sprid,
				sprnupregao as pregao,
				TO_CHAR(sprdtinicio, 'dd/mm/YYYY') as dtinicio,
				TO_CHAR(sprdtfim, 'dd/mm/YYYY') as dtfim,
				TO_CHAR(sprdtinclusao, 'dd/mm/YYYY') as dtinclusao
			FROM
				par.sigarppregao
		
				
			  WHERE 1 = 1 $where
			  ORDER BY
				sprid
        ";

        $sql = ($sql);
        //print_r($sql);exit;
        return $sql;
    }


    public function sqlDataGridImprecoes($arrPost)
    {


        if ($arrPost['sprid'] != "" || $arrPost['sprnupregao'] != "") {

            $where = $arrPost['sprid'] ? " AND sprid = '{$arrPost['sprid']}'" : '';
            $where .= $arrPost['sprnupregao'] ? " AND sprnupregao LIKE '%{$arrPost['sprnupregao']}%'" : '';

        }

        $sql = "
                  SELECT  
                sprid as acao,
				sprid,
				('Nº '||sprnupregao) as pregao,
				TO_CHAR(sprdtinicio, 'dd/mm/YYYY') as dtinicio,
				TO_CHAR(sprdtfim, 'dd/mm/YYYY') as dtfim,
				TO_CHAR(sprdtinclusao, 'dd/mm/YYYY') as dtinclusao,
				CASE WHEN (
                            SELECT
                                ARRAY_TO_STRING(
                                    ARRAY(
                                        SELECT sit.sitdsc  FROM par.sigarppregaodados spd INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid WHERE spd.sprid = spr.sprid group by sit.sitdsc
                                    ),
                                     '<br>'
                                )
                        ) = '' THEN '-' ELSE 

				   (SELECT
                                ARRAY_TO_STRING(
                                    ARRAY(
                                        SELECT sit.sitdsc  FROM par.sigarppregaodados spd INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid WHERE spd.sprid = spr.sprid group by sit.sitdsc
                                    ),
                                     '<br>'
                                )
                          )END AS descricao,



                          

                       	CASE WHEN (
                            SELECT
                                ARRAY_TO_STRING(
                                    ARRAY(
                                        SELECT spd.spduf FROM par.sigarppregaodados spd INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid WHERE spd.sprid = spr.sprid group by spd.spduf
                                    ),
                                     '-'
                                )
                        ) = '' THEN '-' ELSE 
                        (  SELECT
                                ARRAY_TO_STRING(
                                    ARRAY(
                                        SELECT spd.spduf FROM par.sigarppregaodados spd INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid WHERE spd.sprid = spr.sprid group by spd.spduf
                                    ),
                                     '-'
                                )
                                )END AS uf,


                        
                       CASE WHEN (
                            SELECT
                                ARRAY_TO_STRING(
                                    ARRAY(
                                        SELECT CASE WHEN spd.spdvalor = 0 THEN 100  ELSE spd.spdvalor END FROM par.sigarppregaodados spd INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid WHERE spd.sprid = spr.sprid group by spd.spdvalor
                                    ),
                                     ','
                                )
                         ) = '' THEN '-' ELSE 
                         (
                            SELECT
                                ARRAY_TO_STRING(
                                    ARRAY(
                                        SELECT CASE WHEN spd.spdvalor = 0 THEN 100  ELSE spd.spdvalor END FROM par.sigarppregaodados spd INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid WHERE spd.sprid = spr.sprid group by spd.spdvalor
                                    ),
                                     ','
                                )
                         )END AS valor 
                        
				
			FROM
				par.sigarppregao spr

				  WHERE 1 = 1 $where
				  ORDER BY
				spr.sprid






        ";


        return $sql;
    }


}
