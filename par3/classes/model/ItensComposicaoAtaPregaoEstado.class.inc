<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_ata_pregao_estado.
 *
 * @version $Id$
 * @since 2017.06.05
 */

/**
 * Model_Itenscomposicao_ata_pregao_estado: sem descricao
 *
 * @package Model\Itenscomposicao
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Itenscomposicao_ata_pregao_estado($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Itenscomposicao_ata_pregao_estado($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ateid  - default: nextval('par3.itenscomposicao_ata_pregao_estado_ateid_seq'::regclass)
 * @property int $atpid  - default: nextval('par3.itenscomposicao_ata_pregao_estado_atpid_seq'::regclass)
 * @property string $estuf
 */
class Par3_Model_ItenscomposicaoAtaPregaoEstado extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_ata_pregao_estado';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ateid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'atpid' => array('tabela' => 'par3.itenscomposicao_ata_pregao', 'pk' => 'atpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ateid' => null,
        'atpid' => null,
        'estuf' => null,
    );


    public function salvarAtaPregaoEstado($atpid, $arrEstados)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where atpid = {$atpid}";
        $this->executar($query_delete);

        if(in_array('TODOS',$arrEstados)){
            $mEst = new Territorios_Model_Estado();
            $arrEst = $mEst->recuperarTodos('estuf');
            foreach ($arrEst as $est){
                $sql = "INSERT INTO {$this->stNomeTabela}(atpid, estuf)VALUES ($atpid,'{$est['estuf']}')";
                $this->executar($sql);
                $this->commit();
            }
            return true;
        }

        foreach ($arrEstados as $estuf){
            $sql = "INSERT INTO {$this->stNomeTabela}(atpid, estuf)VALUES ($atpid,'{$estuf}')";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function recuperarPorAta($atpid = null)
    {
        $arrAte = array();
        if($atpid){
            $sql = "
            SELECT 
                est.estuf as codigo
            FROM {$this->stNomeTabela} as ate
            INNER JOIN territorios.estado as est ON est.estuf = ate.estuf
            WHERE ate.atpid = $atpid
          ";
            $rsAte = $this->carregar($sql);
            $arrAte = array();
            if(is_array($rsAte)){
                foreach ($rsAte as $ate){
                    $arrAte[] = $ate['codigo'];
                }
            }
        }
        return $arrAte;
    }
}
