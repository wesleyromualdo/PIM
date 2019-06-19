<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_desdobramentos.
 *
 * @version $Id$
 * @since 2017.07.18
 */

/**
 * Par3_Model_Iniciativa_planejamento_desdobramentos: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_desdobramentos($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_desdobramentos($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property \Datetime(Y-m-d H:i:s) $ipddtinclusao
 * @property int $desid
 * @property int $inpid
 * @property int $ipdid  - default: nextval('par3.iniciativa_planejamento_desdobramentos_ipdid_seq'::regclass)
 */
class Par3_Model_IniciativaPlanejamentoDesdobramentos extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_desdobramentos';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipdid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'desid' => array('tabela' => 'par3.iniciativa_desdobramento', 'pk' => 'desid'),
        'inpid' => array('tabela' => 'par3.iniciativa_planejamento', 'pk' => 'inpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ipddtinclusao' => null,
        'desid' => null,
        'inpid' => null,
        'ipdid' => null,
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
            'ipddtinclusao' => array('allowEmpty' => true),
            'desid' => array('Digits'),
            'inpid' => array('Digits'),
            'ipdid' => array('Digits'),
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

    public function salvarDesdobramentos($inpid, $arrDesdobramentos)
    {
        $query_delete = "DELETE FROM {$this->stNomeTabela} where inpid = {$inpid}";
        $this->executar($query_delete);

//        if(in_array('TODOS',$arrDesdobramentos)){
//            $mEst = new Territorios_Model_Estado();
//            $arrEst = $mEst->recuperarTodos('estuf');
//            foreach ($arrEst as $est){
//                $sql = "INSERT INTO {$this->stNomeTabela}(itdid, estuf)VALUES ($itdid,'{$est['estuf']}')";
//                $this->executar($sql);
//                $this->commit();'''
//            }
//            return true;
//        }
        foreach ($arrDesdobramentos as $desid){
            $sql = "INSERT INTO {$this->stNomeTabela}(inpid, desid)VALUES ($inpid,'{$desid}')";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function recuperarDesdobramentos($inpid = null)
    {
        $arrIpd = array();
        if($inpid){
            $sql = "
            SELECT 
                des.desid as codigo
            FROM {$this->stNomeTabela} as ipd
            INNER JOIN par3.iniciativa_desdobramento as des ON des.desid = ipd.desid
            WHERE ipd.inpid = $inpid
          ";
            $rsIpd = $this->carregar($sql);
            if(is_array($rsIpd)){
                foreach ($rsIpd as $ipd){
                    $arrIpd[] = $ipd['codigo'];
                }
            }
        }
        return $arrIpd;
    }

    public function recuperarDesdobramentosEscolhida($inpid = null)
    {
        $arrIpd = array();
        if($inpid){
            $sql = "
                    SELECT 
                        des.desid as codigo,
                        des.desdsc || ' - ' || tip.tipdsc as descricao
                    FROM {$this->stNomeTabela} as ipd
                    INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
                    INNER JOIN par3.iniciativa_desdobramento_tipo tip ON tip.tipid = des.tipid
                    WHERE ipd.inpid = $inpid
          ";
            $rsIpd = $this->carregar($sql);
            if(is_array($rsIpd)){
                foreach ($rsIpd as $ipd){
                    $arrIpd[] = $ipd['descricao'];
                }
            }
        }
        return $arrIpd;
    }

    public function verificaLigacaoIniciativaPlanejamentoDesdobramento($desid){
        $sql = "SELECT * FROM par3.iniciativa_planejamento_desdobramentos iniplandesd WHERE iniplandesd.desid = {$desid}";
        $result = $this->carregar($sql);
        return $result;
    }//end verificaLigacaoIniciativaPlanejamentoDesdobramento

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $array
     * @return array|mixed|NULL
     * @throws Exception
     * @description Esta função retorna os desdobramentos cadastrados
     */
    public function getDesdobramentosIniciativaPlanejamento(array $array){
        $sql = "select 
                    iniplandesd.desid
                    from par3.iniciativa_planejamento as iniplan 
                    inner join par3.iniciativa_planejamento_desdobramentos as iniplandesd on iniplandesd.inpid = iniplan.inpid
                    where  
                    iniplan.inuid = {$array['inuid']}
                    and
                    iniplan.dimid = {$array['dimid']}
                    and 
                    iniplan.iniid = {$array['iniid']}
                    and
                    etaid = {$array['etaid']}
                    and 
                    modid = {$array['modid']}
                    group by iniplandesd.desid";
        try{
            return $this->carregar($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }//end getDesdobramentosIniciativaPlanejamento


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $array
     * @param $desid
     * @return bool
     * @description <Esta função é utilizada para ver se determinado desdobramento existe >
     */
    public function verificaExistenciaDoDesdobramentoByIdDesdobramento(array $array, $desid){
        $sql = "select 
                    iniplandesd.desid
                    from par3.iniciativa_planejamento as iniplan 
                    inner join par3.iniciativa_planejamento_desdobramentos as iniplandesd on iniplandesd.inpid = iniplan.inpid
                    where  
                    iniplan.inuid = {$array['inuid']}
                    and
                    iniplan.dimid = {$array['dimid']}
                    and 
                    iniplan.iniid = {$array['iniid']}
                    and
                    iniplan.etaid = {$array['etaid']}
                    and 
                    iniplan.modid = {$array['modid']}
                    and
                    iniplandesd.desid = {$desid}
                    and
                    inpstatus = 'A';";
        return $this->carregar($sql);
    }
}
