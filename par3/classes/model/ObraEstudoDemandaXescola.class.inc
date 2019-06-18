<?php
/**
 * Classe de mapeamento da entidade par3.obra_estudo_demanda_x_escola.
 *
 * @version $Id$
 * @since 2017.08.17
 */

/**
 * Model_Obra_estudo_demanda_x_escola: sem descricao
 *
 * @package Model\Obra
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Obra_estudo_demanda_x_escola($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Obra_estudo_demanda_x_escola($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $ederemanejamento_justificativa
 * @property int $ederemanejamento_medio
 * @property int $ederemanejamento_fundamental
 * @property int $ederemanejamento_infantil
 * @property int $esccodinep
 * @property int $oedid
 */
class Par3_Model_ObraEstudoDemandaXescola extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_estudo_demanda_x_escola';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array();

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'esccodinep' => array('tabela' => 'par3.escola', 'pk' => 'esccodinep'),
        'oedid' => array('tabela' => 'par3.obra_estudo_demanda', 'pk' => 'oedid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ederemanejamento_justificativa' => null,
        'ederemanejamento_medio' => null,
        'ederemanejamento_fundamental' => null,
        'ederemanejamento_infantil' => null,
        'esccodinep' => null,
        'esccodinep' => null,
        'oedid' => null,
        'oedid' => null,
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
            'ederemanejamento_justificativa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'ederemanejamento_medio' => array('allowEmpty' => true,'Digits'),
            'ederemanejamento_fundamental' => array('allowEmpty' => true,'Digits'),
            'ederemanejamento_infantil' => array('allowEmpty' => true,'Digits'),
            'esccodinep' => array('Digits'),
            'esccodinep' => array('Digits'),
            'oedid' => array('Digits'),
            'oedid' => array('Digits'),
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

    public function salvarEscolas($oedid,$arrEscid = array(),$arrEscQtd,$etaid)
    {
        if(empty($etaid) || empty($arrEscid)){
            return false;
        }
        $strEscIid = implode(',',$arrEscid);
        $query_delete = "DELETE FROM {$this->stNomeTabela} where oedid = {$oedid} AND escid in({$strEscIid})";
        $this->executar($query_delete);

        $ederemanejamento = '';
        if($etaid == 1){$ederemanejamento = 'ederemanejamento_infantil';}
        if($etaid == 2){$ederemanejamento = 'ederemanejamento_fundamental';}
        if($etaid == 3){$ederemanejamento = 'ederemanejamento_medio';}
        for($i = 0;$i < count($arrEscid);$i++){
            if(empty($arrEscQtd[$i])){
                continue;
            }
            $sql = "INSERT INTO {$this->stNomeTabela}(oedid, escid,$ederemanejamento)VALUES ({$oedid},{$arrEscid[$i]},{$arrEscQtd[$i]})";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function recuperarEscolas($oedid = null,$etaid)
    {
        $arrEsc = array();
        $whereOed = ($oedid != null? "AND oedXesc.oedid = {$oedid}" : '');
        $colunaTipoEscolaOedXesc = '';
        $colunaTipoEscolaEsc     = '';
        switch ($etaid){
            case 1:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_infantil';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_infantil';
                break;
            case 2:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_fundamental';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_fundamental';
                break;
            case 3:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_medio';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_medio';
                break;
            default:
                break;
        }
        if($oedid){
            $sql = "
            SELECT 
                {$colunaTipoEscolaEsc},
                {$colunaTipoEscolaEsc}::numeric - (select coalesce(sum({$colunaTipoEscolaOedXesc}), 0) as qtd
                from par3.obra_estudo_demanda_x_escola oedXescll
                INNER JOIN par3.obra_estudo_demanda estd ON oedXescll.oedid = estd.oedid
				INNER JOIN par3.obra obr ON obr.obrid = estd.obrid
				INNER JOIN par3.iniciativa_planejamento inp on inp.inpid = obr.inpid
                where escid = esc.escid and oedXescll.oedid <> oedXesc.oedid and obr.obrstatus = 'A' and inp.inpstatus = 'A')::numeric as qtd,
                *
            FROM par3.obra_estudo_demanda_x_escola 	oedXesc
            INNER JOIN par3.escola esc ON esc.escid = oedXesc.escid
            WHERE (
                {$colunaTipoEscolaEsc}::numeric - 
                (
                  select coalesce(sum($colunaTipoEscolaOedXesc), 0) as qtd
                  from par3.obra_estudo_demanda_x_escola oedXescll
                  INNER JOIN par3.obra_estudo_demanda estd ON oedXescll.oedid = estd.oedid
				  INNER JOIN par3.obra obr ON obr.obrid = estd.obrid
				  INNER JOIN par3.iniciativa_planejamento inp on inp.inpid = obr.inpid
                  where escid = esc.escid and oedXescll.oedid <> oedXesc.oedid and obr.obrstatus = 'A' and inp.inpstatus = 'A' )::numeric
                ) > 0
                {$whereOed}
                ";


            $rsEsc = $this->carregar($sql);
            if(is_array($rsEsc)){
                foreach ($rsEsc as $esc){
                    $arrEsc[] = $esc;
                }
            }
        }
        return is_array($arrEsc)?$arrEsc:array();
    }

    public function removerEscola(array $arrPost){
        try {
            $query_delete = "DELETE FROM {$this->stNomeTabela} where oedid = {$arrPost['oedid']} AND escid in({$arrPost['escid']})";
            $sucesso = $this->executar($query_delete);
            $this->commit();
            return $sucesso;
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }

    public function listarEscolas($oedid,$etaid,$escid = null, $inuid,$ignorarDesdobramentos = null,$escidValidar = array())
    {
        $mdInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $colunaTipoEscolaOedXesc = '';
        $colunaTipoEscolaEsc     = '';

        if(!$ignorarDesdobramentos) {
            $inp = new Par3_Controller_IniciativaPlanejamento();
            $desdobramentos = $inp->getDesdobramentos($_REQUEST['inpid']);
            $filtroLocalizacao = '';

            $filtroLocalizacao = 'AND esc.escco_localizacao IN(1,2)';
            //Verifica se existe desdobramento urbano
            if(in_array(DESDOBRAMENTO_URBANO,$desdobramentos)){
                $filtroLocalizacao = 'AND esc.escco_localizacao = 1';
            }

            //Verifica se existe desdobramento rural
            if(in_array(DESDOBRAMENTO_RURAL,$desdobramentos)){
                $filtroLocalizacao = 'AND esc.escco_localizacao = 2';
            }

            //Verifica se existe desdobramento urbano e rural na iniciativa
            if(in_array(array(DESDOBRAMENTO_URBANO,DESDOBRAMENTO_RURAL),$desdobramentos)){
                $filtroLocalizacao = 'AND esc.escco_localizacao IN(1,2)';
            }
        }

        switch ($etaid){
            case 1:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_infantil';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_infantil';
                break;
            case 2:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_fundamental';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_fundamental';
                break;
            case 3:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_medio';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_medio';
                break;
            default:
                break;
        }
        $arrEsc = array();
        $where = array("({$colunaTipoEscolaEsc}::numeric - 
                        (
                          select DISTINCT coalesce(sum($colunaTipoEscolaOedXesc), 0) as qtd
                            from par3.obra_estudo_demanda_x_escola oedXescll
                  		    INNER JOIN par3.obra_estudo_demanda estd ON oedXescll.oedid = estd.oedid
						    INNER JOIN par3.obra obr ON obr.obrid = estd.obrid
						    INNER JOIN par3.iniciativa_planejamento inp on inp.inpid = obr.inpid
                          where escid = esc.escid and oedXescll.oedid <> {$oedid} and obr.obrstatus = 'A' and inp.inpstatus = 'A')::numeric
                        ) > 0");
        if($mdInstrumentoUnidade->itrid == 2){
            $where[] = "esc.escco_esfera = 'M'";
            $where[] = "esc.muncod = '{$mdInstrumentoUnidade->muncod}'";
        }else{
            $where[] = "esc.escco_esfera = 'E'";
            $where[] = "esc.estuf = '{$mdInstrumentoUnidade->estuf}'";
        }

        $orEscid = '';
        if(count($escidValidar) > 0) {
            $implodeEscid = implode(',',$escidValidar);
            $orEscid = " OR esc.escid in({$implodeEscid})";
        }
        $where[] = " (esc.escco_situacao_funcoinamento in(1,2) {$orEscid} )";

        if($escid) {
            $where[] = "esc.escid in({$escid})";
        }

        $sql = "
            SELECT DISTINCT 
                {$colunaTipoEscolaEsc},
                {$colunaTipoEscolaEsc}::numeric - (select DISTINCT coalesce(sum({$colunaTipoEscolaOedXesc}), 0) as qtd
                from par3.obra_estudo_demanda_x_escola oedXescll
                INNER JOIN par3.obra_estudo_demanda estd ON oedXescll.oedid = estd.oedid
				INNER JOIN par3.obra obr ON obr.obrid = estd.obrid
				INNER JOIN par3.iniciativa_planejamento inp on inp.inpid = obr.inpid
                where escid = esc.escid and oedXescll.oedid <> {$oedid}  and obr.obrstatus = 'A' and inp.inpstatus = 'A')::numeric as qtd,
                *,esc.escid as codigo, esc.escnome as descricao
            FROM par3.escola esc
            WHERE ".implode(' AND ', $where)."
            {$filtroLocalizacao}
            ORDER BY
                escnome";
        //ver($sql);
        $rsEsc = $this->carregar($sql);
        if(is_array($rsEsc)){
            foreach ($rsEsc as $esc){
                $arrEsc[] = $esc;
            }
        }
        return is_array($arrEsc)?$arrEsc:array();
    }
}