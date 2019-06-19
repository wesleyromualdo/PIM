<?php
/**
 * Classe de mapeamento da entidade par3.programa.
 *
 * @version $Id$
 * @since 2017.06.26
 */

/**
 * Par3_Model_Programa: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Programa($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Programa($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $prgid  - default: nextval('par3.programa_prgid_seq'::regclass)
 * @property string $prgdsc
 * @property string $prgstatus  - default: 'A'::bpchar
 * @property int $pgoid
 * @property int $pgoid
 * @property string $prglink
 * @property int $prgtipoprograma
 * @property string $prgabreviatura
 * @property string $prgarquivo  - default: 'padrao.inc'::character varying
 * @property string $prgresumo
 * @property bool $prgatd_extraordinario
 * @property string $prgsituacao
 * @property \Datetime(Y-m-d H:i:s) $prgdtinclusao
 * @property string $prgcpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $prgdtinativacao
 * @property string $prgcpfinativacao
 * @property \Datetime(Y-m-d H:i:s) $prgperiodoinicio
 * @property \Datetime(Y-m-d H:i:s) $prgperiodofim
 * @property string $prgesfera
 * @property int $arqid
 * @property string $prganoreferencia
 */
class Par3_Model_Programa extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.programa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'prgid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'pgoid' => array('tabela' => 'par3.programaorigem', 'pk' => 'pgoid'),
        'pgoid' => array('tabela' => 'par.programaorigem', 'pk' => 'pgoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'prgid' => null,
        'prgdsc' => null,
        'prgstatus' => null,
        'pgoid' => null,
        'pgoid' => null,
        'prglink' => null,
        'prgtipoprograma' => null,
        'prgabreviatura' => null,
        'prgarquivo' => null,
        'prgresumo' => null,
        'prgatd_extraordinario' => null,
        'prgsituacao' => null,
        'prgdtinclusao' => null,
        'prgcpfinclusao' => null,
        'prgdtinativacao' => null,
        'prgcpfinativacao' => null,
        'prgperiodoinicio' => null,
        'prgperiodofim' => null,
        'prgesfera' => null,
        'arqid' => null,
        'prganoreferencia' => null,
    );

    public function pegarSQLSelectCombo($prgid = array(), $opcoes = array())
    {
    	$prgid = is_array($prgid) ? implode(',',$prgid):$prgid;
    	$where = "WHERE prgstatus = 'A' AND prgsituacao = 'A'";
    	$where .= $prgid ? " AND prgid in({$prgid})":'';
		
    	$selectDescricao = "prgdsc as descricao";
    	
    	if(array_key_exists('incluirSigla', $opcoes) AND $opcoes['incluirSigla'] == true){
    		$selectDescricao = "(CASE WHEN prgabreviatura IS NULL THEN ' '  ELSE prgabreviatura || ' - ' END) || prgdsc as descricao";
    	}
    	
    	$select = "
    		SELECT
    			prgid as codigo,
    			$selectDescricao 
    	";
    	
    	$sql = "$select FROM {$this->stNomeTabela} $where";
    	return $sql;
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
            'prgid' => array('Digits'),
            'prgdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'prgstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'pgoid' => array('allowEmpty' => true,'Digits'),
            'pgoid' => array('allowEmpty' => true,'Digits'),
            'prglink' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'prgtipoprograma' => array('allowEmpty' => true,'Digits'),
            'prgabreviatura' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'prgarquivo' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'prgresumo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'prgatd_extraordinario' => array('allowEmpty' => true),
            'prgsituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'prgdtinclusao' => array('allowEmpty' => true),
            'prgcpfinclusao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'prgdtinativacao' => array('allowEmpty' => true),
            'prgcpfinativacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'prgperiodoinicio' => array('allowEmpty' => true),
            'prgperiodofim' => array('allowEmpty' => true),
            'prgesfera' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'arqid' => array('allowEmpty' => true,'Digits'),
            'prganoreferencia' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
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
     * @param $prgid
     * @return array|bool|mixed|NULL
     * @throws Exception
     * @description Este método pega todos os dados do programa pelo id
     */
    public function fetchAllProgramaById($prgid){
        $sql = "SELECT * FROM par3.programa pr inner join par3.proadesao as ad on ad.prgid = pr.prgid WHERE pr.prgid = {$prgid};";
        try{
            $result = $this->pegaLinha($sql);
            return $result;
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }

}
