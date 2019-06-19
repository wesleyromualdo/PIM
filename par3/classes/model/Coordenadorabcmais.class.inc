<?php
/**
 * Classe de mapeamento da entidade par3.coordenadorabcmais.
 *
 * @version $Id$
 * @since 2016.09.26
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Coordenadorabcmais: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Coordenadorabcmais($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Coordenadorabcmais($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $inuid 
 * @property string $camtelefone 
 * @property string $camemail 
 * @property string $camnome 
 * @property string $camcpf 
 * @property int $camid  - default: nextval('par3.coordenadorabcmais_camid_seq'::regclass)
 */
class Par3_Model_Coordenadorabcmais extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.coordenadorabcmais';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'camid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'inuid' => null,
        'adpid' => null,
        'camtelefone' => null,
        'camemail' => null,
        'camnome' => null,
        'camcpf' => null,
        'camid' => null,
        'camstatus' => null,
    );

    
    function Par3_Model_Coordenadorabcmais()
    {
    	
    }
    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'inuid' => array('allowEmpty' => true,'Digits'),
            'camtelefone' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'camemail' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'camnome' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'camcpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'camstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'camid' => array('Digits'),
        );
    }
    
    /**
     * Valida o email do coordenador
     *
     * @param $email
     * @return boolean
     */
    public function validaEmailCoordenador($email)
    {
    	$er = "/^(([0-9a-zA-Z]+[-._+&])*[0-9a-zA-Z]+@([-0-9a-zA-Z]+[.])+[a-zA-Z]{2,6}){0,1}$/";
    	
    	if (preg_match($er, $email)){
    		return true;
    	} else {
    		return false;
    	}
    }
    
    public function deletaGeralPorUnidade($adpid)
    {
        if($adpid){
    		$this->executar("DELETE FROM par3.coordenadorabcmais WHERE adpid = {$adpid}");
    		$this->commit();
    	}
    }
    /**
     * Valida o email do coordenador
     *
     * @param $email
     * @return boolean
     */
    public function retornaListagemCoordenador($adpid)
    {
    	$sqlDados =  "
    			SELECT 
					substr(camcpf, 1, 3) || '.' ||
			        substr(camcpf, 4, 3) || '.' ||
			        substr(camcpf, 7, 3) || '-' ||
                    substr(camcpf, 10) as cpf ,
					camnome as nome,
					camemail as email,
					'(' || substr(camtelefone, 1, 2) || ')' || substr(camtelefone, 3, 11)  as telefone
				FROM 
					par3.coordenadorabcmais
				WHERE
					adpid = {$adpid} AND 
    				camstatus = 'A'";
    	
    	$cabecalho 	= array('CPF','Nome','E-mail','Telefone');
    	$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    	$listagem->setCabecalho($cabecalho);
    	$listagem->setQuery($sqlDados);
    	$listagem->turnOffForm();
    	$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    	return ;
    }

    /***
     * 
     */
    public function existeCoordenador($inuid, $adpid)
    {
        if($adpid){
        	$sql = "SELECT 
        				camid
        			FROM 
        				par3.coordenadorabcmais
        			WHERE
        				inuid = {$inuid} AND
                        adpid = $adpid AND
        				camcpf IS NOT NULL AND
        				camnome IS NOT NULL AND
        				camemail IS NOT NULL AND
        				camtelefone IS NOT NULL AND
        				camstatus = 'A'";
        	
        	$result = $this->pegaUm($sql);
        }
    	return ($result != '' )? true : false;
    }
    
    
    /**
     * 
     * Retorna o coordenador da unidade pelo INUID
     * @param $email
     * @return boolean
     */
    public function recuperarCoordenadorPorInuid($inuid, $adpid)
    {
        if($adpid){
        	$sql = "SELECT 
        				camid,
        				camcpf,
        				camnome,
        				camemail,
        				camtelefone,
        				camstatus,
        				inuid
        			FROM 
        				par3.coordenadorabcmais
        			WHERE
        				inuid = {$inuid} AND
                        adpid = $adpid AND
        				camstatus = 'A'";
        
        	return $this->pegaLinha($sql);
        }
    }
    /**
     *
     * inativa o coordenador pelo camid
     * @param $email
     * @return boolean
     */
    public function inativaCoordenador($camid = null)
    {
    	if($camid)
    	{
	    	$sql = "   		
				UPDATE par3.coordenadorabcmais SET camstatus = 'I' WHERE camid = {$camid}
	    	";
	    
	    	return $this->executar($sql);
    	}
    	else
    	{
    		return false;
    	}
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

}
