<?php
/**
 * Classe de mapeamento da entidade par.entidade.
 *
 * @version $Id$
 * @since 2017.01.06
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'par3/classes/model/seguranca/Usuario.class.inc';

/**
 * Entidade_Model_Entidade: sem descricao
 *
 * @package Entidade\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Entidade_Model_Entidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Entidade_Model_Entidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $entcodinep
 * @property string $enturl
 * @property string $entanocenso
 * @property bool $entescolaespecializada
 * @property string $entrazaosocial
 * @property bool $entsede
 * @property string $entorgcod
 * @property string $entnumcelular
 * @property string $entnumdddcelular
 * @property bool $entproep  - default: false
 * @property string $entungcod Código da entidade Gestora.
 * @property string $entunicod Código da unidade.
 * @property string $entsig Sigla da entidade.
 * @property \Datetime(Y-m-d H:i:s) $entdatainclusao Data de inclusão do Registro. Implementado dia 22-07-2008 (basc) - default: now()
 * @property bool $entescolanova Indica se a escola é nova sem CNPJ ou Código do INEP - default: false
 * @property string $entcodent
 * @property string $entcodent
 * @property string $entcodentsup
 * @property int $tpsid Descrição do campo
 * @property int $tplid
 * @property int $tpcid
 * @property int $tpctgid
 * @property string $entnumfax
 * @property string $entnumramalfax
 * @property string $entnumdddfax
 * @property string $entnumcomercial
 * @property string $entnumramalcomercial
 * @property string $entnumdddcomercial
 * @property string $entnumresidencial
 * @property string $entnumdddresidencial
 * @property \Datetime(Y-m-d H:i:s) $entdatafimass
 * @property \Datetime(Y-m-d H:i:s) $entdatainiass
 * @property \Datetime(Y-m-d H:i:s) $entdatanasc
 * @property string $entsexo
 * @property string $entorgaoexpedidor
 * @property string $entnumrg
 * @property string $entstatus
 * @property string $entobs
 * @property string $entnuninsest
 * @property string $entemail
 * @property string $entnome <LABEL>Nome</LABEL>
 * @property string $entnumcpfcnpj
 * @property string $njuid
 * @property int $entid  - default: nextval('par.entidade_entid_seq'::regclass)
 * @property int $entid  - default: nextval('par.entidade_entid_seq'::regclass)
 */
