<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_detalhamento_estado.
 *
 * @version $Id$
 * @since 2017.06.12
 */

/**
 * Par3_Model_Itenscomposicao_detalhamento_estado: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Itenscomposicao_detalhamento_estado($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Itenscomposicao_detalhamento_estado($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ideid Chave Primária - default: nextval('par3.itenscomposicao_detalhamento_estado_ideid_seq'::regclass)
 * @property int $itdid Chave estrangeira par3.itenscomposicao_detalhamento - default: nextval('par3.itenscomposicao_detalhamento_estado_itdid_seq'::regclass)
 * @property string $estuf Chave estrangeira territorios.estado
 */
class Par3_Model_ItensComposicaoDetalhamentoEstado extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_detalhamento_estado';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ideid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'itdid' => array('tabela' => 'par3.itenscomposicao_detalhamento', 'pk' => 'itdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ideid' => null,
        'itdid' => null,
        'estuf' => null,
    );

    public function salvarItemDetalhamentoEstado($itdid, $arrEstados, $insert = true)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where itdid = {$itdid}";
        $this->executar($query_delete);

        if(in_array('TODOS',$arrEstados)){
            $mEst = new Territorios_Model_Estado();
            $arrEst = $mEst->recuperarTodos('estuf');
            foreach ($arrEst as $est){
                $sql = "INSERT INTO {$this->stNomeTabela}(itdid, estuf)VALUES ($itdid,'{$est['estuf']}')";
                $this->executar($sql);
                $this->commit();
            }
            return true;
        }
        foreach ($arrEstados as $estuf){
            
            if($insert){
                $sql = "SELECT DISTINCT
                        	't' as resp
                        FROM
                        	par3.itenscomposicao_detalhamento itd
                        INNER JOIN par3.itenscomposicao_detalhamento_estado ide ON ide.itdid = itd.itdid
                        WHERE
                            itd.itdstatus = 'A' AND
                            itd.itcid = (SELECT DISTINCT itcid FROM par3.itenscomposicao_detalhamento WHERE itdid = $itdid) AND
                        	(
                        		(SELECT itdperiodoinicio 
                        		FROM par3.itenscomposicao_detalhamento
                        		WHERE itdid = $itdid) BETWEEN itdperiodoinicio AND itdperiodofim OR
                        		(SELECT itdperiodofim 
                        		FROM par3.itenscomposicao_detalhamento 
                        		WHERE itdid = $itdid) BETWEEN itdperiodoinicio AND itdperiodofim 
                        	) AND
                        	estuf = '$estuf';";
                
                $conflito = $this->pegaUm($sql);
            }
            
            $arrErro['erros']['estuf'] = array();
            if($conflito != 't'){
                $sql = "INSERT INTO {$this->stNomeTabela}(itdid, estuf)VALUES ($itdid,'{$estuf}')";
                $this->executar($sql);
                $this->commit();
            }else{
                $arrErro['erros']['estuf'][] = "UF $estuf já possui um detalhamento neste período.";
            }
        }
        if($arrErro['erros']['estuf'][0] != ''){
            $sql = "UPDATE par3.itenscomposicao_detalhamento SET itdstatus = 'I' WHERE itdid = $itdid;";
            $this->executar($sql);
            $this->commit();
            echo simec_json_encode($arrErro);
            die();
        }
    }

    public function recuperarPorItemDetalhamento($itdid = null)
    {
        $arrIde = array();
        if($itdid){
            $sql = "
            SELECT 
                est.estuf as codigo
            FROM {$this->stNomeTabela} as ide
            INNER JOIN territorios.estado as est ON est.estuf = ide.estuf
            WHERE ide.itdid = $itdid
          ";
            $rsIde = $this->carregar($sql);
            if(is_array($rsIde)){
                foreach ($rsIde as $ide){
                    $arrIde[] = $ide['codigo'];
                }
            }
        }
        return $arrIde;
    }
}
