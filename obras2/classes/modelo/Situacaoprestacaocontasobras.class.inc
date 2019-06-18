<?php
/**
 * Classe de mapeamento da entidade obras2.situacaoprestacaocontasobras.
 *
 * @version $Id$
 * @since 2018.08.14
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Obras2_Model_Situacaoprestacaocontasobras: sem descricao
 *
 * @package Obras2\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Obras2_Model_Situacaoprestacaocontasobras($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Obras2_Model_Situacaoprestacaocontasobras($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $spoid  - default: nextval('obras2.situacaoprestacaocontasobras_spoid_seq'::regclass)
 * @property int $proid_par 
 * @property int $proid_pac 
 * @property int $docid 
 * @property string $spostatus  - default: 'A'::bpchar
 */
class Obras2_Model_Situacaoprestacaocontasobras extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'obras2.situacaoprestacaocontasobras';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'spoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'proid_pac' => array('tabela' => 'par.processoobra', 'pk' => 'proid'),
        'proid_par' => array('tabela' => 'par.processoobraspar', 'pk' => 'proid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'spoid' => null,
        'proid_par' => null,
        'proid_pac' => null,
        'docid' => null,
        'spostatus' => null,
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
            'spoid' => array('Digits'),
            'proid_par' => array('allowEmpty' => true,'Digits'),
            'proid_pac' => array('allowEmpty' => true,'Digits'),
            'docid' => array('allowEmpty' => true,'Digits'),
            'spostatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosSemFluxo($arrParams)
    {
        // Recupera parametros
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
    
        // 'par.documentoparprestacaodecontas';
        /*
         * RN = Seleciona todos prpids que não possuam registro na tabela par3.situacaoprestacaocontas
         * RN2 = Que não estajam contemplados pelo SIGPC
         */
        $sqlObrasPar = "
	    	SELECT DISTINCT
				proid,
				pronumeroprocesso
			FROM
				par.processoobraspar
			WHERE
				prostatus = 'A'
			AND
				proid not in (select distinct proid_par from obras2.situacaoprestacaocontasobras WHERE proid_par IS NOT NULL)
			AND
				proid not in (select distinct proid_par from par.termossigpcpar WHERE proid_par IS NOT NULL)
	    	{$offset}
	    	{$limite}
    	";
	    	
        $sqlObrasPac = "
            SELECT DISTINCT
            	proid,
            	pronumeroprocesso
            FROM 
            	par.processoobra  
            WHERE
            	prostatus = 'A'
            AND
            	proid not in (select distinct proid_pac from obras2.situacaoprestacaocontasobras WHERE proid_pac IS NOT NULL)
            AND
				proid not in (select distinct proid_pac from par.termossigpcpar WHERE proid_pac IS NOT NULL)
            {$offset}
            {$limite}
        ";
    
        $arrProcessosObrasPar = $this->carregar($sqlObrasPar);
    	// Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
    	$retornou1 = (is_array($arrProcessosObrasPar)) ? TRUE : FALSE;
    	$registrosPar = (is_array($arrProcessosObrasPar)) ? $arrProcessosObrasPar : Array();

    	$arrProcessosObrasPac = $this->carregar($sqlObrasPac);
    	// Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
    	$retornou2 = (is_array($arrProcessosObrasPac)) ? TRUE : FALSE;
    	$registrosPaC = (is_array($arrProcessosObrasPac)) ? $arrProcessosObrasPac : Array();
    	
    	$retornou = ($retornou1 || $retornou2) ? TRUE : FALSE;
    	
    	$arrReturn = Array(
    			'existeRegistro' => $retornou,
    			'cadastroRegistroPAR' => $registrosPar,
    			'cadastroRegistroPAC' => $registrosPaC
    	);
    	return $arrReturn;
    }
    
    
    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function insereRegistrosInexistenteObrasPar($arrRegistros)
    {
    	
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';
    
        $arrValores = Array();
        foreach ($arrRegistros as $k => $v) {
            $docidFluxo = '';
            $proid = '';
    
            $docidFluxo = wf_cadastrarDocumento(TPDID_FLUXO_SIT_PC_EXEC_FINAN_OBRAS, "Cadastro de documento do Fluxo de situações da prestação de contas Obras PAR - Via acompanhamento", ESDID_SITUACAO_PC_NAO_ENVIADA);
        
            $proid           = $v['proid'];
            $processoObras   = $v['pronumeroprocesso'];
             
    
            if (($proid != '') && ($docidFluxo != '') && ($processoObras != '')) {
                $arrValores[] = "({$proid}, {$docidFluxo}, '{$processoObras}')";
            }
        }
    
        if (count($arrValores) > 0) {
            $sql = "
                INSERT INTO obras2.situacaoprestacaocontasobras
	               (proid_par, docid, spoprocesso)
                VALUES
                ";
            $sql .= implode(",
                            ", $arrValores);
    
            $this->executar($sql);
            return $this->commit();
        }
    
        return FALSE;
    }
    
    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function insereRegistrosInexistenteObrasPac($arrRegistros)
    {
        
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';
    
        $arrValores = Array();
        foreach ($arrRegistros as $k => $v) {
            $docidFluxo = '';
            $proid = '';
    
            $docidFluxo = wf_cadastrarDocumento(TPDID_FLUXO_SIT_PC_EXEC_FINAN_OBRAS, "Cadastro de documento do Fluxo de prestação de contas Obras PAC - Via acompanhamento", ESDID_SITUACAO_PC_NAO_ENVIADA);
    
            $proid           = $v['proid'];
            $processoObras   = $v['pronumeroprocesso'];
             
    
            if (($proid != '') && ($docidFluxo != '') && ($processoObras != '')) {
            	
                $arrValores[] = "({$proid}, {$docidFluxo}, '{$processoObras}')";
            }
        }
    
        if (count($arrValores) > 0) {
            $sql = "
                INSERT INTO obras2.situacaoprestacaocontasobras
	               (proid_pac, docid, spoprocesso)
                VALUES
                ";
            $sql .= implode(",
                            ", $arrValores);
    
            $this->executar($sql);
            return $this->commit();
        }
    
        return FALSE;
    }
    
    

}