class Par_Model_Entidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.entidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'entcodent',
        'entid',
        'entcodent',
        'entid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'njuid' => array('tabela' => 'entidade.naturezajuridica', 'pk' => 'njuid'),
        'tplid' => array('tabela' => 'entidade.tipolocalizacao', 'pk' => 'tplid'),
        'tpctgid' => array('tabela' => 'entidade.tipocategoriaescolaprivada', 'pk' => 'tpctgid'),
        'tpcid' => array('tabela' => 'entidade.tipoclassificacao', 'pk' => 'tpcid'),
        'tpsid' => array('tabela' => 'entidade.tiposituacao', 'pk' => 'tpsid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'entcodinep' => null,
        'enturl' => null,
        'entanocenso' => null,
        'entescolaespecializada' => null,
        'entrazaosocial' => null,
        'entsede' => null,
        'entorgcod' => null,
        'entnumcelular' => null,
        'entnumdddcelular' => null,
        'entproep' => null,
        'entungcod' => null,
        'entunicod' => null,
        'entsig' => null,
        'entdatainclusao' => null,
        'entescolanova' => null,
        'entcodent' => null,
        'entcodent' => null,
        'entcodentsup' => null,
        'tpsid' => null,
        'tplid' => null,
        'tpcid' => null,
        'tpctgid' => null,
        'entnumfax' => null,
        'entnumramalfax' => null,
        'entnumdddfax' => null,
        'entnumcomercial' => null,
        'entnumramalcomercial' => null,
        'entnumdddcomercial' => null,
        'entnumresidencial' => null,
        'entnumdddresidencial' => null,
        'entdatafimass' => null,
        'entdatainiass' => null,
        'entdatanasc' => null,
        'entsexo' => null,
        'entorgaoexpedidor' => null,
        'entnumrg' => null,
        'entstatus' => null,
        'entobs' => null,
        'entnuninsest' => null,
        'entemail' => null,
        'entnome' => null,
        'entnumcpfcnpj' => null,
        'njuid' => null,
        'entid' => null,
        'entid' => null,
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
            'entcodinep' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'enturl' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'entanocenso' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'entescolaespecializada' => array('allowEmpty' => true),
            'entrazaosocial' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 200))),
            'entsede' => array('allowEmpty' => true),
            'entorgcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'entnumcelular' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'entnumdddcelular' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 3))),
            'entproep' => array('allowEmpty' => true),
            'entungcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'entunicod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'entsig' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'entdatainclusao' => array('allowEmpty' => true),
            'entescolanova' => array('allowEmpty' => true),
            'entcodent' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'entcodent' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'entcodentsup' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'tpsid' => array('allowEmpty' => true,'Digits'),
            'tplid' => array('allowEmpty' => true,'Digits'),
            'tpcid' => array('allowEmpty' => true,'Digits'),
            'tpctgid' => array('allowEmpty' => true,'Digits'),
            'entnumfax' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'entnumramalfax' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'entnumdddfax' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 3))),
            'entnumcomercial' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'entnumramalcomercial' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'entnumdddcomercial' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 3))),
            'entnumresidencial' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'entnumdddresidencial' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 3))),
            'entdatafimass' => array('allowEmpty' => true),
            'entdatainiass' => array('allowEmpty' => true),
            'entdatanasc' => array('allowEmpty' => true),
            'entsexo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'entorgaoexpedidor' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 15))),
            'entnumrg' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 40))),
            'entstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'entobs' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 500))),
            'entnuninsest' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 14))),
            'entemail' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'entnome' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'entnumcpfcnpj' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 18))),
            'njuid' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'entid' => array('Digits'),
            'entid' => array('Digits'),
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

    public function getdadosUnidadePar3( $inuidPar3 )
    {
    	if($inuidPar3 != '') {
        	$sql = " select muncod, estuf from par3.instrumentounidade WHERE inuid = {$inuidPar3}";
        	$result = $this->pegaLinha($sql);
        	return $result;
    	}
    }
    
    public function getDadosEntidade( $inuid, $dutid )
    {
    	$sql = "SELECT
    				*
			    FROM
    				par.entidade ent
			    LEFT JOIN territorios.municipio mun ON mun.muncod = ent.muncod
			    LEFT JOIN territorios.estado est ON est.estuf = ent.estuf
			    WHERE
    				ent.entstatus = 'A' AND ent.inuid = {$inuid} AND ent.dutid = {$dutid}
    			limit 1
    			";


    	$result = $this->pegaLinha($sql);
    	$result = ($result) ? $result : Array();

    	if(count($result) > 0 )
    	{
    		return $result;
    	}
    	else
    	{
    		$sql1 = "SELECT
    		*
    		FROM
    		par.entidade ent
    		LEFT JOIN territorios.municipio mun ON mun.muncod = ent.muncod
    		LEFT JOIN territorios.estado est ON est.estuf = ent.estuf
    		WHERE
    		ent.inuid = {$inuid} AND ent.dutid = {$dutid}
    		ORDER BY entdatainclusao  DESC LIMIT 1
    		";
    		$result = $this->pegaLinha($sql1);
    		$result = ($result) ? $result : Array();
    		
    		if(count($result) > 0 )
    		{
    			return $result;
    		}
    	}
    }
    
    /**
     *
     * @param = Array com os parametros
     * 			inuid,
     * 			dutid,
     * 			arrNovaEntidade,
     * 			arrEntidadeAntiga
     * */
    
    public function atualizaDadosRollDosResponsaveis( $params )
    {
    	/*** CARREGA VARIÁVEI COM OS PARAMETROS*/
    	$inuid 				= $params['inuid'];
    	$dutid 				= $params['dutid'];
    	$arrNovaEntidade 	= $params['arrNovaEntidade'];
    	$arrEntidadeAntiga 	= $params['arrEntidadeAntiga'];
    	
    	
    	$id = $arrNovaEntidade['co_tp_entidade'];
    	 
    	/** INSTANCIA INTRUMENTO UNIDADE */
    	$objInu = new InstrumentoUnidade();
    	$modelSAPEDirigente     = new Par3_Model_SAPEDirigente();
    	
    	$dataIntegracao = $arrNovaEntidade['edadtiniciomandato'];
    	$cpf = $arrNovaEntidade['entcpf'];
    	
    	$inuInfos = $this->pegaLinha( "select muncod, estuf from par.instrumentounidade where inuid = {$inuid}" );
    	
    	$muncod = $inuInfos['muncod'];
    	$estuf = $inuInfos['estuf'];
    	
    	
    	//Trata os valores a serem inseridos
    	$cnpjIRoll             = ($arrNovaEntidade['edacnpj']) 	           ? "'{$arrNovaEntidade['edacnpj']}'" : "NULL";
    	$razaoSocialIRoll      = ($arrNovaEntidade['edarazaosocial'])      ? "'{$arrNovaEntidade['edarazaosocial']}'" : "NULL";
    	$entCpfIRoll           = ($arrNovaEntidade['entcpf'])              ? "'{$arrNovaEntidade['entcpf']}'" : "NULL";
    	$nomeIRoll             = ($arrNovaEntidade['entnome'])             ? "'{$arrNovaEntidade['entnome']}'" : "NULL";
    	$emailIRoll            = ($arrNovaEntidade['entemail'])            ? "'{$arrNovaEntidade['entemail']}'" : "NULL";
    	$muncodIRoll           = ($arrNovaEntidade['muncod'])              ? "'{$arrNovaEntidade['muncod']}'" : "NULL";
    	$estufIRoll            = ($arrNovaEntidade['estuf'])               ? "'{$arrNovaEntidade['estuf']}'" : "NULL";
    	$municipioMandatoIRoll = ($arrNovaEntidade['edamunmandato'])       ? "'{$arrNovaEntidade['edamunmandato']}'" : "NULL";
    	$dataInicioIRoll       = ($arrNovaEntidade['edadtiniciomandato'])  ? "'{$arrNovaEntidade['edadtiniciomandato']}'" : "NULL";
    	$funcaoIRoll           = ($arrNovaEntidade['edafuncao'])           ? "'{$arrNovaEntidade['edafuncao']}'" : "NULL";
    	$foneComercialIRoll    = ($arrNovaEntidade['edafonecomercial'])    ? "'{$arrNovaEntidade['edafonecomercial']}'" : "NULL";
    	$foneCelularIRoll      = ($arrNovaEntidade['edafonecelular'])      ? "'{$arrNovaEntidade['edafonecelular']}'" : "NULL";
    	$ufDirigenteIRoll      = ($arrNovaEntidade['sg_uf_dirigente'])     ? "'{$arrNovaEntidade['sg_uf_dirigente']}'" : "NULL";
    	$enderecoIRoll         = ($arrNovaEntidade['edaendereco'])         ? "'{$arrNovaEntidade['edaendereco']}'" : "NULL";
    	$cepIRoll              = ($arrNovaEntidade['edacep'])              ? "'{$arrNovaEntidade['edacep']}'" : "NULL";
    	$muncipioEnderecoIRoll = ($arrNovaEntidade['edamunicipio'])        ? "'{$arrNovaEntidade['edamunicipio']}'" : "NULL";
    	$bairroEnderecoIRoll   = ($arrNovaEntidade['edabairro'])           ? "'{$arrNovaEntidade['edabairro']}'" : "NULL";
    	
    	$eventstart = $arrNovaEntidade['edadtiniciomandato'];
    	if($eventstart != '') {
    	   $eventstart_time = strtotime($eventstart . ' -1 day');
    	   $dataFimVigenciaAnterior = date("Y-m-d",$eventstart_time);
    	}
    	    	
    	if( ($id == '02')) {  /**P*/
    	    $whereLocal = "AND muncod = '{$muncod}' ";
    	    $tabela = "par.rollresponsaveisprefeitos";
    	    $dataInicio = "rrpdtiniciomandato";
    	    $dataFim = "rrpdtfimmandato";
    	    $idTabelaRoll = "rrpid";

    	    if($muncod != '') {
    	       $idAttRoll = $this->pegaUm("SELECT rrpid FROM par.rollresponsaveisprefeitos WHERE muncod = '{$muncod}' AND {$dataFim} IS NULL ORDER BY rrpdtiniciomandato DESC limit 1;");
    	    }
    	    
    	    $sqlInsert = "	
                INSERT INTO par.rollresponsaveisprefeitos 	        
                ( 
                    rrpcnpj, 
                    rrprazaosocial, 
                    usucpf, 
                    rrpnomeprefeito, 
                    rrpemail, 
                    muncod, 
                    rrpufmandato, 
                    rrpmunmandato,
                    rrpdtiniciomandato,
                    rrpfuncao, 
                    rrpfonecomercial, 
                    rrpfonecelular,
                    uf,
                    rrpendereco, 
                    rrpcep,				
                    rrpmunicipio, 
                    rrpbairro	
                )
    	        VALUES
    	        (
    	           {$cnpjIRoll},
                    {$razaoSocialIRoll},
                    {$entCpfIRoll},
                    {$nomeIRoll},
                    {$emailIRoll},
                    {$muncodIRoll},
                    {$estufIRoll},
                    {$municipioMandatoIRoll},
                    {$dataInicioIRoll},
                    {$funcaoIRoll},
                    {$foneComercialIRoll},
                    {$foneCelularIRoll},
                    {$ufDirigenteIRoll},
                    {$enderecoIRoll},
                    {$cepIRoll},
                    {$muncipioEnderecoIRoll},
                    {$bairroEnderecoIRoll}
                )    	        
        ";
	    
	    
    	}
    	else if($id == '10') { /**SECRETÁRIO*/
    	    $whereLocal = "AND uf = '{$estuf}' ";
    	    $tabela = "par.rollresponsaveissecretarios";
    	    $dataInicio = "rrsdtiniciomandato";
    	    $dataFim = "rrsdtfimmandato";
    	    $idTabelaRoll = "rrsid";

    	   if($estuf != '') {
    	       $idAttRoll = $this->pegaUm("SELECT rrsid FROM par.rollresponsaveissecretarios  WHERE rrsufmandato = '{$estuf}' AND {$dataFim} IS NULL  ORDER BY rrsdtiniciomandato DESC limit 1;");
    	    }

    	    $sqlInsert = "
        	    INSERT INTO par.rollresponsaveissecretarios
        	    (
            	   rrscnpj,
                    rrsrazaosocial,
                    usucpf,
                    rrsnomesecretario,
                    rrsemail,
                    muncod,
                    rrsufmandato,
                    rrsmunmandato,
                    rrsdtiniciomandato ,
                    rrsfuncao,
                    rrsfonecomercial,
                    rrsfonecelular,
                    uf,
                    rrsendereco,
                    rrscep,
                    rrsmunicipio,
                    rrsbairro 
        	    )
        	    VALUES
        	    (
            	    {$cnpjIRoll},
            	    {$razaoSocialIRoll},
            	    {$entCpfIRoll},
            	    {$nomeIRoll},
            	    {$emailIRoll},
            	    {$muncodIRoll},
            	    {$estufIRoll},
            	    {$municipioMandatoIRoll},
            	    {$dataInicioIRoll},
            	    {$funcaoIRoll},
            	    {$foneComercialIRoll},
            	    {$foneCelularIRoll},
            	    {$ufDirigenteIRoll},
            	    {$enderecoIRoll},
            	    {$cepIRoll},
            	    {$muncipioEnderecoIRoll},
            	    {$bairroEnderecoIRoll}
        	    )
    	    ";
    	}
    	
    	//Verificar se já tem na tabela, se já houver, e for atual retorna true
    	if( ($idTabelaRoll != '') && ( $dataInicio != '' ) &&  ( $dataFim != '' ) && ( $tabela != '' ) && ( $cpf != '') && ( $whereLocal != '') ) {
           //$dadosRoll = $this->pegaLinha("SELECT {$idTabelaRoll}, {$dataInicio},{$dataFim}  FROM {$tabela} WHERE usucpf = '' ");// @TODO TESTES
    	   $dadosRoll = $this->pegaLinha("SELECT {$idTabelaRoll}, {$dataInicio},{$dataFim}  FROM {$tabela} WHERE usucpf = '{$cpf}' {$whereLocal} ORDER BY {$dataInicio} desc");
        }
    	
    	$idRoll            = $dadosRoll[$idTabelaRoll];
    	$dataInicioRoll    = $dadosRoll[$dataInicio];
    	$dataFimRoll       = $dadosRoll[$dataFim];
    	
    	if(($idRoll != '') && ( $dataInicioRoll >= $dataIntegracao )) {
    	    return true;
    	} elseif( $idRoll != '' ) { // Registro deste mesmo representante já existe
    	    
    	   // A data final deve ser considerada, caso possua a data final, e não seja o último insere normalmente ID = 6 - INSERT ROLL DOS RESPONSAVEIS possuia registro como historico e verifica se é necesário inserir a data final para o anterior
    	   // A segunda verificação é para saber se não é o atual, pois caso seja não é necessário
    	   if( ($dataFimRoll != '') && ($idAttRoll != $idRoll) ) {

    	       // Insere o novo registro
    	       if( $sqlInsert != '')
    	       {
    	           $inseriu = $this->executar($sqlInsert);
    	       }
               // Caso a inserção tenha sido executada com sucesso    	       
    	       if($inseriu) {
    	           $idLogRotina = '6';
    	           $addLog = $modelSAPEDirigente->insereLogSucesso($arrNovaEntidade, $idLogRotina);
    	           //Coloca a data final para anterior caso esteja vazio 
    	           if(($dataFimVigenciaAnterior != '') && ($tabela != '') &&  ($dataFim != '') && ($idTabelaRoll != '') && ($idAttRoll != '') && ($addLog)) {
    	               
    	               $dataFinal = $this->executar("UPDATE {$tabela}  SET {$dataFim} = '{$dataFimVigenciaAnterior}' WHERE {$idTabelaRoll} = {$idAttRoll} ");
    	               
    	               if($dataFinal){
    	                   
    	                   $idLogRotina = '7';
    	                   $modelSAPEDirigente->insereLogSucesso($arrNovaEntidade, $idLogRotina);
    	                   
    	                   return true;
    	               }
    	           } 
    	           return true;
    	           
    	       }
    	   } else {
    	       // Como ele já está com a data fim vazia significa que ele já está como atual então ele apenas retorna verdadeiro pois não precisa atualizar
    	       return true;
    	   }
    	}else {
        	//  Insere o novo registro log 8 
    	       if( $sqlInsert != '')
    	       {
    	           $inseriu = $this->executar($sqlInsert);
    	           
    	       }
    	    
    	       if($inseriu) {
    	          $idLogRotina = '8';
    	          $addLog = $modelSAPEDirigente->insereLogSucesso($arrNovaEntidade, $idLogRotina);
    	           
    	           //Coloca a data final para anterior caso esteja vazio 
    	           if(($dataFimVigenciaAnterior != '') && ($tabela != '') &&  ($dataFim != '') && ($idTabelaRoll != '') && ($idAttRoll != '') && ($addLog)) {
    	           
    	               $dataFinal = $this->executar("UPDATE {$tabela}  SET {$dataFim} = '{$dataFimVigenciaAnterior}' WHERE {$idTabelaRoll} = {$idAttRoll} ");// @TODO TESTES
    	               if($dataFinal){
    	                   
    	                   $idLogRotina = '7';
    	                   $modelSAPEDirigente->insereLogSucesso($arrNovaEntidade, $idLogRotina);
    	                   return true;
    	               } else {
    	                   return false;
    	               }
    	           } 
    	           return true;    
    	       }
    	}
    	return true;
    }
    /**
     *
     * @param = Array com os parametros
     * 			inuid,
     * 			dutid,
     * 			arrNovaEntidade,
     * 			arrEntidadeAntiga
     * */
    
    public function attDadosDirigentesEntidadeRotina( $params )
    {
    	/*** CARREGA VARIÁVEI COM OS PARAMETROS*/
    	$inuid 				= $params['inuid'];
    	$dutid 				= $params['dutid'];
    	$arrNovaEntidade 	= $params['arrNovaEntidade'];
    	$arrEntidadeAntiga 	= $params['arrEntidadeAntiga'];
    	
    	/*** ENTID */
    	$entid = $arrEntidadeAntiga['entid'];
    	
    	/*** VERIFICA A EXISTENCIA DO CPF A SER INSERIDO, POIS É INDISPENSÁVEL PARA A FUNCIONALIDADE*/
    	if( $arrNovaEntidade['entcpf'] )
    	{
    		$arrNovaEntidade['entnumdddcomercial'] = trim($arrNovaEntidade['entnumdddcomercial']);
    		$arrNovaEntidade['entnumcomercial'] = trim($arrNovaEntidade['entnumcomercial']);
    
    
    		/*** TRATA VARIAVEIS */
    		$arrCampos['entnome']                     	= $arrNovaEntidade['entnome'];
    		$arrCampos['entemail']                    	= $arrNovaEntidade['entemail'];
    		$arrCampos['entnumrg']                     	= $arrNovaEntidade['entrg'];
    		$arrCampos['entorgaoexpedidor']             = $arrNovaEntidade['entorgexpedidor'];
    		$arrCampos['entdatanasc']             		= $arrNovaEntidade['entdtnascimento'];
    		$arrCampos['entnumdddcomercial']          	= $arrNovaEntidade['entnumdddcomercial'];
    		$arrCampos['entnumcomercial']             	= $arrNovaEntidade['entnumcomercial'];
    		$arrCampos['muncod'] 					  	= $arrNovaEntidade['co_municipio_ibge_dirigente'] ;
    		$arrCampos['estuf']             		  	= $arrNovaEntidade['sg_uf_dirigente'];
    		$arrCampos['endcep']                      	= $arrNovaEntidade['endcep'];
    		$arrCampos['endlog']               			= $arrNovaEntidade['endlogradouro'];
    		$arrCampos['endcom']              			= $arrNovaEntidade['endcomplemento'];
    		$arrCampos['endbai']                   	    = $arrNovaEntidade['endbairro'];
    		$arrCampos['endnum']                  		= $arrNovaEntidade['endnumero'];
    		$arrCampos['entsexo']						= $arrNovaEntidade['entsexo'];
            
    		if( $arrNovaEntidade['entnumdddcelular'] != "" &&  $arrNovaEntidade['entnumcelular'] != "" ) {
    			$arrCampos['entnumdddcelular']   = $arrNovaEntidade['entnumdddcelular'];
    			$arrCampos['entnumcelular']		 = $arrNovaEntidade['entnumcelular'];
    		}
    		
    		
    		$arrColunas = array();
    		foreach($arrCampos as $nome => $valor)
    		{
    			if( ( $valor != '') && ($valor != NULL) && ($valor != '0' ) )
    			{
    				$arrColunas[] =  " {$nome} = '{$valor}' ";
    			}
    		}
    		
    		if( (count($arrColunas) > 0) && ($entid) )
    		{
    			$colunas = implode($arrColunas, ',');
    
    			$sqlUpdate = " UPDATE par.entidade SET {$colunas}  WHERE entid = {$entid} ";
    			$this->executar($sqlUpdate);
    		}
    	}
    	
    	if( $entid )
    	{
    		return true;
    	}
    	else 
    	{
    		return false;
    	}
    	 
    }
    
    
    /**
     *
     * @param = Array com os parametros
     * 			inuid,
     * 			dutid,
     * 			arrNovaEntidade,
     * 			arrEntidadeAntiga
     * */
    
    public function inativaEntidadesAtuais( $params )
    {
    	/** CARREGA VARIÁVEIS */
    	$inuid = $params['inuid'];
    	$dutid = $params['dutid'];
    	
    	/**CARREGA SQL*/
    	$sql = "
    		UPDATE
	    		par.entidade
	    	SET
		    	entstatus = 'I',
		    	entdataalteracao = 'NOW()',
		    	entcpfalteracao = '00000000191'
	    	WHERE
		    	entstatus = 'A'
		    	AND inuid = {$inuid}
		    	AND dutid = {$dutid}";
    	
    	return $this->executar($sql);
    }
    
    /**
     *
     * @param = Array com os parametros
     * 			inuid,
     * 			dutid,
     * 			arrNovaEntidade,
     * 			arrEntidadeAntiga
     * */
    
    public function insereNovoDirigentesEntidadeRotina( $params )
    {
    	
    	/*** CARREGA VARIÁVEI COM OS PARAMETROS*/
    	$inuid 				= $params['inuid'];
    	$dutid 				= $params['dutid'];
    	$arrNovaEntidade 	= $params['arrNovaEntidade'];
    	$arrEntidadeAntiga 	= $params['arrEntidadeAntiga'];
    	
    	/*** VERIFICA A EXISTENCIA DO CPF A SER INSERIDO, POIS É INDISPENSÁVEL PARA A FUNCIONALIDADE*/
    	if( $arrNovaEntidade['entcpf'] )
    	{
    		/*** INATIVA  A ANTIGA */
    		$params['inuid'] = $inuid;
    		$params['dutid'] = $dutid;
    		$retornoInativacao = $this->inativaEntidadesAtuais($params);
    		if($retornoInativacao)
    		{

    			$arrNovaEntidade['entnumdddcomercial'] = trim($arrNovaEntidade['entnumdddcomercial']);
    			$arrNovaEntidade['entnumcomercial'] = trim($arrNovaEntidade['entnumcomercial']);
    			/*** TRATA VARIAVEIS*/
    			#$entSexo	= "'" . strtoupper($arrNovaEntidade['entsexo']) . "'";
    			$entSexo	= ($arrNovaEntidade['entsexo']) ? "'".strtoupper($arrNovaEntidade['entsexo'])."'" : 'NULL';
    			$cpf		= ($arrNovaEntidade['entcpf']) ? "'{$arrNovaEntidade['entcpf']}'" : 'NULL';
    			$nome		= ($arrNovaEntidade['entnome']) ? "'{$arrNovaEntidade['entnome']}'" : 'NULL';
    			$apelido    = explode(' ', $arrNovaEntidade['entnome']);
    			$apelido    = $apelido[0] ? "'{$apelido[0]}'" : 'NULL';
    			$email 		= ($arrNovaEntidade['entemail']) ? "'{$arrNovaEntidade['entemail']}'" : "''";
    			$rg 		= ($arrNovaEntidade['entrg']) ? "'{$arrNovaEntidade['entrg']}'" : 'NULL';
    			$orgExp		= ($arrNovaEntidade['entorgexpedidor']) ? "'{$arrNovaEntidade['entorgexpedidor']}'" : 'NULL';
    			$orgExp		= ($arrNovaEntidade['entorgexpedidor']) ? "'{$arrNovaEntidade['entorgexpedidor']}'" : 'NULL';
    			$dtNasc		= ($arrNovaEntidade['entdtnascimento']) ? "'{$arrNovaEntidade['entdtnascimento']}'" : 'NULL';
    			$dddComerc	= ($arrNovaEntidade['entnumdddcomercial']) ? "'{$arrNovaEntidade['entnumdddcomercial']}'" : 'NULL';
    			$numComerc	= ($arrNovaEntidade['entnumcomercial']) ? "'{$arrNovaEntidade['entnumcomercial']}'" : 'NULL';
    			$muncod		= ($arrNovaEntidade['co_municipio_ibge_dirigente']) ? "'{$arrNovaEntidade['co_municipio_ibge_dirigente']}'" : 'NULL';
    			$estuf		= ($arrNovaEntidade['sg_uf_dirigente']) ? "'{$arrNovaEntidade['sg_uf_dirigente']}'" : 'NULL';
    			$cep		= ($arrNovaEntidade['endcep']) ? "'{$arrNovaEntidade['endcep']}'" : 'NULL';
    			$logradouro = ($arrNovaEntidade['endlogradouro']) ? "'{$arrNovaEntidade['endlogradouro']}'" : 'NULL';
    			$complem	= ($arrNovaEntidade['endcomplemento']) ? "'{$arrNovaEntidade['endcomplemento']}'" : 'NULL';
    			$bairro		= ($arrNovaEntidade['endbairro']) ? "'{$arrNovaEntidade['endbairro']}'" : 'NULL';
    			$numero		= ($arrNovaEntidade['endnumero']) ? "'{$arrNovaEntidade['endnumero']}'" : 'NULL';
    			
    			$entnumdddcelular		= ($arrNovaEntidade['entnumdddcelular']) ? "'{$arrNovaEntidade['entnumdddcelular']}'" : 'NULL';
    			$entnumcelular		    = ($arrNovaEntidade['entnumcelular']) ? "'{$arrNovaEntidade['entnumcelular']}'" : 'NULL';
    			
    			
    			/*** INSERE A NOVA*/
    			$insert = "INSERT INTO par.entidade
    			(
	    			inuid,
	    			dutid,
	    			entnumcpfcnpj,
	    			entnome,
	    			entemail,
	    			entstatus,
	    			entnumrg,
	    			entorgaoexpedidor,
	    			entsexo,
	    			entdatanasc,
	    			entnumdddcomercial,
	    			entnumcomercial,
	    			entdatainclusao,
	    			usucpf,
	    			muncod,
	    			estuf,
	    			endcep,
	    			endlog,
	    			endcom,
	    			endbai,
	    			endnum,
	    			entnumdddcelular,
	    			entnumcelular
    			)
    			VALUES (
	    			{$inuid},
	    			{$dutid},
	    			{$cpf},
	    			{$nome},
	    			{$email},
	    			'A',
	    			{$rg},
	    			{$orgExp},
	    			{$entSexo},
	    			{$dtNasc},
	    			{$dddComerc},
	    			{$numComerc},
	    			'NOW()',
	    			'00000000191',
	    			{$muncod},
	    			{$estuf},
	    			{$cep},
	    			{$logradouro},
	    			{$complem},
	    			{$bairro},
	    			{$numero},
	    			{$entnumdddcelular},
	    			{$entnumcelular}	
    			)
    			RETURNING entid
    			";
    			
    			$entid = $this->pegaUm($insert);
    			return ($entid) ? TRUE : FALSE;
    		}
    		else
    		{
    			return false;
    		}
    		
    	}
    	else
    	{
    		return false;
    	}
    }	
    			
     

    public function verificaUltimaVezRotinaPrefeitos()
    {
    	$sql = "SELECT agsultexecucao::date FROM seguranca.agendamentoscripts WHERE agsfile ilike 'atualiza_prefeitos_dirigentes.php'";
    	return $this->pegaUm($sql);
    }

}
