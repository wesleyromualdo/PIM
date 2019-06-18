<?php
/**
 * Classe de mapeamento da entidade par.usuarioresponsabilidade.
 *
 * @version $Id$
 * @since 2017.02.06
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
 
/**
 * Par_Model_Usuarioresponsabilidade: Tabela contendo a relação dos usuários com acesso ao sistema por perfil e unidade.
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par_Model_Usuarioresponsabilidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par_Model_Usuarioresponsabilidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iueid Chave primaria da tabela par.instrumentounidadeentidade
 * @property int $entid 
 * @property int $prgid 
 * @property string $muncod 
 * @property string $estuf 
 * @property \Datetime(Y-m-d H:i:s) $rpudata_inc Data de inclusão do registro. - default: '2008-09-02 17:46:42.244463'::timestamp without time zone
 * @property string $rpustatus Status do registro (A - Ativo, I - Inativo). - default: 'A'::bpchar
 * @property string $usucpf CPF do usuário.
 * @property int $pflcod 
 * @property int $rpuid Identificador único da tabela usuarioresponsabilidade. - default: nextval('par.usuarioresponsabilidade_backup_rpuid_seq'::regclass)
 */
class Par_Model_Usuarioresponsabilidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.usuarioresponsabilidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'rpuid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iueid' => null,
        'entid' => null,
        'prgid' => null,
        'muncod' => null,
        'estuf' => null,
        'rpudata_inc' => null,
        'rpustatus' => null,
        'usucpf' => null,
        'pflcod' => null,
        'rpuid' => null,
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
            'iueid' => array('allowEmpty' => true,'Digits'),
            'entid' => array('allowEmpty' => true,'Digits'),
            'prgid' => array('allowEmpty' => true,'Digits'),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'rpudata_inc' => array('allowEmpty' => true),
            'rpustatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'pflcod' => array('allowEmpty' => true,'Digits'),
            'rpuid' => array('Digits'),
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
     * Inativa o UsuarioResponsabilidade 
     * 
     */
    public function inativaUsuarioResponsabilidadeRotina($arrDados)
    {
    	/** CARREGA AS VARIÁVEIS NECESSÁRIAS*/
    	$codCargo = $arrDados['co_tp_entidade'];
    	/** PEGA O ID DO PERFIL DE ACORDO COM O CÓDIGO do CARGO */
    	$arrPerfis = array(
    			'02' 	=> PAR_PERFIL_PREFEITO,
    			'10'	=> PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,
    			'28'	=> PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO
    	);
    	
    	$idPerfil = $arrPerfis[$codCargo];
    	
    	// Busca o inuid do par
    	if( ($codCargo == '02') || ($codCargo == '28') )
    	{
    		$muncod = $arrDados['muncod'];
    		$sql = "UPDATE par.usuarioresponsabilidade SET rpustatus = 'I' WHERE muncod = '{$muncod}' and pflcod = {$idPerfil} ";
    		
    	}
    	else if($codCargo == '10')
    	{
    		$estuf = $arrDados['estuf'];
    		$sql = "UPDATE par.usuarioresponsabilidade SET rpustatus = 'I' WHERE estuf = '{$estuf}' and pflcod = {$idPerfil}";
    		
    	}
    	
    	/** EXECUTA */
    	if( $this->executar($sql) )
    	{
    		return true;
    	}
    	
    }

    /**
     * Inativa o UsuarioResponsabilidade
     *
     */
    public function insereUsuarioResponsabilidadeRotina($arrDados)
    {
    	/** CARREGA AS VARIÁVEIS NECESSÁRIAS*/
    	$codCargo 	= $arrDados['co_tp_entidade'];
    	$usucpf		= $arrDados['entcpf'];
    	/** PEGA O ID DO PERFIL DE ACORDO COM O CÓDIGO do CARGO */
    	$arrPerfis = array(
    			'02' 	=> PAR_PERFIL_PREFEITO,
    			'10'	=> PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,
    			'28'	=> PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO
    	);
    	/** ID DO PERFIL*/
    	$idPerfil = $arrPerfis[$codCargo];
    	
    	if( ($codCargo == '02') || ($codCargo == '28') )
    	{
    		$muncod = $arrDados['muncod'];
    		$sql = "INSERT INTO par.usuarioresponsabilidade
						(pflcod, usucpf, rpustatus, rpudata_inc, muncod )
					VALUES
						({$idPerfil}, '{$usucpf}', 'A', now(), '{$muncod}')";
    
    	}
    	else if($codCargo == '10')
    	{
    		$estuf = $arrDados['estuf'];
    		$sql = "INSERT INTO par.usuarioresponsabilidade
						(pflcod, usucpf, rpustatus, rpudata_inc, estuf )
					VALUES
						({$idPerfil}, '{$usucpf}', 'A', now(), '{$estuf}')";
    
    	}
    	
    	/** EXECUTA */
    	if( $this->executar($sql) )
    	{
    		return true;
    	}
    	 
    }
        
    public function acessoRapidoPegarPerfil($usucpf=null, $sisid=null)
    {
        $usucpf                     = ($usucpf ? $usucpf : $_SESSION['usucpf']);
        $sisid                      = ($sisid ? $sisid : $_SESSION['sisid']);
//         $perfilComResonsabilidade   = ($perfilComResonsabilidade ? $perfilComResonsabilidade : false);
        
//         if ($perfilComResonsabilidade) {
//             $coluna = ", tp.tprcod";
//             $join = "join par.tprperfil tp on tp.pflcod = pu.pflcod";
//         }
        
        $sql = "select
                	distinct
                	pu.pflcod,
                    tp.tprcod,
                    ur.entid,
                	ur.estuf,
                	ur.muncod,
                	ur.iueid,
                	ur.prgid
                from
                    seguranca.perfil p
                join seguranca.perfilusuario pu on pu.pflcod = p.pflcod
                left join par.tprperfil tp on tp.pflcod = pu.pflcod
                left join par.usuarioresponsabilidade ur on ur.pflcod = pu.pflcod and 
                									        ur.usucpf = pu.usucpf and 
                									        ur.rpustatus = 'A'
                where
                    p.sisid = {$sisid} and
                	pu.usucpf = '{$usucpf}'";
        $arDados = $this->carregar($sql);
        $arDados = ($arDados ? $arDados : array());
        
        $perfil                      = array();
        $pefilComresponsabilidade   = array();
//         $responsabilidade           = array();
        $responsabilidade           = array('estuf' => array(), 'entid' => array(), 'muncod' => array(), 'iueid' => array(), 'prgid' => array());
        foreach ($arDados as $d){
            if (array_search($d['pflcod'], $perfil) === false) {
                $perfil[] = $d['pflcod'];
            }
            if (array_search($d['pflcod'], $pefilComresponsabilidade) === false && $d['tprcod']) {
                $pefilComresponsabilidade[] = $d['pflcod'];
            }
            
            if ($d['estuf'] && array_search($d['estuf'], $responsabilidade['estuf']) === false) {
                $responsabilidade['estuf'][] = $d['estuf'];
            }
            if ($d['entid'] && array_search($d['entid'], $responsabilidade['entid']) === false) {
                $responsabilidade['entid'][] = $d['entid'];
            }
            if ($d['muncod'] && array_search($d['muncod'], $responsabilidade['muncod']) === false) {
                $responsabilidade['muncod'][] = $d['muncod'];
            }
            if ($d['iueid'] && array_search($d['iueid'], $responsabilidade['iueid']) === false) {
                $responsabilidade['iueid'][] = $d['iueid'];
            }
            if ($d['prgid'] && array_search($d['prgid'], $responsabilidade['prgid']) === false) {
                $responsabilidade['prgid'][] = $d['prgid'];
            }
        }
        
        $_SESSION['PAR']['acessorapido'][$sisid][$usucpf]['pflcod'] = $perfil;
        $_SESSION['PAR']['acessorapido'][$sisid][$usucpf]['pflcod_responsabilidade'] = $pefilComresponsabilidade;
        $_SESSION['PAR']['acessorapido'][$sisid][$usucpf]['responsabilidade'] = $responsabilidade;
        
        return $perfil;
    }
}
