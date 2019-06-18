<?php
class Par3_Model_ObraTipoEtapaEnsino extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_ensino_etapa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array('oteid');

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otpid' => array('tabela' => 'par3.obra_tipo', 'pk' => 'otpid'),
        'etaid' => array('tabela' => 'par3.ensino_etapa', 'pk' => 'etaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'oteid' => null,
        'otpid' => null,
        'etaid' => null,
    );

    public function salvarEnsinoTipoObra($otpid, $arrEtapas)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where otpid = {$otpid}";
        $this->executar($query_delete);
        foreach ($arrEtapas as $etaid){
            if($etaid != null){
                $sql = "INSERT INTO {$this->stNomeTabela}(otpid, etaid) VALUES ($otpid,$etaid)";
            $this->executar($sql);
            $this->commit();
            }
        }
    }

    public function pegarSQLSelectCombo($nivid = null,$etaid = null)
    {
        $where  = $nivid?"WHERE nivid = $nivid":'';
        $where .= $etaid?"AND etaid = $etaid":'';
        return "SELECT  etaid as codigo,etadsc as descricao FROM {$this->stNomeTabela} $where";
    }

    public function pegarSQLSelectComboNivelEtapa($otpid = 0)
    {
        $sql = "SELECT ote.etaid as codigo
                FROM {$this->stNomeTabela} AS ote
                INNER JOIN par3.ensino_etapa AS eta ON ote.etaid = eta.etaid";

        if($otpid > 0){
            $sql .= " WHERE ote.otpid = ".$otpid."";
        }
      
        $busca = $this->carregar($sql);
        $etaid = array();
        
        if($this->pegalinha($sql) > 0){
            foreach($busca as $key=>$b){
                array_push($etaid, $b['codigo']);
            }    
        }
        
        return $etaid;
    }
}
