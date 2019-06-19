<?php
/**
 * Classe de mapeamento da entidade par3.orientacoesprograma.
 *
 * @version $Id$
 * @since 2016.09.27
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Orientacoesprograma: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Orientacoesprograma($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Orientacoesprograma($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $prgid 
 * @property string $orpdescricao 
 * @property string $orptexto 
 * @property int $orpid  - default: nextval('par3.orientacoesprograma_orpid_seq'::regclass)
 */
class Par3_Model_Orientacoesprograma extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.orientacoesprograma';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'orpid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
    );

    /**
     * construtor
     */
    public function Par3_Model_Orientacoesprograma(){
    	
    }
    /**
     * construtor
     */
    public function getTabelaRestricoes($arrRestricoes)
    {
    	
    	$cabecalho 	= array('Restrição Impeditiva');
    	$dados 		= $arrRestricoes;
    	$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    	$listagem->setCabecalho($cabecalho);
    	$listagem->setDados($dados);
    	$listagem->turnOffForm();
    	$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    		
    }
    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'prgid' => null,
        'orpdescricao' => null,
        'orptexto' => null,
        'orpid' => null,
    );

    /*
     * 
     * */
    
    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'prgid' => array('allowEmpty' => true,'Digits'),
            'orpdescricao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'orptexto' => array('allowEmpty' => true),
            'orpid' => array('Digits'),
        );
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getOrpDescricaoByID($orpid)
    {
        $sql = "
	    	SELECT 
	    		orpdescricao
	    	FROM
	    		{$this->stNomeTabela}
	    	WHERE
	    		orpid = {$orpid}
    	";
    	
    	return $this->pegaUm($sql);
    }

    public function getTextoOrientacaoByID($orpid)
    {
    	$sql = "
	    	SELECT 
	    		orptexto
	    	FROM
	    		{$this->stNomeTabela}
	    	WHERE
	    		orpid = {$orpid}
    	";
    	
    	return $this->pegaUm($sql);
    }
    
    /**
     * retornar 
     * */
    public function getOrientacaoByID($orpid, $nomeAba = NULL)
    {
    	$textoOrientacao = $this->getTextoOrientacaoByID($orpid);
    	
    	//echo 'asçdlfkjaçlsdjfçla ksdjfçlaksj dçlfaskj fçlaksdjf çlaskdjf çlkasjdçflk jasdçklfjsadklfj' . $orpid;
    	include APPRAIZ . 'par3/modulos/principal/adesao/termos/orientacoesprograma.inc';
    	return true;
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
