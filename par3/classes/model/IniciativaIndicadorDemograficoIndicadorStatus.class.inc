<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_indicador_demografico_indicador_status.
 *
 * @version $Id$
 * @since 2017.07.18
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
/**
 * Par3_Model_Iniciativa_indicador_demografico_indicador_status: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_indicador_demografico_indicador_status($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_indicador_demografico_indicador_status($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iniindsstatus 
 * @property int $inddid 
 * @property int $iniidsid 
 * @property int $iniid 
 * @property int $iniindsid  - default: nextval('par3.iniciativa_indicador_demografico_indicador_status_iniindsid_seq'::regclass)
 */
class Par3_Model_IniciativaIndicadorDemograficoIndicadorStatus extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_indicador_demografico_indicador_status';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iniindsid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iniidsid' => array('tabela' => 'par3.iniciativa_indicador_demografico_status', 'pk' => 'iniidsid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
        'inddid' => array('tabela' => 'par3.indicador_demografico', 'pk' => 'inddid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iniindsstatus' => null,
        'inddid' => null,
        'iniidsid' => null,
        'iniid' => null,
        'iniindsid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'iniindsstatus' => array('Digits'),
            'inddid' => array('Digits'),
            'iniidsid' => array('Digits'),
            'iniid' => array('Digits'),
            'iniindsid' => array('Digits'),
        );
    }

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
     * @param $iniid
     * @return bool
     * ##VERIFICA SE JÁ EXISTE UMA INICIATIVA CADASTRADA
     */
    public function existsIniciativa($iniid){
        $sql = "SELECT count(iniid) as iniid FROM par3.iniciativa_indicador_demografico_indicador_status WHERE iniid = {$iniid}";
        $result = $this->pegaLinha($sql);
        if($result['iniid'] <= 0){
            return false;
        }else{
            return true;
        }
    }

    /**
     * ##Valida se existir o indicador demográfico ligado a iniciativa e filtro
     * @param $iniid
     * @param $iniidsid
     * @param $indcod
     */
    public function validaSeCadastrado($iniid, $iniidsid, $indcod){
        //var_dump($iniidsid); exit();
        $sql = "SELECT * FROM par3.iniciativa_indicador_demografico_indicador_status iidis 
                INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iidis.iniid
                INNER JOIN par3.indicador_demografico as indicador_demografico ON indicador_demografico.inddid = iidis.inddid
                INNER JOIN par3.iniciativa_indicador_demografico_status as idstatus ON idstatus.iniidsid = iidis.iniidsid
                WHERE iidis.iniid = {$iniid} AND idstatus.iniidsid = {$iniidsid} AND indicador_demografico.indcod = '{$indcod}' AND iniindsstatus = 1;";
        $result = $this->pegaLinha($sql);
       // var_dump($sql);
        if($result['iniid'] == NULL){
            return false;
        }else{
            return true;
        }
    }

    public function getLinhaIndicador($iniid,$iniidsid,$indcod){
        $sql = "SELECT * FROM par3.iniciativa_indicador_demografico_indicador_status iidis 
                INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iidis.iniid
                INNER JOIN par3.indicador_demografico as indicador_demografico ON indicador_demografico.inddid = iidis.inddid
                INNER JOIN par3.iniciativa_indicador_demografico_status as idstatus ON idstatus.iniidsid = iidis.iniidsid
                WHERE iidis.iniid = {$iniid} AND idstatus.iniidsid = {$iniidsid} AND indicador_demografico.indcod = '{$indcod}';";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    public function montaArrayiIndicadoresFiltro(array $array){

        $modelIndicadoresDemograficos = new Par3_Model_IndicadorDemografico();
        $where = "";
        //se não houver nenhum filtro estabelecido checkbox, retorna todos os indicadores demograficos
        if(!isset($array['filterTableIndicadoresStatus'])){
            return $modelIndicadoresDemograficos->fetchAll();
        }
        //Se existir o array com os filtros
        else{
            $where = " WHERE ";
            if(in_array(0,$array['filterTableIndicadoresStatus'])){
                return $modelIndicadoresDemograficos->fetchAll();
            }else{
                $countFiltros = count($array['filterTableIndicadoresStatus']);
                //se houver somente um filtro
                if($countFiltros == 1){
                    if(in_array(1,$array['filterTableIndicadoresStatus'])){
                        $where .= " idstatus.iniidsid = 1 ";
                    }
                    if(in_array(2,$array['filterTableIndicadoresStatus'])){
                        $where .= " idstatus.iniidsid = 2 ";
                    }
                    if(in_array(3,$array['filterTableIndicadoresStatus'])){
                        $where .= " idstatus.iniidsid = 3 ";
                    }
                }
                if($countFiltros > 1){
                    $i = 0;
                    foreach($array['filterTableIndicadoresStatus'] as $a){
                        if($i > 0){
                            $where .= " AND idstatus.iniidsid = {$a}";
                        }else{
                            $where .= " idstatus.iniidsid = {$a}";
                        }
                        $i++;
                    }
                }

                $where .= " AND iidis.iniindsstatus = 1 AND iniciativa.iniid = {$array['iniid']}";


            }
        }

        $orderBy = " ORDER BY indicador_demografico.indcod ASC ";

        $sql = "SELECT DISTINCT 
                    indicador_demografico.indcod,
                    indicador_demografico.indddesc,
                    indicador_demografico.indtabuf,
                    indicador_demografico.indtabmunic
                    FROM par3.iniciativa_indicador_demografico_indicador_status iidis 
                    INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iidis.iniid 
                    INNER JOIN par3.indicador_demografico as indicador_demografico ON indicador_demografico.inddid = iidis.inddid 
                    INNER JOIN par3.iniciativa_indicador_demografico_status as idstatus ON idstatus.iniidsid = iidis.iniidsid 
                {$where} {$orderBy};";

        return $this->carregar($sql);
    }


    public function montaArrayiIndicadoresFiltroTodos(array $array){

        $orderBy = " ORDER BY indicador_demografico.indcod ASC ";

        $sql = "SELECT DISTINCT 
                    indicador_demografico.indcod,
                    indicador_demografico.indddesc,
                    indicador_demografico.indtabuf,
                    indicador_demografico.indtabmunic
                    FROM par3.iniciativa_indicador_demografico_indicador_status iidis 
                    INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iidis.iniid 
                    INNER JOIN par3.indicador_demografico as indicador_demografico ON indicador_demografico.inddid = iidis.inddid 
                    INNER JOIN par3.iniciativa_indicador_demografico_status as idstatus ON idstatus.iniidsid = iidis.iniidsid 
                    WHERE idstatus.iniidsid = 1 AND idstatus.iniidsid = 2 AND idstatus.iniidsid = 3 AND iniciativa.iniid = {$array['iniid']}
                {$orderBy};";
        return $this->carregar($sql);
    }


    public function fetchAllByIniciativa(array $array){

        $orderBy = " ORDER BY indicador_demografico.indcod ASC ";
        $sql = "                SELECT DISTINCT 
                    indicador_demografico.indcod,
                    indicador_demografico.indddesc,
                    indicador_demografico.indtabuf,
                    indicador_demografico.indtabmunic
                    FROM par3.iniciativa_indicador_demografico_indicador_status iidis 
                    INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iidis.iniid 
                    INNER JOIN par3.indicador_demografico as indicador_demografico ON indicador_demografico.inddid = iidis.inddid 
                    INNER JOIN par3.iniciativa_indicador_demografico_status as idstatus ON idstatus.iniidsid = iidis.iniidsid 
                 ORDER BY indicador_demografico.indcod ASC;";
       // ver($sql);
        return $this->carregar($sql);
    }

    public function indicadorComFiltroTodos($indid,$indcod){
        $modeIndicador = new Par3_Model_IndicadorDemografico();
        $getIndicadorDemografico = $modeIndicador->getIndicadorDemograficoByCod(trim(strtolower($indcod)));
        $inddid = $getIndicadorDemografico['inddid'];
        $modelIniciativaIndicadorDemograficoIndicadorStatus = new Par3_Model_IniciativaIndicadorDemograficoIndicadorStatus();
        //Plano de Trabalho
        $validoPl = $modelIniciativaIndicadorDemograficoIndicadorStatus->consultaIniciativaIndicadorStatusTodosAtivos($indid,$inddid,1);
        //Análise Técnica
        $validoAt = $modelIniciativaIndicadorDemograficoIndicadorStatus->consultaIniciativaIndicadorStatusTodosAtivos($indid,$inddid,2);
        //Análise de Mérito
        $validoTm = $modelIniciativaIndicadorDemograficoIndicadorStatus->consultaIniciativaIndicadorStatusTodosAtivos($indid,$inddid,3);
        if($validoPl == true && $validoAt == true && $validoTm == true){
           return true;
       }
       return false;
    }

    public function consultaIniciativaIndicadorStatusTodosAtivos($indid,$inddid,$status){
        $sql = "SELECT 
                iidis.iniid,
                iidis.inddid,
                iidis.iniidsid,
                iidis.iniindsstatus
                FROM par3.iniciativa_indicador_demografico_indicador_status as iidis
                WHERE iidis.inddid = {$inddid} AND iidis.iniid = {$indid} AND iidis.iniidsid = {$status} AND iidis.iniindsstatus = 1";

        $row = $this->pegaLinha($sql);
        if(isset($row['iniindsstatus'])){
           return true;
        }else{
            return false;
        }

    }

    public function indicadorFiltraPorStatusNaoTodos($iniid,$iniidsid,$indcod){
        $modeIndicador = new Par3_Model_IndicadorDemografico();
        $getIndicadorDemografico = $modeIndicador->getIndicadorDemograficoByCod(trim(strtolower($indcod)));
        $sql = "SELECT 
                iidis.iniid,
                iidis.inddid,
                iidis.iniidsid,
                iidis.iniindsstatus
                FROM par3.iniciativa_indicador_demografico_indicador_status as iidis
                WHERE iidis.inddid = {$getIndicadorDemografico['inddid']} AND iidis.iniid = {$iniid} AND iidis.iniidsid = {$iniidsid} AND iidis.iniindsstatus = 1";
        $row = $this->pegaLinha($sql);
        if($row['iniindsstatus'] == 1){
            return true;
        }
        return false;
    }

    /**
     * @param $iniid
     * @param $indicador_status 1 = Plano de Trabalho
     */
    public function getIndicadorePelaIniciativa($iniid,$indicador_status = 1){
        $sql = "SELECT indcod, indddesc FROM par3.iniciativa_indicador_demografico_indicador_status as iidis
                INNER JOIN par3.indicador_demografico as indicador_demografico ON indicador_demografico.inddid = iidis.inddid
                WHERE iidis.iniid = {$iniid} AND iidis.iniidsid = {$indicador_status} AND iidis.iniindsstatus = 1 ORDER BY iidis.inddid ASC";
        $result = $this->carregar($sql);
        return $result;
    }
}
