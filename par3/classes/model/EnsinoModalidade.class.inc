<?php

/**
 * Classe de mapeamento da entidade par3.ensino_etapa.
 *
 * @version $Id$
 * @since 2017.05.08
 */

/**
 * Model_Ensino_etapa: sem descricao
 *
 * @package Model\Ensino
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_EnsinoModalidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_EnsinoModalidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $modid Chave estrangeira que faz referência a tabela par3.modalidade
 * @property int $etaid Chave estrangeira que faz referência a tabela par3.ensino_etapa

 */
class Par3_Model_EnsinoModalidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.ensino_modalidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array('enmid');

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'modid' => array('tabela' => 'par3.modalidade', 'pk' => 'modid'),
        'etaid' => array('tabela' => 'par3.ensino_modalidade', 'pk' => 'etaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'enmid' => null,
        'modid' => null,
        'etaid' => null,
    );

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


    /**
     * @param int $modid
     * @param array() $arrEnsModalidades
     */
    public function salvarEnsinoModalidades($modid, $arrEtapas)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where modid = {$modid}";
        $this->executar($query_delete);
        foreach ($arrEtapas as $etaid){
            $sql = "INSERT INTO {$this->stNomeTabela}(modid, etaid)VALUES ($modid,$etaid)";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function pegarSQLSelectCombo($nivid = null,$etaid = null)
    {
        $where  = $nivid?"WHERE nivid = $nivid":'';
        $where .= $etaid?"AND etaid = $etaid":'';
        return "SELECT  etaid as codigo,etadsc as descricao FROM {$this->stNomeTabela} $where";
    }


    public function getModalidadeNiveisEtapas($modid)
    {
        $sql = "SELECT
                    mod.modid as id,
                    mod.moddsc as descricao,
                    (SELECT array(
                        SELECT  DISTINCT
                            niv.nivid as nivel
                        FROM
                            par3.ensino_modalidade as ensmod
                        INNER JOIN par3.ensino_etapa as eta ON  ensmod.etaid = eta.etaid
                        INNER JOIN par3.ensino_nivel as niv ON  niv.nivid = eta.nivid
                        WHERE ensmod.modid = mod.modid)
                    ) as niveis,
                    (SELECT array(
                        SELECT  DISTINCT
                            eta.etaid as etapa
                        FROM
                            par3.ensino_modalidade as ensmod
                        INNER JOIN par3.ensino_etapa as eta ON  ensmod.etaid = eta.etaid
                        WHERE ensmod.modid = mod.modid)
                ) as etapas
                FROM
                   par3.modalidade as mod
                WHERE mod.modid = {$modid}";

        return $this->recuperar($sql);
    }

    public function pegarSQLSelectComboNivelEtapa($modid,$etaid = null)
    {
        return "SELECT  niv.nivid as codigo,niv.nivdsc as descricao
                FROM par3.ensino_nivel AS niv
                INNER JOIN par3.ensino_etapa AS eta ON niv.nivid = eta.nivid
                INNER JOIN {$this->stNomeTabela} as ensmod ON ensmod.etaid = eta.etaid
                WHERE ensmod.modid = {$modid}
              ";
    }



    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $array
     * @return string
     * @description Esta função retorna o sql que monta o simec->select() de modalidade filtrando pelo ensino etapa etaid
     */
    public function getFormListModalidadeByEnsinoEtapa(array $array){
        $stringIN = retornValuesStringINSQL($array,'int');
        $sql = "select distinct 
                    modal.modid as codigo,  
                    modal.moddsc as descricao
                        from par3.modalidade as modal
                            inner join par3.ensino_modalidade as enmod on enmod.modid = modal.modid
                            inner join par3.ensino_etapa as eta on eta.etaid = enmod.etaid
                            where 
                                enmod.etaid in({$stringIN}) 
                                and modal.modstatus = 'A' 
                                and modal.modsituacao = 'A' 
                                order by modal.moddsc asc;";
        return $sql;
    }
}
